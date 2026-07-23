# Driver pipeline composition (M3-S-041)

This document anchors **milestone M3** slice **M3-S-041** (**driver — pipeline composition in sv0**): what “lexer→parser→…→emit” means versus the **SML** bootstrap, what is **implemented today** in **`lib/main.sv0`**, and what remains before the slice can move from **Partial** to **Done**.

Normative language semantics remain in **`sv0doc/`**. This file is compiler-bootstrap engineering notes.

## SML reference ordering

The stage-0 compiler’s file driver (**`sv0c/sml-legacy/main.sml`**) follows roughly:

1. Read source (**`TextIO.inputAll`**).
2. **`Lexer.tokenize`** → parallel `(tag, start, end)` streams + **`source`** string.
3. **`Parser.parse`** → program/item arenas.
4. Resolver → checker → … → lowering → codegen (**C** or **VM**), depending on the **`compileProgram*`** entry used.

The **linear step indices** **`0..DRIVER_FULL_PIPELINE_LEN()-1`** exposed from **`lib/main.sv0`** (**`driver_pipeline_step_name`**) mirror this at **coarse** granularity: **tokenize**, **parse**, then **`PHASE_RESOLVE` … `PHASE_EMIT_VM`** (see **`DRIVER_FULL_PIPELINE_LEN`**, **`driver_pipeline_step_for_core_phase`**).

## Current sv0 bootstrap shape (updated 2026-07-03)

- **`lib/driver.sv0`** now implements a **complete, tested pipeline** — tokenize → parse → resolve → check → emit — in a single translation unit, with **49 self-host tests passing** (exit 0 natively). Key `drv_*` functions: `drv_tokenize`, `drv_parse`, `drv_resolve`, `drv_check`, `drv_emit`, and test helpers. This is the canonical single-TU pipeline implementation for P1/P2.
- **`lib/main.sv0`** implements **path / CLI / phase naming / VM output paths**, **`driver_tokenize_sketch`** (read file → tokenize boundary sketch), and tests (**`test_driver_pipeline_step_names`**, **`test_phases`**, …). It does **not** yet call the `drv_*` pipeline — that wiring (Phase A3 in the plan) is the P1 next step.
- Full lexer + parser live in **`lib/lexer.sv0`**, **`lib/parser.sv0`** (see **`lib/LAYOUT.md`** § transliteration order). `lib/driver.sv0` re-implements a self-contained subset of these for its pipeline.
- **Cross-unit linking for sv0** (multiple **`lib/*.sv0`** compilation units merged into one executable) is **not** the same as `#include` text concatenation; until an agreed multi-unit or single-TU policy lands, “one driver TU” is **documentation + naming parity**, not an end-to-end **`main`** that runs the whole compiler. `driver.sv0` is the working single-TU proof-of-concept.

### Bootstrap VM-compile unit model (why **M3-S-041** is hard)

**`./scripts/sv0 bootstrap-build`** / **`vm-compile <rel>`** compile **one** **`lib/<name>.sv0`** file per bootstrap entry. Symbols defined in **`lib/parser.sv0`** are **not** automatically visible to **`lib/main.sv0`** in that compilation model — each program is a **standalone** bytecode image. Wiring **`tokenize`** → **`parse_program`** inside **`main.sv0`** therefore requires either **duplicating** lexer/parser into the same file, adding a **single mega-module** pipeline seed, or introducing a **native linker** for sv0 objects. Until one of those lands, **(B)** in **## Closure criteria** should be read as **stage-0 SML heap composition** (`sources.cm`) being the authoritative combined driver; **`lib/main.sv0`** remains the sv0-side **staging module** for naming + CLI parity.

## Why M3-S-041 stays Partial alongside G6 link work

**`doc/link-g6-blockers.md`** explains why **M3-S-039** (token-handle rewrites) and **M3-S-040** (per-file parse + merged program) affect link-time and project-wide orchestration. A **single** bootstrap driver that faithfully mirrors **SML** **`compileProjectDir`** may need those facilities before it is honest to mark **M3-S-041** **Done**.

## Native closure — both (A) and (B) as sequential milestones (not dual architecture)

> **Evidence-based recommendation (2026-07-23): pursue (A) mega-TU first.** A concrete trade-off analysis with collision counts, feasibility tests, and the finding that **(B) requires a new `extern`/import-symbol language feature** the bootstrap compiler lacks is in **`native-compose-tradeoffs.md`**. It updates the "(B) for maintainability" lean below: (B) is the cleaner *destination* but is gated on a language feature we do not have; (A) is proven-viable now (`driver.sv0` at 98/98; 2-module concat compiles; only ~14 real name collisions). Read that doc before starting compose work.

**Stakeholder confusion:** **§ Closure criteria** asks for **one** agreed composition model **(A) xor (B)** for the **native** sv0 driver — you do **not** ship two competing drivers in production.

**Cleanest path toward M3 closure (engineering recommendation):**

1. **Prefer (B) multi-unit linking** for the **maintainable** native compiler: mirrors **`lib/LAYOUT.md`** module boundaries, matches how **`sources.cm`** composes the **SML** driver today (many compilation units → one heap image). Native closure = **`cc`** link of sv0-emitted **`.c`** objects + runtime (**or** agreed package layout).
2. **Use (A) mega-TU only as an optional spike** if the team needs the **fastest first** proof that **`SV0_SELF_HOST_COMPILER`** can **`diff`** SML on **`lib/self-host-sv0-loop.list`** — e.g. generated single TU or trimmed harness — then **refactor toward (B)** so M3 does not freeze a monolith.

**What “complete both” means for planning:** track **(A)** *spike complete* and **(B)** *linker + roots complete* as **separate milestones** on the way to **one** shipped model (**B)** unless the project explicitly chooses to stay **(A)** (discouraged for long-term sv0 maintenance).

**Already complete today (bootstrap / SML layer):** **multi-unit composition** via **`sources.cm`** + **`sml-legacy/main.sml`** — this is the **authoritative integrated driver** until native **(B)** lands (**§ Bootstrap staging closure**).

## Closure criteria (Working definition)

Promote **M3-S-041** from **Partial** to **Done** when **all** of the following hold:

1. **One agreed composition model** is documented and implemented: either  
   - (A) **one** `.sv0` translation unit whose **`main`** drives **lexer → parse → resolve → check → … → emit** with the same observable phase order as **`sml-legacy/main.sml`** on fixed fixtures, or  
   - (B) explicit **multi-unit** sv0 linking plus a thin **`main`** root that calls the composed pipeline (with tests proving order).
2. **Automated checks**: extend **`./scripts/sv0 test`** (or a documented subset) so regressions in phase order or wiring fail CI — not only naming tests in **`lib/main.sv0`**. **Partial guard (landed):** **`scripts/verify_m3_g6_pipeline_contract.py`** (via **`./scripts/sv0 test-guards`** / **`test`**) asserts **`PHASE_COUNT`**, **`DRIVER_FULL_PIPELINE_LEN` = 2 + `PHASE_COUNT()`**, and presence of coarse step-name strings — this protects the **documented spine** but **does not** execute lexer→emit inside sv0.
3. **Cross-reference**: update **`doc/transliteration-plan.md`** Pipeline/driver row and **`task/sv0-toolchain-milestone-3-self-host.Rmd`** **## M3 G6 slice status** when (1)–(2) land.

Until then, **`lib/main.sv0`** remains the **staging module** for driver-facing helpers; **M3-S-042** (**`doc/cli-parity.md`**) can be **Done** independently.

### Resolution — model chosen: (A) self-contained single TU = `lib/driver.sv0` (2026-07-09)

Per **`l0-closure-roadmap.md`** the composition model is **(A)**, realized as the **self-contained single translation unit** **`lib/driver.sv0`** — **not** a text-`include` mega-TU of the 19 real modules (that is ruled out: every module defines `fn main()` → **E0302**, and the codegen emits `static` for non-main fns → **(B)** object linking is illegal too; see the `driver.sv0` header "WHY SELF-CONTAINED"). `driver.sv0`'s `main` (CLI mode via `/tmp/.sv0_drv_path`) drives **`drv_tokenize` → `drv_parse` → `drv_resolve`/`drv_check` → `drv_emit_c`** in the documented order. **`lib/main.sv0`** stays the naming/CLI **staging module** by design — it does not (and per **`verify_m3_g6_staging_driver_contract.py`** must not) call `parse_program`.

**Criterion (1)** — model documented + implemented: `driver.sv0` (49 self-host unit tests; the *essential subset* per its header). **Criterion (2)** — phase-order/wiring regressions fail CI via **two** checks beyond the numeric spine: (a) **`verify_m3_g6_pipeline_contract.py`** now statically asserts `drv_compile_file` calls `drv_tokenize` before `drv_parse` before `drv_emit_c`; (b) **`./scripts/sv0 self-host-native-parity`** *executes* the composed pipeline end-to-end natively on all 98 seeds (emit+cc+run, 98/98 vs SML) — the "lexer→emit runs inside sv0" proof this section previously lacked. **Scope caveat:** this closes the *composition* gate on the essential-subset driver; full-language **feature parity** (multi-module `linkProjectDir`, resolver/lowering tails) remains the separate **Phase C** work, and the composed *full* compiler + native `--target=vm` remain **Phase B/D**.

## Bootstrap staging closure (**M3-S-041** stage-0 **Done**)

For **bootstrap / transliteration milestone tracking**, promote **M3-S-041** from **Partial** to **Done** when **all** of the following hold (this is **not** a substitute for a future single-TU sv0 driver):

1. **`sv0c/sources.cm`** lists **`sml-legacy/main.sml`** — the SML/NJ heap is the **integrated** stage-0 compiler driver (lexer→…→emit live here).
2. **`lib/main.sv0`** keeps the **nine-step** coarse spine (**`DRIVER_FULL_PIPELINE_LEN`** / **`driver_pipeline_step_name`**) and **`driver_tokenize_sketch`**, and does **not** embed **`parse_program`** (each **`lib/*.sv0`** root is **standalone** under **`bootstrap-build`**).
3. **Automated guards:** **`scripts/verify_m3_g6_pipeline_contract.py`** + **`scripts/verify_m3_g6_staging_driver_contract.py`** pass via **`./scripts/sv0 test-guards`** / **`test`**.

**Deferred engineering** (still required for a **native** self-host story): option **(A)** or **(B)** in **## Closure criteria** — single mega-module sv0 pipeline or sv0 cross-unit linking.

## Related

- **`sv0c/doc/l0-closure-roadmap.md`** — **L0** sequencing toward native **(A)/(B)** closure vs this document’s deferred engineering.
- **`sv0c/doc/cli-parity.md`** — argv / **`classify_cli`** vs **SML** **`Main.main`** (**M3-S-042**).
- **`sv0c/doc/link-g6-blockers.md`** — link-time and merge gates (**M3-S-039**, **M3-S-040**).
- **`sv0c/lib/LAYOUT.md`** — bootstrap directory roles and transliteration spine.
