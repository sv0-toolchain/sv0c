# Complete transliteration plan (SML bootstrap → sv0 compiler)

This document is a **roadmap** for moving the authoritative implementation from **`sv0c/sml/`** into **`.sv0`** sources under **`lib/`**, **`lexer/`**, **`parser/`**, and future compiler directories. It complements **`lib/LAYOUT.md`** (directory roles and dependency order) and **`task/sv0-toolchain-milestone-3-checklist.Rmd`** (phase table).

**Normative language rules** remain in **`sv0doc/`**. This file is **implementation planning** only.

## Definition of done (milestone 3)

1. **Feature parity:** sv0 compiler matches SML on the agreed surface (see **`task/sv0-toolchain-milestone-3-self-host.Rmd`**).
2. **VM + C backends:** Both paths support the same corpus policies (VM parity manifest + allowlist; stage0 C goldens for bootstrap seeds).
3. **Self-host:** Native **`SV0_SELF_HOST_COMPILER`** can rebuild the compiler sources; **`bootstrap-sml-final`** tag and **`sml-legacy/`** layout when SML leaves the default path.

## Inventory: major `sml/` subtrees

| Area | SML path | sv0 today | Next focus |
|------|-----------|-----------|------------|
| Span / diagnostics | `sml/error/` | `lib/span_core`, `diagnostic_*` | Extend diagnostic parity with SML messages where needed |
| Lexer | `sml/lexer/` | `lexer/token_*_core` | Full `lexer.sml` → unified token stream + errors |
| Parser | `sml/parser/` | `parser/*_core` | Merge cores into `parseExpr` / `parseStmt` / `parseItem` tracks; error recovery |
| Name resolution | `sml/name_resolution/` | `lib/env_*`, `lib/resolver_*`, `lib/lookup_*` | Full `resolver.sml`, modules, `use` |
| Types | `sml/type_checker/` | seeds: `type_alias`, lookups, **`checker_mut_assign_core`** (`let mut`, **`=`** / **`+=`** to **`x`** or **`p.f`**, **E0448** / **E0449** / **E0450**) | `checker.sml`, `unify.sml`, full inference |
| Contracts | `sml/contract_analyzer/` | C + VM lowering | Analyzer in sv0; VM already runs quantified **`requires`** (**`forall_requires`**, **`exists_requires`** on **vm-parity**); extend for analyzer + any new clause shapes |
| IR | `sml/ir/` | `lib/lower_unop_core`, `lib/lower_lit_core` | `lowering.sml` transliteration |
| Link | `sml/link/` | `lib/link_strip_core` | `stripLinkDirectives`; full project linking still SML |
| C backend | `sml/backend/c/` | — | `codegen.sml` (large) |
| VM backend | `sml/backend/vm/` | `vm_codegen.sml` (SML) | Keep encoder in SML until self-host; parity tests |

## Suggested sequencing (high level)

1. **Parser + lexer closure** — one front-end entry that matches SML acceptance on a growing corpus.
2. **Resolver + checker** — same diagnostics categories as SML (`E0xxx` / `E05xx` where defined).
3. **IR lowering** — statement/expression coverage for bootstrap list + vm-parity manifest.
4. **Backends** — C first (existing goldens), then VM (bytecode goldens).
5. **Driver** — replace `Main.main` SML orchestration with sv0 `main` calling the same phases.

## Concrete workflow per slice

1. Pick a **vertical** (e.g. “`match` on structs”) or a **horizontal** file (e.g. `parser.sml` §N).
2. Add **`path.sv0`** under **`lib/`** or **`parser/`**, **`#include`** shared stubs as today.
3. Register in **`lib/bootstrap-sources.list`** if it must VM-compile standalone; add **`lib/golden/stage0/<stem>.c`** when C output is locked.
4. Optionally add **`lib/self-host-sv0-loop.list`** when the slice links as a single TU for **`cc`** smoke.
5. Run **`./scripts/sv0 test`**; refresh **`test/vm-parity/golden/sml`** only when manifest programs change.

## Related

- **`lib/LAYOUT.md`** — transliteration dependency order (§2).
- **`doc/self-host-sv0-loop.md`** — pilot loop and **`SV0_SELF_HOST_COMPILER`**.
- **`doc/compiler-passes.md`** — phase mapping for the current SML pipeline.
