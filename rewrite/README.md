# sv0 rewrite track (bootstrap path)

This directory holds **sv0 source** that mirrors slices of the bootstrap compiler (`src/**/*.sml`) for the self-hosting roadmap (`task/sv0-toolchain-milestone-3-self-host.Rmd`).

## Build

From the **sv0-toolchain** root (or `sv0c/` with adjusted paths):

```bash
./scripts/sv0 rewrite-build
```

This compiles every `rewrite/*.sv0` to `build/vm/<stem>.sv0b` and runs each on **sv0vm** (exit code must be 0).

From `sv0c/` only, after `make heap`:

```bash
sml @SMLload=build/sv0c rewrite/span_core.sv0   # C backend to stdout
echo 'CM.make "sources.cm"; Main.main ((), ["--target=vm", "rewrite/span_core.sv0"]);' | sml
```

## `span_core.sv0`

Transliterated subset of `src/error/span.sml` / `span.sig`:

| SML | sv0 |
|-----|-----|
| `type pos` (record) | `struct Pos` |
| `posLine` | `pos_line` |
| `length` on spans (offsets only) | `span_length` |

`main` uses only straight-line code so the bootstrap IR/VM path emits an entry function (some control-flow shapes in `main` are not yet lowered to IR in this slice).

File paths and full `Span` records are deferred until the sv0 surface has a stable owned-string story for structs.
