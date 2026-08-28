# `getenv` host builtin — implementation scoping (NEX-055c)

Deepens `native-executable-reentrant-core-compiler-design.md`'s
high-level identification of a `getenv(name: string) -> string` builtin
as REL-004's minimal real fix, into a full, file-by-file, line-anchored
implementation plan. **Status: scoped, not implemented** — this is the
builtin's own scoping only, not the wider REL-004 migration (adding the
env-var read path to `driver.sv0`/`megaTU-main.sv0`, migrating
`CoreCompilerClient`, retiring `/tmp/.sv0_drv_path`), which stays the
reentrant-core-compiler design doc's own steps 2–6, a separate, later
effort once this builtin exists and is proven self-host-safe.

Every claim below comes from directly reading the current source, not
from the prior design doc's own assumptions — one of them turned out to
be wrong, and is corrected below.

## The real, current precedent: `write_bytes`, not `read_file`

`read_file`/`write_file`/`read_dir` (checker builtin id 13/14/15) are the
*original* host-I/O builtins (T0-8/M3 G2). A **more recent** builtin,
`write_bytes` (checker id 16, lowering/emission id 18), was added after
them and is the more current template — confirmed by direct grep, it
touches exactly `checker.sv0`, `resolver.sv0`, `lowering.sv0`,
`megaTU-main.sv0`, and `runtime/sv0_runtime.h` — but, tellingly, **not
`sv0vm`'s interpreter at all** (`sv0vm/src/interpreter/interpreter.sml`
has cases for host-I/O builtin ids 15/16/17 but none for 18). This is a
real, existing precedent for a builtin shipping **C/native-only**,
deliberately absent from the VM — exactly the shape `getenv` needs: a
host-environment read has no meaningful role in `.sv0b`/VM execution, and
giving the VM interpreter arbitrary host-env access would be a new,
separate capability-boundary decision this scoping pass does not make.

## Correction to the prior design doc

The original design doc said `getenv` should mirror `read_file`'s
"returns empty/sentinel on absence" contract. **Read directly:
`sv0_read_file` has no such contract — it panics
(`sv0_panic("read_file: fopen failed")`) on a missing file.** A `getenv`
that panics on an unset variable would defeat its own purpose (the
core-compiler wrapper needs to detect "no new-channel request present"
and fall back to the legacy control file). `getenv`'s real contract must
be: **return `""` on an unset variable, never panic** — a deliberate
divergence from `read_file`'s panic-on-absence shape, documented here so
a future implementer doesn't "correct" it to match `read_file` too
literally. Accepted, real limitation of the `""`-sentinel design: a
variable explicitly set to the empty string is indistinguishable from an
unset one — acceptable for this builtin's actual use case (the
core-compiler wrapper always sets a non-empty value), stated honestly
rather than silently glossed over.

## The five real touch points, in dependency order

1. **`sv0c/lib/checker.sv0`** (~line 3182–3243). `BUILTIN_COUNT()` — a
   pre-existing, already-stale constant (returns `16` even though IDs run
   0–16 today) that no assertion anywhere actually consumes — stays
   untouched; this scoping doesn't fix it or rely on it. Add to
   `builtin_fn_lookup`: `if string_eq(name_str, "getenv") { return 17; }`.
   Add `bid == 17` arms to `builtin_fn_ret_type` (→ `TY_STRING()`,
   matching `read_file`/`read_dir`), `builtin_fn_param_count` (→ `1`), and
   `builtin_fn_param_type` (→ `TY_STRING()` for the one parameter). Extend
   `test_builtin_fn_lookup` (~line 5839) with a `"getenv"` → `17` case.
2. **`sv0c/lib/resolver.sv0`** (~line 113–166). Add `"getenv"` to
   `is_intrinsic` (→ `true`) and `intrinsic_arity` (→ `1`), in the same
   "Host I/O builtins... not in the original 17" comment block
   `read_file`/`write_file`/`read_dir`/`write_bytes` already live in.
   **Real finding, so a future implementer doesn't "fix" it by
   mistake**: `intrinsic_count()` (line 165) returns a hardcoded `17`,
   asserted by its own self-test (line ~1722) — but this counts only the
   *original* SML-parity intrinsic set (the doc comment above
   `is_intrinsic` says so explicitly: "17 intrinsics from
   `withIntrinsics` in resolver.sml"). `read_file`/`write_file`/
   `read_dir`/`write_bytes` were never added to this count either —
   `getenv` correctly follows that same established precedent and does
   **not** touch `intrinsic_count()`.
3. **`sv0c/lib/lowering.sv0`** (~line 3783–3805). Add
   `if string_eq(s, "getenv") { return 19; }` to
   `build_builtin_map_classify` (the next id after `write_bytes`'s 18).
   Extend `test_build_builtin_map` (~line 9522) with a case. No VM
   `CALL_BUILTIN` interpreter change needed — see the `write_bytes`
   precedent above.
4. **`sv0c/lib/megaTU-main.sv0`** (~line 1016–1050) — the real,
   actually-compiled-and-run C backend (per NEX-016's own established
   finding; `codegen.sv0` has zero host-I/O-builtin references today and
   doesn't need touching, for the same reason NEX-016 left it alone). Add
   `if bid == 19 { return "sv0_getenv"; }` to `megatu_builtin_name`, and
   `if bid == 19 { return "const char*"; }` to `megatu_builtin_ret_cty`
   (string-returning, alongside `read_file`'s 15 / `read_dir`'s 17).
5. **`sv0c/runtime/sv0_runtime.h`/`.c`**. Declare
   `const char *sv0_getenv(const char *name);` in the header, next to the
   `read_file`/`write_file`/`read_dir` declarations (~line 178). Define in
   `sv0_runtime.c`, next to `sv0_read_file` (~line 122):

   ```c
   const char *sv0_getenv(const char *name) {
     const char *v = getenv(name);
     return v != NULL ? v : "";
   }
   ```

   A thin libc wrapper — no `sv0_panic` path at all, per the corrected
   contract above. Its C return value is owned by the environment block,
   not `malloc`'d, unlike `read_file`'s buffer — this builtin never needs
   `free`, a real difference in ownership shape from every other
   string-returning host builtin so far, worth noting rather than
   assuming symmetry.

## Verification (for the future implementation session)

Same discipline as every other slice in this project:

- A real sv0 fixture exercising `getenv` end to end: set/unset an
  environment variable, compile+run via the real native pipeline
  (`build_native_executable`, reusing the existing `.sv0` fixture
  convention this project already has dozens of), assert the observed
  stdout/exit code reflects the variable's value or its absence.
- `sv0c`'s own `make test` (checker/resolver/lowering unit tests +
  `test_builtin_fn_lookup`/`test_build_builtin_map`'s new cases) stays
  green.
- The **existing 114-program native behavior corpus**
  (`verify_behavior_corpus_native.py`) must still pass unmodified — the
  same "stronger evidence than adding synthetic fixtures" standard
  NEX-016 used, proving the new builtin id didn't disturb any existing
  dispatch.
- The self-host loop (`./scripts/sv0 test`) must still pass — the step
  the original design doc correctly flagged as highest-consequence, since
  a mistake here risks the compiler's own ability to compile itself. A
  `getenv` addition changes 5 files' builtin-dispatch tables; the
  self-host loop is what actually proves the compiler can still compile
  *itself* through those same tables, unchanged.

Explicitly **not** part of this scoping or its future implementation:
wiring `getenv` into `driver.sv0`/`megaTU-main.sv0`'s own entry-reading
logic, migrating `CoreCompilerClient`, or touching any existing
`/tmp/.sv0_drv_path` caller — those remain
`native-executable-reentrant-core-compiler-design.md`'s own steps 2–6, a
separate, later effort.
