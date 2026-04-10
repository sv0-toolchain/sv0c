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
| Bootstrap-listed programs | `lib/bootstrap-sources.list` (non-comment, non-empty lines) | ~142 |
| Self-host pilots | `lib/self-host-sv0-loop.list` | 59 |
| Stage0 C golden files | `lib/golden/stage0/*.c` | 69 |
| VM parity allowlist | `test/vm-parity/manifest.txt` | 101 |

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
| IR | `sml/ir/` | `lib/lower_unop_core`, `lib/lower_lit_core`, `lib/lower_binop_core`, `lib/lower_assign_lhs_core`, `lib/lower_ast_ty_cstring_core`, `lib/lower_value_to_expr_core` (`valueToExpr`), `lib/lower_ast_binop_to_c_core` (`astBinopToC` operator-width classes), `lib/lower_expr_init_cty_core` (`exprInitCty` shape classes), `lib/lower_match_scrut_cty_core` (`matchScrutCty` + param **`astTyToCString`** subset), `lib/lower_callee_ret_cty_core` (`calleeRetCty` program lookup + ret type), `lib/lower_enum_tag_core` (`enumTag` discriminant lookup harness), `lib/lower_struct_fields_core` (`structFields` field-count harness vs **`raise`**), `lib/lower_resolve_fn_callee_core` (`resolveFnCallee` alias hit + simple callee path vs **`raise`**), `lib/lower_resolve_enum_ctor_path_core` (`resolveEnumCtorPath` **`en::vn`** alias → two-segment target vs original pair), `lib/lower_split_qname_core` (`splitQName` on **`::`**, segment-count harness), `lib/lower_enum_store_payload_core` (`enumStorePayload` **`p{i}`** store count + **`valueToExpr`** RHS class pairing), `lib/lower_find_variant_ast_core` (`findVariantAst` enum/variant AST hit harness + unit/tuple/struct shape tags), `lib/lower_ok_payload_cty_core` (`okPayloadCty` **`Ok`** tuple arity **1** + payload **`astTyToCString`** subset), `lib/lower_scan_lets_core` (`scanLets` **`StmtLet` / `PatBind` / `SOME init`** → **`astTyToCString`** vs **`exprInitCty`**, packed harness), `lib/lower_with_scrut_locals_core` (`withScrutLocals` **`scrutLocals`** depth enter/exit surrogate), `lib/lower_mentions_result_core` (`mentionsResult` **`ExprPath(["result"])`** leaf + bounded **`expr_shape_tag`** recursive-surrogate harness), `lib/lower_ret_syntax_is_unit_core` (`retSyntaxIsUnit` **`TyUnit`** / **`TyName(["unit"])`** vs other + **`useRetSlot`** **`ensures`** ∧ **not**-unit surrogate), `lib/lower_old_slot_core` (`oldSlot` **`"_sv0old_" ^ x`**, length surrogate **8 + |x|**), `lib/lower_all_old_names_core` (`allOldNames` **`old(x)`** hit + recursive **count** surrogate via **`expr_shape_tag`**), `lib/lower_uniq_old_names_core` (`uniqOldNames` dedup with **`List.exists`**-style **`acc`** scan + length surrogate), `lib/lower_old_targets_core` (`oldTargets` **`PatBind`** filter vs **`oldMentions`**), `lib/lower_old_instrs_core` (`oldInstrs` **2** IR nodes per target: **`int`** **`DeclVar`** vs **`DeclNamed`**), `lib/lower_req_instrs_core` (`reqInstrs` **`List.concat`** over **`reqs`**: **`lowerExprWithInstrs`** **`pre`** + **`Ir.Requires`**), `lib/lower_ens_glue_inject_core` (`ensGlue` / **`lowerEnsuresClause`** pairs + **`injectEnsuresAndRetSlot`** **`Return`** expansion vs empty **`ensParts`**), `lib/lower_store_value_to_slot_core` (`storeValueToSlot` **`VVar`** same slot **0** instr vs **Store**; non-**`VVar`** **1** instr), `lib/lower_return_body_core` (`lowerReturn` **`NONE`**/**`SOME`** → **1** or **`|pre|+1`**; **`lower_body_block_tail_len`** prefix + **`Return NONE`**/**`Return SOME`** tail surrogates), `lib/lower_fn_prefix_core` (`lowerFn` **`decl`** @ **`oldInstrs`** @ **`reqInstrs`** prefix length: **`useRetSlot`** **0/1** + **`old_instrs_total`** + **`req_instrs_total`**), `lib/lower_body_dispatch_core` (`lowerBody` **`ExprBlock`** vs bare expr; **`lowerBlock`** = **`stmt`** instrs @ optional **`lowerReturn`** tail), `lib/lower_stmt_dispatch_core` (`lowerStmt` return **`NONE`**/**`SOME`** vs non-return → **`lowerExprForEffect`** length surrogate), `lib/lower_stmt_let_init_core` (`StmtLet` **`PatBind`** **`SOME init`**: **`ExprStruct`** vs enum ctor vs generic **`lowerExprWithInstrs`** + **`int`** vs other **store** path), `lib/lower_expr_wi_tv_cluster_core` (**`lowerExprWithInstrs`** / **`lowerExprToValue`** tag cluster: block, lit, path, tuple, binop, unop, quant surrogate, **ToValue** default; **ToValue** struct, enum, field, binop, unop, cast, **`wi+Assign`** tail), `lib/lower_expr_effect_iv_cluster_core` (**`lowerExprForEffect`** plus **`lowerIntoVarInstrs`**: block tail, **if**, **while** with invariant concat, **for** init, **loop**, **break**, assign, assign-op, **ToValue** default), `lib/lower_quant_core` (**`lowerQuant`** range temps + acc + **while** predicate body IR-length surrogate), `lib/lower_bind_scrut_core` (**`bindScrutVar`**: **VVar** vs literal tmp vs **`lowerExprWithInstrs`** + **int** vs other cty), `lib/lower_match_one_arm_core` (**`lowerMatchArms`** one arm: outer **`IfElse`**, optional guard inner **`IfElse`**, **`rest`** tail) | `lowering.sml` transliteration |
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
