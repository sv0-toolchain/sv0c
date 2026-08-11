# impl_methods — impl method bodies + `self` + method calls (PC-4c)

`impl Point { fn sum(self: Point) -> i32 { self.x + self.y } … }` with method calls
`p.plus(4)`, `self.sum()` (a method calling another method), and `p.scaled(2)` (a
**struct-returning** method bound into an annotated `let q: Point`). Exercises the
PC-4c acceptance: an impl method body referencing `self` fields, methods with extra
arguments, UFCS method calls, and a struct-returning method whose result is stored in
a typed local. Methods are lowered as free functions with `self` as the first
parameter, so `recv.m(args)` → `m(recv, args)`. Expected **exit 42**
(`q = p.scaled(2)` = `{8,30}`; `q.plus(4)` = `(8+30)+4`).

## Coverage

- **Native** (`build/sv0-megatu-native --project .` / single-file): **exit 42.** The
  parser needed changes (`lib/parser.sv0`): `self` as a NAMED parameter (was `-1`
  / unbindable, like `_`) and `self` as a value expression (`self.x`). The native
  parser already flattens impl methods to top-level `ItemFn` entries, and the native
  checker/lowering already handle those + method calls (`ExprMethodCall` → `Call`). For
  a struct-returning method into a typed `let` (`let q: Point = p.scaled(2)`), lowering
  (`lib/lowering.sv0`) infers the method's return type by name
  (`lower_mcall_ret_user_ty_tok`, the tag-5 analogue of `lower_call_ret_user_ty_tok`) and
  uses it both as the method-call `Call` result-temp type and as the `let`'s deferred
  `DeclNamed` type — the parser drops the annotation, so the init shape is the type source.
- **SML `--project`** (`sml @SMLload=build/sv0c --project .`): **exit 42.** Checker
  (`checker.sml`) registers impl methods in the mod env + checks their bodies + synths
  `ExprMethodCall` (UFCS `m(recv, args)`); lowering (`lowering.sml`) emits each method
  as a free fn + lowers `ExprMethodCall` → `Call`. `linkProjectDir` (`link.sml`) mangles
  each method def to `modId__name` (via `mapFn`), so `mapExpr` mangles the method-call
  name the same way — else the call looks up the unmangled name and is unbound (E0401).
  The method-call result temp is typed by `calleeRetCty`, which falls back to scanning
  `ItemImpl` bodies (`implMethodRetCty`) when the top-level `ItemFn` scan misses — impl
  methods live inside `ItemImpl`, so the fallback rescues the mangled method's struct
  return type (`main__Point`); the `let` itself keeps its retained annotation.
- Gated: native via `scripts/pc3b6-native-project-acceptance.sh` (`--project`), SML
  via the integration harness (`one`/single-file mode).

## Known limitations (not on the PC-4c acceptance path)

- **Cross-module method calls** (SML `--project`): `mapExpr` mangles a method-call name
  with the *calling* module's `modId`, which matches only when the method is defined in
  the same module (as here). A `p.m()` calling a method defined in another module would
  need import-alias resolution, like cross-module fn/type refs. Intra-module works.
