#!/usr/bin/env python3
"""VM raw profiles (sv0cov CV-121) and native/VM count parity.

Through the real drivers (`sv0 native-compile --coverage=instrument`,
`sv0 vm-native-compile --coverage=instrument`, `sv0 vm-run
--coverage-binding`), for the seven sv0cov semantic fixtures and
constructs.sv0:

1. sv0vm publishes exactly one raw profile into SV0COV_PROFILE_DIR, named
   <run_id>-<profile_id>.sv0profraw, mode 0600, that sv0cov's reader
   accepts for the program's map with backend vm-v1 and the run ID;
2. its counts equal the native executable's profile for the same program
   (same target name, so the same map), counter for counter, and the
   fixtures' hand-reviewed expected-counts.json;
2b. sv0cov's reader (sv0cov.resolve, CV-170; run with the first Python >=
   3.10 found) resolves the native (CV-115) and VM (CV-121) profiles through
   the map to the same per-point counts, equal to expected-counts.json, and
   the two together to exactly double;
2c. sv0cov's parity comparator (sv0cov.parity, CV-171, F0-G4: AC-001 same
   point identities and semantic metadata, AC-002 same counts) passes for
   every program; f0's (CV-027) report is written to
   build/coverage-parity/f0.json.

And with small programs:

3. SV0COV_CONTEXT reaches the profile; a contract failure (exit 1) still
   publishes, and so does an integer division by zero (a defined panic); a
   VM crash (truncated bytecode) publishes nothing and leaves no temporary;
4. a required-mode transport failure (an unknown SV0COV_* name, a bad run
   ID) stops the program before it prints anything, with COV2001 and no
   profile; outside sv0cov (no transport, not required) the program runs
   as usual, notes COV2001, and publishes nothing.

SML-level byte parity with the CV-024 goldens, flush, and collision tests
are in sv0vm's test/coverage_test.sml; this script also checks that
sv0vm's golden copies equal sv0cov's.

    python3 sv0c/test/coverage/plan/run_vm_profile.py
"""

from __future__ import annotations

import hashlib
import json
import os
import re
import shutil
import stat
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
SV0 = ROOT / "scripts" / "sv0"
FIXTURES = ROOT / "sv0cov" / "tests" / "fixtures" / "semantic"
RUN_ID = "0123456789abcdef0123456789abcdef"
PARITY_DIR = ROOT / "build" / "coverage-parity"

sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / "sv0cov" / "src"))
from run_hits import programs  # noqa: E402
from run_plan import ensure_built, modern_python  # noqa: E402
from sv0cov.formats.rawprofile import RawProfileError, decode  # noqa: E402


def sv0(*args: str) -> subprocess.CompletedProcess:
    return subprocess.run([str(SV0), *args], capture_output=True, text=True, timeout=600)


def transport(pdir: Path, **over: str | None) -> dict[str, str]:
    env = {k: v for k, v in os.environ.items() if not k.startswith("SV0COV")}
    env.update({"SV0COV_PROFILE_DIR": str(pdir), "SV0COV_RUN_ID": RUN_ID, "SV0COV_REQUIRED": "1"})
    for k, v in over.items():
        if v is None:
            env.pop(k, None)
        else:
            env[k] = v
    return env


def vm_run(sv0b: Path, binding: Path, env: dict[str, str]) -> tuple[int | None, str]:
    """(program exit or None, combined output) of `sv0 vm-run --coverage-binding`."""
    p = subprocess.run([str(SV0), "vm-run", "--coverage-binding", str(binding), str(sv0b)],
                       capture_output=True, text=True, env=env, timeout=120)
    out = p.stdout + p.stderr
    m = re.search(r"vm_exit:([~-]?\d+)", out)
    return (int(m.group(1).replace("~", "-")) & 0xFF if m else None), out


def build_vm(request: list[str], out: Path) -> str | None:
    p = sv0("vm-native-compile", "--coverage=instrument", *request, str(out))
    return None if p.returncode == 0 else p.stderr.strip()


def profile(pdir: Path, m: dict, errors: list[str], label: str):
    files = sorted(pdir.iterdir())
    if len(files) != 1 or not files[0].name.startswith(RUN_ID + "-") or files[0].suffix != ".sv0profraw":
        errors.append(f"{label}: expected one published profile, found {[f.name for f in files]}")
        return None
    if stat.S_IMODE(files[0].stat().st_mode) != 0o600:
        errors.append(f"{label}: the profile is not mode 0600")
    try:
        prof = decode(files[0].read_bytes(), map_counter_count=m["program_counter_count"],
                      expected_map_id=bytes.fromhex(m["map_id"]))
    except RawProfileError as exc:
        errors.append(f"{label}: invalid profile: {exc}")
        return None
    if prof.run_id.hex() != RUN_ID or files[0].name != f"{RUN_ID}-{prof.profile_id.hex()}.sv0profraw":
        errors.append(f"{label}: the profile's run/profile IDs do not match its name")
    return prof


def dense(prof, n: int) -> list[int]:
    out = [0] * n
    for i, c in prof.counts:
        out[i] = c
    return out


def main() -> int:
    ensure_built()
    errors: list[str] = []
    progs = programs()
    resolved: list[tuple[str, str, str, str]] = []
    reader = "skipped (no Python >= 3.10)"
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)
        # 0. sv0vm's golden copies are sv0cov's.
        for g in (ROOT / "sv0cov" / "tests" / "fixtures" / "rawprofile").glob("*.sv0profraw"):
            copy = ROOT / "sv0vm" / "test" / "fixtures" / "rawprofile" / g.name
            if not copy.is_file() or copy.read_bytes() != g.read_bytes():
                errors.append(f"sv0vm/test/fixtures/rawprofile/{g.name} differs from sv0cov's golden")

        # 1-2. Every program: one valid VM profile, equal to the native one.
        for name, request in progs:
            args = request.split(" ", 1) if request.startswith("--project") else [request]
            d = t / name
            d.mkdir()
            # The VM build goes in its own directory, so each backend keeps
            # its own map (same target name: <name>) for the identity check.
            (d / "vm").mkdir()
            exe, sv0b = d / name, d / "vm" / f"{name}.sv0b"
            nmap, vmap = d / f"{name}.sv0covmap.json", d / "vm" / f"{name}.sv0covmap.json"
            p = sv0("native-compile", "--coverage=instrument", "-o", str(exe), *args)
            err = build_vm(args, sv0b)
            if p.returncode or err:
                errors.append(f"{name}: build failed: {p.stderr.strip()} {err or ''}")
                continue
            if nmap.read_bytes() != vmap.read_bytes():
                errors.append(f"{name}: the generated-C and VM maps differ (same compiler, same target)")
            m = json.loads(nmap.read_bytes())
            mv = json.loads(vmap.read_bytes())
            npdir, vpdir = d / "native-profiles", d / "vm-profiles"
            npdir.mkdir()
            vpdir.mkdir()
            rn = subprocess.run([str(exe)], capture_output=True, env=transport(npdir), timeout=60)
            rc, out = vm_run(sv0b, d / "vm" / f"{name}.sv0covbind.json", transport(vpdir))
            if rc is None or rc != rn.returncode:
                errors.append(f"{name}: native exit {rn.returncode}, VM exit {rc}: {out[-300:]}")
            native = profile(npdir, m, errors, f"{name} native")
            vm = profile(vpdir, mv, errors, f"{name} VM")
            if native is None or vm is None:
                continue
            if vm.backend != "vm-v1" or native.backend != "native":
                errors.append(f"{name}: backends {native.backend}/{vm.backend}, want native/vm-v1")
            n = m["program_counter_count"]
            if dense(vm, n) != dense(native, n):
                errors.append(f"{name}: VM counts {dense(vm, n)} differ from native {dense(native, n)}")
            resolved.append((name, str(nmap), str(next(npdir.iterdir())), str(vmap), str(next(vpdir.iterdir()))))
            exp_path = FIXTURES / name / "expected-counts.json"
            if exp_path.is_file():
                counts = dense(vm, n)
                by_id = {pt["point_id"]: counts[pt["counter_index"]] for pt in m["points"]
                         if pt.get("counter_index") is not None}
                for e in json.loads(exp_path.read_bytes())["counts"]:
                    if by_id.get(e["point_id"]) != e["count"]:
                        errors.append(f"{name}: VM counted {e['label']} {by_id.get(e['point_id'])}, expected {e['count']}")
            elif sum(dense(vm, n)) == 0:
                errors.append(f"{name}: the VM profile has no counts")

        # 2b. CV-170: sv0cov's own reader resolves both backends' profiles
        # through the map to the expected per-point counts.
        py = modern_python()
        if py is not None and resolved:
            code = (
                "import json, sys\n"
                "sys.path.insert(0, sys.argv[1])\n"
                "from pathlib import Path\n"
                "from sv0cov.resolve import resolve\n"
                "bad = []\n"
                "regions_checked = []\n"                "models_checked = []\n"                "indexed_checked = []\n"
                "import hashlib\n"
                "SRC = {hashlib.sha256(q.read_bytes()).hexdigest(): q.read_bytes() for q in Path(sys.argv[3]).rglob('*.sv0')}\n"
                "reports_checked = []\n"
                "terminal_checked = []\n"
                "check_checked = []\n"
                "GOLD = Path(sys.argv[1]).parent / 'tests' / 'fixtures' / 'terminal'\n"
                "for name, nm, nat, vmm, vm in json.loads(sys.argv[2]):\n"
                "    mb, vmb = Path(nm).read_bytes(), Path(vmm).read_bytes()\n"
                "    a = resolve(mb, [Path(nat).read_bytes()]).counts()\n"
                "    b = resolve(vmb, [Path(vm).read_bytes()]).counts()\n"
                "    both = resolve(mb, [Path(nat).read_bytes(), Path(vm).read_bytes()])\n"
                "    if a != b: bad.append(f'{name}: native and VM resolve differently')\n"
                "    if both.counts() != {k: 2 * v for k, v in a.items()} or both.backends != ('native', 'vm-v1'):\n"
                "        bad.append(f'{name}: native + VM did not add up')\n"
                "    exp = Path(sys.argv[3]) / name / 'expected-counts.json'\n"
                "    if exp.is_file():\n"
                "        want = {e['point_id']: e['count'] for e in json.loads(exp.read_bytes())['counts']}\n"
                "        if a != want: bad.append(f'{name}: resolved counts differ from expected-counts.json')\n"
                "    # CV-204: every region expression sv0c emitted evaluates (sv0cov.expr,\n"
                "    # per context then aggregated) to the same nonnegative exact count on\n"
                "    # both backends, and to twice that over both profiles.\n"
                "    from sv0cov.lines import region_counts\n"
                "    from sv0cov.expr import ExprError\n"
                "    m = json.loads(mb)\n"
                "    try:\n"
                "        rn = region_counts(m, resolve(mb, [Path(nat).read_bytes()]).context_counts())\n"
                "        rv = region_counts(json.loads(vmb), resolve(vmb, [Path(vm).read_bytes()]).context_counts())\n"
                "        r2 = region_counts(m, both.context_counts())\n"
                "    except ExprError as exc:\n"
                "        bad.append(f'{name}: region evaluation failed: {exc}')\n"
                "    else:\n"
                "        if any(c.inexact for c in rn.values()): bad.append(f'{name}: an exact run gave an inexact region count')\n"
                "        if {i: c.value for i, c in rn.items()} != {i: c.value for i, c in rv.items()}:\n"
                "            bad.append(f'{name}: native and VM region counts differ')\n"
                "        if {i: c.value for i, c in r2.items()} != {i: 2 * c.value for i, c in rn.items()}:\n"
                "            bad.append(f'{name}: native + VM region counts did not add up')\n"
                "        regions_checked.append(len(rn))\n"
                "    from sv0cov.parity import compare\n"
                "    from sv0cov.formats.canonical_json import encode\n"
                "    rep = compare(name, mb, [Path(nat).read_bytes()], vmb, [Path(vm).read_bytes()])\n"
                "    if rep['status'] != 'pass': bad.append(f'{name}: parity report failed: {rep}')\n"
                "    if name == 'f0':\n"
                "        out = Path(sys.argv[4]); out.mkdir(parents=True, exist_ok=True)\n"
                "        (out / 'f0.json').write_bytes(encode(rep))\n"
                "    # CV-221: the semantic model built from each backend's real profile is\n"
                "    # the fixture's hand-derived expected-semantic.json (only map_ids differs:\n"
                "    # the fixture map names the fixture compiler identity).\n"
                "    from sv0cov.formats.map import validate_map\n"
                "    from sv0cov.formats.rawprofile import decode\n"
                "    from sv0cov.formats.indexed import check_semantic_structure\n"
                "    from sv0cov.model.semantic import build_semantic\n"
                "    def model(map_bytes, raws):\n"
                "        mm = validate_map(map_bytes)\n"
                "        profs = [decode(Path(r).read_bytes(), map_counter_count=mm['program_counter_count'],\n"
                "                        expected_map_id=bytes.fromhex(mm['map_id'])) for r in raws]\n"
                "        return check_semantic_structure(build_semantic([mm], profs))\n"
                "    sn, sv, s2 = model(mb, [nat]), model(vmb, [vm]), model(mb, [nat, vm])\n"
                "    if sn != sv: bad.append(f'{name}: the semantic models of the native and VM runs differ')\n"
                "    if not sn['complete'] or sn['map_ids'] != [m['map_id']]: bad.append(f'{name}: semantic model incomplete or wrong map_ids')\n"
                "    if [p['count']['value'] for p in s2['points']] != [2 * p['count']['value'] for p in sn['points']] \\\n"
                "            or [l['status'] for l in s2['lines']] != [l['status'] for l in sn['lines']]:\n"
                "        bad.append(f'{name}: the model of native + VM profiles is not the doubled model')\n"
                "    exps = Path(sys.argv[3]) / name / 'expected-semantic.json'\n"
                "    if exps.is_file():\n"
                "        want = json.loads(exps.read_bytes())\n"
                "        if {**sn, 'map_ids': want['map_ids']} != want:\n"
                "            keys = [k for k in want if k != 'map_ids' and sn[k] != want[k]]\n"
                "            bad.append(f'{name}: semantic model differs from expected-semantic.json in {keys}')\n"
                "        models_checked.append(name)\n"
                "    # CV-222: the full indexed object from the real artifacts reads back valid\n"
                "    # and complete; a native run and a VM run give byte-identical semantic\n"
                "    # projections (COV-MRG-008) while their transport records differ.\n"
                "    # CV-223: the evidence comes from the merge engine, which classifies the\n"
                "    # real artifacts itself.\n"
                "    from sv0cov.model.indexed import read_indexed\n"
                "    from sv0cov.model.merge import Artifact, merge, select_runs\n"
                "    def arts(paths): return [Artifact(Path(q).name, Path(q).read_bytes()) for q in paths]\n"
                "    def indexed(pairs, **kw):\n"
                "        res = merge(arts([a for a, _ in pairs]), arts([b for _, b in pairs]), **kw)\n"
                "        if read_indexed(res.data) != res.indexed: bad.append(f'{name}: merge bytes do not read back')\n"
                "        if res.complete != res.indexed['semantic']['complete'] or res.findings and res.complete:\n"
                "            bad.append(f'{name}: merge result disagrees with its evidence')\n"
                "        return res.indexed\n"
                "    runs = sorted({decode(Path(q).read_bytes()).run_id.hex() for q in (nat, vm)})\n"
                "    xn, xv = indexed([(nm, nat)]), indexed([(vmm, vm)])\n"
                "    xb = indexed([(nm, nat), (vmm, vm)], run_selection=select_runs(runs))\n"
                "    if xb['run_selection']['mode'] != 'explicit-list' or xb['run_selection']['selected_run_ids'] != runs:\n"
                "        bad.append(f'{name}: the declared run set was not recorded')\n"
                "    # CV-227: the merge command over the same files publishes exactly that evidence.\n"
                "    import io, tempfile\n"
                "    from sv0cov import cli\n"
                "    with tempfile.TemporaryDirectory() as td:\n"
                "        o, e = io.StringIO(), io.StringIO()\n"
                "        rc = cli.main(['merge', nm, nat, vmm, vm, '--expect-run', runs[0], '--output', 'out.json'], stdout=o, stderr=e, cwd=Path(td))\n"
                "        if rc != 0 or e.getvalue() or read_indexed((Path(td) / 'out.json').read_bytes()) != xb:\n"
                "            bad.append(f'{name}: sv0cov merge exit {rc}, or its output differs from the engine: {e.getvalue()[:200]}')\n"
                "        rc2 = cli.main(['merge', nm, nat, '--output', 'out.json'], stdout=io.StringIO(), stderr=io.StringIO(), cwd=Path(td))\n"
                "        if rc2 != 5: bad.append(f'{name}: sv0cov merge replaced an existing output (exit {rc2})')\n"
                "    cut = merge(arts([nm]), arts([nat]) + [Artifact('cut.sv0profraw', Path(vm).read_bytes()[:-6])])\n"
                "    if cut.complete or cut.publishable or [r['status'] for r in cut.indexed['profile_inputs']] != ['accepted', 'incomplete']:\n"
                "        bad.append(f'{name}: a truncated real profile was not inventoried as incomplete')\n"

                "    for label, x in (('native', xn), ('VM', xv), ('native + VM', xb)):\n"
                "        if not x['semantic']['complete'] or x['diagnostics']: bad.append(f'{name}: {label} indexed evidence is incomplete')\n"
                "    if xn['semantic_projection_sha256'] != xv['semantic_projection_sha256']:\n"
                "        bad.append(f'{name}: native and VM runs have different semantic projections')\n"
                "    if xn['profile_inputs'] == xv['profile_inputs'] or [r['backend'] for r in xb['profile_inputs']] not in (['native', 'vm-v1'], ['vm-v1', 'native']):\n"
                "        bad.append(f'{name}: the transport inventory does not tell the two runs apart')\n"
                "    if len(xb['map_inputs']) != 1 or len(xb['maps']) != 1 or len(xb['maps'][0]['bindings']) != m['program_counter_count']:\n"
                "        bad.append(f'{name}: the two identical maps were not inventoried once with a full binding')\n"
                "    # CV-229: a native report from the real evidence and the program's own source reads back,\n"
                "    # passes, and is the same for the native and the VM run apart from transport.\n"
                "    from sv0cov.config_resolve import resolve as resolve_config\n"
                "    from sv0cov.model.report import build_report, read_report\n"
                "    srcs = {s['path']: SRC.get(s['digest']) for s in xn['semantic']['sources']}\n"
                "    if all(v is not None for v in srcs.values()):\n"
                "        cfg = resolve_config('report', [('--format', 'json')])\n"
                "        rn, rv = (read_report(encode(build_report(encode(x), cfg, srcs))) for x in (xn, xv))\n"
                "        same = ('summary', 'points', 'regions', 'lines', 'branches', 'contracts', 'policy', 'evidence_eligibility')\n"
                "        if rn['evidence_eligibility'] != 'passing' or any(rn[k] != rv[k] for k in same) \\\n"
                "                or [e['summary'] for e in rn['entities']] != [e['summary'] for e in rv['entities']]:\n"
                "            bad.append(f'{name}: the native and VM reports differ, or are not passing evidence')\n"
                "        if rn['provenance']['backends'] == rv['provenance']['backends']:\n"
                "            bad.append(f'{name}: the reports do not record their backends')\n"
                "        # CV-230: the terminal report of the real run is the checked-in golden from its\n"
                "        # summary on, has no control sequence, and differs across backends only in the backend line.\n"
                "        from sv0cov.formats.terminal import render_terminal\n"
                "        tn, tv = render_terminal(rn), render_terminal(rv)\n"
                "        gold = (GOLD / f'fixture-{name}.txt').read_text(encoding='utf-8')\n"
                "        if tn[tn.index('\\nSummary\\n'):] != gold[gold.index('\\nSummary\\n'):]:\n"
                "            bad.append(f'{name}: the terminal report of the real run is not the golden')\n"
                "        if chr(27) in tn + render_terminal(rv) or [a for a, b in zip(tn.split('\\n'), tv.split('\\n')) if a != b] != ['backends:   native']:\n"
                "            bad.append(f'{name}: the native and VM terminal reports differ beyond the backend line')\n"
                "        terminal_checked.append(name)\n"
                "        # CV-231: `sv0cov check` over the merged native + VM evidence, in a project holding the\n"
                "        # real sources: passes at the exact line fraction, fails one basis point above it, and\n"
                "        # refuses a changed source.\n"
                "        with tempfile.TemporaryDirectory() as td:\n"
                "            for rel, body in srcs.items():\n"
                "                (Path(td) / rel).parent.mkdir(parents=True, exist_ok=True)\n"
                "                (Path(td) / rel).write_bytes(body)\n"
                "            (Path(td) / 'cov.json').write_bytes(encode(xb))\n"
                "            def chk(*a):\n"
                "                return cli.main(['check', '--quiet', *a, 'cov.json'], stdout=io.StringIO(), stderr=io.StringIO(), cwd=Path(td))\n"
                "            ln = rn['summary']['line']\n"
                "            at = ln['covered'] * 10000 // ln['total']\n"
                "            got = [chk(), chk('--threshold-line', f'{at}bp'), chk('--threshold-line', f'{min(at + 1, 10000)}bp')]\n"
                "            if got != [0, 0, 1 if at < 10000 else 0]:\n"
                "                bad.append(f'{name}: sv0cov check exits {got} around the exact line fraction {ln[\"covered\"]}/{ln[\"total\"]}')\n"
                "            # CV-232: `sv0cov report` publishes the terminal and native reports of that evidence\n"
                "            # (equal to the producer's), refuses to replace them, and `sv0cov clean` removes only them.\n"
                "            def cmd(*a):\n"
                "                o = io.StringIO()\n"
                "                return cli.main(list(a), stdout=o, stderr=io.StringIO(), cwd=Path(td)), o.getvalue()\n"
                "            kept = sorted(str(q.relative_to(td)) for q in Path(td).rglob('*'))\n"
                "            rb = read_report(encode(build_report(encode(xb), resolve_config('report', [('--format', 'json')]), srcs)))\n"
                "            r1 = cmd('report', '--format', 'json', '--format', 'term', 'cov.json')\n"
                "            rep = Path(td) / '.sv0cov' / 'reports'\n"
                "            if r1[0] != 0 or read_report((rep / 'coverage.json').read_bytes()) != rb \\\n"
                "                    or (rep / 'coverage.txt').read_text(encoding='utf-8') != render_terminal(rb) \\\n"
                "                    or cmd('report', '--format', 'term', '--output', '-', 'cov.json') != (0, render_terminal(rb)):\n"
                "                bad.append(f'{name}: sv0cov report did not publish the reports of the merged evidence')\n"
                "            if cmd('report', '--format', 'term', '--format', 'json', 'cov.json')[0] != 6:\n"
                "                bad.append(f'{name}: sv0cov report replaced an existing bundle')\n"
                "            if cmd('clean')[0] != 0 or sorted(str(q.relative_to(td)) for q in Path(td).rglob('*')) != kept:\n"
                "                bad.append(f'{name}: sv0cov clean left something behind or removed a project file')\n"
                "            first = sorted(srcs)[0]\n"
                "            (Path(td) / first).write_bytes(srcs[first] + b'\\n')\n"
                "            if chk() != 5: bad.append(f'{name}: sv0cov check accepted a changed source')\n"
                "        check_checked.append(name)\n"
                "        reports_checked.append(name)\n"
                "    indexed_checked.append(name)\n"
                "print('\\n'.join(bad + [f'REGIONS {sum(regions_checked)} MODELS {len(models_checked)} INDEXED {len(indexed_checked)} REPORTS {len(reports_checked)} TERMINAL {len(terminal_checked)} CHECK {len(check_checked)}']))\n"
            )
            p = subprocess.run([*py, "-c", code, str(ROOT / "sv0cov" / "src"), json.dumps(resolved), str(FIXTURES),
                                str(PARITY_DIR)],
                               capture_output=True, text=True, timeout=300)
            out_lines = p.stdout.strip().splitlines()
            nregions = out_lines[-1].split()[1] if out_lines and out_lines[-1].startswith("REGIONS ") else "?"
            nmodels = out_lines[-1].split()[3] if out_lines and out_lines[-1].startswith("REGIONS ") else "?"
            nindexed = out_lines[-1].split()[5] if out_lines and out_lines[-1].startswith("REGIONS ") else "?"
            nreports = out_lines[-1].split()[7] if out_lines and out_lines[-1].startswith("REGIONS ") else "?"
            nterminal = out_lines[-1].split()[9] if out_lines and out_lines[-1].startswith("REGIONS ") else "?"
            ncheck = out_lines[-1].split()[11] if out_lines and out_lines[-1].startswith("REGIONS ") else "?"
            if ncheck != nmodels:
                errors.append(f"sv0cov reader: `sv0cov check` ran on {ncheck} real projects, want one per fixture ({nmodels})")
            if nterminal != nmodels:
                errors.append(f"sv0cov reader: {nterminal} terminal reports of real runs matched their golden, want one per fixture ({nmodels})")
            if nreports != nmodels:
                errors.append(f"sv0cov reader: {nreports} native reports were built from real runs, want one per fixture ({nmodels})")
            if nindexed != str(len(resolved)):
                errors.append(f"sv0cov reader: {nindexed} programs went through the indexed producer and reader, want {len(resolved)}")
            if nmodels != str(len(list(FIXTURES.glob("*/expected-semantic.json")))):
                errors.append(f"sv0cov reader: {nmodels} semantic models compared with expected-semantic.json, want every fixture")
            bad = [line for line in out_lines if not line.startswith("REGIONS ")]
            if p.returncode or bad:
                errors.append(f"sv0cov reader: {' '.join(bad)} {p.stderr.strip()[-600:]}")
            reader = (f"sv0cov reader resolves native + VM to the expected counts; {nregions} region expressions "
                      "evaluate equal and exact on both (CV-204); the semantic model of each real run equals the fixture's "
                      f"expected-semantic.json ({nmodels} fixtures, CV-221); merged indexed evidence of {nindexed} programs (a truncated profile inventoried incomplete; `sv0cov merge` publishes the same bytes and refuses to replace them, CV-227) reads back "
                      f"complete, with one semantic projection for native and VM runs (CV-222); native reports of {nreports} real runs read back passing and agree across backends (CV-229), and their {nterminal} terminal reports are the sv0cov goldens (CV-230); `sv0cov check` on {ncheck} real projects passes at the exact line fraction, fails one basis point above it and refuses a changed source (CV-231); `sv0cov report` publishes their terminal and native reports without replacing anything and `sv0cov clean` removes exactly those (CV-232); parity reports pass "
                      f"(F0-G4; f0 report in {PARITY_DIR.relative_to(ROOT)}/f0.json)")

        # CV-233, CV-234: `sv0cov run` end to end with the real driver, compiler and runtime, on a
        # copy of the multi-module fixture project. The instrumented program runs by name with
        # shell-hostile arguments under a poisoned caller environment; the run's merged evidence
        # is the fixture's expected model; reports are published; and the exit status follows the
        # matrix: pass 0, policy fail 1, test fail 4 (coverage still reported), killed 128+9, and
        # a run that executed no instrumented program is incomplete (5), never zero coverage.
        if not errors and py:
            run_code = (
                "import io, json, os, sys\n"
                "sys.path.insert(0, sys.argv[1])\n"
                "from pathlib import Path\n"
                "from sv0cov import cli\n"
                "from sv0cov.model.indexed import read_indexed\n"
                "from sv0cov.model.report import read_report\n"
                "cwd, bad = Path(sys.argv[2]), []\n"
                "os.environ.update({'SV0COV_RUN_ID': 'f' * 32, 'SV0COV_PROFILE_DIR': '/nonexistent', 'SV0COV_REQUIRED': '0', 'SV0COV_BOGUS': '1'})\n"
                "runs = cwd / '.sv0cov' / 'runs'\n"
                "def cmd(*a):\n"
                "    before = set(os.listdir(runs)) if runs.is_dir() else set()\n"
                "    o, e = io.StringIO(), io.StringIO()\n"
                "    rc = cli.main(list(a), stdout=o, stderr=e, cwd=cwd)\n"
                "    new = sorted((set(os.listdir(runs)) if runs.is_dir() else set()) - before)\n"
                "    return rc, o.getvalue(), e.getvalue(), (runs / new[0] if len(new) == 1 else None)\n"
                "hostile = ['a b', '$(touch pwned)', '; touch pwned', '*', '']\n"
                "rc, out, err, rd = cmd('run', '--report', 'term', '--report', 'json', '--', 'project', *hostile)\n"
                "if rc != 0 or err or rd is None or rd.name == 'f' * 32 or (cwd / 'pwned').exists():\n"
                "    bad.append(f'sv0cov run (pass): exit {rc}: {err[-300:]}')\n"
                "else:\n"
                "    ix = read_indexed((rd / 'coverage.sv0covdata.json').read_bytes())\n"
                "    want = json.loads(Path(sys.argv[3]).read_bytes())\n"
                "    if not ix['semantic']['complete'] or [k for k in want if k != 'map_ids' and ix['semantic'][k] != want[k]] or set(ix['semantic']) != set(want):\n"
                "        bad.append('sv0cov run: the evidence of the run is not the expected semantic model of the fixture (map_ids aside)')\n"
                "    if ix['execution']['status'] != 'passed' or ix['run_selection']['selected_run_ids'] != [rd.name] or len(os.listdir(rd / 'profiles')) != 1:\n"
                "        bad.append('sv0cov run: execution, run selection or profile count is wrong')\n"
                "    rep = read_report((rd / 'reports' / 'coverage.json').read_bytes())\n"
                "    if rep['evidence_eligibility'] != 'passing' or 'evidence:   PASSING' not in out:\n"
                "        bad.append('sv0cov run: the published or printed report is not passing')\n"
                "    ln = rep['summary']['line']\n"
                "    at = ln['covered'] * 10000 // ln['total']\n"
                "    if at < 10000:\n"
                "        rc, _, err, _ = cmd('run', '--quiet', '--threshold-line', f'{at + 1}bp', '--', 'project')\n"
                "        if rc != 1 or 'COV4101' not in err: bad.append(f'sv0cov run (policy fail): exit {rc}')\n"
                "    rc, _, _, _ = cmd('run', '--quiet', '--threshold-line', f'{at}bp', '--', 'project')\n"
                "    if rc != 0: bad.append(f'sv0cov run at the exact line fraction: exit {rc}')\n"
                "rc, out, err, rd = cmd('run', '--', '/bin/sh', '-c', 'project; exit 3')\n"
                "if rc != 4 or 'COV2130' not in err or 'execution:  failed (exit 3)' not in out or rd is None \\\n"
                "        or read_indexed((rd / 'coverage.sv0covdata.json').read_bytes())['execution']['status'] != 'failed':\n"
                "    bad.append(f'sv0cov run (test fail): exit {rc}, want 4 with the coverage still reported')\n"
                "rc, out, err, rd = cmd('run', '--', '/bin/sh', '-c', 'project; kill -9 $$')\n"
                "if rc != 137 or 'execution:  signaled (signal 9)' not in out: bad.append(f'sv0cov run (killed): exit {rc}, want 137')\n"
                "rc, out, err, rd = cmd('run', '--', '/usr/bin/true')\n"
                "if rc != 5 or 'COV3001' not in err or 'coverage report' in out or rd is None or (rd / 'coverage.sv0covdata.json').exists():\n"
                "    bad.append(f'sv0cov run that executed no instrumented program: exit {rc}, want incomplete (5) and no report')\n"
                "if cmd('clean')[0] != 0 or (cwd / '.sv0cov').exists(): bad.append('sv0cov clean did not remove the runs')\n"
                "print('\\n'.join(bad + ['RUN OK' if not bad else 'RUN BAD']))\n"
            )
            proj = t / "run-root" / "project"  # the directory name is the target name, as in the fixture map
            shutil.copytree(FIXTURES / "project", proj, ignore=shutil.ignore_patterns("expected-*"))
            p = subprocess.run([*py, "-c", run_code, str(ROOT / "sv0cov" / "src"), str(proj), str(FIXTURES / "project" / "expected-semantic.json")],
                               capture_output=True, text=True, timeout=900)
            if p.returncode or p.stdout.strip().splitlines()[-1:] != ["RUN OK"]:
                errors.append(f"sv0cov run: {p.stdout.strip()} {p.stderr.strip()[-600:]}")
            else:
                reader += ("; `sv0cov run` builds the multi-module project through this driver and runs it by name with shell-hostile arguments "
                           "under a poisoned environment: its evidence is the expected model, reports are published, and the exit status is 0 on a "
                           "pass, 1 on a failed threshold, 4 on a failing test command, 137 when killed, 5 when no instrumented program ran (CV-233, CV-234)")

        # 3-4. Small programs.
        small = {
            "talk": 'fn main() -> i32 {\n    println("user code ran");\n    return 7;\n}\n',
            "contract": "fn half(x: i32) -> i32 requires(x > 0) {\n    return x / 2;\n}\n\n"
                        "fn main() -> i32 {\n    return half(0);\n}\n",
            "crash": "fn div(a: i32, b: i32) -> i32 {\n    return a / b;\n}\n\n"
                     "fn main() -> i32 {\n    return div(7, 0);\n}\n",
        }
        built = {}
        for name, text in small.items():
            src = t / f"{name}.sv0"
            src.write_text(text)
            sv0b = t / f"{name}.sv0b"
            err = build_vm([str(src)], sv0b)
            if err:
                errors.append(f"{name}: VM build failed: {err}")
            built[name] = (sv0b, t / f"{name}.sv0covbind.json", json.loads((t / f"{name}.sv0covmap.json").read_bytes())
                           if (t / f"{name}.sv0covmap.json").is_file() else None)
        if all(b[2] is not None for b in built.values()):
            sv0b, bind, m = built["talk"]
            pdir = t / "ctx"
            pdir.mkdir()
            rc, out = vm_run(sv0b, bind, transport(pdir, SV0COV_CONTEXT="shard é"))
            prof = profile(pdir, m, errors, "context")
            if rc != 7 or "user code ran" not in out or prof is None or prof.context != "shard é":
                errors.append(f"context: exit {rc}, context {getattr(prof, 'context', None)!r}")

            sv0b, bind, m = built["contract"]
            pdir = t / "contract"
            pdir.mkdir()
            rc, out = vm_run(sv0b, bind, transport(pdir))
            prof = profile(pdir, m, errors, "contract failure")
            if rc != 1 or prof is None or sum(c for _, c in prof.counts) == 0:
                errors.append(f"contract failure: exit {rc}, want 1 with a published profile: {out[-300:]}")

            # Integer division by zero is a defined panic (sv0doc type-system/rules.md
            # 1.5): exit 1 with the profile published, like a contract failure.
            sv0b, bind, m = built["crash"]
            pdir = t / "panic"
            pdir.mkdir()
            rc, out = vm_run(sv0b, bind, transport(pdir))
            prof = profile(pdir, m, errors, "division by zero")
            if rc != 1 or prof is None or "sv0 panic: division by zero" not in out:
                errors.append(f"division by zero: exit {rc}, want the panic, exit 1 and a published profile: {out[-300:]}")
            # A crash is a VM failure with no program exit: the same bytecode cut
            # in half, behind a binding rewritten to match those bytes.
            code = sv0b.read_bytes()
            code = code[:len(code) // 2]
            broken, broken_bind = t / "broken.sv0b", t / "broken.sv0covbind.json"
            broken.write_bytes(code)
            fields = json.loads(bind.read_text())
            fields["bytecode_sha256"] = hashlib.sha256(code).hexdigest()
            fields["bytecode_length"] = len(code)
            broken_bind.write_text(json.dumps(fields, sort_keys=True, separators=(",", ":")) + "\n")
            pdir = t / "crash"
            pdir.mkdir()
            rc, out = vm_run(broken, broken_bind, transport(pdir))
            if rc is not None or list(pdir.iterdir()) or "sv0vm: fatal:" not in out:
                errors.append(f"crash: exit {rc}, files {[f.name for f in pdir.iterdir()]}; want a fatal VM failure, no exit, "
                              f"nothing published: {out[-300:]}")

            sv0b, bind, m = built["talk"]
            for label, over in (("unknown name", {"SV0COV_THRESHOLD": "90"}),
                                ("uppercase run ID", {"SV0COV_RUN_ID": RUN_ID.upper()})):
                pdir = t / label.replace(" ", "-")
                pdir.mkdir()
                rc, out = vm_run(sv0b, bind, transport(pdir, **over))
                if rc is not None or "error[COV2001]" not in out or "user code ran" in out or list(pdir.iterdir()):
                    errors.append(f"required {label}: want COV2001 before user code, got exit {rc}: {out[-300:]}")
                if "90" in out.split("error[COV2001]")[-1][:200] or RUN_ID.upper() in out:
                    errors.append(f"required {label}: the diagnostic shows a transport value")
            pdir = t / "outside"
            pdir.mkdir()
            rc, out = vm_run(sv0b, bind, transport(pdir, SV0COV_PROFILE_DIR=None, SV0COV_RUN_ID=None,
                                                   SV0COV_REQUIRED=None))
            if rc != 7 or "user code ran" not in out or "error[COV2001]" not in out or list(pdir.iterdir()):
                errors.append(f"outside sv0cov: exit {rc}; want the program to run, a COV2001 note, nothing published")

    if errors:
        print("coverage VM profiles: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage VM profiles: OK ({len(progs)} programs: one valid vm-v1 profile each, counts = native = expected; {reader}; "
          "context, contract failure and division by zero publish, a VM crash publishes nothing; required transport failure stops before user "
          "code; outside sv0cov runs and publishes nothing; sv0vm golden copies = sv0cov's)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
