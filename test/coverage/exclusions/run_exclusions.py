#!/usr/bin/env python3
"""Coverage exclusions: policy, resolution and effect (sv0cov CV-305; SPEC
19, 16.3.7, 11.1; COV-EXC-003/004/006/011/020..024; AC-006, AC-007,
AC-040..044; sv0doc attributes/coverage.md sections 4 to 6).

Through the real native compiler and VM emitter:

1. mixed.sv0 (every form: `off` on a function and an impl, `branch_off` on
   an `if` with a nested `match` and on a `while`, `off` on an `if` that
   returns and on a block, `branch_off` on a `match`, `ignore-next` on a
   statement and on an `if`
   expression, an `ignore-start` region) built instrumented for C and the
   VM: the two maps are byte-identical (AC-007, COV-EXC-006/011) and
   sv0cov-valid; the exclusion records and classifications are
   mixed.expected; both backends run it to exit 0 and give equal counts.
2. Exclusion changes no ordinary count: the same program with every
   exclusion blanked out (same bytes otherwise, so the same spans) gives
   every region left ordinary exactly the same count on both backends, and
   every support point counts what the outcome it stands in for counts in
   the plain build (SPEC 11.1). AC-042: the `branch_off` if's own outcomes
   are excluded while its body region and the nested `match` stay ordinary.
3. Policy matrix, in an ordinary build (no --coverage), on both binaries:
   each misattached mode (COV1021), unrepresentable reason (COV3201),
   redundant or subsumed exclusion (COV3203, naming both locations,
   AC-044), attribute/directive conflict (COV3202), unresolvable
   `ignore-next` (COV1021) and straddling or empty region fails with its
   exact code, message and line:col (and the note's line:col).
4. Coverage builds only: an exclusion that takes nothing from the plan is
   COV1021; it is fine in an ordinary build. (`contract_off`'s effect is
   test/coverage/contracts, CV-306.)
5. A two-file project: exclusions in both files land in one valid map with
   the right source indexes.

    python3 sv0c/test/coverage/exclusions/run_exclusions.py [--write]
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
SV0C = HERE.parents[2]
ROOT = SV0C.parent
SV0 = ROOT / "scripts" / "sv0"
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
VM_EMIT = ROOT / "build" / "sv0-megatu-vm-native"
RUN_ID = "0123456789abcdef0123456789abcdef"
sys.dont_write_bytecode = True
sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import build_vm, transport, vm_run  # noqa: E402

# Validate a map; resolve each profile; print JSON: point counts per
# backend, plus the map's exclusions, regions and branches.
READ = r"""
import json, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov.formats.map import validate_map
from sv0cov.resolve import resolve
mb = Path(sys.argv[2]).read_bytes()
srcs = json.loads(sys.argv[3])
m = validate_map(mb, sources={k: Path(v).read_bytes() for k, v in srcs.items()})
counts = [resolve(mb, [Path(p).read_bytes()]).counts() for p in sys.argv[4:]]
print(json.dumps({"map": m, "counts": counts}))
"""

M = {
    1: "`off` excludes code, and this item has none (only a `fn`, or an `impl` or `trait` with a function body, or a statement)",
    2: "`branch_off` goes only on an `if`, `while`, `for`, `loop` or `match` statement",
    3: "`contract_off` goes only on a function that owns a `requires`, `ensures`, `loop_invariant` or `assert`",
    4: "a coverage reason may not contain a NUL, carriage return or line feed, and an escape in it may not stand for a byte above 0x7e",
    5: "this `ignore-next` resolves to no eligible construct",
    6: "this `ignore-next` resolves to no eligible construct: the code after it is part of an expression with no coverage of its own (only an `if` or `match` expression is)",
    7: "this `ignore-start` region holds no code",
    8: "an `ignore-start` region must begin and end between the items or statements of one block",
    9: "this construct already has this coverage exclusion",
    10: "`off` and `branch_off` on one construct: `off` already excludes the decision's outcomes",
    11: "an attribute and a comment directive give this construct different policy",
    12: "this exclusion lies inside an `off` exclusion that already excludes it",
    13: "this exclusion resolves to no eligible construct",
}


def attr(mode: str, reason: str = "r") -> str:
    return f'#[coverage({mode}, reason = "{reason}")]'


OFF, BOFF, COFF = attr("off"), attr("branch_off"), attr("contract_off")
NEXT = '// sv0cov: ignore-next reason="r"'
START = '// sv0cov: ignore-start reason="r"'
END = "// sv0cov: ignore-end"
MAIN = "fn main() -> i32 { return 0; }\n"

# (label, source, code, error at (first occurrence of), message id, note at or None)
NEGATIVE = [
    ("off on a struct", f"{OFF}\nstruct S {{ a: i32 }}\n{MAIN}", "COV1021", OFF, 1, None),
    ("off on a bodyless trait", f"{OFF}\ntrait T {{ fn f(x: i32) -> i32; }}\n{MAIN}", "COV1021", OFF, 1, None),
    ("branch_off on a function", f"{BOFF}\nfn f() -> i32 {{ return 1; }}\n{MAIN}", "COV1021", BOFF, 2, None),
    ("branch_off on a let", f"fn main() -> i32 {{\n    {BOFF}\n    let a: i32 = 0;\n    return a;\n}}\n", "COV1021", BOFF, 2, None),
    ("branch_off on a return", f"fn main() -> i32 {{\n    {BOFF}\n    return 0;\n}}\n", "COV1021", BOFF, 2, None),
    ("branch_off on a block", f"fn main() -> i32 {{\n    {BOFF}\n    {{\n        let a: i32 = 0;\n    }};\n    return 0;\n}}\n", "COV1021", BOFF, 2, None),
    ("contract_off on a statement", f"fn main() -> i32 {{\n    {COFF}\n    let a: i32 = 0;\n    return a;\n}}\n", "COV1021", COFF, 3, None),
    ("contract_off without a clause", f"{COFF}\nfn f() -> i32 {{ return 1; }}\n{MAIN}", "COV1021", COFF, 3, None),
    ("contract_off on a struct", f"{COFF}\nstruct S {{ a: i32 }}\n{MAIN}", "COV1021", COFF, 3, None),
    ("reason with a line feed escape", f'{attr("off", "a" + chr(92) + "nb")}\nfn f() -> i32 {{ return 1; }}\n{MAIN}', "COV3201", '"a', 4, None),
    ("reason with a NUL escape", f'{attr("off", chr(92) + "0")}\nfn f() -> i32 {{ return 1; }}\n{MAIN}', "COV3201", '"' + chr(92), 4, None),
    ("reason with a high byte escape", f'{attr("off", chr(92) + "xff")}\nfn f() -> i32 {{ return 1; }}\n{MAIN}', "COV3201", '"' + chr(92), 4, None),
    ("directive reason with a line feed escape", f'fn main() -> i32 {{\n    // sv0cov: ignore-next reason="a{chr(92)}nb"\n    return 0;\n}}\n', "COV3201", '"a', 4, None),
    ("the same mode twice", f'{OFF}\n{attr("off", "s")}\nfn f() -> i32 {{ return 1; }}\n{MAIN}', "COV3203", attr("off", "s"), 9, OFF),
    ("off with branch_off", f"fn main() -> i32 {{\n    {OFF}\n    {BOFF}\n    if true {{ return 1; }};\n    return 0;\n}}\n", "COV3203", BOFF, 10, OFF),
    ("off inside off (AC-044)", f'{attr("off", "outer")}\nfn f() -> i32 {{\n    {OFF}\n    let a: i32 = 1;\n    return a;\n}}\n{MAIN}', "COV3203", OFF, 12, attr("off", "outer")),
    ("branch_off inside off", f'{attr("off", "outer")}\nfn f() -> i32 {{\n    {BOFF}\n    if true {{ return 1; }};\n    return 0;\n}}\n{MAIN}', "COV3203", BOFF, 12, attr("off", "outer")),
    ("off inside an off statement", f'fn main() -> i32 {{\n    {attr("off", "outer")}\n    if true {{\n        {OFF}\n        let a: i32 = 1;\n    }};\n    return 0;\n}}\n', "COV3203", OFF, 12, attr("off", "outer")),
    ("directive and attribute on one construct", f"fn main() -> i32 {{\n    {NEXT}\n    {OFF}\n    let a: i32 = 1;\n    return a;\n}}\n", "COV3202", OFF, 11, NEXT),
    ("ignore-next inside an off function", f'{attr("off", "outer")}\nfn f() -> i32 {{\n    {NEXT}\n    return 1;\n}}\n{MAIN}', "COV3203", NEXT, 12, attr("off", "outer")),
    ("attribute inside a region", f"fn main() -> i32 {{\n    {START}\n    {OFF}\n    let a: i32 = 1;\n    {END}\n    return a;\n}}\n", "COV3203", OFF, 12, START),
    ("ignore-next before an operand", f"fn f() -> i32 {{ return 1; }}\nfn main() -> i32 {{\n    let a: i32 = 1 +\n        {NEXT}\n        f();\n    return a - 2;\n}}\n", "COV1021", NEXT, 6, "f();"),
    ("ignore-next before a closing brace", f"fn main() -> i32 {{\n    if true {{\n        let a: i32 = 1;\n        {NEXT}\n    }};\n    return 0;\n}}\n", "COV1021", NEXT, 5, None),
    ("ignore-next before declarations only", f"{MAIN}{NEXT}\nstruct S {{ a: i32 }}\n", "COV1021", NEXT, 5, None),
    ("region across a function's end", f"fn f() -> i32 {{\n    {START}\n    return 1;\n}}\n{END}\n{MAIN}", "COV3201", START, 8, END),
    ("region from inside an expression", f"fn main() -> i32 {{\n    let a: i32 = 1 +\n        {START}\n        2;\n    {END}\n    return a - 3;\n}}\n", "COV3201", START, 8, END),
    ("region into a nested block", f"fn main() -> i32 {{\n    {START}\n    if true {{\n        {END}\n        let a: i32 = 1;\n    }};\n    return 0;\n}}\n", "COV3201", START, 8, END),
    ("empty region", f"fn main() -> i32 {{\n    {START}\n    {END}\n    return 0;\n}}\n", "COV1021", START, 7, END),
]

# Programs every build accepts.
POSITIVE = {
    "off and contract_off on one function": f"{OFF}\n{COFF}\nfn f(x: i32) -> i32 requires(x > 0) {{ return x; }}\nfn main() -> i32 {{ return f(1) - 1; }}\n",
    "ignore-next skips a declaration": f"{NEXT}\nstruct S {{ a: i32 }}\nfn f() -> i32 {{ return 1; }}\nfn main() -> i32 {{ return f() - 1; }}\n",
    "ignore-next skips an attribute": f"{NEXT}\n#[inline]\nfn f() -> i32 {{ return 1; }}\nfn main() -> i32 {{ return f() - 1; }}\n",
    "ignore-next on a match expression": f"fn main() -> i32 {{\n    let a: i32 = 1;\n    let b: i32 =\n        {NEXT}\n        match a {{ 1 => 0, _ => 1 }};\n    return b;\n}}\n",
    "region over whole functions": f"{START}\nfn f() -> i32 {{ return 1; }}\n{END}\nfn main() -> i32 {{ return f() - 1; }}\n",
    "region up to a block's end": f"fn main() -> i32 {{\n    let a: i32 = 1;\n    if a > 0 {{\n        {START}\n        let b: i32 = 2;\n        {END}\n    }};\n    return a - 1;\n}}\n",
}

# Refused only in coverage builds: (label, source, error at, message id).
COVERAGE_ONLY = [
    ("a region with no planned code", f"{START}\nstruct S {{ a: i32 }}\n{END}\n{MAIN}", START, 13),
]


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


def blank(text: str) -> str:
    """The program with every exclusion blanked out, byte offsets kept."""
    text = re.sub(r"#\[coverage\([^\]]*\)\]", lambda m: " " * len(m.group(0)), text)
    return text.replace("// sv0cov:", "// plain::")


def build_and_run(src: Path, d: Path, errors: list[str], label: str) -> list[Path]:
    """Instrumented C and VM builds of src in d; returns the two profiles."""
    (d / "vm").mkdir(parents=True)
    p = subprocess.run([str(SV0), "native-compile", "--coverage=instrument", "-o", str(d / "x"), str(src)],
                       capture_output=True, text=True, timeout=300)
    err = build_vm([str(src)], d / "vm" / "x.sv0b")
    if p.returncode or err:
        errors.append(f"{label}: instrumented build failed: {p.stderr.strip()[-300:]} {err or ''}")
        return []
    if (d / "vm" / "x.sv0covmap.json").read_bytes() != (d / "x.sv0covmap.json").read_bytes():
        errors.append(f"{label}: the C and VM maps differ (AC-007)")
    profiles = []
    for backend in ("native", "vm"):
        pdir = d / f"p-{backend}"
        pdir.mkdir()
        if backend == "native":
            rc = subprocess.run([str(d / "x")], capture_output=True, env=transport(pdir), timeout=60).returncode
        else:
            rc, _ = vm_run(d / "vm" / "x.sv0b", d / "vm" / "x.sv0covbind.json", transport(pdir))
        files = sorted(pdir.iterdir())
        if rc != 0 or len(files) != 1:
            errors.append(f"{label} ({backend}): exit {rc}, {len(files)} profiles")
        else:
            profiles.append(files[0])
    return profiles


def read(py: list[str], d: Path, srcs: dict[str, str], profiles: list[Path], errors: list[str], label: str) -> dict | None:
    r = subprocess.run([*py, "-c", READ, str(ROOT / "sv0cov" / "src"), str(d / "x.sv0covmap.json"), json.dumps(srcs),
                        *map(str, profiles)], capture_output=True, text=True, timeout=300)
    if r.returncode:
        errors.append(f"{label}: sv0cov rejects the map or a profile: {r.stderr.strip()[-400:]}")
        return None
    return json.loads(r.stdout)


def region_value(r: dict, counts: dict) -> int:
    return sum(t["coefficient"] * counts[t["point_id"]] for t in r["counter_expression"]["terms"])


def summary(m: dict, counts: dict) -> list[str]:
    lines = []
    for x in m["exclusions"]:
        a = x["affected"]
        lines.append(f"exclusion {x['mode']} {x['origin']} {x['span']['start_line']}:{x['span']['start_column']} "
                     f"entities={len(a['entity_indices'])} branches={len(a['branch_indices'])} "
                     f"regions={len(a['region_indices'])} points={len(a['point_ids'])} reason={x['reason']}")
    for b in m["branches"]:
        lines.append(f"branch {b['kind']} {b['span']['start_line']} " + " ".join(f"{o['name']}={o['classification']}" for o in b["outcomes"]))
    for p in m["points"]:
        if p["semantic_discriminator"].startswith("support:"):
            lines.append(f"support {p['semantic_discriminator']} {p['span']['start_line']} count={counts[p['point_id']]}")
    for r in m["regions"]:
        v = region_value(r, counts) if r["classification"] == "user" else "-"
        lines.append(f"region {r['kind']} {r['span']['start_line']}:{r['span']['start_column']} {r['classification']} {v}")
    lines.append(f"counters {m['program_counter_count']}")
    return lines


def main() -> int:
    ensure_built()
    errors: list[str] = []
    py = modern_python()
    write = "--write" in sys.argv
    mixed = HERE / "mixed.sv0"
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)
        # 1-2. mixed.sv0 and its blanked twin, on both backends
        plain = t / "plain" / "mixed.sv0"
        plain.parent.mkdir()
        plain.write_text(blank(mixed.read_text()))
        got = {}
        for label, src in (("mixed", mixed), ("plain", plain)):
            d = t / f"b-{label}"
            profiles = build_and_run(src, d, errors, label)
            if py is not None and len(profiles) == 2:
                got[label] = read(py, d, {"mixed.sv0": str(src)}, profiles, errors, label)
        if py is not None and got.get("mixed") and got.get("plain"):
            m, (cn, cv) = got["mixed"]["map"], got["mixed"]["counts"]
            pm, (pn, pv) = got["plain"]["map"], got["plain"]["counts"]
            if cn != cv or pn != pv:
                errors.append("the C and VM profiles count differently")
            lines = summary(m, cn)
            expected = HERE / "mixed.expected"
            if write:
                expected.write_text("\n".join(lines) + "\n")
            if lines != expected.read_text().splitlines():
                errors.append("mixed.expected: the exclusions, classifications or counts differ:\n    "
                              + "\n    ".join(lines))
            plain_regions = {(r["kind"], r["span"]["start_byte"], r["span"]["end_byte"]): region_value(r, pn) for r in pm["regions"]}
            for r in m["regions"]:
                if r["classification"] == "user":
                    key = (r["kind"], r["span"]["start_byte"], r["span"]["end_byte"])
                    if plain_regions.get(key) != region_value(r, cn):
                        errors.append(f"region {key} counts {region_value(r, cn)} with exclusions, {plain_regions.get(key)} without")
            plain_outcomes = {}
            for b in pm["branches"]:
                for o in b["outcomes"]:
                    plain_outcomes[(b["span"]["start_byte"], o["name"])] = pn.get(o["point_id"])
            for p in m["points"]:
                if p["semantic_discriminator"].startswith("support:"):
                    want = plain_outcomes.get((p["span"]["start_byte"], p["semantic_discriminator"].split(":", 2)[2]))
                    if counts_of(cn, p) != want:
                        errors.append(f"support point {p['semantic_discriminator']} at byte {p['span']['start_byte']} counts "
                                      f"{counts_of(cn, p)}, the outcome counts {want} without exclusions")
            # AC-042: branch_off on the if keeps its body and the nested match ordinary
            text = mixed.read_text()
            if_at = text.index("if n >= 0")
            match_at = text.index("match n")
            ifb = [b for b in m["branches"] if b["span"]["start_byte"] == if_at]
            mb = [b for b in m["branches"] if b["span"]["start_byte"] == match_at]
            body = [r for r in m["regions"] if r["kind"] == "branch_body" and r["span"]["start_byte"] > if_at
                    and r["span"]["start_byte"] < match_at]
            if (len(ifb) != 1 or {o["classification"] for o in ifb[0]["outcomes"]} != {"excluded"} or len(mb) != 1
                    or {o["classification"] for o in mb[0]["outcomes"]} != {"eligible"} or not body
                    or body[0]["classification"] != "user"):
                errors.append("AC-042: branch_off reached past the if's own outcomes, or missed them")
        # 3. policy matrix, ordinary build
        for label, src_text, code, at, mid, note in NEGATIVE:
            src = t / "neg.sv0"
            src.write_text(src_text)
            where = line_col(src_text, at)
            for binary in (NATIVE, VM_EMIT):
                p = run(binary, str(src))
                err = p.stderr.decode()
                want = f"error[{code}]: {M[mid]}\n  --> {where}\n"
                ok = p.returncode == 6 and want in err and err.count("error[") == 1
                if note is not None:
                    ok = ok and f"  = note: " in err and f"  --> {line_col(src_text, note)}\n" in err.split("= note:", 1)[-1]
                if not ok:
                    errors.append(f"{label} ({binary.name}): exit {p.returncode}, want 6 with {code} at {where}; got {err.strip()[:400]!r}")
        for label, src_text in POSITIVE.items():
            src = t / "pos.sv0"
            src.write_text(src_text)
            for binary in (NATIVE, VM_EMIT):
                p = run(binary, str(src))
                if p.returncode:
                    errors.append(f"{label} ({binary.name}): the ordinary build fails: {p.stderr.decode()[:300]!r}")
        # 4. refused only in coverage builds
        for label, src_text, at, mid in COVERAGE_ONLY:
            src = t / "cov.sv0"
            src.write_text(src_text)
            for binary in (NATIVE, VM_EMIT):
                if run(binary, str(src)).returncode != 0:
                    errors.append(f"{label} ({binary.name}): the ordinary build fails")
                out = t / f"m-{binary.name}.json"
                p = run(binary, str(src), f"map\n{out}\nt\nsv0c+test")
                err = p.stderr.decode()
                if p.returncode != 6 or f"error[COV1021]: {M[mid]}\n  --> {line_col(src_text, at)}\n" not in err or out.exists():
                    errors.append(f"{label} ({binary.name}) under --coverage=map: exit {p.returncode}: {err[:300]!r}")
        # 5. a two-file project
        proj = t / "proj"
        (proj / "lib").mkdir(parents=True)
        (proj / "main.sv0").write_text(f"use lib::helper;\nfn main() -> i32 {{\n    {NEXT}\n    let a: i32 = 1;\n    return helper() - a;\n}}\n")
        (proj / "lib" / "lib.sv0").write_text(f'{attr("off", "library")}\npub fn helper() -> i32 {{ return 1; }}\n')
        out = t / "proj.json"
        p = run(NATIVE, f"--project {proj}", f"map\n{out}\nproj\nsv0c+test")
        vout = t / "projv.json"
        v = run(VM_EMIT, f"--project {proj}", f"map\n{vout}\nproj\nsv0c+test")
        if p.returncode or v.returncode or not out.exists() or out.read_bytes() != vout.read_bytes():
            errors.append(f"project: map builds exit {p.returncode}/{v.returncode}, or the maps differ: {p.stderr.decode()[:300]!r}")
        elif py is not None:
            r = subprocess.run([*py, "-c", READ, str(ROOT / "sv0cov" / "src"), str(out),
                                json.dumps({"main.sv0": str(proj / "main.sv0"), "lib/lib.sv0": str(proj / "lib" / "lib.sv0")})],
                               capture_output=True, text=True, timeout=300)
            if r.returncode:
                errors.append(f"project: sv0cov rejects the map: {r.stderr.strip()[-300:]}")
            else:
                pm2 = json.loads(r.stdout)["map"]
                got2 = sorted((pm2["sources"][x["source_index"]]["path"], x["origin"], x["reason"]) for x in pm2["exclusions"])
                if got2 != [("lib/lib.sv0", "attribute", "library"), ("main.sv0", "comment", "r")]:
                    errors.append(f"project: exclusions {got2}")
    if errors:
        print("coverage exclusions: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage exclusions: OK (mixed.sv0: C map = VM map, sv0cov-valid, mixed.expected, equal C/VM counts; every ordinary "
          f"region and support point counts as without exclusions; AC-042; {len(NEGATIVE)} policy errors and {len(POSITIVE)} "
          f"accepted forms x 2 binaries; {len(COVERAGE_ONLY)} coverage-only refusals; two-file project)")
    return 0


def counts_of(counts: dict, p: dict):
    return counts.get(p["point_id"])


if __name__ == "__main__":
    sys.exit(main())
