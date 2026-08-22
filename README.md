# sv0c — the sv0 compiler

sv0c compiles `.sv0` source to **C** (`cc`/`gcc` → native binary) and to **sv0
bytecode** (run by [sv0vm](../sv0vm/)). It is **self-hosting**: the compiler is
written in sv0 and compiles itself.

## two implementations, one design

sv0c exists as two compilers that share the same pipeline and pass design:

| | tree | role |
|---|---|---|
| **the compiler, in sv0** | [`lib/`](lib/) (21 `*.sv0` modules) | **default.** Built natively into `build/sv0-megatu-compiler-native` (no SML at runtime); this is what the toolchain uses. |
| **the SML bootstrap** | [`sml-legacy/`](sml-legacy/) (SML/NJ) | **retired reference.** The original bootstrap that first compiled `lib/`. Kept as a byte/behavioral cross-check; not the default (see [SML retirement](#sml-retirement)). |

Both accept the same language and emit equivalent output. The native compiler is
the source of truth; SML is a reference the CI still diffs against.

## pipeline

```
sv0 source (.sv0)
      │
      ▼
  lexer          lib/lexer.sv0            tokens
      │
      ▼
  parser         lib/parser.sv0           AST (flat arena) + lib/ast.sv0
      │
      ▼
  resolver       lib/resolver.sv0         name/scope resolution  → E0300/E0301/E0307
      │
      ▼
  checker        lib/checker.sv0          type inference + checking → E0400/E0429/…
      │
      ▼
  contract       lib/contract_analyzer.sv0  requires/ensures/loop_invariant
      │
      ▼
  lowering       lib/lowering.sv0         AST → sv0-IR (lib/ir.sv0)
      │
      ├─▶ codegen      lib/codegen.sv0     IR → C99  ─▶ cc ─▶ native binary
      │
      └─▶ vm_codegen   lib/vm_codegen.sv0  IR → bytecode (lib/bytecode.sv0) ─▶ sv0vm
```

Supporting modules: `driver.sv0` (single-file orchestration), `link.sv0` +
`megaTU-main.sv0` (multi-module / `--project` composition), `diagnostic.sv0`
(`Exxxx` codes + source snippets), `span.sv0`/`token.sv0`/`env.sv0`/`types.sv0`/
`unify.sv0`/`include_expand.sv0` (shared infrastructure).

Pass-by-pass notes: [`doc/compiler-passes.md`](doc/compiler-passes.md). Multi-module
composition: [`doc/driver-pipeline-composition.md`](doc/driver-pipeline-composition.md).

## quickstart

Run from the **sv0-toolchain** root (the parent repo drives everything through
`./scripts/sv0`):

```bash
./scripts/sv0 test            # full gate: units + integration + VM parity + self-host loop
./scripts/sv0 emit-c lib/lexer.sv0     # print the C a module compiles to
./scripts/sv0 vm-compile examples/learn/01_hello.sv0   # compile to bytecode
./scripts/sv0 vm-run build/vm/01_hello.sv0b            # run it on sv0vm
```

Build the native compiler and use it directly:

```bash
bash scripts/build-sv0-megatu-native.sh          # → build/sv0-megatu-compiler-native
printf '' > /tmp/.sv0_drv_path
build/sv0-megatu-compiler-native path/to/file.sv0 > out.c   # emit C
```

**Learning path:** numbered tutorials `examples/learn/01_*.sv0 … 22_*.sv0` +
`examples/learn/23_project_minimal/` — see [`examples/learn/README.md`](examples/learn/README.md).

## self-hosting

The sv0 compiler compiles its own source. `./scripts/sv0 self-host-sv0-loop`
compiles every module in `lib/self-host-sv0-loop.list` with the native compiler,
builds and runs the result, and checks **behavioral parity** against the SML
reference (identical stdout+exit; the emitted C differs by design). This is the
default; see [`doc/self-host-sv0-loop.md`](doc/self-host-sv0-loop.md) and
[`doc/native-self-host-compiler-recipe.md`](doc/native-self-host-compiler-recipe.md).

## directory structure

| path | description |
|---|---|
| `lib/` | **the compiler, in sv0** — 21 modules + `bootstrap-sources.list`, `golden/stage0/`. See [`lib/README.md`](lib/README.md). |
| `sml-legacy/` | retired SML/NJ bootstrap reference (`sources.cm`) |
| `doc/` | compiler documentation — start at [`doc/README.md`](doc/README.md) |
| `examples/learn/` | numbered `.sv0` tutorials ([README](examples/learn/README.md)) |
| `examples/libs/` | educational library-shaped packages, not bootstrap seeds ([README](examples/libs/README.md)) |
| `test/` | unit + integration + diagnostics + VM-parity suites |
| `build/` | build artifacts (gitignored) |

## specification

sv0c implements the language defined in [sv0doc](../sv0doc/) (grammar, type
rules, contracts, memory model, keywords).

## SML retirement

`sml-legacy/` is a retired reference, not the default compiler. The formal
cutover (tag `bootstrap-sml-final`, sv0-only build) is human-gated:
[`doc/sml-retirement-cutover-checklist.md`](doc/sml-retirement-cutover-checklist.md).
Cold-bootstrap recovery if self-host breaks:
[`doc/cold-bootstrap-recovery.md`](doc/cold-bootstrap-recovery.md). Do not run the
cutover before the criteria in `task/sv0-toolchain-milestone-3-self-host.Rmd`.

## building the SML reference (rarely needed)

Requires SML/NJ (110.99.9). `make -C sv0c heap` builds `build/sv0c` (the SML
reference emitter used by the byte-guards); `make -C sv0c test` runs the SML unit
suite. The native path above is the default and needs no SML at runtime.

## license

Licensed under either of [Apache License, Version 2.0](LICENSE-APACHE) or
[MIT license](LICENSE-MIT) at your option.
