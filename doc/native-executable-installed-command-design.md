# Native executable: installed `sv0c --emit=exe` command — design (NEX-060)

Implements the investigation half of OD-009
(`~/Documents/project-specs/sv0c-runtime-executable/SPEC.md` §33.2):
"Stable installed command transition from workspace adapter. Deadline:
before R1." OD-009 was never explicitly resolved as its own decision
record anywhere in this project's history, despite R1 (NEX-048…058)
having since closed — a real, honest process gap this doc closes now.

**Status: scoped, not implemented.** This doc records a completed
investigation and a concrete design; the actual implementation is left
for a future pass. See `task/sv0c-runtime-executable-checklist.Rmd`'s
NEX-060 row for status. Unlike NEX-055c, this investigation found the fix
to be **low-risk and directly implementable whenever picked up** — not a
compiler-internals change requiring its own dedicated high-risk session.

## The problem, and a correction to prior framing

NEX-059 closed the CLI gap for the workspace adapter
(`./scripts/sv0 native-compile`) but left `CLI-017` (installed-adapter
parity) open. NEX-059's own checklist/hub entries characterized the
remaining work as needing "the real sv0c compiler's own argument-parsing
internals changed... a comparably risky sv0c-internals change to
NEX-055c's." **That characterization was premature and is corrected
here**: it conflated `sv0c` (the eventual installed command name) with
`sv0c/build/sv0c` (an existing file that happens to share the name).

`sv0c/build/sv0c` is the SML heap-image bootstrap compiler — invoked as
`sml @SMLload=sv0c/build/sv0c <file>` purely to translate the mega-TU sv0
source into C during the one-time native-compiler build
(`scripts/build-sv0-megatu-native.sh`, step 3: "One-time bootstrap: SML
compiles the mega-TU sv0 -> C"). It has no argv-driven CLI grammar at
all, does not run at build-native time (only once, during the compiler's
own build), and is completely unrelated to what a user would invoke as
`sv0c --emit=exe hello.sv0`. **There is no existing native argv-parsing
compiler binary to modify.** This is a packaging problem, not a
compiler-internals problem.

## What was confirmed by direct investigation (not assumed)

- Spec §25 (worked user examples, line 1402) itself sanctions today's
  arrangement: "The examples use the intended installed spelling. During
  repository development, replace `sv0c --emit=exe` with `./scripts/sv0
  native-compile`." The workspace adapter was always meant to be a
  legitimate stand-in until the installed spelling is packaged, not a
  permanent parallel path.
- Spec §7.5's explicit non-goals rule out ever building "a package
  registry or dependency downloader" or "automatic code signing,
  notarization, installers, or package-manager publication." NEX-060 must
  never grow into a `.pkg`/`.deb`/installer. What IS in scope (§25.1's
  "installation-layout tests outside a source checkout", AC-023, already
  exercised once by NEX-046) is producing a real, self-contained
  **installed directory layout** plus a launcher inside it.
- **The existing Python driver's path resolution already works standalone
  from an arbitrary install prefix, unmodified, with zero code changes.**
  `native_exe_runtime.resolve_runtime_dir()` (no override) resolves
  `sv0c/runtime` relative to `Path(__file__).resolve().parent.parent` —
  i.e. `<wherever native_exe_runtime.py lives>/../sv0c/runtime`.
  `native_exe_build.DEFAULT_COMPILER_PATH` resolves `build/sv0-megatu-native`
  the same way, relative to `<wherever native_exe_build.py lives>/..`. Any
  directory tree that preserves the shape
  `<root>/scripts/native_exe_*.py`, `<root>/sv0c/runtime/*`,
  `<root>/build/sv0-megatu-native` therefore resolves correctly using the
  driver's *default* arguments alone — no `runtime_override`/
  `compiler_path` test seams needed. NEX-046's own test
  (`native_exe_installed_layout.py`) never actually proved this in
  practice, since it always passes those override seams explicitly rather
  than exercising default resolution from a relocated tree.
- A separate, real, pre-existing CLI gap was found while reading
  `native_exe_cli.py`'s grammar during this investigation: `--emit=c`
  (`native_exe_emit_c.emit_c_only`, NEX-039 — write C atomically, never
  invoke the host compiler) is a fully-built, fully-tested engine function
  that is **unreachable from any CLI today** — `native_exe_cli.py`
  explicitly rejects `--emit=c` with `"conflicts with this
  native-executable driver (only --emit=exe is accepted here)"`. This is
  real and worth fixing, but it is distinct from CLI-017 (installed-adapter
  *parity* for the exe path); NEX-060 deliberately does not fold it in —
  flagged separately rather than silently absorbed into this slice.

## Decision (resolving OD-009)

**`sv0c --emit=exe` is a thin launcher that forwards into the exact same,
already-fully-tested Python driver
(`native_exe_cli.py` → `native_exe_request.py` → `native_exe_main.py`) —
not a new implementation, and not a change to any sv0c compiler
internals.** This directly follows OD-001's own already-made resolution
(the entire NEX-001…059 body of work built "the host driver" in Python,
isolated into testable modules, per OD-001's literal default
recommendation) — OD-009 is simply the packaging consequence of a
decision this project already made and has been living with for 59
slices. The bar this must clear is AC-023 (installed layout, path with
spaces, no source checkout) and CLI-017 (installed spelling constructs
the identical normalized request as the workspace spelling).

**Explicitly not chosen, and why**: a full native/C or self-hosted-sv0
reimplementation of the driver (so the installed product needs no Python
interpreter) would be architecturally purer and more "installer-grade,"
but requires either porting ~50 tested Python modules to C for no
functional gain, or growing sv0 itself a subprocess-spawn/exec builtin
that doesn't exist at all today — a strictly bigger, harder gap than
NEX-055c's `getenv` gap, which only needed a value *read*, not a spawned
child process. Filed as a distant, thematically-motivated (matches this
project's overall self-hosting arc) but explicitly out-of-scope future
direction, not attempted here or implied as imminent.

## Recommended sub-slices (for a future implementation pass)

1. **NEX-060b** — Generalize `scripts/native_exe_installed_layout.py`
   (NEX-046) from a test-only runtime+binary copier into a real, reusable
   `build_install_layout(dest_dir)` that additionally copies the full
   `native_exe_*.py` dependency closure into `<dest_dir>/scripts/`,
   preserving the exact relative shape `resolve_runtime_dir`/
   `DEFAULT_COMPILER_PATH` already assume. New test: build a layout at a
   temp prefix with a space in the path, invoke
   `python3 <dest>/scripts/native_exe_main.py hello.sv0 -o <dest>/hello`
   as a real subprocess with **no overrides passed at all**, confirm it
   runs correctly — the default-resolution gap NEX-046 left open.
2. **NEX-060c** — The `<dest_dir>/bin/sv0c` launcher: a short shell script
   (matching `scripts/sv0`'s own precedent of being a bash dispatcher
   fronting Python driver modules, and `build-sv0-megatu-native.sh`'s
   wrapper's own self-locating idiom) that execs
   `python3 "$ROOT/scripts/native_exe_main.py" "$@"`. No new argv
   translation logic needed — `native_exe_cli.py` already speaks the exact
   installed grammar spec §11.1 requires (`--emit=exe` already accepted;
   `--target=vm`/`.sv0b` already rejected per GOV-002/003).
3. **NEX-060d** — Per spec line 472 ("R1 documentation SHALL lead with the
   installed `sv0c` spelling"), rewrite `sv0c/doc/native-executable-stable.md`'s
   worked examples to lead with `sv0c --emit=exe ...`, demoting
   `./scripts/sv0 native-compile` to a documented workspace/development
   adapter note. Correct NEX-059's own checklist/hub prose (the
   "comparable risk to NEX-055c" characterization) in the same pass.
4. **NEX-060e** — Re-run `verify_native_exe_traceability.py
   --active-release R1`; `CLI-017` should move out of the uncited list.
   Checklist + hub updated the same way every other slice is.

Each sub-slice follows the identical discipline as every prior slice:
`--selftest`, a genuine mutation check, `test-guards` wiring, one commit.
