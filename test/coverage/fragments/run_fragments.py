#!/usr/bin/env python3
"""Module fragments and program-map assembly (sv0cov CV-205; SPEC 13.1.1,
16.3.3, COV-MAP-013, AC-091, BL-097).

sv0c compiles a project as one unit and plans one fragment per source file:
fragments in source-path order, each owning one contiguous slice of the
program counter space. SV0_COVERAGE_LINK_ORDER (a test hook in
megaTU-main.sv0) links the project's files in a given order instead of the
sorted listing, so this script can show the map does not depend on it:

1. proj/ (five modules: two plan no counter, one in the middle of the path
   order and one last): in all 120 link orders, the native compiler and the
   native VM emitter write byte-identical maps; the map passes sv0cov's
   validator (fragment IDs recomputed, slices gap-free and in bounds) and
   has the expected fragment table, with the middle zero-length fragment at
   the next positive base and the last at program_counter_count;
2. the sv0cov `project` fixture (three modules, two sharing a basename): in
   all 6 link orders both compilers write its hand-reviewed expected-map.json;
3. instrumented builds of proj/ linked in sorted and reversed order, run on
   native C and the VM, publish profiles whose counts are equal, point for
   point, and equal the hand-derived counts below;
4. a link order that is not a permutation of the files fails the build;
5. CV-206 (COV-C-005): the generated C registers one module per fragment
   (five, two of them empty) and runs under the real sv0cov runtime; each
   tampered copy (a module naming another map, overlapping or gapped
   slices, an empty fragment off a slice boundary, a module registered
   twice, a positive module missing from the aggregator) exits 1 with
   COV1015 before user code and publishes nothing.

    python3 sv0c/test/coverage/fragments/run_fragments.py
"""

from __future__ import annotations

import itertools
import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
PROJ = HERE / "proj"
FIXTURE = ROOT / "sv0cov" / "tests" / "fixtures" / "semantic" / "project"
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
VM_EMIT = ROOT / "build" / "sv0-megatu-vm-native"

sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_emit_c import SV0COV_RT, cc, link  # noqa: E402
from run_vm_profile import build_vm, sv0, transport, vm_run  # noqa: E402

# (fragment source path, slice_base, slice_length), hand-derived: calc has
# sum_to's entry + loop body/exit, geom has area's entry + if true/false,
# main has main's entry; kinds and zz have no function.
FRAGMENTS = [("calc/math.sv0", 0, 3), ("geom/shapes.sv0", 3, 3), ("kinds/kinds.sv0", 6, 0),
             ("main.sv0", 6, 1), ("zz/types.sv0", 7, 0)]
# Counts of one run, by counter index: sum_to(4) entry 1, body 4, exit 1;
# area(3) entry 1, true 1, false 0; main entry 1.
COUNTS = [1, 4, 1, 1, 1, 0, 1]

VALIDATE = r"""
import json, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov.formats.map import validate_map
data = Path(sys.argv[2]).read_bytes()
m = json.loads(data)
validate_map(data, sources={s['path']: (Path(sys.argv[3]) / s['path']).read_bytes() for s in m['sources']})
"""


def build_map(binary: Path, project: Path, out: Path, order: str | None, target: str) -> tuple[int, str]:
    env = {k: v for k, v in os.environ.items() if k != "SV0_COVERAGE_LINK_ORDER"}
    if order is not None:
        env["SV0_COVERAGE_LINK_ORDER"] = order
    env["SV0_DRV_REQUEST"] = f"--project {project}"
    env["SV0_COVERAGE_REQUEST"] = f"map\n{out}\n{target}\nsv0cov-fixture"
    argv = [str(binary)] + (["--project", str(project)] if binary == NATIVE else [])
    p = subprocess.run(argv, capture_output=True, env=env, timeout=300)  # the VM emitter prints bytecode
    return p.returncode, p.stderr.decode("utf-8", "replace")


def registration_matrix(t: Path, errors: list[str]) -> int:
    d = t / "registration"
    d.mkdir()
    env = dict(os.environ, SV0_DRV_REQUEST=f"--project {PROJ}",
               SV0_COVERAGE_REQUEST=f"instrument\n{d / 'm.json'}\nfragments\nsv0c+test")
    env.pop("SV0_COVERAGE_LINK_ORDER", None)
    p = subprocess.run([str(NATIVE), "--project", str(PROJ)], capture_output=True, text=True, env=env, timeout=300)
    if p.returncode:
        errors.append(f"registration: instrument build failed: {p.stderr.strip()}")
        return 0
    c = p.stdout
    m = json.loads((d / "m.json").read_bytes())
    mid = m["map_id"]
    agg = "static const struct __sv0cov_module *const __sv0cov_modules[5] = {" + \
          ", ".join(f"&__sv0cov_module_{j}" for j in range(5)) + "};"
    start = "__sv0cov_start(__sv0cov_modules, 5u);"
    if agg not in c or start not in c:
        errors.append("registration: the C does not register five per-fragment modules")
        return 0
    rt = d / "sv0cov_rt.o"
    r = subprocess.run([cc(), "-std=c11", "-O0", "-c", str(SV0COV_RT), "-o", str(rt)], capture_output=True, text=True)
    if r.returncode:
        errors.append(f"registration: runtime compile failed: {r.stderr[-300:]}")
        return 0
    # (kind, edit): each breaks one rule in a module other than the first.
    cases = {
        "valid": c,
        "wrong map": c.replace(f'= {{\n  1u, "{mid}", 7u, "fragments", "sv0c+test", 3u, 3u',
                               '= {\n  1u, "' + "1" * 64 + '", 7u, "fragments", "sv0c+test", 3u, 3u', 1),
        "overlap": c.replace('"sv0c+test", 3u, 3u, 1u, __sv0cov_fragment_1', '"sv0c+test", 2u, 3u, 1u, __sv0cov_fragment_1')
                    .replace(', 3u, 3u}};', ', 2u, 3u}};', 1),
        "gap": c.replace('"sv0c+test", 6u, 1u, 1u, __sv0cov_fragment_3', '"sv0c+test", 7u, 0u, 1u, __sv0cov_fragment_3')
                .replace(', 6u, 1u}};', ', 7u, 0u}};', 1),
        "empty fragment off a boundary": c.replace('"sv0c+test", 6u, 0u, 1u, __sv0cov_fragment_2',
                                                   '"sv0c+test", 5u, 0u, 1u, __sv0cov_fragment_2')
                                          .replace(', 6u, 0u}};', ', 5u, 0u}};', 1),
        "module registered twice": c.replace(agg, agg.replace("[5]", "[6]").replace("};", ", &__sv0cov_module_1};"))
                                    .replace(start, "__sv0cov_start(__sv0cov_modules, 6u);"),
        "positive module missing": c.replace(agg, agg.replace("[5]", "[4]").replace("&__sv0cov_module_3, ", ""))
                                    .replace(start, "__sv0cov_start(__sv0cov_modules, 4u);"),
    }
    n = 0
    for kind, text in cases.items():
        if kind != "valid" and text == c:
            errors.append(f"registration: the {kind!r} edit did not apply")
            continue
        k = d / kind.replace(" ", "-")
        (k / "p").mkdir(parents=True)
        (k / "main.c").write_text(text)
        lk = link(k / "main.c", k / "main", str(rt))
        if lk.returncode:
            errors.append(f"registration {kind}: link failed: {lk.stderr[-300:]}")
            continue
        env = {kk: v for kk, v in os.environ.items() if not kk.startswith("SV0COV")}
        env.update(SV0COV_PROFILE_DIR=str(k / "p"), SV0COV_RUN_ID="0123456789abcdef0123456789abcdef", SV0COV_REQUIRED="1")
        run = subprocess.run([str(k / "main")], capture_output=True, text=True, env=env, timeout=60)
        published = list((k / "p").iterdir())
        if kind == "valid":
            if run.returncode != 0 or len(published) != 1:
                errors.append(f"registration: the untampered program exited {run.returncode} with {len(published)} profiles: {run.stderr}")
        else:
            if run.returncode != 1 or "error[COV1015]" not in run.stderr or run.stdout or published:
                errors.append(f"registration {kind}: exit {run.returncode}, {len(published)} profiles, stderr {run.stderr.strip()!r}")
            n += 1
    return n


def main() -> int:
    ensure_built()
    errors: list[str] = []
    py = modern_python()
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)

        # 1 + 2. Every link order gives the same map, on both compilers.
        for project, nfiles, target, want_path in ((PROJ, 5, "fragments", None), (FIXTURE, 3, "project", FIXTURE / "expected-map.json")):
            canonical = t / f"{target}-canonical.json"
            rc, err = build_map(NATIVE, project, canonical, None, target)
            if rc or not canonical.is_file():
                errors.append(f"{target}: map build failed: {err.strip()}")
                continue
            want = canonical.read_bytes() if want_path is None else want_path.read_bytes()
            if canonical.read_bytes() != want:
                errors.append(f"{target}: the sorted-order map differs from {want_path}")
            n = 0
            for perm in itertools.permutations(range(nfiles)):
                order = ",".join(map(str, perm))
                for binary in (NATIVE, VM_EMIT):
                    out = t / f"{target}-{binary.name}-{order}.json"
                    rc, err = build_map(binary, project, out, order, target)
                    if rc or not out.is_file() or out.read_bytes() != want:
                        errors.append(f"{target}: link order {order} on {binary.name}: rc={rc}, "
                                      f"{'differs' if out.is_file() else 'no map'} {err.strip()[:200]}")
                    n += 1
            if project == PROJ:
                m = json.loads(want)
                paths = [s["path"] for s in m["sources"]]
                got = [(paths[f["source_indices"][0]], f["slice_base"], f["slice_length"]) for f in m["fragments"]]
                if got != FRAGMENTS or m["program_counter_count"] != 7:
                    errors.append(f"fragments: table {got} (count {m['program_counter_count']}), want {FRAGMENTS}")
                if py is not None:
                    v = subprocess.run([*py, "-c", VALIDATE, str(ROOT / "sv0cov" / "src"), str(canonical), str(PROJ)],
                                       capture_output=True, text=True, timeout=120)
                    if v.returncode:
                        errors.append(f"fragments: sv0cov rejects the map: {v.stderr.strip()[-300:]}")

        # 3. Instrumented runs under two link orders, native and VM.
        runs: dict[str, list[int]] = {}
        for order in ("0,1,2,3,4", "4,3,2,1,0"):
            d = t / f"run-{order.replace(',', '')}"
            (d / "vm").mkdir(parents=True)
            os.environ["SV0_COVERAGE_LINK_ORDER"] = order
            try:
                p = sv0("native-compile", "--coverage=instrument", "-o", str(d / "main"), "--project", str(PROJ))
                err = build_vm(["--project", str(PROJ)], d / "vm" / "main.sv0b")
            finally:
                del os.environ["SV0_COVERAGE_LINK_ORDER"]
            if p.returncode or err:
                errors.append(f"link order {order}: instrumented build failed: {p.stderr.strip()} {err or ''}")
                continue
            for backend in ("native", "vm"):
                pdir = d / f"{backend}-profiles"
                pdir.mkdir()
                if backend == "native":
                    rc = subprocess.run([str(d / "main")], capture_output=True, env=transport(pdir), timeout=60).returncode
                else:
                    rc, _ = vm_run(d / "vm" / "main.sv0b", d / "vm" / "main.sv0covbind.json", transport(pdir))
                profiles = sorted(pdir.iterdir())
                if rc != 0 or len(profiles) != 1:
                    errors.append(f"link order {order} {backend}: exit {rc}, {len(profiles)} profiles")
                    continue
                sys.path.insert(0, str(ROOT / "sv0cov" / "src"))
                from sv0cov.formats.rawprofile import decode
                mp = d / "main.sv0covmap.json" if backend == "native" else d / "vm" / "main.sv0covmap.json"
                m = json.loads(mp.read_bytes())
                prof = decode(profiles[0].read_bytes(), map_counter_count=m["program_counter_count"],
                              expected_map_id=bytes.fromhex(m["map_id"]))
                dense = [0] * m["program_counter_count"]
                for i, c in prof.counts:
                    dense[i] = c
                runs[f"{order} {backend}"] = dense
        for key, dense in runs.items():
            if dense != COUNTS:
                errors.append(f"{key}: counts {dense}, want {COUNTS}")
        if len(runs) != 4:
            errors.append(f"expected 4 runs, got {sorted(runs)}")

        # 4. A bad link order fails the build.
        for bad in ("0,1,2,3", "0,0,1,2,3", "0,1,2,3,9", "a,1,2,3,4", "0,,1,2,3,4"):
            out = t / "bad.json"
            rc, err = build_map(NATIVE, PROJ, out, bad, "fragments")
            if rc == 0 or out.exists() or "not a permutation" not in err:
                errors.append(f"link order {bad!r} was accepted (rc={rc})")

        # 5. Per-fragment registration against the real runtime.
        tampered = registration_matrix(t, errors)

    if errors:
        print("coverage fragments: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    note = "sv0cov-validated" if py is not None else "validation SKIPPED (no Python >= 3.10)"
    print(f"coverage fragments: OK (5-module project: 120 link orders x native + VM emitter give one map ({note}), "
          f"zero-length slices placed per 16.3.3; project fixture: 6 orders = expected-map.json; instrumented runs "
          f"in two link orders count the same on C and VM; bad orders refused; one module per fragment "
          f"registers under the real runtime and {tampered} tampered registrations fail before user code)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
