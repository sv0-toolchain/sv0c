# BH-8 — Native diagnostics attack plan

Bug-hunt finding **#8**: the native checker (`lib/checker.sv0`) silently accepts
several type errors and, where it does reject, emits no `Exxxx` code or message —
just a bare nonzero exit. This blocks promoting the native compiler to the
default (and eventually only) front end, since it would accept ill-typed
programs. See `doc/archive/bug-hunt-findings.md` §8.

## What already exists (this is wiring + missing-checks, not build-from-scratch)

- **Type inference** — `lib/checker.sv0` has `synth_expr` (expression type
  synthesis), `infer_lit`, `expect` (checker.sv0:582), `binop_result_ty_tag`,
  and the full `TY_*` tag set. It already synthesizes a function body's type and
  compares it to the return type in `check_fn_body` (checker.sv0:1999-2012).
- **A diagnostic layer** — `lib/diagnostic.sv0`: `make_error(code, msg, span)`,
  `diag_list_push` / `diag_list_has_errors` / `diag_list_error_count` /
  `diag_sort_by_span`, and `format_diagnostic` / `format_header` /
  `format_location` / `format_snippet`. Its output mirrors SML's
  `Diagnostic.format` (`sml-legacy/error/diagnostic.sml`) exactly:
  `error[E0400]: <msg>\n  --> <file>:<line>:<col>\n` + a `LN | source` gutter and
  a caret underline.
- **A gate harness** — `scripts/verify_diagnostics_corpus_behavior.py` reads
  `test/diagnostics/manifest.txt` rows (`rel_path | needle`), compiles each case,
  and asserts the needle appears in combined stdout+stderr. Today it is
  **SML-only** and has **one** case (`type_mismatch.sv0 | E0400`).

## The two real gaps

1. **No diagnostic sink.** Checker error sites `return -1`, which the native
   compose main (`lib/megaTU-main.sv0:85`) collapses into `exit 4`. The code,
   message, and span are discarded. Nothing carries an `Exxxx` code or a span out
   to stderr.
2. **Missing / no-op checks** (exact sites):
   - **BH-8c return type** — `synth_expr` ExprReturn arm (checker.sv0:3353-3366)
     synths `val_ty` then **unconditionally `return ret_ty`**; never calls
     `expect`. Hence `fn main() -> i32 { return true; }` is accepted.
   - **BH-8b let annot vs init** — LetStmt arm (checker.sv0:3061-3078) **ignores
     the annotation** and `env_extend`s with the *inferred* `ty_init`; no compare.
   - **BH-8d field existence** — ExprField synth has no struct-field membership
     check (`p.z` for a missing `z` defers to `cc`).
   - **BH-8e unknown type** — type-annotation resolution does not reject an
     unknown `TyName` (`let p: Widget = 0;` is accepted).
   - **E0300 unbound / E0307 arity** — caught by the *resolver* (`resolver.sv0`),
     surfaced as a bare exit 3 with no message.

## Reference output format (native must match the code needle)

SML `Diagnostic.format` (`sml-legacy/error/diagnostic.sml`):

```
error[E0400]: <message>
  --> <file>:<line>:<col>
LN | <source line>
   |    ^^^^
```

The behavioral gate only requires the **code** substring (e.g. `E0400`) to appear
in output, so native messages can be a fixed short string per code; span/caret
fidelity is a bonus, not a gate requirement.

## Sequenced slices (each independently gated, refresh goldens + full gate)

### Slice 0 — the enabler (de-risks the whole approach)
Thread a **diagnostic sink** (parallel `Vec<i32>`: code, span_line, span_col,
span_len — via the existing `diag_list_push`) through
`check_program → check_fn_body → synth_expr`. Wire **one** check to prove the
loop: ExprReturn `expect(val_ty, ret_ty)` → push `E0400` on mismatch.
`check_program` formats the sink to **stderr** via `format_diagnostic` and returns
nonzero. Add a **native twin** of the verifier (run
`build/sv0-megatu-compiler-native <case>`, assert needle in stderr + nonzero
exit). Gates on the case already in the corpus (`type_mismatch.sv0 | E0400`).
- Sub-dependency: a `tok → (line, col)` helper if diagnostic.sv0 lacks one (scan
  newlines up to `starts[tok]`). Folded into Slice 0.

**Slice 0 status: DONE.** Landed as above. **Key learning that constrains every
later slice:** `synth_expr` returns *imprecise primitive* types for
field/call/method returns (e.g. a string-typed struct field synths as int), so
you cannot broadly trust its result to flag a mismatch — doing so false-positived
`lib/span.sv0` (`return sp.file`). Slice 0 is therefore restricted to **literal**
returns (`ExprLit`, where `infer_lit` is reliable). Later slices must likewise
only flag when the synthesised type is *trustworthy* (literals, params, and
constructs with precise inference), or first tighten `synth_expr` for the
construct in question. The 98-module corpus-parity is the false-positive backstop
— run `scripts/sv0-megatu-corpus-parity.sh` after every checker edit.

### Slice 1 — BH-8b let annotation vs init (E0400) — DONE
The LetStmt d2 is the annotation type-head **token** (not a pty root), so
`ast_type_name_to_tag(name)` maps it directly to a primitive tag (or -1 for a
struct/enum name, which is skipped). Done in the same top-level `check_fn_body`
walk as Slice 0, restricted to **literal** inits (`infer_lit` is trustworthy;
non-literal inits await tighter inference). Case `let_type_mismatch.sv0`, gated
SML + native.

### Slice 2 — resolver diagnostics: E0301 / E0300 / E0307 — DONE
Reframed from a checker-side E0301: unknown-type, unbound, and arity are all
*name-resolution* diagnostics, and `resolve_program` **already returns the exact
code** (301/300/307/306/309) — the compose main was discarding it as a bare
`exit 3`. So Slice 2 is entirely in `megaTU-main.sv0`: on `rr != 0`, map the code
→ `Exxxx` + message and emit via the existing formatter. No checker/resolver
change, no golden churn. A checker-side E0301 off the let head-token was rejected
as unsafe (can't tell `Vec<i32>` from bare `Widget` without the pty arena's
type-arg count); the resolver path uses the pty arena and handles generics
correctly. Cases `unknown_type.sv0` / `unbound_name.sv0` / `wrong_arity.sv0`,
dual-gated. This also discharges the old **Slice 4 (BH-8g)**.

### Slice 3 — BH-8d field existence (E0429) — DONE
The ExprField synth arm returned `TY_INT()` unconditionally, and the env stores
no struct *identity* (only `TY_STRUCT`). Rather than add struct-identity tracking
to the env + thread struct tables through `synth_expr` (72 sites), the check runs
as a focused pass in `check_program` — which already holds the `sdef_*` struct
tables and the pty arena. For a top-level `<param>.<field>` whose parameter is a
**bare struct** (name recovered from its signature type root, refs/generics
excluded) and whose struct lacks the field, push E0429. Narrow by construction so
the corpus's heavy valid field access is never misflagged; corpus-parity stayed
98/98. Case `field_missing.sv0`.

### Slice 4 — E0300 unbound / E0307 arity (messages)
Route the resolver's existing rejections through the diagnostic layer (code +
message, not a bare exit 3). Lives in `resolver.sv0`. + cases.

### Slice 5 — BH-4a void + contract (E0409) — DONE
`id2` packs `has_ret = ret_bit + contract_count*2`. A fn with `contract_count > 0`
but `ret_bit == 0` (contract, no `-> T`) is E0409 in SML; the native ret scan
resolved it to `TY_UNKNOWN` and the body check failed silently. `check_program`
now pushes E0409 (span = fn name token) for that shape. Valid code never has it
(SML rejects), so corpus-parity stayed 98/98. Case `void_contract.sv0`.

Each slice appends its row to `test/diagnostics/manifest.txt` with the code
needle, gated on **both** SML (existing verifier) and native (Slice 0's twin).
This incrementally discharges BH-8a / BH-X2 (expand the fail corpus).

## Span fidelity (follow-up) — DONE for the resolver diagnostics

The resolver-sourced diagnostics (E0300/E0301/E0307) originally emitted a default
1:1 span because `resolve_program` returns only a numeric code. Rather than thread
a span param through `resolve_expr`'s 56 call sites, the offending token is
**packed into the return value**: `diag_pack(code, tok) = code*1000000 + tok+1`.
The code rides the existing `if r != 0 { return r; }` propagation untouched; the
compose main unpacks `diag_code_of` / `diag_tok_of` → real (line, col). Packed at
the three gated sites (unbound `pp[pps]`, unknown-type `pp[pps]`, arity
`pp[cpps]`); other coded sites stay bare and decode to a default span. **Gotcha:**
resolver self-tests that assert an exact code (`resolve_expr(...) != 300`) must be
wrapped in `diag_code_of(...)` or the packed value fails the compare and the
module RUNFAILs in corpus-parity — found `test_resolve_expr_path` (300) and
`test_resolve_expr_call_arity` (307) this way. Still default-span: the
checker-side E0400 *literal* return/init (an `ExprLit` has no path token) and the
non-gated resolver sites.

## Dominant risk & mitigation

`checker.sv0` is a bootstrap module: every edit shifts its stage0
(`lib/golden/stage0/checker.c`) + vm-parity (`test/vm-parity/golden/sml/checker.sv0b`)
goldens and must keep **self-host-loop 98/98 + all integration fixtures green**.
A new soundness check could reject currently-valid code if inference is imprecise
(`i64` → TY_INT, `isize`/`usize`, TY_NAMED aliasing). Mitigation: start
permissive, tighten per-case, lean on the 98-module corpus + fixtures as the
regression backstop, and run the full `./scripts/sv0 test` gate every slice. The
SML reference stays **untouched** (native-only work), so its diagnostics gate
keeps passing throughout.

## Invariants (per slice)

- SML reference untouched — native-only edits.
- Refresh `checker.c` stage0 + `checker.sv0b` vm-parity goldens after each edit;
  confirm churn is isolated to checker.
- `printf '' > /tmp/.sv0_drv_path` before any gate/push (see
  `feedback_drv_path_reset`).
- Full gate green; commit sv0c first, then parent (gitlink + README pin).
