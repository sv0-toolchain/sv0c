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
PASS=79  PHASEFAIL=2  PANIC=0  CCFAIL=17  RUNFAIL=0   (Phase 2: string-escape fix)
```

The composed compiler again has **zero crashes and zero wrong-output** on the
corpus; every non-PASS is a clean rejection or an incomplete emit.

The composed compiler now **never crashes and never emits wrong output** on the
corpus — every non-PASS is a clean rejection (PHASEFAIL) or an incomplete emit
(CCFAIL, a feature whose lowering/emit is not finished). PASS is the metric.

Raising the PASS number means teaching the composed pipeline the missing features,
one at a time. **This harness is a progress monitor, not a CI gate** (the real gate
is `./scripts/sv0 test`, keeping self-host-sv0-loop at 98/98 via SML). It gates only
on PASS not regressing below a high-water mark (`MIN_PASS`); PANIC/RUNFAIL are printed
as triage signals. This is deliberate: fixing an early phase floods more modules into
later phases, so PHASEFAIL falls while CCFAIL/PANIC/RUNFAIL on the newly-reached but
still-unsupported modules temporarily rise — those modules never passed, so it is
progress, not regression. `sv0_panic` exits 1, so a no-output exit 1 is a PANIC.

After Task 2, the ~22 previously-`PHASEFAIL(resolve)` library modules now reach
lower/emit and surface their next gaps (6 PANIC, 14 CCFAIL, 1 RUNFAIL) — this is the
Phase 2 work below. `string_api`/`ast_types` CCFAIL on the string / generic-type
tasks; `env.sv0` RUNFAILs on a deeper behavioral gap.

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

### Task 2 — Fix call argument count (nested-call / ctor args)  *(DONE)*

**Done:** `parse_arg_list` now records each argument's root node index in a pp
sidecar (`block_stmt_sidecar_push`); the regular `ExprCall` stores the real arg
count in d3 and the sidecar offset in d4; the resolver, checker
(`synth_expr_call_simple`), and lowering (`lower_tag_call`) read arg k via
`block_stmt_index`. Special desugared calls (forall/exists, assert) and method
calls keep d4 = 0 (contiguous fallback). `vm_triple_call` and `enum_tuple_match`
now PASS; PASS rose 72 → 75 and the ~22 library modules moved past resolve into
Phase 2 gaps.

**Gap (original).** An `ExprCall` node's `args_count` (`ed3`) counts **arena nodes
pushed while parsing the args**, not the number of arguments. `bump(bump(bump(0)))` reports
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

**Phase 2 log:**
- `env.sv0` (was RUNFAIL): the `while` condition was lowered with `lower_expr_to_value`,
  which hoists the test into a temp `Assign`ed *before* the loop — so `while (_t)`
  re-checked a frozen value and any loop that iterated ran forever off the end of a
  Vec. Fixed to mirror SML `lowerExprWithInstrs` (new `lower_cond_expr`): lower the
  condition's operands to values but keep the top-level Binop/Unop as an Expr, then
  `while (cond) { body }` for a simple condition (C re-evaluates it) or
  `while (1) { cond_is; ct = cond; if (!ct) break; body }` for a complex one. General
  fix (any iterating `while`), not env-specific. RUNFAIL → 0, PASS 75 → 76.

- Compile-time PANICs (6 modules) triaged to two causes:
  - `box: pool exhausted` (5 largest modules — vm_codegen/checker/lowering/parser/
    driver): the runtime box pool was a fixed 64K words, but the mega-TU box_new's
    every IR node while lowering, so a multi-thousand-line module overflows it.
    Bumped `SV0_BOX_POOL_SIZE` to 16M words (demand-paged BSS). Those 5 now emit
    (still CCFAIL on deeper feature gaps, but no longer crash).
  - `vec: index out of bounds` (`span.sv0`): `return P { ... }` — the struct-literal
    field-name pool was empty because `parse_return_expr` passed a throwaway `sf`
    to `parse_range_expr` instead of the real `sf_names` (the same discarded-local
    pool bug as struct literals in other positions). Threaded `sf_names`.
  Result: PANIC 6 → 0. (Latent: a few other `parse_range_expr` call sites still
  pass a local `sf`; those are `allow_struct = 0` condition parsers, so struct
  literals can't appear there — not currently reachable, worth tidying later.)

- String primitive type (`string_api`, was CCFAIL): the parser discards the `let`
  type annotation, so a `let s: string = "hello"` declared `int s` and every string
  flowed as int. Infer the string type from the init (`lower_init_is_string`: a
  string literal or a string-returning builtin call — string_concat/substr/read_file/
  read_dir), declare it with a sentinel ctype (`lower_string_cty`) that the emit maps
  to `const char*` (`megatu_ty_name`), and type a string-returning builtin call's
  result temp via `megatu_builtin_ret_cty`. `string_api` PASSes; PASS 76 → 78. This
  advanced `lexer.sv0` past its string CCFAIL into a runtime failure (its next bug).
- String-escape emission (`lexer.sv0`, was RUNFAIL): a string literal's `VString`
  handle is a *source* token whose body still holds escape sequences as source
  characters (`\n` is backslash + `n`), but the emit ran `c_escape_string` over it,
  doubling the backslash so the C literal got a literal backslash-n instead of a
  newline — `skip_line_comment` then never found the newline (returned the wrong
  index). sv0 string escapes already match C, so emit the inner body verbatim
  (strip only the outer quotes). `lexer.sv0` PASSes; PASS 78 -> 79, RUNFAIL -> 0.

Each remaining becomes its own small task: diagnose the module's *next* error (300/301/307/…),
identify the one feature it needs (likely another instance of a recurring bug class),
fix, verify parity, commit. Expect features like: multi-segment/module paths, tuple
types and tuple returns, array/index expressions, `for` loops over ranges, casts,
generics on functions, and `impl`/method calls. Split any that turn out large.

- Box builtins (`ast.sv0`, was CCFAIL): `box_new`/`box_deref` (builtin ids 12/13)
  lowered to calls to `sv0_box_new`/`sv0_box_deref`, which the runtime does not
  export (it has `sv0_box_alloc`/`sv0_box_store`/`sv0_box_load` plus the generic
  `sv0__box_new_raw`/`sv0__box_deref_raw` macros). The golden SML codegen boxes
  *flat* multi-word struct values (`store .tag`, `.p0`, …), but the composed
  compiler represents every value as a single-word `int` handle, so boxing is just
  "box one word". Mapped `box_new` → `sv0__box_new_raw` and `box_deref` →
  `sv0__box_deref_raw(h, int)` (the macro needs the target C type; always `int`
  here) in `megatu_builtin_name` + the Call emit. `ast.sv0` PASSes; PASS 79 → 80.
  This also cleared the box errors on `ir.sv0`/`unify.sv0`, narrowing each to a
  single remaining class (see parked item below).

- String-returning user functions (`diagnostic.sv0` et al., was CCFAIL): a `let x =
  user_fn(..)` where `user_fn` returns `string` declared `int x`, and the call's
  result temp was `int _t = user_fn(..)`, so every string flowing out of a user call
  hit `-Wint-conversion` against the `const char*` runtime string API. The prior
  string task only recognised string literals + string-returning *builtins*
  (`lower_init_is_string`); this adds the *user-function* case. New helper
  `lower_call_ret_is_string` (lowering.sv0): a 1-seg ExprCall whose callee name (by
  TEXT) matches a top-level ItemFn with `has_ret` and a return type token of
  `string` (via `lower_scan_fn_ret_ty_tok`). Wired at both `let` handlers (declare
  the `const char*` sentinel) and the plain-call `Instr::Call` (set `rt_h` to the
  string sentinel so the result temp is typed `const char*`). Note `codegen.sv0` is
  stub-resolved/standalone (not the real generator), so no `-7` handling needed
  there; the SML golden + mega-TU emit already map the sentinel. PASS 80 → 81,
  CCFAIL 16 → 13. Refreshed stage0 + vm-parity goldens for lowering.sv0 (lowering.sv0
  is itself a corpus input the SML bootstrap compiles). Exposed 2 new RUNFAILs
  (checker.sv0/parser.sv0 now compile but a deeper newly-reached bug fails a unit
  test — not a regression, they never passed). NEXT string gap: `let x = y` where `y`
  is a string *local variable* (not a call) — needs a local-variable type environment
  during lowering (bigger; e.g. diagnostic.sv0 `let underline = gutter`).

- String-parameter aliasing (`include_expand.sv0`, was CCFAIL): `let mut out = dir`
  where `dir` is a `string` parameter declared `int out`, so every subsequent
  `out = string_concat(..)` / `return out` hit `-Wint-conversion`. New helper
  `lower_init_ident_is_string_param` (lowering.sv0): a 1-seg ExprPath init whose
  identifier (by TEXT) matches a `string`-typed parameter of the current function
  (`item_fn_row`), resolved with the same `lower_scan_item_fn_param_names_ty_toks`
  scan the match-scrutinee typing uses (`pc = item_d3[item_fn_row]`). Wired at both
  `let` handlers. Fully bounds-checked and returns false for `item_fn_row < 0` (the
  case in every lowering unit test), so the non-bounds-safe param scan is never hit
  on synthetic arenas. `include_expand.sv0` now compiles (CCFAIL 13 → 12) but
  RUNFAILs (exit 25 = `test_path_ok` code 5): `path_ok("../escape")` returns true —
  the composed compiler miscompiles `path_ok`'s `while i < len - 1` char loop
  (`string_char_at`/compound-condition), a deeper newly-reached bug unrelated to the
  string fix (path_ok has no `let x = param`). PASS stays 81. Refreshed stage0 +
  vm-parity goldens for lowering.sv0. NEXT string gap: `let x = y` where `y` is a
  string *local variable* (needs tracking declared-string locals within a block).
  NEXT deeper bug: the `path_ok` while-loop/char miscompile (include_expand RUNFAIL).

- Explicit unit return type `-> ()` (`driver.sv0`, contributes): a `fn f(..) -> ()`
  emitted as `int f(..)` while its body used bare `return;` → `-Wreturn-mismatch`
  hard error (e.g. driver.sv0 `drv_resolve_expr`). `megatu_cty_of_root` mapped only
  `root < 0` to void and TyName (ptt tag 0) to a name; the explicit unit type is
  `TyUnit` (ty_tag 6), which fell through to "int". Added a tag-6 → "void" branch
  (megaTU-main.sv0, emit-only — no lowering/golden change). Removes the whole
  return-mismatch class; driver.sv0 stays CCFAIL on its remaining string-local
  aliasing errors, so PASS holds at 81 (5 corpus modules use `-> ()`, now all
  correctly voided).

- String-local aliasing (`link.sv0` PASSES, was CCFAIL): `let y = x` where `x` is a
  `string` local declared earlier in the same block declared `int y`. Completes the
  string-propagation set (literal/builtin, user-call return, param-alias, now
  local-alias). Threads a per-block `str_locals: Vec<i32>` of string-local name
  tokens through `lower_block` → `lower_stmt` (new last param); each string-declaring
  `let` pushes its name token, and a new branch declares `let y = x` string when `x`
  (by TEXT) is in the set (`lower_ident_in_str_locals`, bounds-safe). Function bodies
  lower via `lower_body` → `lower_block` → `lower_stmt` (not `lower_tag_block`, which
  handles nested block-expressions and keeps its own scope — nested blocks don't yet
  see outer string locals, an accepted limitation). PASS 81 → 82 (`link.sv0`),
  CCFAIL 12 → 11; `diagnostic.sv0` 20 → 6 errors (closer). Refreshed stage0 +
  vm-parity goldens. GOTCHA: an over-broad `replace_all` on the shared
  `..., vec_new(), vec_new())` test-call tail added a third arg to every lowering
  test call (lower_expr_to_value/lower_body/lower_fn/…), not just the two `lower_stmt`
  ones — reverted and fixed the 2 sites surgically. NEXT: `diagnostic.sv0` remaining
  6 (another string position or struct); `driver.sv0` still 20.

- User-type (enum/struct) return values (`diagnostic.sv0` + `main.sv0` PASS, were
  CCFAIL): a `let e = make_error(..)` where `make_error -> Severity` (a nullary enum,
  emitted as a consistent `typedef struct { .tag } Severity`) declared `int e`, and
  the call temp was `int _t = make_error(..)`, so `is_error(e)` (param `Severity`) and
  the assignment mismatched. This is the string-return path generalized to *any* user
  struct/enum return. New helpers: `lower_callee_ret_ty_head` (shared front-end
  returning a 1-seg user call's return-type head token), `lower_name_is_user_type`
  (name matches a top-level ItemStruct tag 1 / ItemEnum tag 2), and
  `lower_call_ret_user_ty_tok` (the head token when the return type is a user type).
  Wired at both `let` handlers (DeclNamed with the type-name token) and the plain-call
  `Instr::Call` rt_h (string sentinel, else the user-type token). NOT the parked
  param-mismatch trap: here the enum/struct value representation is already consistent
  in signatures + bodies; only the call temp / let local were mistyped int. PASS 82 →
  84 (`diagnostic.sv0` enum, `main.sv0` struct `MainPhaseResult`), CCFAIL 11 → 9, no
  regressions. Refreshed stage0 + vm-parity goldens.

- Void-returning user calls in statement position (`driver.sv0`, contributes): a
  call to a user fn returning unit — `fn f(..) -> ()` or `fn f(..)` (no `-> T`) — in
  discarded/statement position lowered to a Call with a dst temp, emitting
  `int _t = f(..)` → C error `initializing 'int' with 'void'` (e.g. driver.sv0
  `drv_advance(..) -> ()`). New helper `lower_callee_is_void` (mirrors
  `lower_callee_ret_ty_head`'s bounds-safe item scan): a matched top-level ItemFn is
  void when `has_ret == 0`, or its `-> ()` return type head is TK_LPAREN(6) +
  TK_RPAREN(7). Wired at the top of `lower_tag_call`'s `bid == 0` branch: emit
  `Instr::Call(0, callee_tok, args, 0)` and return `VUnit` (like the builtin void
  path). Removes the void-init error class; driver.sv0 20 → 18 errors, stays CCFAIL
  on unrelated issues, PASS holds at 84. (Implemented by a delegated Sonnet subagent
  to a fixed spec; verified here: compile-run OK, no parity regression, goldens
  refreshed, full suite green.) Refreshed stage0 + vm-parity goldens.

- Type-token / tuple-field name collision (`types.sv0`, contributes): a DeclNamed
  type token that lands in `name_of`'s 500..599 range (tuple-field names `f<n>`, used
  by `megatu_field_name`) rendered the type as `f9` instead of its real name (e.g.
  `f9 us;` for a `Ty` enum local in the large types.sv0). `megatu_ty_name` consulted
  `name_of` unconditionally — the same collision fixed earlier for `megatu_val_name`.
  Fix: only consult `name_of` for `h < 500` (keeps the int(9)/void(10) sentinels);
  resolve `h >= 500` via `handle_to_str`. Emit-only. `us` now declares `Ty`; f9 class
  gone. This also hardens the user-type-return typing (its type tokens can land in
  500..599 in big files). `types.sv0` stays CCFAIL — its remaining errors are the
  parked struct-by-value cluster below (`passing int to parameter of type 'Ty'`).

**Parked (large): struct-by-value representation.** After the box fix, `ir.sv0` and
`unify.sv0` reduce to one error class — `passing 'int' to parameter of incompatible
type 'Value'/'Ty'` — and `span.sv0` to `assigning to 'int' from incompatible type
'Pos'/'Span'`. Root cause: the emit is internally inconsistent about how a user
struct/enum is represented. Function params and struct fields are declared with real
struct C types (`Value x`, `Ty t`, typedef'd structs), but every *value* (temp,
local, argument) is an `int` handle. Passing a handle to a struct-typed param, or
assigning a struct-returning call's result to an `int` local, therefore type-errors.
Fixing this is a single coherent but large decision — pick one representation
end-to-end: either make all values flat structs (like the golden, big emit rework) or
make all struct/enum params/fields/returns `int` handles (and route field access
through the handle). This is the dominant remaining CCFAIL cause and should be its own
planned multi-step task, not squeezed into an incremental fix.

**Stop rule.** A module that needs a genuinely big feature (e.g. full generics or
trait resolution) should be parked with a note here rather than forced — keep tasks
bite-sized.
