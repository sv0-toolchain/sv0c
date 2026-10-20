#!/usr/bin/env python3
"""Exclusion audit and caps on real evidence (sv0cov CV-314; SPEC 19.5,
18.2.2; COV-EXC-005, COV-EXC-025, COV-POL-025).

Two programs are built instrumented by the real native compiler and run:
mixed.sv0 (every exclusion form) and ../contracts/clauses.sv0 (a
`contract_off` function that runs). Their profiles go through `sv0cov merge`
and `sv0cov report`, then:

1. the native report keeps every exclusion record, one per `reason` in the
   source; the terminal and HTML reports give the excluded function,
   region, line, branch and contract counts and, under each record, what it
   removes and whether its code ran (COV-EXC-005);
2. COV-EXC-025: the `contract_off` clause keeps its evaluation evidence in
   the audit with its owner: `trusted#requires:0 true 1 false 0`;
3. COV-POL-025: `sv0cov check` decides every metric cap at its exact
   boundary (the least passing basis points pass, one less fails with
   COV4101), the record cap at the record count, a forbidden reason by
   exact match, and a forbidden source by the Section 18.3 matcher.

    python3 sv0c/test/coverage/exclusions/run_audit.py
"""

from __future__ import annotations

import json
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
sys.dont_write_bytecode = True
sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import sv0, transport  # noqa: E402

PROGRAMS = {"mixed": HERE / "mixed.sv0", "clauses": SV0C / "test" / "coverage" / "contracts" / "clauses.sv0"}

AUDIT = r"""
import io, json, re, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov import cli
from sv0cov.formats.html import check_html
from sv0cov.model.report import excluded_units, read_report
d = Path(sys.argv[2])
METRICS = ("branch", "contract", "function", "line", "region")
errors = []

def cmd(*a):
    o, e = io.StringIO(), io.StringIO()
    rc = cli.main([a[0], "--diagnostic-format=json-lines", *a[1:]], stdout=o, stderr=e, cwd=d)
    return rc, o.getvalue(), [json.loads(x)["code"] for x in e.getvalue().splitlines() if x.startswith("{")]

rc, _, codes = cmd("merge", "in", "--output", "m.sv0covdata.json")
if rc:
    sys.exit(f"merge: {rc} {codes}")
for fmt, name in (("json", "r.json"), ("html", "r.html")):
    rc, _, codes = cmd("report", "--format", fmt, "--output", name, "m.sv0covdata.json")
    if rc:
        sys.exit(f"report {fmt}: {rc} {codes}")
rc, term, codes = cmd("report", "--format", "term", "--output", "-", "m.sv0covdata.json")
r = read_report((d / "r.json").read_bytes())
text = (d / "main.sv0").read_text()
reasons = re.findall(r'reason ?= ?"([^"]*)"', text)
if sorted(x["reason"] for x in r["exclusions"]) != sorted(reasons):
    errors.append(f"the report keeps {len(r['exclusions'])} exclusion records, the source has {len(reasons)} reasons")
body = term.split("Exclusions\n----------\n", 1)[1].split("\n\n", 1)[0].splitlines()
units = {}
for x in r["exclusions"]:
    for k in ("region_indices", "line_indices", "branch_indices", "clause_indices"):
        units.setdefault(k, set()).update(x["affected"][k])
fns = {i for x in r["exclusions"] for i in x["affected"]["entity_indices"] if r["entities"][i]["kind"] == "function"}
want = (f"excluded: {len(fns)} function{'s' * (len(fns) != 1)}, {len(units['region_indices'])} region{'s' * (len(units['region_indices']) != 1)}, "
        f"{len(units['line_indices'])} line{'s' * (len(units['line_indices']) != 1)}, "
        f"{len(units['branch_indices'])} branch{'es' * (len(units['branch_indices']) != 1)}, "
        f"{len(units['clause_indices'])} contract{'s' * (len(units['clause_indices']) != 1)}")
if body[0] != want:
    errors.append(f"terminal totals {body[0]!r}, want {want!r}")
records = body[2:]
if len(records) != 2 * len(r["exclusions"]) or not all(x.startswith("    removes ") for x in records[1::2]):
    errors.append("the terminal report does not follow every exclusion record with what it removes")
page = (d / "r.html").read_bytes()
check_html(page)
if page.count(b"removes ") < len(r["exclusions"]):
    errors.append("the HTML report does not audit every exclusion record")
audit = "\n".join(records)

# COV-POL-025 at the exact boundaries, through check.
def check(toml):
    (d / "sv0.toml").write_text('[tool.sv0cov]\nschema = "1.0"\n[tool.sv0cov.exclusions]\n' + toml)
    rc, _, codes = cmd("check", "m.sv0covdata.json")
    (d / "sv0.toml").unlink()
    return rc, codes

caps = 0
for m in METRICS:
    n = excluded_units(r, m)
    pre = r["summary"][m]["total"] + n
    if n == 0 or pre == 0:
        continue
    least = -(-n * 10000 // pre)
    caps += 1
    if check(f"{m}_basis_points = {least}\n") != (0, []):
        errors.append(f"{m} cap {least}bp (exactly {n}/{pre}) does not pass")
    if check(f"{m}_basis_points = {least - 1}\n") != (1, ["COV4101"]):
        errors.append(f"{m} cap {least - 1}bp does not fail with COV4101")
n = len(r["exclusions"])
if check(f"max_records = {n}\n") != (0, []) or check(f"max_records = {n - 1}\n") != (1, ["COV4101"]):
    errors.append(f"the record cap is not decided at {n} records")
reason = r["exclusions"][0]["reason"]
if check(f"forbidden_reasons = [{json.dumps(reason)}]\n") != (1, ["COV4101"]) or \
        check(f"forbidden_reasons = [{json.dumps(reason + 'x')}]\n") != (0, []):
    errors.append("a forbidden reason is not matched exactly")
if check('forbidden_sources = ["*.sv0"]\n') != (1, ["COV4101"]) or check('forbidden_sources = ["other/"]\n') != (0, []):
    errors.append("a forbidden source pattern is not applied")
print(json.dumps({"errors": errors, "audit": audit, "records": n, "caps": caps}))
"""


def main() -> int:
    ensure_built()
    py = modern_python()
    if py is None:
        print("coverage exclusion audit: OK (skipped: no Python >= 3.10)")
        return 0
    errors: list[str] = []
    notes = []
    with tempfile.TemporaryDirectory() as td:
        for name, src in PROGRAMS.items():
            d = Path(td) / name
            (d / "in").mkdir(parents=True)
            (d / "main.sv0").write_bytes(src.read_bytes())
            exe = d / "x"
            p = sv0("native-compile", "--coverage=instrument", "--coverage-map", str(d / "in" / "x.sv0covmap.json"), "-o", str(exe),
                    str(d / "main.sv0"))
            if p.returncode:
                errors.append(f"{name}: build failed: {p.stderr.strip()[-300:]}")
                continue
            run = subprocess.run([str(exe)], capture_output=True, env=transport(d / "in"), timeout=60)
            if run.returncode != 0:
                errors.append(f"{name}: the program exited {run.returncode}")
                continue
            p = subprocess.run([*py, "-c", AUDIT, str(ROOT / "sv0cov" / "src"), str(d)], capture_output=True, text=True, timeout=600)
            if p.returncode:
                errors.append(f"{name}: {p.stdout.strip()[-300:]} {p.stderr.strip()[-600:]}")
                continue
            out = json.loads(p.stdout)
            errors += [f"{name}: {e}" for e in out["errors"]]
            notes.append(f"{name} {out['records']} records, {out['caps']} metric caps")
            if name == "clauses" and "trusted#requires:0 true 1 false 0" not in out["audit"]:
                errors.append(f"clauses: the contract_off clause's evidence is not in the audit: {out['audit']!r}")
    if errors:
        for e in errors:
            print(f"coverage exclusion audit: FAIL: {e}", file=sys.stderr)
        return 1
    print(f"coverage exclusion audit: OK ({'; '.join(notes)}: inventory, terminal/HTML audit, contract_off evidence, "
          "caps at exact boundaries, forbidden reasons and sources)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
