#!/usr/bin/env python3
"""Coverage planner tests: sources, function entities, function_entry points
(sv0cov CV-107, lib/coverage_plan.sv0).

The planner runs inside the compiler after resolve/check. Its internal
`plan-dump` mode (SV0_COVERAGE_REQUEST="plan-dump\\n...") prints the plan
instead of C. This script:

1. runs every hand-reviewed sv0cov semantic fixture
   (sv0cov/tests/fixtures/semantic/*) and compares the planned sources,
   entities, and function_entry points with the fixture's expected-map.json;
2. runs small synthetic programs (inherent methods, trait impl methods,
   trait default methods, bodyless trait methods (not entities),
   #[extern_c], `pub`, array parameters, contracts, `module` names, CRLF,
   tabs, multibyte text, project source order) against expectations
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
    return _run_both(request, env)


def run_map(request: str, out_dir: Path, target: str, identity: str) -> tuple[int, bytes, int, bytes, str]:
    """Build `request` in map mode on both binaries: (native rc, map, vm rc, map, stderr)."""
    maps = []
    rcs = []
    err = ""
    for binary in ("native", "vm"):
        path = out_dir / f"{binary}.sv0covmap.json"
        env = dict(os.environ, SV0_COVERAGE_REQUEST=f"map\n{path}\n{target}\n{identity}", SV0_DRV_REQUEST=request)
        if binary == "native":
            proc = subprocess.run([str(NATIVE), *request.split(" ", 1)], capture_output=True, text=True, env=env, timeout=300)
            err = proc.stderr
        else:
            proc = subprocess.run([str(VM_EMIT)], capture_output=True, env=env, timeout=300)
        rcs.append(proc.returncode)
        maps.append(path.read_bytes() if path.is_file() else b"")
    return rcs[0], maps[0], rcs[1], maps[1], err


def _run_both(request: str, env: dict) -> tuple[int, str, str, int, str]:
    n = subprocess.run([str(NATIVE), *request.split(" ", 1)], capture_output=True, text=True, env=env, timeout=300)
    v = subprocess.run([str(VM_EMIT)], capture_output=True, env=env, timeout=300)
    return n.returncode, n.stdout, n.stderr, v.returncode, v.stdout.decode("utf-8", "replace")


def parse_dump(text: str) -> dict:
    out: dict = {"sources": [], "entities": [], "points": [], "outcome_points": [], "branches": [], "regions": []}
    for line in text.splitlines():
        f = line.split("\t")
        if f[0] == "source":
            out["sources"].append({"source_index": int(f[1]), "path": f[2], "digest": f[3]})
        elif f[0] == "entity":
            out["entities"].append({"entity_index": int(f[1]), "kind": f[2], "qualified_name": f[3],
                                    "source_index": int(f[4]), "span": span(f[5:11]), "owner": int(f[11])})
        elif f[0] == "point":
            pt = {"kind": f[2], "entity_index": int(f[3]), "semantic_discriminator": f[4],
                  "source_index": int(f[5]), "span": span(f[6:12])}
            if f[2] == "branch_outcome":
                pt["outcome_ordinal"] = int(f[12])
                out["outcome_points"].append(pt)
            else:
                out["points"].append(pt)
        elif f[0] == "branch":
            out["branches"].append({"branch_index": int(f[1]), "kind": f[2], "entity_index": int(f[3]),
                                    "source_index": int(f[4]), "span": span(f[5:11]), "outcomes": int(f[11]),
                                    "names": f[12].split(",")})
        elif f[0] == "region":
            out["regions"].append({"region_index": int(f[1]), "kind": f[2], "entity_index": int(f[3]),
                                   "source_index": int(f[4]), "span": span(f[5:11]),
                                   "line_contributing": f[11] == "1",
                                   "line_numbers": [int(x) for x in f[12].split(",") if x],
                                   "terms": terms(f[13])})
        else:
            raise ValueError(f"unexpected dump line {line!r}")
    return out


def terms(text: str) -> list[tuple[int, str]]:
    out = []
    for t in filter(None, text.split(",")):
        coef, ref = t.split("*", 1)
        out.append((int(coef), ref))
    return sorted(out)


def expected_regions(expected: dict) -> tuple[list, list]:
    """Branches and regions of an expected map, with counter terms named like
    the dump (entry:<entity>, branch:<branch index>:<ordinal>)."""
    names = {p["point_id"]: f"entry:{p['entity_index']}" for p in expected["points"] if p["kind"] == "function_entry"}
    branches = []
    for b in expected["branches"]:
        for o in b["outcomes"]:
            names[o["point_id"]] = f"branch:{b['branch_index']}:{o['ordinal']}"
        branches.append({"branch_index": b["branch_index"], "kind": b["kind"], "entity_index": b["entity_index"],
                         "source_index": b["source_index"], "span": b["span"], "outcomes": len(b["outcomes"]),
                         "names": [o["name"] for o in b["outcomes"]]})
    regions = [{"region_index": r["region_index"], "kind": r["kind"], "entity_index": r["entity_index"],
                "source_index": r["source_index"], "span": r["span"], "line_contributing": r["line_contributing"],
                "line_numbers": r["line_numbers"],
                "terms": sorted((t["coefficient"], names[t["point_id"]]) for t in r["counter_expression"]["terms"])}
               for r in expected["regions"]]
    return branches, regions


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


def check_fixture(name: str, errors: list[str], tmp: Path) -> None:
    d = FIXTURES / name
    expected_bytes = (d / "expected-map.json").read_bytes()
    expected = json.loads(expected_bytes)
    project = any(p.parent != d for p in d.rglob("*.sv0"))
    request = f"--project {d}" if project else str(d / "main.sv0")
    # CV-110: the emitted map is byte-identical to the hand-reviewed one.
    out = tmp / f"map-{name}"
    out.mkdir()
    nrc, nmap, vrc, vmap, merr = run_map(request, out, expected["target"]["name"], expected["compiler"]["identity"])
    if nrc != 0 or vrc != 0:
        errors.append(f"{name}: map build failed (native {nrc}, vm {vrc}): {merr.strip()}")
    elif nmap != expected_bytes or vmap != expected_bytes:
        errors.append(f"{name}: emitted map differs from expected-map.json (native {nmap == expected_bytes}, vm {vmap == expected_bytes})")
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
    want_outcomes = sorted(
        ({"kind": p["kind"], "entity_index": p["entity_index"], "semantic_discriminator": p["semantic_discriminator"],
          "source_index": p["source_index"], "span": p["span"], "outcome_ordinal": p["outcome_ordinal"]}
         for p in expected["points"] if p["kind"] == "branch_outcome"),
        key=lambda p: (p["span"]["start_byte"], p["outcome_ordinal"]))
    got_outcomes = sorted(got["outcome_points"], key=lambda p: (p["span"]["start_byte"], p["outcome_ordinal"]))
    if got_outcomes != want_outcomes:
        errors.append(f"{name}: branch_outcome points differ:\n    got  {got_outcomes}\n    want {want_outcomes}")
    want_branches, want_regions = expected_regions(expected)
    if got["branches"] != want_branches:
        errors.append(f"{name}: branches differ:\n    got  {got['branches']}\n    want {want_branches}")
    if got["regions"] != want_regions:
        diff = [(g, w) for g, w in zip(got["regions"], want_regions) if g != w][:3]
        errors.append(f"{name}: regions differ ({len(got['regions'])} vs {len(want_regions)}); first: {diff}")


# Synthetic programs: (name, files {path: text}, entry file or None for a
# project, [(qualified name, file, start anchor, end anchor)]). A span starts
# at its start anchor and ends after the first following end anchor.
SYNTHETIC = [
    ("methods-and-traits", {"main.sv0": (
        "/* café ☕ — multibyte text before the first item */\n"
        "struct Pt { x: i32 }\n\n"
        "trait Area {\n    fn sides(self: Pt) -> i32;\n    fn twice(self: Pt) -> i32 { return 2; }\n}\n\n"
        "impl Area for Pt {\n    fn sides(self: Pt) -> i32 { return 4; }\n}\n\n"
        "impl Pt {\n    fn get(self: Pt) -> i32 { return self.x; }\n    fn bump(self: Pt) -> i32 {\n"
        "        return self.x + 1;\n    }\n}\n\n"
        "#[extern_c]\nfn abs(x: i32) -> i32;\n\n"
        "pub fn first(a: [i32; 3]) -> i32 { return a[0]; }\n\n"
        "fn checked(n: i32) -> i32 requires(n > 0) ensures(result > 0) { return n; }\n\n"
        "fn main() -> i32 {\n    let p: Pt = Pt { x: 2 };\n"
        "    return p.get() + p.bump() + first([1, 2, 3]) + checked(1) + abs(0) - 7;\n}\n")},
     "main.sv0",
     [("Area::twice", "main.sv0", "fn twice", "return 2; }"),
      ("Pt::sides", "main.sv0", "fn sides(self: Pt) -> i32 {", "return 4; }"),
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
    for mode, needle in (("bogus", "unknown mode"), ("map", "needs <mode>, <map path>"),
                         ("instrument", "needs <mode>, <map path>")):
        rc, out, err, vrc, _ = run_plan(str(d / "plain.sv0"), mode)
        if rc != 9 or vrc != 9 or needle not in err or out:
            errors.append(f"mode {mode}: expected refusal 9 with {needle!r}, got native {rc} vm {vrc}: {err.strip()}")


def modern_python() -> list[str] | None:
    """A command running Python >= 3.10 (sv0cov's floor), or None."""
    import shutil

    if sys.version_info >= (3, 10):
        return [sys.executable]
    for name in ("python3.14", "python3.13", "python3.12", "python3.11", "python3.10"):
        exe = shutil.which(name)
        if exe:
            return [exe]
    return None


def check_constructs_map(tmp: Path, errors: list[str]) -> bool:
    """constructs.sv0's map (no hand-written counterpart) passes sv0cov's own
    map validator with its source bytes. Returns False when no Python >= 3.10
    is available to run the validator (reported as skipped, not passed)."""
    out = tmp / "map-constructs"
    out.mkdir()
    src = HERE / "constructs.sv0"
    nrc, nmap, vrc, vmap, merr = run_map(str(src), out, "constructs", "sv0c+test")
    if nrc != 0 or vrc != 0 or nmap != vmap or not nmap:
        errors.append(f"constructs map: build failed or backends differ (native {nrc}, vm {vrc}): {merr.strip()}")
        return True
    py = modern_python()
    if py is None:
        return False
    code = ("import sys; sys.path.insert(0, sys.argv[1]); from sv0cov.formats.map import validate_map; "
            "validate_map(open(sys.argv[2], 'rb').read(), sources={'constructs.sv0': open(sys.argv[3], 'rb').read()})")
    proc = subprocess.run([*py, "-c", code, str(ROOT / "sv0cov" / "src"), str(out / "native.sv0covmap.json"), str(src)],
                          capture_output=True, text=True, timeout=300)
    if proc.returncode != 0:
        errors.append(f"constructs map: sv0cov validate_map rejected it: {proc.stderr.strip()[-600:]}")
    return True


def check_constructs(errors: list[str]) -> None:
    """constructs.sv0 (else-if, for, loop, while + loop_invariant, match with
    block arms, break/continue, compound assignment, conditional expression,
    tail expression) against the hand-reviewed constructs.expected."""
    rc, out, err, vrc, vout = run_plan(str(HERE / "constructs.sv0"))
    if rc != 0 or vrc != 0 or out != vout:
        errors.append(f"constructs: plan-dump failed or backends differ (native {rc}, vm {vrc}): {err.strip()}")
        return
    got = []
    for line in out.splitlines():
        f = line.split("\t")
        if f[0] == "branch":
            got.append(f"branch {f[1]} {f[2]} {f[3]} {f[7]}:{f[8]}-{f[9]}:{f[10]} {f[11]} {f[12]}")
        elif f[0] == "region":
            got.append(f"region {f[2]} {f[3]} {f[7]}:{f[8]}-{f[9]}:{f[10]} {f[11]} {f[12]} {f[13]}")
    want = [l for l in (HERE / "constructs.expected").read_text().splitlines() if l and not l.startswith("#")]
    if got != want:
        diff = [(g, w) for g, w in zip(got, want) if g != w][:3]
        errors.append(f"constructs: plan differs ({len(got)} vs {len(want)} lines); first: {diff}")


def main() -> int:
    ensure_built()
    errors: list[str] = []
    check_constructs(errors)
    fixtures = sorted(p.parent.name for p in FIXTURES.glob("*/expected-map.json"))
    if len(fixtures) < 7:
        errors.append(f"expected the 7 sv0cov semantic fixtures under {FIXTURES}, found {fixtures}")
    with tempfile.TemporaryDirectory() as td:
        for name in fixtures:
            check_fixture(name, errors, Path(td))
        validated = check_constructs_map(Path(td), errors)
        for case in SYNTHETIC:
            check_synthetic(case, Path(td), errors)
        check_refusals(Path(td), errors)
    if errors:
        print("coverage plan: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    validation = "sv0cov-validated" if validated else "sv0cov validation SKIPPED (no Python >= 3.10)"
    print(f"coverage plan: OK ({len(fixtures)} sv0cov fixtures with byte-identical maps, {len(SYNTHETIC)} synthetic "
          f"programs, constructs map {validation}, native = VM emitter, include/unknown/instrument refused)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
