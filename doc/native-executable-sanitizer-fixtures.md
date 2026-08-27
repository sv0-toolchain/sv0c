# Native executable: sanitizer stress fixtures (NEX-048b)

Implements the red test named by
`~/Documents/project-specs/sv0c-runtime-executable/SPEC.md`'s NEX-048 row:
"known box/vector stress fixtures pass under sanitizers." Turns
`sv0c/doc/native-executable-ub-audit.md`'s (NEX-048a) flagged sites into
concrete fixture programs. These fixtures are **inert** until NEX-050
wires a sanitizer job to actually build and run them under ASan/UBSan —
this slice only establishes the fixtures and their documented expected
verdicts.

## Safe stress fixtures (in `test/behavior/manifest.txt`)

These have fully-defined, non-UB behavior and are already part of the
ordinary behavior-corpus PASS suite (`sv0c/test/behavior/manifest.txt`) —
listed here separately because they were specifically written to stress
audit Sites 1 and 6, not merely to cover a language feature.

| Fixture | Stresses | Expected exit | Expected sanitizer verdict |
|---|---|---:|---|
| `box_deref_chain_stress.sv0` | Site 1 (`sv0__box_deref_raw` pointer-cast punning) — 20 levels of recursive `Box<Expr>` allocation + `box_deref` | 210 | clean (no ASan/UBSan finding) |
| `vec_growth_stress.sv0` | Site 6 (`Vec<T>` handle table) — 200 pushes forcing several `realloc`-doubling growth cycles from the initial capacity of 8 | 188 | clean (no ASan/UBSan finding) |

Both verified against the real driver before being added to the manifest
(`build_native_executable`, unsanitized): exit codes above are observed,
not computed by hand alone.

Audit Site 2 (the `lowering.sv0::Value`/`codegen.sv0::Value`
cross-reinterpretation) is **not** given a dedicated fixture here — it is
internal to the sv0c bootstrap compiler's own generated C and isn't
reachable from a user sv0 program at all. It is already exercised
transitively by every self-host/bootstrap test in the existing suite
(`scripts/sv0 test`'s self-host loop, native mega-TU bootstrap); NEX-050's
sanitizer job running that existing suite is Site 2's actual coverage, not
a new fixture.

## UB-only fixtures (NOT in `test/behavior/manifest.txt`)

These deliberately trigger real C undefined behavior (audit Site 4:
signed-integer-overflow edge cases with no documented sv0 policy). Their
outcome is platform/compiler-dependent by definition, so they are **not**
asserted against a fixed expected exit code the way the manifest's PASS
corpus is — that would bake in one platform's UB-manifestation as if it
were guaranteed behavior, which is exactly the overclaim §21/§26 warn
against.

| Fixture | Triggers | Expected UBSan verdict | Observed today (unsanitized, arm64/Apple Clang, `-O0`) |
|---|---|---|---|
| `ub_arith_neg_i32_min.sv0` | `-INT32_MIN` (negation overflow) | "signed integer overflow" | no crash; silently wraps to `INT32_MIN` unchanged, exits 0 |
| `ub_arith_div_i32_min_by_neg1.sv0` | `INT32_MIN / -1` (division overflow) | "signed integer overflow" | no crash; exits 0 |
| `ub_arith_mod_i32_min_by_neg1.sv0` | `INT32_MIN % -1` (division overflow, same IDIV instruction) | "signed integer overflow" | no crash; exits 0 |

All three construct `INT32_MIN` via `0 - 2147483647 - 1` — every
intermediate step is representable and non-overflowing, so the deliberate
UB is isolated to exactly the fixture's final operation, not smeared
across the setup.

**Platform-dependence, confirmed by direct observation, not assumed**: on
this dev machine (arm64/Apple Clang), none of the three crash — ARM's
`SDIV`/negate instructions don't trap on signed overflow the way x86's
`IDIV` does (x86 raises `SIGFPE` for `INT_MIN / -1` at the hardware level,
independent of any sanitizer). This is itself evidence for NEX-051d's
release-profile CI matrix needing both host families represented: a
program relying on this undefined behavior could crash on one supported
host and silently misbehave on another.

## Open question left for NEX-050/051

The audit (Site 4) intentionally left open whether sv0 should adopt an
explicit overflow policy (wrapping, matching two's-complement hardware
behavior on most targets, or a runtime-checked panic matching
`sv0_vec_get`'s bounds-check precedent) rather than continue inheriting
raw C UB for these edge cases. NEX-050's sanitizer job running these three
fixtures under UBSan is the evidence NEX-051b's behavioral-parity gate
needs before deciding.
