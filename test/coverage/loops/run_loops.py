#!/usr/bin/env python3
"""Loop branches and loop edge cases from real runs (sv0cov CV-203; SPEC
11.5, COV-MET-004, BL-022).

edges.sv0 drives every loop form through zero, one, and many iterations:
`while` (with a loop invariant), `for` (break and continue in nested ifs),
`loop` left by `break` and left only by `return`, `while true` left by
`break`, `return` from a nested loop, an inner `break` under an outer
`continue`, and `break` / `continue` from match arms. On the native C
backend and on the VM:

1. both exit 0 and their counts are equal, point for point;
2. every loop and match outcome equals the hand-derived edges.expected;
   the exits of `loop { }` and `while true { }` are statically_unreachable
   (uncounted, with their evidence identity), every other exit is counted;
3. self-check of the derived region counts: each probe statement `mK();`
   calls a function that only returns, so its region count (an expression
   over entry and outcome points, sv0cov.resolve) must equal mK's own
   function_entry count. All 28 probes are checked, including code after a
   loop left by break, by return, and never left normally.

    python3 sv0c/test/coverage/loops/run_loops.py
"""

from __future__ import annotations

import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
PROBES = 28
EVIDENCE = {"loop": "sv0c:loop:no-condition", "while": "sv0c:loop:condition-literal-true"}

sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import build_vm, sv0, transport, vm_run  # noqa: E402

# Print, from a map and one profile: each branch's outcomes, then each probe
# statement's region count next to its function's entry count.
REPORT = r"""
import json, sys
sys.path.insert(0, sys.argv[1])
from sv0cov.resolve import resolve
from sv0cov.lines import region_count
mb = open(sys.argv[2], 'rb').read()
src = open(sys.argv[4], 'rb').read()
m = json.loads(mb)
c = resolve(mb, [open(sys.argv[3], 'rb').read()], sources={'main.sv0': src}).counts()
ents = m['entities']
entry = {ents[p['entity_index']]['qualified_name']: c[p['point_id']] for p in m['points'] if p['kind'] == 'function_entry'}
for b in m['branches']:
    outs = []
    for o in b['outcomes']:
        if o['classification'] == 'statically_unreachable':
            outs.append(f"{o['name']}=-:{o['evidence_identity']}")
        else:
            outs.append(f"{o['name']}={c[o['point_id']]}")
    print('branch', ents[b['entity_index']]['qualified_name'], b['kind'], b['span']['start_line'], *outs)
for r in m['regions']:
    text = src[r['span']['start_byte']:r['span']['end_byte']].decode()
    if r['kind'] == 'expression_statement':
        print('probe', text, region_count(r, c), entry.get(text.split('(')[0]))
"""


def main() -> int:
    ensure_built()
    errors: list[str] = []
    py = modern_python()
    src = HERE / "edges.sv0"
    lines = src.read_text().splitlines()
    want = {}
    for line in (HERE / "edges.expected").read_text().splitlines():
        if line and not line.startswith("#"):
            f = line.split()
            want[(f[0], f[1], int(f[2]))] = f[3:]
    checked = []
    with tempfile.TemporaryDirectory() as td:
        d = Path(td)
        (d / "vm").mkdir()
        (d / "main.sv0").write_bytes(src.read_bytes())
        exe, sv0b = d / "main", d / "vm" / "main.sv0b"
        p = sv0("native-compile", "--coverage=instrument", "-o", str(exe), str(d / "main.sv0"))
        err = build_vm([str(d / "main.sv0")], sv0b)
        if p.returncode or err:
            print(f"coverage loops: FAIL\n  - build failed: {p.stderr.strip()} {err or ''}", file=sys.stderr)
            return 1
        reports = {}
        for backend in ("native", "vm"):
            pdir = d / f"{backend}-profiles"
            pdir.mkdir()
            if backend == "native":
                rc, out = subprocess.run([str(exe)], capture_output=True, env=transport(pdir), timeout=60).returncode, ""
            else:
                rc, out = vm_run(sv0b, d / "vm" / "main.sv0covbind.json", transport(pdir))
            profiles = sorted(pdir.iterdir())
            if rc != 0 or len(profiles) != 1:
                errors.append(f"{backend}: exit {rc}, {len(profiles)} profiles {out[-300:]}")
                continue
            if py is None:
                continue
            mp = d / "main.sv0covmap.json" if backend == "native" else d / "vm" / "main.sv0covmap.json"
            r = subprocess.run([*py, "-c", REPORT, str(ROOT / "sv0cov" / "src"), str(mp), str(profiles[0]),
                                str(d / "main.sv0")], capture_output=True, text=True, timeout=120)
            if r.returncode:
                errors.append(f"{backend}: report failed: {r.stderr.strip()[-400:]}")
                continue
            reports[backend] = r.stdout.splitlines()
        if len(reports) == 2 and reports["native"] != reports["vm"]:
            errors.append("native and VM outcomes/probes differ:\n    " +
                          "\n    ".join(f"{a} | {b}" for a, b in zip(reports["native"], reports["vm"]) if a != b))
        for backend, rep in reports.items():
            got = {}
            for line in rep:
                f = line.split()
                if f[0] == "branch":
                    key = (f[1], f[2], int(f[3]))
                    keyword = lines[key[2] - 1].split()[0]
                    outs = []
                    for o in f[4:]:
                        name, _, val = o.partition("=")
                        if val.startswith("-:"):
                            if val[2:] != EVIDENCE.get(keyword):
                                errors.append(f"{backend}: {key} {name} has evidence {val[2:]!r} (a `{keyword}`)")
                            val = "-"
                        outs.append(f"{name}={val}")
                    if key in want or f[2] != "if":
                        got[key] = outs
                else:
                    probe, count, entry = f[1], f[2], f[3]
                    if count != entry:
                        errors.append(f"{backend}: probe {probe} region count {count} != {probe[:-3]} entry count {entry}")
                    checked.append(probe)
            if got != want:
                diff = [f"{k}: got {got.get(k)} want {want.get(k)}" for k in sorted(set(got) | set(want)) if got.get(k) != want.get(k)]
                errors.append(f"{backend}: loop/match outcomes differ from edges.expected:\n    " + "\n    ".join(diff))
        if reports and len(set(checked)) != PROBES:
            errors.append(f"expected {PROBES} probe statements, checked {sorted(set(checked))}")

    if errors:
        print("coverage loops: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    note = "outcomes match edges.expected; 28 probe counts = their entry counts" if py else "count checks SKIPPED (no Python >= 3.10)"
    print(f"coverage loops: OK (native + VM, equal counts; {note})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
