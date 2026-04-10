# Self-host loop (sv0 compiler written in sv0)

This document describes the **sv0 → sv0** bootstrap loop for milestone 3: the compiler’s **sources are sv0** (`lib/`, `lexer/`, `parser/`, …), and the goal is for a **compiler binary built from those sources** to compile the **same** sources again, eventually replacing the SML bootstrap for day-to-day work.

## Stages (T-diagram)

1. **Bootstrap compiler (today):** SML image (`build/sv0c` heap) compiles each `.sv0` file to C (`--target` default) or bytecode (`--target=vm`).
2. **Host binary from C:** `gcc` (or `cc`) compiles the emitted C together with `runtime/sv0_runtime.h` into a native executable. That executable runs the program’s `main` (smoke test for the C backend).
3. **Self-host compiler (next):** Build a **compiler** executable from transliterated sv0 sources (partial tree today). That executable must accept the same front-end job as step 1 for a single file (emit C to stdout is the contract used below).
4. **Close the loop:** Run the self-host compiler on the **compiler sv0 sources**; compare its output to the SML reference until the chain is stable.

## Loop **interface** (CI-complete today)

The meta-repo wires the **full mechanical loop** except swapping the bootstrap compiler for a **native sv0-built** binary:

| Leg | What runs in `./scripts/sv0 test` |
|-----|-----------------------------------|
| SML → C (×2) | Byte-identical check (determinism). |
| C → native | `cc` + run each pilot `main` (**`lib/self-host-sv0-loop.list`** — **172** programs: all bootstrap-listed sources; parser / lexer / lib / IR / C-backend / VM-backend / bytecode / driver / include-expansion / type-checker / unify / pipeline-composition seeds + golden-pass / integration / vm-parity tests that link as a single TU today). |
| “Second compiler” → C | **`diff`** vs SML: by default **`scripts/sv0-self-host-emit-c.sh`** (delegates to the SML heap — proves the hook and stdout contract). |

When you have a real compiler built from sv0, set **`SV0_SELF_HOST_COMPILER`** to that binary (or a wrapper). The script is invoked as:

```text
"$SV0_SELF_HOST_COMPILER" /absolute/path/to/sv0c/<rel>
```

with **C on stdout**. A green **`diff`** against SML is the definition of **semantic** loop closure for that pilot list.

To skip the third leg locally (faster iteration), set **`SV0_SKIP_SELF_HOST_COMPILER_DIFF=1`**.

## Pilot `emit-c` harness vs native **`SV0_SELF_HOST_COMPILER`** (`PROGRESS.md` **C-5**)

| Mode | What implements the “second compiler” leg | Meaning |
|------|-------------------------------------------|---------|
| **Pilot / default CI** | **`scripts/sv0-self-host-emit-c.sh`** → loads the **SML** heap **`build/sv0c`** and runs the same job as **`emit-c`** for one absolute **`.sv0`** path, **C on stdout** | Proves the **mechanical** loop (**`cc`**+run on **`lib/self-host-sv0-loop.list`**) and the **stdout contract** for the third-leg **`diff`**. This is **not** semantic self-hosting — it is the **bootstrap stand-in** documented in **`task/sv0-toolchain-milestone-3-self-host.Rmd`**. |
| **Native semantic path** | Set **`SV0_SELF_HOST_COMPILER`** to a **native executable built from sv0 sources** (or a thin wrapper) that honors the same invocation: **one absolute path** → **C on stdout** | When **`./scripts/sv0 self-host-sv0-loop`** runs, the third leg **`diff`**s that output against SML; **byte-identical C** on the pilot list is the **working definition** of semantic loop closure for those seeds. **M3** completion still requires broader parity and stakeholder criteria in the owning task — not only this hook. |

**Prerequisites for turning on native mode:** a binary (or wrapper) that matches the contract exercised by **`sv0-self-host-emit-c.sh`** and **`./scripts/sv0 emit-c <rel>`** (paths relative to **`sv0c/`** in the latter; the self-host script always passes **absolute** paths). Keep **`doc/transliteration-plan.md`** and **`lib/LAYOUT.md`** current as more compiler modules become sv0.

## Commands

From the **sv0-toolchain** repo root:

```bash
./scripts/sv0 self-host-sv0-loop
./scripts/sv0-self-host-emit-c.sh "$(pwd)/sv0c/lib/span_core.sv0"   # same stdout as heap load
```

## CI

`./scripts/sv0 test` runs **`self-host-sv0-loop`** after the stage0 golden checks. Requires **`cc`** and an executable **`scripts/sv0-self-host-emit-c.sh`** (tracked in git).

## SML retirement guardrail (`PROGRESS.md` **C-6**)

**`C-6` = done does *not* mean the SML bootstrap has been retired.** It means the **non-claim** policy is written down and cross-linked so agents and maintainers do not confuse **harness health** with **stakeholder closure**.

| Topic | Where it is defined |
|-------|----------------------|
| Completion criteria before **`bootstrap-sml-final`** / **`sml-legacy/`** | **`task/sv0-toolchain-milestone-3-self-host.Rmd`** |
| Pilot loop ≠ retirement | **`.cursor/rules/28-sml-retirement-and-self-host-bar.mdc`**, this file (**§ Pilot vs native**), **`README.md`** (bootstrap tag table at meta-repo root) |
| Anti-patterns (no early tag, no **`sml/`** rename from pilot growth alone) | **`./scripts/sv0 milestone-orient show M3`** |

**Actual retirement** (git tag on **sv0c**, directory rename, default build switch) stays a **human-owned** cutover only after the M3 task says the bar is met — not an automated checklist flip in **`PROGRESS.md`**.

## Related

- **`task/sv0-toolchain-milestone-3-self-host.Rmd`** — definition of done for milestone 3 (full parity + SML retirement remain open).
- **`sv0c/lib/LAYOUT.md`** — transliteration order and bootstrap lists.
- **`./scripts/sv0 emit-c`**, **`self-host-capture-stage0`**, **`self-host-compare`** — C snapshot workflow.
