# L0 closure — engineering roadmap (sv0c)

Normative **“M3 done”** criteria live in **`task/sv0-toolchain-milestone-3-self-host.Rmd`** (**## completion criteria**, **## stakeholder closure checklist**). This note is **implementation sequencing** toward **L0** (*stakeholder* closure — compiler fully in sv0, parity, **native** self-host), not a substitute for that task file.

> **Absorbs the former root `sv0-g6-to-g9-plan.md`.** That standalone plan has been folded into this doc so the L0 engineering sequence lives in one place under `sv0c/doc/`. Phase labels **A–E** below are the detailed execution steps; the four **L0 prerequisites** table is the criteria summary.

**Status snapshot (2026-07-09):** All slice gates **G1–GX Done**; SML retired to **`sml-legacy/`**; **`bootstrap-sml-final`** tag exists. **`lib/driver.sv0`** is a self-contained lex→parse→resolve→check→emit unit: **49 self-host unit tests pass** and **`./scripts/sv0 self-host-sv0-loop` is 98/98** in SML-emit mode. A **native** binary **`build/sv0-driver-native`** is built from `driver.sv0` (SML→C→cc, CLI mode via `/tmp/.sv0_drv_path`) and now reaches **98/98 native↔SML behavioral parity** (`./scripts/sv0 self-host-native-parity`: emit C with both compilers, cc+run both, identical stdout+exit). Parity is **behavioral, not byte-identical** — the native driver inlines expr trees (`return ((1+2)+3)`) while SML uses IR temporaries (`_sv0t0 = (1+2); …`); the two compilers emit different C by design. **Still open:** `lib/main.sv0` is a **staging** boundary (no phase calls — enforced by `verify_m3_g6_staging_driver_contract.py`); the default `build/sv0-self-host-compiler` is still the SML **bootstrap delegate**; multi-module `linkProjectDir` and VM tier-2 native emission are not done.

## Why L0 is still open after G6/G7/G8/G9/GX

- **G9** (SML retirement orchestration) and **G8** (recipe paths + bootstrap **`SV0_SELF_HOST_COMPILER`** wrapper) are **slice gates**, not **L0**.
- **`build/sv0-self-host-compiler`** remains a **bootstrap delegate** around **`scripts/sv0-self-host-emit-c.sh`** until a **native** binary honors the same argv/stdout contract for the *whole compiler* (**`native-self-host-compiler-recipe.md`**). The per-file `driver.sv0` native binary exists (P2 below), but nothing yet composes the full multi-module compiler natively.
- **`lib/main.sv0`** is a **staging** driver module; **integrated** lexer→emit in **native** code requires the **deferred engineering** in **`doc/driver-pipeline-composition.md`** (**(A)** mega-TU **or** **(B)** multi-unit composition — ship **one** model).

## L0 prerequisites (criteria summary)

The gate slice tables (M3-S-001…M3-S-055) tracked *scaffolding and parity building blocks* — not functional end-to-end pipelines. The four open L0 prerequisites, with current state:

| # | Criterion | Current state | What's missing |
|---|-----------|--------------|----------------|
| **P1** | **Composed sv0 driver** — `main` in sv0 calls lexer→emit | `lib/driver.sv0` has the full pipeline (49 tests; self-host-loop 98/98). `lib/main.sv0` still has **no** phase calls | Wire `driver_compile_file` into `main.sv0` for `--emit-c` and `--target=vm`; drop the `parse_program`-absent invariant (Phase A) |
| **P2** | **Native `SV0_SELF_HOST_COMPILER` binary** ✅ | `build/sv0-driver-native` built from `driver.sv0` (SML→C→cc); CLI mode via `/tmp/.sv0_drv_path`; CI keeps SML default | Compose the *full* multi-module compiler natively (depends on P1) and wire native wrapper as CI default once parity is byte-clean |
| **P3** | **Semantic pipeline parity** — sv0 pipeline matches SML on L0 programs | self-host-loop 98/98 emit+cc+run **and** SML-diff clean for all `lib/*.sv0` seeds; `linkProjectDir` AST merge unimplemented; `check_program` single-unit; resolver/lowering tails | Per-file parse+arena relocation; multi-module checker; resolver/lowering tail cases (Phase C) |
| **P4** | **VM parity tier-2 (native emitter)** | Tier-2 harness uses a surrogate shell script, not sv0-emitted bytecode | Replace surrogate with sv0-emitted VM bytecode once the native compiler exists (Phase D) |

**P1 blocks P2** (full native compose), which **blocks P4**. **P3** is partially parallelizable with P1/P2 and is the furthest along (self-host-loop closure).

## Critical path (dependency order)

Work streams can overlap once upstream interfaces exist; order reflects **hard dependencies** for an honest **L0** claim. Phase labels map onto the criteria: **A → P1**, **B → P2 (full)**, **C → P3**, **D → P4**, **E → evidence**.

### Phase A — Composed sv0 driver (P1)

**Goal:** `fn main()` in sv0 runs a real lexer→emit pass on a file, for both `--emit-c` and `--target=vm`.

**Decision (2026-07-09): the composed driver is the self-contained single-TU `lib/driver.sv0`, not a `main.sv0` that calls the real modules.** Both ways to make `main.sv0` call `lexer.sv0`/`parser.sv0` directly are ruled out by the bootstrap model (standalone units; duplicate `fn main()` → E0302; `static` non-main fns block object linking — see `doc/driver-pipeline-composition.md` § Resolution). `driver.sv0`'s `main` (CLI mode via `/tmp/.sv0_drv_path`) already drives `drv_tokenize → drv_parse → drv_resolve/drv_check → drv_emit_c`, is proven at 98/98 native behavioral parity (`self-host-native-parity`), and phase-order regressions now fail CI (`verify_m3_g6_pipeline_contract.py` asserts the call order in `drv_compile_file`). `main.sv0` stays the naming/CLI staging module (contract keeps `parse_program` out of it). **Remaining in A for full L0:** `--target=vm` in the composed driver (folds into Phase D) and full-language feature parity (Phase C) — the *essential-subset* composition gate is met. The steps below (A1–A4) describe the alternative "wire into main.sv0" path, retained for context but **not** the chosen route.

**A1. Choose composition model** (decide before coding; record here + task Rmd):

- **(A) `include` mega-TU** — `lib/main.sv0` gains `include "lexer.sv0"` … `include "vm_codegen.sv0"` at the top; `IncludeExpand.expandFile` expands them before lexing (SML `main.sml` does this on every file). Expanded TU is large (~15k lines) but the compiler handles it. *Pro:* works with today's `include_expand.sv0`; single-file path to Done. *Con:* all-or-nothing expansion.
- **(B) multi-unit native C link** — each `lib/*.sv0` compiles to `.c` independently (already done by `bootstrap-build`); a new link step combines them + the C runtime. *Pro:* modules stay small, aligns with `lib/LAYOUT.md` / `sources.cm`. *Con:* new packaging rules — each emitted unit today has a full program shape, so naïve linking **duplicates** runtime/init assumptions. See **`doc/driver-pipeline-composition.md`** § Native closure.

**Engineering default:** prefer **(B)** for the durable artifact; an **(A)** spike is acceptable only to unblock an early third-leg `diff`, then refactor toward (B).

**A2. Implement `driver_compile_file`** in `lib/main.sv0` (or the mega-TU entry). Phase order mirrors `sml-legacy/main.sml` `compileProgram`:

```
1. tokenize — lexer.sv0     :: tokenize
2. parse    — parser.sv0    :: parse_program
3. resolve  — resolver.sv0  :: resolve_program
4. check    — checker.sv0   :: check_program
5. lower    — lowering.sv0  :: lower
6. emit     — codegen.sv0   :: emit          (backend = C)
              vm_codegen.sv0 :: emit_program  (backend = VM)
```

Inter-phase data (all already defined in their modules): lexer → `source: string`, `tok_tags/starts/ends: Vec<i32>`; parser → item + body/type arena vecs + `pp: Vec<i32>`; resolver → `name_env: Vec<i32>`; checker → `type_env` or return-code only; lowering → `block_labels/param_names/param_ctys/instrs`; emit → written to `out_path`.

**A3. Wire into `main()`** — parse CLI (`classify_cli` exists), dispatch to `driver_compile_file` (single file) or `driver_compile_project` (`--project`, needs C1); print diagnostics; exit with the phase's code. `--project` may return "not implemented" until C1 lands; single-file must be fully functional.

**A4. CI validation** — `./scripts/sv0 compile-run lib/link.sv0` still exit 0; `./scripts/sv0 test` green. **Update `verify_m3_g6_staging_driver_contract.py`**: it currently asserts `parse_program` is **absent** from `main.sv0`; flip it to assert `driver_compile_file` **is present**, and note the change in the refinement log.

### Phase B — Native binary, full compiler (P2)

**Native full-compose is the critical path** (unblocks Phase C validation + P2 + P4). See **`native-compose-tradeoffs.md`** (2026-07-23) for the concrete (A)-vs-(B) analysis: **recommend (A) mega-TU** — assemble the real modules into one TU by a build script (concat + strip the 18 test `main`s / 8 `test_*` / rename ~14 logic collisions) + a compose `main` threading the real phases; (B) multi-unit link is gated on a new `extern` language feature the compiler lacks. Evidence: 1651 fns / only 21 collisions; a 2-module concat compiles; `driver.sv0` proves the self-contained-TU shape at 98/98.

**B1. Native full-compose recipe — DONE (2026-07-27).** `scripts/build-sv0-megatu-native.sh` composes the **whole** multi-module compiler via the (A) mega-TU model: it derives a CLI compose main from `megaTU-main.sv0` (source ← `read_file` of the path in `/tmp/.sv0_drv_path`; emitted C → `/dev/stdout`), assembles the 18 real modules + that main, SML-compiles the mega-TU sv0 → C **once** (bootstrap), then `cc`s it into `build/sv0-megatu-native`, plus a `build/sv0-megatu-compiler-native` wrapper adapting `argv[1] → /tmp/.sv0_drv_path` (the `SV0_SELF_HOST_COMPILER` contract). The **built binary runs with no SML heap**. It passes **behavioral parity 98/98** (emit + cc + run exit 0) over `self-host-sv0-loop.list`. This is the "composes the full multi-module compiler natively" milestone that only `driver.sv0` (single self-contained unit) had reached before.

**B2. Byte-identical vs SML is NOT the bar — MEASURED (2026-07-27).** A spike compared the composed compiler's C against the SML heap C on all 98 seeds: only **40/98 byte-identical** (all trivial arithmetic/control-flow programs — `hello`, `println_ok`, the `vm_*` micro-tests); the other **58 differ**, and on the real library modules the churn is **~109% of SML lines** (e.g. `parser` 113%, `lowering` 129%, `lexer` 99%, `unify` 103%). The composed emit diverges from SML by design (temp naming, expression structuring, decl ordering) — the same divergence noted for `driver.sv0`-native. So `self-host-sv0-loop`'s `diff -u <SML-ref> <SHC>` leg (which the SML *delegate* passes trivially) is the wrong acceptance surface for the native composed compiler; forcing byte-identity would mean re-implementing `megatu_emit` to reproduce SML's exact C, at no functional benefit (both compile+run identically). **Acceptance = behavioral parity**, already enforced by `sv0-megatu-corpus-parity.sh` in CI (98/98 emit+cc+run) and re-proven here for the native binary.

**B3. CI wiring — DONE (2026-07-27).** The behavioral gate is in `ci.yml` (corpus-parity step, floor 98). Added `scripts/sv0-megatu-native-parity.sh` (builds the native compiler, then emit-twice-determinism + cc + run over the corpus via `build/sv0-megatu-compiler-native`, **no SML at runtime**, 98/98) and wired it into `self-host-native.yml` after the mega-TU `--check` step. Promoting the native binary to the *default* `SV0_SELF_HOST_COMPILER` remains deferred: that default feeds the `self-host-sv0-loop` byte-diff-vs-SML leg (scripts/sv0:520–546), which the composed emit cannot pass (spike), so it needs either a behavioral-mode loop flag or accepting the divergence — a separate decision from shipping the recipe.

### Phase C — Semantic parity gaps (P3, parallel with A/B)

Each item closes a gap between the sv0 pipeline and SML `compileProjectDir`. For each: **write a failing test first, then fix.**

- **C1. `linkProjectDir` AST merge** — SML `link.sml :: linkProjectDir` parses each file, runs `mapProgramUnit`, and concatenates the `Ast.program` lists. Building blocks exist in `lib/link.sv0` (`link_apply_map_link_pass_program_source`, `link_merge_parallel_token_streams_reloc_b`, `link_program_item_vecs_append`, `link_project_concat_sources_offsets_from_listing`); **missing** is the orchestration loop. Write `link_project_dir_from_listing(listing, offsets, merged_source, merged_starts, merged_ends, out_item_*, out_body_*, out_pty_*) -> i32` that, per file, calls `parse_program` on the source slice, relocates arena indices by the per-file offset (`link_reloc_i32_vec_inplace`), runs the mangle pass, and appends into the output arenas. *Test:* two-file project (A defines `fn Foo()`, B calls it) → merged item arena has `link__Foo` and the call site references it. Refs: **`doc/link-g6-blockers.md`** (M3-S-040), **`doc/transliteration-plan.md`** Link row.
- **C2. Resolver gaps** (`lib/resolver.sv0`) — `TyArray` size-expr resolution (tag 5); enum variant-constructor alias plumbing for payload variants; impl method bodies resolved independently (cross-check SML `NameResolution.resolveTopItem` `ItemEnum` / impl).
- **C3. Multi-module `check_program`** (`lib/checker.sv0`) — single-unit today; after C1 merges arenas it should work, provided C2 populated the name env. *Test:* A defines struct `Foo`, B uses it in a signature → checker resolves `Foo` to `link__Foo` via the merged name env.
- **C4. Lowering tail cases** (`lib/lowering.sv0`) — `PatStruct` binds with nested field patterns (vs SML `Lower.lowerPat`); `match_scrut_cty` resolving 1-segment local/param scrutinees to the precise enum typedef instead of coarse "pointer" (thread the fn/param type table into `lower_tag_match`).
- **C5. `include_expand.sv0` host I/O** — confirm `expand`/`expandFile` actually call `read_file` in the bootstrap context; test on a real multi-file `include`. Needed for the (A) mega-TU option.

**Composition constraint (2026-07-23):** the full parse-and-merge orchestration for C1 **cannot live in a standalone `lib/*.sv0` unit** — `link.sv0` can neither reference `tokenize`/`parse_program` (unbound across units) nor `include` `lexer.sv0`/`parser.sv0` (each defines `fn main` → E0302). This is the same standalone-unit wall P1 hit, so end-to-end `linkProjectDir` parity is **gated on the composed pipeline** (Phase A native full-compose). Until then, the C1 sv0-side work is a merge-of-pre-parsed-programs primitive with tag-aware arena relocation, exercisable only on hand-built arenas (the established `link.sv0` test pattern); the **real** acceptance surface is driven through the SML `--project` pipeline.

**Coverage + known cross-module gaps (via SML `--project`):** cross-module **fn calls** work — `test/integration/modules`, `import_use_match`, and (2026-07-23) `test/integration/modules_types` (cross-module fns wrapping intra-module struct construction + field access + enum construction + match; `linkProjectDir` mangles `lib__Point`/`lib__Color`/`lib__*`; exit 42). Two concrete cross-module **user-type** gaps in the reference pipeline remain (fixtures reproduce them): (1) a `let p: T = …` where `T` is a **use-imported** struct emits `int p = …` instead of `lib__T p = …` — the local's C type loses the cross-module alias (codegen/lowering local-type resolution); (2) a cross-module enum in a `match` fails type-check with `E0400`. These pin C2/C3/C4 acceptance: they must compile+run once the alias/type-table threading matches SML's within-module behavior.

**Acceptance:** `./scripts/sv0 test` green **without** `SV0_SKIP_SELF_HOST_COMPILER_DIFF` when claiming semantic closure (unless a documented intermediate — `milestone-orientation.json` `pre_merge_validation`); **`doc/transliteration-plan.md`** § Feature + diagnostic parity rows flip to **Done** only when the milestone says so.

### Phase D — VM parity tier-2 native (P4)

**Goal:** a **native** sv0-built VM emitter that, given a `.sv0` path, writes `build/vm/<stem>.sv0b` **byte-identical** to the SML golden (`sv0c/test/vm-parity/golden/sml/<stem>.sv0b`). It replaces the surrogate `scripts/sv0-vm-tier2-emit-bootstrap.sh` (which shells out to the SML heap `--target=vm`) as the `SV0_VM_BYTECODE_EMITTER`.

**The bar is stricter than P3.** P3's C third leg accepts **behavioral** parity (native and SML emit different C that runs the same — see `self-host-native-parity`). P4 must be **byte-exact**: the harness does `cmp -s built golden` (`run_vm_parity_tier2_emit_compare` in `scripts/sv0`), because `.sv0b` is a binary format the VM loads verbatim (magic `SV0B`, `bytecode.sv0` `magic_byte_0..3` = 83/86/48/66). There is no "behaviorally equivalent bytecode" fallback — the emitter must reproduce SML's exact byte layout: opcode encoding and instruction sizes (`bytecode.sv0 insn_encoded_size` / `opcode_has_{i32,u32}_payload`), constant/string pool **ordering and index assignment**, function-table entry format, and integer endianness/padding. Spec: `sv0doc/bytecode/format.md` + `instructions.md`; reference: `sv0vm/src/bytecode/bytecode.sml` and `sml-legacy/backend/vm/vm_codegen.sml`.

**Assets that already exist:** `lib/vm_codegen.sv0` (~3050 lines) and `lib/bytecode.sv0` (~1370 lines) are transliterations of the VM backend + encoder, and both pass C behavioral parity in the self-host loop. **But passing the loop only proves their C-compiled *self-tests* run — not that, wired as the compiler's VM backend, they emit byte-identical `.sv0b` on real programs.** That is the open P4 work.

**Dependency:** P4 needs the lower→`vm_codegen`→`bytecode`-encode path runnable natively. Two routes: **(i)** ride **P1/Phase B** — once the composed driver exists, add `--target=vm` to it; or **(ii)** a dedicated self-contained `vm_driver.sv0` (mirroring today's `driver.sv0` C-emit unit) that composes lower + `vm_codegen` + encode and writes `.sv0b` in CLI mode. **(i)** is preferred (one driver, no second self-contained reimplementation to maintain); **(ii)** is a fallback spike if P1 slips.

**Ordered steps:**

- **D0. Byte-diff instrumentation — DONE (2026-07-27).** `scripts/sv0b-hexdiff.sh <ref> <cand>` reports byte-identical or the first divergence as an offset + SV0B-section guess (parses magic/version/`str_section_len` from the header; sections: magic / version / str-len / string-section@N / function-table-code@M). Verified on the goldens (`span.sv0b`: version 1, str_section_len 644).
- **D1a. Binary-write builtin `write_bytes` — DONE (2026-07-27).** Confirmed the blocker: `sv0_write_file` writes via `strlen`, truncating `.sv0b` at the first NUL (span golden = 4565/7834 NUL). Added `write_bytes(path: string, bytes: Vec<i32>)` — raw binary write of a vec's byte values, NUL-safe. Runtime `sv0_write_bytes` is **`static inline` in `sv0_runtime.h`** (NOT a `.c` function): the vec table is `static` per-TU, so a `.c` definition reads runtime.c's empty table, not the emitted program's (found + fixed — direct C test showed `vec_len=5` in main but 0 inside the `.c` fn). Registered as builtin **id 18** (lowering `build_builtin_map` + `is_void`; resolver `is_intrinsic`/`intrinsic_arity`; checker `builtin_fn_id`=16/`ret_type`=UNIT/`param_count`=2/`param_type`=string,Vec; emit `megatu_builtin_name`). Verified: the composed compiler emits `sv0_write_bytes(path, b)` and a NUL-containing vec writes correctly (`53 00 56 00 42`). Corpus stays 98/98 (additive). Refreshed lowering/resolver/checker goldens.
- **D1b. VM compose main — RECIPE BUILDS + RUNS; BLOCKED on IR-ADT drift (2026-07-31).** `scripts/build-sv0-megatu-vm-native.sh` derives a VM compose main from `megaTU-main.sv0` (reusing phases 1-5 + `megatu_find_item_by_label`/`megatu_type_root_name_tok`), replacing the phase-6 C emit with the VM tail: bridge `out_blocks` (interleaved boxed quads, params EMPTY) → `emit_program`'s 4 parallel vecs (labels, `box_deref` instrs, per-fn param name/cty vecs reconstructed from the parse arenas) → `vm_codegen_emit_program` → `bytecode.encode_strings` → `encode_file` → `write_bytes("/dev/stdout", out)`. **Two blockers cleared:** (1) the mega-TU is compiled by the SML reference, which didn't know `write_bytes` → added it to `sml-legacy` (resolver `registerFnArity` arity 2; lowering `sv0_write_bytes` void call; checker `(string, Vec<i32>) -> unit`), mirroring `write_file`. Full `sv0 test` green (additive). (2) The assembler namespaces `vm_codegen`'s `emit_program` → **`vm_codegen_emit_program`** (collides with `codegen`'s); bare `emit_program` binds to the C backend's (wrong signature) → call the namespaced name. The native binary `build/sv0-megatu-vm-native` now builds and runs. **REMAINING BLOCKER (the real D1b finding): lowering's and vm_codegen's IR ADTs have DRIFTED.** `Instr` matches (tags 0-15 identical), but the nested `Expr`/`Value` do not: `Expr` — lowering `Load=1,Binop=2,Unop=3` vs vm_codegen `Binop=1,Unop=2,Load=3` (skewed); `Value` — lowering `VBoolTrue=1,VBoolFalse=2,VVar=3,…,VString=5` vs vm_codegen `VFloat=1,VBool(bool)=2,VString=3,…,VVar=5` (skewed AND structurally different — two bool variants vs one `VBool(bool)`, plus vm-only `VFloat`/`IndexAccess`). So `emit_instrs` misreads lower's boxed sub-exprs → `vec: index out of bounds` panic on a trivial `fn main()->i32{return 2+3;}`. In the SML reference there is ONE shared `Ir` feeding both backends; the sv0 per-module split let them diverge — **that drift is the bug.** Two fix paths (deferred, user chose to pause + commit): **(B, recommended)** converge vm_codegen's `Expr`/`Value` to lowering's (matching SML's single-Ir design) — direct data path, test-guarded, touches `vm_codegen.sv0` (golden refresh); vm_codegen's `VFloat`/`VBool`/`IndexAccess` arms mostly `return -1` so they are not load-bearing for the lowering surface. **(A)** an additive recursive re-tagging translator in the compose main — no golden churn but ~150 lines of fragile hand-written cross-ADT recursion with no isolated test. After the bridge: native recipe already writes `.sv0b` to stdout; wire `build/vm/<stem>.sv0b` + the `SV0_VM_BYTECODE_EMITTER` contract, then D2.
- **D2. Byte-exact convergence on the 4-program tier-2 manifest.** Iterate against `cmp -s` vs golden. Expected mismatch classes: pool ordering/index assignment, string encode byte order, function-table entry layout, opcode payload width, trailing padding. Fix in `lib/bytecode.sv0` / `lib/vm_codegen.sv0` (keep C behavioral parity green — `self-host-native-parity` must not regress).
- **D3. Widen** the tier-2 manifest toward the full 97 goldens as programs go `cmp`-clean; `verify_vm_parity_tier2_policy.py` keeps `tier2-manifest ⊆ manifest` and golden presence enforced.
- **D4. Replace the surrogate.** Point `SV0_VM_BYTECODE_EMITTER` at the native binary by default; update `scripts/sv0-vm-tier2-emit-bootstrap.sh` (or retire it) and run `run_vm_parity_tier2_emit_compare` in CI **without** the SML-heap surrogate.
- **D5. CI + evidence.** Add a native-VM-parity step to `self-host-native.yml` (alongside the P3 `self-host-native-parity` step) and record it in the Phase E "VM parity v1" evidence row.

**Acceptance:** `SV0_VM_BYTECODE_EMITTER=<native> ./scripts/sv0 test` passes the tier-2 `cmp` for every manifest entry with **no** SML-heap fallback; `tier2-manifest.txt` covers the intended program set; policy guard green.

### Phase E — M3 completion declaration (evidence)

Map harness outputs, recipe commands, and recovery paths to the task Rmd **## completion criteria**; obtain **closure_authority** review per `milestone-orientation.json`. All rows must be satisfied **simultaneously**:

| Evidence | How to satisfy |
|----------|---------------|
| Full pipeline in sv0 | `driver_compile_file` compiles all `self-host-sv0-loop.list` programs, byte-identical C to SML |
| Self-compile (native) | `build/sv0-self-host-compiler` produced by the **sv0** pipeline (not SML) and compiles sv0 sources correctly |
| VM parity v1 | all tier-2 programs `cmp`-clean vs `golden/sml/` using the **native** emitter |
| Diagnostics baseline | `verify_diagnostics_corpus_behavior.py` passes |
| SML retired | `bootstrap-sml-final` tag exists, `sml-legacy/` present, default build is sv0-only |

Flip **`## stakeholder closure checklist`** rows from No/Partial to Yes/Done with **named** commands/artifacts; only then update `README.md` and `task/sv0-toolchain-milestone-3-checklist.Rmd`. Do **not** flip the owning task `state:` on harness green alone.

## Key files (current state)

| File | Current state | L0 role |
|------|--------------|---------|
| `lib/driver.sv0` | Full lex→parse→resolve→check→emit; 49 unit tests + self-host-loop 98/98 | Phase A source of the composed pipeline / P2 native binary source |
| `lib/main.sv0` | Constants + CLI staging; **no** phase calls | Phase A target: add `driver_compile_file` wiring |
| `lib/link.sv0` | Map passes + merge primitives done; orchestration loop missing | Phase C1: `link_project_dir_from_listing` |
| `lib/resolver.sv0` | Mostly complete; TyArray/enum/impl gaps | Phase C2 |
| `lib/checker.sv0` | Single-unit; multi-module after C1 merge | Phase C3 verify |
| `lib/lowering.sv0` | `PatStruct` / `scrut_cty` tail cases | Phase C4 |
| `lib/include_expand.sv0` | `expand`/`expandFile` present | Phase C5 verify; Phase A1 (option A) |
| `scripts/build-sv0-self-host-compiler.sh` | Builds per-file `driver.sv0` native binary | Phase B1: compose the full compiler natively |
| `scripts/sv0-vm-tier2-emit-bootstrap.sh` | Uses SML heap to emit bytecode | Phase D: replace with native |
| `.github/workflows/self-host-native.yml` | Workflow exists (manual dispatch) | Phase B3: promote / drop skip flags |
| `scripts/verify_m3_g6_staging_driver_contract.py` | Asserts `parse_program` **absent** from `main.sv0` | Phase A4: flip to assert `driver_compile_file` present |

## Highest-leverage next actions

1. Confirm option **A vs B** in `doc/driver-pipeline-composition.md` (does `include_expand.sv0` call `read_file`; does the bootstrap compiler expand `include` before parsing?).
2. If (A) is viable, write `driver_compile_file` in `lib/main.sv0` and get `./scripts/sv0 compile-run lib/main.sv0` to run a full lexer→emit pass on a test file (this is P1's core).
3. In parallel, start **C1** (`link_project_dir_from_listing`) — an orchestration loop over existing primitives, not new algorithm work.

## Validation ladder (agents)

Narrow → wide (see **`.cursor/rules/40-validation-and-proof.mdc`**):

1. **`./scripts/sv0 test-guards`**
2. Targeted **`./scripts/sv0 vm-compile`** / **`compile-run`** / **`emit-c`** for touched **`.sv0`**
3. **`./scripts/sv0 self-host-sv0-loop`** with **`SV0_SELF_HOST_COMPILER`** pointed at the artifact under test
4. **`./scripts/sv0 self-host-native-parity`** — native driver ↔ SML **behavioral** parity (emit+cc+run, identical stdout+exit) on the whole seed list; the principled native third-leg check
5. **`./scripts/sv0 test`** before claiming integration closure

## Related

- **`doc/native-self-host-compiler-recipe.md`** — interface contract + bootstrap vs forward path
- **`doc/native-compose-tradeoffs.md`** — (A) mega-TU vs (B) multi-unit link, evidence + recommendation (2026-07-23)
- **`doc/driver-pipeline-composition.md`** — (A)/(B), staging vs native
- **`doc/self-host-sv0-loop.md`** — pilot loop semantics
- **`task/sv0-toolchain-milestone-3-self-host.Rmd`** — **L0** prerequisites table + ordered execution steps
