# Bootstrap compiler in sv0 (`lib/`)

This tree holds **sv0 source** that mirrors slices of the SML bootstrap compiler (`sml/**/*.sml`) for the self-hosting roadmap (`task/sv0-toolchain-milestone-3-self-host.Rmd`).

Sibling directories at the `sv0c/` root (e.g. **`lexer/`**, **`parser/`**) are reserved for additional transliterated modules as the surface grows. **`sml/`** is the bootstrap implementation; **`lib/`** + those dirs are the sv0 track.

## Build

From the **sv0-toolchain** root:

```bash
./scripts/sv0 bootstrap-build
```

CI reads **`lib/bootstrap-sources.list`**: one path per line (relative to `sv0c/`). If the list is missing or empty, every top-level `lib/*.sv0` is used. Each listed file is compiled to `build/vm/<stem>.sv0b` and run on **sv0vm** (exit code 0).

**Layout and transliteration order:** [`LAYOUT.md`](./LAYOUT.md).

**Golden bootstrap C:** `golden/stage0/<stem>.c` — compared on `./scripts/sv0 test` / `ci` to fresh SML heap output for the matching source file.

**C snapshots:**

```bash
./scripts/sv0 emit-c lib/span_core.sv0
./scripts/sv0 self-host-capture-stage0 lib/span_core.sv0
./scripts/sv0 self-host-compare span_core
./scripts/sv0 self-host-check-golden
```

From `sv0c/` after `make heap`:

```bash
sml @SMLload=build/sv0c lib/span_core.sv0
echo 'CM.make "sources.cm"; Main.main ((), ["--target=vm", "lib/span_core.sv0"]);' | sml
```

## `span_core.sv0`

Transliterated subset of `sml/error/span.sml` / `span.sig`. `main` uses a simple `return` so the bootstrap IR/VM path emits an entry function.

## `diagnostic_core.sv0`

Transliterated subset of `sml/error/diagnostic.sig` + `diagnostic.sml`: enum `Severity`, proxy `Diagnostic` (i32 fields for code/message/span/related/help shapes), and a **byte-length** model of `format` (header, `  -->` line, optional gutter+snippet, related/help block sizes, final newline). Helpers use scalar `i32` parameters; see [`LAYOUT.md`](./LAYOUT.md) for transliteration order.
