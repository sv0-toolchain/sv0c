# include_basic — `include "file"` on the native single-file pipeline (BH-9)

Regression for bug-hunt finding #9. `include "relpath";` (SML expands it via
`IncludeExpand.expandFile` before lexing) parsed on SML but **not** on native:
the native compose main read the source with `read_file` and never expanded
includes, so a file beginning with `include "helper.sv0";` failed to parse
(exit 2). Fixed by reading the single-file source via `expand_from_file`
(read + include-expand) in the native build (`scripts/build-sv0-megatu-native.sh`),
mirroring SML's `expandFile`. Expected **exit 42** (`helper()`).

Single-file only: native `--project` uses source-concat (all `.sv0` in the dir),
which would double-define `helper` if it also expanded the `include`. So this is
gated single-file (native drv_path + SML `one` mode), not `--project`.
