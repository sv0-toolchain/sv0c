# Complete transliteration plan (SML bootstrap → sv0 compiler)

This document is a **roadmap** for moving the authoritative implementation from **`sv0c/sml/`** into **`.sv0`** sources under **`lib/`**, **`lexer/`**, **`parser/`**, and future compiler directories. It complements **`lib/LAYOUT.md`** (directory roles and dependency order) and **`task/sv0-toolchain-milestone-3-checklist.Rmd`** (phase table).

**Normative language rules** remain in **`sv0doc/`**. This file is **implementation planning** only.

## Keeping this plan current (**`PROGRESS.md` C-4**)

This file and **`lib/LAYOUT.md`** are the **paired** transliteration map for milestone 3. **`PROGRESS.md`** checklist **C-4** is satisfied when both stay aligned with the tree as seeds land.

**Maintainer contract**

1. When you add, rename, or drop a **`.sv0`** seed under **`lib/`**, **`lexer/`**, or **`parser/`**, update **this** §2 inventory row and **`lib/LAYOUT.md`** (directory table + transliteration §2) in the **same** integration when practical so readers never chase two divergent lists.
2. **Phase / row-level** tracking lives in **`task/sv0-toolchain-milestone-3-checklist.Rmd`** — link it from decomposition notes; do not fork a second full inventory here.
3. **Regenerate the snapshot** below after material **`bootstrap-sources.list`**, **`self-host-sv0-loop.list`**, stage0 goldens, or vm-parity manifest churn (counts are advisory; CI truth is the files).

**Snapshot (replace on churn)**

| Metric | Source | Count (this revision) |
|--------|--------|-------------------------|
| Bootstrap-listed programs | `lib/bootstrap-sources.list` (non-comment, non-empty lines) | 97 |
| Self-host pilots | `lib/self-host-sv0-loop.list` | 97 |
| Stage0 C golden files | `lib/golden/stage0/*.c` | 21 |
| VM parity allowlist | `test/vm-parity/manifest.txt` | 84 |

## Definition of done (milestone 3)

1. **Feature parity:** sv0 compiler matches SML on the agreed surface (see **`task/sv0-toolchain-milestone-3-self-host.Rmd`**).
2. **VM + C backends:** Both paths support the same corpus policies (VM parity manifest + allowlist; stage0 C goldens for bootstrap seeds).
3. **Self-host:** Native **`SV0_SELF_HOST_COMPILER`** can rebuild the compiler sources; **`bootstrap-sml-final`** tag and **`sml-legacy/`** layout when SML leaves the default path.

## Inventory: major `sml/` subtrees

| Area | SML path | sv0 today | Next focus |
|------|-----------|-----------|------------|
| Span / diagnostics | `sml/error/` | `lib/span.sv0` (flattened Pos/Span), `lib/diagnostic.sv0` (severity, getLine, spaces/carets/padLeft, hasErrors, errorCount, **Vec-based diag list**, **sort-by-span**, **format_header**, **format_snippet**) | `format` with full Span/related/help, `report`/`reportAll` I/O |
| Lexer | `sml/lexer/` | `lib/lexer.sv0` (**full tokenize pipeline**: char class, keywords, escape, scan string/char/number/op, `tokenize` → parallel Vec output), `lexer/token.sv0` (98-variant enum) | **Pipeline parity reached** — remaining: integrate with parser token stream consumption |
| Parser | `sml/parser/` | `lib/parser.sv0` (binop/prec maps, token helpers, skip functions, **token stream nav** (peek/advance/expect/match), **parsePath**, **skip helpers on token stream** (pub/unsafe/generics/where/attrs), 28 keyword constants — **tags fixed** to match token.sv0) | **Full recursive descent** (`parseExpr`/`parseType`/`parsePat`/`parseItem`) — largest deferred piece |
| Name resolution | `sml/name_resolution/` | `lib/env.sv0` (**SML parity**: scope mgmt, mod val/ty, fnArity, tyAlias, registerValueAlias, registerTypeAlias, lookupType), `lib/resolver.sv0` (intrinsics, path join, helpers) | **Full resolver** (`resolveTy`/`resolveExpr`/`resolveStmt`, `applyUseClause`) |
| Types | `sml/type_checker/types.sml` | `lib/types.sv0` (20-variant Ty enum, Box, Vec, ty_tag, fresh_var) | Mostly complete; string payloads pending |
| Unification | `sml/type_checker/unify.sml` | `lib/unify.sv0` (full structural unify over Ty) | **Parity reached** per header |
| Type checker | `sml/type_checker/checker.sml` | `lib/checker.sv0` (binop class, type predicates, variant/ctor helpers, error codes, env helpers) | **Full inference** (`synth`/`checkStmt`/`checkFn`/`initTypes`) — second largest deferred piece |
| Contracts | `sml/contract_analyzer/` | `lib/contract_analyzer.sv0` (identity pass-through) | **Parity reached** (analyzer is trivial in SML too) |
| Pipeline / driver | `sml/main.sml` | `lib/main.sv0` (paths, phase constants, CLI helpers) | `readFile`/`compileFile`/`compileProjectDir` — I/O dependent |
| IR + lowering | `sml/ir/` | `lib/ir.sv0` (Value 8v/Expr 6v/Instr 17v enums, Box/Vec recursive), `lib/lowering.sv0` (QName split, C strings, slot/enum/struct helpers, literal class) | **Full `lowerExpr`/`lowerStmt`/`lowerFn`/`lower`** — IR lists from AST |
| Link | `sml/link/` | `lib/link.sv0` (is_sv0, mangling, strip, rewrite predicates) | **`mapTy`/`mapExpr`/`mapItem`**, **`walkSv0`/`parseFile`/`linkProjectDir`** |
| C backend | `sml/backend/c/` | `lib/codegen.sv0` (**full** `emitValue`/`emitExpr`/`emitInstr`/`emitFn`/top `emit`) | **Parity reached** per header |
| VM backend | `sml/backend/vm/` | `lib/vm_codegen.sv0` (layout, opcodes, pool, builtins map), `lib/bytecode.sv0` (50 opcodes, sizes, LE helpers, file layout) | **`emitValue`/`emitExpr`/`emitInstr`/`emitBlock`** + byte-vector encode/decode |
| Include expansion | `sml/include_expand.sml` | `lib/include_expand.sv0` (trim, parse_include_line, path_ok, **split_lines**, **cycle detection**, **path_dir/path_join**) | **`expand`/`expandFile`** — file I/O dependent (all non-I/O building blocks done) |

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
- **`doc/tooling-formatter-linter.md`** — **`sv0-formatter`** / **`sv0-linter`** design sketch (tooling; not spec).
