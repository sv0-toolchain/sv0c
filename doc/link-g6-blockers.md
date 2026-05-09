# G6 link/driver notes (bootstrap staging + deferred engineering)

This note tracks **bootstrap-transliteration closure** vs **full SML parity** for link rewrite passes (**M3-S-039**), project merge orchestration (**M3-S-040**), and driver composition (**M3-S-041**). **G6** slice rows move to **Done** once staging criteria + CI guards land; remaining bullets describe **follow-on** work (single sv0 TU pipeline, full **`linkProjectDir`** arena merge).

## M3-S-039 — `mapTy` / `mapExpr` / `mapPat` / `mapItem` arena rewrites

**SML** (`sv0c/sml/link/link.sml`) builds recursive **`Ast.*`** trees; `mapPathSegs` returns **new** path lists with **fresh** identifiers after `mangle`.

**sv0 bootstrap** stores paths as **indices into the lexer token stream** (`pp` pool + `starts`/`ends` into **`source`**). A **semantic** rewrite must either:

1. **Replace token indices** with handles whose span text equals `mod__Name` (requires **new** source text + **new** `(start,end)` rows — i.e. a **synthetic token append** API), or  
2. **Clone** the whole program into a side arena that owns copied strings (major pipeline change).

**Shipped toward closure**

- Predicate stack (`link_*_needs_mangle`, subtree counts) — verification-only.  
- **String preview (no mutation):** **`link_ty_tyname_first_seg_preview`** applies `map_path_segs_rewrite_1` / `_2` to the **text** of the first path segment for **`TyName`** rows; **`link_ty_tyname_second_seg_preview`** surfaces the **second** segment text when `sc==2` (unchanged by SML **`mapPathSegs`** except via the first segment). **`link_expr_path_first_seg_preview`** / **`link_expr_path_second_seg_preview`** mirror that for **`ExprPath`** (same **`pp`** pool encoding as **`link_expr_path_needs_mangle`**) — documents **what** the rewriter would emit before handles exist.
- **Path-level mutation (token-backed arenas):** **`link_append_synth_ident`** + per-tag rewrite helpers + arena-wide drivers (**`link_ty_arena_rewrite_all_first_seg`**, **`link_expr_arena_rewrite_all_first_seg`**, **`link_pat_arena_rewrite_all_first_seg`**) + unified entry **`link_apply_map_path_segs_program_source`** — covers SML **`mapPathSegs`** effects on **`TyName`**, **`ExprPath`**, and **`PatStruct`/`PatEnum`** paths for the flat bootstrap representation.

- **Defining item identifiers (partial `mapItem`):** **`link_item_row_rewrite_defining_name`** / **`link_item_arena_rewrite_defining_names`** append synthetic tokens for **`item_names`** rows where **`is_top_defining`** (ItemFn / ItemStruct / ItemEnum / ItemTrait / ItemTypeAlias), using **`fn_name_for_link`** for **`main`**. **`link_apply_map_link_pass_program_source`** runs **`link_apply_map_path_segs_program_source`** first (same **`tops`** as **`collect_top_names`**), then defining-item passes — duplicate mangled **lines** can occur only if the same lexer token index is rewritten by **both** path pools and **`item_names`** (distinct indices in real sources).

**M3-S-039 closure (bootstrap):** the stage-0 **`parse_program`** / **`parse_item`** surface only builds **top-level** **`item_tags`** rows (trait/impl methods appear as additional **`ItemFn`** rows). There is **no** nested **`StmtItem`** / inner-**`mapItem`** path in the flat item arena — SML’s stmt-level item forms are out of scope for this bootstrap representation. Cross-module **`mapPathSegs`** + **`link_item_arena_rewrite_defining_names`** therefore match the **`link.sml`** passes that apply to **`mapProgramUnit`**’s **body** item list. **`M3-S-039`** is **Done** in **## M3 G6 slice status** (passes + tests + this note).

## M3-S-040 — `linkProjectDir` program merge

**SML:** `List.concat (map (fn p => mapProgramUnit p (parseFile p)) paths)` — **per-file** parse, **per-module** `mapItem`, then concatenate **AST** item lists.

**sv0:** `parse_program` consumes **one** token stream. Concatenating sources then lexing once is **not** equivalent (diagnostics paths, module boundaries, token indices). A faithful merge needs **per-file** `parse_program` plus **index relocation** / merged program-level arenas.

**Shipped toward closure**

- Listing/dir bridges + **`link_project_concat_sources_offsets_*`** sidecars (source byte offsets per file in a concat buffer).
- **Token-stream merge helpers:** **`link_merge_sources_two`**, **`link_second_file_byte_offset_after_concat`**, **`link_merge_parallel_token_streams_reloc_b`**, **`link_reloc_i32_vec_inplace`** — relocate file-B lexer **`starts`/`ends`** by **`len(fileA)+1`** for the newline separator used by **`link_project_concat_sources_from_listing`**, then concatenate parallel tag/spans Vecs. **`link_program_item_vecs_append`** concatenates parallel **item** rows (`it`, **`id1`**–**`id5`**) for a merged program **after** caller-supplied expr/ty relocation. **`scripts/verify_m3_g6_link_merge_contract.py`** (**`test-guards`** / **`test`**) keeps these symbols from disappearing.

**M3-S-040 bootstrap scope (Done in ## M3 G6):** listing/dir/entry bridges, source offset sidecars, token merge, and item-row append are **shipped** with unit tests + contract script.

**Remaining engineering (not G6 “full SML parity”):** run **`tokenize` + `parse_program` per path**, relocate **expr/ty/pat** arena indices, then feed **`link_program_item_vecs_append`** — end-to-end orchestration for a future milestone slice (see **`doc/transliteration-plan.md`** Link row).

## M3-S-041 — single bootstrap TU driver (staging **Done**)

**Bootstrap / transliteration closure:** stage-0 **composition** is the **SML/NJ `sources.cm` heap** (`sml/main.sml` entry). Each **`lib/*.sv0`** file is **VM-compiled independently** — **`lib/main.sv0`** cannot call **`parse_program`** from **`lib/parser.sv0`** without a linker or mega-module. **M3-S-041** is **Done** in **## M3 G6** when **`driver-pipeline-composition.md`** documents this model and **`scripts/verify_m3_g6_pipeline_contract.py`** + **`scripts/verify_m3_g6_staging_driver_contract.py`** pass (**`test-guards`** / **`test`**).

**Deferred:** one sv0 TU (or native sv0 link) that runs **lexer → parser → … → emit** — not required to clear **G6** in the staging sense above.

## Related

- **`sv0c/doc/transliteration-plan.md`** — Link row.  
- **`sv0c/doc/driver-pipeline-composition.md`** — driver single-TU vs composition (**M3-S-041**).  
- **`sv0c/doc/bootstrap-compiler-workarounds.md`** — synthetic names / arena limits.  
- **`42-transliteration-complexity-invariant.mdc`**.
