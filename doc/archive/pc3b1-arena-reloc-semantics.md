# PC-3b.1 — parse arena index-relocation semantics

**Deliverable of PC-3b.1** (see `pc3b-linkprojectdir-scoping.md`). To merge a
second parsed file **B** into file **A**, every cross-arena index in B's arenas
must be offset by A's corresponding arena size. This table pins, **per arena
column and per node tag**, the exact relocation rule.

**Key finding:** all layouts are **documented in `lib/parser.sv0`** (item arena
@L3063, body-expr arena @L1155, pty arena @L664, pat rows @L880), so the pass is
**transcription, not reverse-engineering** — the relocation is *tractable*.

## Relocation deltas (from file A's sizes)

| Delta | = size of A's arena | Applies to columns holding an index into… |
|-------|---------------------|-------------------------------------------|
| `dTok` | `len(tags_A)` (= token count) | token stream (`starts`/`ends` are byte offsets → use `dByte`) |
| `dByte` | `len(sourceA)+1` | `starts_A`/`ends_A` byte positions (existing `link_merge_parallel_token_streams_reloc_b`) |
| `dPP` | `len(pp_A)` | path pool |
| `dBody` | `len(body_et_A)` | body-expr arena |
| `dPty` | `len(pty_tt_A)` | parse-type arena |
| `dSf` | `len(body_sf_A)` | struct-field-name token sidecar (`ExprStruct.d4`) |
| `dParamName` | `len(fn_param_name_toks_A)` | fn param name sidecar (`ItemFn.d5` base) |
| `dStructName` | `len(struct_field_name_toks_A)` | struct field name sidecar (`ItemStruct.d3` base) |
| `dEnumName` | `len(enum_variant_name_toks_A)` | enum variant name sidecar (`ItemEnum.d3` base) |
| `dContract` | `len(fn_contract_root_A)` | `fn_contract_base_by_item` base |
| `dPayloadTy` | `len(enum_variant_payload_ty_root_A)` | `enum_variant_payload_base_by_item` base |

Rule of thumb per value class: **`*_idx`/`*_root` → the arena it indexes; `pp_start`
→ `dPP`; `*_tok`/`tok_pos`/`name_tok` → `dTok`; `*_base` → the sidecar it offsets;
tags / counts / packed / flags / `-1` sentinels → NOT relocated.**

## Item arena `it` / `id1..id5` (parser.sv0 L3063)

| tag | d1 | d2 | d3 | d4 | d5 |
|-----|----|----|----|----|----|
| 0 ItemFn | name_tok `dTok` | packed — | param_count — | **body_root `dBody`** | **param_name base `dParamName`** (or -1) |
| 1 ItemStruct | name_tok `dTok` | field_count — | **field-name base `dStructName`** | field_count — | -1 |
| 2 ItemEnum | name_tok `dTok` | variant_count — | **variant-name base `dEnumName`** | variant_count — | -1 |
| 3 ItemTrait | name_tok `dTok` | method_count — | 0 | -1 | -1 |
| 4 ItemImpl | has_trait — | items_count — | 0 | -1 | -1 |
| 5 ItemUse | **pp_start `dPP`** | pp_count — | | | -1 |
| 6 ItemModule | **pp_start `dPP`** | pp_count — | | | -1 |
| 7 ItemTypeAlias | name_tok `dTok` | | | | -1 |

## Body-expr arena `body_et` / `ed1..ed4` (parser.sv0 L1155)

`_idx`/`_first` = `dBody`; `pp_start` = `dPP`; `*_tok`/`tok_pos` = `dTok`;
`type_arena_idx` = `dPty`; `sf_names_start` = `dSf`; counts/tags = —.

- 0 Lit: d1 lit_tag —, d2 tok `dTok`
- 1 Path: d1 pp_start `dPP`, d2 pp_count —
- 2 Unop: d1 tag —, d2 operand `dBody`
- 3 Binop: d1 tag —, d2 lhs `dBody`, d3 rhs `dBody`
- 4 Call: d1 callee `dBody`, d2 args_first `dBody`, d3 count —
- 5 MethodCall: d1 obj `dBody`, d2 method_tok `dTok`, d3 args_first `dBody`, d4 count —
- 6 Field: d1 obj `dBody`, d2 field_tok `dTok`
- 7 TupleField: d1 obj `dBody`, d2 field_num —
- 8 Index: d1 obj `dBody`, d2 index `dBody`
- 9 Block: d1 stmts_first `dBody`, d2 count —, d3 tail `dBody`(-1), **d4 stmt-order sidecar base `dPP`** (`offset+1`; 0 = none — see pp-heterogeneity note below)
- 10 If: d1 cond `dBody`, d2 then `dBody`, d3 else `dBody`(-1)
- 11 Match: d1 scrut `dBody`, d2 arms_first `dBody`, d3 count —
- 12 While: d1 cond `dBody`, d2 body `dBody`, d3 inv_first `dBody`, d4 count —
- 13 For: d1 pat `dBody`, d2 iter `dBody`, d3 body `dBody`
- 14 Loop: d1 body `dBody`
- 15 Return / 16 Break: d1 value `dBody`(-1)
- 17 Continue: —
- 18 Assign: d1 lhs `dBody`, d2 rhs `dBody`
- 19 AssignOp: d1 tag —, d2 lhs `dBody`, d3 rhs `dBody`
- 20 Cast: d1 expr `dBody`, d2 **type_arena_idx `dPty`**
- 21 Range: d1 lo `dBody`(-1), d2 hi `dBody`(-1)
- 22 Try / 23 Assert: d1 expr `dBody`
- 24 Struct: d1 pp_start `dPP`, d2 pp_count —, d3 field_count —, d4 **sf_names_start `dSf`**
- 25 Tuple / 26 Array: d1 first_elem `dBody`, d2 elem_count —
- 27 LetStmt: d1 name_tok `dTok`, d2 annot-type-head tok `dTok`(-1), d3 init `dBody`(-1), d4 is_mut —
- 28 SemiStmt: d1 expr `dBody`
- 29 MatchArm: d1 pat_tag —, d2 pat_d1 **or** pat_extra_idx `dBody` (tag-dependent — see note), d3 guard `dBody`(-1), d4 body `dBody`
- 30 PatExtra / 31 PatFieldEmbed / 32 PatStructMeta: embed parse_pat row data (PatBind d1 name_tok `dTok`; PatLit d1 tag —, d2 tok `dTok`; PatStruct/PatEnum d1 pp_start `dPP`, d2 pp_count —) + child refs `dBody`.

**Note (MatchArm d2):** when the arm pattern is simple (PatWild/PatBind) d2 is
inline pat data (name_tok → `dTok`); when PatLit/PatStruct/PatEnum, d2 is a
`dBody` index to the ExprPatExtra node. The pass must switch on d1 (pat_tag).
This is the one column needing tag-aware handling beyond the table.

## Parse-type arena `pty_tt` / `td1..td3` (parser.sv0 L664)

- 0 TyName: d1 pp_start `dPP`, d2 seg_count —, d3 type_arg_count —
- 1 TyRef / 2 TyRefMut: d1 inner `dPty`
- 3 TyArray: d1 elem `dPty`, d2 len_tok `dTok`
- 4 TySlice: d1 elem `dPty`
- 5 TyTuple: d1 first_elem `dPty`, d2 elem_count —
- 6 TyUnit: —

## Sidecar arrays (each element's class)

| Array | element class | delta |
|-------|---------------|-------|
| `pp` (path pool) | **HETEROGENEOUS** — token index OR block-order body index (see note) | `dTok` **or** `dBody` |
| `enum_variant_name_toks`, `struct_field_name_toks`, `fn_param_name_toks` | token index (or -1) | `dTok` |
| `fn_param_ty_root`, `fn_ret_ty_root_by_item`, `struct_field_ty_root`, `enum_variant_payload_ty_root` | pty root | `dPty` |
| `fn_contract_root` | body-expr root (contract exprs live in body arena) | `dBody` |
| `fn_contract_base_by_item` | base into `fn_contract_root` | `dContract` |
| `enum_variant_payload_base_by_item` | base into `enum_variant_payload_ty_root` | `dPayloadTy` |
| `enum_variant_payload_count_by_item`, `enum_variant_payload_max_by_item` | count | — |

## pp heterogeneity (CORRECTION, 2026-08-08 — found in PC-3b.4b, extended in PC-2e/enum-struct)

The `pp` pool is **not** uniformly token indices. `parser.sv0
block_stmt_sidecar_push` stores an ordered list of **body-expr** child indices in
`pp` (children are interleaved with their sub-exprs in the body arena, so
`first + si` can't recover order); the owning node records that sidecar's base in
its **`d4 = offset+1`** column (0 = no sidecar). **Three body tags carry such a
sidecar** (the only `block_stmt_sidecar_push` callers — parser.sv0 L1627/2560/2584/3017):

| tag | node | count column | sidecar-base column |
|-----|------|--------------|---------------------|
| 4 | Call (arg roots) | `ed3` (args_count) | `ed4` |
| 9 | Block (stmt ids) | `ed2` (stmt_count) | `ed4` |
| 11 | Match (arm ids) | `ed3` (arms_count) | `ed4` |

(MethodCall tag 5 is **contiguous** — `margs_first + mi`, no sidecar.) So a `pp`
entry owned by one of these sidecars is a **body index** (`dBody`); every other `pp`
entry (path segments from `ExprPath`/`ExprStruct`/`TyName`/`ItemUse`) is a **token
index** (`dTok`). A single whole-column shift is wrong. Fixes in `lib/link.sv0`:
`link_reloc_pp_contents_hetero(pp, bet, ed2, ed3, ed4, dTok, dBody)` marks the
sidecar ranges (base `d4-1`, count per `link_pp_sidecar_count`) → `dBody`, the rest
→ `dTok`; and `link_reloc_body_arena` shifts the `d4` base by `dPP` for **all three**
tags (4/9/11). Even single-statement blocks / single-arg calls create a sidecar, so
this affects nearly every fn body. (This corrects the `pp` row above and the omitted
`d4` sidecar-base columns in the body-expr table for Call/Block/Match.)

## Consequences for PC-3b.2 (the relocation pass)

- **~11 deltas**, computed once from A's arena lengths.
- Most columns are a **flat `link_reloc_i32_vec_inplace(colB, delta)`** guarded by
  a `-1` sentinel skip. Only three columns need **tag-aware** relocation: the
  body-expr `ed1..ed4` (switch on `body_et` tag per the table above), the pty
  `td1..td3` (switch on `pty_tt` tag), and the item `id1..id5` (switch on `it`
  tag). Everything else is a whole-column shift.
- **Order:** relocate B first, then append B's columns onto A's (PC-3b.3), so the
  appended indices already point into the merged arenas.
- **Test surface (PC-3b.2):** build two tiny programs' arenas by hand (extend the
  PC-3a pattern), relocate B, and assert representative indices land in A's ranges
  — one assertion per delta class (body_root, pp_start, ty_root, param base, a
  body-expr `_idx`, a pty child, a sidecar token). Then the end-to-end acceptance
  (PC-3b.6) diffs native `--project` vs SML `--project`.

**Bottom line:** the relocation is fully specified by the documented layouts and
reduces to ~11 whole-column shifts + 3 tag-aware column walks. PC-3b.1 confirms
PC-3b.2 is **well-defined and testable**; the residual risk is transcription
accuracy (mitigated by the per-class unit tests above), not unknown semantics.
