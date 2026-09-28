#!/usr/bin/env python3
"""Generated-C coverage emission tests (sv0cov CV-113, SPEC 14.1).

With --coverage=instrument the native compiler emits C that calls the
coverage runtime: a registration prelude after the runtime include (the
runtime interface, one descriptor per map fragment, and the module record
with the map ID, program counter count, protocol major 1, target, and
compiler identity), `__sv0cov_hit(&__sv0cov_module, <i>u);` at every placed
hit, and `__sv0cov_start(__sv0cov_modules, 1u);` in the hosted main right
after sv0_runtime_init, before user code. The map is written once the C is.

For the seven sv0cov semantic fixtures and constructs.sv0 this checks:

1. The instrument map is byte-identical to the map-mode map.
2. The prelude carries the map's ID, counter count, fragment table, target,
   and identity; every counter 0..N-1 has exactly one hit in the C.
3. Removing the prelude, the hits, and the start call, and undoing the
   CV-112 loop rewrite (`while (1) { if (c) { } else { break; } ...` back to
   `while (c) { ...`), gives exactly the `off` build's C: instrumentation
   adds nothing else.
4. Linked with stub_rt.c (a test-only stand-in for the CV-114 runtime that
   validates the registration and counts hits) and run, the program exits
   as the off build does, and each point's count equals the fixture's
   hand-reviewed expected-counts.json. No gcov, profiling, or debug flags
   are used, and the C has no #line directives (COV-C-002).
5. f0's instrumented C equals the golden emit-f0.expected.c (COV-C-001;
   `--update` rewrites it).
6. A program whose main has no hosted wrapper is refused (exit 9) and
   leaves no map.

    python3 sv0c/test/coverage/plan/run_emit_c.py [--update]
"""

from __future__ import annotations

import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
RUNTIME = SV0C / "runtime"
GOLDEN = HERE / "emit-f0.expected.c"
IDENTITY = "sv0c+test"

sys.path.insert(0, str(HERE))
from run_hits import NATIVE, compile_with, programs  # noqa: E402
from run_plan import ensure_built  # noqa: E402

HIT_RE = re.compile(r"^\s*__sv0cov_hit\(&__sv0cov_module, (\d+)u\);$")
START = "  __sv0cov_start(__sv0cov_modules, 1u);"
PRELUDE_START = "/* sv0cov coverage instrumentation, generated-C protocol 1"
PRELUDE_END = "static const struct __sv0cov_module *const __sv0cov_modules[1] = {&__sv0cov_module};"


def c_str(s: str) -> str:
    """The generator's C string literal for an ASCII-printable string."""
    return '"' + s.replace("\\", "\\\\").replace('"', '\\"').replace("?", "\\?") + '"'


# CV-112 counts a loop's exit only when its condition is false (not on
# `break`), so an instrumented `while (c) {` becomes this shape, with the
# body and exit hits inside the `if`; with the hits removed it is undone.
LOOP_RE = re.compile(r"^( *)while \(1\) \{\n\1  if \((.*)\) \{\n\1  \} else \{\n\1    break;\n\1  \}\n", re.M)


def strip_coverage(c: str) -> str:
    """The instrumented C without its prelude, hits, start call, and loop rewrite."""
    lines = c.split("\n")
    a = next(i for i, line in enumerate(lines) if line.startswith(PRELUDE_START))
    b = lines.index(PRELUDE_END)
    kept = lines[:a] + lines[b + 2:]
    plain = "\n".join(line for line in kept if line != START and not HIT_RE.match(line))
    return LOOP_RE.sub(lambda m: f"{m.group(1)}while ({m.group(2)}) {{\n", plain)


def cc() -> str:
    return os.environ.get("CC") or shutil.which("cc") or "cc"


def link(c_path: Path, exe: Path, with_stub: bool) -> subprocess.CompletedProcess:
    argv = [cc(), "-std=gnu99", "-O0", "-I", str(RUNTIME), "-o", str(exe), str(c_path),
            str(RUNTIME / "sv0_runtime.c")]
    if with_stub:
        argv.append(str(HERE / "stub_rt.c"))
    return subprocess.run(argv, capture_output=True, text=True, timeout=300)


def main() -> int:
    update = "--update" in sys.argv[1:]
    ensure_built()
    errors: list[str] = []
    progs = programs()
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)
        for name, request in progs:
            d = t / name
            d.mkdir()
            ref = d / "map.json"
            inst_map = d / "inst.json"
            p = compile_with(NATIVE, request, f"map\n{ref}\n{name}\n{IDENTITY}")
            off = compile_with(NATIVE, request, "")
            q = compile_with(NATIVE, request, f"instrument\n{inst_map}\n{name}\n{IDENTITY}")
            if p.returncode or off.returncode or q.returncode:
                errors.append(f"{name}: map rc={p.returncode} off rc={off.returncode} instrument rc={q.returncode}: "
                              f"{off.stderr.strip()} {q.stderr.strip()}")
                continue
            c = q.stdout
            # 1. The same map as map mode.
            if not inst_map.is_file() or inst_map.read_bytes() != ref.read_bytes():
                errors.append(f"{name}: the instrument map is missing or differs from the map-mode map")
                continue
            m = json.loads(ref.read_bytes())
            total = m["program_counter_count"]
            # 2. The prelude carries the map; every counter has one hit.
            frags = "".join(f'  {{"{f["fragment_id"]}", {f["slice_base"]}u, {f["slice_length"]}u}},\n'
                            for f in m["fragments"])
            want = (f"static const struct __sv0cov_fragment __sv0cov_fragments[{len(m['fragments'])}] = {{\n{frags}}};\n"
                    f"static const struct __sv0cov_module __sv0cov_module = {{\n"
                    f"  1u, \"{m['map_id']}\", {total}u, {c_str(name)}, {c_str(IDENTITY)}, 0u, {total}u, "
                    f"{len(m['fragments'])}u, __sv0cov_fragments\n}};\n")
            if want not in c:
                errors.append(f"{name}: the prelude does not carry the map's fragment table and module record")
            hits = sorted(int(x.group(1)) for x in map(HIT_RE.match, c.split("\n")) if x)
            if hits != list(range(total)):
                errors.append(f"{name}: hits in the C {hits} are not exactly 0..{total - 1}")
            if c.count(START) != 1 or "sv0_runtime_init(argc, argv);\n" + START not in c:
                errors.append(f"{name}: __sv0cov_start is not called once, right after sv0_runtime_init")
            if "#line" in c:
                errors.append(f"{name}: the C has #line directives")
            # 3. Nothing else changes.
            try:
                stripped = strip_coverage(c)
            except (ValueError, StopIteration):
                errors.append(f"{name}: the coverage prelude is missing")
                continue
            if stripped != off.stdout:
                errors.append(f"{name}: the instrumented C minus coverage differs from the off build's C")
            # 4. Link with the stub runtime and run.
            (d / "inst.c").write_text(c)
            (d / "off.c").write_text(off.stdout)
            l1 = link(d / "inst.c", d / "inst", True)
            l2 = link(d / "off.c", d / "off", False)
            if l1.returncode or l2.returncode:
                errors.append(f"{name}: cc failed: {l1.stderr.strip()} {l2.stderr.strip()}")
                continue
            out = d / "counts.txt"
            r_inst = subprocess.run([str(d / "inst")], capture_output=True, timeout=60,
                                    env=dict(os.environ, SV0COV_STUB_OUT=str(out)))
            r_off = subprocess.run([str(d / "off")], capture_output=True, timeout=60)
            if r_inst.returncode != r_off.returncode or r_inst.stdout != r_off.stdout:
                errors.append(f"{name}: instrumented run (rc {r_inst.returncode}) differs from off (rc {r_off.returncode}) "
                              f"{r_inst.stderr.decode(errors='replace').strip()}")
                continue
            lines = out.read_text().split("\n") if out.is_file() else []
            if not lines or lines[0] != m["map_id"] or len(lines) != total + 2:
                errors.append(f"{name}: the stub runtime did not dump {total} counts for this map")
                continue
            counts = [int(x) for x in lines[1:total + 1]]
            exp_path = ROOT / "sv0cov" / "tests" / "fixtures" / "semantic" / name / "expected-counts.json"
            if exp_path.is_file():
                exp = json.loads(exp_path.read_bytes())
                if r_off.returncode != exp["exit_code"]:
                    errors.append(f"{name}: exit {r_off.returncode}, expected {exp['exit_code']}")
                by_id = {pt["point_id"]: counts[pt["counter_index"]] for pt in m["points"]
                         if pt.get("counter_index") is not None}
                for e in exp["counts"]:
                    got = by_id.get(e["point_id"])
                    if got != e["count"]:
                        errors.append(f"{name}: {e['label']} counted {got}, expected {e['count']}")
            elif sum(counts) == 0:
                errors.append(f"{name}: no counter was hit")
            # 5. The generated-C golden.
            if name == "f0":
                if update:
                    GOLDEN.write_text(c)
                elif not GOLDEN.is_file() or GOLDEN.read_text() != c:
                    errors.append(f"f0: instrumented C differs from {GOLDEN.relative_to(SV0C)} (--update to rewrite)")

        # 6. No hosted main to register from.
        src = t / "nohost.sv0"
        src.write_text("fn main(argc: i32) -> i32 {\n    return argc;\n}\n")
        mp = t / "nohost.json"
        p = compile_with(NATIVE, str(src), f"instrument\n{mp}\nnohost\n{IDENTITY}")
        if p.returncode != 9 or "needs a hosted" not in p.stderr or mp.exists():
            errors.append(f"no hosted main: rc={p.returncode} map left={mp.exists()} stderr={p.stderr.strip()!r}")

    if errors:
        print("coverage C emission: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage C emission: OK ({len(progs)} programs: map-mode map, prelude = map, one hit per counter, "
          "C minus coverage = off, stub-runtime counts = expected-counts.json; f0 golden; no-hosted-main refused)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
