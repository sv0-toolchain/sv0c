#!/usr/bin/env python3
"""Coverage comment directives: the pre-lex scan (sv0cov CV-304; SPEC 19.3,
19.4; sv0doc attributes/coverage.md section 6; COV-EXC-002, -003, -004).

Through the real native compiler and VM emitter (both run lib/directives.sv0):

1. Snapshot: the directive table of placement.sv0 (every legal shape, next to
   strings, character literals and comments that only look like directives)
   is placement.dirs on both binaries: kind, comment span, reason, the
   start/end pairing, and where the code after an ignore-next begins.
2. Hostile text: strings (escaped quotes, several lines), character
   literals, block comments (nested too) and prose that mention `sv0cov:`
   give an empty table and build; tabs, trailing spaces and CRLF are allowed.
3. Directives change nothing else: placement.sv0 and the same program with
   its directives made ordinary comments give byte-identical C and bytecode,
   and both exit 0 on both backends.
4. Negative matrix: each malformed, misplaced or unpaired directive fails the
   ordinary build (no --coverage) with exit 6 and exactly its
   error[COVxxxx], message and line:col, on both binaries.
5. Until exclusions are implemented (sv0cov CV-305), a valid directive is
   refused under --coverage=map (COV1021), never ignored; no map is written.
6. Project mode: each file is its own unit; a region left open in one file
   and closed in another fails in both, with the file's path; directives in
   two files are dumped with their unit.

    python3 sv0c/test/directives/run_directives.py [--write]
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
sys.dont_write_bytecode = True
from run_plan import ensure_built  # noqa: E402

M = {
    1: "a `// sv0cov:` directive must be a comment on a line of its own",
    2: "write a coverage directive exactly as `// sv0cov: <action> reason=\"...\"` (lower case, one space after `//`)",
    3: "unknown coverage directive action; the actions are `ignore-next`, `ignore-start` and `ignore-end`",
    4: "`ignore-end` takes nothing after it",
    5: "the coverage directive needs `reason=\"...\"` after the action",
    6: "the coverage directive's reason must be written `reason=\"...\"`",
    7: "the coverage directive's reason string is not closed on its line",
    8: "the coverage directive's reason is empty",
    9: "the coverage directive's reason is longer than 1024 bytes",
    10: "nothing may follow the reason of a coverage directive",
    11: "this directive comes between a `sv0cov: ignore-next` and the construct it excludes",
    12: "this directive is inside a `sv0cov: ignore-start` region, which already excludes everything up to its `ignore-end`",
    13: "`ignore-end` without an `ignore-start` before it in this file",
    14: "this `ignore-start` is never closed by an `ignore-end` in its file",
    15: "nothing follows this `ignore-next` in its file",
    16: "a coverage directive must be a `//` line comment",
    17: "coverage exclusions are not implemented by this compiler yet (sv0cov CV-305); build without --coverage, or remove the directive",
}
NEXT = '// sv0cov: ignore-next reason="r"'
START = '// sv0cov: ignore-start reason="r"'
END = "// sv0cov: ignore-end"


def prog(body: str) -> str:
    return "fn main() -> i32 {\n" + body + "    return 0;\n}\n"


# (label, source, code, the text the error points at (its first occurrence), message id)
NEGATIVE = [
    ("after code", prog(f"    let a: i32 = 1; {NEXT}\n"), "COV3201", NEXT, 1),
    ("after a block comment", prog(f"    /* c */ {NEXT}\n    let a: i32 = 1;\n"), "COV3201", NEXT, 1),
    ("block comment", prog('    /* sv0cov: ignore-next reason="r" */\n    let a: i32 = 1;\n'), "COV3201", "/* sv0cov", 16),
    ("block comment, upper case", prog('    /** SV0COV: ignore-start */\n'), "COV3201", "/** SV0COV", 16),
    ("doc comment", prog('    /// sv0cov: ignore-next reason="r"\n    let a: i32 = 1;\n'), "COV3201", "///", 2),
    ("no space", prog('    //sv0cov: ignore-next reason="r"\n    let a: i32 = 1;\n'), "COV3201", "//sv0cov", 2),
    ("two spaces", prog('    //  sv0cov: ignore-next reason="r"\n    let a: i32 = 1;\n'), "COV3201", "//  sv0cov", 2),
    ("upper case", prog('    // SV0COV: ignore-next reason="r"\n    let a: i32 = 1;\n'), "COV3201", "// SV0COV", 2),
    ("no space after the colon", prog('    // sv0cov:ignore-next reason="r"\n    let a: i32 = 1;\n'), "COV3201", "// sv0cov", 2),
    ("no action", prog("    // sv0cov:\n    let a: i32 = 1;\n"), "COV3201", "// sv0cov", 3),
    ("unknown action", prog('    // sv0cov: ignore reason="r"\n    let a: i32 = 1;\n'), "COV3201", "ignore reason", 3),
    ("differently cased action", prog('    // sv0cov: Ignore-Next reason="r"\n    let a: i32 = 1;\n'), "COV3201", "Ignore-Next", 3),
    ("a synonym", prog('    // sv0cov: off reason="r"\n    let a: i32 = 1;\n'), "COV3201", "off", 3),
    ("end with a reason", prog('    // sv0cov: ignore-end reason="r"\n'), "COV3201", "reason", 4),
    ("no reason", prog("    // sv0cov: ignore-next\n    let a: i32 = 1;\n"), "COV3207", "// sv0cov", 5),
    ("spaces around =", prog('    // sv0cov: ignore-next reason = "r"\n    let a: i32 = 1;\n'), "COV3201", "reason =", 6),
    ("unquoted reason", prog("    // sv0cov: ignore-next reason=r\n    let a: i32 = 1;\n"), "COV3201", "reason=r", 6),
    ("another name", prog('    // sv0cov: ignore-next why="r"\n    let a: i32 = 1;\n'), "COV3201", "why", 6),
    ("reason first", prog('    // sv0cov: reason="r" ignore-next\n    let a: i32 = 1;\n'), "COV3201", 'reason="r" ignore', 3),
    ("unclosed reason", prog('    // sv0cov: ignore-next reason="r\n    let a: i32 = 1;\n'), "COV3201", 'reason="r\n', 7),
    ("reason closed only by an escape", prog('    // sv0cov: ignore-next reason="r\\"\n    let a: i32 = 1;\n'), "COV3201", "reason=", 7),
    ("empty reason", prog('    // sv0cov: ignore-next reason=""\n    let a: i32 = 1;\n'), "COV3207", 'reason=""', 8),
    ("reason too long", prog(f'    // sv0cov: ignore-next reason="{"x" * 1025}"\n    let a: i32 = 1;\n'), "COV3201", "xxx", 9),
    ("text after the reason", prog('    // sv0cov: ignore-next reason="r" extra\n    let a: i32 = 1;\n'), "COV3201", "extra", 10),
    ("two reasons", prog('    // sv0cov: ignore-next reason="r" reason="s"\n    let a: i32 = 1;\n'), "COV3201", 'reason="s"', 10),
    ("ignore-next twice", prog(f"    {NEXT}\n    // sv0cov: ignore-next reason=\"s\"\n    let a: i32 = 1;\n"), "COV3203", '// sv0cov: ignore-next reason="s"', 11),
    ("ignore-next before a start", prog(f"    {NEXT}\n    {START}\n    let a: i32 = 1;\n"), "COV3203", "// sv0cov: ignore-start", 11),
    ("ignore-next before an end", prog(f"    {NEXT}\n    {END}\n    let a: i32 = 1;\n"), "COV3203", END, 11),
    ("start inside a region", prog(f"    {START}\n    // sv0cov: ignore-start reason=\"s\"\n    let a: i32 = 1;\n    {END}\n"), "COV3203", '// sv0cov: ignore-start reason="s"', 12),
    ("ignore-next inside a region", prog(f"    {START}\n    {NEXT}\n    let a: i32 = 1;\n    {END}\n"), "COV3203", "// sv0cov: ignore-next", 12),
    ("end without a start", prog(f"    let a: i32 = 1;\n    {END}\n"), "COV3201", END, 13),
    ("end twice", prog(f"    {START}\n    let a: i32 = 1;\n    {END}\n    {END}\n"), "COV3201", f"{END}\n    return", 13),
    ("start never closed", prog(f"    {START}\n    let a: i32 = 1;\n"), "COV3201", START, 14),
    ("ignore-next at the end of the file", f"fn main() -> i32 {{ return 0; }}\n{NEXT}\n", "COV1021", NEXT, 15),
    ("ignore-next before comments only", f"fn main() -> i32 {{ return 0; }}\n{NEXT}\n// a comment\n/* another */\n", "COV1021", NEXT, 15),
]

# Programs whose `sv0cov:` text is not a directive, or is a well-formed one.
HOSTILE = {
    "a string over several lines": 'fn main() -> i32 {\n    let s: string = "a\n// sv0cov: ignore-start reason=\\"x\\"\nb";\n    let n: i32 = string_len(s);\n    return 0;\n}\n',
    "escaped quotes in a string": 'fn main() -> i32 {\n    let s: string = "\\\\\\" // sv0cov: ignore-next";\n    let n: i32 = string_len(s);\n    return 0;\n}\n',
    "character literals": "fn main() -> i32 {\n    let a: char = '/'; let b: char = '/'; let c: char = '\"';\n    return 0;\n}\n",
    "a nested block comment": 'fn main() -> i32 {\n    /* outer /* inner */\n    // sv0cov: ignore-start reason="still a comment"\n    */\n    return 0;\n}\n',
    "prose": "// sv0cov, the coverage tool\n// note: sv0cov: comments are directives\nfn main() -> i32 { return 0; }\n",
}
POSITIVE = {
    "trailing spaces and tabs": (f"fn main() -> i32 {{\n    {NEXT}  \t\n    let a: i32 = 1;\n    {START}\t\n    let b: i32 = 2;\n    {END}   \n    return a + b - 3;\n}}\n", 3),
    "CRLF line ends": (f"fn main() -> i32 {{\r\n    {NEXT}\r\n    let a: i32 = 1;\r\n    {START}\r\n    {END}\r\n    return a - 1;\r\n}}\r\n", 3),
    "a reason of 1024 bytes": (prog(f'    // sv0cov: ignore-next reason="{"x" * 1024}"\n    let a: i32 = 1;\n'), 1),
    "UTF-8 in the reason": (prog('    // sv0cov: ignore-next reason="日本語の理由"\n    let a: i32 = 1;\n'), 1),
}


def run(binary: Path, request: str, cov: str = "") -> subprocess.CompletedProcess:
    env = {k: v for k, v in os.environ.items() if not k.startswith("SV0")}
    env["SV0_DRV_REQUEST"] = request
    if cov:
        env["SV0_COVERAGE_REQUEST"] = cov
    argv = [str(binary)] if binary == VM_EMIT else [str(binary), *request.split(" ", 1)]
    return subprocess.run(argv, capture_output=True, env=env, timeout=300)


def line_col(text: str, at: str) -> str:
    pos = text.index(at)
    return f"{text.count(chr(10), 0, pos) + 1}:{pos - text.rfind(chr(10), 0, pos)}"


def plain(text: str) -> str:
    """The same program with every directive line made an ordinary comment."""
    return "".join(ln.replace("// sv0cov:", "// coverage", 1) if ln.lstrip().startswith("// sv0cov:") else ln
                   for ln in text.splitlines(keepends=True))


def main() -> int:
    ensure_built()
    errors: list[str] = []
    reg = json.loads((ROOT / "sv0doc" / "attributes" / "coverage-attribute.json").read_text())["directives"]
    if (reg["actions"] != ["ignore-next", "ignore-start", "ignore-end"] or reg["prefix"] != "// sv0cov:"
            or reg["reason"]["max_bytes"] != 1024 or reg["end_form"] != END):
        errors.append("sv0doc coverage-attribute.json: the directive grammar differs from what this suite tests")
    write = "--write" in sys.argv
    placement = HERE / "placement.sv0"
    golden = HERE / "placement.dirs"
    with tempfile.TemporaryDirectory() as tmp:
        t = Path(tmp)
        # 1. snapshot, on both binaries
        dumps = []
        for binary in (NATIVE, VM_EMIT):
            p = run(binary, str(placement), "dir-dump\n/unused")
            if p.returncode:
                errors.append(f"dir-dump on {binary.name}: exit {p.returncode}: {p.stderr.decode()[-300:]}")
            dumps.append(p.stdout.decode())
        if write:
            golden.write_text(dumps[0])
        if dumps[0] != golden.read_text():
            errors.append("placement.dirs: the native directive table differs from the snapshot")
        if dumps[1] != dumps[0]:
            errors.append("placement.dirs: the VM emitter's directive table differs from the native compiler's")
        text = placement.read_text()
        outside = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
        directive_lines = [ln for ln in outside.splitlines() if ln.lstrip().startswith("// sv0cov:")]
        if len(dumps[0].splitlines()) != len(directive_lines):
            errors.append("placement.dirs: not one record per directive line")
        # 2. hostile text and well-formed variants
        for label, src_text in HOSTILE.items():
            src = t / "hostile.sv0"
            src.write_text(src_text)
            for binary in (NATIVE, VM_EMIT):
                d = run(binary, str(src), "dir-dump\n/unused")
                if d.returncode or d.stdout:
                    errors.append(f"hostile {label} ({binary.name}): exit {d.returncode}, table {d.stdout.decode()!r}")
            exe = t / "hostile"
            subprocess.run([str(SV0), "native-compile", "-o", str(exe), str(src)], capture_output=True, timeout=300)
            if not exe.exists() or subprocess.run([str(exe)], timeout=60).returncode != 0:
                errors.append(f"hostile {label}: the native program does not build and exit 0")
            exe.unlink(missing_ok=True)
        for label, (src_text, count) in POSITIVE.items():
            src = t / "positive.sv0"
            src.write_bytes(src_text.encode())
            for binary in (NATIVE, VM_EMIT):
                d = run(binary, str(src), "dir-dump\n/unused")
                if d.returncode or len(d.stdout.decode().splitlines()) != count:
                    errors.append(f"{label} ({binary.name}): exit {d.returncode}, want {count} records: {d.stdout.decode()[:200]!r} {d.stderr.decode()[:200]!r}")
                if run(binary, str(src)).returncode != 0:
                    errors.append(f"{label} ({binary.name}): the ordinary build fails")
        # 3. directives change nothing else
        ordinary = t / "plain.sv0"
        ordinary.write_text(plain(text))
        c_with, c_without = run(NATIVE, str(placement)), run(NATIVE, str(ordinary))
        if c_with.returncode or c_without.returncode or c_with.stdout != c_without.stdout or not c_with.stdout:
            errors.append(f"generated C differs with and without directives (exit {c_with.returncode}/{c_without.returncode})")
        bytecode = []
        for src in (placement, ordinary):
            out = t / f"{src.stem}.sv0b"
            subprocess.run([str(SV0), "vm-native-compile", str(src), str(out)], capture_output=True, text=True, timeout=300)
            bytecode.append(out.read_bytes() if out.is_file() else b"")
            r = subprocess.run([str(SV0), "vm-run", str(out)], capture_output=True, text=True, timeout=300)
            if "vm_exit:0" not in r.stdout + r.stderr:
                errors.append(f"{src.name}: the VM run does not exit 0")
            exe = t / src.stem
            subprocess.run([str(SV0), "native-compile", "-o", str(exe), str(src)], capture_output=True, timeout=300)
            if not exe.exists() or subprocess.run([str(exe)], timeout=60).returncode != 0:
                errors.append(f"{src.name}: the native program does not exit 0")
        if not bytecode[0] or bytecode[0] != bytecode[1]:
            errors.append("bytecode differs with and without directives")
        # 4. negative matrix, both binaries, ordinary build
        for label, src_text, code, at, mid in NEGATIVE:
            src = t / "neg.sv0"
            src.write_text(src_text)
            where = line_col(src_text, at)
            for binary in (NATIVE, VM_EMIT):
                p = run(binary, str(src))
                err = p.stderr.decode()
                want = f"error[{code}]: {M[mid]}"
                if p.returncode != 6 or want not in err or f"--> {where}\n" not in err or err.count("error[") != 1:
                    errors.append(f"{label} ({binary.name}): exit {p.returncode}, want 6 with {code} at {where}; got {err.strip()[:300]!r}")
        # 5. a valid directive is refused under --coverage until CV-305
        ok = t / "ok.sv0"
        ok.write_text(prog(f"    {NEXT}\n    let a: i32 = 1;\n"))
        for binary in (NATIVE, VM_EMIT):
            p = run(binary, str(ok), f"map\n{t / 'm.json'}\nok\nsv0c+test")
            if p.returncode != 6 or f"error[COV1021]: {M[17]}" not in p.stderr.decode() or "--> 2:5\n" not in p.stderr.decode() or (t / "m.json").exists():
                errors.append(f"--coverage=map with a directive ({binary.name}): exit {p.returncode}, {p.stderr.decode()[:200]!r}")
            if run(binary, str(ok)).returncode != 0:
                errors.append(f"a valid directive fails the ordinary build ({binary.name})")
        # 6. project mode: one unit per file
        proj = t / "proj"
        (proj / "lib").mkdir(parents=True)
        main_text = "use lib::helper;\nfn main() -> i32 {\n    // sv0cov: ignore-next reason=\"main\"\n    return helper() - 1;\n}\n"
        lib_text = "// sv0cov: ignore-next reason=\"lib\"\npub fn helper() -> i32 { return 1; }\n"
        (proj / "main.sv0").write_text(main_text)
        (proj / "lib" / "lib.sv0").write_text(lib_text)
        request = f"--project {proj}"
        tables = [run(binary, request, "dir-dump\n/unused") for binary in (NATIVE, VM_EMIT)]
        units = sorted(line.split("\t")[2] + line.split("\t")[6] for line in tables[0].stdout.decode().splitlines())
        if tables[0].returncode or tables[0].stdout != tables[1].stdout or len(units) != 2 or {u[0] for u in units} != {"0", "1"}:
            errors.append(f"project dir-dump: exits {tables[0].returncode}/{tables[1].returncode}, {tables[0].stdout.decode()!r} {tables[0].stderr.decode()[:300]!r}")
        if run(NATIVE, request).returncode != 0:
            errors.append("project with directives: the ordinary build fails")
        (proj / "lib" / "lib.sv0").write_text(lib_text + f"{START}\n")
        (proj / "main.sv0").write_text(main_text + f"{END}\n")
        for binary in (NATIVE, VM_EMIT):
            p = run(binary, request)
            err = p.stderr.decode()
            if (p.returncode != 6 or f"error[COV3201]: {M[14]}\n  --> {proj / 'lib' / 'lib.sv0'}:3:1\n" not in err
                    or f"error[COV3201]: {M[13]}\n  --> {proj / 'main.sv0'}:6:1\n" not in err):
                errors.append(f"region across files ({binary.name}): exit {p.returncode}: {err[:400]!r}")
    if errors:
        print("directives: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"directives: OK (placement snapshot of {len(directive_lines)} directives on both binaries; {len(HOSTILE)} hostile texts "
          f"and {len(POSITIVE)} well-formed variants; C and bytecode unchanged by directives; {len(NEGATIVE)} negative cases x 2 "
          f"binaries; refused under --coverage until CV-305; one unit per project file)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
