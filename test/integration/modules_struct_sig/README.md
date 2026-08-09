# modules_struct_sig — cross-module type in a fn SIGNATURE (PC-3c)

`lib` defines `struct Point`; the importer uses `Point` **in a function signature**
(`fn shift(p: Point) -> Point`), not only as a local annotation (that is
`modules_struct_type`, PC-1). Exercises the checker resolving a cross-module type in
a param + return position over the merged program. Expected **exit 42**
(`shift(origin())` = `{4,4}`, then `4 + 4 + 34`).

## Native `--project` (source-concat) vs SML `--project` (mangling)

- **Native** (`build/sv0-megatu-compiler-native --project .`): **compiles + runs to
  exit 42.** The collision-free source-concat merge keeps `Point` an ordinary
  top-level struct, so the sv0 checker resolves the signature type directly — no
  per-case canonicalization needed. This is the PC-3c acceptance surface (gated by
  `scripts/pc3b6-native-project-acceptance.sh`).
- **SML reference** (`sml @SMLload=build/sv0c --project .`): **fails `E0406: unknown
  return type`.** The SML `linkProjectDir` mangles `Point → lib__Point`, but its
  checker does not canonicalize the fn-signature return/param type through the import
  alias (the Epic-1 fix covered only the local-declaration C type, not the checker's
  signature type check). A known limitation of the frozen mangling-model reference;
  the go-forward native pipeline does not have it. This fixture is therefore **not**
  wired into the SML `--project` integration harness.
