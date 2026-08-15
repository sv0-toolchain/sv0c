# sv0c bug-hunt findings (2026-08-11)

A targeted audit of the sv0c toolchain (`lib/*.sv0` native + `sml-legacy/`
reference + runtime), hunting for correctness defects. **Not exhaustive** — a
45k-line self-hosting compiler cannot be fully verified by inspection in one
pass. Findings are prioritized by a mix of severity and reachability. Each entry
notes whether it is **live** (on a pipeline path exercised by the corpus),
**latent** (real defect on a code path not currently exercised), or **dead**
(defect in code with no live caller).

Severity: **P1** = wrong output / crash on a plausible input the pipeline
accepts; **P2** = wrong on an input outside the current corpus but within the
language; **P3** = dead/defensive/cosmetic, or a documented design limitation
worth revisiting.

## Executive summary

13 findings across 6 probing passes. Each finding below has a bite-sized task
breakdown in [Remediation tasks](#remediation-tasks-bite-sized) at the end.

| # | Finding | Sev | Backend(s) | Status |
|---|---------|-----|-----------|--------|
| 1 | Method calls miscompile compound arguments (`p.m(a+b)` → `m(p,a)`) | P1 | native | ✅ **FIXED** (sv0c) |
| 2 | VM integer arithmetic ≠ i32: overflow no-wrap + floor div/mod | P1 | VM | ✅ **FIXED** (sv0vm `a0876d9`) |
| 3 | Stale pty type-tag scheme in checker type-resolvers (2/3/7) | P2/P3 | native (dead) | ✅ **FIXED** (sv0c) |
| 4 | Void fn + contract, no `-> T`: native fails silently vs SML E0409 | P3 | native |
| 5 | Cross-module method calls fail SML `--project` (E0401) | P2 | SML |
| 6 | Checker doesn't scope block-local bindings | P3 | native |
| 7 | Literal `2147483648` (2³¹) handled 3 different ways | P2/P3 | all | ✅ **FIXED** (wrap to INT_MIN, all 3) |
| 8 | Native checker accepts type errors + emits no diagnostics | P1 | native |
| 9 | `include` unwired on native (works on SML) | P2 | native | ✅ **FIXED** (native build + fixture) |
| 10 | All runtime contracts dropped on native; VM aborts ungracefully | P1 / P3 | native / VM |
| 11 | `?` / enum-return / tuple / nested-struct silently mis-emit | P1/P2 | native | ⚠️ enum-return **FIXED** (BH-11b); `?`/tuple/nested-struct open |
| 12 | Variable shadowing → invalid C (redeclare) + VM scope leak | P2 | SML + native + VM |
| 13 | Match guards → invalid C (guard evaluated before binding) | P2 | SML + native | ✅ **FIXED** (sv0c) — also fixed native catch-all bind arms |

**Two systemic stories:**

1. **Native feature incompleteness (the dominant risk).** The native mega-TU
   compiler was built to self-host its own `lib/*.sv0` and pass a fixed fixture
   set — both of which avoid many constructs. For anything outside that diet it
   **silently emits wrong, stub, garbage, or empty C at exit 0** instead of a
   diagnostic (#1, #8, #10, #11). This matters directly because native is being
   promoted to the default front end.
2. **The reference (SML/C) has real bugs too.** Shadowing (#12) and match guards
   (#13) emit invalid C from the *SML* pipeline, and the VM's integer model (#2)
   and scope handling (#12) diverge from C. So "matches SML" is not a sufficient
   correctness bar.

**One cross-cutting cause:** the gates verify **bytecode-byte parity** and
**self-host emit**, never **runtime behavior on arbitrary programs**, a
**diagnostic corpus** beyond 7 contract cases, or a **contract violation**. A
behavioral 3-way differential harness (C vs VM vs native) plus an expanded
diagnostic corpus would have caught #1, #2, #8, #10, #12, and #13 — see task
**BH-X1**.

---

## 1. Native method calls silently miscompile any compound argument — **P1 (live, native-only)** — ✅ FIXED (sv0c)

**Files:** `lib/parser.sv0` ~L1670 (method-call parse), `lib/lowering.sv0` ~L1263-1290 (tag-5 lowering).

> **Fixed 2026-08-11:** tag-5 (ExprMethodCall) now carries the same `pp` arg-root
> sidecar as tag-4 ExprCall — the parser stores `ed3 = argc`,
> `ed4 = block_stmt_sidecar_push(pp, arg_roots)`, and all three consumers
> (`resolver.sv0`, `checker.sv0`, `lowering.sv0`) read arg *k* via
> `block_stmt_index(pp, _, ed4, k)` instead of `args_first + k`. Fixture
> `test/integration/mcall_compound_arg/` (compound + nested-method args → exit 42)
> gated in the native `--project` acceptance and the SML harness; parser/resolver/
> checker/lowering stage0 + vm-parity goldens refreshed; full gate green
> (behavior-preserving — self-host-loop 98/98, 26/26 integration). **BH-1d
> deferred:** the link.sv0 arena-reloc/merge path does not yet treat the tag-5 pp
> sidecar (add tag 5 to the tags-4/9/11 handling) — latent only, since the CLI
> uses source-concat, not arena-merge.

A method call `recv.m(arg)` whose argument is anything other than a single atomic
node (a binop, a nested call, another method call, etc.) is **silently
miscompiled**: the argument is replaced by its *first sub-expression node*.

**Reproduction** (native `build/sv0-megatu-native`):

```sv0
struct Point { x: i32, y: i32 }
impl Point { fn add(self: Point, k: i32) -> i32 { return self.x + self.y + k; } }
fn main() -> i32 {
  let p: Point = Point { x: 8, y: 12 };
  let n: i32 = 5;
  return p.add(n + 17);   // expected 8+12+22 = 42
}
```

Native emits `int _sv0t0 = add(p, n);` — the argument `n + 17` became just `n`.
The program **compiles and runs, returning 25** (8+12+5), no diagnostic. Other
shapes: `p.add(p.sum())` → `add(p, p)`; `p.m(f(x))` → drops to `f` or `x`.

**SML reference is correct** — it emits `int _sv0t0 = (n + 17); add(p, _sv0t0);`
→ 42. So this is **native-only**, in the pipeline being promoted to default.

**Root cause:** the tag-5 (ExprMethodCall) node stores `ed3 = args_before`
(`vec_len(et)` captured *before* `parse_arg_list`) and `ed4 = argc`. The parser
*does* compute the real argument root indices into a local `arg_roots_m`, **but
discards them** (comment: "Method calls (tag 5) have no free field for an arg
sidecar; store the real arg count ... nested method args are not yet
supported"). Lowering then reconstructs each argument as
`ax_mc = args_first_mc + aj_mc` (`lowering.sv0:1277`) — a contiguous
single-node-per-arg assumption. For a compound arg the sub-expression operands
are appended *before* the arg's root node, so `args_first + 0` lands on the
first operand (`n`), not the binop root (`n + 17`). Atomic args (literals,
bare paths — e.g. the `impl_methods` fixture's `p.plus(4)`) happen to *be* their
own first node, which is why every existing test passes and the corpus/self-host
loop never diverged.

**Why the self-host loop misses it:** the loop compares native vs SML emit
behaviorally over the compiler's own sources; evidently no `.sv0` lib module
calls a method with a compound argument, so the divergence never triggers.

**Fix:** give tag-5 method calls a real arg-root sidecar like tag-4 (ExprCall)
has — store `arg_roots_m` into the shared `pp` pool (as `block_stmt_sidecar_push`
does for Call(4)/Block(9)/Match(11)) and have lowering read the actual root
indices instead of `args_first + j`. Then add a regression fixture with a
compound method-call argument (native + SML → exit 42). Note the reloc/merge
primitives already treat Call/Match pp sidecars (PC-3b §5g); tag-5 would join them.

---

## 2. VM integer arithmetic does not match i32 (C) semantics — **P1 (live, backend divergence)** — ✅ FIXED (sv0vm `a0876d9`)

**File:** `sv0vm/src/interpreter/interpreter.sml` (arithmetic opcodes, ~L217-227).

> **Fixed 2026-08-11:** `ADD/SUB/MUL/NEG_I32` now wrap through `Word32`
> (`Word32.toIntX (Word32.+ (asWord32 a, asWord32 b))`), and `DIV/MOD_I32` use
> `Int.quot`/`Int.rem` (truncate toward zero); `MOD` gained a by-zero guard.
> Verified: overflow add/mul, negative div/mod, and `INT_MIN / -1` now match the
> C backend. Regression test added to `sv0vm/test/bytecode_test.sml`; full
> `./scripts/sv0 test` green (VM integration, bootstrap-on-VM, vm-parity 98,
> self-host-loop all pass).

The VM executes the i32 arithmetic opcodes with SML's native `int` operations
(63-bit on SML/NJ, floor-based `div`/`mod`) **without truncating to 32-bit
two's-complement**, so the VM and the C backend produce different results for two
whole classes of input. This is broader than a single opcode — it is the VM's
integer model diverging from `i32`.

### 2a. Overflow does not wrap on the VM

`ADD_I32`/`SUB_I32`/`MUL_I32` are `fn (a, b) => a + b` etc. with no mask to i32.
(By contrast the *bitwise* ops at ~L264-268 correctly go through
`Word32.andb (asWord32 a, asWord32 b)` — only the arithmetic ops skip it.)

```sv0
fn main() -> i32 { let a: i32 = 2000000000; let b: i32 = a + a; if b < 0 { return 42; } return 7; }
// C (32-bit wrap → INT_MIN, negative) → 42 ;  VM (stays 4e9, positive) → 7
```

`46341 * 46341` (= 2_147_488_281, overflows i32): C → negative (wraps), VM →
positive. **Any** overflowing i32 computation diverges.

### 2b. Division/modulo floor instead of truncate-toward-zero

`DIV_I32 => a div b`, `MOD_I32 => a mod b` — SML `div`/`mod` floor toward negative
infinity; C's `/`/`%` (and Rust's) truncate toward zero:

| expression | C backend (& Rust) | VM |
|---|---|---|
| `(-100) / 3` | -33 | **-34** |
| `(-7) % 3` | -1 | **+2** |
| `(-1) % 100` | -1 | **+99** |

```sv0
fn main() -> i32 { let a: i32 = 0 - 100; return a / 3 + 100; }   // C → 67, VM → 66
```

**Which is right:** the C backend matches Rust/`i32` semantics (wrap on overflow,
truncate toward zero). The **VM is wrong** on both counts.

**Fix (both):** make the VM arithmetic ops operate mod 2³² with sign-extension —
e.g. `ADD_I32 => arithII (fn (a,b) => Word32.toIntX (asWord32 a + asWord32 b))`
(and SUB/MUL likewise), and `DIV_I32 => Word32.toIntX (asWord32 (Int.quot (a,b)))`,
`MOD_I32 => … Int.rem …`. That gives 32-bit wraparound + truncate-toward-zero in
one stroke. Decide div-by-zero (C = UB/garbage; VM currently emits no `vm_exit`)
— a deterministic panic on `b = 0` is preferable.

**Why the gates miss it:** vm-parity compares *bytecode bytes* (SML-emit vs
native-emit), not runtime results; the self-host corpus is the compiler's own
sources, which evidently never overflow i32 or do negative `/`/`%`. A
**behavioral C-vs-VM differential test** over an arithmetic battery (overflow,
negative div/mod, boundaries) would catch both and belongs in the gate.

Everything else agrees between backends: shifts (incl. arithmetic right-shift of
negatives; both mask the shift count to 5 bits), comparisons, non-overflowing
multiply, enum match, struct mutation, boolean logic, `Box`, strings, `Vec`.

---

## 3. Stale pty type-tag scheme in a cluster of checker type-resolution fns — P2/P3 (latent + dead)

**Files:** `lib/checker.sv0` — `resolve_field_ty_tag` (~L1075), `ast_ty_to_ty_payload` (~L1130), `item_fn_ret_ty_tag` (~L1207).

The parser pty arena and the AST `Ty` enum use the SAME type-tag scheme
(verified: `parse_type` in `lib/parser.sv0` ~L678 and `ty_tag` in `lib/ast.sv0`
L197-204):

```
0 TyName   1 TyRef   2 TyRefMut   3 TyArray   4 TySlice   5 TyTuple   6 TyUnit   7 TyInfer
```

`link.sv0:map_ty_kind` and the new `checker.sv0:pty_root_to_type_tag` (PC-7a)
both use this scheme correctly. But `resolve_field_ty_tag` and
`ast_ty_to_ty_payload` dispatch on a **different, wrong** scheme:

```sv0
if tag == 6 { return TY_UNIT(); }     // ok
if tag == 2 { return TY_REF(); }      // WRONG: tag 2 is TyRefMut  → should be TY_REFMUT
if tag == 3 { return TY_REFMUT(); }   // WRONG: tag 3 is TyArray   → should be -1 (arrays unsupported)
if tag == 7 { return TY_TUPLE(); }    // WRONG: tag 7 is TyInfer   → tuples are tag 5
// tag 1 (TyRef) and tag 5 (TyTuple) are NOT handled → fall through to -1
```

**Consequences if reached:** a `&mut T` type resolves to `TY_REF` (drops the
mut), a `TyArray` resolves to `TY_REFMUT`, a `TyInfer` (`_`) resolves to
`TY_TUPLE`, and a real tuple `(A, B)` resolves to `TY_UNKNOWN`.

**Reachability:** currently **dead/latent**. `resolve_field_ty_tag`'s only
non-test caller is `item_fn_ret_ty_tag`, which has **zero callers** anywhere.
`ast_ty_to_ty_payload`'s only callers are its own unit tests. The comment on
`resolve_field_ty_tag` claims it is "used by initTypes for struct/enum field
definitions", but that wiring is not present. So no live program is
mis-typed today — but if any of these are wired up (e.g. to type struct fields
of ref/tuple type, or fn signatures with such types) they will silently
misclassify.

**Unit-test gap:** `test_resolve_field_ty_tag` only builds `TyName` (0) and
`TyUnit` (6) nodes, so it never exercises the wrong 1/2/3/5/7 branches — the bug
is neither caught nor locked in.

**Fix:** ✅ **DONE (sv0c).** Both `resolve_field_ty_tag` and `ast_ty_to_ty_payload`
now use the canonical scheme (tag 1→TY_REF, 2→TY_REFMUT, 5→TY_TUPLE; 3 TyArray /
4 TySlice / 7 TyInfer fall through to -1), matching `pty_root_to_type_tag`.
`test_resolve_field_ty_tag` gained ref/refmut/tuple/array cases (the old test
only built TyName/TyUnit, so the wrong branches were unexercised). checker
stage0 + vm-parity goldens refreshed; full gate green. (Kept both functions
rather than deleting the dead chain — the fix is minimal and now correct if ever
wired.)

---

## 4. Void fn with a contract but no explicit return type: native fails silently — P3 (diagnostic divergence) — ✅ FIXED (BH-4a / Slice 5, emits E0409)

**Files:** `lib/checker.sv0` (ret-type resolution → check failure path).

`fn f(x: i32) requires(x > 0) { }` (a fn with a contract and NO `-> T`):

- **SML** rejects it cleanly: `E0409: function needs an explicit return type in
  this slice`.
- **Native** rejects it too, but **silently** — `check_program` returns nonzero,
  the compiler exits 4 with **no diagnostic message**.

Adding `-> ()` makes both accept it. So this is not a miscompile — it is a
**diagnostic-quality gap**: native produces the right verdict (reject) with the
wrong UX (no error text). Mechanically it stems from the return-type scan
treating a packed `id2` (has_ret bit 0 + contract_count*2 ≠ 0) as "has a return
type" and then finding none → `TY_UNKNOWN` → body-check failure, rather than
emitting E0409. Low priority, but native should surface E0409 to match the
reference. (`test/…/contracts` only exercises contracts on `-> i32` fns, so this
path is untested.)

---

## 5. Cross-module method calls fail on SML `--project` — P2 (known divergence, documented)

**Files:** `sml-legacy/link/link.sml` (`mapExpr` ExprMethodCall).

Already documented in `test/integration/impl_methods/README.md`. A method defined
in another module and called across the module boundary
(`use geom::Point; … p.scaled(2)` where `scaled` lives in `geom`) fails SML
`--project` with `E0401: unbound value main__scaled` — `mapExpr` mangles the
method-call name with the *calling* module's id, not the defining module's. The
**native** pipeline handles it correctly (source-concat, no mangling). Intra-module
method calls work on both. Fixing SML needs type-directed method resolution
(resolve the method via the receiver's type module) — intricate, and native (the
future default) already works, so this was deliberately deferred.

---

## 6. Block-local bindings leak out of their block in the checker — P3 (documented design limitation, self-host risk)

**Files:** `lib/checker.sv0` `check_stmt_in_block` (~L3018).

The checker's env extensions are **not scoped** — a `let` inside a block stays
visible after the block (comment: "bindings leak out of the block. This is
acceptable for well-formed programs that have already been validated by the SML
compiler"). This is a deliberate simplification, safe for the current use where
the SML reference has already validated inputs. But as the native compiler is
promoted to the default (and eventually the only) front end, it will accept
programs that *should* be rejected (a variable used after its block scope ends),
i.e. it under-diagnoses. Not a miscompile, but a correctness gap in the
type-checker's scope enforcement that the promotion plan should close.

---

## 7. The literal `2147483648` (2³¹) is handled inconsistently across the three backends — P2/P3 (edge) — ✅ FIXED (wrap to INT_MIN on all three)

**Files:** literal lexing/parsing (`lib/lexer.sv0` / `lib/parser.sv0`), native emit.

`2147483648` is one past `i32::MAX`. The three backends disagree on
`let a: i32 = 0 - 2147483648;` (i.e. `INT_MIN`):

- **SML→C:** accepts, `a < 0` is true → 42 (treats it as INT_MIN).
- **native→C:** compiles but `a < 0` is **false** → returns the wrong branch;
  the large literal is mis-parsed (overflowed to a wrong/positive value).
- **SML→VM:** the VM run **fails** (no `vm_exit` — likely an `Overflow` during
  bytecode int encoding or execution).

This is an edge (a decimal literal exactly at `2³¹`, normally written via `i32`
min helpers), but the three-way inconsistency shows the literal path has no
agreed overflow behavior. Relatedly, `INT_MIN / -1` and shift-by-≥32 are UB in
C and happen to coincide across backends here only by luck (low-byte masking /
5-bit shift masking); a defined semantics (Rust panics/wraps) would be safer.

---

## 8. Native checker under-diagnoses: silently accepts several type errors + emits no diagnostics — **P1 for native-default promotion**

**Files:** `lib/checker.sv0` (the native type-checker).

The native checker was built to run *after* the SML reference has already
validated the program (cf. finding #6: "acceptable for well-formed programs that
have already been validated by the SML compiler"). As a result it is much weaker
than SML, and this matters directly for promoting native to the default (and
eventually only) front end. Comparing native vs SML on common mistakes:

| program | SML | native |
|---|---|---|
| `let x: i32 = true;` | rejects **E0400** | **accepts** → emits `x = 1;`, runs |
| `let p: Widget = 0;` (unknown type) | rejects **E0301** | **accepts** (exit 0) |
| `fn main() -> i32 { return true; }` | rejects **E0400** | **accepts** (exit 0) |
| `return p.z;` (no field `z`) | rejects **E0429** | **accepts**, defers to `cc` ("no member named 'z'") |
| `return y;` (unbound) | **E0300** | rejects but **no message** (bare exit 3) |
| `f(1)` for `f(a,b)` (arity) | **E0307** | rejects but **no message** (bare exit 3) |

Two distinct problems:

1. **Missing checks (soundness).** Native silently accepts type mismatches,
   unknown types, wrong return types, and nonexistent field accesses. The
   type-mismatch case produces a *running program with a wrong value*; the
   field case leaks a confusing C-compiler error to the user. Under SML
   pre-validation these never reached native; as the default, native will accept
   ill-typed programs.
2. **No diagnostics (UX).** Even where native *does* reject (unbound, arity), it
   prints no `Exxxx` code or message — just a nonzero exit. Every SML rejection
   carries a precise code. (Same class as finding #4's void+contract silent
   exit.)

**Fix scope:** this is the big-ticket item behind the native-default promotion —
the native checker needs real type-equality/return/field/arity checks and a
diagnostic layer emitting the `Exxxx` codes the SML reference and the golden
corpus already define. Until then, native should not be the sole front end for
untrusted/unvalidated input.

**Coverage note:** the golden **fail** corpus (`test/data/golden/fail/`) has only
7 cases, all contract-related (E0444/E0521-E0526). None of E0300/E0301/E0307/
E0400/E0429 above are gated, which is why native's missing checks went unnoticed.

---

## 9. Native does not expand `include` (SML does) — P2 (native feature gap)

**Files:** `lib/include_expand.sv0` (native pass, **unwired**); `lib/megaTU-main.sv0`.

`include "relpath";` works on SML — `sml-legacy/main.sml:54,90` and
`link.sml:308` call `IncludeExpand.expandFile` before lexing. The native
`include_expand.sv0` module exists and is unit-tested, but the native compose
main (`megaTU-main.sv0`) **never calls it**, so a source beginning with
`include "helper.sv0";` compiles on SML and **fails to parse on native**
(exit 2 — the parser sees the bare `include` token). Wiring
`link_project_concat`-style expansion (or `expand_from_file`) into the native
`source` setup would close the gap.

---

## 10. Native drops ALL runtime contract checks; VM aborts ungracefully — **P1 (native) / P3 (VM)**

**Files:** `lib/lowering.sv0` / native contract emit (missing); `lib/contract_analyzer.sv0` (a no-op placeholder, per its own comment); `sv0vm/src/interpreter/interpreter.sml:484`.

`requires`/`ensures` are core sv0 features with a whole `contract_analyzer` +
`sv0_requires`/`sv0_ensures` runtime. Enforcement diverges sharply across
backends. For `fn f(x: i32) -> i32 requires(x > 0) { return x + 100; }` called
as `f(-5)`:

- **SML→C:** emits `sv0_requires((x > 0), "f");` → at runtime prints
  `sv0 contract violation: requires ...` and exits 1. **Correct.**
- **native→C:** emits **just** `int _sv0t0 = (x + 100); return _sv0t0;` — **no
  contract code at all.** `f(-5)` returns 95, no check, no abort. Same for
  `ensures`: `g(x) ensures(result == x + 1) { return x; }` returns `x`
  unchecked. **Native silently ignores every runtime contract.**
- **SML→VM:** *does* enforce, but as an **uncaught SML exception**
  (`uncaught exception Fail [Fail: contract: requires failed: f]` at
  `interpreter.sml:484`) — no clean program abort / exit code, unlike C's
  `contract violation` message + exit 1.

**Two issues:**

1. **Native (P1):** the native **mega-TU** compiler emits **no**
   `sv0_requires`/`sv0_ensures` for any function — verified on both a non-main
   fn and `main` itself (`fn main() -> i32 requires(false) … { return 42; }`
   still returns 42 instead of aborting). Note this is *not* missing codegen —
   `lib/codegen.sv0:368` and `lib/megaTU-main.sv0:800` both know how to emit
   `sv0_requires`, and `lower_fn` (`lib/lowering.sv0:3558`) builds
   `Instr::Requires`/`Ensures` — but the native compose-main pipeline never
   routes contracts through to the output, so the whole path is dead in the
   shipping native compiler. Contracts — a headline language feature — are a
   complete no-op at native runtime; a `requires` precondition guarding against
   bad input gives no protection. Another native-default-promotion blocker
   alongside #8. (`lib/contract_analyzer.sv0` is separately a documented "no-op
   placeholder".)
2. **VM (P3):** enforcement works but the failure path is an uncaught exception
   rather than a defined "contract violation → exit 1" like C. Should abort
   cleanly and consistently with the C runtime.

**Coverage note:** the golden **pass** corpus exercises `requires(true)`/
`ensures(true)` (always satisfied), and the golden **fail** corpus checks
*compile-time* contract-placement errors (E0521-E0526) — but nothing exercises a
**runtime** contract *violation* on the native backend, so the silent drop was
invisible.

---

## 11. Native silently mis-emits for `?`, enum-returning lets, and tuples — **P1/P2 (native)**

**Files:** `lib/lowering.sv0` / `lib/megaTU-main.sv0` (native compose-main lowering).

Several language constructs that SML compiles correctly cause the **native**
compiler to emit **wrong or degenerate C with exit 0** (silent) — again invisible
because the affected fixtures (`option_result`, `box_expr`, …) are gated on SML,
not native.

- **`?` (try) operator → stub `main`.** Any program using `?` compiles to exactly
  `#include "sv0_runtime.h"` + `int main(void) { return 0; }` — the whole program
  (enums, functions, real `main`) is dropped, exit 0. Minimal repro:
  ```sv0
  enum Opt { Some(i32), None }
  fn f(o: Opt) -> Opt { let v: i32 = o?; return Opt::Some(v); }
  fn main() -> i32 { let s: Opt = Opt::Some(42); let r: Opt = f(s);
                     return match r { Opt::Some(x) => x, Opt::None => 0 }; }
  ```
  SML → 42; native → the stub (returns 0). **Root cause (diagnosed 2026-08-12):**
  *two* gaps. (1) The **parser** never produces an `ExprTry` node — `parse_postfix_expr`
  has no `?` (token 34) case, so `let v = o?;` stops parsing at `o`, truncating
  the item arena to whatever preceded the `?`-bearing fn (`num_blocks == 0` →
  `megatu_emit_program` emits the `int main(){return 0;}` stub). A one-line parser
  addition (push tag-22 `ExprTry(inner)`) fixes the parse. (2) But then
  `lower_tag_try` — which *is* implemented — can only resolve the operand's enum
  type when the operand is a **call** (`f()?`, via `fn_ctx`) or a 2-segment path
  (`Enum::V(..)?`); for the common `o?` where `o` is a **bound variable**, it can't
  find the enum type (the native lowering doesn't track variable types) → bails to
  `VUnit` → wrong `v = 0`. So the parser fix alone makes `?` *parse* but produce
  silently-wrong results (worse than the loud stub) — it was reverted.
  **Remaining work (BH-11a):** give `lower_tag_try` operand-type resolution for
  bound variables (look up a param's type via the fn's `id5`/`fpn`/`fpt` arenas;
  locals need a lowering-side var→type map). Deferred — a real native-completeness
  addition, not a quick fix.

- **Enum-returning function into a typed `let` → invalid C type `i`.** ✅ **FIXED (BH-11b, sv0c).**
  ```sv0
  enum Opt { Some(i32), None }
  fn mk() -> Opt { return Opt::None; }
  fn main() -> i32 { let r: Opt = mk(); return match r { Opt::Some(x)=>x, Opt::None=>42 }; }
  ```
  Native emitted `i _sv0t0 = mk(); i r;` — the enum type `Opt` became the bogus
  type name **`i`** (cc: "use of undeclared identifier 'i'"). **Root cause:**
  `megatu_ty_name` (`lib/megaTU-main.sv0`) resolved a real *type* token by first
  consulting `name_of` — a codegen *value-name* **test stub** that maps handles
  1..15 (and 500..599) to fabricated identifiers, with `15 → "i"`. An enum
  return-type token landing at index 15 was corrupted to "i". (The comment had
  already patched the 500..599 collision but not 1..15.) **Fix:** resolve type
  ctypes directly via `handle_to_str` — a type token is always a real source
  token; only the string sentinel (`lower_string_cty` = -7) needs special-casing,
  and lowering never passes an int(9)/void(10) sentinel here. Fixture
  `test/integration/enum_return_let/` (exit 42, native `--project` + SML + native
  single-file); `megaTU-main.sv0` is not in the golden sets so no golden churn.
  (`?`/tuple/nested-struct below remain open.)

- **Tuple type annotation → empty emit.** `let t: (i32, i32) = (40, 2);` makes the
  native compiler emit **nothing at all** (0 lines / empty output), exit non-clean.
  (Tuples are a documented limitation, but the failure mode — empty output — is
  worse than a diagnostic.)

- **Nested struct (struct-typed field) → empty emit on native.**
  ```sv0
  struct Inner { v: i32 }
  struct Outer { inner: Inner, k: i32 }
  fn main() -> i32 { let o: Outer = Outer { inner: Inner { v: 40 }, k: 2 }; return o.inner.v + o.k; }
  ```
  **C backend → 42** (handles nested structs); **native → empty emit (0 lines)**;
  **VM → fails** (consistent with `lib/span.sv0`'s note that "codegen does not yet
  support multi-slot sub-struct fields"). So a struct containing another struct is
  a C-only capability — native and VM both drop it, native silently.

Common thread with #1/#8/#10: the native compiler produces **silently wrong or
empty output** for constructs outside its self-host/fixture diet, rather than a
diagnostic. For native-default promotion each of these needs either real support
or an explicit "unsupported" error — never a stub/garbage emit at exit 0.

---

## 12. Variable shadowing emits invalid C (redeclaration) on BOTH C backends — **P2 (SML + native; VM correct)** — ✅ FIXED (all 3 backends)

**Files:** `sml-legacy/ir/lowering.sml` and `lib/lowering.sv0` (C emit), + the C
backend variable-naming.

```sv0
fn main() -> i32 { let x: i32 = 10; let x: i32 = x + 32; return x; }   // valid; = 42
```

Both C backends emit:

```c
int x = 10;
int x = (x + 32);   // C: redefinition of 'x'; and the RHS 'x' now refers to the
return x;           // just-declared (uninitialized) x → UB
```

This is invalid C twice over (redeclaration in one block + self-reference in the
initializer). Here the **VM** happens to return 42, but shadowing is broken on
every backend once a **block** is involved:

```sv0
fn main() -> i32 { let a: i32 = 1; { let a: i32 = 40; } let b: i32 = 2; return a + b + 39; }
// correct = 1 + 2 + 39 = 42
```

- **C backends (SML + native):** `ccfail` — the inner `int a = 40;` redeclares
  `a` in the same emitted function scope.
- **VM:** returns **81**, not 42 — the inner block's `a = 40` **leaks out** and
  overwrites the outer `a` (so `40 + 2 + 39`). This is the *runtime* face of
  finding #6 (the checker does not scope block bindings): the VM lowering reuses
  one local slot per name, so an inner shadow clobbers the outer binding.

So shadowing/block-scoping is unsound across the board: the C backend can't
compile it (no renaming to fresh identifiers), and the VM mis-scopes it (leaks).
By contrast **match-arm bindings and for-loop index variables *are* correctly
scoped** (two matches each binding `x`, or two `for i` loops, both work on all
three backends) — the gap is specifically sequential/nested `let` shadowing.
Notable because the redeclaration defect is in the **SML reference**, not only
native. Fix: rename shadowed `let` bindings to fresh C identifiers / VM slots (or
emit each shadowing `let` in its own nested block), and make the checker scope
block bindings (#6).

---

## 13. Match guards (`pat if cond => …`) emit invalid C on both C backends — **P2 (SML + native)** — ✅ FIXED (sv0c)

**Files:** `sml-legacy/ir/lowering.sml` / `lib/lowering.sv0` match lowering; `lib/checker.sv0` (native).

> **Fixed 2026-08-12.** Two defects, both closed:
> 1. **Lowering order (both C backends):** `lowerMatchArms` / `lower_match_arms`
>    put the pattern bind *inside* the guard's then-branch (`th = guard_pre @
>    [if guard then (bind; body)]`), so the guard referenced the bind before it
>    was declared. Now the bind comes first: `th = bind @ guard_pre @ [if guard
>    then body else next]`. Byte-identical for non-guard arms (so the corpus /
>    self-host loop are unaffected).
> 2. **Native checker gap (broader):** the native checker never bound a
>    *top-level* PatBind (`arm_pt == 1`), so not only guards but **every**
>    catch-all `x => …` arm failed to type-check (silent exit 4). Added the
>    PatBind case (bind the arm's `ed2` token with the scrutinee's type).
>
> Fixture `test/integration/match_guard/` (guards + catch-all bind → exit 42) on
> native `--project`, native single-file, and SML. checker + lowering stage0 +
> vm-parity goldens refreshed; full gate green. *(NB: the self-host-loop runs
> `driver.sv0`'s binary against the ambient `/tmp/.sv0_drv_path`; a stale guard
> program there fails it spuriously — a pre-existing gate fragility, not this fix.)*

```sv0
fn classify(n: i32) -> i32 { return match n { x if x > 10 => 42, _ => 0 }; }
```

SML-C emits the guard test **before** binding the pattern variable:

```c
if ((x > 10)) {        // 'x' is undeclared here — it's bound below
  int x = n;           // binding happens inside the arm body, after the guard
  _sv0t0 = 42;
} else { ... }
```

`x` in the guard is undeclared → the C won't compile. **native** bails entirely
(empty emit / exit non-clean); the **VM** also fails to produce a result. So
match guards are effectively broken on every backend — the guard condition must
be evaluated *after* the pattern binding is in scope. (Parser accepts the
syntax, so this is a lowering bug, not "unsupported".) The pass corpus uses no
guarded arms, so it was never exercised.

---

## Documented limitations confirmed (effectively "won't-compile" bugs, by design)

These are catalogued in code/READMEs as intentional gaps; listing for
completeness. Each rejects or under-supports a valid-language construct:

- **Multi-element tuples** — `lib/checker.sv0:3117` ("unwrap (1-elem) or
  unsupported (multi)"); `lib/types.sv0` notes tuples/const-char* limits.
- **`PatOr` (`a | b` patterns)** — `lib/resolver.sv0` returns `E0306`
  unsupported (SML raises Fail).
- **`&` / `&mut` outside contracts** — `lib/lowering.sml`/parser reject borrow
  operators except in contract expressions (`E0540`/`E0541`).
- **Struct/enum-valued `if`-expression on the VM backend** —
  `lib/lowering.sv0:1722`: the VM temp path assumes int-typed temps, so a
  struct/enum-valued `if` would emit `int t;` for a struct. Guarded to the VM
  target; the C backend is correct. Latent (no corpus program hits the VM path
  with a struct-valued if).
- **Generic functions (`fn f<T>(x: T)`)** — not a real language feature (the
  `generics` fixture is a monomorphic placeholder). Divergence on the
  unsupported syntax: **SML accepts** `fn id<T>(x: T) -> T` and both SML→C and
  SML→VM return the right value, while the **native** compiler rejects it at
  resolve time with **exit 3 and no diagnostic message**. If generics stay
  unsupported, native should at least emit a proper "generics unsupported"
  diagnostic instead of a silent nonzero exit; if they are meant to work,
  native needs the resolver/monomorphization SML already tolerates.

## Test-coverage observations (why the bugs above hid)

- The `impl_methods` fixture and every method-call test use **atomic**
  arguments (`p.plus(4)`, `p.scaled(2)`) — literals/paths that are their own
  first node — so finding #1 (compound method-call args) never fired.
- `test_resolve_field_ty_tag` builds only `TyName`/`TyUnit` pty nodes, so
  finding #3's wrong ref/array/tuple branches are unexercised.
- The self-host loop compares native-vs-SML over the compiler's own sources;
  since no `.sv0` lib module uses a method call with a compound argument, #1 is
  invisible to it. **Adding a compound-method-arg fixture would both fix the
  coverage gap and gate the #1 fix.**

## Coverage note (what this audit did and did not cover)

**Pass 1 (native C backend + SML C):** method calls
(atomic/compound/chained/nested), struct literals with compound fields, enum
ctors with compound args, free-fn calls (multi/nested/compound args), recursion,
`while` loops, arrays (literal + index), field access, contracts. Passed every
case except the method-call compound argument (#1).

**Pass 2 (three-way differential: SML→C vs SML→VM vs native→C):** negative
division/modulo (**found #2**), shifts (incl. arithmetic right-shift of
negatives), comparisons, negative multiply, modulo, overflow/large multiply,
`Box`/`box_new`/`box_deref`, `string_concat`/`string_substr`/`string_len`/
`string_eq`, `Vec` push/get, enum match with payloads, `Option`-style
unwrap_or, struct field mutation, boolean logic, generics (native rejects —
divergence noted). Only `/` and `%` on negative operands diverged between C and
VM; everything else agreed across all three backends.

**Pass 3 (i32 boundaries, diagnostics, include):** overflow add/mul (**found
#2a**), INT_MIN literal / `2³¹` (**found #7**), shift-by-≥32 (both mask to 5
bits — agree), INT_MIN/-1 (UB, coincide); native-vs-SML diagnostic quality on
type-mismatch / unbound / unknown-type / arity / return-mismatch / no-such-field
(**found #8** — native under-diagnoses), and `include` (**found #9** — unwired
on native).

**Pass 4 (contracts, stdout, deep control flow):** runtime contract enforcement
(**found #10** — native drops all contracts; VM aborts via uncaught exception);
`println` text output (C/native/VM all print correctly); deep loop / large
backward-jump bytecode (1000 iterations — all three agree, so the `le_byte`
sign-extension fix holds).

**Pass 5 (`?`/try, enum-returning, tuples, contract expressions):** the `?`
operator (**found #11** — native stubs), enum-returning function into a typed
`let` (**found #11** — native emits `i`), tuple annotation (**found #11** —
native empty emit), `old(x)` in `ensures` (works on C+VM; native drops per #10),
`forall` in `requires` (works C+VM). C and VM handle `?`/`old`/`forall`
correctly; native is where the gaps cluster.

**Emerging pattern:** the four native-side P1s (#1 method args, #8 missing type
checks, #10 dropped contracts, #11 `?`/enum-ret/tuple mis-emit) share a root
cause — the native mega-TU compiler was built to self-host its own `lib/*.sv0`
and pass a specific fixture set, both of which avoid these constructs. For
anything outside that diet it **silently emits wrong, stub, or empty C at exit
0** instead of a diagnostic. The gates never catch it because they check
bytecode-byte parity and self-host emit, never native runtime behavior on
arbitrary programs.

**Still not covered** (candidate areas for a further pass): the bytecode binary
format decoder edge cases (malformed/truncated bytecode); VM heap/GC pressure and
large data structures; nested/recursive `Box` on the VM; `exists` quantifier;
generic *type* instantiation depth; and formal conformance to `sv0doc/`. This
remains an inspection-plus-probe sweep, not a proof — but native feature
coverage is now clearly the dominant risk surface.

---

## Remediation tasks (bite-sized)

Each task is small and independently landable. IDs are `BH-<finding><letter>`.
Reminder: `parser.sv0`/`lowering.sv0`/`checker.sv0` are bootstrap lib modules — a
change to any of them requires refreshing **both** its stage0 golden
(`lib/golden/stage0/<stem>.c`) and its vm-parity golden
(`test/vm-parity/golden/sml/<stem>.sv0b` via `scripts/capture_vm_parity_goldens.sh`),
then a green `./scripts/sv0 test`. `sml-legacy/*` and `sv0vm/*` changes shift no
sv0c goldens.

### #1 — Native method-call compound args (P1) — ✅ DONE (sv0c)
- [x] **BH-1a** Added fixture `test/integration/mcall_compound_arg/` (`p.add(n+17)` → exit 42); confirmed it failed native (returned 25), passed SML.
- [x] **BH-1b** `parser.sv0` tag-5 parse: `ed3 = argc`, `ed4 = block_stmt_sidecar_push(pp, arg_roots_m)` (mirrors Call(4)).
- [x] **BH-1c** Arg-root reads via `block_stmt_index(pp, _, ed4, k)` in **all three** consumers — `lowering.sv0`, `resolver.sv0` (site 911), `checker.sv0` (site 3222) — plus the tag-5 unit test (`test_lower_expr_method_call_effect`).
- [ ] **BH-1d** `link.sv0` reloc/merge: add tag-5 to the pp-sidecar handling (join tags 4/9/11). **Deferred** — latent (CLI uses source-concat, not arena-merge).
- [x] **BH-1e** Refreshed parser+resolver+checker+lowering stage0 + vm-parity goldens; gated in `pc3b6-native-project-acceptance.sh` + SML harness (26/26 integration).

### #2 — VM integer arithmetic (P1) — ✅ DONE (sv0vm `a0876d9`)
- [x] **BH-2a** `interpreter.sml`: wrapped `ADD_I32`/`SUB_I32`/`MUL_I32`/`NEG_I32` to 32-bit via `Word32`.
- [x] **BH-2b** `interpreter.sml`: `DIV_I32 => Int.quot`, `MOD_I32 => Int.rem`, wrapped through `Word32` (fixes INT_MIN/-1 too).
- [x] **BH-2c** Div-by-zero: `DIV` already raised `Fail`; added the matching `MOD`-by-zero guard. *(kept the existing `Fail` abort; a clean exit-code panic is deferred to BH-10c's VM-abort cleanup.)*
- [x] **BH-2d** Added an arithmetic regression test to `sv0vm/test/bytecode_test.sml` (overflow add/mul, negative div/mod, INT_MIN/-1). The full cross-backend differential harness is still **BH-X1**.

### #3 — Stale pty tag scheme (P2/P3) — ✅ DONE (sv0c)
- [x] **BH-3a** `resolve_field_ty_tag`: fixed mapping to 1→TY_REF, 2→TY_REFMUT, 5→TY_TUPLE, 3/4/7→-1.
- [x] **BH-3b** `ast_ty_to_ty_payload` fixed identically (same wrong block; one `replace_all`). Both kept (minimal fix; dead but now correct).
- [x] **BH-3c** Added ref/refmut/tuple/array cases to `test_resolve_field_ty_tag`.

### #4 — Void+contract silent native fail (P3) — ✅ FIXED
- [x] **BH-4a (Slice 5)** ✅ `check_program`: `id2` packs `has_ret = ret_bit +
  contract_count*2`; a fn with `contract_count > 0` but `ret_bit == 0` (contract,
  no `-> T`) now pushes `E0409` to the diagnostic sink (span = fn name token),
  matching SML, instead of the ret scan silently resolving `TY_UNKNOWN` → bare
  exit 4. Valid code never has this shape (SML rejects it), so corpus-parity
  stayed 98/98. Case `void_contract.sv0`, dual-gated.

### #5 — Cross-module methods on SML `--project` (P2, optional)
- [ ] **BH-5a** `link.sml` `mapExpr` ExprMethodCall: resolve the method via the receiver's type module (type-directed) instead of mangling with the calling module id. *(deferred — native already works)*

### #6 — Checker block-scope leak (P3)
- [ ] **BH-6a** `checker.sv0` `check_stmt_in_block`: save env length before a block, restore after (drop bindings that go out of scope). Pairs with BH-12b.

### #7 — `2³¹` literal inconsistency (P2/P3) — ✅ FIXED
Semantics chosen: **wrap to i32** (`2147483648` → INT_MIN), matching SML→C's
existing runtime behavior; all three backends now agree.
- [x] **BH-7a** Native C: `int_to_string(INT_MIN)` special-cased — `0 - n`
  overflows back to negative, so `int_to_str_pos` emitted `-?` (invalid C). Now
  detects the negation-overflow (`0 - n` still `< 0`) and emits `"-2147483648"`
  directly (`lib/codegen.sv0`). This is a general codegen robustness fix (any
  INT_MIN constant, not just this literal). Unit case added to `test_int_format`.
- [x] **BH-7b** VM emit: `VmCodegen.int32` did `Int32.fromLarge 2147483648` →
  uncaught `Overflow` (silent exit 1). Now wraps through `Word32`
  (`sml-legacy/backend/vm/vm_codegen.sml`), truncating `2³¹` → INT_MIN to match
  the C wrap; the VM interpreter's Word32 arithmetic (BH-2) then evaluates
  `0 - INT_MIN` correctly at run time. Identity for in-range values (only
  `codegen.sv0b` vm-parity golden shifted, from the codegen.sv0 source edit).
- [x] **BH-7c** Fixture `test/integration/int_min/` (exit 42) gated on all three:
  native `--project` (pc3b6), SML `one` mode, sv0vm integration.

### #8 — Native checker under-diagnoses (P1, milestone) — 🔨 IN PROGRESS
Full plan: **`doc/bh8-native-diagnostics-plan.md`** (sink threading, slice order,
risk). Key discovery: the diagnostic layer (`lib/diagnostic.sv0`) and type
inference (`synth_expr`/`expect`) already exist — this is wiring a *diagnostic
sink* into the checker + adding the missing checks, not building from scratch.
Sliced:
- [x] **BH-8f / Slice 0 (enabler)** ✅ DONE. Threaded a `diag_sink: Vec<i32>`
  (flat `(code, span_tok)` pairs) through `check_program → check_fn_body` — the
  checker stays formatter-free (just `vec_push`). The compose main
  (`megaTU-main.sv0`) maps codes→`Exxxx`+message, resolves the token→(line,col),
  and formats via diagnostic.sv0's SML-parity formatters to stderr. Added
  `scripts/verify_diagnostics_corpus_behavior_native.py` (native twin — asserts
  the needle **and** a nonzero exit) wired into `./scripts/sv0 test`. Gates on
  `type_mismatch.sv0 | E0400`.
- [x] **BH-8c (Slice 0 scope)** ✅ Return-type mismatch for **literal** returns:
  `check_fn_body` re-checks each top-level `return <ExprLit>` and pushes `E0400`
  when the literal's type conflicts with the signature (`return true` in `-> i32`).
  Restricted to `ExprLit` because `synth_expr` returns imprecise primitives for
  field/call/method returns (broadly trusting it false-positived `lib/span.sv0`).
  Broaden to non-literal returns once inference tightens.
- [x] **BH-8b (Slice 1)** ✅ `let x: T = <literal>` where primitive `T` conflicts
  with the literal's type (`let x: i32 = true`). Done in the same top-level
  `check_fn_body` walk: LetStmt d2 = the annotation type-head **token** (not a pty
  root), so `ast_type_name_to_tag(name)` gives the primitive tag (or -1 for
  struct/enum, which is skipped), `infer_lit` gives the init type, and a primitive
  mismatch pushes E0400. Literal-only (same inference caveat as Slice 0). Case
  `let_type_mismatch.sv0`, gated SML + native.
- [x] **BH-8e + BH-8g (Slice 2)** ✅ E0300 / E0301 / E0307 (and E0306 / E0309) all
  at once. **Key insight:** these are *name-resolution* diagnostics, and
  `resolve_program` already **returns the specific code** (301 unknown type, 300
  unbound, 307 arity …) — the compose main was collapsing it to a bare `exit 3`.
  Fix is entirely in `megaTU-main.sv0`: on `rr != 0`, map the code → `Exxxx` +
  message and emit via the existing formatter. No checker/resolver change, no
  golden churn (megaTU-main is not a bootstrap/golden module). Default span (the
  resolver threads no token yet). Cases `unknown_type.sv0` (E0301),
  `unbound_name.sv0` (E0300), `wrong_arity.sv0` (E0307); all dual-gated SML +
  native. (Handles the unknown-type case via the pty arena, so generics like
  `Vec<i32>` correctly resolve to `TY_NAMED`, not "unknown" — the head-token
  approach a checker-side E0301 would have needed was unsafe here.)
- [x] **BH-8d (Slice 3)** ✅ Field-existence → E0429. The ExprField synth arm
  returned `TY_INT()` unconditionally and the env carries no struct *identity*, so
  this is done as a focused pass in `check_program` (which holds the `sdef_*`
  struct tables + pty arena): for a top-level `<param>.<field>` where the
  parameter is a **bare struct** (resolved from its signature type root) and the
  struct lacks that field, push E0429. Deliberately narrow (single-seg ExprPath
  object, bare-`TyName` param — refs/generics/locals skipped) so the corpus's
  pervasive valid field access is never misflagged (corpus-parity stayed 98/98).
  Case `field_missing.sv0`, dual-gated.
- [ ] **BH-8a** Each slice appends its `rel|code` row to
  `test/diagnostics/manifest.txt`, gated on both SML + native. *(overlaps BH-X2)*

### #9 — `include` unwired on native (P2) — ✅ FIXED
- [x] **BH-9a** Native compose main now reads the single-file source via
  `expand_from_file` (read + `expand_file`) instead of `read_file`, mirroring
  SML's `expandFile` — done in `scripts/build-sv0-megatu-native.sh` (the
  `cli_read` patch, file-mode branch). `--project` intentionally left on plain
  source-concat: expanding an `include` there would double-define the includee.
- [x] **BH-9b** Fixture `test/integration/include_basic/` (`main.sv0` includes
  `helper.sv0` → exit 42). Gated single-file on **both** pipelines: native via
  `scripts/pc3b6-native-project-acceptance.sh` (file-mode wrapper), SML `one`
  mode via `task/sv0c-milestone-1/02-integration-test.sh`.

### #10 — Native drops contracts / VM ungraceful abort (P1 / P3) — ✅ FIXED
- [x] **BH-10a** ✅ `lower` (lib/lowering.sv0) called `lower_fn` with **empty**
  `req_roots`/`ens_roots` (line ~3768), dropping every contract. Now `lower`
  receives `fn_contract_root`/`fn_contract_base` and, per fn, splits the flat
  clause roots into requires/ensures and passes them through — `lower_fn` builds
  `Instr::Requires`/`Ensures` and the emit turns them into `sv0_requires`/
  `sv0_ensures`. The parser stores clause roots but not their **kind**; rather
  than thread a kind array through the whole parser, `contract_kw_pos_of_root`
  recovers it by finding a token in the clause expr (`expr_any_tok`) and scanning
  the token stream back to the nearest `requires`(83)/`ensures`(61) keyword.
  Advanced clauses (quantifiers `exists`/`forall`, `old`) are detected
  (`contract_is_advanced`) and **dropped** — the native emit doesn't lower them
  yet — so scope is simple requires/ensures. Verified: `f(-5)`, `ensures`
  violation, and `main requires(false)` all abort with exit 1 + the C-runtime
  message; `requires(true)` runs normally; corpus-parity 98/98.
- [x] **BH-10b** ✅ Fixture `test/integration/contract_violation/` gated on native
  (pc3b6: emit has `sv0_requires`, runs → exit 1 + "contract violation") and the
  VM (BH-10c → vm_exit:1).
- [x] **BH-10c** ✅ `interpreter.sml`: a runtime contract violation now raises a
  dedicated `ContractViolation` exception caught in `runWithStack`, which prints
  `sv0 contract violation: <msg>` to stderr and returns exit code 1 — a clean
  abort (and a proper `vm_exit:1`), not an uncaught `Fail`. Fixture
  `test/integration/contract_violation/` gated on the sv0vm harness (→ vm_exit:1).

### #11 — Native `?` / enum-ret / tuple / nested-struct mis-emit (P1/P2)
- [ ] **BH-11a** Fix `?` (ExprTry): (a) add the `?` postfix case to `parse_postfix_expr` (token 34 → tag-22 `ExprTry(inner)`) — *trivial, but insufficient alone*; (b) give `lower_tag_try` operand-type resolution for **bound variables** (`o?` where `o` is a param/local of enum type) — it currently only handles call / 2-seg-path operands and bails to `VUnit` otherwise, so (a) without (b) makes `?` silently wrong. **Deferred** — needs a lowering-side operand→enum-type lookup (params via `id5`/`fpn`/`fpt`; locals need a var→type map). Diagnosed 2026-08-12; see finding #11.
- [x] **BH-11b** Fixed enum-returning-`let` C type: `megatu_ty_name` now resolves type tokens via `handle_to_str` instead of the `name_of` value-name stub (which mapped index 15 → "i"). Fixture `enum_return_let` gated. (sv0c)
- [ ] **BH-11c** Tuples on native: either support, or emit a clean "tuples unsupported" diagnostic instead of empty output.
- [ ] **BH-11d** Nested structs (multi-slot sub-struct fields) on native + VM: support, or a clean diagnostic (see `span.sv0`).

### #12 — Shadowing invalid C + VM leak (P2)
- [x] **BH-12a native** ✅ (sv0c `40667e4`) Native C: a shadowing pre-pass in
  `megaTU-main.sv0` (`assign_shadow_indices` + `sh_walk`, a scope-stack arena
  walk) assigns each local-var name token a shadow index = number of currently
  **live** bindings of that name (0 = first). The emit (`megatu_val_name`)
  renders index N>0 as `<name>_N`; a `let` initializer is walked before the new
  binding is pushed, so `let x = x + 32` reads the old `x`. Live-depth (not a
  persistent counter) keeps disjoint sibling bindings at 0, so non-shadowing code
  is byte-identical → no golden churn, corpus-parity 98/98.
- [x] **BH-12a SML→C + BH-12b VM** ✅ Both fixed by **one** AST→AST alpha-rename
  pass, `ShadowRename.renameProgram` (`sml-legacy/ir/shadow_rename.sml`), run in
  `main.sml` after checking and before `Lowering.lower` for **both** targets (they
  share `Lowering.lower`, and the VM's `VmCodegen.emit` takes the same renamed
  AST). It renames a binding to a fresh `x__N` **only** when it shadows a
  currently-live binding of the same name, rewriting `ExprPath [x]` references to
  the current alias; a `let x = <init>` renames its init under the pre-binding env
  so it reads the old `x`. Crucially it participates for **every** binder — `let`,
  match-arm patterns, `for` indices — and enters every scope (block, arm, for
  body) with the enclosing env, so the match/for entanglement that sank the naive
  in-lowering rename does not arise. Non-shadowing code is structurally unchanged,
  so the (shadow-free) bootstrap corpus is **byte-identical → no golden churn**,
  self-host-loop 98/98. This supersedes the reverted in-lowering attempt and is
  cleaner than the native token pre-pass (one pass vs. two backends).
- [x] **BH-12c** ✅ Fixture `test/integration/shadowing/` (sequential `x` +
  nested-block `a` → 42), gated on **all three** backends: native `--project`
  (pc3b6), SML `one` mode, and sv0vm (`vm_exit:42`).

### #13 — Match guards invalid C (P2) — ✅ DONE (sv0c)
- [x] **BH-13a** Match lowering (SML `lowering.sml` + native `lowering.sv0`): bind → guard → body (bind now precedes the guard test).
- [x] **BH-13a2** Native checker (`lib/checker.sv0`): bind a top-level PatBind (`arm_pt == 1`) into the env — fixes catch-all `x => …` arms (which failed check) in addition to guards.
- [x] **BH-13b** Fixture `match_guard` (guards + catch-all bind → exit 42) on native `--project` + native single-file + SML.

### Cross-cutting
- [ ] **BH-X1** Build a behavioral 3-way differential harness (SML→C run vs SML→VM run vs native→C run) over a feature+arithmetic battery; wire into `./scripts/sv0 test`. Catches #1, #2, #8, #10, #12, #13 as regressions.
- [ ] **BH-X2** Expand the golden **fail** corpus beyond the 7 contract cases to cover the common `Exxxx` codes (feeds BH-8a).
- [ ] **BH-X3** Audit the native compose-main for other silent-stub/empty-emit fallbacks; convert "can't handle" paths into explicit `Exxxx` diagnostics (never exit-0 garbage).
