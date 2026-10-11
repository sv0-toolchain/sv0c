#!/usr/bin/env python3
"""Region inventory, unreachable provenance, and corpus sweep (sv0cov CV-201).

1. Targeted programs, each against its hand-reviewed `<name>.expected`
   (kind, span, classification, and the region's count from one native run
   resolved through the map by sv0cov's reader):
   - dead_code.sv0: code after an unconditional `return` / `break` is
     `statically_unreachable` with the canonical empty expression (SPEC 11.7)
     and counts 0;
   - unsafe_block.sv0: `unsafe { ... }` is planned as its block, with no
     region of its own;
   - scrutinee.sv0: every `match` scrutinee (statement and tail position) is
     an `expression` region at the enclosing position, so a call in it is
     owned (SPEC 11.3);
   - desugar.sv0 (COV-MET-008): `for` and compound assignment plan only
     source regions (the iterable, the loop body, one `mutation` region);
     no compiler-generated code gets a region.
2. `?` is planned (CV-213; the reviewed `question` fixture pins its points
   and counts). Where the planner cannot reach one, in a contract clause
   (question_requires.sv0) or a loop invariant (question_invariant.sv0), the
   build is refused (exit 9, no map) rather than left over-counting the code
   after the early return. The corpus `?` programs must each carry a
   synthetic-support propagate point. Three of them, including `?` on a
   struct field and on a block (operands lowering used to drop silently),
   also run instrumented on the C backend and on sv0vm: the same exit
   status and the same counts, with the propagate point counting exactly
   the early returns.
3. Sweep: every program in sv0c/test/behavior/cases, every
   sv0c/test/integration project, and lib/parser.sv0 (7k lines) plan in map
   mode, except the known refusal (`include`), and every map passes
   sv0cov's own validate_map with its source bytes (run with the first
   Python >= 3.10 found), and its line flags follow CV-202: a region
   contributes to lines exactly when it is a `user` region and not a
   container (branch body, loop body, block match arm).

    python3 sv0c/test/coverage/regions/run_regions.py
"""

from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
SV0 = ROOT / "scripts" / "sv0"
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
RUN_ID = "0123456789abcdef0123456789abcdef"

sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import build_vm, transport, vm_run  # noqa: E402
from run_vm_profile import profile as read_profile  # noqa: E402

TARGETED = ("dead_code", "unsafe_block", "scrutinee", "desugar")
# Programs coverage refuses on purpose, with the reason it must name.
REFUSED = {
    "include_basic": "compiled source differs",
    "question_requires": "`?` operator where coverage cannot plan it",
    "question_invariant": "`?` operator where coverage cannot plan it",
}
# Corpus programs that use `?`: each map has a propagate point (CV-213).
QUESTION = ("question_op", "question_none_propagates", "option_result", "question_field_operand", "question_block_operand",
            "question_if_operand")
# name -> (exit status, early returns through `?`), run on native and sv0vm.
QUESTION_RUNS = {"question_none_propagates": (7, 1), "question_field_operand": (7, 1), "question_block_operand": (9, 1),
                 "question_op": (42, 0), "question_if_operand": (14, 2)}

# Resolve a native profile through its map and print each region's line.
RESOLVE = r"""
import json, sys
sys.path.insert(0, sys.argv[1])
from sv0cov.resolve import resolve
mb = open(sys.argv[2], 'rb').read()
m = json.loads(mb)
c = resolve(mb, [open(sys.argv[3], 'rb').read()]).counts()
for r in m['regions']:
    sp = r['span']
    v = sum(t['coefficient'] * c[t['point_id']] for t in r['counter_expression']['terms'])
    print(f"{r['kind']} {sp['start_line']}:{sp['start_column']}-{sp['end_line']}:{sp['end_column']} {r['classification']} {v}")
"""

# Validate many maps against their source bytes; print one line per failure.
VALIDATE = r"""
import json, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov.formats.map import validate_map
totals = {}
for mpath, root in json.loads(sys.argv[2]):
    data = Path(mpath).read_bytes()
    m = json.loads(data)
    srcs = {s['path']: (Path(root) / s['path']).read_bytes() for s in m['sources']}
    try:
        validate_map(data, sources=srcs)
    except Exception as exc:
        print(f"{mpath}: {exc}")
        continue
    for r in m['regions']:
        totals[r['classification']] = totals.get(r['classification'], 0) + 1
        at = r['span']['start_byte']
        src = srcs[m['sources'][r['source_index']]['path']]
        container = r['kind'] in ('branch_body', 'loop_body') or (r['kind'] == 'match_arm' and src[at:at + 1] == b'{')
        if r['line_contributing'] != (r['classification'] == 'user' and not container):
            print(f"{mpath}: region {r['region_index']} ({r['kind']}, {r['classification']}) has line_contributing={r['line_contributing']}")
print("TOTALS " + json.dumps(totals, sort_keys=True))
"""


def map_build(request: list[str], out: Path) -> subprocess.CompletedProcess:
    env = dict(os.environ, SV0_DRV_REQUEST=" ".join(request),
               SV0_COVERAGE_REQUEST=f"map\n{out}\n{out.stem}\nsv0c+test")
    return subprocess.run([str(NATIVE), *request], capture_output=True, text=True, env=env, timeout=300)


def main() -> int:
    ensure_built()
    errors: list[str] = []
    py = modern_python()
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)

        # 1. Targeted programs.
        for name in TARGETED:
            src = HERE / f"{name}.sv0"
            d = t / name
            (d / "p").mkdir(parents=True)
            p = subprocess.run([str(SV0), "native-compile", "--coverage=instrument", "-o", str(d / "x"), str(src)],
                               capture_output=True, text=True, timeout=300)
            if p.returncode:
                errors.append(f"{name}: build failed: {p.stderr.strip()}")
                continue
            env = {k: v for k, v in os.environ.items() if not k.startswith("SV0COV")}
            env.update(SV0COV_PROFILE_DIR=str(d / "p"), SV0COV_RUN_ID=RUN_ID, SV0COV_REQUIRED="1")
            r = subprocess.run([str(d / "x")], capture_output=True, env=env, timeout=60)
            if r.returncode != 0:
                errors.append(f"{name}: the program exited {r.returncode}")
                continue
            if py is None:
                continue
            profile = next((d / "p").iterdir())
            got = subprocess.run([*py, "-c", RESOLVE, str(ROOT / "sv0cov" / "src"), str(d / "x.sv0covmap.json"),
                                  str(profile)], capture_output=True, text=True, timeout=120)
            want = [line for line in (HERE / f"{name}.expected").read_text().splitlines()
                    if line and not line.startswith("#")]
            if got.returncode or got.stdout.splitlines() != want:
                errors.append(f"{name}: regions differ from {name}.expected:\n    got  {got.stdout.splitlines()}\n"
                              f"    want {want}\n    {got.stderr.strip()[-300:]}")

        # 2. `?` programs run instrumented on both backends.
        for name, (status, early) in QUESTION_RUNS.items():
            src = SV0C / "test" / "behavior" / "cases" / f"{name}.sv0"
            d = t / f"q-{name}"
            (d / "vm").mkdir(parents=True)
            p = subprocess.run([str(SV0), "native-compile", "--coverage=instrument", "-o", str(d / "x"), str(src)],
                               capture_output=True, text=True, timeout=300)
            err = build_vm([str(src)], d / "vm" / "x.sv0b")
            if p.returncode or err:
                errors.append(f"{name}: instrumented build failed: {p.stderr.strip()[-300:]} {err or ''}")
                continue
            m = json.loads((d / "x.sv0covmap.json").read_bytes())
            if (d / "vm" / "x.sv0covmap.json").read_bytes() != (d / "x.sv0covmap.json").read_bytes():
                errors.append(f"{name}: the C and VM maps differ")
            qctr = sorted(q["counter_index"] for q in m["points"] if q["semantic_discriminator"] == "question:propagate")
            seen = {}
            for backend in ("native", "vm"):
                pdir = d / f"p-{backend}"
                pdir.mkdir()
                if backend == "native":
                    rc = subprocess.run([str(d / "x")], capture_output=True, env=transport(pdir), timeout=60).returncode
                else:
                    rc, _ = vm_run(d / "vm" / "x.sv0b", d / "vm" / "x.sv0covbind.json", transport(pdir))
                prof = read_profile(pdir, m, errors, f"{name} {backend}")
                if rc != status or prof is None:
                    errors.append(f"{name} {backend}: exit {rc}, want {status} with a profile")
                    continue
                counts = dict(prof.counts)
                seen[backend] = counts
                if sum(counts.get(c, 0) for c in qctr) != early:
                    errors.append(f"{name} {backend}: `?` propagate points count {[counts.get(c, 0) for c in qctr]}, want {early} in total")
            if len(seen) == 2 and seen["native"] != seen["vm"]:
                errors.append(f"{name}: native and VM counts differ")

        # 2 + 3. The corpus sweep.
        requests: list[tuple[str, list[str], Path]] = []
        for f in sorted((SV0C / "test" / "behavior" / "cases").glob("*.sv0")):
            requests.append((f.stem, [str(f)], f.parent))
        for d in sorted(p for p in (SV0C / "test" / "integration").iterdir() if p.is_dir() and any(p.glob("*.sv0"))):
            # `include` is a single-file feature: that case builds as a file.
            mode = [str(d / "main.sv0")] if d.name == "include_basic" else ["--project", str(d)]
            requests.append((d.name, mode, d))
        requests.append(("parser", [str(SV0C / "lib" / "parser.sv0")], SV0C / "lib"))
        for name in ("question_requires", "question_invariant"):
            requests.append((name, [str(HERE / f"{name}.sv0")], HERE))
        maps: list[tuple[str, str]] = []
        refused_seen = set()
        question_seen: set[str] = set()
        for name, request, root in requests:
            out = t / "sweep" / f"{name}-{len(maps)}.json"
            out.parent.mkdir(exist_ok=True)
            p = map_build(request, out)
            if name in REFUSED:
                refused_seen.add(name)
                if p.returncode != 9 or REFUSED[name] not in p.stderr or out.exists():
                    errors.append(f"{name}: want a refusal naming {REFUSED[name]!r}, got rc={p.returncode}: {p.stderr.strip()}")
                continue
            if p.returncode or not out.is_file():
                errors.append(f"{name}: map mode failed (rc={p.returncode}): {p.stderr.strip()[:300]}")
                continue
            maps.append((str(out), str(root)))
            if name in QUESTION:
                question_seen.add(name)
                m = json.loads(out.read_bytes())
                qs = [q for q in m["points"] if q["semantic_discriminator"] == "question:propagate"]
                bad_q = [q for q in qs if (q["kind"], q["classification"], q["outcome_ordinal"]) != ("region", "synthetic_support", None)
                         or q["counter_index"] is None]
                syn = [r for r in m["regions"] if r["classification"] == "synthetic_support"]
                if not qs or bad_q or len(syn) != len(qs) or any(r["kind"] != "return" or r["line_contributing"] for r in syn):
                    errors.append(f"{name}: want one counted synthetic-support region point and `return` region per `?`: "
                                  f"{len(qs)} points, {len(bad_q)} malformed, {len(syn)} regions")
        if not set(QUESTION) <= question_seen:
            errors.append(f"the sweep did not plan every `?` corpus program: {sorted(set(QUESTION) - question_seen)}")
        if refused_seen != set(REFUSED):
            errors.append(f"the sweep did not meet every expected refusal: {sorted(set(REFUSED) - refused_seen)}")
        totals = "validation SKIPPED (no Python >= 3.10)"
        if py is not None and maps:
            v = subprocess.run([*py, "-c", VALIDATE, str(ROOT / "sv0cov" / "src"), json.dumps(maps)],
                               capture_output=True, text=True, timeout=1200)
            lines = v.stdout.splitlines()
            bad = [line for line in lines if not line.startswith("TOTALS ")]
            if v.returncode or bad:
                errors.extend(f"invalid map: {line}" for line in bad[:10])
                if v.returncode:
                    errors.append(f"validator crashed: {v.stderr.strip()[-600:]}")
            else:
                totals = "regions " + lines[-1][len("TOTALS "):]

    if errors:
        print("coverage regions: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage regions: OK ({len(TARGETED)} targeted programs match their reviewed regions and counts; "
          f"{len(maps)} programs swept and sv0cov-validated ({totals}); {len(QUESTION)} `?` programs planned with synthetic-support propagate points; {len(QUESTION_RUNS)} of them run on C + sv0vm with equal counts; {len(REFUSED)} expected refusals)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
