# Self-hosting compiler layout (milestone 3, phase 1)

All **compiler-in-sv0** sources live in the **sv0c** repo. This document is the working plan for directories and **transliteration order** (bootstrap SML → sv0). It is not normative for the language (see **sv0doc**). For a **milestone-3-wide** inventory and sequencing narrative, see **`doc/transliteration-plan.md`**.

## Directories (current + planned)

| Path | Role |
|------|------|
| `sv0c/sml/**/*.sml` | **Stage-0** bootstrap compiler (SML/NJ); reference for transliteration |
| `sv0c/lib/` | **Stage-1+** sv0 sources: shared/compiler-core slices (`span_core.sv0`, …) |
| `sv0c/lib/bootstrap-sources.list` | Standalone `.sv0` programs CI runs via **`bootstrap-build`** (each has `main`, VM exit 0); paths relative to `sv0c/` |
| `sv0c/lib/self-host-sv0-loop.list` | Pilot sources for **`./scripts/sv0 self-host-sv0-loop`** (SML→C→native smoke + optional **`SV0_SELF_HOST_COMPILER`**); see **`doc/self-host-sv0-loop.md`** |
| `sv0c/lib/golden/stage0/<stem>.c` | Checked-in bootstrap C for listed programs; `sv0 test` diffs fresh SML heap output |
| `sv0c/lexer/`, `sv0c/parser/`, `sv0c/lib/` (IR / link slices), … | **sv0** pipeline modules (same names as under `sml/`); parser seeds include `expr_entry_core`, …, `expr_cast_core`, `expr_unary_stub_core`, `type_parse_core`, `try_assign_stmt_core`; IR seeds: `lower_unop_core`, `lower_lit_core`, `lower_binop_core`, `lower_assign_lhs_core`, `lower_ast_ty_cstring_core`, `lower_value_to_expr_core`, `lower_ast_binop_to_c_core`, `lower_expr_init_cty_core`, `lower_match_scrut_cty_core`, `lower_callee_ret_cty_core`, `lower_enum_tag_core`, `lower_struct_fields_core`, `lower_resolve_fn_callee_core`, `lower_resolve_enum_ctor_path_core`, `lower_split_qname_core`, `lower_enum_store_payload_core`, `lower_find_variant_ast_core`, `lower_ok_payload_cty_core`, `lower_scan_lets_core`, `lower_with_scrut_locals_core`, `lower_mentions_result_core`, `lower_ret_syntax_is_unit_core`, `lower_old_slot_core`, `lower_all_old_names_core`, `lower_uniq_old_names_core`, `lower_old_targets_core`; link seed: `link_strip_core` (see **§6**) |
| `build/self-host-compare/` | Generated C snapshots for stage0 vs stage1 (gitignored at meta-repo root) |
| `test/vm-parity/` | Milestone **3** VM bytecode parity: **`manifest.txt`**, **`golden/sml/*.sv0b`** (SML reference); see **`test/vm-parity/README.md`** |

**`sv0c --project <dir>`** collects **all `*.sv0` recursively** under `dir` (skips hidden path components). Subdirectories are supported (see `test/integration/modules/`).

## Transliteration dependency order (SML → sv0)

Rough bottom-up order (mirrors `sml/` data flow):

1. **Span / positions** — `sml/error/span.sml` → `lib/span_core.sv0` (seed)
2. **Diagnostics** — `sml/error/diagnostic.*` → `lib/diagnostic_core.sv0` (format length model); `lib/diagnostic_batch_core.sv0` (`hasErrors` / `errorCount` on three tag slots).
3. **Lexer** — `sml/lexer/*` → `lexer/*.sv0` (seeds: `lexer/token_keyword_core.sv0` — keyword band **1–5** `fn`/`let`/`if`/`break`/`continue`, `lexer/token_delim_core.sv0` — punct **10–19** (includes **`.`** and assignment **`=`**), `lexer/token_op_core.sv0` — ops **20–29** (**`%`** **29**) + compound assigns **211–220**).
4. **Env (name resolution)** — `sml/name_resolution/env.*` → `lib/env_core.sv0` (bounded module-value register + lookup); `lib/env_scope_core.sv0` (`enterScope` / `bindLocal` / `exitScope` slice, two frames × two locals); `lib/lookup_value_core.sv0` (**merged** `lookupValue`: locals then module); `lib/resolver_value_core.sv0` (three-slot path-only `lookup_value`); `lib/resolver_arity_core.sv0` (`registerFnArity` / `lookupFnArity` / call-arity check); `lib/resolver_call_core.sv0` (**merged** value env + arity + `expr_call_ok`); `lib/lookup_type_core.sv0` (prelude type tags + `registerModuleType` slice + single-segment `lookupType` / **Self**); `lib/type_alias_core.sv0` (`tyAlias`, `hasTyAliasName`, `resolveCanonicalTy`); `lib/lookup_type_alias_core.sv0` (**integrated** `resolveCanonicalTy` then prelude/mod **`lookupType`** in one struct — primary dev/test story for type paths).
5. **AST** — `sml/ast/*`
6. **Parser** — `sml/parser/*` → `parser/expr_entry_core.sv0` (**parsePrimaryExpr** dispatch + lexer bridges); `parser/item_entry_core.sv0` (**parseItem** top-level dispatch + **`tag_kw_fn`** bridge); `parser/stmt_entry_core.sv0` (**tryStmt** first-token order + **`tag_kw_let`** / assert stand-in + assign fallback arm); `parser/assign_lhs_core.sv0` (**parseAssignTarget** shapes + **`tryParseAssignLHS`**; **`tag_delim_dot`** and **`tag_delim_eq`**, tags **18**, **19**); `parser/assign_binop_core.sv0` (**assignBinop** → tags **211–220**); `parser/expr_rhs_stub_core.sv0` (assignment **RHS** stub: atom or **lit + lit**); `parser/expr_pratt_stub_core.sv0` (**pratt_from_atom**, **pratt_is_add_binop** tags **20**, **21**, **pratt_rhs_ok_with_semi7**); `parser/expr_prec_ladder_core.sv0` (**parseAddExpr** / **parseMulExpr** tier ranks vs **`token_op_core`**); `parser/expr_cast_core.sv0` (**parseCastExpr** slice + **`parse_cast_stub_ok_with_semi6` or `7`** vs **`type_parse_prefix`**); `parser/expr_unary_stub_core.sv0` (**parseUnaryExpr** five-token window: minus, bang, star deref, amp, amp-plus-mut, then atom); `parser/type_parse_core.sv0` (**parseType** prefix via **`include/type_parse_prefix.sv0`**); `parser/try_assign_stmt_core.sv0` (**tryAssignStmt** + flat and Pratt RHS helpers on ident / deref / field / index LHS)
7. **Name resolution** — `sml/name_resolution/*` (full resolver; builds on **§4**)
8. **Type checker** — `sml/type_checker/*`
9. **Contracts** — `sml/contract_analyzer/*`
10. **IR** — `sml/ir/*` → `lib/lower_unop_core.sv0` (**unopToC** / **ExprUnop** discriminant slice toward **`Ir.Unop`** vs **addr-of**); `lib/lower_lit_core.sv0` (**lowerLit** literal-kind slice: **Int** / **Bool** / **Unit** / **String** vs unsupported); `lib/lower_binop_core.sv0` (**Ir.Binop** class for **Add**…**Shr** vs logic/cmp tags); `lib/lower_assign_lhs_core.sv0` (**classifyAssignLhs**); `lib/lower_ast_ty_cstring_core.sv0` (**astTyToCString** classes); `lib/lower_value_to_expr_core.sv0` (**valueToExpr**); `lib/lower_ast_binop_to_c_core.sv0` (**astBinopToC** width classes); `lib/lower_expr_init_cty_core.sv0` (**exprInitCty** expr-shape → C type string decision); `lib/lower_match_scrut_cty_core.sv0` (**matchScrutCty** scrutinee C-type classes + param-type mirror); `lib/lower_callee_ret_cty_core.sv0` (**calleeRetCty** **`ItemFn`** lookup + return **`astTyToCString`**); `lib/lower_enum_tag_core.sv0` (**enumTag** enum/variant → discriminant **`k`**, harness hits vs SML **`raise`**); `lib/lower_struct_fields_core.sv0` (**structFields** **`structFieldOrder`** hit → field count, miss → **255**); `lib/lower_resolve_fn_callee_core.sv0` (**resolveFnCallee** import-alias hit + simple **`splitQName`** target vs **`raise`**, else original name); `lib/lower_resolve_enum_ctor_path_core.sv0` (**resolveEnumCtorPath** alias hit + two-segment **`splitQName`** → **`(e2,v2)`**, else **`(en,vn)`**); `lib/lower_split_qname_core.sv0` (**splitQName** on **`::`**, bounded ASCII-slot harness → segment **count**); `lib/lower_enum_store_payload_core.sv0` (**enumStorePayload** **`p{i}`** suffix + store count + **`valueToExpr`** RHS class per slot); `lib/lower_find_variant_ast_core.sv0` (**findVariantAst** **`ItemEnum`** / variant name hit harness + **1/2/3** shape tags vs **`enumTag`** error band); `lib/lower_ok_payload_cty_core.sv0` (**okPayloadCty** **`Ok`** = **`VariantTuple`** with arity **1** + payload **`astTyToCString`** subset **0/1/2**); `lib/lower_scan_lets_core.sv0` (**scanLets** one-binding harness: explicit ty vs **`exprInitCty`**, **1000+/2000+** packed codes); `lib/lower_with_scrut_locals_core.sv0` (**withScrutLocals** **`extra @ old`** depth **+** enter, restore **`old`** exit); `lib/lower_mentions_result_core.sv0` (**mentionsResult** **`result`** path leaf + **`expr_shape_tag`** OR-combine harness for **`Ast.expr`** forms); `lib/lower_ret_syntax_is_unit_core.sv0` (**retSyntaxIsUnit** **`has_ret`** + **`ret_ty_class`** vs **`lowerFn`** **`useRetSlot`** gate); `lib/lower_old_slot_core.sv0` (**oldSlot** **`_sv0old_`^`x`**, prefix length **8** + param name length); `lib/lower_all_old_names_core.sv0` (**allOldNames** **`old(x)`** count **1**, leaves **0**, **`expr_shape_tag`** recursive **+**); `lib/lower_uniq_old_names_core.sv0` (**uniqOldNames** **`uon_acc_has`** / prepend **`acc`**, **`in_len`** **≤4**); `lib/lower_old_targets_core.sv0` (**oldTargets** **`pat_kind`** + **`ot_mentions_has`**, **`old_targets_hits`**)
11. **Link** — `sml/link/link.sml` → `lib/link_strip_core.sv0` (**stripLinkDirectives** top-level item filter: drop **ItemUse** / **ItemModule** only)
12. **Backends** — `sml/backend/c/*`, `sml/backend/vm/*`

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
| `./scripts/sv0 self-host-sv0-loop` | Milestone **3** pilot: deterministic heap emit ×2, **`cc`**+run, optional self-host compiler diff |
