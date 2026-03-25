# sv0c compiler passes (Milestone 1)

This document summarizes what each pipeline stage does today and how errors are reported.

It aligns with the pipeline stages in the [sv0 compiler vision and design](http://development.sasankvishnubhatla.net/tcowmbh/task/sv0-compiler-vision-and-design.html) document (lexer → parser → name resolution → type checker → contract analysis → **sv0-IR** → backends). **Note:** the vision describes **SSA-form** IR for analysis and optimization; the Milestone 1 compiler lowers to an **imperative IR** with labeled blocks (`Ir.program`). SSA remains future work; the C backend consumes this IR today.

## Pipeline order

1. **Lexer** (`Lexer.tokenize`) — Characters to located tokens. Lexical errors are not yet classified with E01xx codes in all cases.
2. **Parser** (`Parser.parse`) — Tokens to `Ast.program`. Syntax errors raise `Fail` with a short message. **Expression parsing** threads `allowStruct`: in positions where a `{` must start a block or match arms (`match`, `if`, `while`, `for` iterator), a bare identifier must **not** be parsed as the start of a struct literal `name { ... }` (see `parseRangeExpr` / `parsePrimaryExpr` in `parser.sml`). **Patterns:** `IDENT` followed by `::` is parsed as a qualified path (enum/struct pattern suffix), not as a simple `PatBind`.
3. **Name resolution** (`Resolver.resolve`) — Same as before for a single translation unit, plus **`use module::item;`** (see **E0309**): value imports copy arity from the mangled target (`module__item` after linking); type and enum imports register **Env** type aliases and per-variant ctor aliases. **Multi-file** programs are built only via **`sv0c --project dir`**: `Link.linkProjectDir` reads every `*.sv0` in `dir`, applies optional leading `module name;`, mangles top-level symbols to `name__symbol` (except the C entry **`fn main`**, which stays `main`), then concatenates items. **Lowering** uses `Resolver.peekImportAliases` / `Lowering.setImportAliases` so calls like `bump(...)` after `use lib::bump` emit **`lib__bump`**. **Intrinsics** and **`Enum::Variant`** paths behave as before. Some errors use **`raise Diagnostic.Diag`** (**`E0300`**, **`E0301`**, **`E0309`**) so `Main` can print **`Diagnostic.format`** with a source snippet when compiling a single file.
4. **Type checker** (`Checker.check`) — Maps `Ast.ty` to `Types.ty` including **user struct and enum names** (`TyStruct` / `TyEnum`). Checks `fn` bodies including **struct literals** (`ExprStruct`), **field access** (`ExprField`), **enum construction** (`Enum::Variant(...)`), **`match`** on enum, `bool`, and `i32`, **parenthesized unary tuples** (`ExprTuple` of one element, for `(expr)` grouping), **`as` casts** between **integral** types only, **string literals** (`TyString`), **`println("literal")`**, **`?`** on enums shaped **`Ok(T)` / `Err(E)`** with one field each (same enum as the function return), and **contract-only** forms when **`inContractExpr`** is set for **`requires` / `ensures`**: **`old(x)`** (parameter `x` only), **`forall(i in lo..hi, p)`** / **`exists(...)`** (half-open **`i32`** range, **`p : bool`**), **`no_alias(&a, &b)`** / **`&mut`** (same inner **`T`**), and **`&` / `&mut`** on a simple parameter name. Still covers **`while` / `for` over `lo..hi` / `loop`** and **`break` / `continue` only inside loops**. `requires` contracts must be `bool`-typed. Messages use **`E04xx` / `E05xx`** and often include **`(hint: …)`** after the code (see table). **`Fail`** is caught at the **`sv0c`** CLI and printed as **`sv0c error: …`** ([`main.sml`](../src/main.sml)).
5. **Contract analyzer** (`ContractAnalyzer.analyze`) — No-op for now; `requires` is already type-checked in the checker.
6. **IR lowering** (`Lowering.lower`) — One `Ir.block` per `ItemFn`: `StmtLet`, `return`, `if`, calls, **`while`**, **`for`** (desugared per Q4 to a counter + `while`), **`loop`**, **`break` / `continue`**, **`loop_invariant(e)` on `while`**, **struct** values, **enum** values, **`match`** as **`if` chains**, **`as` casts** (`Ir.Unop` with a C cast prefix), **`println`** → **`sv0_println`**, **`?`** → load scrutinee, compare **`.tag`** to **`Err`**, **return** whole enum on error else **store `p0`** into a pre-declared payload temp (so it stays in scope for `let x = …?`). **Phase 9 contracts:** for each parameter **`x`** mentioned in **`old(x)`**, emit **`_sv0old_x`** initialized from **`x`** after any **`_sv0ret`** decl and **before** **`requires`**; **`old(x)`** lowers to **`Load _sv0old_x`**. **`forall` / `exists`** lower to a **`while`** with an accumulator (**`exists`** breaks on first true predicate). **`no_alias`** → **`sv0_no_alias`** with **`Ir.VAddrOf`** arguments. **`requires` / `ensures` IR** is lowered **after** **`tmpCtr` reset** in this order: **`requires`** clauses, **function body**, then **`ensures`** clauses (so temporaries do not collide). **`let`** from a non-struct/enum RHS uses **`DeclNamed` + `Store`** when the inferred C type is not `int`. Typedef names and C type hints flow through `Ir.program`.
7. **C codegen** (`Codegen.emit`) — Emits C99: a **typedef prelude** for struct and enum carrier types, **`static`** non-`main` functions, global `main`. Emits **`int8_t` / `int16_t` / …** via `stdint.h` (included from `sv0_runtime.h`) for integral casts. **`Ir.VString`** → C string literal (escaped). **`Ir.VAddrOf`** → **`(&name)`**. **`Ir.FieldAccess`** for enum payload loads. Empty IR still yields a trivial `int main(void) { return 0; }`.

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
| E0309  | **`use`** path invalid or unknown symbol (expects `module::item` in this slice). |
| E0310–E0311 | Import alias target missing (value or type). |
| E0320–E0321 | **`Link`**: bad `module` path or empty project directory. |

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
| E0521  | **`old(...)`** only in **`requires` / `ensures`**. |
| E0522  | **`old`** needs a single **parameter** name (not a local, not a compound expression). |
| E0523  | **`forall` / `exists`:** only in contracts; domain must be **`lo..hi`**; body **`bool`**. |
| E0524  | **`no_alias`** needs **`&T` / `&mut T`** with matching inner **`T`**. |
| E0525  | **`&` / `&mut`** only in contract expressions (for **`no_alias`**). |
| E0526  | **`no_alias(...)`** only in **`requires` / `ensures`**. |
| E0540  | Lowering: unsupported unary (beyond slice). |
| E0541  | Lowering: **`&`** not applied to a simple name (should not reach lowering if checker is consistent). |

## Structs and enums (Phase 5)

- **Structs:** `ItemStruct` defines fields; literals must use the struct type path; field reads compile to member access in C.
- **Enums:** `ItemEnum` defines variants (unit, tuple, or struct payload). Each variant is registered as a value with arity. Runtime representation is a C `struct` with a **`tag`** discriminator and payload fields (`p0`, …) as needed for the largest variant shape the compiler uses for that enum.
- **`match`:** Enum arms use `PatEnum` patterns; arms are lowered to comparisons on `.tag` (and bindings from payload locals). `bool` / `i32` matches use literal and wildcard/bind patterns supported by the checker.

## Builtins and casts (Phase 6)

- **`as`:** C-style truncation / sign-extension between integrals; emitted as `(target_ctype)expr`. `bool` and floats are rejected.
- **`println`:** Calls **`sv0_println`** from [`sv0_runtime.h`](../runtime/sv0_runtime.h) (`printf` + newline). Only string **literals** for now.
- **`?`:** Rust-style on a user-defined enum with **`Ok`** / **`Err`** variants (single payload each). The **function must return that enum**; on `Err`, the lowered code **returns** the enum value from the function.
- **Deferred (not Phase 6):** **`Vec` / `Box` / heap `string` APIs**, generic **`Option`/`Result`**, and **method dispatch** — need more type system and runtime surface than this slice.

## Contract builtins (Phase 9)

- **`result`:** Already supported in **`ensures`** (lowering maps to the return-value slot when needed).
- **`old(x)`:** Snapshot of parameter **`x`** at function entry (`_sv0old_x` in C).
- **`forall` / `exists`:** Bounded **`i32`** iteration with runtime checking (not a verifier).
- **`no_alias`:** Emits **`sv0_no_alias`** (pointer inequality; approximation only). See [`sv0_runtime.h`](../runtime/sv0_runtime.h).
- **Diagnostics:** Prefer stable **`E05xx`** strings with **`(hint: …)`** for contract mistakes. Resolver **`E0300` / `E0301` / `E0309`** raise **`Diagnostic.Diag`**; other stages still often use raw **`Fail`** until migrated.
- **Golden tests:** Sources under [`test/data/golden/`](../test/data/golden/) (`fail/` = checker errors with hints; `pass/` = compile + **`cc`** + run with exit **0**). For **`pass/`**, optional sidecars next to the **`.sv0`** file: **`<stem>.golden`** lists one rule per non-comment line — plain text = required substring in emitted C; a line starting with **`!`** = forbidden substring (must not appear); **`<stem>.stdout`**, if present, is the expected program stdout after trim (e.g. **`println`** output), compared to the captured run output.

## Unification

`Unify.unify` implements structural equality on ground `Types.ty` (including `TyStruct` / `TyEnum` names) and same-index `TyVar`. It is used by `Checker.expect` for compatibility checks.

## End-to-end compile

- **In-process:** `make test` runs `[e2e]` (C string assertions), **`[golden]`** (file-based **`test/data/golden/**`), and smaller slices (e.g. `return 42`).
- **Host C compiler:** `make e2e` runs `scripts/export_e2e.sml` to write `build/e2e_generated.c`, then compiles and runs the binary; the process exit code must be **42**.
- **Rmd integration:** `make integration` (or `task/sv0c-milestone-1/02-integration-test.sh`) runs **`sv0c/test/integration/**`** scenarios end-to-end.

## Environment representation

`Env` is an opaque datatype wrapping module-level value and type name lists plus a stack of local binding frames (inner scopes first). This avoids SML/NJ “flex record” issues with opaque signatures while keeping the implementation hidden from clients.

## Tests

Run `make test` from `sv0c/`. Sections in `test/test_runner.sml`: `[parser]`, `[resolver]`, `[checker]`, `[e2e]`, **`[golden]`**, `[pipeline]`. Run `make e2e` for a full compile-and-execute smoke test.

**Block parsing:** A `while` / `for` / `loop` expression that is **not** the block’s tail must be followed by **`;`** before the next statement (same rule as other expression statements).
