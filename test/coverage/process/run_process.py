#!/usr/bin/env python3
"""Process-creation coverage boundary, end to end (sv0cov CV-405; SPEC
14.2.2; D-4; COV-INS-016, COV-C-007, COV-VM-026, AC-096).

sv0 has no process API. A native program can only fork through `#[extern_c]`
FFI, which reaches the sv0cov runtime as an unmanaged fork; sv0vm accepts no
FFI at all, so it exposes no host-process operation.

1. A real sv0 program built with `--coverage=instrument` for the C backend
   forks through `#[extern_c] fn fork`, and the child returns into
   instrumented code. Required mode: the child stops before its first hit,
   the parent's coverage is unresolved, both say COV2120, the parent exits 1,
   and no profile is published. Not required: the program's own status
   stands and still nothing is published (the parent is unresolved and the
   child never publishes inherited counters).
2. The same program without the fork call publishes one complete profile
   (control: the fork, not the program, withholds it).
3. The VM build of the forking program is refused at compile time (E0553)
   and writes no bytecode, map or binding.

    python3 sv0c/test/coverage/process/run_process.py
"""

from __future__ import annotations

import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent

sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
sys.path.insert(0, str(ROOT / "sv0cov" / "src"))
from run_plan import ensure_built  # noqa: E402
from run_vm_profile import sv0, transport  # noqa: E402

PROGRAM = """\
/// Caller obligation: none; the child is a copy of this process (POSIX fork).
#[extern_c]
fn fork() -> i32;

fn work(n: i32) -> i32 {
    if n > 1 {
        return n * 2;
    }
    return n;
}

fn main() -> i32 {
    let before: i32 = work(3);
    let pid: i32 = FORK;
    if pid == 0 {
        return work(1);
    }
    println("parent");
    return before - 1;
}
"""

CHILD_HIT = "a coverage hit ran in a forked process image"
UNRESOLVED = "an unmanaged fork left this process image's coverage unresolved"


def main() -> int:
    ensure_built()
    errors: list[str] = []
    with tempfile.TemporaryDirectory() as tmp:
        t = Path(tmp)
        forker, plain = t / "forker.sv0", t / "plain.sv0"
        forker.write_text(PROGRAM.replace("FORK", "fork()"))
        plain.write_text(PROGRAM.replace("FORK", "1"))
        for src in (forker, plain):
            b = sv0("native-compile", "--coverage=instrument", "-o", str(t / src.stem), str(src))
            if b.returncode != 0:
                print(f"coverage process: FAIL: native build of {src.name}: {b.stderr.strip()[-300:]}")
                return 1

        # 1. Unmanaged fork, required and not.
        for required, want in (("1", 1), ("0", 5)):
            pdir = t / f"p-fork-{required}"
            pdir.mkdir()
            # Both images share the pipes: run() returns once the child has exited too.
            r = subprocess.run([str(t / "forker")], capture_output=True, text=True, timeout=60,
                               env=transport(pdir, SV0COV_REQUIRED=required))
            label = f"fork (required={required})"
            if r.returncode != want:
                errors.append(f"{label}: exit {r.returncode}, want {want}: {r.stderr.strip()[-300:]}")
            if r.stdout != "parent\n":
                errors.append(f"{label}: stdout {r.stdout!r}")
            if UNRESOLVED not in r.stderr:
                errors.append(f"{label}: the parent did not report its unresolved lifecycle")
            if required == "1" and CHILD_HIT not in r.stderr:
                errors.append(f"{label}: the child was not stopped before its first hit")
            if required == "0" and CHILD_HIT in r.stderr:
                errors.append(f"{label}: the child was stopped outside required mode")
            if r.stderr.count("error[") != r.stderr.count("error[COV2120]"):
                errors.append(f"{label}: a diagnostic other than COV2120: {r.stderr!r}")
            if list(pdir.iterdir()):
                errors.append(f"{label}: published {[p.name for p in pdir.iterdir()]}")

        # 2. Control: no fork, one complete profile.
        pdir = t / "p-plain"
        pdir.mkdir()
        r = subprocess.run([str(t / "plain")], capture_output=True, text=True, timeout=60, env=transport(pdir))
        names = [p.name for p in pdir.iterdir()]
        if r.returncode != 5 or r.stderr or len(names) != 1:
            errors.append(f"control: exit {r.returncode}, stderr {r.stderr!r}, profiles {names}")

        # 3. The VM refuses the FFI declaration before producing anything.
        vm = t / "vm"
        vm.mkdir()
        v = sv0("vm-native-compile", "--coverage=instrument", str(forker), str(vm / "forker.sv0b"))
        if v.returncode == 0 or "error[E0553]" not in v.stderr:
            errors.append(f"vm: not refused with E0553 (exit {v.returncode}): {v.stderr.strip()[-300:]}")
        if list(vm.iterdir()):
            errors.append(f"vm: refused build left {[p.name for p in vm.iterdir()]}")

    if errors:
        for e in errors:
            print(f"coverage process: FAIL: {e}")
        return 1
    print("coverage process: OK (native unmanaged fork through #[extern_c]: required stops the child before its "
          "first hit and withholds the unresolved parent, not required publishes nothing either; the unforked "
          "control publishes one profile; the VM build refuses the FFI with E0553 and writes nothing)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
