# modules_struct_sig — cross-module type in a fn SIGNATURE (PC-3c)

`lib` defines `struct Point`; the importer uses `Point` **in a function signature**
(`fn shift(p: Point) -> Point`), not only as a local annotation (that is
`modules_struct_type`, PC-1). Exercises the checker resolving a cross-module type in
a param + return position over the merged program. Expected **exit 42**
(`shift(origin())` = `{4,4}`, then `4 + 4 + 34`).

## Native `--project` (source-concat) vs SML `--project` (mangling)

Both pipelines now compile this fixture to **exit 42**; it is wired into the native
acceptance (`scripts/pc3b6-native-project-acceptance.sh`) and the SML `--project`
integration harness (`task/sv0c-milestone-1/02-integration-test.sh`).

- **Native** (`build/sv0-megatu-compiler-native --project .`): the collision-free
  source-concat merge keeps `Point` an ordinary top-level struct, so the sv0 checker
  resolves the signature type directly — no per-case canonicalization needed.
- **SML reference** (`sml @SMLload=build/sv0c --project .`): the mangling
  `linkProjectDir` renames `Point → lib__Point`, so every type-name site must
  canonicalize the imported short name through the import alias. This fixture
  originally exposed four missing sites (checker `E0406` fn-signature type, checker
  `E0400` struct-literal result type, lowering unknown-struct field lookup, and the
  lowering struct-construction temp C type); all four now apply `canonTyImport` /
  `canonTyName`. See `doc/pc3b-linkprojectdir-scoping.md` §5i.
