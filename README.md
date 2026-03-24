# sv0c -- sv0 bootstrap compiler

the sv0 bootstrap compiler, implemented in Standard ML (SML/NJ).

## purpose

sv0c reads `.sv0` source files and compiles them to C source code, which is then compiled to native binaries by a standard C compiler (`cc`/`gcc`). this is the milestone 1 bootstrap compiler that will eventually be rewritten in sv0 itself.

## architecture

```
sv0 source (.sv0)
     |
     v
  Lexer         src/lexer/
     |
     v
  Parser        src/parser/ + src/ast/
     |
     v
  Name Resolver src/name_resolution/
     |
     v
  Type Checker  src/type_checker/
     |
     v
  Contract      src/contract_analyzer/
  Analyzer
     |
     v
  IR Lowering   src/ir/
     |
     v
  C Backend     src/backend/c/
     |
     v
  C source -> cc -> native binary
```

## directory structure

| directory | description |
|---|---|
| `src/lexer/` | tokenizer -- keywords, identifiers, literals, operators |
| `src/parser/` | recursive descent parser producing AST |
| `src/ast/` | AST type definitions (SML datatypes) |
| `src/name_resolution/` | scope resolution, import handling, module graph |
| `src/type_checker/` | type inference, trait resolution, overload resolution |
| `src/contract_analyzer/` | contract validation, runtime check insertion |
| `src/ir/` | sv0-IR definition and AST-to-IR lowering |
| `src/backend/c/` | sv0-IR to C99 code generation |
| `src/error/` | error reporting with source spans |
| `test/` | test suite |
| `build/` | build artifacts (gitignored) |

## building

requires SML/NJ (tested with 110.99.9).

```bash
make build         # compile all modules via CM
make test          # run test suite (15 tests)
make heap          # export heap image to build/sv0c
make clean         # remove .cm cache and build artifacts
```

to run the compiler directly:

```bash
sml @SMLload=build/sv0c input.sv0       # after make heap
```

or interactively:

```bash
echo 'CM.make "sources.cm"; Main.compile "input.sv0";' | sml
```

## specification

sv0c implements the specification defined in [sv0doc](../sv0doc/).
