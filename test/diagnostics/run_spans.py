#!/usr/bin/env python3
"""Diagnostic locations that used to print at 1:1.

The diagnostics corpus (manifest.txt) checks codes only; this checks where a
few diagnostics point, on the binaries that emit them:

- E0408 for a function body with no value and no return, at the function's
  name (it was E0400 at 1:1);
- E0400 for a body whose value has the wrong type, at the function's name;
- E0553 (VM backend: raw pointers, `unsafe`, `#[extern_c]`), at the first
  such construct (it was 1:1).

    python3 sv0c/test/diagnostics/run_spans.py
"""

from __future__ import annotations

import os
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[1]
ROOT = SV0C.parent
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
VM_EMIT = ROOT / "build" / "sv0-megatu-vm-native"
sys.dont_write_bytecode = True
sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built  # noqa: E402

HEADER = "/* SPDX-License-Identifier: MIT OR Apache-2.0 */\n"
# (label, source, binaries, code, line:col)
CASES = [
    ("no value", HEADER + "fn f() -> i32 {\n    let a: i32 = 1;\n}\nfn main() -> i32 { return f(); }\n", "both", "E0408", "2:4"),
    ("if without else", HEADER + "fn f(x: i32) -> i32 {\n    if x > 0 { return 1; };\n}\nfn main() -> i32 { return f(1); }\n",
     "both", "E0408", "2:4"),
    ("wrong value type", HEADER + "fn f() -> i32 {\n    true\n}\nfn main() -> i32 { return f(); }\n", "both", "E0400", "2:4"),
    ("extern_c on the VM", HEADER + "\n#[extern_c] fn abs(x: i32) -> i32;\nfn main() -> i32 { return abs(0); }\n", "vm", "E0553", "3:1"),
    ("unsafe on the VM", HEADER + "fn main() -> i32 {\n    let a: i32 = 0;\n    unsafe {\n        let b: i32 = a;\n    };\n    return a;\n}\n",
     "vm", "E0553", "4:5"),
]


def run(binary: Path, src: Path) -> subprocess.CompletedProcess:
    env = {k: v for k, v in os.environ.items() if not k.startswith("SV0")}
    env["SV0_DRV_REQUEST"] = str(src)
    argv = [str(binary)] if binary == VM_EMIT else [str(binary), str(src)]
    return subprocess.run(argv, capture_output=True, text=True, env=env, timeout=300)


def main() -> int:
    ensure_built()
    errors: list[str] = []
    with tempfile.TemporaryDirectory() as td:
        src = Path(td) / "p.sv0"
        for label, text, which, code, where in CASES:
            src.write_text(text)
            binaries = {"both": (NATIVE, VM_EMIT), "vm": (VM_EMIT,)}[which]
            for b in binaries:
                p = run(b, src)
                if p.returncode == 0 or f"error[{code}]" not in p.stderr or f"--> {where}\n" not in p.stderr:
                    errors.append(f"{label} ({b.name}): exit {p.returncode}, want {code} at {where}: {p.stderr.strip()[:200]!r}")
    if errors:
        print("diagnostic spans: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"diagnostic spans: OK ({len(CASES)} cases: E0408/E0400 at the function, E0553 at the construct)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
