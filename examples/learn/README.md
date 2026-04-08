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
| `12_logical_operators.sv0` | Boolean **`&&`** / **`||`** in`if` conditions |
| `13_compare_chain.sv0` | Chained **`<`** comparisons with **`&&`** |
| `14_match_integer.sv0` | `match` on an **`i32`** literal + default **`_`** arm |
| `15_enum_payload.sv0` | Enum variant carrying **`i32`**; bind in **`match`** |
| `16_try_operator.sv0` | **`?`** on a call returning the same enum as **`main`** |
| `17_no_alias_contract.sv0` | **`requires(no_alias(&a, &b))`** on a function |
| `18_continue_in_loop.sv0` | **`continue`** in **`while`** with **`let mut`** + assignment |
| `19_loop_invariant.sv0` | **`loop_invariant`** on **`while`** (see **`emit-c`** for `sv0_requires` lowering) |
| `20_struct_mut_field_assign.sv0` | **`let mut`** struct + **`p.f =`** and **`p.f +=`** (one field level) |
| `21_mut_i32_compound.sv0` | **`let mut`** scalar + **`+=`** / **`-=`** / **`*=`** (arithmetic compound ops) |
| `22_native_c_path.sv0` | **Native `cc` path** — emit C, link **`sv0_runtime.c`**, run a binary (comments in file) |

## Prerequisites

- [SML/NJ](https://smlnj.org/) on your `PATH` (for the bootstrap compiler).
- Repo layout: this directory lives under **`sv0c/examples/learn/`**; toolchain commands take paths **relative to `sv0c/`**.

## Compile to bytecode and run (VM)

```bash
# from sv0-toolchain root (paths below are relative to sv0c/)
./scripts/sv0 vm-compile examples/learn/01_minimal_main.sv0
./scripts/sv0 vm-run sv0c/build/vm/01_minimal_main.sv0b
```

The VM exit code is derived from **`main`** (for **`i32`** programs it is the integer return). Example: **`12_logical_operators.sv0`** exits **`0`** because **`true && false`** takes the **`else`** branch. **`16_try_operator.sv0`** uses **`main() -> R`** (`Ok`/`Err` enum); **`vm-run`** reports the lowered result (e.g. **`vm_exit:7`** for **`Ok(7)`** in this sample).

## Full toolchain check (optional)

From the repo root, **`./scripts/sv0 test`** runs bootstrap builds, stage0 golden C diffs, VM parity, integration tests, and the self-host pilot loop (see **`task/sv0-toolchain-milestone-2-prep.Rmd`**). For Python-only guards without SML/NJ, use **`./scripts/sv0 test-guards`**.

## Print C (bootstrap compiler)

```bash
./scripts/sv0 emit-c examples/learn/08_println.sv0 | head
```

For **`loop_invariant`**, the interesting lowering is visible in C output:

```bash
./scripts/sv0 emit-c examples/learn/19_loop_invariant.sv0 | head -30
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
