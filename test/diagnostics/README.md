# Diagnostics corpus (M3-S-044)

**Purpose:** curated **reject** sources plus stable **substring** expectations against the **SML** bootstrap compiler’s combined output (same categories/codes as production diagnostics).

## Layout

- **`manifest.txt`** — one entry per line: **`path_relative_to_sv0c/ | needle`** (needle must appear in stdout or stderr from **`Main.main`** compiling that file).
- **`cases/*.sv0`** — minimal programs that **fail** compile (type/parse/etc.).

## Validation

- **`./scripts/sv0 test-guards`** runs **`scripts/verify_diagnostics_corpus_layout.py`** (README + manifest + case files).
- **`./scripts/sv0 test`** runs **`scripts/verify_diagnostics_corpus_behavior.py`** (invokes SML **`sources.cm`** + **`Main.main`** per manifest row).

Grow the manifest when adding new stable **E0xxx** / parser failure modes; keep needles tied to documented codes in **`sv0doc/compiler/bootstrap-diagnostics.md`** where applicable.
