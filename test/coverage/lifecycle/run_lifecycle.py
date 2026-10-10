#!/usr/bin/env python3
"""VM lifecycle parity with native (sv0cov CV-212; SPEC 14.2.2, 14.3, 15.5,
23.4; BL-026, COV-VM-004, COV-INS-007).

The same program, built with `--coverage=instrument` for the native C backend
and for sv0vm, is run the same way on both. For every case the two processes
must agree on: the exit status, whether user code ran, whether a profile was
published, the diagnostic's registry code and title, and (when published) the
counts.

1. Exit paths, with SV0COV_REQUIRED 1 and 0: return 0 / 7 / 300 / -1 (the
   status is the low 8 bits), a failed `requires` and `ensures`, and the
   runtime panics the C runtime defines (Vec get/set, string index, substring,
   slice index, a missing file). All are orderly: exit 1 for a failure, the
   same `sv0 panic:` line, one valid profile with equal counts.
2. Transport failures (SPEC 14.3): an unknown SV0COV_* name, a malformed,
   zero or missing run ID, a missing, relative or nonexistent profile
   directory, an overlong or non-UTF-8 context, a malformed SV0COV_REQUIRED,
   and no transport at all. Required: status 1 before user code. Not
   required: the program runs and exits with its own status. Never a
   profile, never a transport value in the diagnostic.
3. Publication failure: a profile directory that cannot be written is
   COV2010; required mode turns it into status 1, otherwise the program's
   status stands.
4. Hard termination: SIGKILL and SIGTERM in the middle of a run publish
   nothing and leave nothing behind, on both backends.
5. Capability rejection (sv0vm; COV-VM-008, COV-VM-020): a binding that
   declares another capability, plan capability, VM profile, raw-profile
   version, schema or version, or no binding at all, is COV2201 before any
   user instruction in both modes. (The native peer, a registration with
   another protocol, is COV1015 in sv0cov's runtime tests; required or not,
   sv0vm cannot run a COVER_HIT it has not bound, so it always stops.)
6. A fatal VM failure (integer division by zero) is not an exit: one
   `sv0vm: fatal:` diagnostic, status 134, no `vm_exit` line, no profile.

    python3 sv0c/test/coverage/lifecycle/run_lifecycle.py
"""

from __future__ import annotations

import itertools
import json
import os
import re
import signal
import subprocess
import sys
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
SV0 = ROOT / "scripts" / "sv0"

sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built  # noqa: E402
from run_vm_profile import RUN_ID, build_vm, profile, sv0, transport  # noqa: E402

SAY = '    println("user code ran");\n'


def main_of(body: str, before: str = "") -> str:
    return f"{before}fn main() -> i32 {{\n{SAY}{body}}}\n"


# name -> (source, exit status, first stderr line or None)
PROGRAMS = {
    "ret0": (main_of("    return 0;\n"), 0, None),
    "ret7": (main_of("    return 7;\n"), 7, None),
    "ret300": (main_of("    return 300;\n"), 44, None),
    "neg": (main_of("    return 0 - 1;\n"), 255, None),
    "requires": (main_of("    return half(0);\n", "fn half(x: i32) -> i32 requires(x > 0) {\n    return x / 2;\n}\n\n"),
                 1, "sv0 contract violation: requires failed"),
    "ensures": (main_of("    return half(4);\n", "fn half(x: i32) -> i32 ensures(result > 5) {\n    return x / 2;\n}\n\n"),
                1, "sv0 contract violation: ensures failed"),
    "vec-get": (main_of("    let v: Vec<i32> = vec_new();\n    vec_push(v, 4);\n    return vec_get(v, 1);\n"),
                1, "sv0 panic: vec: index out of bounds"),
    "vec-set": (main_of("    let v: Vec<i32> = vec_new();\n    vec_push(v, 4);\n    vec_set(v, 0 - 1, 9);\n    return 3;\n"),
                1, "sv0 panic: vec: index out of bounds"),
    "str-index": (main_of('    let s: string = "abc";\n    return string_char_at(s, 9);\n'),
                  1, "sv0 panic: string: index out of bounds"),
    "substr": (main_of('    let s: string = "abc";\n    return string_len(string_substr(s, 2, 5));\n'),
               1, "sv0 panic: string: substring out of bounds"),
    "read-file": (main_of('    return string_len(read_file("/nonexistent/sv0cov-cv212"));\n'),
                  1, "sv0 panic: read_file: fopen failed"),
}
# Runs until it is killed.
SPIN = main_of("    let mut i: i32 = 0;\n    while true {\n        i = i + 1;\n        if i > 1000 {\n            i = 0;\n        }\n"
               "    }\n    return i;\n")
FATAL = main_of("    return div(7, 0);\n", "fn div(a: i32, b: i32) -> i32 {\n    return a / b;\n}\n\n")

DIAG = re.compile(r"error\[(COV\d{4})\]: ([^:\n]+):")


class Built:
    def __init__(self, d: Path) -> None:
        self.exe = d / "p"
        self.sv0b = d / "vm" / "p.sv0b"
        self.bind = d / "vm" / "p.sv0covbind.json"
        self.map_path = d / "p.sv0covmap.json"

    def argv(self, backend: str, bind: Path | None = None) -> list[str]:
        if backend == "native":
            return [str(self.exe)]
        return [str(SV0), "vm-run", "--coverage-binding", str(bind or self.bind), str(self.sv0b)]


def build(d: Path, text: str, errors: list[str], label: str) -> Built | None:
    (d / "vm").mkdir(parents=True)
    (d / "p.sv0").write_text(text)
    b = Built(d)
    p = sv0("native-compile", "--coverage=instrument", "-o", str(b.exe), str(d / "p.sv0"))
    err = build_vm([str(d / "p.sv0")], b.sv0b)
    if p.returncode or err:
        errors.append(f"{label}: build failed: {p.stderr.strip()[-300:]} {err or ''}")
        return None
    return b


def observe(argv: list[str], env: dict[str, str], pdir: Path | None) -> dict:
    """What one run did, in backend-neutral terms."""
    r = subprocess.run(argv, capture_output=True, env=env, timeout=120)
    out, err = r.stdout.decode("utf-8", "replace"), r.stderr.decode("utf-8", "replace")
    codes = DIAG.findall(err)
    lines = [ln for ln in err.splitlines() if ln.startswith(("sv0 panic", "sv0 contract violation", "sv0vm: fatal"))]
    return {"status": r.returncode, "ran": "user code ran" in out, "diag": codes[0][0] if codes else None,
            "title": codes[0][1] if codes else None, "ndiag": len(codes), "line": lines[0] if lines else None, "vm_exit": "vm_exit:" in out, "stderr": err,
            "files": sorted(f.name for f in pdir.iterdir()) if pdir is not None and pdir.is_dir() else []}


def same(a: dict, b: dict, keys: tuple[str, ...]) -> list[str]:
    return [f"{k}: native {a[k]!r}, VM {b[k]!r}" for k in keys if a[k] != b[k]]


def main() -> int:
    ensure_built()
    errors: list[str] = []
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)
        built: dict[str, Built] = {}
        todo = {**PROGRAMS, "spin": (SPIN, 0, None), "fatal": (FATAL, 0, None)}
        with ThreadPoolExecutor(max_workers=4) as pool:
            for name, b in zip(todo, pool.map(lambda kv: build(t / kv[0], kv[1][0], errors, kv[0]), todo.items())):
                if b is not None:
                    built[name] = b
        if errors:
            print("coverage lifecycle: FAIL", file=sys.stderr)
            for e in errors:
                print(f"  - {e}", file=sys.stderr)
            return 1
        n = itertools.count(1)

        def pair(label: str, b: Built, env_of) -> tuple[dict, dict, Path, Path]:
            k = next(n)
            obs = []
            dirs = []
            for backend in ("native", "vm"):
                pdir = t / f"run-{k}-{backend}"
                pdir.mkdir()
                dirs.append(pdir)
                obs.append(observe(b.argv(backend), env_of(pdir), pdir))
            return obs[0], obs[1], dirs[0], dirs[1]

        # 1. Exit paths.
        def exit_case(case: tuple[str, str]) -> None:
            name, req = case
            _, status, line = PROGRAMS[name]
            b = built[name]
            m = json.loads(b.map_path.read_bytes())
            label = f"{name} (required={req})"
            nat, vm, nd, vd = pair(label, b, lambda pdir: transport(pdir, SV0COV_REQUIRED=req))
            diff = same(nat, vm, ("status", "ran", "diag"))
            if diff:
                errors.append(f"{label}: backends differ: {'; '.join(diff)}")
            for backend, o in (("native", nat), ("VM", vm)):
                if o["status"] != status or not o["ran"] or o["diag"] is not None:
                    errors.append(f"{label} {backend}: status {o['status']} (want {status}), ran {o['ran']}, diagnostic {o['diag']}")
                if line is not None and not (o["line"] or "").startswith(line):
                    errors.append(f"{label} {backend}: stderr line {o['line']!r}, want {line!r}")
                if line is None and o["line"] is not None:
                    errors.append(f"{label} {backend}: unexpected failure line {o['line']!r}")
            if line is not None and line.startswith("sv0 panic") and nat["line"] != vm["line"]:
                errors.append(f"{label}: panic text differs: native {nat['line']!r}, VM {vm['line']!r}")
            if line is not None and "contract" in line and "half" not in (vm["line"] or "") + "|" + (nat["line"] or "")[-4:]:
                errors.append(f"{label}: the contract failure does not name the function")
            if not vm["vm_exit"]:
                errors.append(f"{label} VM: no vm_exit line for an orderly end")
            pn, pv = profile(nd, m, errors, f"{label} native"), profile(vd, m, errors, f"{label} VM")
            if pn is not None and pv is not None:
                if pn.counts != pv.counts or not pn.counts:
                    errors.append(f"{label}: counts differ or are empty: native {pn.counts}, VM {pv.counts}")
                if pn.profile_id == pv.profile_id:
                    errors.append(f"{label}: the two processes share a profile ID")
        exit_cases = [(name, req) for name in PROGRAMS for req in ("1", "0")]
        with ThreadPoolExecutor(max_workers=4) as pool:
            list(pool.map(exit_case, exit_cases))
        exits = len(exit_cases)

        # 2. Transport failures. (overrides, behaves as required whatever SV0COV_REQUIRED says)
        long_ctx = "x" * 257
        bad_utf8 = b"\xff\xfe".decode("utf-8", "surrogateescape")
        transport_cases: list[tuple[str, dict, bool, tuple[str, ...]]] = [
            ("unknown name", {"SV0COV_THRESHOLD": "90"}, False, ("90",)),
            ("uppercase run ID", {"SV0COV_RUN_ID": RUN_ID.upper()}, False, (RUN_ID.upper(),)),
            ("zero run ID", {"SV0COV_RUN_ID": "0" * 32}, False, ("0" * 32,)),
            ("short run ID", {"SV0COV_RUN_ID": RUN_ID[:31]}, False, (RUN_ID[:31],)),
            ("no run ID", {"SV0COV_RUN_ID": None}, False, ()),
            ("no profile directory", {"SV0COV_PROFILE_DIR": None}, False, ()),
            ("relative profile directory", {"SV0COV_PROFILE_DIR": "profiles/here"}, False, ("profiles/here",)),
            ("missing profile directory", {"SV0COV_PROFILE_DIR": str(t / "not-there")}, False, (str(t / "not-there"),)),
            ("overlong context", {"SV0COV_CONTEXT": long_ctx}, False, (long_ctx,)),
            ("non-UTF-8 context", {"SV0COV_CONTEXT": bad_utf8}, False, ()),
            ("malformed SV0COV_REQUIRED", {"SV0COV_REQUIRED": "yes"}, True, ("yes",)),
            ("empty SV0COV_REQUIRED", {"SV0COV_REQUIRED": ""}, True, ()),
        ]
        b = built["ret7"]
        def transport_case(case: tuple[str, dict, bool, tuple[str, ...], str]) -> None:
            label0, over, forced, secrets, req = case
            label = f"transport: {label0} (required={req})"
            env_over = {"SV0COV_REQUIRED": req, **over}
            nat, vm, _, _ = pair(label, b, lambda pdir: transport(pdir, **env_over))
            stops = req == "1" or forced
            diff = same(nat, vm, ("status", "ran", "diag", "title", "ndiag", "files"))
            if diff:
                errors.append(f"{label}: backends differ: {'; '.join(diff)}")
            for backend, o in (("native", nat), ("VM", vm)):
                want = (1, False) if stops else (7, True)
                if (o["status"], o["ran"]) != want or o["diag"] != "COV2001" or o["ndiag"] != 1 or o["files"]:
                    errors.append(f"{label} {backend}: status {o['status']}, ran {o['ran']}, diagnostic {o['diag']} x{o['ndiag']}, "
                                  f"files {o['files']}; want status {want[0]}, ran {want[1]}, one COV2001, nothing published")
                if any(s in o["stderr"] for s in secrets):
                    errors.append(f"{label} {backend}: the diagnostic shows a transport value")
                if backend == "VM" and o["vm_exit"] != (not stops):
                    errors.append(f"{label} VM: vm_exit line present = {o['vm_exit']}")
        t_cases = [(*c, req) for c in transport_cases for req in ("1", "0") if not (c[2] and req == "0")]
        with ThreadPoolExecutor(max_workers=4) as pool:
            list(pool.map(transport_case, t_cases))
        transports = len(t_cases)
        # No transport at all: the program runs outside sv0cov.
        nat, vm, _, _ = pair("outside", b, lambda pdir: transport(pdir, SV0COV_PROFILE_DIR=None, SV0COV_RUN_ID=None, SV0COV_REQUIRED=None))
        for backend, o in (("native", nat), ("VM", vm)):
            if (o["status"], o["ran"], o["diag"]) != (7, True, "COV2001") or o["files"]:
                errors.append(f"outside sv0cov {backend}: status {o['status']}, ran {o['ran']}, diagnostic {o['diag']}; "
                              "want the program to run with one COV2001 note and no profile")

        # 3. Publication failure: the directory cannot be written.
        publish = 0
        if os.geteuid() != 0:
            for name, status in (("ret0", 0), ("ret7", 7), ("vec-get", 1)):
                for req in ("1", "0"):
                    label = f"unwritable directory: {name} (required={req})"

                    def locked(pdir: Path) -> dict[str, str]:
                        pdir.chmod(0o500)
                        return transport(pdir, SV0COV_REQUIRED=req)
                    nat, vm, nd, vd = pair(label, built[name], locked)
                    nd.chmod(0o700)
                    vd.chmod(0o700)
                    want = 1 if req == "1" else status
                    diff = same(nat, vm, ("status", "ran", "diag", "title", "files"))
                    if diff:
                        errors.append(f"{label}: backends differ: {'; '.join(diff)}")
                    for backend, o in (("native", nat), ("VM", vm)):
                        if (o["status"], o["ran"], o["diag"]) != (want, True, "COV2010") or o["files"]:
                            errors.append(f"{label} {backend}: status {o['status']} (want {want}), ran {o['ran']}, "
                                          f"diagnostic {o['diag']} (want COV2010), files {o['files']}")
                    publish += 1

        # 4. Hard termination.
        kills = 0
        procs = []
        for sig in (signal.SIGKILL, signal.SIGTERM):
            for backend in ("native", "vm"):
                pdir = t / f"kill-{sig.name}-{backend}"
                pdir.mkdir()
                p = subprocess.Popen(built["spin"].argv(backend), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                                     env=transport(pdir), start_new_session=True)
                procs.append((sig, backend, pdir, p))
        time.sleep(6)
        for sig, backend, pdir, p in procs:
            label = f"{sig.name} {backend}"
            if p.poll() is not None:
                errors.append(f"{label}: the program ended by itself (status {p.returncode}) before the signal")
            try:
                os.killpg(p.pid, sig)
            except ProcessLookupError:
                pass
            try:
                p.wait(timeout=30)
            except subprocess.TimeoutExpired:
                os.killpg(p.pid, signal.SIGKILL)
                p.wait()
                errors.append(f"{label}: the process survived the signal")
            if p.returncode == 0 or list(pdir.iterdir()):
                errors.append(f"{label}: status {p.returncode}, files {[f.name for f in pdir.iterdir()]}; "
                              "want a failed status and an empty profile directory")
            kills += 1

        # 5. Capability rejection on sv0vm.
        b = built["ret7"]
        good = b.bind.read_text()
        swaps = [
            ("another coverage capability", '"sv0cov.coverage.v1"', '"sv0cov.coverage.v2"'),
            ("a capability in another case", '"sv0cov.coverage.v1"', '"SV0COV.coverage.v1"'),
            ("an extra capability", '["sv0cov.coverage.v1"]', '["sv0cov.coverage.v1","sv0cov.branch.v1"]'),
            ("no capability", '["sv0cov.coverage.v1"]', "[]"),
            ("another plan capability", '"sv0cov.plan.v1"', '"sv0cov.plan.v2"'),
            ("the typed-v2 VM profile", '"sv0vm-v1-coverage"', '"sv0vm-v2-typed"'),
            ("another raw-profile version", '"raw_profile_version":"1.0"', '"raw_profile_version":"2.0"'),
            ("another schema", '"sv0cov.vm-binding"', '"sv0cov.vm-binding2"'),
            ("another binding version", '"version":"1.0"}', '"version":"1.1"}'),
        ]
        rejections = 0
        for label0, old, new in swaps:
            if good.count(old) != 1:
                errors.append(f"capability: the binding does not contain {old} exactly once")
                continue
            bad = t / f"bind-{rejections}.json"
            bad.write_text(good.replace(old, new))
            for req in ("1", "0"):
                pdir = t / f"cap-{rejections}-{req}"
                pdir.mkdir()
                o = observe(b.argv("vm", bad), transport(pdir, SV0COV_REQUIRED=req), pdir)
                if (o["status"], o["ran"], o["diag"], o["vm_exit"]) != (1, False, "COV2201", False) or o["files"]:
                    errors.append(f"capability: {label0} (required={req}): status {o['status']}, ran {o['ran']}, "
                                  f"diagnostic {o['diag']}, files {o['files']}; want COV2201 before user code")
            rejections += 1
        for req in ("1", "0"):
            pdir = t / f"cap-none-{req}"
            pdir.mkdir()
            r = subprocess.run([str(SV0), "vm-run", str(b.sv0b)], capture_output=True, text=True,
                               env=transport(pdir, SV0COV_REQUIRED=req), timeout=120)
            if r.returncode != 1 or "error[COV2201]" not in r.stderr or "user code ran" in r.stdout or list(pdir.iterdir()):
                errors.append(f"capability: no binding (required={req}): status {r.returncode}; want COV2201 before user code")
        pdir = t / "cap-good"
        pdir.mkdir()
        o = observe(b.argv("vm"), transport(pdir), pdir)
        if (o["status"], o["ran"], o["diag"]) != (7, True, None) or len(o["files"]) != 1:
            errors.append(f"capability: the unchanged binding did not run and publish: {o['status']}, {o['files']}")

        # 6. A fatal VM failure is not an exit.
        for req in ("1", "0"):
            pdir = t / f"fatal-{req}"
            pdir.mkdir()
            o = observe(built["fatal"].argv("vm"), transport(pdir, SV0COV_REQUIRED=req), pdir)
            if o["status"] != 134 or not o["ran"] or o["vm_exit"] or o["files"] or not (o["line"] or "").startswith("sv0vm: fatal:"):
                errors.append(f"fatal VM failure (required={req}): status {o['status']}, vm_exit {o['vm_exit']}, files {o['files']}, "
                              f"line {o['line']!r}; want status 134, one `sv0vm: fatal:` line, no vm_exit, no profile")

    if errors:
        print("coverage lifecycle: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage lifecycle: OK (native = sv0vm on {exits} exit-path runs (returns, contract failures, runtime panics; "
          f"status, publication, counts), {transports} transport failures + outside sv0cov, {publish} unwritable-directory runs"
          f"{'' if publish else ' (SKIPPED as root)'}, {kills} hard terminations; {rejections} bad bindings + no binding rejected "
          "before user code; a fatal VM failure exits 134 with no profile)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
