# Driver pipeline composition (M3-S-041)

This document anchors **milestone M3** slice **M3-S-041** (**driver — pipeline composition in sv0**): what “lexer→parser→…→emit” means versus the **SML** bootstrap, what is **implemented today** in **`lib/main.sv0`**, and what remains before the slice can move from **Partial** to **Done**.

Normative language semantics remain in **`sv0doc/`**. This file is compiler-bootstrap engineering notes.

## SML reference ordering

The stage-0 compiler’s file driver (**`sv0c/sml/main.sml`**) follows roughly:

1. Read source (**`TextIO.inputAll`**).
2. **`Lexer.tokenize`** → parallel `(tag, start, end)` streams + **`source`** string.
3. **`Parser.parse`** → program/item arenas.
4. Resolver → checker → … → lowering → codegen (**C** or **VM**), depending on the **`compileProgram*`** entry used.

The **linear step indices** **`0..DRIVER_FULL_PIPELINE_LEN()-1`** exposed from **`lib/main.sv0`** (**`driver_pipeline_step_name`**) mirror this at **coarse** granularity: **tokenize**, **parse**, then **`PHASE_RESOLVE` … `PHASE_EMIT_VM`** (see **`DRIVER_FULL_PIPELINE_LEN`**, **`driver_pipeline_step_for_core_phase`**).

## Current sv0 bootstrap shape

- **`lib/main.sv0`** implements **path / CLI / phase naming / VM output paths**, **`driver_tokenize_sketch`** (read file → tokenize boundary sketch), and tests (**`test_driver_pipeline_step_names`**, **`test_phases`**, …). It does **not** call **`resolve_program`**, **`check_program`**, **`lower`**, or **`emit`** — those live in other **`lib/*.sv0`** roots compiled independently by the **SML** **`CM.make`** heap loader.
- Full lexer + parser live in **`lib/lexer.sv0`**, **`lib/parser.sv0`** (see **`lib/LAYOUT.md`** § transliteration order).
- **Cross-unit linking for sv0** (multiple **`lib/*.sv0`** compilation units merged into one executable) is **not** the same as `#include` text concatenation; until an agreed multi-unit or single-TU policy lands, “one driver TU” is **documentation + naming parity**, not an end-to-end **`main`** that runs the whole compiler.

## Why M3-S-041 stays Partial alongside G6 link work

**`doc/link-g6-blockers.md`** explains why **M3-S-039** (token-handle rewrites) and **M3-S-040** (per-file parse + merged program) affect link-time and project-wide orchestration. A **single** bootstrap driver that faithfully mirrors **SML** **`compileProjectDir`** may need those facilities before it is honest to mark **M3-S-041** **Done**.

## Closure criteria (Working definition)

Promote **M3-S-041** from **Partial** to **Done** when **all** of the following hold:

1. **One agreed composition model** is documented and implemented: either  
   - (A) **one** `.sv0` translation unit whose **`main`** drives **lexer → parse → resolve → check → … → emit** with the same observable phase order as **`sml/main.sml`** on fixed fixtures, or  
   - (B) explicit **multi-unit** sv0 linking plus a thin **`main`** root that calls the composed pipeline (with tests proving order).
2. **Automated checks**: extend **`./scripts/sv0 test`** (or a documented subset) so regressions in phase order or wiring fail CI — not only naming tests in **`lib/main.sv0`**.
3. **Cross-reference**: update **`doc/transliteration-plan.md`** Pipeline/driver row and **`task/sv0-toolchain-milestone-3-self-host.Rmd`** **## M3 G6 slice status** when (1)–(2) land.

Until then, **`lib/main.sv0`** remains the **staging module** for driver-facing helpers; **M3-S-042** (**`doc/cli-parity.md`**) can be **Done** independently.

## Related

- **`sv0c/doc/cli-parity.md`** — argv / **`classify_cli`** vs **SML** **`Main.main`** (**M3-S-042**).
- **`sv0c/doc/link-g6-blockers.md`** — link-time and merge gates (**M3-S-039**, **M3-S-040**).
- **`sv0c/lib/LAYOUT.md`** — bootstrap directory roles and transliteration spine.
