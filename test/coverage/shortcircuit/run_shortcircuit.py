#!/usr/bin/env python3
"""Short-circuit coverage branches from real runs (sv0cov CV-307; SPEC 11.5,
27.3; COV-MET-005, BL-055).

Every `&&` / `||` in a function body expression (match guards included, the
contract clauses requires/ensures/assert/loop_invariant excluded) is a
`short_circuit` branch with the outcomes `evaluated` (the right operand ran)
and `skipped`, and the right operand is also an expression region counted
like `evaluated`. On the native C backend and on the VM:

1. matrix.sv0 (both operators over all four truth combinations; mixed
   precedence, parentheses and left-associative chains; `let`, `if`, `while`,
   `return`, call argument, assignment, struct field, match arm, match guard,
   conditional expression; an `if` and a `match` as a right operand) exits 0
   on both backends with equal counts, point for point;
2. every short_circuit branch (and the listed if/loop/match/conditional
   branches) equals the hand-derived matrix.expected, keyed by function,
   kind, line and the exact span text; so do the entry counts it lists;
3. every region expression evaluates exact on both backends; every probe
   `mK();` has the entry count of mK; the right operand's expression region
   counts exactly the branch's `evaluated`;
4. every map is sv0cov-valid and every short_circuit branch names its
   outcomes exactly `evaluated`, `skipped` (ordinals 0, 1); the span of
   chosen branches is the byte range of the source text;
5. contract.sv0: `&&` / `||` in requires, ensures, assert and loop_invariant
   make no short_circuit branch, the one in the body does;
6. partial.sv0: `let r2: bool = ready && compute();` run with ready=false is
   a `partial` line, skipped covered and evaluated uncovered;
7. excl.sv0: `// sv0cov: ignore-next` on a statement holding `&&` excludes
   that branch; `#[coverage(branch_off, ...)]` on `if a && b { }` excludes
   only the if's outcomes and leaves the short_circuit ordinary (AC-042).

    python3 sv0c/test/coverage/shortcircuit/run_shortcircuit.py
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
sys.dont_write_bytecode = True
sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import build_vm, sv0, transport, vm_run  # noqa: E402

# From a map, one profile and the source: validate the map, then print JSON
# with every branch (function, kind, line, span text, outcome counts and
# classifications), entry counts, probe counts, inexact regions, the right
# operand check of each short_circuit branch, line statuses and exclusions.
REPORT = r"""
import json, sys
sys.path.insert(0, sys.argv[1])
from sv0cov.formats.map import validate_map
from sv0cov.resolve import resolve
from sv0cov.lines import region_count, region_counts, line_records
mb = open(sys.argv[2], 'rb').read()
src = open(sys.argv[4], 'rb').read()
srcs = {'main.sv0': src}
m = validate_map(mb, sources=srcs)
res = resolve(mb, [open(sys.argv[3], 'rb').read()], sources=srcs)
c = res.counts()
ents = m['entities']
entry = {ents[p['entity_index']]['qualified_name']: c[p['point_id']] for p in m['points'] if p['kind'] == 'function_entry'}
out = {'branches': [], 'entry': entry, 'probes': [], 'inexact': [], 'rhs': [], 'lines': {}, 'exclusions': []}
for b in m['branches']:
    sp = b['span']
    outs = {}
    for o in b['outcomes']:
        outs[o['name']] = {'cls': o['classification'], 'ord': o['ordinal'], 'count': c.get(o['point_id']),
                           'evidence': o['evidence_identity']}
    out['branches'].append({'func': ents[b['entity_index']]['qualified_name'], 'kind': b['kind'], 'line': sp['start_line'],
                            'start': sp['start_byte'], 'end': sp['end_byte'],
                            'text': src[sp['start_byte']:sp['end_byte']].decode(), 'outs': outs,
                            'index': b['branch_index']})
    if b['kind'] == 'short_circuit' and all(o['classification'] != 'excluded' for o in b['outcomes']):
        ev = c[b['outcomes'][0]['point_id']]
        cands = [r for r in m['regions'] if r['kind'] == 'expression' and r['span']['end_byte'] == sp['end_byte']
                 and r['span']['start_byte'] > sp['start_byte'] and r['classification'] == 'user']
        if not cands:
            out['rhs'].append({'line': sp['start_line'], 'ok': False, 'why': 'no right-operand expression region'})
        else:
            r = min(cands, key=lambda r: r['span']['start_byte'])
            n = region_count(r, c)
            out['rhs'].append({'line': sp['start_line'], 'ok': n == ev, 'why': f'right operand region {n} != evaluated {ev}'})
for r in m['regions']:
    text = src[r['span']['start_byte']:r['span']['end_byte']].decode()
    if r['kind'] == 'expression_statement' and text.startswith('m') and text.endswith('();') and text[1:-3].isdigit():
        out['probes'].append([text, region_count(r, c), entry.get(text[:-3])])
for i, rec in sorted(region_counts(m, res.context_counts()).items()):
    if rec.inexact:
        out['inexact'].append(i)
for rec in line_records(m, c, srcs):
    out['lines'][rec['line']] = rec['status']
for e in m['exclusions']:
    kinds = sorted(m['branches'][i]['kind'] for i in e['affected']['branch_indices'])
    out['exclusions'].append({'mode': e['mode'], 'origin': e['origin'], 'line': e['span']['start_line'],
                              'branches': e['affected']['branch_indices'], 'kinds': kinds})
out['sc_names'] = [[o['name'] for o in b['outcomes']] for b in m['branches'] if b['kind'] == 'short_circuit']
out['sc_ords'] = [[o['ordinal'] for o in b['outcomes']] for b in m['branches'] if b['kind'] == 'short_circuit']
print(json.dumps(out))
"""


def run_both(src: Path, errors: list[str], label: str) -> dict[str, dict]:
    """Build `src` instrumented for C and the VM, run both, return the
    REPORT of each backend (and record failures in `errors`)."""
    py = modern_python()
    reports: dict[str, dict] = {}
    with tempfile.TemporaryDirectory() as td:
        d = Path(td)
        (d / "vm").mkdir()
        (d / "main.sv0").write_bytes(src.read_bytes())
        exe, sv0b = d / "main", d / "vm" / "main.sv0b"
        p = sv0("native-compile", "--coverage=instrument", "-o", str(exe), str(d / "main.sv0"))
        err = build_vm([str(d / "main.sv0")], sv0b)
        if p.returncode or err:
            errors.append(f"{label}: build failed: {p.stderr.strip()[-300:]} {err or ''}")
            return reports
        for backend in ("native", "vm"):
            pdir = d / f"{backend}-profiles"
            pdir.mkdir()
            if backend == "native":
                rc, out = subprocess.run([str(exe)], capture_output=True, env=transport(pdir), timeout=60).returncode, ""
            else:
                rc, out = vm_run(sv0b, d / "vm" / "main.sv0covbind.json", transport(pdir))
            profiles = sorted(pdir.iterdir())
            if rc != 0 or len(profiles) != 1:
                errors.append(f"{label}: {backend}: exit {rc}, {len(profiles)} profiles {out[-300:]}")
                continue
            if py is None:
                continue
            mp = d / "main.sv0covmap.json" if backend == "native" else d / "vm" / "main.sv0covmap.json"
            r = subprocess.run([*py, "-c", REPORT, str(ROOT / "sv0cov" / "src"), str(mp), str(profiles[0]),
                                str(d / "main.sv0")], capture_output=True, text=True, timeout=120)
            if r.returncode:
                errors.append(f"{label}: {backend}: report/validation failed: {r.stderr.strip()[-500:]}")
                continue
            reports[backend] = json.loads(r.stdout)
    if len(reports) == 2 and reports["native"] != reports["vm"]:
        a, b = reports["native"], reports["vm"]
        errors.append(f"{label}: native and VM reports differ in: {[k for k in a if a[k] != b[k]]}")
    return reports


def count_str(o: dict) -> str:
    return "-" if o["count"] is None else str(o["count"])


def check_matrix(errors: list[str]) -> int:
    src = HERE / "matrix.sv0"
    text = src.read_text()
    want_b: dict[tuple, str] = {}
    want_e: dict[str, int] = {}
    for line in (HERE / "matrix.expected").read_text().splitlines():
        if not line or line.startswith("#"):
            continue
        f = line.split("\t")
        if f[0] == "entry":
            want_e[f[1]] = int(f[2])
        else:
            want_b[(f[0], f[1], int(f[2]), f[3].replace("\\n", "\n") if f[3] != "-" else "-")] = f[4]
    reports = run_both(src, errors, "matrix")
    for backend, rep in reports.items():
        got = {}
        for b in rep["branches"]:
            key = (b["func"], b["kind"], b["line"], b["text"] if b["kind"] == "short_circuit" else "-")
            if b["kind"] == "short_circuit" or key in want_b:
                got[key] = " ".join(f"{n}={count_str(o)}" for n, o in b["outs"].items())
        if got != want_b:
            diff = [f"{k}: got {got.get(k)} want {want_b.get(k)}" for k in sorted(set(got) | set(want_b), key=str)
                    if got.get(k) != want_b.get(k)]
            errors.append(f"matrix {backend}: outcomes differ from matrix.expected:\n    " + "\n    ".join(diff))
        for fn, n in want_e.items():
            if rep["entry"].get(fn) != n:
                errors.append(f"matrix {backend}: entry count of {fn} is {rep['entry'].get(fn)}, want {n}")
        if rep["inexact"]:
            errors.append(f"matrix {backend}: inexact region counts {rep['inexact']}")
        for t, cnt, ent in rep["probes"]:
            if cnt != ent:
                errors.append(f"matrix {backend}: probe {t} region count {cnt} != entry count {ent}")
        if len(rep["probes"]) != 8:
            errors.append(f"matrix {backend}: expected 8 probes, found {len(rep['probes'])}")
        for r in rep["rhs"]:
            if not r["ok"]:
                errors.append(f"matrix {backend}: line {r['line']}: {r['why']}")
        sc = [b for b in rep["branches"] if b["kind"] == "short_circuit"]
        if len(rep["rhs"]) != len(sc):
            errors.append(f"matrix {backend}: {len(rep['rhs'])} right-operand checks for {len(sc)} branches")
        if rep["sc_names"] != [["evaluated", "skipped"]] * len(sc) or rep["sc_ords"] != [[0, 1]] * len(sc):
            errors.append(f"matrix {backend}: short_circuit outcome names/ordinals are not evaluated(0), skipped(1)")
        for b in sc:
            if any(o["cls"] != "eligible" for o in b["outs"].values()):
                errors.append(f"matrix {backend}: line {b['line']} short_circuit outcomes not all eligible")
        # Spans: the byte range of the source text, independent of the plan.
        for fn, needle in (("or_op", "a || pick(b)"), ("and_op", "a && pick(b)"),
                           ("in_while", "i < n && pos(10 - i)"), ("rhs_if", "a && if pick(b) { rt() } else { rf() }")):
            start = text.index(needle)
            hit = [b for b in sc if b["func"] == fn and (b["start"], b["end"]) == (start, start + len(needle))]
            if len(hit) != 1:
                errors.append(f"matrix {backend}: no {fn} branch spans bytes {start}..{start + len(needle)} ({needle!r})")
        # The multiple branches of one line start at distinct (start, end) pairs.
        spans = [(b["start"], b["end"]) for b in sc]
        if len(set(spans)) != len(spans):
            errors.append(f"matrix {backend}: two short_circuit branches share a span")
    return len(want_b)


def check_contract(errors: list[str]) -> None:
    reports = run_both(HERE / "contract.sv0", errors, "contract")
    for backend, rep in reports.items():
        sc = [b for b in rep["branches"] if b["kind"] == "short_circuit"]
        if len(sc) != 1 or sc[0]["text"] != "a && pick(b)" or sc[0]["func"] != "both":
            errors.append(f"contract {backend}: want exactly the body's `a && pick(b)` as short_circuit, got "
                          f"{[(b['func'], b['line'], b['text']) for b in sc]}")
        elif [sc[0]["outs"][n]["count"] for n in ("evaluated", "skipped")] != [2, 0]:
            errors.append(f"contract {backend}: body && counts {sc[0]['outs']}")


def check_partial(errors: list[str]) -> None:
    reports = run_both(HERE / "partial.sv0", errors, "partial")
    line = 10  # let r2: bool = ready && compute();
    for backend, rep in reports.items():
        if rep["lines"].get(str(line)) != "partial":
            errors.append(f"partial {backend}: line {line} status {rep['lines'].get(str(line))!r}, want 'partial'")
        sc = [b for b in rep["branches"] if b["kind"] == "short_circuit"]
        if len(sc) != 1 or sc[0]["line"] != line:
            errors.append(f"partial {backend}: want one short_circuit on line {line}, got {[b['line'] for b in sc]}")
            continue
        o = sc[0]["outs"]
        if (o["skipped"]["count"], o["evaluated"]["count"]) != (1, 0):
            errors.append(f"partial {backend}: skipped={o['skipped']['count']} evaluated={o['evaluated']['count']}, "
                          "want skipped covered (1), evaluated uncovered (0)")


def check_exclusions(errors: list[str]) -> None:
    reports = run_both(HERE / "excl.sv0", errors, "exclusions")
    for backend, rep in reports.items():
        br = {(b["kind"], b["line"]): b for b in rep["branches"]}
        want = {
            ("short_circuit", 12): ("excluded", "excluded"),   # ignore-next statement
            ("if", 14): ("excluded", "excluded"),              # branch_off: the if only
            ("short_circuit", 14): ("eligible", "eligible"),   # AC-042: stays ordinary
            ("short_circuit", 17): ("eligible", "eligible"),
        }
        if set(br) != set(want):
            errors.append(f"exclusions {backend}: branches {sorted(br)} want {sorted(want)}")
            continue
        for k, (c1, c2) in want.items():
            got = tuple(o["cls"] for o in br[k]["outs"].values())
            if got != (c1, c2):
                errors.append(f"exclusions {backend}: {k} classifications {got}, want {(c1, c2)}")
        for k, cnt in ((("short_circuit", 14), (1, 0)), (("short_circuit", 17), (0, 1))):
            got = tuple(o["count"] for o in br[k]["outs"].values())
            if got != cnt:
                errors.append(f"exclusions {backend}: {k} counts {got}, want {cnt}")
        ex = {(e["mode"], e["line"]): e for e in rep["exclusions"]}
        off, boff = ex.get(("off", 11)), ex.get(("branch_off", 13))
        if not off or off["kinds"] != ["short_circuit"] or off["branches"] != [br[("short_circuit", 12)]["index"]]:
            errors.append(f"exclusions {backend}: ignore-next record {off}")
        if not boff or boff["kinds"] != ["if"] or boff["branches"] != [br[("if", 14)]["index"]]:
            errors.append(f"exclusions {backend}: branch_off record {boff} (AC-042: only the if)")


def main() -> int:
    ensure_built()
    if modern_python() is None:
        print("coverage shortcircuit: OK (skipped: no Python >= 3.10)")
        return 0
    errors: list[str] = []
    n = check_matrix(errors)
    check_contract(errors)
    check_partial(errors)
    check_exclusions(errors)
    if errors:
        print("coverage shortcircuit: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage shortcircuit: OK (native + VM, equal counts; {n} hand-derived branches in matrix.expected, "
          "right-operand regions = evaluated, contract clauses plan no branch, partial line, exclusions)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
