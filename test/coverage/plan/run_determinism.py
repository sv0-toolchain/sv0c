#!/usr/bin/env python3
"""Coverage map determinism and map-mode no-op proof (sv0cov CV-111).

For each program (the seven sv0cov semantic fixtures and constructs.sv0):

1. Determinism (COV-MAP-012): five map builds with the native compiler and
   one with the native VM emitter, each from a different working directory,
   map path, and environment noise (locale, time zone, a stray unrelated
   variable), produce byte-identical maps.
2. No-op (COV-INS-001/002): `map` mode adds nothing to the build. The
   generated C (native compiler) and the `.sv0b` (VM emitter) are
   byte-identical to an `off` build of the same program, and neither
   contains a coverage hit: no `sv0cov` symbol in the C.

    python3 sv0c/test/coverage/plan/run_determinism.py
"""

from __future__ import annotations

import os
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
FIXTURES = ROOT / "sv0cov" / "tests" / "fixtures" / "semantic"
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
VM_EMIT = ROOT / "build" / "sv0-megatu-vm-native"

sys.path.insert(0, str(HERE))
from run_plan import ensure_built  # noqa: E402

NOISE = [
    {},
    {"LANG": "C", "LC_ALL": "C"},
    {"TZ": "Pacific/Auckland", "LANG": "ja_JP.UTF-8"},
    {"SV0_UNRELATED": "x" * 100},
    {"HOME": "/nonexistent-home"},
]


def programs() -> list[tuple[str, str]]:
    out = []
    for d in sorted(p.parent for p in FIXTURES.glob("*/expected-map.json")):
        project = any(p.parent != d for p in d.rglob("*.sv0"))
        out.append((d.name, f"--project {d}" if project else str(d / "main.sv0")))
    out.append(("constructs", str(HERE / "constructs.sv0")))
    return out


def build(binary: Path, request: str, cwd: Path, coverage: str | None, extra: dict) -> tuple[int, bytes, str]:
    env = {k: v for k, v in os.environ.items() if k != "SV0_COVERAGE_REQUEST"}
    env.update(extra)
    env["SV0_DRV_REQUEST"] = request
    if coverage is not None:
        env["SV0_COVERAGE_REQUEST"] = coverage
    argv = [str(binary)] + (request.split(" ", 1) if binary == NATIVE else [])
    p = subprocess.run(argv, capture_output=True, env=env, cwd=cwd, timeout=300)
    return p.returncode, p.stdout, p.stderr.decode("utf-8", "replace")


def main() -> int:
    ensure_built()
    errors: list[str] = []
    progs = programs()
    if len(progs) < 8:
        errors.append(f"expected 7 sv0cov fixtures + constructs, found {[n for n, _ in progs]}")
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)
        for name, request in progs:
            maps = []
            for i, noise in enumerate(NOISE):
                cwd = t / name / f"cwd{i}"
                cwd.mkdir(parents=True)
                path = cwd / f"out{i}.sv0covmap.json"
                rc, _, err = build(NATIVE, request, cwd, f"map\n{path}\n{name}\nsv0c+test", noise)
                if rc != 0 or not path.is_file():
                    errors.append(f"{name}: native map build {i} failed ({rc}): {err.strip()}")
                    break
                maps.append(path.read_bytes())
            vpath = t / name / "vm.sv0covmap.json"
            rc, _, err = build(VM_EMIT, request, t / name, f"map\n{vpath}\n{name}\nsv0c+test", NOISE[2])
            if rc != 0 or not vpath.is_file():
                errors.append(f"{name}: VM emitter map build failed ({rc}): {err.strip()}")
            else:
                maps.append(vpath.read_bytes())
            if len(maps) == len(NOISE) + 1 and len(set(maps)) != 1:
                errors.append(f"{name}: {len(set(maps))} distinct maps from {len(maps)} builds")

            for binary, what in ((NATIVE, "generated C"), (VM_EMIT, ".sv0b")):
                rc_off, off, err_off = build(binary, request, t / name, None, {})
                rc_map, mapped, err_map = build(binary, request, t / name,
                                                f"map\n{t / name / 'noop.json'}\n{name}\nsv0c+test", {})
                if rc_off or rc_map:
                    errors.append(f"{name}: {what} build failed (off {rc_off}, map {rc_map}): {err_off}{err_map}")
                elif off != mapped:
                    errors.append(f"{name}: map-mode {what} differs from the off build")
                elif binary == NATIVE and b"sv0cov" in mapped:
                    errors.append(f"{name}: map-mode generated C mentions sv0cov")
    if errors:
        print("coverage determinism: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage determinism: OK ({len(progs)} programs x {len(NOISE) + 1} map builds byte-identical; "
          "map-mode C and .sv0b identical to off)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
