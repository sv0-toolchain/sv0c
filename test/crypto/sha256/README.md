# SHA-256 known-answer tests (CV-103, red until CV-104)

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
  on native and VM. Today it reports **RED** because `lib/sha256.sv0` does not
  exist. CV-104 adds the implementation and wires this runner into
  `./scripts/sv0 test`.

```bash
python3 sv0c/test/crypto/sha256/run_kat.py
```
