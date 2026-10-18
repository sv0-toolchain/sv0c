#!/usr/bin/env python3
"""Contract coverage and enforcement (sv0cov CV-306; SPEC 12, 13.1, 16.3.7;
COV-MET-009/010/015, COV-MAP-014; AC-010, AC-043, AC-106; sv0doc
contracts/semantics.md 1.4, 3.1 and keywords/reference.md 1.12).

1. Enforcement, ordinary builds, native and VM: `requires`, `ensures`,
   `loop_invariant` (at the head of every iteration, so also once more on
   exit) and `assert` stop the program (an `ensures` at every return,
   however nested) with exit 1 and a message naming the
   clause kind and function when false, and change nothing when true.
   `--contract-mode=disabled` strips every check (native).
2. clauses.sv0 built instrumented for C and the VM: byte-identical,
   sv0cov-valid maps; one contract entity per clause (`<fn>#<kind>:<k>`,
   right after its function), owned by its function; the plan and both
   backends' counts are clauses.expected (an ensures checked at two returns
   counts every evaluation; the invariant counts n + 1 evaluations).
3. AC-010: stripping the requires/ensures/loop_invariant clauses (blanked,
   so spans stay) leaves every ordinary region and branch and its count
   unchanged.
4. Dispositions: a disabled build has every clause `disabled` and no
   contract points; a verified build whose proof lists the ensures and
   loop_invariant lines has those `statically_verified` with evidence and
   no points, the rest `runtime_checked`; a model-only clause (`old`) is
   `unsupported`.
5. AC-106 through `sv0cov merge` and `report` (native runs: sv0vm has no
   `getenv` to select the failing run): a passing run and a run that
   fails a `requires` (its profile is still published) give contract
   coverage 2/3 (true-only and false-only clauses covered, the
   never-evaluated one not), with the `contract_off` and model-only clauses
   visible outside the fraction.
6. AC-043: `contract_off` on a function whose `requires` fails: the
   program still stops on the violation, and the false evaluation is
   recorded as excluded evidence (count kept, outside the denominator).

    python3 sv0c/test/coverage/contracts/run_contracts.py [--write]
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
RUN_ID = "0123456789abcdef0123456789abcdef"
sys.dont_write_bytecode = True
sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import build_vm, transport, vm_run  # noqa: E402

# Validate maps and decode profiles through sv0cov; print the semantic
# contract and ordinary records for each (map, profile) pair.
READ = r"""
import json, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov.formats.map import validate_map
from sv0cov.formats.rawprofile import decode
from sv0cov.model.semantic import build_semantic
out = []
for mp, src, prof in json.loads(sys.argv[2]):
    mb = Path(mp).read_bytes()
    m = validate_map(mb, sources={Path(src).name: Path(src).read_bytes()})
    rec = {"map": m}
    if prof:
        p = decode(Path(prof).read_bytes(), map_counter_count=m["program_counter_count"], expected_map_id=bytes.fromhex(m["map_id"]))
        rec["semantic"] = build_semantic([m], [p])
    out.append(rec)
print(json.dumps(out))
"""

# merge + report through the sv0cov CLI in a directory; print the report.
REPORT = r"""
import io, json, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov import cli
cwd = Path(sys.argv[2])
e = io.StringIO()
rc = cli.main(["merge", "in", "--output", "m.sv0covdata.json"], stdout=io.StringIO(), stderr=e, cwd=cwd)
if rc:
    sys.exit(f"merge exit {rc}: {e.getvalue()}")
rc = cli.main(["report", "--format", "json", "--output", "r.json", "m.sv0covdata.json"], stdout=io.StringIO(), stderr=e, cwd=cwd)
if rc:
    sys.exit(f"report exit {rc}: {e.getvalue()}")
print((cwd / "r.json").read_text())
"""

ENFORCE = [
    # (label, source, exit, native message, VM message)
    ("assert false", "fn f(x: i32) -> i32 {\n    assert x > 5;\n    return x;\n}\nfn main() -> i32 { return f(1) - 1; }\n",
     1, "assert failed in f", "assert failed: f"),
    ("assert true", "fn f(x: i32) -> i32 {\n    assert x > 0;\n    return x;\n}\nfn main() -> i32 { return f(1) - 1; }\n", 0, None, None),
    ("loop_invariant broken in the loop",
     "fn main() -> i32 {\n    let mut i: i32 = 0;\n    while i < 5 loop_invariant(i < 3) {\n        i = i + 1;\n    }\n    return i;\n}\n",
     1, "loop_invariant failed in main", "loop_invariant failed: main"),
    ("loop_invariant broken only on exit",
     "fn main() -> i32 {\n    let mut i: i32 = 0;\n    while i < 3 loop_invariant(i < 3) {\n        i = i + 1;\n    }\n    return i;\n}\n",
     1, "loop_invariant failed in main", "loop_invariant failed: main"),
    ("loop_invariant held",
     "fn main() -> i32 {\n    let mut i: i32 = 0;\n    while i < 3 loop_invariant(i <= 3) {\n        i = i + 1;\n    }\n    return i - 3;\n}\n",
     0, None, None),
    ("loop_invariant with a complex condition",
     "fn main() -> i32 {\n    let v: Vec<i32> = vec_new();\n    vec_push(v, 1);\n    vec_push(v, 2);\n    let mut i: i32 = 0;\n"
     "    while i < vec_len(v) - 0 loop_invariant(i <= 1) {\n        i = i + 1;\n    }\n    return i;\n}\n",
     1, "loop_invariant failed in main", "loop_invariant failed: main"),
    ("requires false", "fn f(x: i32) -> i32 requires(x > 5) { return x; }\nfn main() -> i32 { return f(1); }\n",
     1, "requires failed in f", "requires failed: f"),
    ("ensures false", "fn f(x: i32) -> i32 ensures(result > 5) { return x; }\nfn main() -> i32 { return f(1); }\n",
     1, "ensures failed in f", "ensures failed: f"),
    ("ensures false at an early return",
     "fn f(x: i32) -> i32 ensures(result <= 0) {\n    if x > 0 { return 5; };\n    return 0;\n}\nfn main() -> i32 { return f(1); }\n",
     1, "ensures failed in f", "ensures failed: f"),
    ("ensures false at a return in a loop",
     "fn f(x: i32) -> i32 ensures(result <= 0) {\n    let mut i: i32 = 0;\n    while i < 3 {\n        if i == x { return 7; };\n"
     "        i = i + 1;\n    }\n    return 0;\n}\nfn main() -> i32 { return f(1); }\n",
     1, "ensures failed in f", "ensures failed: f"),
]


def compile_c(request: str, cov: str = "") -> subprocess.CompletedProcess:
    env = {k: v for k, v in os.environ.items() if not k.startswith("SV0")}
    env["SV0_DRV_REQUEST"] = request
    if cov:
        env["SV0_COVERAGE_REQUEST"] = cov
    return subprocess.run([str(NATIVE), *request.split(" ")], capture_output=True, env=env, timeout=300)


def instrumented(src: Path, d: Path, errors: list[str], label: str, envs: list[dict] | None = None,
                 backends: tuple = ("native", "vm")) -> list[Path]:
    """Instrumented C and VM builds of src; one profile per backend and
    entry of envs (extra environment); returns them backend by backend."""
    envs = envs or [{}]
    (d / "vm").mkdir(parents=True)
    p = subprocess.run([str(SV0), "native-compile", "--coverage=instrument", "-o", str(d / "x"), str(src)],
                       capture_output=True, text=True, timeout=300)
    err = build_vm([str(src)], d / "vm" / "x.sv0b")
    if p.returncode or err:
        errors.append(f"{label}: instrumented build failed: {p.stderr.strip()[-300:]} {err or ''}")
        return []
    if (d / "vm" / "x.sv0covmap.json").read_bytes() != (d / "x.sv0covmap.json").read_bytes():
        errors.append(f"{label}: the C and VM maps differ")
    profiles = []
    for backend in backends:
        for k, extra in enumerate(envs):
            pdir = d / f"p-{backend}-{k}"
            pdir.mkdir()
            env = {**transport(pdir), **extra}
            if backend == "native":
                subprocess.run([str(d / "x")], capture_output=True, env=env, timeout=60)
            else:
                vm_run(d / "vm" / "x.sv0b", d / "vm" / "x.sv0covbind.json", env)
            files = sorted(pdir.iterdir())
            if len(files) != 1:
                errors.append(f"{label} ({backend}, run {k}): {len(files)} profiles, want 1")
            else:
                profiles.append(files[0])
    return profiles


def read(py: list[str], items: list[tuple], errors: list[str], label: str) -> list[dict] | None:
    r = subprocess.run([*py, "-c", READ, str(ROOT / "sv0cov" / "src"), json.dumps([[str(a) for a in i] for i in items])],
                       capture_output=True, text=True, timeout=300)
    if r.returncode:
        errors.append(f"{label}: sv0cov rejects a map or profile: {r.stderr.strip()[-400:]}")
        return None
    return json.loads(r.stdout)


def value(c: dict | None) -> int | None:
    return None if c is None else c["value"]


def plan_lines(m: dict, sem: dict) -> list[str]:
    lines = []
    for e in m["entities"]:
        lines.append(f"entity {e['entity_index']} {e['kind']} {e['qualified_name']} {e['span']['start_line']}:{e['span']['start_column']}")
    for c, s in zip(m["contracts"], sem["contracts"]):
        lines.append(f"contract {c['clause_index']} {c['kind']} {c['enforcement']} entity={c['entity_index']} "
                     f"function={c['function_entity_index']} true={value(s['count_true'])} false={value(s['count_false'])} "
                     f"result={s['runtime_result']}")
    for x in m["exclusions"]:
        lines.append(f"exclusion {x['mode']} clauses={x['affected']['clause_indices']} entities={x['affected']['entity_indices']} "
                     f"points={len(x['affected']['point_ids'])}")
    kinds = {}
    for p in m["points"]:
        kinds[f"{p['kind']}/{p['classification']}"] = kinds.get(f"{p['kind']}/{p['classification']}", 0) + 1
    lines.append("points " + " ".join(f"{k}={v}" for k, v in sorted(kinds.items())))
    lines.append(f"counters {m['program_counter_count']}")
    return lines


def blank_clauses(text: str) -> str:
    """requires/ensures/loop_invariant clauses (and contract_off, which
    needs one) blanked, byte offsets kept."""
    out = re.sub(r"#\[coverage\(contract_off[^\]]*\]", lambda m: " " * len(m.group(0)), text)
    for kw in ("requires", "ensures", "loop_invariant"):
        while True:
            i = out.find(kw + "(")
            if i < 0:
                break
            depth, j = 0, i + len(kw)
            while True:
                if out[j] == "(":
                    depth += 1
                elif out[j] == ")":
                    depth -= 1
                    if depth == 0:
                        break
                j += 1
            out = out[:i] + " " * (j + 1 - i) + out[j + 1:]
    return out


def ordinary(m: dict, counts: dict) -> list[tuple]:
    rows = []
    for r in m["regions"]:
        v = sum(t["coefficient"] * counts[t["point_id"]] for t in r["counter_expression"]["terms"])
        rows.append(("region", r["kind"], r["span"]["start_byte"], r["span"]["end_byte"], r["classification"], v))
    for b in m["branches"]:
        rows.append(("branch", b["kind"], b["span"]["start_byte"], b["span"]["end_byte"],
                     tuple((o["name"], o["classification"], counts.get(o["point_id"])) for o in b["outcomes"])))
    for e in m["entities"]:
        if e["kind"] == "function":
            rows.append(("function", e["qualified_name"], e["span"]["start_byte"]))
    return rows


def point_counts(sem: dict) -> dict:
    return {p["point_id"]: (p["count"] or {}).get("value", 0) for p in sem["points"]}


def main() -> int:
    ensure_built()
    errors: list[str] = []
    py = modern_python()
    write = "--write" in sys.argv
    clauses = HERE / "clauses.sv0"
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)
        # 1. enforcement, ordinary builds
        for label, text, want, nmsg, vmsg in ENFORCE:
            src = t / "e.sv0"
            src.write_text(text)
            exe = t / "e"
            b = subprocess.run([str(SV0), "native-compile", "-o", str(exe), str(src)], capture_output=True, text=True, timeout=300)
            r = subprocess.run([str(exe)], capture_output=True, text=True, timeout=60) if b.returncode == 0 else None
            if r is None or r.returncode != want or (nmsg and f"sv0 contract violation: {nmsg}" not in r.stderr):
                errors.append(f"{label} (native): exit {None if r is None else r.returncode}, want {want}; "
                              f"{b.stderr[-200:] if r is None else r.stderr[-200:]!r}")
            vb = t / "e.sv0b"
            subprocess.run([str(SV0), "vm-native-compile", str(src), str(vb)], capture_output=True, timeout=300)
            q = subprocess.run([str(SV0), "vm-run", str(vb)], capture_output=True, text=True, timeout=300)
            m = re.search(r"vm_exit:([~-]?\d+)", q.stdout + q.stderr)
            rc = int(m.group(1).replace("~", "-")) & 0xFF if m else None
            if rc != want or (vmsg and f"sv0 contract violation: {vmsg}" not in q.stdout + q.stderr):
                errors.append(f"{label} (VM): exit {rc}, want {want}; {(q.stdout + q.stderr)[-200:]!r}")
            if want:
                d = subprocess.run([str(SV0), "native-compile", "--contract-mode=disabled", "-o", str(exe), str(src)],
                                   capture_output=True, text=True, timeout=300)
                r = subprocess.run([str(exe)], capture_output=True, text=True, timeout=60) if d.returncode == 0 else None
                if r is None or "contract violation" in r.stderr:
                    errors.append(f"{label}: --contract-mode=disabled still checks the clause: {d.stderr[-200:]}")
        if py is None:
            errors.append("no Python >= 3.10 to read maps with sv0cov")
        else:
            # 2. clauses.sv0 on both backends
            profiles = instrumented(clauses, t / "c", errors, "clauses")
            if len(profiles) == 2:
                got = read(py, [(t / "c" / "x.sv0covmap.json", clauses, profiles[0]), (t / "c" / "x.sv0covmap.json", clauses, profiles[1])],
                           errors, "clauses")
                if got:
                    m, sn, sv = got[0]["map"], got[0]["semantic"], got[1]["semantic"]
                    if sn["contracts"] != sv["contracts"] or sn["points"] != sv["points"]:
                        errors.append("clauses: the C and VM counts differ")
                    lines = plan_lines(m, sn)
                    expected = HERE / "clauses.expected"
                    if write:
                        expected.write_text("\n".join(lines) + "\n")
                    if lines != expected.read_text().splitlines():
                        errors.append("clauses.expected: the contract plan or counts differ:\n    " + "\n    ".join(lines))
                    # 3. AC-010: the clauses do not move ordinary coverage
                    plain = t / "plain" / "clauses.sv0"
                    plain.parent.mkdir()
                    plain.write_text(blank_clauses(clauses.read_text()))
                    pprof = instrumented(plain, t / "p", errors, "clauses without contracts")
                    if pprof:
                        g2 = read(py, [(t / "p" / "x.sv0covmap.json", plain, pprof[0])], errors, "plain")
                        if g2:
                            a = ordinary(m, point_counts(sn))
                            b2 = ordinary(g2[0]["map"], point_counts(g2[0]["semantic"]))
                            if a != b2:
                                diff = [x for x in a if x not in b2][:3]
                                errors.append(f"AC-010: ordinary regions, branches or functions change with contracts: {diff}")
            # 4. dispositions: disabled and verified builds
            dm = t / "disabled.json"
            p = compile_c(f"--disabled {clauses}", f"map\n{dm}\nt\nsv0c+test")
            text = clauses.read_text()
            ens_line = text[:text.index("ensures(result >= 0)")].count("\n") + 1
            inv_line = text[:text.index("loop_invariant(i <= n)")].count("\n") + 1
            proof = t / "proof.txt"
            proof.write_text(f"{ens_line}\n{inv_line}\n")
            vmp = t / "verified.json"
            q = compile_c(f"--verified {proof} {clauses}", f"map\n{vmp}\nt\nsv0c+test")
            if p.returncode or q.returncode:
                errors.append(f"disabled/verified map builds: exit {p.returncode}/{q.returncode}: {p.stderr.decode()[-200:]} {q.stderr.decode()[-200:]}")
            else:
                got = read(py, [(dm, clauses, ""), (vmp, clauses, "")], errors, "dispositions")
                if got:
                    dmap, vmap = got[0]["map"], got[1]["map"]
                    if {c["enforcement"] for c in dmap["contracts"]} != {"disabled"} or any(
                            pt["kind"].startswith("contract") for pt in dmap["points"]):
                        errors.append("disabled build: not every clause disabled with no contract point")
                    disp = [(c["kind"], c["enforcement"], c["evidence_identity"], c["span"]["start_line"]) for c in vmap["contracts"]]
                    sv = [d for d in disp if d[1] == "statically_verified"]
                    if sorted((d[0], d[2]) for d in sv) != sorted([("ensures", f"sv0c:smt:ensures:{ens_line}"),
                                                                  ("loop_invariant", f"sv0c:smt:loop_invariant:{inv_line}")]):
                        errors.append(f"verified build: statically_verified clauses {sv}")
                    if [(d[0], d[1]) for d in disp if d[1] == "unsupported"] != [("ensures", "unsupported")]:
                        errors.append(f"verified build: the model-only clause is not the one unsupported clause: {disp}")
            # 5. AC-106 through merge and report
            ac = t / "ac"
            ac.mkdir()
            src = ac / "main.sv0"
            src.write_text(
                "fn checked(x: i32) -> i32 requires(x >= 0) { return x; }\n"
                "fn fails(x: i32) -> i32 requires(x > 100) { return x; }\n"
                "fn never(x: i32) -> i32 requires(x != 7) { return x; }\n"
                "#[coverage(contract_off, reason = \"the caller checks\")]\n"
                "fn trusted(x: i32) -> i32 requires(x >= 0) { return x; }\n"
                "fn model(x: i32) -> i32 ensures(result == old(x)) { return x; }\n"
                "fn main() -> i32 {\n"
                "    if string_len(getenv(\"FAIL\")) > 0 { return fails(1); };\n"
                "    return checked(1) + trusted(1) + model(1) - 3;\n"
                "}\n")
            # (native only: sv0vm has no getenv builtin to select the failing run)
            profs = instrumented(src, ac / "b", errors, "AC-106", envs=[{}, {"FAIL": "1"}], backends=("native",))
            if len(profs) == 2:
                for backend, pair in (("native", profs),):
                    inp = ac / f"run-{backend}" / "in"
                    inp.mkdir(parents=True)
                    (inp / "x.sv0covmap.json").write_bytes((ac / "b" / "x.sv0covmap.json").read_bytes())
                    (inp.parent / "main.sv0").write_bytes(src.read_bytes())
                    for f in pair:
                        (inp / f"{f.parent.name}-{f.name}").write_bytes(f.read_bytes())
                    r = subprocess.run([*py, "-c", REPORT, str(ROOT / "sv0cov" / "src"), str(inp.parent)], capture_output=True, text=True,
                                       timeout=300)
                    if r.returncode:
                        errors.append(f"AC-106 ({backend}): {r.stderr.strip()[-300:]}")
                        continue
                    rep = json.loads(r.stdout)
                    c = rep["summary"]["contract"]
                    res = [(x["kind"], x["enforcement"], x["runtime_result"]) for x in rep["contracts"]]
                    want = [("requires", "runtime_checked", "evaluated_true"), ("requires", "runtime_checked", "evaluated_false"),
                            ("requires", "runtime_checked", "not_evaluated"), ("requires", "runtime_checked", "evaluated_true"),
                            ("ensures", "unsupported", "not_applicable")]
                    if (c["covered"], c["total"]) != (2, 3) or res != want:
                        errors.append(f"AC-106 ({backend}): contract {c['covered']}/{c['total']}, results {res}")
            # 6. AC-043: contract_off keeps enforcement and the false evaluation
            a43 = t / "a43" / "main.sv0"
            a43.parent.mkdir()
            a43.write_text("#[coverage(contract_off, reason = \"r\")]\nfn trusted(x: i32) -> i32 requires(x > 0) { return x; }\n"
                           "fn main() -> i32 { return trusted(0); }\n")
            pr = instrumented(a43, t / "a43" / "b", errors, "AC-043")
            if len(pr) == 2:
                (t / "a43" / "q").mkdir()
                ex = subprocess.run([str(t / "a43" / "b" / "x")], capture_output=True, text=True, env=transport(t / "a43" / "q"), timeout=60)
                if ex.returncode != 1 or "requires failed in trusted" not in ex.stderr:
                    errors.append(f"AC-043: contract_off changed enforcement: exit {ex.returncode} {ex.stderr[-200:]!r}")
                got = read(py, [(t / "a43" / "b" / "x.sv0covmap.json", a43, pr[0])], errors, "AC-043")
                if got:
                    m43, s43 = got[0]["map"], got[0]["semantic"]
                    c43 = s43["contracts"][0]
                    x43 = m43["exclusions"][0]["affected"]
                    if (value(c43["count_false"]), c43["runtime_result"], x43["clause_indices"]) != (1, "evaluated_false", [0]):
                        errors.append(f"AC-043: the excluded clause's false evaluation is not kept as evidence: {c43}, {x43}")
    if errors:
        print("coverage contracts: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage contracts: OK ({len(ENFORCE)} enforcement cases on native + VM, disabled mode strips them; clauses.sv0: "
          "C map = VM map, sv0cov-valid, clauses.expected, equal counts; AC-010 ordinary coverage unchanged; disabled and "
          "verified dispositions; AC-106 2/3 through merge + report; AC-043)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
