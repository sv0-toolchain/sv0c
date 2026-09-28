#!/usr/bin/env python3
"""Coverage hit placement tests (sv0cov CV-112).

For --coverage=instrument, lowering places CovHit(counter) instructions from
the planner's node table, and the compiler checks the placement after
lowering: every planned counter exactly once, nothing else (COV1020
otherwise). The internal `hit-dump` mode prints each function's hits in IR
pre-order instead of C. This script checks:

1. For the seven sv0cov semantic fixtures and constructs.sv0, on the native
   compiler and the VM emitter: the hits are exactly counters
   0..program_counter_count-1 (from the program's map), and each function's
   first hit is its function_entry counter.
2. The negative cases fail the build with COV1020: a dropped hit
   (SV0_COVERAGE_FAULT=drop), a duplicated one (dup), a hit naming no planned
   counter (orphan), and any hit in map mode (dup under --coverage=map).
3. Through the real drivers, --coverage=instrument passes the check: the C
   driver emits instrumented C (CV-113), and the VM emitter is then refused
   (no COVER_HIT emission yet, CV-117) without COV1020.

    python3 sv0c/test/coverage/plan/run_hits.py
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
SV0 = ROOT / "scripts" / "sv0"
FIXTURES = ROOT / "sv0cov" / "tests" / "fixtures" / "semantic"
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
VM_EMIT = ROOT / "build" / "sv0-megatu-vm-native"

sys.path.insert(0, str(HERE))
from run_plan import ensure_built  # noqa: E402

PENDING = "not available on the VM yet: coverage hits are placed and checked"


def compile_with(binary: Path, request: str, coverage: str, fault: str | None = None) -> subprocess.CompletedProcess:
    env = {k: v for k, v in os.environ.items() if k not in ("SV0_COVERAGE_REQUEST", "SV0_COVERAGE_FAULT")}
    env["SV0_COVERAGE_REQUEST"] = coverage
    env["SV0_DRV_REQUEST"] = request
    if fault:
        env["SV0_COVERAGE_FAULT"] = fault
    argv = [str(binary)] + (request.split(" ", 1) if binary == NATIVE else [])
    return subprocess.run(argv, capture_output=True, text=True, env=env, timeout=300)


def programs() -> list[tuple[str, str]]:
    out = []
    for d in sorted(p.parent for p in FIXTURES.glob("*/expected-map.json")):
        project = any(p.parent != d for p in d.rglob("*.sv0"))
        out.append((d.name, f"--project {d}" if project else str(d / "main.sv0")))
    out.append(("constructs", str(HERE / "constructs.sv0")))
    return out


def main() -> int:
    ensure_built()
    errors: list[str] = []
    progs = programs()
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)
        for name, request in progs:
            mpath = t / f"{name}.json"
            p = compile_with(NATIVE, request, f"map\n{mpath}\n{name}\nsv0c+test")
            if p.returncode or not mpath.is_file():
                errors.append(f"{name}: map build failed: {p.stderr.strip()}")
                continue
            m = json.loads(mpath.read_bytes())
            total = m["program_counter_count"]
            entry = {}
            for pt in m["points"]:
                if pt["kind"] == "function_entry":
                    qname = m["entities"][pt["entity_index"]]["qualified_name"]
                    entry[qname.rsplit("::", 1)[-1]] = pt["counter_index"]
            for binary in (NATIVE, VM_EMIT):
                who = "native" if binary == NATIVE else "vm"
                p = compile_with(binary, request, "hit-dump\n/unused")
                if p.returncode:
                    errors.append(f"{name} ({who}): hit-dump failed ({p.returncode}): {p.stderr.strip()}")
                    continue
                seen = []
                for line in p.stdout.splitlines():
                    _, label, hits = line.split("\t")
                    counters = [int(x) for x in hits.split(",") if x]
                    seen.extend(counters)
                    if label in entry and (not counters or counters[0] != entry[label]):
                        errors.append(f"{name} ({who}): {label}'s first hit {counters[:1]} is not its entry counter {entry[label]}")
                if sorted(seen) != list(range(total)):
                    errors.append(f"{name} ({who}): hits {sorted(seen)} are not exactly 0..{total - 1}")

        # 2. Negative cases fail with COV1020.
        f0 = str(FIXTURES / "f0" / "main.sv0")
        for fault, needle in (("drop", "has no placement"), ("dup", "placed more than once"),
                              ("orphan", "names no planned counter")):
            for binary in (NATIVE, VM_EMIT):
                p = compile_with(binary, f0, f"instrument\n{t / 'neg.json'}\nf0\nsv0c+test", fault)
                if p.returncode != 9 or "COV1020" not in p.stderr or needle not in p.stderr:
                    errors.append(f"fault {fault} ({binary.name}): rc={p.returncode} stderr={p.stderr.strip()!r}")
        p = compile_with(NATIVE, f0, f"map\n{t / 'neg-map.json'}\nf0\nsv0c+test", "dup")
        if p.returncode != 9 or "COV1020" not in p.stderr or "--coverage=map placed coverage hits" not in p.stderr:
            errors.append(f"map-mode hit: rc={p.returncode} stderr={p.stderr.strip()!r}")

        # 3. The drivers reach the check, which passes: the C driver emits the
        # instrumented C (CV-113); the VM emitter refuses after the check
        # until COVER_HIT emission (CV-117).
        cpath = t / "inst.c"
        p = subprocess.run([str(SV0), "native-compile", "--emit=c", "--coverage=instrument", "-o", str(cpath), f0],
                           capture_output=True, text=True, timeout=300)
        if p.returncode or "COV1020" in p.stderr or "__sv0cov_hit(" not in (cpath.read_text() if cpath.exists() else ""):
            errors.append(f"native-compile --emit=c --coverage=instrument: rc={p.returncode} stderr={p.stderr.strip()!r}")
        p = subprocess.run([str(SV0), "vm-native-compile", "--coverage=instrument", f0, str(t / "inst.sv0b")],
                           capture_output=True, text=True, timeout=300)
        if p.returncode != 9 or PENDING not in p.stderr or "COV1020" in p.stderr:
            errors.append(f"vm-native-compile --coverage=instrument: rc={p.returncode} stderr={p.stderr.strip()!r}")

    if errors:
        print("coverage hits: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage hits: OK ({len(progs)} programs: every counter placed exactly once on native + VM emitter, "
          "entry first; drop/dup/orphan/map-mode faults fail with COV1020; drivers pass the check)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
