#!/usr/bin/env python3
"""Coverage planner tests: sources, function entities, function_entry points
(sv0cov CV-107, lib/coverage_plan.sv0).

The planner runs inside the compiler after resolve/check. Its internal
`plan-dump` mode (SV0_COVERAGE_REQUEST="plan-dump\\n...") prints the plan
instead of C. This script:

1. runs every hand-reviewed sv0cov semantic fixture
   (sv0cov/tests/fixtures/semantic/*) and compares the planned sources,
   entities, and function_entry points with the fixture's expected-map.json;
2. runs small synthetic programs (inherent methods, trait default methods,
   #[extern_c], `pub`, array parameters, contracts, `module` names, CRLF,
   tabs, multibyte text, project source order). Trait impls and bodyless
   trait methods do not pass the native checker yet, so they are not here against expectations
   anchored to their source text, with lines/columns from a Python
   reference (LF lines, Unicode-scalar columns);
3. requires the native compiler and the native VM emitter, which share the
   planner, to print identical plans (one plan for both backends);
4. checks the fail-closed paths: an `include`d file (the compiled source is
   not the file's bytes), an unknown mode, and map/instrument, which are
   still refused.

    python3 sv0c/test/coverage/plan/run_plan.py
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
FIXTURES = ROOT / "sv0cov" / "tests" / "fixtures" / "semantic"
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
VM_EMIT = ROOT / "build" / "sv0-megatu-vm-native"
BUILDS = ((ROOT / "build" / "sv0-megatu-native", "build-sv0-megatu-native.sh"),
          (VM_EMIT, "build-sv0-megatu-vm-native.sh"))


def ensure_built() -> None:
    inputs = list((SV0C / "lib").glob("*.sv0")) + [SV0C / "lib" / "megaTU-modules.list",
                                                   SV0C / "runtime" / "sv0_runtime.h"]
    newest = max(p.stat().st_mtime for p in inputs)
    for binary, script in BUILDS:
        script_path = ROOT / "scripts" / script
        if not binary.exists() or binary.stat().st_mtime < max(newest, script_path.stat().st_mtime):
            subprocess.run(["bash", str(script_path)], check=True, capture_output=True)


def run_plan(request: str, mode: str = "plan-dump") -> tuple[int, str, str, int, str]:
    """(native rc, native stdout, native stderr, vm rc, vm stdout)."""
    env = dict(os.environ, SV0_COVERAGE_REQUEST=f"{mode}\n/unused.sv0covmap.json", SV0_DRV_REQUEST=request)
    n = subprocess.run([str(NATIVE), *request.split(" ", 1)], capture_output=True, text=True, env=env, timeout=300)
    v = subprocess.run([str(VM_EMIT)], capture_output=True, env=env, timeout=300)
    return n.returncode, n.stdout, n.stderr, v.returncode, v.stdout.decode("utf-8", "replace")


def parse_dump(text: str) -> dict:
    out: dict = {"sources": [], "entities": [], "points": []}
    for line in text.splitlines():
        f = line.split("\t")
        if f[0] == "source":
            out["sources"].append({"source_index": int(f[1]), "path": f[2], "digest": f[3]})
        elif f[0] == "entity":
            out["entities"].append({"entity_index": int(f[1]), "kind": f[2], "qualified_name": f[3],
                                    "source_index": int(f[4]), "span": span(f[5:11]), "owner": int(f[11])})
        elif f[0] == "point":
            out["points"].append({"kind": f[2], "entity_index": int(f[3]), "semantic_discriminator": f[4],
                                  "source_index": int(f[5]), "span": span(f[6:12])})
        else:
            raise ValueError(f"unexpected dump line {line!r}")
    return out


def span(f: list[str]) -> dict:
    keys = ("start_byte", "end_byte", "start_line", "start_column", "end_line", "end_column")
    return dict(zip(keys, map(int, f)))


def ref_span(data: bytes, start: int, end: int) -> dict:
    def pos(off: int) -> tuple[int, int]:
        line_start = data.rfind(b"\n", 0, off) + 1
        col = sum(1 for b in data[line_start:off] if b < 0x80 or b >= 0xC0) + 1
        return data.count(b"\n", 0, off) + 1, col

    sl, sc = pos(start)
    el, ec = pos(end)
    return {"start_byte": start, "end_byte": end, "start_line": sl, "start_column": sc,
            "end_line": el, "end_column": ec}


def check_fixture(name: str, errors: list[str]) -> None:
    d = FIXTURES / name
    expected = json.loads((d / "expected-map.json").read_bytes())
    project = any(p.parent != d for p in d.rglob("*.sv0"))
    request = f"--project {d}" if project else str(d / "main.sv0")
    rc, out, err, vrc, vout = run_plan(request)
    if rc != 0 or vrc != 0:
        errors.append(f"{name}: plan-dump failed (native {rc}, vm {vrc}): {err.strip()}")
        return
    if out != vout:
        errors.append(f"{name}: the native compiler and the VM emitter planned differently")
    got = parse_dump(out)
    want_sources = [{k: s[k] for k in ("source_index", "path", "digest")} for s in expected["sources"]]
    if got["sources"] != want_sources:
        errors.append(f"{name}: sources {got['sources']} != {want_sources}")
    want_entities = [{"entity_index": e["entity_index"], "kind": e["kind"], "qualified_name": e["qualified_name"],
                      "source_index": e["source_index"], "span": e["span"], "owner": e["entity_index"]}
                     for e in expected["entities"]]
    if got["entities"] != want_entities:
        errors.append(f"{name}: entities differ:\n    got  {got['entities']}\n    want {want_entities}")
    want_points = sorted(
        ({"kind": p["kind"], "entity_index": p["entity_index"], "semantic_discriminator": p["semantic_discriminator"],
          "source_index": p["source_index"], "span": p["span"]}
         for p in expected["points"] if p["kind"] == "function_entry"),
        key=lambda p: p["entity_index"])
    if got["points"] != want_points:
        errors.append(f"{name}: function_entry points differ:\n    got  {got['points']}\n    want {want_points}")


# Synthetic programs: (name, files {path: text}, entry file or None for a
# project, [(qualified name, file, start anchor, end anchor)]). A span starts
# at its start anchor and ends after the first following end anchor.
SYNTHETIC = [
    ("methods-and-traits", {"main.sv0": (
        "/* café ☕ — multibyte text before the first item */\n"
        "struct Pt { x: i32 }\n\n"
        "trait Area {\n    fn twice(self: Pt) -> i32 { return 2; }\n}\n\n"
        "impl Area for Pt { }\n\n"
        "impl Pt {\n    fn get(self: Pt) -> i32 { return self.x; }\n    fn bump(self: Pt) -> i32 {\n"
        "        return self.x + 1;\n    }\n}\n\n"
        "#[extern_c]\nfn abs(x: i32) -> i32;\n\n"
        "pub fn first(a: [i32; 3]) -> i32 { return a[0]; }\n\n"
        "fn checked(n: i32) -> i32 requires(n > 0) ensures(result > 0) { return n; }\n\n"
        "fn main() -> i32 {\n    let p: Pt = Pt { x: 2 };\n"
        "    return p.get() + p.bump() + first([1, 2, 3]) + checked(1) + abs(0) - 7;\n}\n")},
     "main.sv0",
     [("Area::twice", "main.sv0", "fn twice", "return 2; }"),
      ("Pt::get", "main.sv0", "fn get", "self.x; }"),
      ("Pt::bump", "main.sv0", "fn bump", "+ 1;\n    }"),
      ("first", "main.sv0", "fn first", "a[0]; }"),
      ("checked", "main.sv0", "fn checked", "return n; }"),
      ("main", "main.sv0", "fn main", "- 7;\n}")]),
    ("crlf-module", {"main.sv0": (
        "module shapes;\r\n\r\n"
        "fn side(n: i32) -> i32 {\r\n\treturn n;\r\n}\r\n\r\n"
        "fn main() -> i32 { return side(0); }")},
     "main.sv0",
     [("shapes::side", "main.sv0", "fn side", "n;\r\n}"),
      ("shapes::main", "main.sv0", "fn main", "side(0); }")]),
    ("project-order", {
        "zeta.sv0": "module zeta;\n\nfn z(x: i32) -> i32 { return x; }\n",
        "alpha/a.sv0": "module alpha;\n\nfn a1(x: i32) -> i32 { return x; }\n\nfn a2(x: i32) -> i32 { return x + 0; }\n",
        "main.sv0": "use zeta::z;\nuse alpha::a1;\nuse alpha::a2;\n\nfn main() -> i32 { return z(0) + a1(0) + a2(0); }\n"},
     None,
     [("alpha::a1", "alpha/a.sv0", "fn a1", "x; }"),
      ("alpha::a2", "alpha/a.sv0", "fn a2", "+ 0; }"),
      ("main", "main.sv0", "fn main", "a2(0); }"),
      ("zeta::z", "zeta.sv0", "fn z", "x; }")]),
]


def check_synthetic(case, tmp: Path, errors: list[str]) -> None:
    name, files, entry, expect = case
    d = tmp / name
    for rel, text in files.items():
        p = d / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(text.encode("utf-8"))
    request = str(d / entry) if entry else f"--project {d}"
    rc, out, err, vrc, vout = run_plan(request)
    if rc != 0 or vrc != 0:
        errors.append(f"{name}: plan-dump failed (native {rc}, vm {vrc}): {err.strip()}")
        return
    if out != vout:
        errors.append(f"{name}: the native compiler and the VM emitter planned differently")
    got = parse_dump(out)
    paths = sorted(files, key=lambda s: s.encode()) if entry is None else [entry]
    want_sources = [{"source_index": i, "path": p, "digest": __import__("hashlib").sha256(files[p].encode()).hexdigest()}
                    for i, p in enumerate(paths)]
    if got["sources"] != want_sources:
        errors.append(f"{name}: sources {got['sources']} != {want_sources}")
    want_entities, want_points = [], []
    for i, (qname, rel, start_anchor, end_anchor) in enumerate(expect):
        data = files[rel].encode("utf-8")
        start = data.index(start_anchor.encode())
        end = data.index(end_anchor.encode(), start) + len(end_anchor.encode())
        sp = ref_span(data, start, end)
        src = paths.index(rel)
        want_entities.append({"entity_index": i, "kind": "function", "qualified_name": qname,
                              "source_index": src, "span": sp, "owner": i})
        want_points.append({"kind": "function_entry", "entity_index": i, "semantic_discriminator": "entry",
                            "source_index": src, "span": sp})
    if got["entities"] != want_entities:
        errors.append(f"{name}: entities differ:\n    got  {got['entities']}\n    want {want_entities}")
    if got["points"] != want_points:
        errors.append(f"{name}: points differ:\n    got  {got['points']}\n    want {want_points}")


def check_refusals(tmp: Path, errors: list[str]) -> None:
    d = tmp / "refusals"
    d.mkdir()
    (d / "inc.sv0").write_text("fn helper() -> i32 { return 0; }\n")
    (d / "main.sv0").write_text('include "inc.sv0";\nfn main() -> i32 { return helper(); }\n')
    rc, _, err, vrc, _ = run_plan(str(d / "main.sv0"))
    # The VM emitter does not expand `include` at all, so it fails earlier.
    if rc != 9 or vrc == 0 or "compiled source differs" not in err:
        errors.append(f"include: expected refusal 9, got native {rc} vm {vrc}: {err.strip()}")
    (d / "plain.sv0").write_text("fn main() -> i32 { return 0; }\n")
    for mode, needle in (("bogus", "unknown mode"), ("map", "is not available yet"),
                         ("instrument", "is not available yet")):
        rc, out, err, vrc, _ = run_plan(str(d / "plain.sv0"), mode)
        if rc != 9 or vrc != 9 or needle not in err or out:
            errors.append(f"mode {mode}: expected refusal 9 with {needle!r}, got native {rc} vm {vrc}: {err.strip()}")


def main() -> int:
    ensure_built()
    errors: list[str] = []
    fixtures = sorted(p.parent.name for p in FIXTURES.glob("*/expected-map.json"))
    if len(fixtures) < 7:
        errors.append(f"expected the 7 sv0cov semantic fixtures under {FIXTURES}, found {fixtures}")
    for name in fixtures:
        check_fixture(name, errors)
    with tempfile.TemporaryDirectory() as td:
        for case in SYNTHETIC:
            check_synthetic(case, Path(td), errors)
        check_refusals(Path(td), errors)
    if errors:
        print("coverage plan: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage plan: OK ({len(fixtures)} sv0cov fixtures, {len(SYNTHETIC)} synthetic programs, "
          "native = VM emitter, include/unknown/map/instrument refused)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
