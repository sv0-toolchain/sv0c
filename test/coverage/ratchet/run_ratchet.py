#!/usr/bin/env python3
"""Ratchets end to end (sv0cov CV-313; SPEC 22.4; AC-017, COV-POL-005).

A small project is built and run by `sv0cov run` through this toolchain's
driver, compiler and runtime, with a ratchet policy, in a real Git
repository:

1. with no baseline yet, every ratchet evaluation is ineligible (COV4201,
   the baseline "missing") and the run fails its policy (1);
2. `sv0cov baseline` on that run's evidence prints the five metrics before
   it writes the pinned baseline at HEAD; the next run passes (0);
3. AC-017: the uncalled function is excluded with `#[coverage(off, ...)]`.
   The percentages improve, yet the ratchet reports exclusion growth for
   function, region and line and the extra exclusion record, and the run
   fails (1, COV4201) - the improvement is not silently accepted;
4. a new baseline accepts it, printing the excluded deltas, and the next run
   passes;
5. a change that stops calling a covered function regresses the numerator:
   the line ratio fails at tolerance 0 and passes at a tolerance that
   covers exactly the drop;
6. a baseline made on a branch HEAD no longer descends from is stale.

    python3 sv0c/test/coverage/ratchet/run_ratchet.py
"""

from __future__ import annotations

import json
import os
import shutil
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

NAME = "ratchet"
PROGRAM = """\
/* sv0cov CV-313 ratchet fixture. */

fn double(n: i32) -> i32 {
  return n + n;
}

fn unused() -> i32 {
  let a = 1;
  return a + 6;
}

fn main() -> i32 {
  let x = double(2);
  if x == 4 {
    return 0;
  };
  return 1;
}
"""
EXCLUDED = PROGRAM.replace("fn unused()", '#[coverage(off, reason = "kept for the API, never called")]\nfn unused()')
# main no longer calls double: one function and its line go uncovered.
REGRESSED = EXCLUDED.replace("let x = double(2);", "let x = 4;")
TOML = '[tool.sv0cov]\nschema = "1.0"\n[tool.sv0cov.ratchet]\nbaseline = "coverage-baseline.json"\nmetrics = ["function", "region", "line", "branch"]\n'

STEP = r"""
import io, json, os, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov import cli
from sv0cov.model.report import read_report
cwd = Path(sys.argv[2])
runs = cwd / ".sv0cov" / "runs"
what, extra = sys.argv[3], json.loads(sys.argv[4])
before = set(os.listdir(runs)) if runs.is_dir() else set()
o, e = io.StringIO(), io.StringIO()
if what == "run":
    rc = cli.main(["run", "--diagnostic-format=json-lines", "--report", "json", *extra, "--", sys.argv[5]], stdout=o, stderr=e, cwd=cwd)
else:
    rc = cli.main(["baseline", "--diagnostic-format=json-lines", *extra], stdout=o, stderr=e, cwd=cwd)
new = sorted((set(os.listdir(runs)) if runs.is_dir() else set()) - before)
codes = [json.loads(line)["code"] for line in e.getvalue().splitlines() if line.startswith("{")]
out = {"rc": rc, "codes": codes, "stdout": o.getvalue(), "err": e.getvalue()[-600:]}
if new:
    rd = runs / new[0]
    out["evidence"] = str((rd / "coverage.sv0covdata.json").relative_to(cwd))
    rp = rd / "reports" / "coverage.json"
    if rp.exists():
        r = read_report(rp.read_bytes())
        out["ratchet"] = r["ratchet"]
        out["summary"] = {m: [r["summary"][m]["covered"], r["summary"][m]["total"]] for m in ("function", "region", "line", "branch")}
        out["evals"] = [[x["metric"], x["comparison"], x["result"], x["diagnostics"]] for x in r["policy"]["evaluations"]
                        if x["assertion_kind"] == "ratchet"]
print(json.dumps(out))
"""


def git_env() -> dict[str, str]:
    env = {k: v for k, v in os.environ.items() if not k.startswith("GIT_")}
    env.update({"GIT_CONFIG_GLOBAL": os.devnull, "GIT_CONFIG_NOSYSTEM": "1", "GIT_TERMINAL_PROMPT": "0"})
    return env


def git(cwd: Path, *args: str) -> str:
    p = subprocess.run(["git", "-c", "user.name=Fixture", "-c", "user.email=fixture@example.invalid", "-c", "commit.gpgsign=false", *args],
                       cwd=cwd, env=git_env(), capture_output=True, text=True)
    if p.returncode:
        raise RuntimeError(f"git {' '.join(args)}: {p.stderr.strip()}")
    return p.stdout.strip()


def main() -> int:
    if shutil.which("git") is None:
        print("coverage ratchets: OK (skipped: git is not installed)")
        return 0
    ensure_built()
    py = modern_python()
    if py is None:
        print("coverage ratchets: OK (skipped: no Python >= 3.10)")
        return 0
    src = str(ROOT / "sv0cov" / "src")
    errors: list[str] = []
    with tempfile.TemporaryDirectory() as td:
        proj = Path(td) / NAME
        proj.mkdir()
        git(proj, "init", "-q", "-b", "main")
        (proj / ".gitignore").write_text(".sv0cov/\n")
        (proj / "sv0.toml").write_text(TOML)
        (proj / "main.sv0").write_text(PROGRAM)
        git(proj, "add", "-A")
        git(proj, "commit", "-q", "-m", "start")

        def step(what: str, *extra: str) -> dict:
            p = subprocess.run([*py, "-c", STEP, src, str(proj), what, json.dumps(list(extra)), NAME], capture_output=True, text=True,
                               timeout=900)
            if p.returncode:
                raise RuntimeError(p.stderr.strip()[-800:])
            return json.loads(p.stdout)

        def failing(x: dict) -> list[list]:
            return [e[:2] for e in x.get("evals", []) if e[2] != "pass"]

        # 1. No baseline yet.
        x = step("run")
        if x["rc"] != 1 or x.get("ratchet", {}).get("state") != "missing" or set(x["codes"]) != {"COV4201"}:
            errors.append(f"no baseline: exit {x['rc']} {x['codes']} state {x.get('ratchet', {}).get('state')}, want 1, COV4201, missing")
        first = x
        # 2. Write the baseline from that evidence; the next run holds.
        b = step("baseline", first["evidence"])
        stdout = b["stdout"]
        if b["rc"] != 0 or "was none" not in stdout or stdout.index("metric") > stdout.index("wrote coverage-baseline.json"):
            errors.append(f"sv0cov baseline: exit {b['rc']} {b['codes']} {b['err']}")
        made = json.loads((proj / "coverage-baseline.json").read_bytes())
        if made["commit"] != git(proj, "rev-parse", "HEAD"):
            errors.append(f"the baseline names commit {made['commit']}, want HEAD")
        git(proj, "add", "coverage-baseline.json")
        git(proj, "commit", "-q", "-m", "baseline")
        x = step("run")
        if x["rc"] != 0 or failing(x):
            errors.append(f"run against its own baseline: exit {x['rc']} {x['codes']} failing {failing(x)}")
        held = x["summary"]

        # 3. AC-017: exclude the uncalled function.
        (proj / "main.sv0").write_text(EXCLUDED)
        git(proj, "commit", "-q", "-am", "exclude unused")
        x = step("run")
        better = x.get("summary", {})
        outcomes = {c["metric"]: c["outcomes"] for c in x.get("ratchet", {}).get("comparisons", [])}
        if x["rc"] != 1 or set(x["codes"]) != {"COV4201"}:
            errors.append(f"AC-017: exit {x['rc']} {x['codes']}, want 1 with COV4201")
        if not better or better["function"][0] * held["function"][1] <= held["function"][0] * better["function"][1]:
            errors.append(f"AC-017: the function percentage did not improve ({held['function']} -> {better.get('function')})")
        want_failing = [["exclusion_records", "at_most"], ["function", "at_most"], ["line", "at_most"], ["region", "at_most"]]
        if failing(x) != want_failing:
            errors.append(f"AC-017: failing ratchet evaluations {failing(x)}, want only the exclusion growth {want_failing}")
        for m in ("function", "line", "region"):
            if "exclusion_growth" not in outcomes.get(m, []):
                errors.append(f"AC-017: {m} does not report exclusion growth ({outcomes.get(m)})")
        # 4. Accept it explicitly.
        b = step("baseline", x["evidence"])
        if b["rc"] != 0 or "excluded +1" not in b["stdout"] or "exclusion records: 0 -> 1" not in b["stdout"]:
            errors.append(f"baseline update: exit {b['rc']}, deltas {b['stdout'][-500:]}")
        git(proj, "commit", "-q", "-am", "accept the exclusion")
        x = step("run")
        if x["rc"] != 0 or failing(x):
            errors.append(f"run after accepting the exclusion: exit {x['rc']} {failing(x)}")
        accepted = x["summary"]

        # 5. Numerator regression and the exact tolerance boundary.
        (proj / "main.sv0").write_text(REGRESSED)
        git(proj, "commit", "-q", "-am", "stop calling double")
        x = step("run")
        now = x.get("summary", {})
        line = {c["metric"]: c["outcomes"] for c in x.get("ratchet", {}).get("comparisons", [])}.get("line", [])
        if x["rc"] != 1 or ["line", "at_least"] not in failing(x) or "numerator_regression" not in line:
            errors.append(f"numerator regression: exit {x['rc']} failing {failing(x)} outcomes {line}")
        if now:
            (c0, t0), (c1, t1) = accepted["line"], now["line"]
            # the least tolerance that admits c1/t1: ceil((c0*t1 - c1*t0) * 10000 / (t0*t1))
            need = -(-((c0 * t1 - c1 * t0) * 10000) // (t0 * t1))
            for tol, rc in ((need, None), (need - 1, 1)):
                y = step("run", "--ratchet-metric", "line", "--ratchet-tolerance-basis-points", str(tol))
                ratio = [e for e in y.get("evals", []) if e[:2] == ["line", "at_least"]]
                want = "pass" if rc is None else "fail"
                if not ratio or ratio[0][2] != want:
                    errors.append(f"tolerance {tol} bp: line ratio {ratio}, want {want}")
        (proj / "main.sv0").write_text(EXCLUDED)
        git(proj, "commit", "-q", "-am", "restore")

        # 6. A history the baseline is not part of.
        git(proj, "checkout", "-q", "--orphan", "elsewhere")
        git(proj, "commit", "-q", "-m", "unrelated root")
        x = step("run")
        if x["rc"] != 1 or x.get("ratchet", {}).get("state") != "stale":
            errors.append(f"stale baseline: exit {x['rc']} state {x.get('ratchet', {}).get('state')}, want 1 and stale")

    if errors:
        for e in errors:
            print(f"coverage ratchets: FAIL: {e}", file=sys.stderr)
        return 1
    print("coverage ratchets: OK (sv0cov run + baseline on a real repository: missing baseline, pinned baseline at HEAD, AC-017 "
          "exclusion growth refused despite a better percentage, explicit acceptance, numerator regression at the exact tolerance "
          "boundary, stale baseline)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
