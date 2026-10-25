#!/usr/bin/env python3
"""Privacy and determinism of real coverage output end to end (sv0cov
CV-424; SPEC 21.6, 23; COV-MAP-011, COV-SEC-005).

`sv0cov run` builds a small project through this toolchain's driver,
compiler and native runtime, with every report format selected (term, html,
json, lcov, cobertura), from two projects at two marked absolute paths under
two marked environments (user, login, host, home, temporary directory).

1. No file sv0cov, the compiler or the runtime wrote (maps, raw profiles,
   the indexed profile, every report, both export manifests), and no
   stdout or stderr, holds either project's absolute path, the home or
   temporary directory, a marked name, or any absolute path under a host
   root. The program built into `<run>/bin/` is the project's and is not
   scanned.
2. The two runs give byte-identical coverage maps and the same semantic
   projection (SPEC 21.6 gate 1); raw profiles differ only in their random
   profile IDs, so the indexed profiles differ only through those.

    python3 sv0c/test/coverage/privacy/run_privacy.py
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
sys.dont_write_bytecode = True
sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402

NAME = "privacy"  # the project directory, and so the program's name
RUN = "0123456789abcdef0123456789abcdef"
PROGRAM = """\
/* sv0cov CV-424 privacy fixture. */

fn pick(n: i32) -> i32 {
  if n > 100 {
    return n - 99;
  };
  return n + 2;
}

fn main() -> i32 {
  if pick(3) == 5 {
    return 0;
  };
  return 1;
}
"""

RUN_SCRIPT = r"""
import io, json, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov import cli
o, e = io.StringIO(), io.StringIO()
rc = cli.main(["run", "--diagnostic-format=json-lines", "--run-id", sys.argv[3], "--report", "term", "--report", "html", "--report", "json",
               "--report", "lcov", "--report", "cobertura", "--", sys.argv[4]], stdout=o, stderr=e, cwd=Path(sys.argv[2]))
print(json.dumps({"rc": rc, "out": o.getvalue(), "err": e.getvalue()}))
"""
HOST_ROOT = re.compile(rb"(?<![\w.])/(?:Users|home|private|tmp|var|root)/")


def one(top: Path, tag: str, py: list[str]) -> tuple[dict, Path, list[str]]:
    home, tmp, proj = top / f"home-{tag}", top / f"tmp-{tag}", top / f"projects-{tag}" / NAME
    for d in (home, tmp, proj):
        d.mkdir(parents=True)
    (proj / "main.sv0").write_text(PROGRAM)
    env = {**{k: v for k, v in os.environ.items() if k in ("PATH", "SV0_CC", "CC")}, "HOME": str(home), "TMPDIR": str(tmp),
           "USER": f"privuser{tag}", "LOGNAME": f"privlogin{tag}", "HOSTNAME": f"privhost{tag}", "PWD": str(proj)}
    p = subprocess.run([*py, "-c", RUN_SCRIPT, str(ROOT / "sv0cov" / "src"), str(proj), RUN, NAME], capture_output=True, text=True,
                       timeout=900, env=env, cwd=str(proj))
    if p.returncode:
        raise RuntimeError(p.stderr.strip()[-800:])
    markers = [str(proj), str(proj.resolve()), str(proj.parent), str(home), str(tmp), str(top), str(top.resolve()),
               f"privuser{tag}", f"privlogin{tag}", f"privhost{tag}"]
    return json.loads(p.stdout), proj, markers


def files(proj: Path) -> dict[str, bytes]:
    out = {}
    for p in sorted((proj / ".sv0cov").rglob("*")):
        rel = str(p.relative_to(proj))
        if p.is_file() and not rel.startswith(f".sv0cov/runs/{RUN}/bin/"):
            out[rel] = p.read_bytes()
    return out


def main() -> int:
    ensure_built()
    py = modern_python()
    if py is None:
        print("coverage privacy: OK (skipped: no Python >= 3.10)")
        return 0
    sys.path.insert(0, str(ROOT / "sv0cov" / "src"))
    from sv0cov.model.indexed import read_indexed

    errors: list[str] = []
    with tempfile.TemporaryDirectory(prefix="privtop-") as td:
        top = Path(td)
        a, proj_a, marks_a = one(top / "first", "a", py)
        b, proj_b, marks_b = one(top / "second" / "deeper", "b", py)
        fa, fb = files(proj_a), files(proj_b)
        for tag, result, found, marks in (("a", a, fa, marks_a), ("b", b, fb, marks_b)):
            if result["rc"] != 0:
                errors.append(f"run {tag}: exit {result['rc']}: {result['err'][-400:]}")
            kinds = {Path(n).name.split(".", 1)[-1] for n in found}
            for want in ("sv0covmap.json", "sv0profraw", "sv0covdata.json", "html", "info", "xml", "manifest.json", "json"):
                if want not in kinds and not any(n.endswith(want) for n in found):
                    errors.append(f"run {tag}: no .{want} artifact was written")
            outputs = {**{f"file {k}": v for k, v in found.items()},
                       "stdout": result["out"].encode(), "stderr": result["err"].encode()}
            for where, data in outputs.items():
                for m in marks:
                    if m.encode() in data:
                        errors.append(f"run {tag}: {where} holds {m!r}")
                hit = HOST_ROOT.search(data)
                if hit:
                    errors.append(f"run {tag}: {where} holds an absolute host path near {data[max(0, hit.start() - 20):hit.end() + 40]!r}")
        maps_a = {k: v for k, v in fa.items() if k.endswith(".sv0covmap.json")}
        maps_b = {k: v for k, v in fb.items() if k.endswith(".sv0covmap.json")}
        if not maps_a or maps_a != maps_b:
            errors.append(f"the coverage maps differ between the two projects ({sorted(maps_a)} vs {sorted(maps_b)})")
        ix = f".sv0cov/runs/{RUN}/coverage.sv0covdata.json"
        if ix in fa and ix in fb:
            pa, pb = read_indexed(fa[ix])["semantic_projection_sha256"], read_indexed(fb[ix])["semantic_projection_sha256"]
            if pa != pb:
                errors.append(f"the semantic projections differ: {pa} vs {pb}")
        else:
            errors.append("a run published no indexed profile")
    if errors:
        for e in errors:
            print(f"coverage privacy: FAIL: {e}", file=sys.stderr)
        return 1
    print(f"coverage privacy: OK ({len(fa)} artifacts per run scanned; maps and semantic projection identical across two projects)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
