# L0 closure — engineering roadmap (sv0c)

Normative **“M3 done”** criteria live in **`task/sv0-toolchain-milestone-3-self-host.Rmd`** (**## completion criteria**, **## stakeholder closure checklist**). This note is **implementation sequencing** toward **L0** (*stakeholder* closure — compiler fully in sv0, parity, **native** self-host), not a substitute for that task file.

## Why L0 is still open after G6/G7/G8/G9/GX

- **G9** (SML retirement orchestration) and **G8** (recipe paths + bootstrap **`SV0_SELF_HOST_COMPILER`** wrapper) are **slice gates**, not **L0**.
- **`build/sv0-self-host-compiler`** remains a **bootstrap delegate** around **`scripts/sv0-self-host-emit-c.sh`** until a **native** binary honors the same argv/stdout contract (**`native-self-host-compiler-recipe.md`**).
- **`lib/main.sv0`** is a **staging** driver module; **integrated** lexer→emit in **native** code requires **deferred engineering** in **`doc/driver-pipeline-composition.md`** (**(A)** mega-TU **or** **(B)** multi-unit composition — ship **one** model).

## Critical path (dependency order)

Work streams below can overlap once upstream interfaces exist; order reflects **hard dependencies** for a honest **L0** claim.

### 1 — Project merge + link orchestration (unblocks realistic `compileProjectDir` parity)

**Goal:** After **per-file** `parse_program`, relocate dependent arenas and merge item/type/name state so **`linkProjectDir`** matches **SML** semantics beyond concatenated sources.

**Primary refs:** **`doc/link-g6-blockers.md`** (**M3-S-040** remaining), **`doc/transliteration-plan.md`** Link row.

**Acceptance (incremental):**

- Integration tests under **`sv0c/test/integration/`** (or a dedicated harness) that exercise **multi-file** resolve/check on merged ASTs with **deterministic** diagnostics paths.
- **`./scripts/sv0 test`** / **`test-guards`** stay green; refresh goldens only when behavior is intentional.

### 2 — Choose and implement **one** native composition model

**Goal:** A **single** agreed way to produce **one** native artifact that implements **`SV0_SELF_HOST_COMPILER`** (absolute `.sv0` path → **C on stdout**).

**Engineering default (see **`doc/driver-pipeline-composition.md`** § **Native closure**):**

- Prefer **(B)** multi-unit linking aligned with **`lib/LAYOUT.md`** / **`sources.cm`** spirit.
- Optional **(A)** mega-TU **spike** only if it unblocks an early **`self-host-sv0-loop`** third-leg **`diff`** — then **refactor toward (B)**.

**Constraint:** Today each **`bootstrap-build`** unit emits **standalone** C with a full program shape; **naïvely linking** multiple emitted **`.c`** files typically **duplicates** runtime/init assumptions. Closing **(B)** may require ** codegen / runtime packaging rules** agreed in **`sv0doc/`** or compiler-bootstrap notes — track decisions next to implementation, not only here.

**Acceptance:**

- **`scripts/build-sv0-self-host-compiler.sh`** (or successor) produces a **native** executable **not** wrapping **`sv0-self-host-emit-c.sh`**.
- **`./scripts/sv0 self-host-sv0-loop`** with **`SV0_SELF_HOST_COMPILER`** set to that binary reports **byte-identical** C vs the **SML** heap on **`lib/self-host-sv0-loop.list`** for the third leg (**`doc/self-host-sv0-loop.md`** § Pilot vs native).

### 3 — Semantic + VM parity tails vs **`## completion criteria`**

**Goal:** Close **resolver/checker/lowering/link** gaps called out in **`doc/transliteration-plan.md`** until behavior matches **SML** on the agreed surface; extend vm-parity / tier-2 policy as **native** bytecode emission replaces surrogates.

**Acceptance:**

- **`./scripts/sv0 test`** green without **`SV0_SKIP_SELF_HOST_COMPILER_DIFF`** when claiming semantic closure (**unless** a documented intermediate — see **`milestone-orientation.json`** **pre_merge_validation**).
- **`doc/transliteration-plan.md`** § **Feature + diagnostic parity** rows move to **Done** only when the milestone definition says so.

### 4 — Evidence package + stakeholder review

**Goal:** Map harness outputs, recipe commands, and recovery paths to **`task/sv0-toolchain-milestone-3-self-host.Rmd`** **## completion criteria** bullets; obtain **closure_authority** review per **`milestone-orientation.json`**.

**Acceptance:**

- **`## stakeholder closure checklist (M3 evidence)`** rows flip from **No**/**Partial** to **Yes**/**Done** with **named** commands and artifacts.
- Optionally flip **`state:`** on the owning task when criteria are truly met (do **not** do this on harness green alone).

## Validation ladder (agents)

Narrow → wide (see **`.cursor/rules/40-validation-and-proof.mdc`**):

1. **`./scripts/sv0 test-guards`**
2. Targeted **`./scripts/sv0 vm-compile`** / **`compile-run`** / **`emit-c`** for touched **`.sv0`**
3. **`./scripts/sv0 self-host-sv0-loop`** with **`SV0_SELF_HOST_COMPILER`** pointed at the artifact under test
4. **`./scripts/sv0 test`** before claiming integration closure

## Related

- **`doc/native-self-host-compiler-recipe.md`** — interface contract + bootstrap vs forward path  
- **`doc/driver-pipeline-composition.md`** — (A)/(B), staging vs native  
- **`doc/self-host-sv0-loop.md`** — pilot loop semantics  
- **`task/sv0-toolchain-milestone-3-self-host.Rmd`** — **L0** prerequisites table + ordered execution steps  
