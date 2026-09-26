#!/usr/bin/env python3
"""SHA-256 known-answer tests for the sv0 implementation (CV-103 / CV-104).

Runs in ``./scripts/sv0 test`` (with ``--differential``).

Steps:
1. ``vectors.tsv`` digests are recomputed with Python ``hashlib`` (the data
   itself must be right), and every digest appears in ``kat_main.sv0`` in
   table order.
2. The implementation file (default ``sv0c/lib/sha256.sv0``) is prepended to
   ``kat_main.sv0``, mirroring the mega-TU, and the result is compiled and run
   on the native backend and on the VM. Both must exit 0; a nonzero status is
   the 1-based index of the first failing vector.

    python3 sv0c/test/crypto/sha256/run_kat.py [--impl FILE] [--backend native|vm|both]
"""

from __future__ import annotations

import argparse
import hashlib
import re
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]  # sv0-toolchain
SV0 = ROOT / "scripts" / "sv0"
DEFAULT_IMPL = ROOT / "sv0c" / "lib" / "sha256.sv0"


def vectors() -> list[tuple[str, bytes, str]]:
    out = []
    for line in (HERE / "vectors.tsv").read_text().splitlines():
        if not line or line.startswith("#"):
            continue
        name, spec, digest = line.split("\t")
        kind, _, rest = spec.partition(":")
        if kind == "text":
            msg = rest.encode("ascii")
        else:
            text, _, count = rest.rpartition(":")
            msg = text.encode("ascii") * int(count)
        out.append((name, msg, digest))
    return out


def check_data() -> list[str]:
    errors = []
    kat = (HERE / "kat_main.sv0").read_text()
    found = re.findall(r'"([0-9a-f]{64})"', kat)
    rows = vectors()
    for name, msg, digest in rows:
        if hashlib.sha256(msg).hexdigest() != digest:
            errors.append(f"vectors.tsv {name}: digest does not match hashlib")
    if found != [d for _, _, d in rows]:
        errors.append("kat_main.sv0 digests differ from vectors.tsv (order or value)")
    return errors


ALPHABET = [chr(c) for c in range(0x20, 0x7F) if chr(c) not in '"\\{}'] + ["\u00e9", "\u00df", "\u65e5", "\u672c", "\u2713", "\U0001f600"]


def differential_program(cases: int = 200, seed: int = 20260925) -> str:
    """A KAT program over seeded pseudo-random UTF-8 messages, digests from hashlib."""
    state = seed
    lines = ["fn main() -> i32 {"]
    for i in range(cases):
        state = (state * 6364136223846793005 + 1442695040888963407) % (1 << 64)
        length = (state >> 33) % 301
        chars = []
        for _ in range(length):
            state = (state * 6364136223846793005 + 1442695040888963407) % (1 << 64)
            chars.append(ALPHABET[(state >> 33) % len(ALPHABET)])
        msg = "".join(chars)
        digest = hashlib.sha256(msg.encode("utf-8")).hexdigest()
        lines.append(f'  if string_eq(sha256_hex("{msg}"), "{digest}") {{\n  }} else {{\n    return {i + 1};\n  }};')
    lines.append("  return 0;")
    lines.append("}")
    return "\n".join(lines) + "\n"


LONG_VECTOR = re.compile(r'  if check\(repeat_str\("a", 1000000\).*?\n  \};\n', re.S)


def run(impl: Path, backend: str, program: bytes | None = None, vm_long: bool = False) -> int:
    kat = (HERE / "kat_main.sv0").read_text()
    native_prog = program if program is not None else kat.encode()
    # The SML interpreter needs many minutes for the 15,626-block million-'a'
    # vector, so the VM skips it unless asked (--vm-long); native always runs it.
    vm_prog = program if program is not None else (kat if vm_long else LONG_VECTOR.sub("", kat, count=1)).encode()
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "main.sv0"
        src.write_bytes(impl.read_bytes() + b"\n" + native_prog)
        status = 0
        if backend in ("native", "both"):
            exe = Path(tmp) / "kat"
            c = subprocess.run([str(SV0), "native-compile", "-o", str(exe), str(src)], capture_output=True, text=True)
            if c.returncode:
                print(f"native: compile failed:\n{c.stderr.strip()[-2000:]}", file=sys.stderr)
                return 1
            r = subprocess.run([str(exe)], capture_output=True)
            print(f"native: exit {r.returncode}" + ("" if r.returncode == 0 else f" (first failing vector #{r.returncode})"))
            status |= r.returncode != 0
        if backend in ("vm", "both"):
            src.write_bytes(impl.read_bytes() + b"\n" + vm_prog)
            sv0b = Path(tmp) / "kat.sv0b"
            c = subprocess.run([str(SV0), "vm-native-compile", str(src), str(sv0b)], capture_output=True, text=True)
            if c.returncode:
                print(f"vm: compile failed:\n{c.stderr.strip()[-2000:]}", file=sys.stderr)
                return 1
            r = subprocess.run([str(SV0), "vm-run", str(sv0b)], capture_output=True, text=True)
            m = re.search(r"vm_exit:(-?\d+)", r.stdout + r.stderr)
            code = int(m.group(1)) if m else -1
            print(f"vm: exit {code}" + ("" if code == 0 else f" (first failing vector #{code})"))
            status |= code != 0
        return status


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--impl", type=Path, default=DEFAULT_IMPL)
    ap.add_argument("--backend", choices=("native", "vm", "both"), default="both")
    ap.add_argument("--vm-long", action="store_true", help="include the million-'a' vector on the VM (slow)")
    ap.add_argument("--differential", action="store_true", help="also run 200 seeded random messages against hashlib")
    ap.add_argument("--differential-backend", choices=("native", "vm", "both"), default="native")
    args = ap.parse_args()
    errors = check_data()
    for e in errors:
        print(f"run_kat: {e}", file=sys.stderr)
    if errors:
        return 2
    if not args.impl.is_file():
        print(f"run_kat: RED — {args.impl.relative_to(ROOT) if args.impl.is_relative_to(ROOT) else args.impl} does not exist yet (CV-104)")
        return 1
    status = run(args.impl, args.backend, vm_long=args.vm_long)
    if args.differential:
        print("differential (200 seeded UTF-8 messages vs hashlib):")
        status |= run(args.impl, args.differential_backend, differential_program().encode("utf-8"))
    return status


if __name__ == "__main__":
    sys.exit(main())
