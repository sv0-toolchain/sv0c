#!/usr/bin/env python3
"""VM coverage emission tests (sv0cov CV-117, sv0doc bytecode/coverage.md).

With --coverage=instrument the native VM emitter lowers each placed hit to
COVER_HIT <counter> (opcode 119, u32le operand, 5 bytes, stack-neutral), and
jump displacements, which are byte-relative, widen by five bytes for every
hit they cross. For the seven sv0cov semantic fixtures and constructs.sv0
this checks:

1. The instrument map is byte-identical to the map-mode map.
2. The bytecode holds exactly one COVER_HIT per counter 0..N-1, and every
   JUMP / JUMP_IF / JUMP_IF_NOT lands on an instruction boundary of its
   function (or its end).
3. Jump offsets are right, not just aligned: removing every COVER_HIT and
   re-deriving each jump's displacement from its old target gives bytecode
   that sv0vm runs to the same exit code as the `off` build.
4. The emitter's disassembly (SV0_VM_DISASM, bytecode.sv0 disasm_file)
   agrees with this script's own decoder, and f0's instrumented disassembly
   equals the golden vm-disasm-f0.expected.txt (`--update` rewrites it).
5. A VM without coverage support rejects the instrumented bytecode with
   "unknown opcode 119" before running anything (until CV-119 teaches
   sv0vm the opcode, this is the reference sv0vm).
6. The companion binding (CV-118; request line 5) is written beside it:
   sv0cov's own decoder (sv0cov.formats.vmbinding, run with a Python >=
   3.10) accepts it bound to the exact bytecode bytes, it equals
   encode_v1 byte for byte and names the map's ID, counter count, and
   compiler identity, and a second emission gives the same bytes. Without a
   binding path the VM build is refused (exit 9) and writes no map.

    python3 sv0c/test/coverage/plan/run_emit_vm.py [--update]
"""

from __future__ import annotations

import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
SV0VM = ROOT / "sv0vm"
GOLDEN = HERE / "vm-disasm-f0.expected.txt"
IDENTITY = "sv0c+test"

sys.path.insert(0, str(HERE))
from run_hits import VM_EMIT, programs  # noqa: E402
from run_plan import ensure_built, modern_python  # noqa: E402

JUMPS = {112, 113, 114}
COVER_HIT = 119
# bytecode.sv0 insn_encoded_size
SIZES = {0: 1, 1: 1, 2: 1, 3: 1, 4: 5, 5: 9, 6: 9, 7: 2, 8: 5, 96: 5, 97: 5, 112: 5, 113: 5, 114: 5, 115: 9,
         116: 1, 117: 5, 118: 2, 119: 5, 128: 5, 129: 5, 130: 5, 131: 5, 132: 1, 133: 1, 144: 13, 145: 1,
         146: 5, 160: 5, 161: 5}
for lo, hi in ((16, 21), (32, 45), (48, 52), (64, 73), (80, 82), (88, 93)):
    SIZES.update({op: 1 for op in range(lo, hi + 1)})


class Sv0b:
    """A parsed .sv0b v1: header + string section kept as bytes, and each
    function's (name, arity, locals, code bytes)."""

    def __init__(self, data: bytes) -> None:
        if data[:4] != b"SV0B" or struct.unpack_from("<H", data, 4)[0] != 1:
            raise ValueError("not a .sv0b v1 file")
        slen = struct.unpack_from("<I", data, 6)[0]
        self.strings = data[6:10 + slen]
        pos = 10 + slen
        flen, count = struct.unpack_from("<II", data, pos)
        pos += 8
        code_start = pos - 4 + flen + 4
        self.funcs = []
        for k in range(count):
            name, arity, local, off, length = struct.unpack_from("<5I", data, pos + 20 * k)
            self.funcs.append((name, arity, local, data[code_start + off:code_start + off + length]))

    def encode(self) -> bytes:
        out = bytearray(b"SV0B" + struct.pack("<H", 1) + self.strings)
        out += struct.pack("<II", 4 + 20 * len(self.funcs), len(self.funcs))
        off = 0
        for name, arity, local, code in self.funcs:
            out += struct.pack("<5I", name, arity, local, off, len(code))
            off += len(code)
        out += struct.pack("<I", off)
        for f in self.funcs:
            out += f[3]
        return bytes(out)


def insns(code: bytes) -> list[tuple[int, int, bytes]]:
    """(offset, opcode, encoded bytes) for each instruction."""
    out, pos = [], 0
    while pos < len(code):
        op = code[pos]
        size = SIZES.get(op)
        if size is None:
            raise ValueError(f"unknown opcode {op} at {pos}")
        out.append((pos, op, code[pos:pos + size]))
        pos += size
    return out


def strip_hits(code: bytes) -> bytes:
    """The code without COVER_HIT, each jump retargeted to the instruction it
    reached before (a jump onto a hit reaches what follows the hit)."""
    old = insns(code)
    new_off, pos = {}, 0
    for off, op, raw in old:
        new_off[off] = pos
        if op != COVER_HIT:
            pos += len(raw)
    new_off[len(code)] = pos
    out = bytearray()
    for off, op, raw in old:
        if op == COVER_HIT:
            continue
        if op in JUMPS:
            target = off + 5 + struct.unpack_from("<i", raw, 1)[0]
            raw = bytes([op]) + struct.pack("<i", new_off[target] - (new_off[off] + 5))
        out += raw
    return bytes(out)


def run_sv0vm(path: Path) -> tuple[int | None, str]:
    try:
        p = subprocess.run(["sml"], stdin=open(SV0VM / "scripts" / "run_sv0b.sml"), capture_output=True, text=True,
                           cwd=SV0VM, env={**os.environ, "SV0B": str(path)}, timeout=60)
    except subprocess.TimeoutExpired:
        return None, "sv0vm did not finish within 60 s (a misplaced jump can loop forever)"
    out = p.stdout + p.stderr
    m = re.search(r"vm_exit:([~-]?\d+)", out)  # SML writes negatives as ~7
    return (int(m.group(1).replace("~", "-")) & 0xFF if m else None), out


def emit(request: str, coverage: str, disasm: bool = False) -> subprocess.CompletedProcess:
    env_extra = {"SV0_VM_DISASM": "1"} if disasm else {}
    env = {k: v for k, v in os.environ.items() if k not in ("SV0_COVERAGE_REQUEST", "SV0_VM_DISASM")}
    env.update(env_extra, SV0_DRV_REQUEST=request)
    if coverage:
        env["SV0_COVERAGE_REQUEST"] = coverage
    return subprocess.run([str(VM_EMIT)], capture_output=True, env=env, timeout=300)


def main() -> int:
    update = "--update" in sys.argv[1:]
    ensure_built()
    errors: list[str] = []
    progs = programs()
    rejected_checked = False
    bindings: list[tuple[str, str, str]] = []
    validated = False
    with tempfile.TemporaryDirectory() as td:
        t = Path(td)
        for name, request in progs:
            ref, imap = t / f"{name}.map.json", t / f"{name}.inst.json"
            m = emit(request, f"map\n{ref}\n{name}\n{IDENTITY}")
            off = emit(request, "")
            bind = t / f"{name}.sv0covbind.json"
            inst = emit(request, f"instrument\n{imap}\n{name}\n{IDENTITY}\n{bind}")
            if m.returncode or off.returncode or inst.returncode:
                errors.append(f"{name}: map rc={m.returncode} off rc={off.returncode} instrument rc={inst.returncode}: "
                              f"{inst.stderr.decode(errors='replace').strip()}")
                continue
            # 1. The same map as map mode.
            if not imap.is_file() or imap.read_bytes() != ref.read_bytes():
                errors.append(f"{name}: the instrument map is missing or differs from the map-mode map")
                continue
            total = json.loads(ref.read_bytes())["program_counter_count"]
            # 2. One hit per counter; jumps land on boundaries.
            prog = Sv0b(inst.stdout)
            hits = []
            for fi, (_, _, _, code) in enumerate(prog.funcs):
                listing = insns(code)
                bounds = {at for at, _, _ in listing} | {len(code)}
                for at, op, raw in listing:
                    if op == COVER_HIT:
                        hits.append(struct.unpack_from("<I", raw, 1)[0])
                    if op in JUMPS:
                        target = at + 5 + struct.unpack_from("<i", raw, 1)[0]
                        if target not in bounds:
                            errors.append(f"{name}: fn {fi} jump at {at} lands at {target}, not an instruction")
            if sorted(hits) != list(range(total)):
                errors.append(f"{name}: COVER_HIT operands {sorted(hits)} are not exactly 0..{total - 1}")
            # 3. Stripped of its hits, the bytecode runs like the off build.
            stripped = t / f"{name}.stripped.sv0b"
            for k, f in enumerate(prog.funcs):
                prog.funcs[k] = (*f[:3], strip_hits(f[3]))
            stripped.write_bytes(prog.encode())
            offb = t / f"{name}.off.sv0b"
            offb.write_bytes(off.stdout)
            rc_off, out_off = run_sv0vm(offb)
            rc_str, out_str = run_sv0vm(stripped)
            if rc_off is None or rc_off != rc_str:
                errors.append(f"{name}: off exit {rc_off}, stripped instrumented exit {rc_str}: {out_str[-300:]}")
            # 4. Disassembly agrees with this decoder; f0 is pinned.
            bind2 = t / f"{name}.again.sv0covbind.json"
            d = emit(request, f"instrument\n{t / 'disasm.json'}\n{name}\n{IDENTITY}\n{bind2}", disasm=True)
            if not bind.is_file() or not bind2.is_file() or bind.read_bytes() != bind2.read_bytes():
                errors.append(f"{name}: the binding is missing or differs between two emissions")
            instb = t / f"{name}.inst.sv0b"
            instb.write_bytes(inst.stdout)
            bindings.append((str(bind), str(instb), str(ref)))
            text = d.stdout.decode()
            if d.returncode or text.count("COVER_HIT ") != total:
                errors.append(f"{name}: disassembly (rc {d.returncode}) shows {text.count('COVER_HIT ')} hits, want {total}")
            if name == "f0":
                if update:
                    GOLDEN.write_text(text)
                elif not GOLDEN.is_file() or GOLDEN.read_text() != text:
                    errors.append(f"f0: disassembly differs from {GOLDEN.relative_to(SV0C)} (--update to rewrite)")
            # 5. A VM without coverage support refuses it before running.
            if not rejected_checked:
                rc, out = run_sv0vm(instb)
                if rc is not None or "unknown opcode 119" not in out:
                    errors.append(f"{name}: sv0vm did not reject COVER_HIT before running (exit {rc})")
                rejected_checked = True
        # 6. The bindings validate with sv0cov itself.
        py = modern_python()
        if py is not None and bindings:
            code = ("import json, sys; sys.path.insert(0, sys.argv[1])\n"
                    "from sv0cov.formats.vmbinding import Binding, decode_v1, encode_v1\n"
                    "for b, c, m in json.loads(sys.argv[2]):\n"
                    "    data, bc, mp = open(b, 'rb').read(), open(c, 'rb').read(), json.load(open(m))\n"
                    "    got = decode_v1(data, bc)\n"
                    f"    want = Binding(mp['map_id'], mp['program_counter_count'], {IDENTITY!r})\n"
                    "    assert got == want, (b, got, want)\n"
                    "    assert encode_v1(want, bc) == data, b\n")
            p = subprocess.run([*py, "-c", code, str(ROOT / "sv0cov" / "src"), json.dumps(bindings)],
                               capture_output=True, text=True, timeout=300)
            if p.returncode:
                errors.append(f"sv0cov rejected a binding: {p.stderr.strip()[-600:]}")
            validated = True
        # A VM instrument build without a binding path is refused and writes nothing.
        f0 = next(r for n, r in progs if n == "f0")
        nomap = t / "nobinding.json"
        p = emit(f0, f"instrument\n{nomap}\nf0\n{IDENTITY}")
        if p.returncode != 9 or b"companion binding path" not in p.stderr or p.stdout or nomap.exists():
            errors.append(f"VM instrument without a binding path: rc={p.returncode} map left={nomap.exists()}")
    if errors:
        print("coverage VM emission: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    bind_note = "bindings sv0cov-validated" if validated else "binding validation SKIPPED (no Python >= 3.10)"
    print(f"coverage VM emission: OK ({len(progs)} programs: map-mode map, one COVER_HIT per counter, jumps on "
          f"boundaries, hit-stripped bytecode runs like off on sv0vm; disassembly + f0 golden; sv0vm rejects opcode 119; "
          f"{bind_note}, deterministic; no binding path refused)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
