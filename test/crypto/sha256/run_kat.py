#!/usr/bin/env python3
"""SHA-256 known-answer tests for the sv0 implementation (CV-103 / CV-104).

Red until CV-104 adds ``sv0c/lib/sha256.sv0``. Not wired into ``./scripts/sv0
test`` yet; CV-104 wires it in when the vectors pass.

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


def run(impl: Path, backend: str) -> int:
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "main.sv0"
        src.write_bytes(impl.read_bytes() + b"\n" + (HERE / "kat_main.sv0").read_bytes())
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
    args = ap.parse_args()
    errors = check_data()
    for e in errors:
        print(f"run_kat: {e}", file=sys.stderr)
    if errors:
        return 2
    if not args.impl.is_file():
        print(f"run_kat: RED — {args.impl.relative_to(ROOT) if args.impl.is_relative_to(ROOT) else args.impl} does not exist yet (CV-104)")
        return 1
    return run(args.impl, args.backend)


if __name__ == "__main__":
    sys.exit(main())
