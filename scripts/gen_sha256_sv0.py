#!/usr/bin/env python3
"""Generate sv0c/lib/sha256.sv0: SHA-256 in sv0 for the compiler (CV-104).

sv0cov decision D-2: coverage point and map IDs are SHA-256 digests computed by
the compiler itself, in sv0 source, so both the native and the VM-built
compiler can emit maps.

Representation. Every 32-bit word is held as two i32 halves in 0..65535
(``xh`` high, ``xl`` low). Additions sum halves and propagate one carry, so no
intermediate exceeds 2^19: nothing relies on signed-overflow wrap, which is
undefined in the generated C (plain ``int``, no ``-fwrapv``). Rotations and
shifts mask before shifting left. The message schedule is a rolling window of
16 words in locals and the 64 rounds are unrolled with the round constants as literals, so hashing
allocates no Vec, slice, or string-table handle; only the 64-character hex
result is built with ``string_concat``.

``sha256_hex(s)`` hashes the bytes of an sv0 ``str`` (``string_char_at``
yields 0..255). ``sha256_hex_bytes(v)`` hashes a ``Vec<i32>`` of byte values
0..255, for binary preimages that may hold any byte (sv0cov point identities
carry raw digests, little-endian integers and NUL bytes).

The file has no ``module`` line: compiler library files are concatenated into
one translation unit (mega-TU), so every name is prefixed ``sha256_``.

    python3 sv0c/scripts/gen_sha256_sv0.py          # check (exit 1 if stale)
    python3 sv0c/scripts/gen_sha256_sv0.py --write
"""

from __future__ import annotations

import sys
from pathlib import Path

OUT = Path(__file__).resolve().parents[1] / "lib" / "sha256.sv0"

K = [
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
]
H0 = [0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19]
LETTERS = "abcdefgh"


def _check_constants() -> None:
    """K and H0 are the fractional parts of cube and square roots of the first primes."""
    primes = [p for p in range(2, 312) if all(p % q for q in range(2, int(p**0.5) + 1))][:64]

    def frac_root(p: int, k: int) -> int:
        lo, hi = 0, 1 << 40  # integer k-th root of p * 2^(32k), then keep 32 fractional bits
        target = p << (32 * k)
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if mid**k <= target:
                lo = mid
            else:
                hi = mid - 1
        return lo & 0xFFFFFFFF

    assert K == [frac_root(p, 3) for p in primes], "K constants"
    assert H0 == [frac_root(p, 2) for p in primes[:8]], "H0 constants"


def rounds(w) -> None:
    """The 64 rounds of one block, unrolled with K[t] as literals (CV-428).

    The -O0 compiler paid a constant lookup per half per round and a 30-move
    register and schedule shift. Here round t names state letter i by the
    local of letter (i - t) mod 8, and W[t] lives in local t mod 16, written in
    place for t >= 16, so no round moves a value; 64 rounds rotate the names
    back to where they started.
    """
    for v in ("t1lsum", "t1l", "t1h", "t2lsum", "t2l", "t2h", "elsum", "alsum", "nlsum"):
        w(f"    let mut {v} = 0;")
    for t in range(64):
        a, b, c, d, e, f, g, h = (LETTERS[(i - t) % 8] for i in range(8))
        s = t % 16
        w(f"    /* round {t} */")
        if t >= 16:
            # W[t] = s1(W[t-2]) + W[t-7] + s0(W[t-15]) + W[t-16]; the old slot value is W[t-16]
            x, y, z = (t - 2) % 16, (t - 7) % 16, (t - 15) % 16
            w(f"    nlsum = sha256_small1_lo(w{x}h, w{x}l) + w{y}l + sha256_small0_lo(w{z}h, w{z}l) + w{s}l;")
            w(f"    w{s}h = (sha256_small1_hi(w{x}h, w{x}l) + w{y}h + sha256_small0_hi(w{z}h, w{z}l) + w{s}h + nlsum / 65536) % 65536;")
            w(f"    w{s}l = nlsum % 65536;")
        k = K[t]
        # T1 = h + S1(e) + ch(e,f,g) + K[t] + W[t]; T2 = S0(a) + maj(a,b,c)
        w(f"    t1lsum = {h}l + sha256_big1_lo({e}h, {e}l) + sha256_ch({e}l, {f}l, {g}l) + {k & 0xFFFF} + w{s}l;")
        w(f"    t1h = ({h}h + sha256_big1_hi({e}h, {e}l) + sha256_ch({e}h, {f}h, {g}h) + {k >> 16} + w{s}h + t1lsum / 65536) % 65536;")
        w("    t1l = t1lsum % 65536;")
        w(f"    t2lsum = sha256_big0_lo({a}h, {a}l) + sha256_maj({a}l, {b}l, {c}l);")
        w(f"    t2h = (sha256_big0_hi({a}h, {a}l) + sha256_maj({a}h, {b}h, {c}h) + t2lsum / 65536) % 65536;")
        w("    t2l = t2lsum % 65536;")
        # e' = d + T1 lands in d's local; a' = T1 + T2 lands in h's local
        w(f"    elsum = {d}l + t1l;")
        w(f"    {d}h = ({d}h + t1h + elsum / 65536) % 65536;")
        w(f"    {d}l = elsum % 65536;")
        w("    alsum = t1l + t2l;")
        w(f"    {h}h = (t1h + t2h + alsum / 65536) % 65536;")
        w(f"    {h}l = alsum % 65536;")


def generate() -> str:
    _check_constants()
    out: list[str] = []
    w = out.append
    w("/* sv0c SHA-256 (FIPS 180-4), generated by sv0c/scripts/gen_sha256_sv0.py; do not edit.")
    w(" * sha256_hex(s) = lowercase hex SHA-256 of the bytes of s (sv0cov D-2, CV-104).")
    w(" * 32-bit words are pairs of 16-bit halves in i32 locals; see the generator's")
    w(" * docstring for why (no reliance on signed overflow, no Vec allocation).")
    w(" */")
    w("")
    # Rotations and shifts on (h, l) halves, expanded at generation time into
    # shift-and-mask expressions with literal amounts (CV-428): the compiler
    # is built at -O0, so the earlier rotr/mix helper calls and their 2^n
    # loop dominated map building. Operands are 0..65535 halves and every
    # amount is 0 < n < 16 after the half swap, so nothing reaches the sign bit.
    w("fn sha256_pow2(n: i32) -> i32 {")
    w("  return 1 << n;")
    w("}")
    w("")

    def mix(x: str, y: str, n: int) -> str:
        """(x >> n) with the low n bits of y on top, 0 < n < 16."""
        return f"(({x} >> {n}) | (({y} & {(1 << n) - 1}) << {16 - n}))"

    def rotr(part: str, n: int) -> str:
        h, l = ("h", "l") if n < 16 else ("l", "h")
        n %= 16
        hi, lo = (h, l) if part == "hi" else (l, h)
        return hi if n == 0 else mix(hi, lo, n)

    def shr(part: str, n: int) -> str:
        return f"(h >> {n})" if part == "hi" else mix("l", "h", n)

    # sigma functions
    for name, a, b, c, is_shr in (("big0", 2, 13, 22, False), ("big1", 6, 11, 25, False), ("small0", 7, 18, 3, True), ("small1", 17, 19, 10, True)):
        for part in ("hi", "lo"):
            third = shr(part, c) if is_shr else rotr(part, c)
            w(f"fn sha256_{name}_{part}(h: i32, l: i32) -> i32 {{")
            w(f"  return {rotr(part, a)} ^ {rotr(part, b)} ^ {third};")
            w("}")
            w("")
    w("fn sha256_ch(e: i32, f: i32, g: i32) -> i32 {")
    w("  return (e & f) ^ ((e ^ 65535) & g);")
    w("}")
    w("")
    w("fn sha256_maj(a: i32, b: i32, c: i32) -> i32 {")
    w("  return (a & b) ^ (a & c) ^ (b & c);")
    w("}")
    w("")
    # padded message bytes
    w("/* Byte i of the padded message: data, 0x80, zeros, then the 64-bit big-endian bit length. */")
    w("fn sha256_byte(s: str, n: i32, total: i32, i: i32) -> i32 {")
    w("  if i < n {")
    w("    return string_char_at(s, i);")
    w("  };")
    w("  if i == n {")
    w("    return 128;")
    w("  };")
    w("  let k = total - 1 - i;")
    w("  if k >= 8 {")
    w("    return 0;")
    w("  };")
    w("  if k == 0 {")
    w("    return (n % 32) * 8;")
    w("  };")
    w("  if k >= 5 {")
    w("    return 0;")
    w("  };")
    w("  return (n / sha256_pow2(8 * k - 3)) % 256;")
    w("}")
    w("")
    w("fn sha256_word_hi(s: str, n: i32, total: i32, i: i32) -> i32 {")
    w("  return sha256_byte(s, n, total, i) * 256 + sha256_byte(s, n, total, i + 1);")
    w("}")
    w("")
    w("fn sha256_word_lo(s: str, n: i32, total: i32, i: i32) -> i32 {")
    w("  return sha256_byte(s, n, total, i + 2) * 256 + sha256_byte(s, n, total, i + 3);")
    w("}")
    w("")
    w("fn sha256_hex4(x: i32) -> str {")
    w('  let digits = "0123456789abcdef";')
    w("  let mut out = \"\";")
    w("  let mut shift = 4096;")
    w("  while shift > 0 {")
    w("    out = string_concat(out, string_substr(digits, (x / shift) % 16, 1));")
    w("    shift = shift / 16;")
    w("  };")
    w("  return out;")
    w("}")
    w("")
    # main function
    w("fn sha256_hex(s: str) -> str {")
    w("  let n = string_len(s);")
    w("  let total = ((n + 8) / 64 + 1) * 64;")
    for i, x in enumerate(H0):
        w(f"  let mut h{i}h = {x >> 16};")
        w(f"  let mut h{i}l = {x & 0xFFFF};")
    w("  let mut off = 0;")
    w("  while off < total {")
    for j in range(16):
        w(f"    let mut w{j}h = sha256_word_hi(s, n, total, off + {4 * j});")
        w(f"    let mut w{j}l = sha256_word_lo(s, n, total, off + {4 * j});")
    for i, c in enumerate(LETTERS):
        w(f"    let mut {c}h = h{i}h;")
        w(f"    let mut {c}l = h{i}l;")
    rounds(w)
    for i, c in enumerate(LETTERS):
        w(f"    let s{i} = h{i}l + {c}l;")
        w(f"    h{i}l = s{i} % 65536;")
        w(f"    h{i}h = (h{i}h + {c}h + s{i} / 65536) % 65536;")
    w("    off = off + 64;")
    w("  };")
    w('  let mut out = "";')
    for i in range(8):
        w(f"  out = string_concat(out, sha256_hex4(h{i}h));")
        w(f"  out = string_concat(out, sha256_hex4(h{i}l));")
    w("  return out;")
    w("}")
    # the same padded-message readers over a Vec of byte values (0..255)
    w("/* Byte i of the padded message: data, 0x80, zeros, then the 64-bit big-endian bit length. */")
    w("fn sha256_vbyte(s: Vec<i32>, n: i32, total: i32, i: i32) -> i32 {")
    w("  if i < n {")
    w("    return vec_get(s, i);")
    w("  };")
    w("  if i == n {")
    w("    return 128;")
    w("  };")
    w("  let k = total - 1 - i;")
    w("  if k >= 8 {")
    w("    return 0;")
    w("  };")
    w("  if k == 0 {")
    w("    return (n % 32) * 8;")
    w("  };")
    w("  if k >= 5 {")
    w("    return 0;")
    w("  };")
    w("  return (n / sha256_pow2(8 * k - 3)) % 256;")
    w("}")
    w("")
    w("fn sha256_vword_hi(s: Vec<i32>, n: i32, total: i32, i: i32) -> i32 {")
    w("  return sha256_vbyte(s, n, total, i) * 256 + sha256_vbyte(s, n, total, i + 1);")
    w("}")
    w("")
    w("fn sha256_vword_lo(s: Vec<i32>, n: i32, total: i32, i: i32) -> i32 {")
    w("  return sha256_vbyte(s, n, total, i + 2) * 256 + sha256_vbyte(s, n, total, i + 3);")
    w("}")
    w("")
    # main function over a Vec of byte values (binary preimages such as
    # sv0cov point identities, which may hold any byte)
    w("fn sha256_hex_bytes(s: Vec<i32>) -> str {")
    w("  let n = vec_len(s);")
    w("  let total = ((n + 8) / 64 + 1) * 64;")
    for i, x in enumerate(H0):
        w(f"  let mut h{i}h = {x >> 16};")
        w(f"  let mut h{i}l = {x & 0xFFFF};")
    w("  let mut off = 0;")
    w("  while off < total {")
    for j in range(16):
        w(f"    let mut w{j}h = sha256_vword_hi(s, n, total, off + {4 * j});")
        w(f"    let mut w{j}l = sha256_vword_lo(s, n, total, off + {4 * j});")
    for i, c in enumerate(LETTERS):
        w(f"    let mut {c}h = h{i}h;")
        w(f"    let mut {c}l = h{i}l;")
    rounds(w)
    for i, c in enumerate(LETTERS):
        w(f"    let s{i} = h{i}l + {c}l;")
        w(f"    h{i}l = s{i} % 65536;")
        w(f"    h{i}h = (h{i}h + {c}h + s{i} / 65536) % 65536;")
    w("    off = off + 64;")
    w("  };")
    w('  let mut out = "";')
    for i in range(8):
        w(f"  out = string_concat(out, sha256_hex4(h{i}h));")
        w(f"  out = string_concat(out, sha256_hex4(h{i}l));")
    w("  return out;")
    w("}")
    return "\n".join(out) + "\n"


def main() -> int:
    text = generate()
    if "--write" in sys.argv:
        OUT.write_text(text)
        print(f"gen_sha256_sv0: wrote {OUT}")
        return 0
    if not OUT.is_file() or OUT.read_text() != text:
        print(f"gen_sha256_sv0: {OUT} is stale; run with --write", file=sys.stderr)
        return 1
    print("gen_sha256_sv0: current")
    return 0


if __name__ == "__main__":
    sys.exit(main())
