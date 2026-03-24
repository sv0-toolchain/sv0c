# sv0c compiler passes (Milestone 1)

This document summarizes what each pipeline stage does today and how errors are reported.

## Pipeline order

1. **Lexer** (`Lexer.tokenize`) — Characters to located tokens. Lexical errors are not yet classified with E01xx codes in all cases.
2. **Parser** (`Parser.parse`) — Tokens to `Ast.program`. Syntax errors raise `Fail` with a short message.
3. **Name resolution** (`Resolver.resolve`) — Validates duplicate top-level value and type names, checks unbound identifiers in expressions, and checks that type paths refer to built-in or module-level types. Single-segment value and type paths are supported; qualified paths (`a::b`) are rejected with E0305 until module resolution is complete.
4. **Type checker** (`Checker.check`) — Maps a subset of `Ast.ty` to `Types.ty`, checks `fn` bodies (literals, paths, `let` with `PatBind`, `return`, block tails, empty `unit` bodies). `requires` contracts must be `bool`-typed expressions. Raises `Fail` with **E04xx** / **E05xx** (see below).
5. **Contract analyzer** (`ContractAnalyzer.analyze`) — No-op for now; `requires` is already type-checked in the checker.
6. **IR lowering** (`Lowering.lower`) — One `Ir.block` per `ItemFn`: `StmtLet` with literal rhs, `return`, optional block tail. Other statement/expression forms raise `Fail`.
7. **C codegen** (`Codegen.emit`) — Emits C99: non-`main` functions as `static`, `main` as global. Empty IR still yields a trivial `int main(void) { return 0; }`.

## Name resolution (E03xx)

| Code   | Meaning |
|--------|---------|
| E0300  | Unbound identifier (single-segment path). |
| E0301  | Unknown type (including struct names in patterns/literals). |
| E0302  | Duplicate module-level value (e.g. two `fn` with the same name). |
| E0303  | Attempt to define a type with the same name as a built-in primitive. |
| E0304  | Duplicate module-level type. |
| E0305  | Qualified value path not implemented. |
| E0306  | `|` pattern alternatives not supported in this pass. |

Built-in types include numeric primitives, `bool`, `char`, `str`, `string`, `String`, and `unit`. `Self` is allowed only inside `trait` and `impl` bodies for type positions.

## Type checker (E04xx) and contracts (E05xx)

| Code   | Meaning |
|--------|---------|
| E0400  | Type mismatch (e.g. `return` value vs declared return type). |
| E0401  | Unbound name in type-checker value environment. |
| E0402  | Expression form not in the checked slice. |
| E0403–E0410 | Unsupported or ill-formed `let`, parameters, nested items, missing return. |
| E0501  | `ensures` not type-checked in this slice. |
| E0502  | `loop_invariant` not type-checked in this slice. |

## Unification

`Unify.unify` implements structural equality on ground `Types.ty` (and same-index `TyVar`). It is used by `Checker.expect` for compatibility checks.

## End-to-end compile

- **In-process:** `make test` runs `[e2e]` checks that build C text from a sample `fn main() -> i32 { return 42; }`.
- **Host C compiler:** `make e2e` runs `scripts/export_e2e.sml` to write `build/e2e_generated.c`, then compiles and runs the binary; the process exit code must be **42**.

## Environment representation

`Env` is an opaque datatype wrapping module-level value and type name lists plus a stack of local binding frames (inner scopes first). This avoids SML/NJ “flex record” issues with opaque signatures while keeping the implementation hidden from clients.

## Tests

Run `make test` from `sv0c/`. Sections in `test/test_runner.sml`: `[parser]`, `[resolver]`, `[checker]`, `[e2e]`, `[pipeline]`. Run `make e2e` for a full compile-and-execute smoke test.
