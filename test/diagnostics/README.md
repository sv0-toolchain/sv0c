# Diagnostics corpus (M3-S-044)

**Purpose:** curated **reject** sources plus stable **substring** (E-code) expectations,
run against **both** compilers — the SML bootstrap and the native default — so a
regression in either backend's diagnostics breaks CI.

## Layout

- **`manifest.txt`** — one entry per line:

  ```
  path_relative_to_sv0c/ | needle [ | backend ]
  ```

  The needle (an `E0xxx` code) must appear in the compiler's combined stdout+stderr.
  `backend` is optional and one of:
  - **`both`** (default) — the code must be emitted by **both** backends.
  - **`native`** — gated only on the native compiler (SML diverges here).
  - **`sml`** — gated only on the SML reference (native diverges here).
- **`cases/*.sv0`** — minimal programs that **fail** to compile, each with a header
  comment naming the code and backend tag.

## Validation

- `./scripts/sv0 test-guards` runs `verify_diagnostics_corpus_layout.py` (README +
  manifest + case files exist).
- `./scripts/sv0 test` runs both behavioral verifiers:
  `verify_diagnostics_corpus_behavior.py` (SML) and
  `verify_diagnostics_corpus_behavior_native.py` (native). Each skips rows tagged
  for the other backend; the native verifier also requires a non-zero exit.

Grow the manifest when adding new stable failure modes. Probe both backends first
and set the `backend` tag to what actually holds (a shared-manifest `both` row must
pass on both, or CI fails). Keep needles tied to documented codes in
`sv0doc/compiler/bootstrap-diagnostics.md` where applicable.

## known under-diagnosis gaps (neither/one backend catches — future work)

Probing while building this corpus surfaced constructs that compile with **no**
diagnostic (or only one backend diagnoses). These are not gated here (a test that
asserts an un-emitted code would fail); they are candidate fixes for a future
diagnostics pass:

| construct | current behavior | wanted |
|---|---|---|
| duplicate top-level `fn f` / `fn f` | both accept | E0302 duplicate definition |
| duplicate struct field `struct S { x, x }` | both accept | E0405 duplicate field |
| `1 + true` (binop operand type mismatch) | both accept | E0400 type mismatch |
| `if c { 1 } else { false }` (branch type mismatch) | both accept | E0400 |
| `let x = 1; x = 2;` (assign to immutable) | both accept | E0426/E0448 |
| extra field in struct literal `P { x, z }` | both accept | E0429 |
| `p.nope()` (unknown method) | both accept (defers to `cc`) | E0401 |
| `println(42)` (non-string arg) | SML: E0444; native: accepts | native E0444 |
