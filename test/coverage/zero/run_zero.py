#!/usr/bin/env python3
"""Zero-counter programs (sv0cov CV-207; SPEC 13.1.1, 14.1, 16.4;
COV-INS-015, COV-C-006, COV-VM-025, COV-FMT-034, AC-095).

A zero-counter program plans no counter: it has no hit, yet an instrumented
build keeps its map and its binding or registration, and an orderly run
publishes one complete empty profile.

What exists today. Every runnable sv0 program has a `main`, whose entry is
always counted, so no real program has zero counters until exclusions
(CV-305) can exclude everything. A source with declarations only (decl.sv0)
does plan zero counters, but it is not an executable: the native driver
refuses it (ENTRY-004) and sv0vm has no `main` to run. An empty file is not
an sv0 program at all (the compiler rejects it in every mode).

So this script checks, on the native compiler and the VM emitter:

1. decl.sv0 in map mode: one byte-identical, sv0cov-valid map with zero
   counters, no points, and one empty fragment at base 0. A VM instrument
   build writes the same kind of map, bytecode identical to an off build
   (no COVER_HIT), and a valid binding counting 0. A native instrument
   build is refused (no hosted main), and so is the native driver.
2. Stand-in for a fully excluded build (owner's choice): standin.sv0's
   uninstrumented C, with the zero-counter registration protocol 1
   prescribes for decl's map, linked with the real sv0cov runtime; and its
   uninstrumented bytecode with decl's binding re-bound to it. Each runs
   exactly like the off build and publishes one complete empty profile
   (zero pairs, zero overflow words, no overflow flag; mode 0600) bound to
   the zero-counter map; sv0cov's reader resolves both to no counts and
   every line of decl.sv0 is non_executable. CV-305 repeats this with a
   real fully excluded program.
3. COV-VM-025 through `sv0 vm-run`: a zero-count binding on bytecode that
   has COVER_HIT, a positive-count binding on bytecode that has none, and
   instrumented bytecode with no binding are all rejected (COV2201) before
   user code, with no profile.
4. An empty source is rejected by both compilers in every mode, with no map.

    python3 sv0c/test/coverage/zero/run_zero.py
"""

from __future__ import annotations

import hashlib
import json
import os
import re
import stat
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
VM_EMIT = ROOT / "build" / "sv0-megatu-vm-native"
RUN_ID = "0123456789abcdef0123456789abcdef"

sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
sys.path.insert(0, str(ROOT / "sv0cov" / "src"))
from run_emit_c import SV0COV_RT, cc, link  # noqa: E402
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import sv0, transport, vm_run  # noqa: E402
from sv0cov.formats.rawprofile import OVERFLOW_PRESENT, RawProfileError, decode  # noqa: E402

DECLS = (
    "struct __sv0cov_fragment {\n  const char *fragment_id;\n  uint32_t slice_base;\n  uint32_t slice_length;\n};\n"
    "struct __sv0cov_module {\n  uint32_t protocol_major;\n  const char *map_id;\n  uint32_t program_counter_count;\n"
    "  const char *target;\n  const char *compiler_identity;\n  uint32_t slice_base;\n  uint32_t slice_length;\n"
    "  uint32_t fragment_count;\n  const struct __sv0cov_fragment *fragments;\n};\n"
    "void __sv0cov_start(const struct __sv0cov_module *const *modules, uint32_t module_count);\n"
    "void __sv0cov_hit(const struct __sv0cov_module *module, uint32_t local_index);\n"
)

# Validate a map with its sources; resolve profiles through it; print the
# resolution's point count, backends, and the distinct line statuses.
READER = r"""
import json, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov.formats.map import validate_map
from sv0cov.formats.vmbinding import decode_v1
from sv0cov.lines import line_records
from sv0cov.resolve import resolve
mb = Path(sys.argv[2]).read_bytes()
src = {'decl.sv0': Path(sys.argv[3]).read_bytes()}
m = validate_map(mb, sources=src)
for b, code in zip(sys.argv[4::2], sys.argv[5::2]):
    if b != '-':
        decode_v1(Path(b).read_bytes(), Path(code).read_bytes())
profiles = [Path(p).read_bytes() for p in json.loads(sys.argv[-1])]
out = {'count': m['program_counter_count'], 'points': len(m['points']), 'regions': len(m['regions']),
       'fragments': [[f['slice_base'], f['slice_length']] for f in m['fragments']]}
if profiles:
    r = resolve(mb, profiles, sources=src)
    out['resolved_points'] = len(r.points)
    out['backends'] = list(r.backends)
    out['statuses'] = sorted({x['status'] for x in line_records(m, None, src, contexts=r.context_counts())})
print(json.dumps(out, sort_keys=True))
"""


def compiler(binary: Path, src: Path, coverage: str | None) -> subprocess.CompletedProcess:
    env = {k: v for k, v in os.environ.items() if k not in ("SV0_COVERAGE_REQUEST", "SV0_COVERAGE_LINK_ORDER")}
    env["SV0_DRV_REQUEST"] = str(src)
    if coverage is not None:
        env["SV0_COVERAGE_REQUEST"] = coverage
    argv = [str(binary)] + ([str(src)] if binary == NATIVE else [])
    return subprocess.run(argv, capture_output=True, env=env, timeout=300)


def empty_profile(pdir: Path, map_id: str, backend: str, errors: list[str], label: str) -> Path | None:
    files = sorted(pdir.iterdir())
    if len(files) != 1 or not files[0].name.startswith(RUN_ID + "-") or files[0].suffix != ".sv0profraw":
        errors.append(f"{label}: expected one published profile, found {[f.name for f in files]}")
        return None
    data = files[0].read_bytes()
    try:
        prof = decode(data, map_counter_count=0, expected_map_id=bytes.fromhex(map_id))
    except RawProfileError as exc:
        errors.append(f"{label}: the profile is not a complete empty profile of the map: {exc}")
        return None
    flags = int.from_bytes(data[12:16], "little")
    if prof.counts or flags & OVERFLOW_PRESENT or prof.backend != backend or len(data) != 104:
        errors.append(f"{label}: profile has counts {prof.counts}, flags {flags:#x}, backend {prof.backend}, {len(data)} bytes")
    if stat.S_IMODE(files[0].stat().st_mode) != 0o600:
        errors.append(f"{label}: the profile is not mode 0600")
    return files[0]


def main() -> int:
    ensure_built()
    errors: list[str] = []
    py = modern_python()
    decl, standin = HERE / "decl.sv0", HERE / "standin.sv0"
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)

        # 1. The declarations-only source plans zero counters.
        maps = {}
        for binary in (NATIVE, VM_EMIT):
            out = t / f"map-{binary.name}.json"
            p = compiler(binary, decl, f"map\n{out}\ndecl\nsv0c+test")
            if p.returncode or not out.is_file():
                errors.append(f"decl map mode on {binary.name}: rc={p.returncode} {p.stderr.decode()[:200]}")
            else:
                maps[binary.name] = out.read_bytes()
        if len(maps) != 2 or len(set(maps.values())) != 1:
            print("coverage zero: FAIL\n  - the native and VM map-mode maps of decl.sv0 are missing or differ", file=sys.stderr)
            return 1
        zmap = t / "map-sv0-megatu-compiler-native.json"
        m = json.loads(zmap.read_bytes())
        if (m["program_counter_count"], m["points"], m["regions"], m["branches"], m["entities"]) != (0, [], [], [], []):
            errors.append("decl: the map is not a zero-counter map with empty semantic arrays")
        if [(f["slice_base"], f["slice_length"]) for f in m["fragments"]] != [(0, 0)]:
            errors.append(f"decl: fragments {m['fragments']}, want one empty fragment at base 0")

        vm = t / "vm"
        vm.mkdir()
        p = sv0("vm-native-compile", "--coverage=instrument", str(decl), str(vm / "decl.sv0b"))
        off = compiler(VM_EMIT, decl, None)
        vmap, vbind = vm / "decl.sv0covmap.json", vm / "decl.sv0covbind.json"
        if p.returncode or not vmap.is_file() or not vbind.is_file():
            errors.append(f"decl: VM instrument build failed: {p.stderr.strip()}")
            print("coverage zero: FAIL\n  - " + "\n  - ".join(errors), file=sys.stderr)
            return 1
        vm_m = json.loads(vmap.read_bytes())
        bind = json.loads(vbind.read_bytes())
        if vm_m["program_counter_count"] != 0 or vm_m["points"] or bind["program_counter_count"] != 0 \
                or bind["map_id"] != vm_m["map_id"]:
            errors.append("decl: the VM instrument map or binding is not zero-counter and consistent")
        if (vm / "decl.sv0b").read_bytes() != off.stdout or off.returncode:
            errors.append("decl: zero-counter instrumented bytecode differs from the off build (it must have no COVER_HIT)")
        r = compiler(NATIVE, decl, f"instrument\n{t / 'refused.json'}\ndecl\nsv0c+test")
        if r.returncode != 9 or (t / "refused.json").exists() or b"hosted" not in r.stderr:
            errors.append(f"decl: a native instrument build with no main was not refused (rc={r.returncode})")
        r = sv0("native-compile", "--coverage=instrument", "-o", str(t / "decl-exe"), str(decl))
        if r.returncode == 0 or "ENTRY-004" not in r.stderr:
            errors.append(f"decl: the native driver built a program with no main: {r.stderr.strip()[:200]}")

        # 2. Stand-in for a fully excluded build, on both backends.
        zid = vm_m["map_id"]
        frag = vm_m["fragments"][0]
        native_dir, pn, pv = t / "native", t / "native-profiles", t / "vm-profiles"
        for d in (native_dir, pn, pv):
            d.mkdir()
        offc = compiler(NATIVE, standin, None)
        head, init = '#include "sv0_runtime.h"\n\n', "  sv0_runtime_init(argc, argv);\n"
        c = offc.stdout.decode()
        if offc.returncode or not c.startswith(head) or c.count(init) != 1:
            errors.append("stand-in: the off build's C does not have the hosted main shape")
        else:
            prelude = (DECLS + f'static const struct __sv0cov_fragment __sv0cov_fragment_0[1] = '
                       f'{{{{"{frag["fragment_id"]}", 0u, 0u}}}};\n'
                       f'static const struct __sv0cov_module __sv0cov_module_0 = {{\n'
                       f'  1u, "{zid}", 0u, "decl", "{bind["compiler_identity"]}", 0u, 0u, 1u, __sv0cov_fragment_0\n}};\n'
                       f'static const struct __sv0cov_module *const __sv0cov_modules[1] = {{&__sv0cov_module_0}};\n\n')
            inst = head + prelude + c[len(head):].replace(init, init + "  __sv0cov_start(__sv0cov_modules, 1u);\n")
            (native_dir / "off.c").write_text(c)
            (native_dir / "zero.c").write_text(inst)
            rt = native_dir / "sv0cov_rt.o"
            k = subprocess.run([cc(), "-std=c11", "-O0", "-c", str(SV0COV_RT), "-o", str(rt)], capture_output=True, text=True)
            l1 = link(native_dir / "off.c", native_dir / "off", None)
            l2 = link(native_dir / "zero.c", native_dir / "zero", str(rt))
            if k.returncode or l1.returncode or l2.returncode:
                errors.append(f"stand-in: native link failed: {k.stderr[-200:]} {l1.stderr[-200:]} {l2.stderr[-200:]}")
            else:
                a = subprocess.run([str(native_dir / "off")], capture_output=True, timeout=60)
                b = subprocess.run([str(native_dir / "zero")], capture_output=True, env=transport(pn), timeout=60)
                if (a.returncode, a.stdout) != (b.returncode, b.stdout) or b.returncode != 0 or b"stand-in ran" not in b.stdout:
                    errors.append(f"stand-in native: exit {b.returncode} {b.stdout!r} {b.stderr.decode()[-200:]!r}, off exit {a.returncode}")
        nprof = empty_profile(pn, zid, "native", errors, "stand-in native") if any(pn.iterdir()) or not errors else None

        p = sv0("vm-native-compile", str(standin), str(vm / "standin.sv0b"))
        code = (vm / "standin.sv0b").read_bytes() if (vm / "standin.sv0b").is_file() else b""
        text = vbind.read_text()
        rebound = re.sub(r'"bytecode_length":\d+', f'"bytecode_length":{len(code)}', text)
        rebound = re.sub(r'"bytecode_sha256":"[0-9a-f]{64}"', f'"bytecode_sha256":"{hashlib.sha256(code).hexdigest()}"', rebound)
        zbind = vm / "standin.sv0covbind.json"
        zbind.write_text(rebound)
        vprof = None
        if p.returncode or not code or rebound == text:
            errors.append(f"stand-in: VM off build or binding re-bind failed: {p.stderr.strip()}")
        else:
            rc, out = vm_run(vm / "standin.sv0b", zbind, transport(pv))
            if rc != 0 or "stand-in ran" not in out:
                errors.append(f"stand-in VM: exit {rc}: {out[-300:]}")
            vprof = empty_profile(pv, zid, "vm-v1", errors, "stand-in VM")

        reader = "sv0cov reader SKIPPED (no Python >= 3.10)"
        if py is not None:
            profiles = [str(x) for x in (nprof, vprof) if x is not None]
            q = subprocess.run([*py, "-c", READER, str(ROOT / "sv0cov" / "src"), str(vmap), str(decl),
                                str(vbind), str(vm / "decl.sv0b"), str(zbind), str(vm / "standin.sv0b"),
                                json.dumps(profiles)], capture_output=True, text=True, timeout=120)
            want = {"count": 0, "points": 0, "regions": 0, "fragments": [[0, 0]], "resolved_points": 0,
                    "backends": ["native", "vm-v1"], "statuses": ["non_executable"]}
            if q.returncode or json.loads(q.stdout or "{}") != want:
                errors.append(f"sv0cov reader: {q.stdout.strip()} {q.stderr.strip()[-400:]} (want {want})")
            else:
                reader = "sv0cov validates the map and bindings and resolves both profiles to no counts"

        # 3. COV-VM-025: count and hit presence must agree.
        inst_dir = t / "inst"
        inst_dir.mkdir()
        p = sv0("vm-native-compile", "--coverage=instrument", str(standin), str(inst_dir / "standin.sv0b"))
        ibind = inst_dir / "standin.sv0covbind.json"
        if p.returncode or not ibind.is_file():
            errors.append(f"mismatch matrix: instrumented stand-in build failed: {p.stderr.strip()}")
        else:
            zero_on_hits = inst_dir / "zero-count.json"
            zero_on_hits.write_text(re.sub(r'"program_counter_count":\d+', '"program_counter_count":0', ibind.read_text()))
            count_no_hits = vm / "positive-count.json"
            count_no_hits.write_text(rebound.replace('"program_counter_count":0', '"program_counter_count":3'))
            for label, sv0b, binding in (("zero-count binding, bytecode with COVER_HIT", inst_dir / "standin.sv0b", zero_on_hits),
                                         ("positive-count binding, bytecode without COVER_HIT", vm / "standin.sv0b", count_no_hits)):
                pdir = t / ("m-" + label.split()[0])
                pdir.mkdir()
                rc, out = vm_run(sv0b, binding, transport(pdir))
                if rc is not None or "COV2201" not in out or "stand-in ran" in out or any(pdir.iterdir()):
                    errors.append(f"{label}: not rejected before user code (exit {rc}): {out[-300:]}")
            pdir = t / "m-unbound"
            pdir.mkdir()
            q = subprocess.run([str(ROOT / "scripts" / "sv0"), "vm-run", str(inst_dir / "standin.sv0b")],
                               capture_output=True, text=True, env=transport(pdir), timeout=120)
            if "COV2201" not in q.stdout + q.stderr or "stand-in ran" in q.stdout or any(pdir.iterdir()):
                errors.append(f"instrumented bytecode without a binding was not rejected: {(q.stdout + q.stderr)[-300:]}")

        # 4. An empty source is not a program.
        empty = t / "empty.sv0"
        empty.write_text("")
        for binary in (NATIVE, VM_EMIT):
            for mode in (None, "map", "instrument"):
                out = t / f"empty-{binary.name}-{mode}.json"
                p = compiler(binary, empty, None if mode is None else f"{mode}\n{out}\nempty\nsv0c+test\n{out}.bind")
                if p.returncode == 0 or out.exists():
                    errors.append(f"an empty source compiled on {binary.name} in mode {mode}")

    if errors:
        print("coverage zero: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print("coverage zero: OK (declarations-only source: one zero-counter map on native + VM emitter, VM binding counts 0, "
          "bytecode = off; stand-in fully excluded program publishes one complete empty profile on C and VM; "
          f"{reader}; count/hit mismatches and unbound hits rejected (COV2201); empty source refused)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
