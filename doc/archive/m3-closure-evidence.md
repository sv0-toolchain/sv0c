# M3 stakeholder closure — evidence mapping

**Purpose.** Map each `## completion criteria` bullet in
`task/sv0-toolchain-milestone-3-self-host.Rmd` to reproducible **commands**,
**artifacts**, and **CI gates**, with a **verified status**, as the input to
`milestone-orientation.json` **closure_authority** review (M3 authority =
"Stakeholder completion criteria … feature parity, VM policy, native self-host,
SML retirement — not pilot list growth alone").

**Closure authority ruling (2026-08-05).** Reviewed against this map, the M3
stakeholder authority accepted **all five** completion criteria:
- **Row 1** closes on the **corpus** evidence (98/98 C behavioral + 18/18 VM
  byte-identical) as "feature parity on the agreed surface." Whole-language
  feature parity (Phase C) continues as **post-M3** hardening, not an M3 blocker.
- **Row 2** accepts the documented **native-once-bootstrap** mega-TU recipe
  (SML→C→cc once, then SML-free, 98/98) + cold path as "self-compile." Default
  `SV0_SELF_HOST_COMPILER` promotion continues as **post-M3**.

With that ruling, the `## stakeholder closure checklist` rows are flipped to
Yes/Done and the M3 stakeholder completion criteria are **met**. Harness-green
alone did not close these — this explicit authority decision did.

**Snapshot:** 2026-08-05 (sv0c `06017cf` / parent `db91c3a`).

## Summary

| # | Completion criterion | Status |
|---|----------------------|--------|
| 1 | full pipeline in sv0 | **DONE** — corpus-complete (98/98 C + 18/18 VM, no SML); feature parity = post-M3 Phase C |
| 2 | self-compile | **DONE** — native-once-bootstrap recipe + 98/98 SML-free compose + cold path; default-native promotion = post-M3 |
| 3 | VM strict parity v1 | **DONE** — 18/18 byte-identical via native emitter, push/PR gate |
| 4 | diagnostics | **DONE (v1 behavioral bar)** — strict TextIO sinks deferred |
| 5 | SML retirement | **DONE (G9)** — tag + `sml-legacy/` + sv0-only default |

All five criteria met per the closure-authority ruling above. Post-M3 hardening
(Phase C semantic feature parity; default-native compiler promotion) is tracked
in `l0-closure-roadmap.md` and does not reopen M3.

## Row-by-row evidence

### 1. full pipeline in sv0
> lexer → parser → name resolution → type check → contracts → IR → C backend and
> VM backend; feature parity with SML on the agreed surface and semantics.

- **Capability — DONE on the agreed corpus.** The native mega-TU compiler
  composes all 18 `lib/*.sv0` modules and runs the whole pipeline with **no SML
  at runtime**.
  - Command: `bash scripts/build-sv0-megatu-native.sh` → `build/sv0-megatu-native`
    (+ `build/sv0-megatu-compiler-native` wrapper).
  - C backend: **98/98** behavioral parity — `./scripts/sv0-megatu-native-parity.sh`
    (emit-twice-determinism + cc + run over the corpus, no SML). CI: `ci.yml`
    corpus-parity (floor 98); `self-host-native.yml`.
  - VM backend: **18/18** byte-identical — see row 3.
- **Remaining for full closure.** "Feature parity with SML on the agreed surface
  **and semantics**" — full-language feature parity (multi-module
  `linkProjectDir` AST merge, resolver `TyArray`/enum/impl tails, multi-module
  checker, lowering `PatStruct`/`scrut_cty` tails) is **Phase C** (open). The
  curated corpus surface passes; whole-language parity does not yet.
- **Status: Partial** — corpus-complete pipeline in sv0; feature parity = Phase C.

### 2. self-compile
> documented recipe: sv0 compiler builds sv0 sources end-to-end; cold path from
> `bootstrap-sml-final` / `sml-legacy/` documented until optional binary-only
> bootstrap exists.

- **Recipe — DONE.** `scripts/build-sv0-megatu-native.sh` composes the whole
  multi-module compiler (mega-TU), SML→C→`cc` **once** (bootstrap), producing a
  native binary that runs with no SML. Docs: `doc/native-self-host-compiler-recipe.md`,
  `doc/archive/native-compose-tradeoffs.md`.
- **End-to-end build of sv0 sources — VERIFIED.** Native binary emits+cc+runs the
  corpus at **98/98** (`./scripts/sv0-megatu-native-parity.sh` →
  `PASS=98 FAIL=0 … no SML at runtime`, verified 2026-08-05; per-file
  `driver.sv0` leg at 98/98 via `./scripts/sv0 self-host-native-parity`).
- **Cold path — DONE.** `bootstrap-sml-final` tag on sv0c + `sml-legacy/` present;
  recovery documented (G9).
- **Remaining for full closure.** Default `build/sv0-self-host-compiler` is still
  the **SML bootstrap delegate**; promoting the native binary to the default
  `SV0_SELF_HOST_COMPILER` is deferred (the default feeds the self-host-loop's
  byte-diff-vs-SML leg, which the composed native emit cannot pass by design —
  see `l0-closure-roadmap.md` Phase B3). Parity of the native compose is
  **behavioral, not byte-identical** (native inlines expr trees; SML uses IR
  temps).
- **Status: Substantially met** — recipe + native compose + 98/98 + cold path;
  default-native promotion is a stakeholder call, not a technical blocker.

### 3. VM strict parity v1 — **DONE**
> curated ~50–100 programs, SML golden bytecode, sv0 matches except allowlist.

- **All 18 mega-TU compiler modules are byte-identical** to their SML
  `--target=vm` goldens via the native emitter (no SML-heap surrogate).
  - Command: `./scripts/sv0 vm-parity-tier2-emit` → `18 file(s) matched golden/sml`.
  - Emitter: `build/sv0-megatu-vm-native` (`build-sv0-megatu-vm-native.sh`), wrapped
    by `scripts/sv0-vm-tier2-native-emitter.sh` (the default `SV0_VM_BYTECODE_EMITTER`).
  - Policy guard: `verify_vm_parity_tier2_policy` — `18 tier-2 path(s)`
    (`tier2-manifest ⊆ manifest`, goldens present).
  - CI gate: `.github/workflows/vm-parity-tier2.yml` on **push/PR/dispatch**.
- Driver — the last module — closed via `prepool_loop_seq` in `vm_codegen.sv0`
  (see `l0-closure-roadmap.md` Phase D / `[[project_driver_poolorder]]`).
- **Status: DONE** (verified 2026-08-05).

### 4. diagnostics — **DONE (v1 behavioral bar)**
> behavioral baseline; tighten toward strict where practical.

- Behavioral baseline verified: `python3 scripts/verify_diagnostics_corpus_behavior.py --root .`
  → `OK`. Layout guard: `verify_diagnostics_corpus_layout`.
- Strict `TextIO`-style sinks are deferred (`M3-S-021`); the v1 bar is behavioral.
- **Status: DONE for the v1 bar**; strict tightening is post-M3.

### 5. SML retirement — **DONE (G9)**
> tag `bootstrap-sml-final` on sv0c; `sml-legacy/`; default build sv0-only; recovery documented.

- `git -C sv0c tag -l bootstrap-sml-final` → present; `sml-legacy/` present (12 entries).
- Default orchestration is sv0-only; recovery documented (G9 slice backlog:
  `M3-S-050`–`M3-S-053`, `verify_m3_g9_retirement_docs_layout`).
- **Status: DONE.**

## Closure-authority decisions (2026-08-05) — resolved

1. **Row 1 (full pipeline):** **Accepted on corpus evidence.** 98/98 C
   behavioral + 18/18 VM byte-identical over the curated corpus satisfies
   "feature parity on the agreed surface." → **Done.**
2. **Row 2 (self-compile):** **Native recipe accepted.** The SML→C→cc-once
   mega-TU recipe + 98/98 SML-free native compose + documented cold path
   satisfies "sv0 builds sv0 sources." → **Done.**
3. **Rows 3–5:** evidence confirmed. → **Done.**

**Outcome:** M3 stakeholder completion criteria **met**. Post-M3 hardening
(Phase C semantic feature parity; default `SV0_SELF_HOST_COMPILER` promotion)
proceeds on `l0-closure-roadmap.md` and does not reopen M3.
