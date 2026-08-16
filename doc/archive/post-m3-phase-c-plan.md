# Post-M3 hardening — Phase C task plan (whole-language feature parity)

**Context.** M3 stakeholder completion criteria are **met** (closure 2026-08-05,
`m3-closure-evidence.md`) on the **corpus surface**. This plan decomposes the
**post-M3 hardening** — `l0-closure-roadmap.md` **Phase C** (whole-language
feature parity) + the deferred **native-default compiler promotion** (Phase B3) —
into small, dependency-ordered, individually-shippable tasks.

> **STATUS (2026-08-12).** **All numbered PC slices below are DONE** — Epics 1–6
> (cross-module E1/E2/E3, standalone gaps PC-4a/4b/4c, include E5, native-default
> behavioral-mode loop E6). Live per-slice status: `task/sv0-toolchain-milestone-3-checklist.Rmd`
> (PC-### rows). **PC-7 — the §5e checker forward-scan blocker — is RESOLVED**
> (2026-08-11), so genuine cross-module name-collision *mangling* is unblocked at the
> checker (see `pc3b-linkprojectdir-scoping.md` §5j). **Still open:** (a) the
> **native-default promotion** (Epic 6 tail — flipping the local
> `SV0_SELF_HOST_COMPILER` default) is gated on the native-completeness bugs below;
> (b) **bug-hunt remediation** — `bug-hunt-findings.md` catalogs 13 correctness
> findings (4 P1), of which **BH-1** (native method-call compound args) and **BH-2**
> (VM i32 arithmetic) are fixed; the native-completeness cluster (#8 checker
> under-diagnoses, #10 dropped contracts, #11 `?`/enum-ret/tuple/nested-struct
> mis-emit) is the real blocker for making native the default front end. Rollup:
> `task/sv0-toolchain-progress.md` ## Post-M3 hardening status.

**Standing discipline (applies to every task):**
- **Test-first.** Write the failing test (fixture) before the fix.
- **Acceptance surface.** Cross-module behavior is driven through the **SML
  `--project`** pipeline (the reference), plus `./scripts/sv0 test` green
  **without** `SV0_SKIP_SELF_HOST_COMPILER_DIFF` when claiming semantic closure.
- **Guardrails that must not regress:** C corpus **98/98**
  (`./scripts/sv0-megatu-corpus-parity.sh`), VM byte-parity **18/18**
  (`./scripts/sv0 vm-parity-tier2-emit`), full `./scripts/sv0 test`.
- After editing `resolver.sv0` / `checker.sv0` / `lowering.sv0` / `parser.sv0` /
  `vm_codegen.sv0`: run `./scripts/sv0 test` **bare**, then refresh goldens
  (`self-host-capture-stage0` → `lib/golden/stage0/`; `capture_vm_parity_goldens.sh`).
- **Never** use `result` as a local var in sv0 (reserved; silent SML crash).
- Mechanical/well-scoped tasks are marked **[delegable]** (hand to a cheaper
  model); design/threading tasks are **[design]** (keep in-house).

**Sizing:** S = <½ day focused, M = ~1 day, L = multi-day / needs a spike.

---

## Epic 1 — Cross-module imported-struct local type (reference Gap 1)

**Symptom (reference pipeline, via SML `--project`).** A `let p: T = …` where
`T` is a **use-imported** struct emits `int p = …` instead of `lib__T p = …` —
the local's C type loses the cross-module alias. Fixture base:
`test/integration/modules_types/`.

| ID | Task | Files | Size | Notes |
|----|------|-------|------|-------|
| **PC-1a** | Lock a **failing** fixture: two-module project, module B does `let p: A::Point = …; p.x`; assert C emits `lib__Point p` and it compiles+runs (exit code check). | `test/integration/modules_types/` (extend) | S | [delegable] test-only; reproduces the gap |
| **PC-1b** | Resolver: register **use-imported struct type aliases** so `T` resolves to `lib__T` in a local's declared type (cross-check SML `NameResolution` import handling). | `lib/resolver.sv0` | M | [design] alias plumbing; feeds PC-1c |
| **PC-1c** | Lowering/codegen: local declared-type resolution uses the cross-module alias — `let p: T` → `DeclNamed(lib__T, p)` not `int`. Cross-check `ast_ty_to_c_string_with_user` + `expr_init_cty`. | `lib/lowering.sv0`, `lib/codegen.sv0` | M | [design] |

**Epic acceptance:** PC-1a fixture compiles+runs via SML `--project`; C corpus
98/98; goldens refreshed.

---

## Epic 2 — Cross-module enum in `match` (reference Gap 2)

**Symptom.** A cross-module enum used in a `match` fails type-check with
**E0400**. This pins the resolver enum-variant alias + checker + lowering
`match_scrut_cty` threading.

| ID | Task | Files | Size | Notes |
|----|------|-------|------|-------|
| **PC-2a** | **DONE (2026-08-05).** Fixture `test/integration/modules_enum_match/`: `lib` defines `enum Signal { On(i32), Off() }` + `make()`, importer `match`es on it. Reproduces `E0400: type mismatch` via `--project`; within-module control compiles → gap is cross-module. Not yet in the pass harness (fails to compile). | `test/integration/modules_enum_match/` | S | [delegable] test-only |
| **PC-2b** | Resolver: **enum variant-constructor alias plumbing** for payload variants — register per-variant qualified names (`A::Color::Green`), not just the enum type name (today's gap, `resolver.sv0` ~L788). | `lib/resolver.sv0` | M | [design] feeds PC-2c/2d |
| **PC-2c** | Lowering: thread the **fn/param type table** into `lower_tag_match` so `match_scrut_cty` resolves 1-segment local/param scrutinees to the precise enum typedef (today it passes empty tables). | `lib/lowering.sv0` | M | [design] the `match_scrut_cty` threading called out in the roadmap |
| **PC-2d** | Checker: multi-module `check_program` resolves the cross-module enum + variant so the `match` type-checks (clear E0400) with the merged/aliased name env. | `lib/checker.sv0` | M | [design] depends on PC-2b |

**Epic acceptance:** PC-2a fixture type-checks + runs via SML `--project`; no E0400.

---

## Epic 3 — `linkProjectDir` AST merge + multi-module checker (C1, C3)

**Now unblocked:** the composition constraint (needs the composed pipeline) is
satisfied by the **native mega-TU** (M3 Phase B). The building blocks exist in
`link.sv0`; the orchestration loop is missing.

| ID | Task | Files | Size | Notes |
|----|------|-------|------|-------|
| **PC-3a** | Hand-built-arena **failing test** for `link_project_dir_from_listing` (two pre-parsed programs; assert merged item arena has `link__Foo` and B's call references it). | `lib/link.sv0` tests | S | [delegable] follows the established `link.sv0` test pattern |
| **PC-3b** | Implement `link_project_dir_from_listing(listing, offsets, merged_source, …) -> i32`: per file, `parse_program` on the slice, relocate arena indices by per-file offset (`link_reloc_i32_vec_inplace`), run the mangle pass, append into output arenas. | `lib/link.sv0` | L | [design] the orchestration loop; refs `doc/link-g6-blockers.md` |
| **PC-3c** | Multi-module `check_program` over the merged arenas: A defines `struct Foo`, B uses it in a signature → checker resolves `Foo`→`link__Foo` via the merged name env (depends on PC-3b + Epic-2 resolver). | `lib/checker.sv0` | M | [design] |

**Epic acceptance:** two-file project parses→merges→checks; end-to-end via SML
`--project` matches SML `linkProjectDir`.

---

## Epic 4 — Standalone resolver/lowering gaps (parallelizable, mostly mechanical)

Independent of the cross-module epics; each is a self-contained parity gap.

| ID | Task | Files | Size | Notes |
|----|------|-------|------|-------|
| **PC-4a** | **RECLASSIFIED low-priority (2026-08-05, grounding).** Arrays are **unused** in the compiler corpus (only a parser literal-length test); the flat arena stores the length as a single **token** (`data2=len_tok_pos`), not an expr, so faithful parity vs SML `TyArray (t2,e,_) => (resolveTy t2; resolveExpr e)` needs parser enrichment + threading `tags`+`mod_vals` through `resolve_ty`'s **17** call sites. Not the small warm-up assumed — defer. | `lib/resolver.sv0`, `lib/parser.sv0` | M | [design, low value] |
| **PC-4b** | **Lowering `PatStruct` nested field patterns** — bind with nested field patterns vs SML `Lower.lowerPat` (`lowering.sv0` ~L2015). Failing test: `match s { S { a: Inner { … } } => … }`. | `lib/lowering.sv0` | M | [design] |
| **PC-4c** | **Resolver impl method bodies** resolved independently — cross-check SML `resolveTopItem` `ItemEnum`/impl. Failing test: an `impl` block with a method referencing `self` fields. | `lib/resolver.sv0` | M | [design] |

**Epic acceptance:** each failing test passes; C corpus 98/98; goldens refreshed.

---

## Epic 5 — `include_expand` host I/O (C5)

| ID | Task | Files | Size | Notes |
|----|------|-------|------|-------|
| **PC-5a** | Confirm `expand`/`expandFile` actually call `read_file` in the bootstrap context; add a **real multi-file `include`** test. | `lib/include_expand.sv0`, test | S | [delegable] verification + one fixture; needed for the (A) mega-TU option |

---

## Epic 6 — Native-default compiler promotion (Phase B3 deferred) — ✅ COMPLETE (2026-08-15)

Promote `build/sv0-self-host-compiler` from the SML **bootstrap delegate** to the
**native** mega-TU binary. Was blocked on an **acceptance-surface decision**:
the `self-host-sv0-loop` byte-diff-vs-SML leg is passed trivially by the SML
delegate but **cannot** be passed by the native composed emit (behavioral, not
byte-identical — by design, measured in B2: only 40/98 byte-identical, ~109%
line churn on real modules), plus the **native-completeness cluster** (#8/#10/#11).

| ID | Task | Files | Size | Notes |
|----|------|-------|------|-------|
| **PC-6a** | **[DECISION]** Choose the promotion path: (i) add a **behavioral-mode** flag to the self-host loop (skip the byte-diff leg, keep emit+cc+run parity), or (ii) accept + document the divergence. Recommend (i). | — | S | ✅ DONE — chose (i) behavioral-mode |
| **PC-6b** | Behavioral-mode flag (`SV0_SELF_HOST_LOOP_MODE=behavioral`) + native CI gate in `self-host-native.yml`. | `scripts/sv0`, workflows | M | ✅ DONE (2026-08-05) — 98/98 behavioral |
| **PC-6c** | **The flip.** Gated on the native-completeness cluster (#8/#10/#11, all closed 2026-08-15). `run_self_host_sv0_loop` now defaults `SV0_SELF_HOST_COMPILER` → `build/sv0-megatu-compiler-native` + behavioral mode when unset (`ensure_sv0_megatu_native` builds it). The **SML byte-guards stay pinned to the SML delegate**: the stage0 golden gate emits via the delegate in a subshell, and the loop's reference leg uses the SML heap directly. `run_test` no longer pre-exports the delegate, so the flip reaches main CI (`./scripts/sv0 test`) automatically. Escape hatch: export `SV0_SELF_HOST_COMPILER=build/sv0-self-host-compiler` (+ `SV0_SELF_HOST_LOOP_MODE=bytediff`) for the old SML byte-diff path. | `scripts/sv0` | M | ✅ DONE (2026-08-15) — parent-only; full gate + escape hatch green |

---

## Recommended execution order

*(Updated 2026-08-05 after grounding: PC-4a deprioritized — see its row.)*

1. **Epic 2** (**PC-2a ✅** → 2b → 2c → 2d) — cross-module enum match; highest
   feature-parity value; failing fixture already in place.
2. **Epic 1** (PC-1a → 1b → 1c) — imported-struct local type; shares resolver
   alias work with Epic 2.
3. **Epic 3** (PC-3a → 3b → 3c) — the native multi-module link path (larger).
4. **PC-4b, PC-4c, PC-5a** — parallelizable cleanup, delegable in parts.
5. **Epic 6** (PC-6a decision → 6b) — promotion, once the pipeline gaps close.
6. **PC-4a** (TyArray) — low-priority; only when a real array-length use case appears.

**Milestone marker:** when Epics 1–3 land, the two reference cross-module gaps
compile+run and `transliteration-plan.md` § Feature + diagnostic parity rows can
flip to **Done** — the substantive "whole-language feature parity" claim behind
`l0-closure-roadmap.md` Phase C.

## Task-creation checklist (per task)

Each spun-out task should carry: the **failing test** to add first; the exact
**files/functions**; the **SML reference** (`sml-legacy/…`) to cross-check; the
**acceptance command**; the **goldens** to refresh; and the **guardrail re-run**
(C corpus 98/98 + VM 18/18 + `./scripts/sv0 test`).
