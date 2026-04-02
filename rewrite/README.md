# sv0 rewrite track (bootstrap path)

This directory holds **sv0 source** that mirrors slices of the bootstrap compiler (`src/**/*.sml`) for the self-hosting roadmap (`task/sv0-toolchain-milestone-3-self-host.Rmd`).

## Build

From the **sv0-toolchain** root (or `sv0c/` with adjusted paths):

```bash
./scripts/sv0 rewrite-build
```

CI uses **`rewrite-sources.list`**: one path per line (paths relative to `sv0c/`). If the list is missing or empty, every `rewrite/*.sv0` is used. Each entry is compiled to `build/vm/<stem>.sv0b` and run on **sv0vm** (exit code must be 0).

**Layout and transliteration order:** see [`LAYOUT.md`](./LAYOUT.md).

**Golden bootstrap C:** checked-in files under `golden/stage0/<stem>.c` are compared on every `./scripts/sv0 test` / `ci` to the current SML heap output for the matching `rewrite/<stem>.sv0` (see `rewrite-sources.list`). Add or update a golden when you intentionally change emitted C.

**C backend / self-host snapshots** (from toolchain root):

```bash
./scripts/sv0 emit-c rewrite/span_core.sv0
./scripts/sv0 self-host-capture-stage0 rewrite/span_core.sv0   # → build/self-host-compare/stage0/span_core.c
./scripts/sv0 self-host-compare span_core   # diffs stage0 vs stage1 when both exist
./scripts/sv0 self-host-check-golden        # diff golden/stage0/*.c vs fresh emit (same as test)
```

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
