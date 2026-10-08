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
   the two together to exactly double.

And with small programs:

3. SV0COV_CONTEXT reaches the profile; a contract failure (exit 1) still
   publishes; a crash (division by zero) publishes nothing and leaves no
   temporary;
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

import json
import os
import re
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
            exe, sv0b = d / name, d / f"{name}.sv0b"
            p = sv0("native-compile", "--coverage=instrument", "-o", str(exe), *args)
            err = build_vm(args, sv0b)
            if p.returncode or err:
                errors.append(f"{name}: build failed: {p.stderr.strip()} {err or ''}")
                continue
            m = json.loads((d / f"{name}.sv0covmap.json").read_bytes())
            npdir, vpdir = d / "native-profiles", d / "vm-profiles"
            npdir.mkdir()
            vpdir.mkdir()
            rn = subprocess.run([str(exe)], capture_output=True, env=transport(npdir), timeout=60)
            rc, out = vm_run(sv0b, d / f"{name}.sv0covbind.json", transport(vpdir))
            if rc is None or rc != rn.returncode:
                errors.append(f"{name}: native exit {rn.returncode}, VM exit {rc}: {out[-300:]}")
            native = profile(npdir, m, errors, f"{name} native")
            vm = profile(vpdir, m, errors, f"{name} VM")
            if native is None or vm is None:
                continue
            if vm.backend != "vm-v1" or native.backend != "native":
                errors.append(f"{name}: backends {native.backend}/{vm.backend}, want native/vm-v1")
            n = m["program_counter_count"]
            if dense(vm, n) != dense(native, n):
                errors.append(f"{name}: VM counts {dense(vm, n)} differ from native {dense(native, n)}")
            resolved.append((name, str(d / f"{name}.sv0covmap.json"), str(next(npdir.iterdir())),
                             str(next(vpdir.iterdir()))))
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
                "for name, m, nat, vm in json.loads(sys.argv[2]):\n"
                "    mb = Path(m).read_bytes()\n"
                "    a = resolve(mb, [Path(nat).read_bytes()]).counts()\n"
                "    b = resolve(mb, [Path(vm).read_bytes()]).counts()\n"
                "    both = resolve(mb, [Path(nat).read_bytes(), Path(vm).read_bytes()])\n"
                "    if a != b: bad.append(f'{name}: native and VM resolve differently')\n"
                "    if both.counts() != {k: 2 * v for k, v in a.items()} or both.backends != ('native', 'vm-v1'):\n"
                "        bad.append(f'{name}: native + VM did not add up')\n"
                "    exp = Path(sys.argv[3]) / name / 'expected-counts.json'\n"
                "    if exp.is_file():\n"
                "        want = {e['point_id']: e['count'] for e in json.loads(exp.read_bytes())['counts']}\n"
                "        if a != want: bad.append(f'{name}: resolved counts differ from expected-counts.json')\n"
                "print('\\n'.join(bad))\n"
            )
            p = subprocess.run([*py, "-c", code, str(ROOT / "sv0cov" / "src"), json.dumps(resolved), str(FIXTURES)],
                               capture_output=True, text=True, timeout=300)
            if p.returncode or p.stdout.strip():
                errors.append(f"sv0cov reader: {p.stdout.strip()} {p.stderr.strip()[-600:]}")
            reader = "sv0cov reader resolves native + VM to the expected counts"

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

            sv0b, bind, m = built["crash"]
            pdir = t / "crash"
            pdir.mkdir()
            rc, out = vm_run(sv0b, bind, transport(pdir))
            if rc is not None or list(pdir.iterdir()):
                errors.append(f"crash: exit {rc}, files {[f.name for f in pdir.iterdir()]}; want no exit and nothing published")

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
          "context, contract failure publishes, crash publishes nothing; required transport failure stops before user "
          "code; outside sv0cov runs and publishes nothing; sv0vm golden copies = sv0cov's)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
