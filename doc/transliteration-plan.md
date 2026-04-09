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
| IR | `sml/ir/` | `lib/lower_unop_core`, `lib/lower_lit_core`, `lib/lower_binop_core`, `lib/lower_assign_lhs_core`, `lib/lower_ast_ty_cstring_core`, `lib/lower_value_to_expr_core` (`valueToExpr`), `lib/lower_ast_binop_to_c_core` (`astBinopToC` operator-width classes), `lib/lower_expr_init_cty_core` (`exprInitCty` shape classes), `lib/lower_match_scrut_cty_core` (`matchScrutCty` + param **`astTyToCString`** subset), `lib/lower_callee_ret_cty_core` (`calleeRetCty` program lookup + ret type), `lib/lower_enum_tag_core` (`enumTag` discriminant lookup harness), `lib/lower_struct_fields_core` (`structFields` field-count harness vs **`raise`**), `lib/lower_resolve_fn_callee_core` (`resolveFnCallee` alias hit + simple callee path vs **`raise`**), `lib/lower_resolve_enum_ctor_path_core` (`resolveEnumCtorPath` **`en::vn`** alias → two-segment target vs original pair), `lib/lower_split_qname_core` (`splitQName` on **`::`**, segment-count harness), `lib/lower_enum_store_payload_core` (`enumStorePayload` **`p{i}`** store count + **`valueToExpr`** RHS class pairing), `lib/lower_find_variant_ast_core` (`findVariantAst` enum/variant AST hit harness + unit/tuple/struct shape tags), `lib/lower_ok_payload_cty_core` (`okPayloadCty` **`Ok`** tuple arity **1** + payload **`astTyToCString`** subset), `lib/lower_scan_lets_core` (`scanLets` **`StmtLet` / `PatBind` / `SOME init`** → **`astTyToCString`** vs **`exprInitCty`**, packed harness), `lib/lower_with_scrut_locals_core` (`withScrutLocals` **`scrutLocals`** depth enter/exit surrogate), `lib/lower_mentions_result_core` (`mentionsResult` **`ExprPath(["result"])`** leaf + bounded **`expr_shape_tag`** recursive-surrogate harness), `lib/lower_ret_syntax_is_unit_core` (`retSyntaxIsUnit` **`TyUnit`** / **`TyName(["unit"])`** vs other + **`useRetSlot`** **`ensures`** ∧ **not**-unit surrogate), `lib/lower_old_slot_core` (`oldSlot` **`"_sv0old_" ^ x`**, length surrogate **8 + |x|**) | `lowering.sml` transliteration |
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
