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

Sequencing for the actual implementation (updated in place as steps land,
rather than left as a stale forward-looking plan):

1. **Done** — the `getenv` builtin, end-to-end (sv0c commits `fc19be9`,
   `62baf46`).
2. **Done** — the env-var read path in `driver.sv0`/`megaTU-main.sv0`,
   alongside (not replacing) the legacy control-file path (sv0c commit
   `647d2a0`, parent commit `4474e53`).
3. **Done** — `CoreCompilerClient` migrated to set `SV0_DRV_REQUEST` per
   invocation instead of writing+locking the shared file; the `flock` and
   the control-file write/reset are both gone from
   `scripts/native_exe_core_compiler.py` entirely (parent commit pending,
   see checklist). Verified: the module's own selftest (real thread
   concurrency against a fake compiler, plus a deterministic aliasing
   check); a real end-to-end build through `./scripts/sv0 native-compile`
   confirming the legacy control file is never touched; 4 genuinely
   concurrent real OS-process builds, each correctly isolated;
   `native_exe_build.py`/`native_exe_emit_c.py` selftests unmodified;
   `./scripts/sv0 test`/`test-guards --active-release R1` show only the
   same pre-existing failures, no new ones.
4. **Done** — explicit self-host-loop + SML byte-guard re-verification.
   Formalized as its own checked step rather than left as an informal
   side effect of step 3's own verification: `./scripts/sv0 test` re-run
   fresh both immediately before and after step 5's changes, each time
   showing only the same two pre-existing failures (`checker rejects
   int/bool binop`; `self-host-sv0-loop: native run exit 232 for
   lib/checker.sv0`, both root-caused and recorded under `KC-001`
   earlier in this same body of work) — no new failures introduced by
   steps 3 or 5. This is the step where a mistake would be most
   consequential (breaking the compiler's own ability to compile
   itself); steps 3 and 5 are both pure driver-layer changes (Python and
   `scripts/sv0` respectively) that never touch sv0c compiler internals
   or the compiled binaries themselves, only how those binaries are
   invoked, so this step's own risk was never actually in play for
   either — confirmed empirically rather than assumed.
5. **Done** — `scripts/sv0`'s own remaining direct `/tmp/.sv0_drv_path`
   *request* writers (`run_compile`, `run_emit_verified`) migrated to
   `SV0_DRV_REQUEST`, following the same pattern as step 3's Python
   migration: an env var set per-invocation on the compiler subprocess,
   no file write, no reset-on-exit dance needed. Two harmless *reset-only*
   lines (in `run_verify` and `run_emit_verified`'s pass-1, both clearing
   the file to empty before invoking the *separate*
   `build/sv0-megatu-verify` binary, which reads its own distinct
   `/tmp/.sv0_verify_path` and was never affected by these resets) were
   removed as dead code in the same pass, verified behaviorally
   unchanged. `scripts/sv0`'s `ensure_sv0_self_host_compiler`/
   `ensure_sv0_megatu_native` file-existence guarantees were
   deliberately left in place — they are NOT request writers (they only
   guarantee the file exists so `driver.sv0`'s legacy fallback
   `read_file` doesn't panic), and every other still-unmigrated legacy
   caller (see step 6) still depends on that guarantee. Verified: real
   invocations of `sv0 verify`/`sv0 compile`/`sv0 compile
   --contract-mode=disabled`/`sv0 emit-verified` against real fixtures,
   each confirmed to leave the legacy file untouched throughout; the
   emitted C from `sv0 compile` compiled and ran correctly.
6. **Partially done, stated honestly** — NOT "remove the legacy
   control-file path entirely" as originally written above; a full-repo
   scan (done while implementing this step) found the legacy file still
   load-bearing in at least eleven more places beyond what steps 3/5
   migrated: `scripts/build-sv0-megatu-native.sh`,
   `build-sv0-megatu-verify-native.sh`, `build-sv0-megatu-vm-native.sh`
   (a **separate** injected compose-main for the VM-bytecode emitter
   target that step 2's `SV0_DRV_REQUEST` wiring never touched at all —
   it still reads the file unconditionally), `build-sv0-self-host-compiler.sh`,
   `sv0-megatu-corpus-parity.sh`, `sv0-megatu-native-parity.sh`,
   `sv0-native-behavioral-parity.sh`, `sv0-vm-tier2-native-emitter.sh`,
   `verify_behavior_corpus_native.py`, `assemble-sv0-megaTU.py`, plus
   `scripts/sv0`'s own file-existence guarantees from step 5.  Removing
   `driver.sv0`/`megaTU-main.sv0`'s legacy read path today would break
   every one of them — a real, separate, much larger migration than any
   single step in this sequence, left as an explicit, tracked follow-up
   rather than attempted or silently dropped. What WAS done: the other
   half of step 6 that is safe today — a static guard
   (`scripts/native_exe_no_new_legacy_control_file.py`, mirroring
   NEX-058's own `native_exe_no_duplicate_cc_recipe.py` precedent
   exactly) that fails closed if any file *not* on its exhaustive,
   documented allowlist references the legacy control file, stopping the
   channel from growing any further while its existing callers' real
   migration stays a tracked, separate future effort. Verified
   load-bearing by mutation (a synthetic new legacy-writing script is
   caught immediately). **REL-004 itself remains open** — closed only
   once the eleven-plus files above are migrated and the allowlist (and
   this guard) can be deleted entirely.

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
