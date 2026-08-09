# struct_field_pattern — plain struct field pattern in `match` (PC-4b)

`match p { Point { x, y } => x + y }` — a **plain struct** (single-segment path)
destructured in a match arm, binding its fields to their real member names. Distinct
from the enum struct-variant pattern `E::V { … }` (two-segment path, payload `p<i>`
slots). Irrefutable (one shape). Expected **exit 42** (`8 + 34`).

Compiles + runs to exit 42 on **both** the native `--project` (source-concat) and SML
`--project` pipelines; gated by `scripts/pc3b6-native-project-acceptance.sh` and the
SML integration harness (`task/sv0c-milestone-1/02-integration-test.sh`).

## Sites added (both pipelines previously rejected this)

- **Parser** (`lib/parser.sv0` `parse_pat_atom`, `sml-legacy/.../parser.sml`
  `parsePatAtom`): an `Ident {` in pattern position now routes to the struct-pattern
  parser (was treated as a bare `PatBind`, leaving `{ … }` unparsed → "expected =>").
- **Checker** (`sml-legacy/.../checker.sml`): allow a `TyStruct` match scrutinee
  (was enum/bool/i32 only, E0433) and a single-segment `PatStruct` in `bindPat`
  (bind each field to its struct field type). The sv0 checker already allowed both.
- **Lowering** (`lib/lowering.sv0`, `sml-legacy/.../lowering.sml`): a single-segment
  `PatStruct` arm extracts each field via its real C member name
  (`scrut.<field>`), not the enum payload `p<i>` slot.
