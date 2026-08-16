# `lib/` — the sv0 compiler, in sv0

These 21 `*.sv0` modules **are** the compiler. They are compiled to a single
native binary (`build/sv0-megatu-compiler-native`) via the mega-TU compose and
are the default toolchain; `../sml-legacy/` is the retired SML reference. For the
big picture see the [repo README](../README.md); for a pass-by-pass walkthrough
see [`../doc/compiler-passes.md`](../doc/compiler-passes.md).

## modules by pipeline stage

| stage | modules |
|---|---|
| **front end** | `lexer.sv0` → `parser.sv0` (+ `ast.sv0`) |
| **analysis** | `resolver.sv0` (names/scopes), `checker.sv0` (types), `contract_analyzer.sv0` (`requires`/`ensures`/`loop_invariant`) |
| **lowering** | `lowering.sv0` (AST → sv0-IR, `ir.sv0`) |
| **backends** | `codegen.sv0` (IR → C99), `vm_codegen.sv0` (IR → bytecode, `bytecode.sv0`) |
| **composition** | `driver.sv0` (single file), `link.sv0` + `megaTU-main.sv0` (multi-module / `--project`), `main.sv0` (CLI entry) |
| **infrastructure** | `diagnostic.sv0` (`Exxxx` codes + snippets), `span.sv0`, `env.sv0`, `types.sv0`, `unify.sv0`, `include_expand.sv0` |

Each module carries its own `test_*` unit tests in-file; `./scripts/sv0 test`
compiles and runs them.

## build & self-host

From the **sv0-toolchain** root:

```bash
./scripts/sv0 test                        # full gate (units, integration, VM parity, self-host loop)
./scripts/sv0 emit-c lib/lexer.sv0        # C a module compiles to
./scripts/sv0 self-host-sv0-loop          # the sv0→sv0 self-hosting loop (native, behavioral parity)
bash scripts/build-sv0-megatu-native.sh   # build the native compiler
```

## the lists & goldens

| file | role |
|---|---|
| `bootstrap-sources.list` | modules CI compiles + runs (one path per line, relative to `sv0c/`) |
| `self-host-sv0-loop.list` | modules the self-host loop compiles with the native compiler |
| `golden/stage0/<stem>.c` | checked-in reference C; `./scripts/sv0 test` diffs the SML delegate's fresh emit against it (an SML byte-guard) |

When you edit a module, both its `golden/stage0/<stem>.c` and its
`test/vm-parity/golden/sml/<stem>.sv0b` shift — refresh both before committing
(see [`../doc/self-host-sv0-loop.md`](../doc/self-host-sv0-loop.md)). Read
[`../doc/bootstrap-compiler-workarounds.md`](../doc/bootstrap-compiler-workarounds.md)
for the sv0-language constraints these sources work within.

> Historical: the milestone-3 transliteration plan and directory/seed layout are
> archived at [`../doc/archive/lib-LAYOUT.md`](../doc/archive/lib-LAYOUT.md) and
> [`../doc/archive/transliteration-plan.md`](../doc/archive/transliteration-plan.md).
