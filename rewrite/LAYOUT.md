# Self-hosting compiler layout (milestone 3, phase 1)

All **compiler-in-sv0** sources live in the **sv0c** repo. This document is the working plan for directories and **transliteration order** (bootstrap SML → sv0). It is not normative for the language (see **sv0doc**).

## Directories (current + planned)

| Path | Role |
|------|------|
| `sv0c/src/**/*.sml` | **Stage-0** bootstrap compiler (SML/NJ); reference for transliteration |
| `sv0c/rewrite/` | **Stage-1+** sv0 sources: seeds and, over time, full pipeline modules |
| `sv0c/rewrite/rewrite-sources.list` | Ordered list of **standalone** `.sv0` programs CI runs via `rewrite-build` (each has `main`, VM exit 0) |
| `sv0c/rewrite/golden/stage0/<stem>.c` | **Checked-in** bootstrap C for `rewrite/<stem>.sv0`; `sv0 test` / `ci` diffs fresh SML heap output against these when present |
| `build/self-host-compare/` | Generated C snapshots for **stage0 vs stage1** comparison (gitignored at meta-repo root; see `./scripts/sv0 self-host-capture-stage0`) |

Future splits (rename when the first real module lands):

- `rewrite/lib/` — shared types and utilities (span, diagnostics, token enums)
- `rewrite/lexer/`, `rewrite/parser/`, … — pipeline slices matching `src/` layout

Until the language supports multi-file **library** compilation for the compiler itself, keep **one program per `rewrite-build` entry** (whole pipeline in one file is temporary, or use list entries as isolated test slices).

## Transliteration dependency order (SML → sv0)

Rough bottom-up order (mirrors `src/` data flow):

1. **Span / positions** — `src/error/span.sml` → `rewrite/span_core.sv0` (seed)
2. **Diagnostics** — `src/error/diagnostic.*`
3. **Lexer** — `src/lexer/*`
4. **AST** — `src/ast/*` (may require sv0 ADT features)
5. **Parser** — `src/parser/*`
6. **Name resolution** — `src/name_resolution/*`
7. **Type checker** — `src/type_checker/*`
8. **Contracts** — `src/contract_analyzer/*`
9. **IR** — `src/ir/*`
10. **Backends** — `src/backend/c/*`, `src/backend/vm/*`

Adjust when a layer needs a feature not yet in sv0; either extend the language or keep that slice in SML longer.

## Stage equality (CI goal)

- **Stage 0**: SML `sv0c` emits C (or bytecode) for a fixed corpus.
- **Stage 1**: sv0-built compiler emits the same for the same inputs.

**Regression (CI today):** for each `rewrite-sources.list` entry, if `rewrite/golden/stage0/<stem>.c` exists, `./scripts/sv0 test` emits C with the SML heap compiler and **`diff`**s against that file. Refresh the golden after intentional C-backend changes (same command you used to generate it, or copy from `self-host-capture-stage0` and review).

**Stage0 vs stage1 (later):**

1. `./scripts/sv0 self-host-capture-stage0 <path-relative-to-sv0c>` writes `build/self-host-compare/stage0/<stem>.c` using the SML heap compiler.
2. When a stage-1 binary exists, generate `build/self-host-compare/stage1/<stem>.c` the same way.
3. `./scripts/sv0 self-host-compare <stem>` runs `diff` (or a normalized compare ignoring timestamps).

Golden corpus can start with `rewrite/span_core.sv0` and grow with integration fixtures under `test/`.

## Commands (toolchain root)

| Command | Purpose |
|---------|---------|
| `./scripts/sv0 rewrite-build` | VM-compile and run every entry in `rewrite-sources.list` (fallback: `rewrite/*.sv0`) |
| `./scripts/sv0 emit-c <rel>` | Print C for one `.sv0` (requires `make heap` in sv0c) |
| `./scripts/sv0 self-host-capture-stage0 <rel>` | Save stage-0 C to `build/self-host-compare/stage0/<stem>.c` |
| `./scripts/sv0 self-host-compare <stem>` | Diff stage0 vs stage1 C when both exist |
| `./scripts/sv0 self-host-check-golden` | Same golden diff as `sv0 test` (standalone) |
