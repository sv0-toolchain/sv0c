# Self-hosting compiler layout (milestone 3, phase 1)

All **compiler-in-sv0** sources live in the **sv0c** repo. This document is the working plan for directories and **transliteration order** (bootstrap SML → sv0). It is not normative for the language (see **sv0doc**).

## Directories (current + planned)

| Path | Role |
|------|------|
| `sv0c/sml/**/*.sml` | **Stage-0** bootstrap compiler (SML/NJ); reference for transliteration |
| `sv0c/lib/` | **Stage-1+** sv0 sources: shared/compiler-core slices (`span_core.sv0`, …) |
| `sv0c/lib/bootstrap-sources.list` | Standalone `.sv0` programs CI runs via **`bootstrap-build`** (each has `main`, VM exit 0); paths relative to `sv0c/` |
| `sv0c/lib/golden/stage0/<stem>.c` | Checked-in bootstrap C for listed programs; `sv0 test` diffs fresh SML heap output |
| `sv0c/lexer/`, `sv0c/parser/`, … | Future **sv0** pipeline modules (same names as under `sml/`, but `.sv0`) |
| `build/self-host-compare/` | Generated C snapshots for stage0 vs stage1 (gitignored at meta-repo root) |

**`sv0c --project <dir>`** collects **all `*.sv0` recursively** under `dir` (skips hidden path components). Subdirectories are supported (see `test/integration/modules/`).

## Transliteration dependency order (SML → sv0)

Rough bottom-up order (mirrors `sml/` data flow):

1. **Span / positions** — `sml/error/span.sml` → `lib/span_core.sv0` (seed)
2. **Diagnostics** — `sml/error/diagnostic.*` → `lib/diagnostic_core.sv0` (format length model); `lib/diagnostic_batch_core.sv0` (`hasErrors` / `errorCount` on three tag slots).
3. **Lexer** — `sml/lexer/*` → `lexer/*.sv0` (seed: `lexer/token_keyword_core.sv0`).
4. **Env (name resolution)** — `sml/name_resolution/env.*` → `lib/env_core.sv0` (bounded module-value register + lookup).
5. **AST** — `sml/ast/*`
6. **Parser** — `sml/parser/*`
7. **Name resolution** — `sml/name_resolution/*` (full resolver; builds on **§4**)
8. **Type checker** — `sml/type_checker/*`
9. **Contracts** — `sml/contract_analyzer/*`
10. **IR** — `sml/ir/*`
11. **Backends** — `sml/backend/c/*`, `sml/backend/vm/*`

Adjust when a layer needs a feature not yet in sv0; either extend the language or keep that slice in SML longer.

## Stage equality (CI goal)

- **Stage 0**: SML `sv0c` emits C (or bytecode) for a fixed corpus.
- **Stage 1**: sv0-built compiler emits the same for the same inputs.

**Regression (CI today):** for each `lib/bootstrap-sources.list` entry, if `lib/golden/stage0/<stem>.c` exists, `./scripts/sv0 test` emits C with the SML heap compiler and **`diff`**s against that file.

**Stage0 vs stage1 (later):** use `./scripts/sv0 self-host-capture-stage0`, `self-host-compare` as before.

Golden corpus starts with `lib/span_core.sv0` and grows with integration fixtures under `test/`.

## Commands (toolchain root)

| Command | Purpose |
|---------|---------|
| `./scripts/sv0 bootstrap-build` | VM-compile and run every entry in `lib/bootstrap-sources.list` (fallback: top-level `lib/*.sv0`) |
| `./scripts/sv0 emit-c <rel>` | Print C for one `.sv0` |
| `./scripts/sv0 self-host-capture-stage0 <rel>` | Save stage-0 C snapshot |
| `./scripts/sv0 self-host-compare <stem>` | Diff stage0 vs stage1 C |
| `./scripts/sv0 self-host-check-golden` | Same golden diff as `sv0 test` |
