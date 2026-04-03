# Bootstrap compiler in sv0 (`lib/`)

This tree holds **sv0 source** that mirrors slices of the SML bootstrap compiler (`sml/**/*.sml`) for the self-hosting roadmap (`task/sv0-toolchain-milestone-3-self-host.Rmd`).

Sibling directories at the `sv0c/` root (**`lexer/`**, **`parser/`**, …) hold additional transliterated modules as the surface grows. **`sml/`** is the bootstrap implementation; **`lib/`** + those dirs are the sv0 track.

## Build

From the **sv0-toolchain** root:

```bash
./scripts/sv0 bootstrap-build
```

CI reads **`lib/bootstrap-sources.list`**: one path per line (relative to `sv0c/`). If the list is missing or empty, every top-level `lib/*.sv0` is used. Each listed file is compiled to **`build/vm/<basename>.sv0b`** (basename only — keep names unique across **`lib/`**, **`lexer/`**, **`parser/`**, …).

**Layout and transliteration order:** [`LAYOUT.md`](./LAYOUT.md).

**Golden bootstrap C:** `golden/stage0/<stem>.c` — compared on `./scripts/sv0 test` / `ci` to fresh SML heap output for the matching source file.

**C snapshots:**

```bash
./scripts/sv0 emit-c lib/span_core.sv0
./scripts/sv0 self-host-capture-stage0 lib/span_core.sv0
./scripts/sv0 self-host-compare span_core
./scripts/sv0 self-host-check-golden
```

From `sv0c/` after `make heap`:

```bash
sml @SMLload=build/sv0c lib/span_core.sv0
echo 'CM.make "sources.cm"; Main.main ((), ["--target=vm", "lib/span_core.sv0"]);' | sml
```

## `span_core.sv0`

Transliterated subset of `sml/error/span.sml` / `span.sig`. `main` uses a simple `return` so the bootstrap IR/VM path emits an entry function.

## `diagnostic_core.sv0`

Transliterated subset of `sml/error/diagnostic.sig` + `diagnostic.sml`: enum `Severity`, proxy `Diagnostic` (i32 fields for code/message/span/related/help shapes), and a **byte-length** model of `format` (header, `  -->` line, optional gutter+snippet, related/help block sizes, final newline). Helpers use scalar `i32` parameters; see [`LAYOUT.md`](./LAYOUT.md) for transliteration order.

## `diagnostic_batch_core.sv0`

`hasErrors` / `errorCount` over three severity tags (0 = error), matching `diagnostic.sml` list helpers without a list type.

## `env_core.sv0`

Bounded slice of `sml/name_resolution/env.*`: two-slot module value list (numeric ids), `env_register` / `env_lookup`, `env_empty`. Exercises **multi-slot struct** arguments and returns on the VM.

## `lib/resolver_value_core.sv0`

Bounded three-slot module-value list with `lookup_value` / `resolve_path_ok` — tiny slice of `ExprPath` resolution (`sml/name_resolution/resolver.sml` + `Env.lookupValue`).

## `lib/resolver_arity_core.sv0`

Two-slot function arity table (`FnArity2`), `lookup_fn_arity` (−1 = absent), and `call_arity_ok` matching resolver checks when arity is known.

## `lib/env_scope_core.sv0`

`scope_enter` / `scope_bind` / `scope_exit` / `scope_lookup` on a bounded two-frame, two-local-per-frame model (`sml/name_resolution/env.sml` frames).

## `lib/lookup_value_core.sv0`

**Merged** `FullEnv`: module slots + scope stack in one struct; **`lookup_value`** matches SML order (innermost locals, then `modVals`). Smaller seeds (`env_core`, `env_scope_core`) remain as isolated building blocks.

## `lib/resolver_call_core.sv0`

**Merged** `FullEnvCall`: same shape as `lookup_value_core` plus embedded fn-arity table; **`ec_expr_call_ok`** = value must resolve, then arity check when a row exists (`resolver.sml` `ExprCall` path).

## `lib/lookup_type_core.sv0`

Prelude type tags (`i32`/`bool`/`unit`/`str` as numeric ids), two-slot module types, **`lookup_type_simple`** with optional **Self** (sentinel id 99) when `allow_self` is set — slice of `lookupType` for one-segment paths.

## `lexer/token_keyword_core.sv0`

Keyword discriminants (`fn` / `let` / `if`) as `i32` tags and a classifier — anchor for `sml/lexer/token.sml`. Lives under **`sv0c/lexer/`** (see repo root next to `lib/`).

## `lexer/token_delim_core.sv0`

Delimiter / punctuation tags (`( ) { } , ;`) in a band disjoint from keyword tags, plus `is_delimiter_tag`.

## `lexer/token_op_core.sv0`

Operator tags (`+ - * / == != -> => ::`) in band **20–28**, plus `is_operator_tag`.

## `lib/type_alias_core.sv0`

Two-row `tyAlias` table, `has_ty_alias_name`, `resolve_canonical_ty` with unrolled multi-hop resolution (chains into prelude id **1** = `i32` per `lookup_type_core` convention).

## `lib/lookup_type_alias_core.sv0`

**Integrated** `TypeEnvAlias`: module types + aliases together; **`lookup_type_resolved`** = `resolveCanonicalTy` then prelude or `modTys` (matches `lookupType` for a one-segment path). **Self** (99) bypasses alias expansion when `allow_self` is set — use this file as the main reference when testing the type-lookup pipeline end-to-end; keep `lookup_type_core` and `type_alias_core` as smaller pieces.

## `parser/expr_entry_core.sv0`

**parsePrimaryExpr** entry dispatch: **`parse_primary_dispatch`** maps stand-in token tags to arm ids **1–20** in the same order as `parser.sml` after **`litFromTok`**. **`lexer_keyword_tag_to_dispatch`** / **`lexer_delim_tag_to_dispatch`** tie **`token_keyword_core`** (**`if` = 3**) and **`token_delim_core`** (**`(` = 10**, **`{` = 12**) into that table; **`primary_arm_is_compound`** flags arms that always recurse. Compare directly with `parsePrimaryExpr` when extending the bootstrap parser.
