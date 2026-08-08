# PC-3b — `linkProjectDir` in sv0: scoping investigation

**Task.** Implement `link_project_dir_from_listing` — the per-file
`parse_program` + arena-relocation + merge orchestration that makes the **sv0**
pipeline compile a **multi-file project**, matching SML `Link.linkProjectDir`.

**Why it matters.** It is the gateway to **sv0-native multi-module compilation**
and the **only** thing gating **PC-2e** (the sv0 transliteration of the Epic-1/2
cross-module import-alias fixes — untestable cross-module until the sv0 compiler
can compile projects). The reference pipeline already handles cross-module types
(Epics 1/2, done in sml-legacy); PC-3b lets the *sv0-built* compiler match it.

**Status:** scoping only — not started. PC-3a (the mechanical two-file merge
*composition* test) is done and green.

---

## 1. What SML does (the reference — trivially simple)

`sml-legacy/link/link.sml`:
```sml
fun linkProjectDir dir =
  List.concat (map (fn p => mapProgramUnit p (parseFile p)) (listSv0 dir))
```
- `parseFile p` = read + `IncludeExpand.expandFile` + `Lexer.tokenize` + `Parser.parse` → an **`Ast.program`** (an immutable **list** of items).
- `mapProgramUnit` = `splitModule` + `collectTops` + `map (mapItem tops modNm)` — mangle each file's defining names + path segs with its module id.
- `List.concat` — merge is just list append. **No index relocation** because `Ast.program` is a recursive immutable tree, not flat arenas.

The sv0 side is hard *only* because sv0's AST is **flat parallel Vec<i32> arenas**, so "concat" means relocating every cross-arena index.

## 2. Two real blockers

### 2a. The standalone-unit wall
`lib/link.sv0` **cannot** call `parse_program` (it lives in `lib/parser.sv0`;
each `lib/*.sv0` is VM-compiled as an independent unit — no linker; a second
`fn main` → E0302). So the orchestration **cannot** be a self-contained
`link.sv0` function. It **must** live where `parse_program` is already callable:
the **mega-TU compose main** (`lib/megaTU-main.sv0` and the C/VM/native compose
mains derived from it by `scripts/build-sv0-megatu-*.sh`). That compose context
dissolves the wall — it already runs `tokenize → parse_program → resolve → check
→ lower → emit` on a single source.

### 2b. Flat-arena relocation (the hard part)
`parse_program` fills **~30 parallel arenas**. Merging file B into file A means
offsetting **every cross-arena index** in B by A's corresponding arena sizes.
The relocation surface (from the `parse_program` signature):

| Group | Arenas | Index semantics (what must be relocated) |
|-------|--------|------------------------------------------|
| Token stream | `tags`, `starts`, `ends` | `starts`/`ends` are **byte offsets** → +len(sourceA)+1 (existing primitive `link_merge_parallel_token_streams_reloc_b`) |
| Path pool | `pp` | entries are **token indices** → +tokenCountA |
| Item arena | `it`, `id1`–`id5` | `id4` = **body-expr root**; `id5`/bases = **sidecar bases**; others = token idx / counts (append exists via `link_program_item_vecs_append`, but it does **not** relocate `id4`/bases) |
| Fn/enum/struct sidecars | `enum_variant_name_toks`, `struct_field_name_toks`, `fn_param_name_toks`, `fn_param_ty_root`, `fn_ret_ty_root_by_item`, `fn_contract_base_by_item`, `fn_contract_root`, `enum_variant_payload_ty_root`, `enum_variant_payload_base_by_item`, `enum_variant_payload_count_by_item`, `enum_variant_payload_max_by_item` | mix of **token indices**, **pty-arena roots**, **body-arena roots**, **base offsets into other sidecars**, and plain **counts** — each column needs its semantics pinned |
| Parse type arena | `pty_tt`, `pty_td1`–`td3`, `struct_field_ty_root` | `pty_td*` hold **pty-arena child indices** + **pp/token refs** → +ptyCountA / +ppCountA; `struct_field_ty_root` = **pty root** |
| Body expr arena | `body_et`, `body_ed1`–`ed4`, `body_sf` | `body_ed*` hold **body-arena child indices**, **pp indices**, **pty roots**, **token positions** depending on the node tag → the most intricate to relocate |

**The crux:** a correct relocation pass must know, **per column and (for the
expr/ty arenas) per node tag**, whether each value is a token index, a pp index,
a body-arena index, a pty-arena index, a sidecar base, or a plain count/scalar.
Getting one column wrong = a subtle miscompile. This is new code — the existing
primitives cover only the token merge + the item-arena *append* (no reloc).

## 3. No `--project` entry in any sv0 compose main
`sml-legacy/main.sml` has `compileProjectDir` / `compileProjectDirVm` (argv
`--project <dir>`). The sv0 compose mains (`megaTU-main.sv0` + the derived
CLI/VM/native mains) are **single-file only** (`megaTU-main.sv0` hardcodes one
`source`; `build-sv0-megatu-native.sh`'s CLI main reads one path from
`/tmp/.sv0_drv_path`). PC-3b must add a directory/multi-file entry, incl.
`read_dir` host I/O to list the `.sv0` files (runtime builtin exists — used by
`include_expand` / driver).

## 4. Decomposed sub-tasks

- **PC-3b.1 — Pin the arena index semantics.** Read `parse_program` (+ the
  lower/resolve consumers) and produce a table: for every arena column (and every
  expr/ty node tag), the exact relocation rule. *(Design-critical; the rest
  depends on it. ~½–1 day of careful reading.)*
- **PC-3b.2 — `relocate_program_arenas(delta_*)`.** A pass that, given file B's
  full arena set and file A's sizes (tokenCount, ppCount, itemCount, bodyCount,
  ptyCount, sidecar bases), offsets every cross-arena index per PC-3b.1. Unit-test
  on hand-built two-program arenas (extend the PC-3a pattern). *(The hard, risky
  core.)*
- **PC-3b.3 — `merge_program_arenas`.** Append B's relocated arenas onto A's
  across all ~30 columns (generalize `link_program_item_vecs_append`).
- **PC-3b.4 — Per-file parse loop in the compose main.** `read_dir` → for each
  `.sv0`: `read_file` + `expand_file` + `tokenize` + `parse_program` into per-file
  arenas; then mangle (`link_apply_map_link_pass_program_source`) + relocate
  (3b.2) + merge (3b.3). Lives in `megaTU-main.sv0` behind a `--project` branch.
- **PC-3b.5 — `--project` CLI wiring.** A directory mode in the compose main +
  the `build-sv0-megatu-*.sh` adapters (dir path, not a single file).
- **PC-3b.6 — Acceptance.** Native mega-TU `--project` on
  `test/integration/modules_enum_match` + `modules_struct_type` compiles+runs
  (exit 42) matching SML `--project`. This is the sv0-native cross-module surface
  and directly **unblocks PC-2e**.

## 5. Risks & effort

- **Effort: LARGE** (multi-day). PC-3b.1+3b.2 are the bulk and the risk.
- **Correctness risk: HIGH.** ~30 arenas × per-tag index semantics; one wrong
  column is a silent miscompile that only shows on specific constructs. Mitigate
  with aggressive unit tests on hand-built merges + diffing native `--project`
  output against SML `--project` per-file.
- **Surface risk: MEDIUM.** Touches `megaTU-main.sv0` (⇒ the C, VM, and native
  compose mains all re-derive from it) — must keep single-file compile + the
  98/98 corpus + 18/18 VM parity green throughout.
- **Golden/gotcha:** editing `megaTU-main.sv0` / parser consumers ⇒ refresh
  stage0 + vm-parity goldens; never edit sv0c source mid-pre-push.

## 5b. PC-3b.4 blocker found (2026-08-05): the mangle passes are partial

Investigating 3b.4 surfaced that the M3-S-039 mangle primitives
(`link_apply_map_link_pass_program_source` → `link_expr_path_rewrite_first_seg_handle`)
only rewrite **ExprPath (body tag 1)** — the per-row handle bails on any other
tag. They do **not** mangle:
- **ExprStruct (24)** — intra-module struct construction `Point { … }`.
- **pattern rows (30 PatExtra / 31 PatFieldEmbed)** — the `pp_start` of a
  `PatStruct`/`PatEnum` arm (intra-module `match c { Color::Red => … }`).
- (verify) the **pty TyName** first-seg for intra-module type references.

SML `mapProgramUnit`/`mapPathSegs` rewrites all of these (that is why the
existing `modules_types` fixture — struct+enum used *within* `lib` — passes via
SML `--project`). So a faithful sv0 merge needs the mangle extended to cover
`parse_program`'s real body-embedded path-bearing tags. This is a real
prerequisite, decomposed as:

- **PC-3b.4a — complete the mangle passes.** Extend the expr path-rewrite to
  handle ExprStruct(24) + pattern rows(30/31) (and confirm pty TyName), matching
  SML `mapPathSegs`. Standalone-testable in `link.sv0` (hand-built arenas, like
  PC-3a/3b.2/3b.3) — **safe, low-risk**.
- **PC-3b.4b — the per-file parse loop** (compose main): parse each file →
  extended-mangle → reloc (3b.2) → merge (3b.3). Then 3b.5 `--project` CLI +
  3b.6 acceptance.

**Note on the cross-module reference model:** cross-*module* references
(`use lib::Signal; Signal::On`) resolve via the import-alias canonicalization
already added in PC-1/PC-2 (checker/lowering), **not** via link mangling. Link
mangling covers each module's **defining names** + its **intra-module** qualified
references. So 3b.4a is specifically about intra-module struct/pattern mangling.

## 5c. PC-3b.4b implementation spec (ready to execute)

All primitives (3b.1–3b.4a) are done + unit-tested. 3b.4b is the orchestration,
which **must live in `megaTU-main.sv0`** (parse_program callable) and be
**runtime-validated** (a temporary test call at the top of `main` → build the
native mega-TU → run → confirm → **remove the test call before commit**; do NOT
ship a self-test in the shared corpus `main`, and do NOT restructure `main`'s
`let source` line or phase-6 emit — the build scripts regex-patch them).

**Algorithm — `megatu_link_two_programs(entry_a, src_a, entry_b, src_b, <~30 out arenas>) -> merged_src`:**
1. **Parse A into the out arenas:** `tokenize(src_a, out_tags, out_starts, out_ends)`;
   `parse_program(out_tags, …, src_a, 0, <out arenas>)`.
2. **Mangle A** (order matters — mangle mutates source + appends tokens, so do it
   before computing deltas): `mod_a = module_id_from_entry_path(entry_a)`;
   `tops_a = collect_top_names(out_it, out_inm)`;
   `s = link_item_arena_rewrite_defining_names(out_it, out_inm, mod_a, out_starts, out_ends, src_a)`;
   `s = link_ty_arena_rewrite_all_first_seg(out_ptt, out_ptd1, out_ptd2, out_pp, tops_a, mod_a, out_starts, out_ends, s)`;
   `s = link_body_arena_rewrite_all_paths(out_bet, out_bed1, out_bed2, out_bed4, out_pp, tops_a, mod_a, out_starts, out_ends, s)`.
   (Mangling appends synthetic-ident tokens to `out_tags/out_starts/out_ends` and
   updates arena refs; so after this, A's token count = `len(out_tags)`.)
3. **Parse B into temp arenas** (declare ~30 `tb_*` locals): tokenize + parse `src_b`.
4. **Mangle B** the same way with `mod_b` (into `tb_*` + `s_b`).
5. **Compute deltas from A's post-mangle sizes:** `d_tok=len(out_tags)`,
   `d_pp=len(out_pp)`, `d_body=len(out_bet)`, `d_pty=len(out_ptt)`,
   `d_sf=len(out_bsf)`, `d_paramname=len(out_fpn)`, `d_structname=len(out_sfn)`,
   `d_enumname=len(out_evn)`, `d_contract=len(out_fcr)`, `d_payloadty=len(out_evpt)`,
   `d_byte = len(s) + 1` (byte offset for B's token positions).
6. **Reloc B** (PC-3b.2): `link_reloc_item_arena` / `link_reloc_pty_arena` /
   `link_reloc_body_arena` on the `tb_*` body/item/pty; `link_reloc_skip_neg` per
   sidecar with its delta (fpn/sfn/evn→`d_tok`; fpt/frt/sft/evpt→`d_pty`;
   fcr→`d_body`; fcb→`d_contract`; evpb→`d_payloadty`; pp→`d_tok`).
7. **Merge token streams:** `link_merge_parallel_token_streams_reloc_b(out_tags,
   out_starts, out_ends, tb_tags, tb_starts, tb_ends, d_byte, …)` into out (or
   append tb_tags to out_tags and `tb_starts/ends += d_byte`).
8. **Merge arenas** (PC-3b.3): `link_program_item_vecs_append` (item) +
   `link_append_body_arena` + `link_append_pty_arena` + `link_append_vec` per
   sidecar (pp, evn, sfn, fpn, fpt, frt, fcb, fcr, evpt, evpb, evpc, evpm, sft).
9. **Return** `merged_src = s + "\n" + s_b`.

**Validation test (temporary, in `main`, removed before commit):**
`A = "fn helper() -> i32 { return 7; }"` (no main), `B = "fn main() -> i32 { return 35; }"`.
Merge → run phases 3–6 (resolve/check/lower/emit) on the merged arenas → assert
`megatu_emit_program` returns non-empty C containing `helper` (mangled) + a
`main` returning 35. This proves the merge is structurally sound + the reloc is
correct (arena refs resolve). Cross-module *calls/types* are validated separately
by 3b.6 (the real `--project` fixtures) once 3b.5 wires the CLI.

**Gates to keep green throughout:** `./scripts/sv0 assemble-megatu --check`
(mega-TU compiles), `./scripts/sv0-megatu-corpus-parity.sh` (98/98, single-file
unaffected), full `./scripts/sv0 test`, VM 18/18. Refresh no goldens (megaTU-main
is not in stage0/vm-parity/self-host sets — it only lives in the assembly).

## 5d. PC-3b.4b attempt (2026-08-05): runtime validation found a mangle token-model bug

Implemented the full `test_megatu_link_merge` orchestration per §5c in
`megaTU-main.sv0` (parse×2 → mangle×2 → reloc → merge → resolve+check), added a
temporary `main` call, built the native mega-TU, and ran it. **It compiled but
crashed/failed at runtime** (corpus went 0/98) — exactly what runtime validation
is for. Root cause, confirmed by reading `link_append_synth_ident`:

> The M3-S-039 mangle appends each synthetic (mangled) identifier to **`starts`
> and `ends` but NOT `tags`**, and `link_last_tok_handle` returns
> `len(starts)-1`. So after mangling, `len(starts) > len(tags)`, and mangled
> token handles have **no `tags` entry**.

Consequences for the merge:
1. **Wrong `d_tok`.** It must be `len(a_starts)` (the mangle-extended token
   count), **not** `len(a_tags)` — otherwise B's relocated token refs collide
   with A's appended mangled tokens.
2. **`check_program` breaks.** C-emit only needs `starts/ends` (`handle_to_str`
   reads `source`), so the single-file path never noticed. But `check_program`
   indexes `tags` by handle → OOB/`-1` on the mangled handles. `tags`/`starts`
   must stay parallel.

**Prerequisite fix — PC-3b.4a2 (complete the mangle token model):** make
`link_append_synth_ident` also push an **IDENT tag (`token.sv0` `IDENT` = 5)** to
a `tags` vec so `tags`/`starts`/`ends` stay parallel; thread `tags` through the
mangle handles (`link_ty_tyname_rewrite`, `link_expr_path_rewrite`,
`link_body_path_rewrite_one`, `link_item_row_rewrite_defining_name`) + the map
wrappers. Standalone-testable in `link.sv0` (assert `len(tags)==len(starts)`
after a mangle). **Then** PC-3b.4b's orchestration (use `d_tok=len(a_starts)`;
merge `m_tags` parallel to `m_starts`) is correct. The `megaTU-main.sv0`
orchestration was reverted to keep the gates green; re-apply it from §5c after
3b.4a2 lands.

## 6. Recommendation

**Do it incrementally, PC-3b.1 → 3b.2 first, gated behind `--project` so the
single-file path is untouched.** The value is real (sv0-native multi-module +
unblocks PC-2e = the last "whole-language parity" gap), but it is the **largest,
riskiest** remaining Phase C item, and everything *else* valuable in Phase C is
already done. Reasonable options:
1. **Green-light the full PC-3b** as a multi-session effort, starting with 3b.1
   (pin arena semantics) + 3b.2 (relocation pass) validated on hand-built merges.
2. **Defer PC-3b** — bank Phase C's completed high-value work (Epics 1/2/5/6 +
   PC-3a); the sv0 pipeline stays single-file, cross-module parity proven in the
   reference. Revisit when sv0-native multi-module is actually needed.

Either is defensible. If proceeding, **PC-3b.1 (pin the arena index semantics)**
is the correct, low-risk first step — it's pure investigation and de-risks the
whole effort before any arena-mutation code is written.
