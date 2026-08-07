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
