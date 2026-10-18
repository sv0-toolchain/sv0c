#!/usr/bin/env python3
"""Attribute retention, placement and `coverage` validation (sv0cov CV-302;
sv0doc attributes/coverage.md; SPEC 13.4, COV-EXC-007, -009, -012, -014,
-015, -016).

Through the real native compiler and VM emitter (both run lib/attributes.sv0):

1. Snapshot: the attribute table of placement.sv0 (every legal place,
   misleading layouts included) is placement.attrs on both binaries: the
   order, spans, argument spans, run, position, frame, and the token each
   attribute attaches to.
2. Attributes change nothing else: placement.sv0 and the same program with
   its attributes removed give byte-identical C and bytecode, and both run to
   exit 0 on both backends.
3. Negative matrix: each misplaced or malformed `coverage` attribute fails
   the ordinary build (no --coverage) with exit 6 and exactly its
   error[COVxxxx], message, and line:col, on both binaries.
4. A valid `coverage` attribute builds in an ordinary build and, under
   --coverage=map, lands in the map as one exclusion record (sv0cov CV-305;
   the exclusion effect itself is test/coverage/exclusions).
5. Unchanged behavior: `#[extern_c]` declarations, other attributes before
   items and statements, and a dangling non-coverage attribute.

    python3 sv0c/test/attributes/run_attributes.py [--write]
"""

from __future__ import annotations

import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[1]
ROOT = SV0C.parent
SV0 = ROOT / "scripts" / "sv0"
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
VM_EMIT = ROOT / "build" / "sv0-megatu-vm-native"
sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built  # noqa: E402

M1 = "coverage attributes go only before an item or a statement; there is no expression attribute"
M2 = "this is not the coverage attribute namespace; write `coverage` exactly"
M3 = "the coverage attribute takes `(<mode>, reason = \"...\")`"
M4 = "unknown coverage mode; the modes are `off`, `branch_off` and `contract_off`"
M5 = "the coverage mode comes first, as a bare `off`, `branch_off` or `contract_off`"
M6 = "the coverage attribute needs `reason = \"...\"` after the mode"
M7 = "the coverage reason is empty"
M8 = "the second coverage argument must be `reason = \"...\"` with a string literal"
M9 = "the coverage reason is longer than 1024 bytes"
M10 = "the coverage attribute takes exactly two arguments, `<mode>` and `reason = \"...\"`"

R = 'reason = "r"'
LONG = "x" * 1025
# (label, source, code, line:col, message)
NEGATIVE = [
    ("expression operand", f"fn main() -> i32 {{\n    let a: i32 = 1 + #[coverage(off, {R})] 2;\n    return a - 3;\n}}\n", "COV1021", "2:22", M1),
    ("block tail", f"fn f() -> i32 {{\n    #[coverage(off, {R})] 1\n}}\nfn main() -> i32 {{ return f() - 1; }}\n", "COV1021", "2:5", M1),
    ("before a closing brace", f"fn main() -> i32 {{\n    return 0;\n    #[coverage(off, {R})]\n}}\n", "COV1021", "3:5", M1),
    ("before a lone semicolon", f"fn main() -> i32 {{\n    #[coverage(off, {R})] ;\n    return 0;\n}}\n", "COV1021", "2:5", M1),
    ("before else", f"fn main() -> i32 {{\n    if true {{ return 0; }}\n    #[coverage(off, {R})] else {{ return 1; }};\n    return 2;\n}}\n", "COV1021", "3:5", M1),
    ("match arm", f"fn main() -> i32 {{\n    return match 1 {{\n        #[coverage(off, {R})] 1 => 0,\n        _ => 1,\n    }};\n}}\n", "COV1021", "3:9", M1),
    ("struct field", f"struct S {{\n    #[coverage(off, {R})] a: i32,\n}}\nfn main() -> i32 {{ return 0; }}\n", "COV1021", "2:5", M1),
    ("enum variant", f"enum E {{\n    #[coverage(off, {R})] A,\n    B,\n}}\nfn main() -> i32 {{ return 0; }}\n", "COV1021", "2:5", M1),
    ("parameter", f"fn f(#[coverage(off, {R})] a: i32) -> i32 {{ return a; }}\nfn main() -> i32 {{ return f(0); }}\n", "COV1021", "1:6", M1),
    ("call argument", f"fn f(a: i32) -> i32 {{ return a; }}\nfn main() -> i32 {{ return f(#[coverage(off, {R})] 0); }}\n", "COV1021", "2:29", M1),
    ("struct literal field", f"struct S {{ a: i32 }}\nfn main() -> i32 {{\n    let s: S = S {{ #[coverage(off, {R})] a: 0 }};\n    return s.a;\n}}\n", "COV1021", "3:20", M1),
    ("end of file", f"fn main() -> i32 {{ return 0; }}\n#[coverage(off, {R})]\n", "COV1021", "2:1", M1),
    ("case variant", f"#[Coverage(off, {R})]\nfn main() -> i32 {{ return 0; }}\n", "COV3201", "1:3", M2),
    ("upper case", f"#[COVERAGE(off, {R})]\nfn main() -> i32 {{ return 0; }}\n", "COV3201", "1:3", M2),
    ("tool name", f"#[sv0cov(off, {R})]\nfn main() -> i32 {{ return 0; }}\n", "COV3201", "1:3", M2),
    ("prefixed", f"#[sv0::coverage(off, {R})]\nfn main() -> i32 {{ return 0; }}\n", "COV3201", "1:3", M2),
    ("no arguments", "#[coverage]\nfn main() -> i32 { return 0; }\n", "COV3201", "1:1", M3),
    ("value form", '#[coverage = "off"]\nfn main() -> i32 { return 0; }\n', "COV3201", "1:1", M3),
    ("empty list", "#[coverage()]\nfn main() -> i32 { return 0; }\n", "COV3201", "1:1", M5),
    ("reason first", f"#[coverage({R}, off)]\nfn main() -> i32 {{ return 0; }}\n", "COV3201", "1:12", M5),
    ("mode with a list", f"#[coverage(off(x), {R})]\nfn main() -> i32 {{ return 0; }}\n", "COV3201", "1:12", M5),
    ("differently cased mode", f"#[coverage(Off, {R})]\nfn main() -> i32 {{ return 0; }}\n", "COV3201", "1:12", M4),
    ("unknown mode", f"#[coverage(region_off, {R})]\nfn main() -> i32 {{ return 0; }}\n", "COV3201", "1:12", M4),
    ("no reason", "#[coverage(off)]\nfn main() -> i32 { return 0; }\n", "COV3207", "1:1", M6),
    ("empty reason", '#[coverage(off, reason = "")]\nfn main() -> i32 { return 0; }\n', "COV3207", "1:26", M7),
    ("another argument name", '#[coverage(off, why = "r")]\nfn main() -> i32 { return 0; }\n', "COV3201", "1:17", M8),
    ("reason not a string", "#[coverage(off, reason = 3)]\nfn main() -> i32 { return 0; }\n", "COV3201", "1:17", M8),
    ("mode twice", "#[coverage(off, off)]\nfn main() -> i32 { return 0; }\n", "COV3201", "1:17", M8),
    ("reason twice", f'#[coverage(off, {R}, reason = "s")]\nfn main() -> i32 {{ return 0; }}\n', "COV3201", "1:31", M10),
    ("reason too long", f'#[coverage(off, reason = "{LONG}")]\nfn main() -> i32 {{ return 0; }}\n', "COV3201", "1:26", M9),
]


def run(binary: Path, src: Path, cov: str = "") -> subprocess.CompletedProcess:
    env = {k: v for k, v in os.environ.items() if not k.startswith("SV0")}
    env["SV0_DRV_REQUEST"] = str(src)
    if cov:
        env["SV0_COVERAGE_REQUEST"] = cov
    argv = [str(binary), str(src)] if binary == NATIVE else [str(binary)]
    return subprocess.run(argv, capture_output=True, env=env, timeout=300)


def strip_attributes(text: str) -> str:
    return re.sub(r"#\[coverage\([^\]]*\)\]", "", text)


def main() -> int:
    ensure_built()
    errors: list[str] = []
    write = "--write" in sys.argv
    with tempfile.TemporaryDirectory() as tmp:
        t = Path(tmp)
        placement = HERE / "placement.sv0"
        # 1. snapshot, on both binaries
        dumps = []
        for binary in (NATIVE, VM_EMIT):
            p = run(binary, placement, "attr-dump\n/unused")
            if p.returncode:
                errors.append(f"attr-dump on {binary.name}: exit {p.returncode}: {p.stderr.decode()[-300:]}")
            dumps.append(p.stdout.decode())
        golden = HERE / "placement.attrs"
        if write:
            golden.write_text(dumps[0])
        if dumps[0] != golden.read_text():
            errors.append("placement.attrs: the native attribute table differs from the snapshot")
        if dumps[1] != dumps[0]:
            errors.append("placement.attrs: the VM emitter's attribute table differs from the native compiler's")
        if len([line for line in dumps[0].splitlines() if line.startswith("attr\t")]) != placement.read_text().count("#[coverage("):
            errors.append("placement.attrs: not one record per attribute")
        # 2. attributes change nothing else
        plain = t / "plain.sv0"
        plain.write_text(strip_attributes(placement.read_text()))
        c_with, c_without = run(NATIVE, placement), run(NATIVE, plain)
        if c_with.returncode or c_without.returncode or c_with.stdout != c_without.stdout or not c_with.stdout:
            errors.append(f"generated C differs with and without attributes (exit {c_with.returncode}/{c_without.returncode})")
        bytecode = []
        for src in (placement, plain):
            out = t / f"{src.stem}.sv0b"
            p = subprocess.run([str(SV0), "vm-native-compile", str(src), str(out)], capture_output=True, text=True, timeout=300)
            bytecode.append(out.read_bytes() if out.is_file() else b"")
            r = subprocess.run([str(SV0), "vm-run", str(out)], capture_output=True, text=True, timeout=300)
            if "vm_exit:0" not in r.stdout + r.stderr:
                errors.append(f"{src.name}: the VM run does not exit 0")
            exe = t / src.stem
            subprocess.run([str(SV0), "native-compile", "-o", str(exe), str(src)], capture_output=True, timeout=300)
            if not exe.exists() or subprocess.run([str(exe)], timeout=60).returncode != 0:
                errors.append(f"{src.name}: the native program does not exit 0")
        if not bytecode[0] or bytecode[0] != bytecode[1]:
            errors.append("bytecode differs with and without attributes")
        # 3. negative matrix, both binaries, ordinary build
        for label, text, code, where, message in NEGATIVE:
            src = t / "neg.sv0"
            src.write_text(text)
            for binary in (NATIVE, VM_EMIT):
                p = run(binary, src)
                err = p.stderr.decode()
                want = f"error[{code}]: {message}"
                if p.returncode != 6 or want not in err or f"--> {where}" not in err or err.count("error[") != 1:
                    errors.append(f"{label} ({binary.name}): exit {p.returncode}, want 6 with {code} at {where}; got {err.strip()[:300]!r}")
        # 4. a valid attribute is one exclusion record under --coverage=map
        ok = t / "ok.sv0"
        ok.write_text(f"#[coverage(off, {R})]\nfn f() -> i32 {{ return 1; }}\nfn main() -> i32 {{ return f() - 1; }}\n")
        for binary in (NATIVE, VM_EMIT):
            mp = t / f"m-{binary.name}.json"
            p = run(binary, ok, f"map\n{mp}\nok\nsv0c+test")
            xs = json.loads(mp.read_bytes())["exclusions"] if mp.exists() else None
            if p.returncode or not xs or [(x["mode"], x["origin"], x["reason"]) for x in xs] != [("off", "attribute", "r")]:
                errors.append(f"--coverage=map with a coverage attribute ({binary.name}): exit {p.returncode}, {xs}, {p.stderr.decode()[:200]!r}")
            if run(binary, ok).returncode != 0:
                errors.append(f"a valid coverage attribute fails the ordinary build ({binary.name})")
        # 5. unchanged behavior
        cases = {
            "extern_c": ('#[extern_c]\nfn abs(x: i32) -> i32;\nfn main() -> i32 { return abs(0); }\n', 0),
            "other attributes": ('#[inline]\nfn f() -> i32 { return 1; }\nfn main() -> i32 {\n    #[allow(x)]\n    let a: i32 = f();\n    return a - 1;\n}\n', 0),
            "dangling non-coverage attribute": ("fn main() -> i32 { return 0; }\n#[bogus]\n", 0),
        }
        for label, (text, want) in cases.items():
            src = t / "keep.sv0"
            src.write_text(text)
            p = run(NATIVE, src)
            if p.returncode != want:
                errors.append(f"{label}: exit {p.returncode}, want {want}: {p.stderr.decode()[:200]!r}")
    if errors:
        print("attributes: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"attributes: OK (placement snapshot of {placement.read_text().count('#[coverage(')} attributes on both binaries; "
          f"C and bytecode unchanged by attributes; {len(NEGATIVE)} negative cases x 2 binaries; one exclusion record under "
          f"--coverage=map; extern_c and other attributes unchanged)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
