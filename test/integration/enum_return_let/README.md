# enum_return_let — enum-returning function into a typed `let` (BH-11b)

Regression for bug-hunt finding #11 (enum sub-case): a function returning a user
`enum` bound to an annotated `let` (`let a: Opt = mk_none();`) miscompiled on the
native pipeline — the C declaration type came out as the bogus name `i` instead
of `Opt` (cc: "use of undeclared identifier 'i'"), so the program failed to
compile. Root cause: `megatu_ty_name` consulted the `name_of` codegen *value-name*
test stub (which maps handles 1..15 to fabricated names, `15 → "i"`) for real type
tokens; an enum return-type token at index 15 got corrupted. Fixed by resolving
type ctypes directly via `handle_to_str`. Expected **exit 42** (`22 + 20`). Runs
on native `--project`, native single-file, and SML.
