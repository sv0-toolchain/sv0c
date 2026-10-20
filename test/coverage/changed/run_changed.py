#!/usr/bin/env python3
"""Changed-region coverage end to end (sv0cov CV-312; SPEC 22.3; AC-016,
AC-045, AC-047..049, COV-POL-003/004).

`sv0cov run` builds a small project in a real Git repository through this
toolchain's driver, compiler and runtime, with a diff policy against the
branch it forked from. The topic branch changes two lines of `pick`: one the
program runs and one it never reaches. A function the program never calls
stays as it was.

1. the base is recorded (spelling, reference, HEAD, merge base) and the
   capture names exactly the two changed lines;
2. the diff line fraction is exactly 1/2 (the reached line covered, the
   unreached one not), while the project's line fraction also counts the
   unchanged uncalled function; every diff denominator holds exactly the
   records that meet a changed line (an oracle over the report's own
   records);
3. the diff line threshold passes at 50% and fails at 50.01% (COV4101);
4. `sv0cov check` of the run's evidence decides the same boundary;
5. an uncommitted edit to a comment makes the run dirty (AC-047) without
   adding to any denominator (COV-POL-004: no executable changed line);
6. a test command that edits the source while it runs gets COV4305 and no
   report (AC-049); a compiled source Git does not track gets COV4304 and
   no report (AC-048); no base, or one Git cannot resolve, builds nothing
   (AC-045).

    python3 sv0c/test/coverage/changed/run_changed.py
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
sys.dont_write_bytecode = True
sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402

NAME = "changed"  # the project directory, and so the program's name
BASE = """\
/* sv0cov CV-312 changed-region fixture. */

fn pick(n: i32) -> i32 {
  let mut r = 0;
  if n > 100 {
    r = n - 99;
  } else {
    r = n + 2;
  };
  return r;
}

fn unused() -> i32 {
  return 7;
}

fn main() -> i32 {
  let x = pick(3);
  if x == 4 {
    return 0;
  };
  return 1;
}
"""
# The topic branch: line 6 (never reached) and line 8 (reached) change.
TOPIC = BASE.replace("r = n - 99;", "r = n - 100;").replace("r = n + 2;", "r = n + 1;")
CHANGED = [6, 8]

CHECK = r"""
import io, json, os, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov import cli
from sv0cov.model.report import read_report
cwd = Path(sys.argv[2])
runs = cwd / ".sv0cov" / "runs"

def cmd(*a):
    before = set(os.listdir(runs)) if runs.is_dir() else set()
    o, e = io.StringIO(), io.StringIO()
    rc = cli.main([a[0], "--diagnostic-format=json-lines", *a[1:]], stdout=o, stderr=e, cwd=cwd)
    new = sorted((set(os.listdir(runs)) if runs.is_dir() else set()) - before)
    codes = [json.loads(line)["code"] for line in e.getvalue().splitlines() if line.startswith("{")]
    return rc, o.getvalue(), codes, (runs / new[0] if len(new) == 1 else None), e.getvalue()

def diff(rep):
    return {e["metric"]: e for e in rep["policy"]["evaluations"] if e["assertion_kind"] == "diff"}

def oracle(rep, changed):
    def meets(r):
        return any(r["span"]["start_line"] <= n <= r["span"]["end_line"] for n in changed)
    lines = [x["line_index"] for x in rep["lines"] if x["status"] in ("covered", "partial", "uncovered") and x["line"] in changed]
    regions = [x["region_index"] for x in rep["regions"] if x["classification"] == "user" and meets(x)]
    branches = sorted({b["branch_index"] for b in rep["branches"] if meets(b)
                       and any(o["status"] in ("covered", "uncovered") for o in b["outcomes"])})
    return {"line": lines, "region": regions, "branch": branches}

out = {}
args = ["--report", "json", "--report", "term", "--diff-base", "main", "--diff-line", "0", "--diff-region", "0", "--diff-branch", "0"]
rc, text, codes, rd, err = cmd("run", *args, "--", sys.argv[3])
out["pass"] = [rc, codes, text, err[-400:]]
if rc == 0 and rd is not None:
    rep = read_report((rd / "reports" / "coverage.json").read_bytes())
    out["changes"] = rep["changes"]
    out["diff"] = {m: [e["actual"], e["members"]] for m, e in diff(rep).items()}
    out["oracle"] = oracle(rep, set(json.loads(sys.argv[4])))
    out["project_line"] = rep["summary"]["line"]
    out["line_numbers"] = {str(x["line_index"]): x["line"] for x in rep["lines"]}
    out["statuses"] = {str(x["line"]): x["status"] for x in rep["lines"]}
    evidence = str((rd / "coverage.sv0covdata.json").relative_to(cwd))
    rc, _, codes, _, _ = cmd("check", "--diff-base", "main", "--diff-line", "50", evidence)
    out["check50"] = [rc, codes]
    rc, _, codes, _, _ = cmd("check", "--diff-base", "main", "--diff-line", "5001bp", evidence)
    out["check5001"] = [rc, codes]
rc, _, codes, _, _ = cmd("run", "--quiet", "--diff-base", "merge-base:main", "--diff-line", "50", "--", sys.argv[3])
out["at50"] = [rc, codes]
rc, _, codes, _, _ = cmd("run", "--quiet", "--diff-base", "main", "--diff-line", "5001bp", "--", sys.argv[3])
out["above50"] = [rc, codes]
print(json.dumps(out))
"""

LATER = r"""
import io, json, os, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov import cli
from sv0cov.model.report import read_report
cwd = Path(sys.argv[2])
runs = cwd / ".sv0cov" / "runs"
before = set(os.listdir(runs)) if runs.is_dir() else set()
o, e = io.StringIO(), io.StringIO()
rc = cli.main(["run", "--diagnostic-format=json-lines", "--report", "json", *json.loads(sys.argv[3])], stdout=o, stderr=e, cwd=cwd)
new = sorted((set(os.listdir(runs)) if runs.is_dir() else set()) - before)
codes = [json.loads(line)["code"] for line in e.getvalue().splitlines() if line.startswith("{")]
rd = runs / new[0] if len(new) == 1 else None
rep = None
if rd is not None and (rd / "reports" / "coverage.json").exists():
    r = read_report((rd / "reports" / "coverage.json").read_bytes())
    rep = {"changes": r["changes"], "diff": {e["metric"]: e["actual"] for e in r["policy"]["evaluations"] if e["assertion_kind"] == "diff"}}
print(json.dumps({"rc": rc, "codes": codes, "run_dir": rd is not None, "built": rd is not None and (rd / "bin" / sys.argv[4]).exists(),
                  "report": rep, "err": e.getvalue()[-400:]}))
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
    import shutil

    if shutil.which("git") is None:
        print("coverage changed regions: OK (skipped: git is not installed)")
        return 0
    ensure_built()
    py = modern_python()
    if py is None:
        print("coverage changed regions: OK (skipped: no Python >= 3.10)")
        return 0
    src = str(ROOT / "sv0cov" / "src")
    errors: list[str] = []
    with tempfile.TemporaryDirectory() as td:
        proj = Path(td) / NAME
        proj.mkdir()
        git(proj, "init", "-q", "-b", "main")
        (proj / "main.sv0").write_text(BASE)
        (proj / ".gitignore").write_text(".sv0cov/\n")
        git(proj, "add", "-A")
        git(proj, "commit", "-q", "-m", "base")
        git(proj, "checkout", "-q", "-b", "topic")
        (proj / "main.sv0").write_text(TOPIC)
        git(proj, "commit", "-q", "-am", "topic")
        head, base = git(proj, "rev-parse", "HEAD"), git(proj, "rev-parse", "main")

        p = subprocess.run([*py, "-c", CHECK, src, str(proj), NAME, json.dumps(CHANGED)], capture_output=True, text=True, timeout=900)
        if p.returncode:
            print(f"coverage changed regions: FAIL: {p.stderr.strip()[-800:]}", file=sys.stderr)
            return 1
        r = json.loads(p.stdout)
        rc, codes, text, err = r["pass"]
        if rc != 0:
            errors.append(f"run with a diff policy: exit {rc} {codes} {err}")
        else:
            c = r["changes"]
            if c["base"] != {"head": head, "merge_base": base, "reference": base, "spelling": "main"}:
                errors.append(f"recorded base {c['base']}, want HEAD {head} and merge base {base}")
            if [x["changed_lines"] for x in c["sources"]] != [CHANGED] or not c["clean"] or c["sources"][0]["state"] != "modified":
                errors.append(f"capture {c['sources']}, want one clean modified source with lines {CHANGED}")
            line, members = r["diff"]["line"]
            if line != {"denominator": 2, "numerator": 1}:
                errors.append(f"diff line fraction {line}, want 1/2 (line 8 ran, line 6 did not)")
            if r["statuses"].get("6") != "uncovered" or r["statuses"].get("8") != "covered":
                errors.append(f"line 6 / 8 statuses {r['statuses'].get('6')} / {r['statuses'].get('8')}, want uncovered / covered")
            if r["statuses"].get("14") != "uncovered" or r["project_line"]["total"] <= 2:
                errors.append("the unchanged, uncalled function is not counted by the project")
            keys = {"line": "line_indices", "region": "region_indices", "branch": "branch_indices"}
            for metric, want in r["oracle"].items():
                got = r["diff"][metric][1][keys[metric]]
                if got != want:
                    errors.append(f"diff {metric} members {got}, the oracle says {want}")
            if not r["oracle"]["region"] or not r["oracle"]["branch"]:
                errors.append("no region or branch met a changed line: the fixture does not exercise them")
            if f"since {base[:12]}" not in text or "; clean" not in text:
                errors.append("the terminal report does not name the merge base and a clean run")
            if r["check50"] != [0, []] or r["check5001"] != [1, ["COV4101"]]:
                errors.append(f"sv0cov check of the run's evidence: {r['check50']} at 50%, {r['check5001']} at 50.01%")
        if r["at50"] != [0, []]:
            errors.append(f"diff line threshold 50%: {r['at50']}, want a pass")
        if r["above50"][0] != 1 or r["above50"][1] != ["COV4101"]:
            errors.append(f"diff line threshold 50.01%: {r['above50']}, want exit 1 with COV4101")

        def later(*args: str) -> dict:
            p = subprocess.run([*py, "-c", LATER, src, str(proj), json.dumps(list(args)), NAME], capture_output=True, text=True, timeout=900)
            if p.returncode:
                raise RuntimeError(p.stderr.strip()[-800:])
            return json.loads(p.stdout)

        diff = ["--diff-base", "main", "--diff-line", "0"]
        # AC-047 / COV-POL-004: an uncommitted comment edit: dirty, but no executable changed line.
        (proj / "main.sv0").write_text(TOPIC.replace("changed-region fixture.", "changed-region fixture, edited."))
        x = later(*diff, "--", NAME)
        if x["rc"] != 0 or x["report"] is None:
            errors.append(f"dirty run: exit {x['rc']} {x['codes']} {x['err']}")
        else:
            c = x["report"]["changes"]
            if c["clean"] or not c["sources"][0]["unstaged"] or c["sources"][0]["changed_lines"] != [1, *CHANGED]:
                errors.append(f"dirty capture {c['sources']}, want unstaged with lines {[1, *CHANGED]}")
            if x["report"]["diff"]["line"] != {"denominator": 2, "numerator": 1}:
                errors.append(f"dirty diff line {x['report']['diff']['line']}: a changed comment entered the denominator")
        (proj / "main.sv0").write_text(TOPIC)
        # AC-049: the test command edits the compiled source while it runs.
        x = later(*diff, "--", "/bin/sh", "-c", f'{NAME} && echo "// edited" >> main.sv0')
        if x["rc"] != 5 or x["codes"] != ["COV4305"] or x["report"] is not None:
            errors.append(f"source edited during the run: exit {x['rc']} {x['codes']}, want 5, COV4305 and no report")
        (proj / "main.sv0").write_text(TOPIC)
        # AC-045: no base, or one Git cannot resolve: nothing is built.
        for args, want in ((["--diff-line", "0"], None), (["--diff-base", "no-such-ref", "--diff-line", "0"], ["COV4302"])):
            x = later(*args, "--", NAME)
            if x["rc"] != 2 or x["run_dir"] or (want and x["codes"] != want):
                errors.append(f"run {args}: exit {x['rc']} {x['codes']}, want 2{' ' + str(want) if want else ''} before any build")
        # AC-048: the compiled source is not tracked.
        git(proj, "rm", "-q", "--cached", "main.sv0")
        git(proj, "commit", "-q", "-m", "untrack")
        x = later(*diff, "--", NAME)
        if x["rc"] != 5 or x["codes"] != ["COV4304"] or x["report"] is not None or not x["built"]:
            errors.append(f"compiled untracked source: exit {x['rc']} {x['codes']}, want 5, COV4304 and no report")

    if errors:
        for e in errors:
            print(f"coverage changed regions: FAIL: {e}", file=sys.stderr)
        return 1
    print("coverage changed regions: OK (sv0cov run with a diff policy on a real repository: base recorded, diff line 1/2 "
          "against the oracle, threshold boundary, dirty comment edit, edited / untracked source and bad base refused)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
