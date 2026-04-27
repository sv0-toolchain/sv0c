# VM integration fixtures (`sv0c`)

These **`.sv0`** programs are exercised by **`task/sv0vm-milestone-2/02-integration-test.sh`** (invoked from **`./scripts/sv0 test`** as the VM pipeline integration step). Each case is compiled with the **SML** bootstrap compiler **`--target=vm`**, then the resulting **`.sv0b`** is run under **`sv0vm`**.

## Case list (keep in sync with the shell script)

| Stem | Path | Expected VM exit |
|------|------|-------------------|
| hello | `hello/hello.sv0` | 0 |
| contracts | `contracts/contracts.sv0` | 0 |
| patterns | `patterns/patterns.sv0` | 0 |
| structs | `structs/structs.sv0` | 0 |
| field_assign | `field_assign/field_assign.sv0` | 0 |
| generics | `generics/generics.sv0` | 0 |
| call_arg_order | `call_arg_order/call_arg_order.sv0` | 0 |
| enum_tuple_match | `enum_tuple_match/enum_tuple_match.sv0` | 0 |
| string_api | `string_api/string_api.sv0` | 0 |
| enum_struct_match | `enum_struct_match/enum_struct_match.sv0` | 0 |
| vec_api | `vec_api/vec_api.sv0` | 0 |
| option_result | `option_result/option_result.sv0` | 0 |
| box_expr | `box_expr/box_expr.sv0` | 0 |
| ast_types | `ast_types/ast_types.sv0` | 0 |
| import_use_match | `import_use_match/main.sv0` (project: eight **`use lib::…`** lines + guards + **`match`** on imported callees; `lib/lib.sv0`) | 0 |

Additional harness steps in the same script: **multi-file project** (`modules/`, exit **42**), **`println_ok`** golden, optional **gcd**, and **`test/data/golden/pass/no_alias_requires.sv0`**.

When you add a row here, add the corresponding **`run_exit_case`** (or project block) to **`02-integration-test.sh`** in the **meta-repo** and land the **`sv0c`** sources first (**submodule** → **meta** pointer + **`README.md`** **sv0c** SHA per **`29-submodule-git-visibility`**).

## Related

- **`sv0c/test/vm-parity/README.md`** — bytecode golden tier (different harness).
- **`sv0c/doc/transliteration-plan.md`** — milestone 3 transliteration map.
- **`task/sv0-toolchain-milestone-3-self-host.Rmd`** — M3 definition of done.
