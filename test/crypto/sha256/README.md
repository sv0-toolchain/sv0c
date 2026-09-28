# SHA-256 known-answer tests (CV-103, CV-104)

The sv0 SHA-256 implementation the compiler will use for coverage point and
map IDs (sv0cov SPEC COV-MAP-004, decision D-2 = SHA-256 in sv0 source,
pulling M5-S-040a/b forward).

- `vectors.tsv`: the four NIST SHA-256 examples (empty, `abc`, the 448-bit
  two-block message, one million `a`) plus padding-boundary lengths
  (55, 56, 63, 64, 119, 120 bytes of `a`), with provenance.
- `kat_main.sv0`: checks each vector through `sha256_hex(s: str) -> str`
  (lowercase hex of the bytes of `s`); exit status is the 1-based index of the
  first failing vector.
- `run_kat.py`: verifies the table against Python `hashlib`, prepends
  `sv0c/lib/sha256.sv0` to `kat_main.sv0` (as the mega-TU does), and runs it
  on native and VM. `--differential` adds 200 seeded pseudo-random UTF-8
  messages (0..300 characters) checked against `hashlib`. The VM skips the
  million-`a` vector unless `--vm-long` is given (the SML interpreter needs
  many minutes for its 15,626 blocks); native always runs it. Every run
  also checks `sha256_hex_bytes` (a `Vec<i32>` of bytes, used for binary
  point-identity preimages) on 17 lengths around the block and padding
  boundaries, with NUL and 0xff bytes, against `hashlib`.
- `./scripts/sv0 test` runs `run_kat.py --differential`; `test-guards`
  checks that `lib/sha256.sv0` matches `sv0c/scripts/gen_sha256_sv0.py`.

The implementation (`lib/sha256.sv0`, CV-104) is generated: 32-bit words are
pairs of 16-bit halves in i32 locals, so it never relies on signed overflow
(undefined in the generated C). `sha256_hex` hashes a `str`;
`sha256_hex_bytes` hashes a `Vec<i32>` of byte values. It is in the mega-TU
module list (CV-107); the coverage map writer (CV-110) uses both.

```bash
python3 sv0c/test/crypto/sha256/run_kat.py
```
