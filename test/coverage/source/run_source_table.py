#!/usr/bin/env python3
"""Source-unit table tests for lib/coverage_source.sv0 (sv0cov CV-105).

Builds temporary projects (multi-file, same basename, multibyte paths and
text, CRLF without a final newline, a normalized-path collision, a root
escape), generates an sv0 program that prints the table and position queries,
runs it on native and VM (lib/sha256.sv0 + lib/coverage_source.sv0 prepended,
as in the mega-TU), and compares the output with this file's Python
reference: logical-path normalization, hashlib digests, the link concat rule
(files joined with one LF), LF-split lines, and Unicode-scalar columns.

    python3 sv0c/test/coverage/source/run_source_table.py [--backend native|vm|both]
"""

from __future__ import annotations

import argparse
import hashlib
import re
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
SV0 = ROOT / "scripts" / "sv0"
LIBS = (SV0C / "lib" / "sha256.sv0", SV0C / "lib" / "coverage_source.sv0")


# --- Python reference -------------------------------------------------------------


def ref_logical(root: str, path: str) -> str:
    if root:
        prefix = root if root.endswith("/") else root + "/"
        if path.startswith(prefix):
            rest = path[len(prefix):]
        elif root != ".":
            return ""
        else:
            rest = path
    else:
        rest = path
    if not rest or rest.startswith("/") or re.match(r"[A-Za-z]:", rest) or any(c in rest for c in "\\\n\r\t"):
        return ""
    parts: list[str] = []
    for comp in rest.split("/"):
        if comp in ("", "."):
            continue
        if comp == "..":
            if not parts:
                return ""
            parts.pop()
        else:
            parts.append(comp)
    out = "/".join(parts)
    return out if parts and len(out.encode()) <= 4096 else ""


def ref_position(data: bytes, off: int) -> tuple[int, int]:
    if off < 0 or off > len(data) or (off < len(data) and data[off] & 0xC0 == 0x80):
        return 0, 0
    line_start = data.rfind(b"\n", 0, off) + 1
    return data.count(b"\n", 0, off) + 1, 1 + len(data[line_start:off].decode("utf-8"))


def ref_table(root: str, listing: list[str]) -> tuple[str, list[tuple[int, int]], list[bytes]]:
    rows, spans, datas, seen = [], [], [], set()
    nxt = 0
    for host in listing:
        logical = ref_logical(root, host)
        if not logical:
            return f"\ninvalid-path\t{host}", [], []
        if logical in seen:
            return f"\ncollision\t{logical}", [], []
        seen.add(logical)
        data = Path(host).read_bytes()
        rows.append(f"{logical}\t{hashlib.sha256(data).hexdigest()}")
        spans.append((nxt, len(data)))
        datas.append(data)
        nxt += len(data) + 1
    return "\n".join(rows), spans, datas


# --- cases -------------------------------------------------------------------------


def sv0_str(s: str) -> str:
    return '"' + s.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n").replace("\t", "\\t") + '"'


def build_cases(base: Path) -> list[dict]:
    def write(rel: str, data: bytes) -> str:
        p = base / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(data)
        return str(p)

    main = b"fn main() -> i32 {\n  return 0;\n}\n"
    cases = []
    r = str(base / "multi")
    cases.append({"name": "multi-file", "root": r, "listing": [write("multi/a.sv0", main), write("multi/b.sv0", b"fn b() -> i32 {\n  return 1;\n}\n"),
                                                               write("multi/sub/c.sv0", b"fn c() -> i32 {\n  return 2;\n}")]})
    r = str(base / "same")
    cases.append({"name": "same-basename", "root": r, "listing": [write("same/x/util.sv0", b"module x;\n"), write("same/y/util.sv0", b"module y;\n")]})
    r = str(base / "mb")
    text = "fn main() -> i32 {\n  let s = \"\u00e9\u65e5\u672c\U0001F600\";\n  return 0;\n}\n".encode()
    cases.append({"name": "multibyte", "root": r, "listing": [write("mb/\u00e9.sv0", text), write("mb/\u65e5\u672c/\u8a9e.sv0", "/* \u00df */\nfn f() -> i32 { return 0; }\n".encode())]})
    r = str(base / "crlf")
    cases.append({"name": "crlf", "root": r, "listing": [write("crlf/w.sv0", b"fn main() -> i32 {\r\n  return 0;\r\n}")]})
    r = str(base / "col")
    a = write("col/a.sv0", main)
    cases.append({"name": "collision", "root": r, "listing": [a, str(base / "col" / "." / "a.sv0")]})
    r = str(base / "esc")
    write("esc/in.sv0", main)
    write("outside.sv0", main)
    cases.append({"name": "escape", "root": r, "listing": [str(base / "esc" / "in.sv0"), str(base / "esc" / ".." / "outside.sv0")]})
    return cases


NORMALIZE = [
    ("proj", "proj/a.sv0"), ("proj", "proj/./a.sv0"), ("proj", "proj/sub/../a.sv0"), ("proj", "proj//x//y.sv0"),
    ("proj/", "proj/a.sv0"), ("proj", "proj/../a.sv0"), ("proj", "other/a.sv0"), ("proj", "/abs/a.sv0"),
    (".", "a/b.sv0"), (".", "./a/./b.sv0"), (".", "C:x.sv0"), (".", "a\\b.sv0"), (".", "/a.sv0"),
    (".", "A.sv0"), (".", "\u00e9/\u65e5.sv0"), (".", ".."), (".", "."), ("proj", "proj/"),
]


def queries(spans: list[tuple[int, int]], datas: list[bytes]) -> list[int]:
    offs = set()
    for (start, length), data in zip(spans, datas):
        offs.update({start, start + length, start + length + 1, start + 1})
        for i, b in enumerate(data):
            if b in (0x0A, 0x0D) or b >= 0x80:
                offs.update({start + i, start + i + 1})
    offs.add(-1)
    return sorted(o for o in offs if o >= -1)


def expected(case: dict) -> tuple[str, str, list[int]]:
    """(table text + LF, query output lines, query offsets)."""
    table, spans, datas = ref_table(case["root"], case["listing"])
    lines: list[str] = []
    qs = queries(spans, datas) if spans else []
    for off in qs:
        unit = next((i for i, (s, n) in enumerate(spans) if s <= off <= s + n), -1)
        if unit < 0:
            lines.append(f"q {off} -1")
            continue
        fo = off - spans[unit][0]
        line, col = ref_position(datas[unit], fo)
        lines.append(f"q {off} {unit} {fo} {line} {col}")
    return table + "\n", "".join(line + "\n" for line in lines), qs


def program(cases: list[dict]) -> tuple[str, str]:
    body, want = [], []
    for root, path in NORMALIZE:
        body.append(f"  println(string_concat(\"n \", cov_src_logical_path({sv0_str(root)}, {sv0_str(path)})));")
        want.append(f"n {ref_logical(root, path)}\n")
    for k, case in enumerate(cases):
        table_text, query_lines, qs = expected(case)
        want.append(table_text)
        listing = "\n".join(case["listing"])
        body.append(f"  let st{k}: Vec<i32> = vec_new();")
        body.append(f"  let ln{k}: Vec<i32> = vec_new();")
        body.append(f"  let t{k}: string = cov_src_table_build({sv0_str(case['root'])}, {sv0_str(listing)}, st{k}, ln{k});")
        body.append(f"  println(t{k});")
        if qs:
            body.append(f"  let files{k}: string = {sv0_str(listing)};")
            body.append(f"  println(string_concat(\"c \", cov_src_digest(cov_src_concat(files{k}))));")
            concat = b"\n".join(Path(h).read_bytes() for h in case["listing"])
            want.append(f"c {hashlib.sha256(concat).hexdigest()}\n")
        want.append(query_lines)
        for off in qs:
            body.append(f"  cov_src_test_query(t{k}, files{k}, st{k}, ln{k}, {off});")
    helper = """
fn cov_src_test_query(table: string, listing: string, starts: Vec<i32>, lens: Vec<i32>, off: i32) -> i32 {
  let unit: i32 = cov_src_unit_at(starts, lens, off);
  let head: string = string_concat(string_concat("q ", cov_src_itoa(off)), " ");
  if unit < 0 {
    println(string_concat(head, "-1"));
    return 0;
  };
  let src: string = read_file(cov_src_field(listing, unit, 0));
  let fo: i32 = cov_src_file_offset(starts, unit, off);
  let mut out: string = string_concat(head, cov_src_itoa(unit));
  out = string_concat(string_concat(out, " "), cov_src_itoa(fo));
  out = string_concat(string_concat(out, " "), cov_src_itoa(cov_src_line(src, fo)));
  out = string_concat(string_concat(out, " "), cov_src_itoa(cov_src_column(src, fo)));
  println(out);
  return 0;
}
"""
    src = helper + "\nfn main() -> i32 {\n" + "\n".join(body) + "\n  return 0;\n}\n"
    return src, "".join(want)


def run(backend: str, src: str, tmp: Path) -> str:
    main = tmp / "main.sv0"
    main.write_bytes(b"".join(p.read_bytes() + b"\n" for p in LIBS) + src.encode())
    if backend == "native":
        exe = tmp / "t"
        c = subprocess.run([str(SV0), "native-compile", "-o", str(exe), str(main)], capture_output=True, text=True)
        if c.returncode:
            raise SystemExit(f"native compile failed:\n{c.stderr[-3000:]}")
        return subprocess.run([str(exe)], capture_output=True, check=True).stdout.decode()
    sv0b = tmp / "t.sv0b"
    c = subprocess.run([str(SV0), "vm-native-compile", str(main), str(sv0b)], capture_output=True, text=True)
    if c.returncode:
        raise SystemExit(f"vm compile failed:\n{c.stderr[-3000:]}")
    out = subprocess.run([str(SV0), "vm-run", str(sv0b)], capture_output=True).stdout.decode("utf-8", "replace")
    m = re.search(r"SV0VM_RUN_BEGIN\n(.*?)(?:val exitCode = |vm_exit:|\Z)", out, re.S)
    return m.group(1) if m else out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--backend", choices=("native", "vm", "both"), default="both")
    args = ap.parse_args()
    status = 0
    with tempfile.TemporaryDirectory() as t:
        tmp = Path(t).resolve()
        cases = build_cases(tmp / "p")
        src, want = program(cases)
        for backend in (("native", "vm") if args.backend == "both" else (args.backend,)):
            got = run(backend, src, tmp)
            if got == want:
                print(f"{backend}: source table OK ({len(cases)} projects, {want.count(chr(10) + 'q ') + want.startswith('q ')} queries, {len(NORMALIZE)} paths)")
            else:
                status = 1
                gl, wl = got.splitlines(), want.splitlines()
                diff = next((i for i in range(max(len(gl), len(wl))) if (gl[i:i + 1] or [None]) != (wl[i:i + 1] or [None])), None)
                print(f"{backend}: MISMATCH at output line {diff}: got {gl[diff] if diff is not None and diff < len(gl) else None!r}, want {wl[diff] if diff is not None and diff < len(wl) else None!r}", file=sys.stderr)
    return status


if __name__ == "__main__":
    sys.exit(main())
