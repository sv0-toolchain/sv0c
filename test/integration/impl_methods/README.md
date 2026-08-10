# impl_methods — impl method bodies + `self` + method calls (PC-4c)

`impl Point { fn sum(self: Point) -> i32 { self.x + self.y } … }` with method calls
`p.plus(4)` and `self.sum()` (a method calling another method). Exercises the PC-4c
acceptance: an impl method body referencing `self` fields, methods with extra
arguments, and UFCS method calls. Methods are lowered as free functions with `self`
as the first parameter, so `recv.m(args)` → `m(recv, args)`. Expected **exit 42**
(`(8+30) + 4`).

## Coverage

- **Native** (`build/sv0-megatu-native --project .` / single-file): **exit 42.** Only
  the parser needed changes (`lib/parser.sv0`): `self` as a NAMED parameter (was `-1`
  / unbindable, like `_`) and `self` as a value expression (`self.x`). The native
  parser already flattens impl methods to top-level `ItemFn` entries, and the native
  checker/lowering already handle those + method calls (`ExprMethodCall` → `Call`).
- **SML single-file** (`sml @SMLload=build/sv0c main.sv0`): **exit 42.** Checker
  (`checker.sml`) registers impl methods in the mod env + checks their bodies + synths
  `ExprMethodCall` (UFCS `m(recv, args)`); lowering (`lowering.sml`) emits each method
  as a free fn + lowers `ExprMethodCall` → `Call`.
- Gated: native via `scripts/pc3b6-native-project-acceptance.sh` (`--project`), SML
  via the integration harness (`one`/single-file mode).

## Known limitations (not on the PC-4c acceptance path)

- **SML `--project` (mangling)**: fails `E0401: unbound value 'sum'` — `linkProjectDir`
  mangles the method definition but not the call site (the same mangling-model
  canonicalization gap tracked in `doc/pc3b-linkprojectdir-scoping.md`). The native
  source-concat `--project` never mangles, so it is unaffected. Hence SML is gated
  single-file here.
- **Struct-returning method into an annotated `let`** on native (`let q: Point =
  p.scaled(…)`): the parser does not retain a `let`'s type annotation, so the C type
  is inferred from the init, and the native method-call return-type inference is
  incomplete for a user struct/enum (the method-call temp defaults to `int`).
  i32-returning methods (this fixture) are unaffected.
