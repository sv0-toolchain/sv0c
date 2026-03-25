# sv0c compiler passes (Milestone 1)

This document summarizes what each pipeline stage does today and how errors are reported.

## Pipeline order

1. **Lexer** (`Lexer.tokenize`) — Characters to located tokens. Lexical errors are not yet classified with E01xx codes in all cases.
2. **Parser** (`Parser.parse`) — Tokens to `Ast.program`. Syntax errors raise `Fail` with a short message. **Expression parsing** threads `allowStruct`: in positions where a `{` must start a block or match arms (`match`, `if`, `while`, `for` iterator), a bare identifier must **not** be parsed as the start of a struct literal `name { ... }` (see `parseRangeExpr` / `parsePrimaryExpr` in `parser.sml`). **Patterns:** `IDENT` followed by `::` is parsed as a qualified path (enum/struct pattern suffix), not as a simple `PatBind`.
3. **Name resolution** (`Resolver.resolve`) — Validates duplicate top-level value and type names, checks unbound identifiers, and validates type paths against the module type environment. **Value paths** may be multi-segment for **enum variant constructors** (`Enum::Variant`) and similar keys registered from `ItemEnum`. **Intrinsics:** `println` is pre-registered (arity 1); a top-level `fn println` collides with **E0302**. **Type paths** cover struct/enum names for literals and patterns. Cross-file `module::item` resolution is deferred (Milestone 1 Phase 8 scope).
4. **Type checker** (`Checker.check`) — Maps `Ast.ty` to `Types.ty` including **user struct and enum names** (`TyStruct` / `TyEnum`). Checks `fn` bodies including **struct literals** (`ExprStruct`), **field access** (`ExprField`), **enum construction** (`Enum::Variant(...)`), **`match`** on enum, `bool`, and `i32`, **parenthesized unary tuples** (`ExprTuple` of one element, for `(expr)` grouping), **`as` casts** between **integral** types only, **string literals** (`TyString`), **`println("literal")`**, and **`?`** on enums shaped **`Ok(T)` / `Err(E)`** with one field each, where the **enclosing function’s return type is that same enum** (early error return on `Err`). Still covers **`while` / `for` over `lo..hi` / `loop`** and **`break` / `continue` only inside loops**. `requires` contracts must be `bool`-typed. Raises `Fail` with **E04xx** / **E05xx** (see below).
5. **Contract analyzer** (`ContractAnalyzer.analyze`) — No-op for now; `requires` is already type-checked in the checker.
6. **IR lowering** (`Lowering.lower`) — One `Ir.block` per `ItemFn`: `StmtLet`, `return`, `if`, calls, **`while`**, **`for`** (desugared per Q4 to a counter + `while`), **`loop`**, **`break` / `continue`**, **`loop_invariant(e)` on `while`**, **struct** values, **enum** values, **`match`** as **`if` chains**, **`as` casts** (`Ir.Unop` with a C cast prefix), **`println`** → **`sv0_println`**, **`?`** → load scrutinee, compare **`.tag`** to **`Err`**, **return** whole enum on error else **store `p0`** into a pre-declared payload temp (so it stays in scope for `let x = …?`). **`let`** from a non-struct/enum RHS uses **`DeclNamed` + `Store`** when the inferred C type is not `int`. Typedef names and C type hints flow through `Ir.program`.
7. **C codegen** (`Codegen.emit`) — Emits C99: a **typedef prelude** for struct and enum carrier types, **`static`** non-`main` functions, global `main`. Emits **`int8_t` / `int16_t` / …** via `stdint.h` (included from `sv0_runtime.h`) for integral casts. **`Ir.VString`** → C string literal (escaped). **`Ir.FieldAccess`** for enum payload loads. Empty IR still yields a trivial `int main(void) { return 0; }`.

## Name resolution (E03xx)

| Code   | Meaning |
|--------|---------|
| E0300  | Unbound identifier (single-segment path). |
| E0301  | Unknown type (including struct names in patterns/literals). |
| E0302  | Duplicate module-level value (e.g. two `fn` with the same name). |
| E0303  | Attempt to define a type with the same name as a built-in primitive. |
| E0304  | Duplicate module-level type. |
| E0305  | Reserved (historical); multi-segment **value** paths are used for `Enum::Variant` in this milestone. Arbitrary module-qualified paths remain future work. |
| E0306  | `|` pattern alternatives not supported in this pass. |

Built-in types include numeric primitives, `bool`, `char`, `str`, `string`, `String`, and `unit`. `Self` is allowed only inside `trait` and `impl` bodies for type positions.

## Type checker (E04xx) and contracts (E05xx)

| Code   | Meaning |
|--------|---------|
| E0400  | Type mismatch (e.g. `return` value vs declared return type). |
| E0401  | Unbound name in type-checker value environment. |
| E0402  | Expression form not in the checked slice. |
| E0403–E0410 | Unsupported or ill-formed `let`, parameters, nested items, missing return. |
| E0414  | `break` outside a loop. |
| E0415  | `break expr` (value) not supported in this slice. |
| E0416  | `continue` outside a loop. |
| E0502  | `loop_invariant` as an `fn`-level contract not supported; use `loop_invariant(...)` on `while` (parsed into `ExprWhile`, type-checked as `bool`, lowered to `sv0_requires`). |
| E0520  | `ensures` must not use `result` when the return type is `unit`. |
| E0440  | `as`-cast only between integral types (`i8`–`i128`, `u8`–`u128`, `isize`, `usize`). |
| E0441  | **`?`:** enum must have **`Ok(T)`** and **`Err(E)`** with exactly **one** field each. |
| E0442  | **`?`:** scrutinee enum must equal the **function return** enum type. |
| E0443  | **`?`:** enclosing function return must be that **enum** (not `i32` alone). |
| E0444  | **`println`** requires a **single string literal** argument. |
| E0445  | **`?`** operand must be an **enum**. |
| E0446  | Multi-element **tuples** are not in this slice (unary `(e)` is peeled to `e`). |

## Structs and enums (Phase 5)

- **Structs:** `ItemStruct` defines fields; literals must use the struct type path; field reads compile to member access in C.
- **Enums:** `ItemEnum` defines variants (unit, tuple, or struct payload). Each variant is registered as a value with arity. Runtime representation is a C `struct` with a **`tag`** discriminator and payload fields (`p0`, …) as needed for the largest variant shape the compiler uses for that enum.
- **`match`:** Enum arms use `PatEnum` patterns; arms are lowered to comparisons on `.tag` (and bindings from payload locals). `bool` / `i32` matches use literal and wildcard/bind patterns supported by the checker.

## Builtins and casts (Phase 6)

- **`as`:** C-style truncation / sign-extension between integrals; emitted as `(target_ctype)expr`. `bool` and floats are rejected.
- **`println`:** Calls **`sv0_println`** from [`sv0_runtime.h`](../runtime/sv0_runtime.h) (`printf` + newline). Only string **literals** for now.
- **`?`:** Rust-style on a user-defined enum with **`Ok`** / **`Err`** variants (single payload each). The **function must return that enum**; on `Err`, the lowered code **returns** the enum value from the function.
- **Deferred (not Phase 6):** **`Vec` / `Box` / heap `string` APIs**, generic **`Option`/`Result`**, and **method dispatch** — need more type system and runtime surface than this slice.

## Unification

`Unify.unify` implements structural equality on ground `Types.ty` (including `TyStruct` / `TyEnum` names) and same-index `TyVar`. It is used by `Checker.expect` for compatibility checks.

## End-to-end compile

- **In-process:** `make test` runs `[e2e]` checks including casts, `println`, `?` on enums, struct/enum/match, and smaller slices (e.g. `return 42`).
- **Host C compiler:** `make e2e` runs `scripts/export_e2e.sml` to write `build/e2e_generated.c`, then compiles and runs the binary; the process exit code must be **42**.

## Environment representation

`Env` is an opaque datatype wrapping module-level value and type name lists plus a stack of local binding frames (inner scopes first). This avoids SML/NJ “flex record” issues with opaque signatures while keeping the implementation hidden from clients.

## Tests

Run `make test` from `sv0c/`. Sections in `test/test_runner.sml`: `[parser]`, `[resolver]`, `[checker]`, `[e2e]`, `[pipeline]`. Run `make e2e` for a full compile-and-execute smoke test.

**Block parsing:** A `while` / `for` / `loop` expression that is **not** the block’s tail must be followed by **`;`** before the next statement (same rule as other expression statements).
