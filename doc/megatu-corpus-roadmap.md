# Mega-TU corpus roadmap — closing the full-language gap

**Goal:** raise the native-compose (A) mega-TU compiler from **71/98** to full coverage
of the `self-host-sv0-loop` corpus, so the composed sv0 pipeline compiles the real
library modules (and eventually itself) natively.

This document breaks the remaining work into **small, independently-shippable tasks**.
Each is scoped so a focused agent can execute it end to end without re-deriving the
whole pipeline. Do them roughly in order (later tasks depend on earlier ones being
out of the way), one commit per task.

## Where we are

`scripts/sv0-megatu-corpus-parity.sh` runs the composed compiler over all 98 seeds:

```
PASS=72  PHASEFAIL=24  PANIC=0  CCFAIL=2  RUNFAIL=0   (after Task 1)
```

Raising the number means teaching the composed pipeline the missing features, one at
a time. **Gating:** the harness fails only on PANIC (crash) or RUNFAIL (wrong output);
PHASEFAIL (clean rejection) and CCFAIL (a feature that resolves/checks but whose emit
is not complete yet) are expected during bring-up and are reported, not fatal — drive
them to zero. Note `sv0_panic` exits 1, so exit 1 with no output is counted as PANIC
(the compose main's own return-1 gate is unreachable for real seeds).

The 2 current CCFAILs are known next tasks: `string_api` (string-typed lets / string
builtin returns — the string-primitive task) and `lib/env.sv0` (Task 2, call
arg-count).

The 27 rejections, by the phase + raw error code they hit (see "Diagnostic method"):

| Cluster | Code | Count | Files (representative) |
|---|---|---|---|
| Builtins unbound | resolve=300 | ~most | `println_ok`, `string_api`, and the first error in nearly every `lib/*.sv0` |
| Call arg-count | resolve=307 | 4 | `vm_triple_call`, `enum_tuple_match`, `span`, `contract_analyzer` |
| Contracts | parse (0 items) | 1 | `old_ensures` |
| Struct-variant enums | resolve=300 | 1+ | `enum_struct_match` |
| Generic payload types | resolve=300 | 1+ | `ast_types` |
| Full library modules | resolve=300 | ~20 | `lib/types.sv0`, `lib/span.sv0`, … (use everything at once) |

## Diagnostic method (every task uses this)

The committed harness builds a normal corpus binary. For diagnosis, build a variant
whose compose main writes the raw phase + error code to a status file:

1. Generate a diag main from `sv0c/lib/megaTU-main.sv0`: replace the hardcoded
   `let source = "...";` with `read_file("/tmp/megatu_in.sv0")`, and at each phase
   gate (`vec_len(it) < 1`, `rr != 0`, `cr != 0`) `write_file("/tmp/st.txt", …)` the
   raw `rr`/`cr`. (See how `scripts/sv0-megatu-corpus-parity.sh` derives its main.)
2. `assemble-sv0-megaTU.py --main <diag-main> --out build/megaTU-diag.sv0`, then
   `sml @SMLload=sv0c/build/sv0c build/megaTU-diag.sv0 > build/megaTU-diag.c`, then
   `cc … -o build/megaTU-diag-bin`.
3. `cp <seed> /tmp/megatu_in.sv0 && build/megaTU-diag-bin` → read `/tmp/st.txt`.
4. To name the *specific* unbound identifier / bad arity, instrument the relevant
   resolver/checker return site with a `write_file` marker (revert after).

Resolver error codes: **300** = unbound value/type (E0300), **301** = unknown type,
**306** = unsupported PatOr, **307** = call arity mismatch. Checker returns `-1`
(compose main maps to exit 4) on any failure — instrument to localize.

`build/` is gitignored; diag artifacts never get committed.

## Recurring bug classes (context for every task)

Prior increments hit the same handful of transliteration bugs. Expect these:

1. **def-vs-use token mismatch.** Tables built from *definitions* are compared to
   *use* tokens by index; two occurrences of the same name have different token
   indices. Fix = compare by **source text** (`handle_to_str` / `tok_str`), as done
   for `env_lookup_str`, `enum_tag_lookup_str`, `megatu_is_user_type`.
2. **non-contiguous arena children.** A construct's children (block statements, match
   arms, **call args**) are interleaved with their sub-expressions, so
   `first + i` only finds child 0. Fix = record an ordered index list in the shared
   `pp` pool with the offset in an unused node field, read via `block_stmt_index`
   (see `parser.sv0 block_stmt_sidecar_push`, and the block/match-arm precedent).
3. **encoding drift.** The same concept has two encodings (AST binop tags vs codegen
   `op_of`; lit_tag 1 vs 5 for bool). Fix = make the pipeline use one consistent
   encoding — usually the AST one the emitter (`megatu_binop_c`) already follows.
4. **stub helpers never wired.** A correct helper exists but the phase never calls it
   (`is_intrinsic`, `last_stmt_returns_arena`). Fix = wire it in.
5. **local pool discarded.** A sub-parser writes into a fresh local `pp`/`sf` that's
   thrown away while a node records an offset into it. Fix = thread the shared pool
   (struct-lit `sf_names`, match pattern `pp`).
6. **compose-main de-stub.** Name/type/operator resolution that `codegen.sv0` stubs is
   overridden in `sv0c/lib/megaTU-main.sv0` (it sees both modules), keeping
   `codegen.sv0` standalone-compilable. Emit-side gaps usually belong here.

## Definition of done (every task)

- The task's repro seed(s) now report `PASS` in `scripts/sv0-megatu-corpus-parity.sh
  --verbose`, and the total PASS count goes up (never down).
- Any changed `lib/*.sv0` module still compiles standalone and its self-tests pass;
  refresh its `lib/golden/stage0/<m>.c` **and** vm-parity `.sv0b`
  (`bash scripts/capture_vm_parity_goldens.sh`).
- Full `./scripts/sv0 test` is green — **self-host-sv0-loop stays 98/98 vs SML**.
- Commit (no AI/agent attribution — see the git hooks), bump the parent `sv0c`
  gitlink + README pin, push submodule then parent, confirm both CI runs pass.
- Update the PASS number in this doc's "Where we are".

---

## Tasks

### Task 1 — Register builtins in the resolver  *(DONE — sv0c bump after this doc)*

**Done:** `resolve_expr`'s tag-1 handler now accepts a 1-segment `is_intrinsic`
name; `read_file`/`write_file`/`read_dir` were added to the registry; the emit maps
a builtin call's negative sentinel to `sv0_<name>` (`megatu_builtin_name`), and the
string-literal double-quote bug (`handle_to_str` keeps the source quotes) was fixed.
Also removed a latent `megatu_val_name` bug that rendered any value handle in
500..599 as `f<n>` (real token indices reach that range in large modules — it was
turning `env.sv0`'s calls into `f24(...)`). `println_ok` now PASSES.

**Not in Task 1 (split out):** `string_api` needs string-typed `let`s and
string-returning builtin call types → folded into the **string-primitive type**
task (a sibling of "let types for non-struct/enum inits" below). Original notes:

**Gap.** `resolve_program` (resolver.sv0) never wires the intrinsic registry, so
`println`, `string_len/eq/concat/char_at/substr`, `vec_new/push/len/get/set`,
`box_new/box_deref`, `read_file/write_file/read_dir`, `no_alias`, `old`,
`forall`/`exists` all resolve as **unbound (300)**. The helpers already exist —
`is_intrinsic(name)` (resolver.sv0 ~113) and `intrinsic_arity(name)` (~134) — they're
just not called (`register_all_intrinsics` is dead; and note `read_file`/`write_file`/
`read_dir` may need adding to `is_intrinsic`, cross-check `build_builtin_map` in
lowering.sv0).

**Repro.** `test/data/golden/pass/println_ok.sv0`, `test/integration/string_api/string_api.sv0`.

**Fix.** In `resolve_expr`'s value-path (tag 1, ~743) and call (tag 4, ~773) handlers:
when a 1-segment name isn't found in `mod_vals`/`frames`, accept it if
`is_intrinsic(name)`; for calls, use `intrinsic_arity(name)` in the arity check
instead of only `res_lookup_fn_arity_str`. Prefer the string-based `is_intrinsic`
over `register_all_intrinsics` (which needs real token handles the source may lack).

**Why first.** Nearly every `lib/*.sv0` module uses `vec_*`/`box_*`/`string_*`; this
clears their *first* error (they'll then surface later gaps for Phase 2).

**Acceptance.** `println_ok` + `string_api` PASS.

### Task 2 — Fix call argument count (nested-call / ctor args)

**Gap.** An `ExprCall` node's `args_count` (`ed3`) counts **arena nodes pushed while
parsing the args**, not the number of arguments. `bump(bump(bump(0)))` reports
`args=5` (arity 1) → **307**; `classify(Msg::Pair(20,22))` reports `args=2`. Simple
flat calls (`add(2,5)`) happen to be right because each arg is one node.

**Repro.** `test/vm-parity/programs/vm_triple_call.sv0`, `test/integration/enum_tuple_match/enum_tuple_match.sv0`.

**Fix.** This is the call-args instance of bug class #2 (non-contiguous children).
`parse_arg_list` (parser.sv0 ~1205) must store the **argument count** (number of
top-level args it parsed) and an ordered list of the arg root node indices (each arg
is the last node of its sub-tree) — reuse the `block_stmt_sidecar_push` mechanism,
stashing the arg-root list in `pp` with the offset in an unused `ExprCall` field.
Then the resolver (arity check + arg walk), checker, and lowering (`lower_tag_call`
arg iteration) read the real count and roots via `block_stmt_index`. Keep flat calls
working (they already pass — don't regress `add`/`f(3)`).

**Acceptance.** `vm_triple_call`, `enum_tuple_match` PASS (also unblocks `span`,
`contract_analyzer` at this phase).

### Task 3 — Parse function contracts (`requires` / `ensures` / `old` / `result`)

**Gap.** `old_ensures.sv0` (`fn inc(x) requires(true) ensures(result == old(x)+1)`)
makes the composed parser return **0 items**. Contract clauses on a fn signature
break parsing in the composed pipeline (the compose main threads `fcb`/`fcr` contract
arenas — check they're wired and that `parse_contracts` runs).

**Repro.** `test/data/golden/pass/old_ensures.sv0` (and `forall_requires`,
`exists_requires` already pass — compare).

**Fix.** Diagnose why parse yields 0 items for a contract fn; likely a parser gap in
the composed path or a compose-main arena wiring issue. After parse, `result` and
`old` are contract builtins — coordinate with Task 1 (`old` is intrinsic arity 1;
`result` is the ensures-result binding). Contracts lower to `sv0_requires`/
`sv0_ensures` calls (codegen already emits these).

**Acceptance.** `old_ensures` PASS.

### Task 4 — Struct-variant enum variants and patterns

**Gap.** `enum_struct_match.sv0` uses a struct-shaped variant `Rect { w: i32, h: i32 }`,
constructs it `Shape::Rect(3, 4)`, and matches `Shape::Rect { w, h }` — resolve=300.
Nullary and tuple variants already work (Tasks done earlier); struct variants add
named-field payloads.

**Repro.** `test/integration/enum_struct_match/enum_struct_match.sv0`.

**Fix.** Extends the enum work: the variant's named fields need typedef slots + field
names, the constructor stores by field, and `Shape::Rect { w, h }` patterns bind by
field name. Touches resolver (variant field binds), checker (pattern field types),
lowering (ctor + pattern field access), and possibly the emit field-name resolver.
Larger — keep tuple/nullary variants working.

**Acceptance.** `enum_struct_match` PASS.

### Task 5 — Generic payload types (`Box<T>`, `Vec<T>`)

**Gap.** `ast_types.sv0` uses `Box<Ty>`, `Box<Expr>`, `Vec<i32>` as enum-variant
payload types — resolve=300 (the resolver/checker don't resolve the generic type
application, or `Box`/`Vec` as type names in that position).

**Repro.** `test/integration/ast_types/ast_types.sv0`.

**Fix.** Resolve `Box<T>`/`Vec<T>` type applications in payload/field/param positions
(`Box`/`Vec` are known type constructors → C `int` handle in the current surface, per
`ast_ty_to_c_string`). Check the resolver's type-path handling and the type arena's
`type_arg_count` (TyName d3).

**Acceptance.** `ast_types` PASS.

### Phase 2 — The full library modules  *(iterative; after Tasks 1–5)*

The ~20 `lib/*.sv0` seeds use everything at once. Once Tasks 1–5 land, **re-run the
harness and diagnose them one module at a time**, smallest first. Suggested order by
size: `span`, `diagnostic`, `env`, `types`, `unify`, `ir`, `ast`, `include_expand`,
`link`, `bytecode`, `lexer`, `contract_analyzer`, `resolver`, `checker`, `vm_codegen`,
`codegen`, `lowering`, `parser`, `main`, `driver`.

Each becomes its own small task: diagnose the module's *next* error (300/301/307/…),
identify the one feature it needs (likely another instance of a recurring bug class),
fix, verify parity, commit. Expect features like: multi-segment/module paths, tuple
types and tuple returns, array/index expressions, `for` loops over ranges, casts,
generics on functions, and `impl`/method calls. Split any that turn out large.

**Stop rule.** A module that needs a genuinely big feature (e.g. full generics or
trait resolution) should be parked with a note here rather than forced — keep tasks
bite-sized.
