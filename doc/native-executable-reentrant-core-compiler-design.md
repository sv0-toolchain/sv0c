# Native executable: reentrant core-compiler entry point — design (NEX-055c)

Implements the investigation half of REL-004
(`~/Documents/project-specs/sv0c-runtime-executable/SPEC.md` §22.2): "the
production core compiler interface SHALL accept argv directly or a unique
private request channel. The current global control file is a release
blocker."

**Status: scoped, not implemented.** This doc records a completed
investigation and a concrete design; the actual fix is deliberately
deferred to its own dedicated future session given its size and risk
(comparable to NEX-016/NEX-017, but touching more call sites and the
compiler's own self-host bootstrap correctness). See
`task/sv0c-runtime-executable-checklist.Rmd`'s NEX-055c row for status.

## The problem, confirmed by direct investigation

`scripts/native_exe_core_compiler.py`'s `CoreCompilerClient` is an
explicitly-documented migration shim: every core-compiler invocation
writes a request into the single, global, hardcoded file
`/tmp/.sv0_drv_path`, then invokes the compiled sv0 binary, which reads
that one fixed path back out. An `flock` on the same file serializes
concurrent invocations to prevent one build reading another's request.
This satisfies correctness today (proven by `native_exe_parallel_builds.py`,
NEX-035) but violates REL-004's static requirement outright: the global
control file must not appear in the stable executable path at all.

**Why a fixed global path is used in the first place**: sv0 the language
has no way for a compiled sv0 program to read process argv, an
environment variable, or stdin. Confirmed directly — the only host I/O
primitives that exist today are:

```
sv0_read_file(path: string) -> string
sv0_write_file(path: string, contents: string) -> ()
sv0_read_dir(dir: string) -> string
```

(`sv0c/runtime/sv0_runtime.h`/`.c`; recognized as builtins in
`sv0c/lib/checker.sv0` around line 3198, builtin IDs 13/14/15). Every one
of them takes the exact path/data it operates on as an sv0-level function
*argument* — there is no way for a compiled sv0 binary to discover a
*per-invocation* value at all without one of argv, env, or stdin, and none
of those exist as sv0 builtins yet. That absence is the root cause the
global-file design works around.

## The minimal real fix: a `getenv` builtin, not full argv

Each child process already has its own, unshared environment. Adding one
new host builtin,

```
getenv(name: string) -> string
```

(empty string, or a documented sentinel, when the variable is unset) is
enough to eliminate the shared-file race entirely: the Python wrapper sets
a unique, per-invocation environment variable on the child process it
spawns — either the literal request text directly
(`SV0_DRV_REQUEST=<...>`), or a per-invocation unique temp file path
(`SV0_DRV_PATH=<unique path>`, keeping the existing "control file holds
the request" shape but making the *path itself* per-invocation instead of
global) — with zero cross-process contention, satisfying REL-004's
literal "a unique private request channel" wording directly.

Full argv support (`argc` + a real argv array, e.g. `Vec<string>`) would
also satisfy REL-004, but needs strictly more new surface (array
marshaling into and out of C, a new sv0-level collection-of-strings
concept if one doesn't already fit cleanly) for no benefit over `getenv`
in this specific use case, where a single string value is all that's ever
actually passed.

## Implementation shape, following existing precedent exactly

**Update**: this section's sketch is superseded by a deeper, line-anchored
scoping pass — see
[`native-executable-getenv-builtin-scoping.md`](native-executable-getenv-builtin-scoping.md).
It corrects two things stated here: `write_bytes` (the most recently added
host builtin, not `read_file`/`write_file`/`read_dir`) is the more
current, more trustworthy template to follow; and `getenv` must return
`""` on an unset variable, never panic — `read_file`'s actual behavior
(re-read directly) is to panic on absence, the opposite of the
"returns empty/sentinel on absence" contract assumed just below.

Adding `getenv` is the same shape of change as adding `read_file`/
`write_file`/`read_dir` originally was (T0-8/M3 G2,
`sv0doc/compiler/bootstrap-host-io.md`), touching:

1. **`sv0c/lib/checker.sv0`** — recognize `"getenv"` as a builtin name with
   its arity/type signature, at the next available builtin ID after the
   existing 13/14/15.
2. **`sv0c/lib/resolver.sv0`** — name resolution for the new builtin.
3. **`sv0c/lib/lowering.sv0`** and/or **`sv0c/lib/codegen.sv0`** — IR
   construction and C emission calling a new `sv0_getenv` runtime
   function.
4. **`sv0c/runtime/sv0_runtime.h`/`.c`** — the actual C function, a thin
   wrapper over libc `getenv()`, mirroring `sv0_read_file`'s existing
   shape (including its "returns empty/sentinel on absence" contract,
   analogous to how a missing file is currently handled).

Each of these four changes gets its own `--selftest`-equivalent coverage
and mutation test, per this project's standing discipline — no different
from any other slice.

## Migration path, not a flag day

Once the builtin exists, `sv0c/lib/driver.sv0` and
`sv0c/lib/megaTU-main.sv0` (both currently read the entry request from
`/tmp/.sv0_drv_path` directly) gain a **second** read path: prefer the new
env-var-based request when the relevant variable is set, fall back to the
legacy control-file path otherwise. This mirrors the project's own
established migration discipline (e.g. the PC-6c native-default
promotion kept `SV0_SELF_HOST_COMPILER`/`SV0_SELF_HOST_LOOP_MODE` escape
hatches rather than a hard cutover).

Sequencing for the actual future implementation session:

1. Add the `getenv` builtin end-to-end, with its own tests.
2. Add the env-var read path to `driver.sv0`/`megaTU-main.sv0`, alongside
   (not replacing) the legacy control-file path.
3. Migrate `CoreCompilerClient` to set the new env var per invocation
   instead of writing+locking the shared file; drop the `flock`.
4. Re-run the full self-host loop and SML byte-guards to confirm the
   compiler still compiles itself correctly through the new channel —
   this is the step where a mistake would be most consequential (breaking
   the compiler's own ability to compile itself), so it gates every
   subsequent step.
5. Migrate the remaining direct `/tmp/.sv0_drv_path` writers
   (`scripts/sv0`'s several call sites) one at a time, keeping the legacy
   path live until every caller has moved.
6. Only then remove the legacy control-file path entirely and add a
   static guard (a `test-guards` check) confirming
   `/tmp/.sv0_drv_path`/`DEFAULT_CONTROL_FILE` no longer appears anywhere
   in the stable executable path — closing REL-004 for real.

## Why this is deferred rather than attempted now

This is a genuinely new sv0-language capability plus a driver-wide
migration across every existing caller of the legacy control file,
including the self-host loop whose bootstrap correctness is
safety-critical to the whole toolchain. It is comparable in *kind* to the
NEX-016 (`sv0_user_main` wrapper) and NEX-017 (reserved-symbol mangling)
changes, which each received their own dedicated planning-and-execution
pass — but larger in *scope*, since those touched one emission site each
while this touches the compiler's own entry-reading logic plus every
existing caller. Attempting it inline alongside R1's other ~30 slices
risks exactly the kind of rushed, under-verified change this project's
mutation-testing and self-host-parity discipline exists to prevent.
