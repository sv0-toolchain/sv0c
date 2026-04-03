# sv0c -- sv0 bootstrap compiler

the sv0 bootstrap compiler, implemented in Standard ML (SML/NJ).

## purpose

sv0c reads `.sv0` source files and compiles them to C source code, which is then compiled to native binaries by a standard C compiler (`cc`/`gcc`). this is the milestone 1 bootstrap compiler that will eventually be rewritten in sv0 itself. for what is actually in that slice versus deferred to milestone 2, see [sv0doc/milestone-1-complete.md](../sv0doc/milestone-1-complete.md).

## architecture

```
sv0 source (.sv0)
     |
     v
  Lexer         sml/lexer/
     |
     v
  Parser        sml/parser/ + sml/ast/
     |
     v
  Name Resolver sml/name_resolution/
     |
     v
  Type Checker  sml/type_checker/
     |
     v
  Contract      sml/contract_analyzer/
  Analyzer
     |
     v
  IR Lowering   sml/ir/
     |
     v
  C Backend     sml/backend/c/
     |
     v
  C source -> cc -> native binary
```

## directory structure

| directory | description |
|---|---|
| `sml/` | **Bootstrap compiler** (SML/NJ) — lexer through backends (`sources.cm`) |
| `sml/lexer/` | tokenizer -- keywords, identifiers, literals, operators |
| `sml/parser/` | recursive descent parser producing AST |
| `sml/ast/` | AST type definitions (SML datatypes) |
| `sml/name_resolution/` | scope resolution, import handling, module graph |
| `sml/type_checker/` | type inference, trait resolution, overload resolution |
| `sml/contract_analyzer/` | contract validation, runtime check insertion |
| `sml/ir/` | sv0-IR definition and AST-to-IR lowering |
| `sml/backend/c/` | sv0-IR to C99 code generation |
| `sml/backend/vm/` | sv0-IR to bytecode |
| `sml/error/` | error reporting with source spans |
| `lib/` | **Compiler in sv0** — transliteration seeds, `bootstrap-sources.list`, `golden/stage0/` |
| `lexer/` | future sv0 lexer sources (parallel to `sml/lexer/`) |
| `test/` | test suite |
| `doc/` | pass-by-pass notes ([compiler-passes.md](doc/compiler-passes.md)) — includes structs, enums, and `match` (Milestone 1 Phase 5) |
| `build/` | build artifacts (gitignored) |

## building

requires SML/NJ (tested with 110.99.9).

```bash
make build         # compile all modules via CM
make check         # CM compile only (fast smoke; no test_runner)
make test          # run test suite (see test_runner for current count)
make e2e           # emit C, compile with cc, run binary (exit code 42)
make integration   # Rmd-style scenarios under test/integration/ (needs heap)
make integration-vm # bytecode compile + sv0vm run (see parent repo `make ci`)
make test-contract-runtime  # compile requires(false) fixture; expect exit 1
make heap          # export heap image to build/sv0c
make clean         # remove .cm cache and build artifacts
```

VM backend: `Main.main ((), ["--target=vm", "path/to/file.sv0"])` writes `build/vm/<stem>.sv0b`. Multi-file projects: `Main.main ((), ["--target=vm", "--project", "path/to/dir"])` links **every `*.sv0` under `dir` recursively** (skips hidden names), writes `build/vm/main.sv0b` when `main.sv0` exists in that tree. Run bytecode via `sv0vm/scripts/run_sv0b.sml` with `SV0B` set, or `./scripts/sv0 vm-run` / `./scripts/sv0 repl` from the toolchain root.

to run the compiler directly:

```bash
sml @SMLload=build/sv0c input.sv0       # after make heap
sml @SMLload=build/sv0c --project path/to/dir   # all *.sv0 under dir (recursive)
```

or interactively:

```bash
echo 'CM.make "sources.cm"; Main.compile "input.sv0";' | sml
```

## specification

sv0c implements the specification defined in [sv0doc](../sv0doc/).
