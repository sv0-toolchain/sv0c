# VM parity suite (milestone 3, v1)

**Goal:** **strict** bytecode parity between the **SML** bootstrap compiler and the future **sv0** compiler on a **curated corpus** (see `task/sv0-toolchain-milestone-3-self-host.Rmd`).

## v1 scope

- **`manifest.txt`** lists **`.sv0`** sources (paths relative to **`sv0c/`**), one per line. Lines starting with **`#`** are comments.
- **v1** mirrors **`lib/bootstrap-sources.list`** (same programs as **`bootstrap-build`** / **`./scripts/sv0 test`**) — **lib/lexer/parser** seeds, **VM-supported** integration tests (**`hello`**, **`contracts`**, **`no_alias_requires`** golden, **`patterns`**, **`structs`**, **`generics`**, **`call_arg_order`**, **`continue_loops`**, **`println_ok`**, other contract goldens as supported), plus **`test/vm-parity/programs/*.sv0`** smoke — **100** entries as of **vm-parity-v2.0** (nine more arithmetic / enum / control-flow smokes). **CI:** every **`manifest.txt`** path must also appear in **`lib/bootstrap-sources.list`** so **`bootstrap-build`** emits **`build/vm/<stem>.sv0b`** before the golden **`cmp`**. **`no_alias` / `VAddrOf`:** VM encodes **`&param`** as the parameter’s **local base slot** (**`i32`**) and **`sv0_no_alias`** as **`CALL_BUILTIN` 1** (same pointer-inequality idea as C **`sv0_no_alias`**). Programs that still need VM work (e.g. some **`forall` / `exists`** paths) stay off **`bootstrap-sources.list`** / manifest until covered.

## Golden bytecode (SML reference)

- Checked-in files: **`golden/sml/<stem>.sv0b`** where **`<stem>`** is the basename of the source without **`.sv0`** (matches **`build/vm/<stem>.sv0b`** from **`--target=vm`**).
- **Regenerate** after intentional VM-backend or lowering changes:

```bash
# from sv0-toolchain repo root (requires SML/NJ)
./scripts/capture_vm_parity_goldens.sh
```

Then commit updated **`golden/sml/*.sv0b`** in **sv0c** together with any compiler change.

## CI

Parent **`./scripts/sv0 test`** runs **`bootstrap-build`** (SML → **`.sv0b`** + VM run), then **`cmp`**s each **`build/vm/<stem>.sv0b`** against **`test/vm-parity/golden/sml/<stem>.sv0b`** for every entry in **`manifest.txt`**. Mismatch fails the build (same pattern as **stage0** C goldens).

When the **sv0** compiler emits VM bytecode, add a second tier (e.g. **`golden/sv0/`** or a dedicated compare step) without dropping the SML reference tier.
