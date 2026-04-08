# Example libraries (educational sv0)

This directory holds **small, heavily commented library-shaped projects** written in **sv0**. They exist to teach:

- how to structure a **multi-file** package (`main.sv0` + `lib/…`) like a future **`std`** or **`core`** tier;
- how the **toolchain** compiles and runs them (**`vm-project-compile`**, **`vm-run`**, and optionally **`emit-c`**);
- how **contracts** and **clear APIs** fit the [language vision](http://development.sasankvishnubhatla.net/tcowmbh/task/sv0-compiler-vision-and-design.html) (types first, contracts where the type system is not enough).

## Not the real standard library

- These trees are **not** on **`lib/bootstrap-sources.list`** and are **not** VM-parity or bootstrap seeds. They are **documentation + pedagogy**, not compiler internals.
- A future **shipped** standard library will live under its own policy (versioning, stability, **`sv0doc/`** linkage). Treat everything here as **examples to copy from**, not stable ABI.

## Layout convention

Each subfolder is one **project root** for **`--project`**:

```text
examples/libs/<name>/
  README.md          ← human narrative + “future std” notes
  main.sv0           ← tiny demo binary (exit code is your “assertion”)
  lib/lib.sv0        ← `module lib;` — library API (single module per project, like integration `modules/`)
```

The **module** is named **`lib`** so `main.sv0` can `use lib::…` without extra resolver setup. The **folder name** (`edu_clamp`, `edu_bits`, …) is the *package* name for humans.

## Build and run (from sv0-toolchain root)

Paths are relative to **`sv0c/`**:

```bash
./scripts/sv0 vm-project-compile examples/libs/edu_clamp
./scripts/sv0 vm-run sv0c/build/vm/main.sv0b
```

Repeat with **`edu_bits`** (or any other package). For **C** output from the demo entry only:

```bash
./scripts/sv0 emit-c examples/libs/edu_clamp/main.sv0 | head -40
```

(Linking a full static library for C is not the focus here; the educational path is **VM bytecode** + readable **`.sv0`** sources.)

## Index

| Package | Intent |
|---------|--------|
| [`edu_clamp/`](edu_clamp/README.md) | Bounded **`i32`** helpers (`clamp`, `min`, `max`) + comments on preconditions you might express as **`requires`** later. |
| [`edu_bits/`](edu_bits/README.md) | Tiny **bit / parity** helpers — good for teaching **`%`**, **`==`**, and boolean **`i32`** bridges in **`main`**. |

## Related

- [`examples/learn/README.md`](../learn/README.md) — single-file tutorials and **`23_project_minimal/`**.
- [`sv0c/lib/README.md`](../../lib/README.md) — **compiler-in-sv0** transliteration seeds (different purpose).
- [`task/sv0-toolchain-milestone-3-checklist.Rmd`](../../../task/sv0-toolchain-milestone-3-checklist.Rmd) — what “real” self-hosting libraries mean for milestone 3.
