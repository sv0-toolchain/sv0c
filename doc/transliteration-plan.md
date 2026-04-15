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
| VM parity allowlist | `test/vm-parity/manifest.txt` | 97 |

## Definition of done (milestone 3)

1. **Feature parity:** sv0 compiler matches SML on the agreed surface (see **`task/sv0-toolchain-milestone-3-self-host.Rmd`**).
2. **VM + C backends:** Both paths support the same corpus policies (VM parity manifest + allowlist; stage0 C goldens for bootstrap seeds).
3. **Self-host:** Native **`SV0_SELF_HOST_COMPILER`** can rebuild the compiler sources; **`bootstrap-sml-final`** tag and **`sml-legacy/`** layout when SML leaves the default path.

## Inventory: major `sml/` subtrees

| Area | SML path | sv0 today | Next focus |
|------|-----------|-----------|------------|
| Span / diagnostics | `sml/error/` | `lib/span.sv0` (flattened Pos/Span, **`span_merge`** (SML `merge`), **`pos_to_string`** (SML `posToString`), **`span_to_string`** (SML `toString`)), `lib/diagnostic.sv0` (severity, getLine, spaces/carets/padLeft, hasErrors, errorCount, **Vec-based diag list**, **sort-by-span**, **format_header**, **format_snippet**, **diag_int_to_str**, **format_location**, **format_diagnostic**, **`format_header_with_code`** (SML `codeStr` bracket), **`format_gutter`/`format_snippet_numbered`** (SML numbered gutter), **`format_related`/`format_help`** (SML related/help text)) | `report`/`reportAll` I/O. **`span.sml` parity reached**; diagnostic.sv0 covers all SML `format` sub-expressions |
| Lexer | `sml/lexer/` | `lib/lexer.sv0` (**full tokenize pipeline**: char class, keywords, escape, scan string/char/number/op, `tokenize` → parallel Vec output), `lexer/token.sv0` (98-variant enum) | **Pipeline parity reached** — remaining: integrate with parser token stream consumption |
| Parser | `sml/parser/` | `lib/parser.sv0` (binop/prec maps, token helpers, skip functions, **token stream nav** (peek/advance/expect/match), **parsePath**, **parseGenericParams** (extracts type param names from `<T, U: Bound>`), **skip helpers on token stream** (pub/unsafe/generics/where/attrs), 28 keyword constants, **`parse_use_item`** (SML `parseUseItem`), **`parse_module_decl`** (SML `parseModuleDecl`)) | **Full recursive descent** (`parseExpr`/`parseType`/`parsePat`/`parseItem`) — largest deferred piece |
| Name resolution | `sml/name_resolution/` | `lib/env.sv0` (**SML parity**: scope mgmt, mod val/ty, fnArity, tyAlias, registerValueAlias, registerTypeAlias, lookupType, **`type_is_registered`** (combined prelude ∪ modTys — SML `typeIsRegistered`)), `lib/resolver.sv0` (intrinsics, path join, **path_join_vec** (Vec-based generalized join), **register_all_intrinsics** (env registration), **import alias mgmt** (add/lookup/count), **`variant_stem`** (SML `variantStem`), **`enum_variant_reg_path`/`enum_variant_reg_arity`** (SML `enumVariantReg`), **`pat_binds`** (SML `patBinds` — collect bound var handles), **`bind_pattern_locals`** (SML `bindPatternLocals`), helpers) | **Full resolver** (`resolveTy`/`resolveExpr`/`resolveStmt`, `applyUseClause`) |
| Types | `sml/type_checker/types.sml` | `lib/types.sv0` (20-variant Ty enum, Box, Vec, ty_tag, fresh_var) | Mostly complete; string payloads pending |
| Unification | `sml/type_checker/unify.sml` | `lib/unify.sv0` (full structural unify over Ty, **`unify_list`** (pairwise Vec<i32> unification — SML `unifyList`; TyTuple/TyFn/TyNamed now element-wise)) | **Parity reached** (full unify + unifyList) |
| Type checker | `sml/type_checker/checker.sml` | `lib/checker.sv0` (binop class, type predicates, variant/ctor helpers, error codes, env helpers, **path_key_vec** (Vec-based n-segment join), **is_contract_param**, **struct/enum name extraction**, **`expect`** (type unification gate), **`fields_of_struct`** (struct-def index lookup), **`variant_shape_of`** (enum+variant search), **`assign_lhs_bad`** (error sentinel), **`enter_loop`/`exit_loop`** (SML `withLoop` decomposed), **`infer_lit`** (SML `synth` literal arms — literal tag → type tag)) | **Full inference** (`synth`/`checkStmt`/`checkFn`/`initTypes`) — second largest deferred piece |
| Contracts | `sml/contract_analyzer/` | `lib/contract_analyzer.sv0` (identity pass-through) | **Parity reached** (analyzer is trivial in SML too) |
| Pipeline / driver | `sml/main.sml` | `lib/main.sv0` (paths, phase constants, CLI helpers, **`base_name_sv0`** (SML `baseNameSv0` — strip dir + `.sv0` ext)) | `readFile`/`compileFile`/`compileProjectDir` — I/O dependent |
| IR + lowering | `sml/ir/` | `lib/ir.sv0` (Value 8v/Expr 6v/Instr 17v enums, Box/Vec recursive), `lib/lowering.sv0` (QName split, C strings, **full 6-variant unop table**, **old_slot_name** (string concat), **param_name_tag**, **`param_name`** (SML `paramName` — extract handle from PatBind), **uniq_old_names** (Vec dedup), slot/enum/struct helpers, literal class, **`fresh_tmp_str`** (string `_sv0t<N>` — SML `freshTmp`), **`lower_lit_to_ir_tag`** (SML `lowerLit` dispatch), **`fn_ret_cty`** (SML `fnRetCty`), **import alias infra** (`lower_alias_new/add/lookup`), **`resolve_fn_callee`** (SML `resolveFnCallee`), **`resolve_enum_ctor_path`** (SML `resolveEnumCtorPath`), **`lower_field_index`** (SML local `fieldIndex` — struct field name→index)) | **Full `lowerExpr`/`lowerStmt`/`lowerFn`/`lower`** — IR lists from AST |
| Link | `sml/link/` | `lib/link.sv0` (is_sv0, mangling, strip, rewrite predicates, **fn_name_for_link** (main exemption), **link_dir_from_entry**, **backslash-aware find_last_slash**, **`map_path_segs_needs_mangle`/`map_path_segs_mangle_first`** (SML `mapPathSegs` — 1/2-segment path mangling)) | **`mapTy`/`mapExpr`/`mapItem`**, **`walkSv0`/`parseFile`/`linkProjectDir`** |
| C backend | `sml/backend/c/` | `lib/codegen.sv0` (**full** `emitValue`/`emitExpr`/`emitInstr`/`emitFn`/top `emit`, **is_main_block**, **emit_block_body**, **`c_escape_one` control-char parity** (ord < 32 → `\<ord>`)) | **Parity reached** — all SML functions have named sv0 equivalents |
| VM backend | `sml/backend/vm/` | `lib/vm_codegen.sv0` (layout, opcodes, pool, builtins map, **fixed width_of_cty** (proper layout lookup), **enum field helpers**, **layout_lookup**, **`lookup_slot`/`slot_base`/`slot_width`** (SML `lookupSlot`), **`alloc_local`** (SML `allocLocal`/`bindParam`), **`replace_loop_exit_jump`** (SML `replaceLoopExitJump`), **`enum_field_names`** (SML `enumFieldNames`), **`value_width`** (SML `valueWidth`), **`bind_param`** (SML `bindParam` — slot_info with struct/enum width)), `lib/bytecode.sv0` (50 opcodes, sizes, LE helpers, file layout, **Vec-based LE encoding** (encode_u16_le/u32_le/i32_le), **encode_header**, **decode_u16_at/u32_at**, **verify_magic**, **`encode_strings`** (SML `encodeStrings` — string pool byte codec), **`encode_string_literals`** (handle-list variant), **`decode_strings`** (SML `decodeStrings` — reverse parse)) | **`emitValue`/`emitExpr`/`emitInstr`/`emitBlock`** + full `encodeInsn`/`decodeInsnVec`/`encodeFile`/`decodeFile` |
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
