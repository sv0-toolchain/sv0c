#!/usr/bin/env python3
"""Line coverage from real runs: the partial-line matrix (sv0cov CV-202;
SPEC 11.4, COV-MET-003, BL-021).

matrix.sv0 holds every line shape the R0 planner produces: one-line `if` /
`else` and `while` with an untaken part (partial), a block match arm (a
container: its brace lines are non_executable), an expression arm under a
multi-line `return match` (partial), a multi-line statement with a comment
line (counted: a comment is a non-whitespace byte) and a blank line (not
counted), a conditional-expression initializer, `for` with `continue`,
`while true` left by `break`, dead code after a `return` (non_executable),
and an uncalled function (uncovered). matrix.expected is the hand-reviewed
status of every physical line.

For two spellings of the same program (as written; and CRLF line endings,
tab indentation, a multibyte comment, and no final newline), on the native
C backend and on the VM:

1. the program exits 0 and publishes one profile; native and VM maps are
   byte-identical;
2. the map's line flags follow the conventions: only `user` regions
   contribute, and branch bodies, loop bodies, and block match arms do not;
3. `python -m sv0cov.lines` (sv0cov's SPEC 11.4 derivation, which validates
   the map against the exact source bytes) prints matrix.expected.

    python3 sv0c/test/coverage/lines/run_lines.py
"""

from __future__ import annotations

import json
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent

sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import build_vm, sv0, transport, vm_run  # noqa: E402


def variants() -> dict[str, bytes]:
    lf = (HERE / "matrix.sv0").read_bytes()
    lines = []
    for line in lf.decode("utf-8").split("\n"):
        body = line.lstrip(" ")
        lines.append("\t" * ((len(line) - len(body)) // 4) + body)
    other = "\r\n".join(lines).replace("a comment inside", "a comment (注释, Größe) inside").rstrip("\r\n")
    return {"lf": lf, "crlf-tabs-multibyte-nofinal": other.encode("utf-8")}


def main() -> int:
    ensure_built()
    errors: list[str] = []
    py = modern_python()
    want = [line for line in (HERE / "matrix.expected").read_text().splitlines() if line and not line.startswith("#")]
    want = [f"main.sv0:{line.split()[0]} {line.split()[1]}" for line in want]
    checked = 0
    with tempfile.TemporaryDirectory() as td:
        for name, data in variants().items():
            d = Path(td) / name
            (d / "vm").mkdir(parents=True)
            src = d / "main.sv0"
            src.write_bytes(data)
            exe, sv0b = d / "main", d / "vm" / "main.sv0b"
            p = sv0("native-compile", "--coverage=instrument", "-o", str(exe), str(src))
            err = build_vm([str(src)], sv0b)
            if p.returncode or err:
                errors.append(f"{name}: build failed: {p.stderr.strip()} {err or ''}")
                continue
            nmap, vmap = d / "main.sv0covmap.json", d / "vm" / "main.sv0covmap.json"
            if nmap.read_bytes() != vmap.read_bytes():
                errors.append(f"{name}: the generated-C and VM maps differ")
            m = json.loads(nmap.read_bytes())
            for r in m["regions"]:
                if r["line_contributing"] and r["classification"] != "user":
                    errors.append(f"{name}: a {r['classification']} region contributes to lines")
                at = r["span"]["start_byte"]
                container = r["kind"] in ("branch_body", "loop_body") or (r["kind"] == "match_arm" and data[at:at + 1] == b"{")
                if container and r["line_contributing"]:
                    errors.append(f"{name}: container {r['kind']} at byte {at} contributes to lines")
            for backend in ("native", "vm"):
                pdir = d / f"{backend}-profiles"
                pdir.mkdir()
                if backend == "native":
                    rc = subprocess.run([str(exe)], capture_output=True, env=transport(pdir), timeout=60).returncode
                    out = ""
                else:
                    rc, out = vm_run(sv0b, d / "vm" / "main.sv0covbind.json", transport(pdir))
                profiles = sorted(pdir.iterdir())
                if rc != 0 or len(profiles) != 1:
                    errors.append(f"{name} {backend}: exit {rc}, {len(profiles)} profiles {out[-300:]}")
                    continue
                if py is None:
                    continue
                got = subprocess.run([*py, "-m", "sv0cov.lines", str(nmap if backend == "native" else vmap), str(d),
                                      str(profiles[0])], capture_output=True, text=True, timeout=120,
                                     env={**transport(pdir), "PYTHONPATH": str(ROOT / "sv0cov" / "src")})
                lines = got.stdout.splitlines()
                if got.returncode or lines != want:
                    diff = [f"{g!r} != {w!r}" for g, w in zip(lines, want) if g != w][:5]
                    errors.append(f"{name} {backend}: line statuses differ from matrix.expected "
                                  f"({len(lines)} vs {len(want)} lines) {diff} {got.stderr.strip()[-300:]}")
                    continue
                checked += 1

    if errors:
        print("coverage lines: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    note = f"{checked} runs match matrix.expected" if py is not None else "status derivation SKIPPED (no Python >= 3.10)"
    print(f"coverage lines: OK (2 spellings x native + VM; {note})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
