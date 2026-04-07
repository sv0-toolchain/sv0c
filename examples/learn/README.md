# Learning examples (sv0)

Small **`.sv0`** programs that mirror the integration tests and VM smoke corpus. They are **not** on the bootstrap or VM-parity manifests; use them to learn syntax and the toolchain from the **sv0-toolchain** repo root.

| File | Topic |
|------|--------|
| `01_minimal_main.sv0` | Smallest `main` / exit code |
| `02_arithmetic.sv0` | Integer operators and precedence |
| `03_if_expression.sv0` | `if` as expression |
| `04_while_loop.sv0` | `while` + `break` |
| `05_for_range.sv0` | `for` over half-open range |
| `06_call_function.sv0` | Calling a sibling `fn` |
| `07_enum_match.sv0` | `enum` + `match` |
| `08_println.sv0` | `println` and stdout |
| `09_struct.sv0` | `struct`, literal, field access |
| `10_generics_placeholder.sv0` | Monomorphic stand-in (see integration `generics`) |
| `11_contracts_requires.sv0` | `requires` / `ensures` on `main` |

## Prerequisites

- [SML/NJ](https://smlnj.org/) on your `PATH` (for the bootstrap compiler).
- Repo layout: this directory lives under **`sv0c/examples/learn/`**; toolchain commands take paths **relative to `sv0c/`**.

## Compile to bytecode and run (VM)

```bash
# from sv0-toolchain root
./scripts/sv0 vm-compile examples/learn/01_minimal_main.sv0
./scripts/sv0 vm-run sv0c/build/vm/01_minimal_main.sv0b
```

The VM exit code is the program’s **`main`** return value (often **`0`**).

## Print C (bootstrap compiler)

```bash
./scripts/sv0 emit-c examples/learn/08_println.sv0 | head
```

## Format and REPL

```bash
./scripts/sv0 fmt
./scripts/sv0 repl
# try: 1 + 2
# try: println("hello")
```

## Self-host pilot loop (SML → C → native)

The mechanical loop exercised in **`./scripts/sv0 test`** uses **`lib/self-host-sv0-loop.list`** (pilot seeds, not these tutorials). See **`sv0c/doc/self-host-sv0-loop.md`** and **`task/sv0-toolchain-milestone-3-self-host.Rmd`**.

## Normative language definition

Grammar and bytecode: **`sv0doc/`** (submodule), linked from the workspace **`README.md`**.
