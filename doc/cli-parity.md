# Bootstrap compiler CLI parity (`sv0c` / M3-S-042)

This document closes **M3-S-042** for the **compiler binary argv surface**: exact modes implemented by **`sv0c/sml/main.sml`** `Main.main`, how **`lib/main.sv0`** classifies the same shapes, and what stays **out of scope** (meta-repo orchestration).

## SML reference (`Main.main`)

`Main.main (_, args)` accepts **only** these patterns (anything else prints usage to stderr and exits failure):

| argv shape | Behavior |
|------------|----------|
| `[file.sv0]` | C backend — expanded source, compile to **stdout** |
| `[--target=vm, file.sv0]` | VM — write **`build/vm/<stem>.sv0b`** (`stem` = `baseNameSv0 file`) |
| `[--target=vm, --project, dir]` | VM project — link dir, emit **`build/vm/<stem>.sv0b`** (`stem` = `projectVmEntryStem dir`) |
| `[--project, dir, --target=vm]` | Same as previous row (**flag order** does not matter) |
| `[--project, dir]` | C project — link `*.sv0` under `dir`, compile to **stdout** |

There are **no** short flags, `--help`, or additional modes in `main.sml` today.

## sv0 transliteration (`lib/main.sv0`)

- **Classification:** `classify_cli(argc, has_vm_flag, has_project_flag)` maps the five supported combinations above to **`CLI_FILE`** (0) … **`CLI_VM_PROJECT`** (3); unsupported shapes return **`CLI_USAGE`** (4).
- **Tests:** `test_cli`, `test_cli_invalid_patterns` lock supported vs unsupported `(argc, vm, project)` triples.
- **Helpers:** `is_target_vm_flag`, `is_project_flag` match the exact strings **`--target=vm`** and **`--project`** (same as SML string literals in the `case`).

**Non-goal for this slice:** wiring `classify_cli` into a standalone sv0 `main` that invokes the full pipeline — that is **M3-S-041** (single-driver composition).

## Meta-repo `scripts/sv0`

`./scripts/sv0` is the **workspace orchestrator** (test harness, goldens, self-host loop, MCP, etc.). Its **`usage()`** block lists the **same four bootstrap compiler modes** as orientation for developers; it does **not** duplicate every `main.sml` code path.

**Explicit non-goals:**

- Parity between **`scripts/sv0 <subcommand>`** and **`sv0c`** argument grammar — different programs by design.
- Documenting every shell helper (`emit-c`, `vm-compile`, …) in this file — see **`AGENTS.md`** and **`scripts/sv0`** source.

**Alignment:** `vm-compile` and `vm-project-compile` invoke `Main.main` with argv consistent with the table above (including **`--target=vm --project <dir>`**).

## When this doc must change

- **`sv0c/sml/main.sml`** gains or removes an argv branch.
- **`classify_cli`** semantics change (supported triples).
- **`scripts/sv0`** `usage()` bootstrap section is edited — keep the **four-mode** summary consistent with this matrix.

See **`task/sv0-toolchain-milestone-3-self-host.Rmd`** **## M3 G6 slice status** for slice bookkeeping.
