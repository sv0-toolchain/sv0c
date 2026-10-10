#!/usr/bin/env python3
"""Source ownership and default scope (sv0cov CV-209; SPEC 16.3.2, 18.1;
COV-SCP-001, COV-SCP-002, COV-SCP-004 seed).

Ownership comes from the build graph, never from how a path is spelled. An
sv0 build has one package, the root project, so every compiled source is
`user`-owned with no package, and directory names mean nothing.

proj/ is one program whose modules sit under tests/, examples/, vendor/,
third_party/, generated/, build/, node_modules/, benchmarks/, fixtures/ and
src/. flat/ is the same program, byte for byte per file, under neutral
directory names (m0/ .. m9/). On the native C backend and on the VM:

1. proj/'s map lists exactly the files on disk, each `user` with a null
   package; native and VM maps are byte-identical and sv0cov-valid;
2. every file of proj/ has the same regions, counts, and line statuses as
   its twin in flat/ (only the logical path differs), from real runs;
3. the root metric (sv0cov.lines over `user` sources) is the same for both
   layouts and counts every file: nothing under tests/ or vendor/ drops out;
4. a `.sv0` file under a dot-directory is not compiled, so it is not a
   source (scope is the compiled set, not a path rule): the map is unchanged;
5. directory names with a space, non-ASCII text, and a dot (generated at
   run time) are ordinary too: the program builds and runs on both
   backends, and every source is user-owned under its exact logical path.

    python3 sv0c/test/coverage/ownership/run_ownership.py
"""

from __future__ import annotations

import json
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
SPECIAL = ("tests", "examples", "vendor", "third_party", "generated", "build", "node_modules", "benchmarks", "fixtures")

sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import build_vm, sv0, transport, vm_run  # noqa: E402

# From a map, its project root, and one profile: the validated map's sources
# (ownership, package), and per source its regions with counts and its line
# statuses, keyed by file name; then the root line metric.
REPORT = r"""
import json, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov.lines import line_metric, line_records, region_counts, user_source_indices
from sv0cov.resolve import resolve
mb = Path(sys.argv[2]).read_bytes()
m = json.loads(mb)
src = {s['path']: (Path(sys.argv[3]) / s['path']).read_bytes() for s in m['sources']}
res = resolve(mb, [Path(sys.argv[4]).read_bytes()], sources=src)
counts = region_counts(m, res.context_counts())
recs = line_records(m, None, src, contexts=res.context_counts())
out = {'sources': [[s['path'], s['ownership'], s['package']] for s in m['sources']], 'files': {}}
for s in m['sources']:
    i = s['source_index']
    regions = [[r['kind'], r['classification'], r['span']['start_byte'], r['span']['end_byte'], r['line_contributing'],
                r['line_numbers'], counts[r['region_index']].value if r['classification'] == 'user' else None]
               for r in m['regions'] if r['source_index'] == i]
    out['files'][s['path'].rsplit('/', 1)[-1]] = {'regions': regions,
                                                  'lines': [x['status'] for x in recs if x['source_index'] == i]}
out['metric'] = line_metric(recs, user_source_indices(m))
out['all'] = line_metric(recs)
print(json.dumps(out, sort_keys=True))
"""


def build_and_run(project: Path, d: Path, errors: list[str], label: str) -> dict[str, tuple[Path, Path]]:
    """{backend: (map, profile)} for one layout."""
    (d / "vm").mkdir(parents=True)
    p = sv0("native-compile", "--coverage=instrument", "-o", str(d / "main"), "--project", str(project))
    err = build_vm(["--project", str(project)], d / "vm" / "main.sv0b")
    if p.returncode or err:
        errors.append(f"{label}: build failed: {p.stderr.strip()} {err or ''}")
        return {}
    out = {}
    for backend in ("native", "vm"):
        pdir = d / f"{backend}-profiles"
        pdir.mkdir()
        if backend == "native":
            rc = subprocess.run([str(d / "main")], capture_output=True, env=transport(pdir), timeout=60).returncode
        else:
            rc, _ = vm_run(d / "vm" / "main.sv0b", d / "vm" / "main.sv0covbind.json", transport(pdir))
        profiles = sorted(pdir.iterdir())
        if rc != 0 or len(profiles) != 1:
            errors.append(f"{label} {backend}: exit {rc}, {len(profiles)} profiles")
            continue
        out[backend] = (d / "main.sv0covmap.json" if backend == "native" else d / "vm" / "main.sv0covmap.json", profiles[0])
    return out


def main() -> int:
    ensure_built()
    errors: list[str] = []
    py = modern_python()
    proj, flat = HERE / "proj", HERE / "flat"
    on_disk = sorted(str(f.relative_to(proj)) for f in proj.rglob("*.sv0"))
    twins = sorted(f.name for f in flat.rglob("*.sv0"))
    if sorted(Path(p).name for p in on_disk) != twins or not all(any(p.startswith(s + "/") for p in on_disk) for s in SPECIAL):
        errors.append("fixture: proj/ and flat/ do not hold the same files, or a special directory is missing")
    for f in proj.rglob("*.sv0"):
        twin = next(flat.rglob(f.name))
        if f.read_bytes() != twin.read_bytes():
            errors.append(f"fixture: {f.name} differs between proj/ and flat/")
    checked = 0
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)
        runs = {"proj": build_and_run(proj, t / "proj", errors, "proj"), "flat": build_and_run(flat, t / "flat", errors, "flat")}
        reports: dict[tuple[str, str], dict] = {}
        for layout, by_backend in runs.items():
            maps = [mp.read_bytes() for mp, _ in by_backend.values()]
            if len(maps) == 2 and maps[0] != maps[1]:
                errors.append(f"{layout}: the generated-C and VM maps differ")
            for backend, (mp, prof) in by_backend.items():
                m = json.loads(mp.read_bytes())
                bad = [s for s in m["sources"] if (s["ownership"], s["package"]) != ("user", None)]
                if bad:
                    errors.append(f"{layout} {backend}: sources not user-owned with a null package: {bad}")
                if layout == "proj" and [s["path"] for s in m["sources"]] != on_disk:
                    errors.append(f"proj {backend}: the map's sources {[s['path'] for s in m['sources']]} are not the files on disk {on_disk}")
                if py is None:
                    continue
                root = proj if layout == "proj" else flat
                r = subprocess.run([*py, "-c", REPORT, str(ROOT / "sv0cov" / "src"), str(mp), str(root), str(prof)],
                                   capture_output=True, text=True, timeout=300)
                if r.returncode:
                    errors.append(f"{layout} {backend}: sv0cov report failed: {r.stderr.strip()[-400:]}")
                    continue
                reports[(layout, backend)] = json.loads(r.stdout)
        for backend in ("native", "vm"):
            a, b = reports.get(("proj", backend)), reports.get(("flat", backend))
            if a is None or b is None:
                continue
            if a["files"] != b["files"]:
                diff = [n for n in a["files"] if a["files"][n] != b["files"].get(n)]
                errors.append(f"{backend}: files differ between proj/ and flat/ (directory names changed coverage): {diff}")
            if a["metric"] != b["metric"] or a["metric"] != a["all"]:
                errors.append(f"{backend}: root metric proj {a['metric']} flat {b['metric']} all {a['all']}")
            # Every file contributes executable lines: nothing dropped out of scope.
            idle = [n for n, f in a["files"].items() if not any(s != "non_executable" for s in f["lines"])]
            if idle or len(a["files"]) != len(on_disk):
                errors.append(f"{backend}: files with no executable line in scope: {idle}")
            checked += 1
        if ("proj", "native") in reports and ("proj", "vm") in reports and reports[("proj", "native")] != reports[("proj", "vm")]:
            errors.append("proj: native and VM reports differ")

        # 4. A dot-directory is not listed, so its file is not compiled.
        copy = t / "with-hidden"
        shutil.copytree(proj, copy)
        (copy / ".cache").mkdir()
        (copy / ".cache" / "stale.sv0").write_text("module stale;\n\nfn never(n: i32) -> i32 {\n    return n;\n}\n")
        hidden_map = t / "hidden.json"
        p = sv0("native-compile", "--coverage=map", "--coverage-map", str(hidden_map), "-o", str(t / "hidden-exe"),
                "--project", str(copy))
        ref = t / "ref.json"
        q = sv0("native-compile", "--coverage=map", "--coverage-map", str(ref), "-o", str(t / "hidden-exe"),
                "--project", str(proj))
        if p.returncode or q.returncode or not hidden_map.is_file() or hidden_map.read_bytes() != ref.read_bytes():
            errors.append(f"a .sv0 file under a dot-directory changed the map (or the build failed): {p.stderr.strip()[-200:]}")

        # 5. Unusual spellings are ordinary directories.
        odd = t / "odd"
        mods = (("Vendor Stuff/Größe", "sp", "spaced", 1), ("日本", "jp", "nihon", 2), ("a.b/tests", "dot", "dotted", 3))
        for d, mod, fn, k in mods:
            (odd / d).mkdir(parents=True)
            (odd / d / f"{mod}.sv0").write_text(f"module {mod};\n\nfn {fn}(n: i32) -> i32 {{\n    return n + {k};\n}}\n",
                                                encoding="utf-8")
        (odd / "main.sv0").write_text("".join(f"use {mod}::{fn};\n" for _, mod, fn, _ in mods)
                                      + "\nfn main() -> i32 {\n    return spaced(1) + nihon(1) + dotted(1) - 9;\n}\n")
        odd_runs = build_and_run(odd, t / "odd-out", errors, "odd names")
        want_paths = sorted([f"{d}/{mod}.sv0" for d, mod, _, _ in mods] + ["main.sv0"], key=lambda x: x.encode())
        for backend, (mp, _) in odd_runs.items():
            m = json.loads(mp.read_bytes())
            got = [(s["path"], s["ownership"], s["package"]) for s in m["sources"]]
            if got != [(x, "user", None) for x in want_paths]:
                errors.append(f"odd names {backend}: sources {got}, want {want_paths} (user, null)")
        if len(odd_runs) != 2:
            errors.append("odd names: the program did not build and run on both backends")

    if errors:
        print("coverage ownership: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    note = f"{checked} backends: per-file regions, counts, lines and the root metric equal the neutral layout" if py else \
        "per-file comparison SKIPPED (no Python >= 3.10)"
    print(f"coverage ownership: OK ({len(on_disk)} sources under tests/, examples/, vendor/, ... all user-owned, null package, "
          f"in scope on C + VM; {note}; a dot-directory file is not a source; "
          "spaces, non-ASCII and dotted directory names are ordinary)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
