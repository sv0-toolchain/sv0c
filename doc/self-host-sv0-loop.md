# Self-host loop (sv0 compiler written in sv0)

This document describes the **sv0 → sv0** bootstrap loop for milestone 3: the compiler’s **sources are sv0** (`lib/`, `lexer/`, `parser/`, …), and the goal is for a **compiler binary built from those sources** to compile the **same** sources again, eventually replacing the SML bootstrap for day-to-day work.

## Stages (T-diagram)

1. **Bootstrap compiler (today):** SML image (`build/sv0c` heap) compiles each `.sv0` file to C (`--target` default) or bytecode (`--target=vm`).
2. **Host binary from C:** `gcc` (or `cc`) compiles the emitted C together with `runtime/sv0_runtime.h` into a native executable. That executable runs the program’s `main` (smoke test for the C backend).
3. **Self-host compiler (next):** Build a **compiler** executable from transliterated sv0 sources (partial tree today). That executable must accept the same front-end job as step 1 for a single file (emit C to stdout is the contract used below).
4. **Close the loop:** Run the self-host compiler on the **compiler sv0 sources**; compare its output to the SML reference (or to a prior generation) until the chain is stable.

## What is automated today

From the **sv0-toolchain** repo root:

```bash
./scripts/sv0 self-host-sv0-loop
```

For each path listed in **`sv0c/lib/self-host-sv0-loop.list`**, the script:

- Emits C **twice** via the SML heap and asserts the outputs are **byte-identical** (determinism).
- Compiles the first emit with **`cc`** (or **`$CC`**) using **`-std=c99 -O0`** and **`-I sv0c/runtime`**, links a native binary, runs it, and expects **exit status 0**. (**`-Werror`** is intentionally omitted so Clang and GCC both accept the current emitter’s parenthesized comparisons.)

Not every transliteration seed links as a standalone C program yet (some golden-checked C still has struct-return ABI gaps). The pilot list in **`lib/self-host-sv0-loop.list`** is curated to files known to **link and run** today; grow it as the C backend matures.

This is **not** yet “sv0 compiles sv0”; it is the **host-side** chain (SML → C → native) on **real compiler-in-sv0 sources**, and it is the hook on which the second leg will attach.

## Optional: compare a self-built compiler to SML

If you have a wrapper that runs a **compiler built from sv0** and prints C for one input file to stdout (same shape as `sml @SMLload=build/sv0c <path>`), set:

```bash
export SV0_SELF_HOST_COMPILER=/path/to/emit-c-wrapper
./scripts/sv0 self-host-sv0-loop
```

The wrapper is invoked as:

```text
"$SV0_SELF_HOST_COMPILER" /absolute/path/to/sv0c/<rel>
```

The script diffs its stdout against the SML emit for each list entry. If the variable is unset, this step is skipped.

## CI

`./scripts/sv0 test` runs **`self-host-sv0-loop`** after the stage0 golden checks. Requires a working **`cc`** on the runner (Ubuntu CI provides one).

## Related

- **`task/sv0-toolchain-milestone-3-self-host.Rmd`** — definition of done for milestone 3.
- **`sv0c/lib/LAYOUT.md`** — transliteration order and bootstrap lists.
- **`./scripts/sv0 emit-c`**, **`self-host-capture-stage0`**, **`self-host-compare`** — C snapshot workflow.
