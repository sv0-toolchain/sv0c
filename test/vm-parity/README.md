# VM parity suite (milestone 3, v1)

**Goal:** **strict** bytecode parity between the **SML** bootstrap compiler and the future **sv0** compiler on a **curated corpus** (see `task/sv0-toolchain-milestone-3-self-host.Rmd`).

## v1 scope

- **`manifest.txt`** lists **`.sv0`** sources (paths relative to **`sv0c/`**), one per line. Lines starting with **`#`** are comments.
- **v1** mirrors **`lib/bootstrap-sources.list`** (same programs as **`bootstrap-build`** / **`./scripts/sv0 test`**) — **lib/lexer/parser** seeds, **VM-supported** integration tests (**`hello`**, **`contracts`**, **`patterns`**, **`structs`**, **`generics`**, **`call_arg_order`**, **`continue_loops`**, **`println_ok`**, contract goldens), plus **`test/vm-parity/programs/*.sv0`** smoke — **91** entries as of **vm-parity-v1.9** (VM codegen fix for **`break`** in **`for`** + **`vm_for_break`**). Programs that use VM-unsupported lowering (e.g. **`no_alias`** / **`VAddrOf`**) stay out of this list until the VM backend catches up.

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
