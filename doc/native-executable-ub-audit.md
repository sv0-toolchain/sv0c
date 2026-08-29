# Native executable: C/runtime UB and strict-aliasing audit (NEX-048a)

Implements TOOL-012
(`~/Documents/project-specs/sv0c-runtime-executable/SPEC.md` §16.5): before
`--profile=release` (`-O2`) can be enabled, every raw pointer-punning,
box/vector representation, and signed-arithmetic site in `sv0_runtime.h`/
`.c` and the megaTU C emitter must be enumerated and classified as one of:

- **safe under `-O2`** — no aliasing/overflow assumption an optimizer could
  exploit differently than at `-O0`;
- **needs `-fno-strict-aliasing`** — correct only because the compiler
  doesn't perform type-based alias analysis; §16.5 explicitly anticipates
  this flag "if the current raw box representation remains";
- **needs a representation change** — no compiler flag makes it safe; the
  data layout itself must change.

This is a decision record from reading the actual runtime and bootstrap
compiler source (`sv0c/runtime/sv0_runtime.h`, `sv0c/lib/lowering.sv0`,
`sv0c/lib/codegen.sv0`), not a restatement of the spec's risk register.
It gates NEX-048b (turning the flagged sites into sanitizer fixtures) and
all of NEX-051 (release-profile enablement).

## Site 1 — box-pool deref via pointer-cast (`sv0__box_deref_raw`)

`sv0_runtime.h`:

```c
static intptr_t sv0_box_pool[SV0_BOX_POOL_SIZE];
...
#define sv0__box_deref_raw(h, T) (*(T *)(void *)&sv0_box_pool[(h)])
```

`sv0_box_pool`'s declared element type is `intptr_t`. `sv0__box_deref_raw`
reads a value of caller-supplied type `T` through a pointer cast from that
`intptr_t` storage. Under C's strict-aliasing rule (an object may only be
accessed through an lvalue of its own type, a compatible type, or
`char`/`unsigned char`), this is undefined behavior for any `T` that isn't
`intptr_t`-compatible — which is the entire point of the macro (it's used
to box arbitrary struct-shaped values).

The write side (`sv0__box_new_raw_impl`) is asymmetric with the read side:
it writes via `memcpy` (always well-defined, no punning), while the read
side casts-and-derefs (a live aliasing violation). A compiler doing
type-based alias analysis at `-O2` is entitled to assume a write through an
`intptr_t*` and a later read through an unrelated `T*` do not alias, and
could reorder or eliminate the read in principle — this hasn't been
observed to miscompile at `-O0`/`-Og`/current `-O2` on Clang/GCC's default
alias-analysis conservatism for this specific pattern in practice, but the
spec (correctly) treats "hasn't misbehaved yet" as insufficient evidence
for a stable release.

**Classification: FIXED (post-R1 cleanup pass, KC-004).** Originally
classified "needs `-fno-strict-aliasing`", deferred as a documented,
lower-priority follow-up rather than blocking R1. Since fixed for real:
confirmed by direct reading of every emission site (`megaTU-main.sv0`'s
`Call` codegen) that `sv0__box_deref_raw`'s expansion is used strictly as
an rvalue — `T dst = sv0__box_deref_raw(h, T);`, never an assignment
target — so the macro was rewritten as a statement expression that
`memcpy`s the pool bytes into a same-typed local (well-defined regardless
of strict-aliasing, mirroring `sv0__box_new_raw`'s existing pattern)
instead of a pointer-cast-and-deref. Verified: sv0c's 308/308 unit tests
pass; the full 114-program native behavior corpus passes unmodified; the
same corpus recompiled at `-O2 -Wstrict-aliasing=2` with **no**
`-fno-strict-aliasing` produces zero aliasing diagnostics and identical
exit codes to the mitigated build.

## Site 2 — cross-struct reinterpretation between `lowering.sv0::Value` and `codegen.sv0::Value`

`sv0c/lib/lowering.sv0` and `sv0c/lib/codegen.sv0` each independently
define an `enum Value` with the same variant order and payload shapes.
Both files carry matching comments (`lowering.sv0:65-71`,
`codegen.sv0:20-24`) documenting that this is deliberate: the megaTU
emitter's `box_deref` reinterprets a box built from `lowering.sv0`'s
`Value` as if it were `codegen.sv0`'s `Value` — two *nominally distinct*
sv0 enum types that compile to two distinct, differently-named C struct
types. Reading one through a pointer typed for the other is the same
underlying pattern as Site 1 (pointer-cast-and-deref through
`sv0__box_deref_raw`), but the aliasing violation here is between two
struct types that are merely *layout-compatible by convention*, not
between `intptr_t` and a payload type — a stronger, more fragile instance
of the same risk, embedded directly in the bootstrap compiler's own
generated C rather than in a generic runtime primitive.

**Classification: FIXED as a side effect of the Site 1 fix (KC-004).**
Originally classified "needs `-fno-strict-aliasing`", with a real
representation change (a shared type between the two files, or an explicit
reinterpretation helper) deferred as out of scope for R1. Since this site
is realized through the exact same `sv0__box_deref_raw` macro Site 1 uses
(confirmed above: "the same underlying pattern... through
`sv0__box_deref_raw`"), Site 1's fix — rewriting the macro to `memcpy` the
pool bytes into a same-typed local instead of a pointer-cast deref —
applies here too with no separate change needed: a `memcpy`-based read
into a local of the *requested* type `T` is well-defined regardless of
what type originally wrote those bytes, provided the byte pattern is a
valid representation of `T` (true here, by the "layout-compatible by
convention" property both files' matching comments already document).
Verified directly, not just inferred: the mega-TU compiler binary itself
(where this cross-struct reinterpretation actually executes, at compile
time, translating other `.sv0` sources) was rebuilt at `-O2` with **no**
`-fno-strict-aliasing` and used to recompile every `enum_*`/`struct_*`/
`combo_*` fixture in the behavior corpus — output byte-for-byte identical
(same exit codes) to the previously-mitigated build for every one.

## Site 3 — enum payload "flat tagged struct" field reuse (related, already mitigated)

`sv0c/lib/lowering.sv0` (`enum_payload_ty_name_to_category`, ~line 5925)
documents that every variant of an sv0 enum shares the *same numbered*
payload slot in its emitted C struct (a flat tagged struct, not a real C
union) — slot 0 might be `Some(T)`'s `T` for one variant and a different
type for another. This is **not** a pointer-cast aliasing violation like
Sites 1/2: each struct instance is only ever populated as one live
variant, and the category-resolution logic this function implements
(BUGS.md #8 in sv0-mathlib, already fixed and covered by
[[project_mathlib_toolchain_bugs]]) exists specifically to keep each
slot's *declared* C field type consistent across every variant that
actually uses it, preventing silent truncation/precision loss. No further
R1 action needed here — noted for completeness since it's adjacent to
Sites 1/2's "flat struct, not a union" pattern.

## Site 4 — signed integer arithmetic overflow (undocumented policy)

No sv0-level document defines an integer-overflow policy (wrapping,
trapping, or explicitly undefined) for `i32`/`i64` arithmetic. The
generated C for `+`, `-`, `*`, unary negation, `/`, and `%` on signed
integer types inherits whatever C itself does — which includes real
platform-observable UB at specific edge cases regardless of optimization
level:

- unary negation of the type-minimum value (`-INT32_MIN`, `-INT64_MIN`);
- `INT32_MIN / -1` and `INT32_MIN % -1` (and the `i64` equivalents).

The existing behavior-corpus fixtures (`arith_div_trunc`, `arith_mod`,
`arith_neg`) exercise ordinary truncating division/modulo/negation but do
not target these specific overflow-adjacent edge cases.

**Classification: needs a decision + fixture coverage**, not a
representation change — this is NEX-048b's job (turning this into a
concrete UBSan fixture) plus a documentation decision (does sv0 define
these as reachable-only-by-a-bug programmer errors, matching C's own
stance, or does it need an explicit runtime check similar to
`sv0_vec_get`'s bounds check?). Left open for NEX-048b/050 to resolve with
sanitizer evidence rather than resolved by assumption here.

## Site 5 — box pool / vec table: unchecked indexing (design, not UB)

`sv0_box_load`/`sv0_box_store` perform **no bounds check** against
`SV0_BOX_POOL_SIZE`, unlike `sv0_vec_get`/`sv0_vec_set`, which do check
against the vector's own length and `sv0_panic` on violation. For
compiler-generated indices (always in-range by construction of the megaTU
emitter's own box-allocation bookkeeping), this is safe in practice.
`sv0_box_alloc` itself does check the pool doesn't overflow its total
capacity.

**Classification: safe under `-O2` for well-formed emitted code.** Flagged
for NEX-048b's sanitizer fixture set as an intentional, documented
design choice (arena-style allocation, no per-load/-store bounds check) so
a future contributor doesn't mistake its absence for an oversight.

## Site 6 — `Vec<T>` handle table: uniform `intptr_t` storage

`sv0_vec_table[h].data` is declared and accessed as `intptr_t*`
throughout `sv0_vec_new`/`push`/`get`/`set` — every read and write is at
the table's own declared type, with pointer-to-`intptr_t` round-trip casts
handled at the *emitted C call-site* level (outside the runtime header)
rather than through any pointer-cast-and-deref inside the runtime itself.

**Classification: safe under `-O2`.** No punning inside the runtime;
pointer-integer round-trips are implementation-defined per C99 but not
UB, and are unaffected by strict-aliasing optimization.

## Site 7 — string operations

`sv0_string_len`/`_eq`/`_concat`/`_char_at`/`_substr` are thin wrappers
over `strlen`/`strcmp`/`malloc`/`memcpy` at their own natural types (no
casts to an unrelated pointer type, no punning).

**Classification: safe under `-O2`.**

## Summary table

| Site | Location | Classification |
|---|---|---|
| 1 | `sv0__box_deref_raw` (`sv0_runtime.h`) | FIXED (KC-004: memcpy-based read, no `-fno-strict-aliasing` needed) |
| 2 | `lowering.sv0`/`codegen.sv0` `Value` cross-reinterpretation | FIXED (KC-004: same fix as Site 1, same macro) |
| 3 | flat tagged-struct payload slot reuse | already mitigated (BUGS.md #8); no action |
| 4 | signed arithmetic overflow (negation, div/mod by -1 at type-min) | needs a decision + fixture coverage (NEX-048b) |
| 5 | box pool/vec table unchecked box indexing | safe under `-O2` (documented design, not UB) |
| 6 | `Vec<T>` handle table | safe under `-O2` |
| 7 | string operations | safe under `-O2` |

## Conclusion for NEX-051 (release profile)

Originally: `native_exe_argv_builder.build_release_profile_argv` (NEX-051a)
included `-fno-strict-aliasing` alongside `-O2`, covering Sites 1 and 2 —
the only sites this audit classified as needing a compiler-flag mitigation
rather than being already safe or needing further fixture-driven decision
work.

**Post-R1 update (KC-004 cleanup pass):** both Sites 1 and 2 are now fixed
at the representation level (see above) — no remaining site in this audit
needs `-fno-strict-aliasing`. `build_release_profile_argv` has had the flag
removed accordingly; the release profile now runs under full strict-aliasing
optimization with no known live violation, re-verified via the full
114-program behavior corpus (byte-identical exit codes with and without the
flag) plus the mega-TU compiler binary itself (where Site 2's cross-struct
reinterpretation actually executes) recompiling the enum/struct/combo
fixture set at `-O2` with no aliasing mitigation, output unchanged.

Site 4's arithmetic-overflow question is NEX-048b/050's job to resolve with
sanitizer evidence before NEX-051's behavioral-parity gate (051b) can
honestly claim dev/release equivalence on the full corpus — unrelated to
strict-aliasing and not affected by this update.
