# sv0c native executable output (F0/R0)

Governing spec: `~/Documents/project-specs/sv0c-runtime-executable/SPEC.md`
(tracked in the parent `sv0-toolchain` repo as
`task/sv0c-runtime-executable.Rmd` / `-checklist.Rmd`; this file is itself
`NEX-038`, and its host-matrix section is `NEX-037` — slices `NEX-001`
through `NEX-036` of the R0 corpus/project/reliability band are done as of
this writing).

## Current status

**The engine and both CLI command surfaces are real and tested end to
end.** Every phase described below — entry validation, runtime
resolution, host-compiler selection and probing, core-compiler invocation,
staging, atomic publication — is implemented and covered by a real
`--selftest` corpus (including all 114 rows of
`sv0c/test/behavior/manifest.txt` compiled, linked, and run) in the parent
repo's `scripts/native_exe_*.py`. Per spec §11.1/line 472 ("R1
documentation SHALL lead with the installed `sv0c` spelling"), the
examples below use the installed `sv0c --emit=exe` spelling directly —
that command is real (NEX-059/060: `native_exe_cli.py` → `native_exe_request.py`
→ `native_exe_main.py`, fronted by an installed `<install>/bin/sv0c`
launcher, `scripts/native_exe_install_launcher.py`). During repository
development, `./scripts/sv0 native-compile` is the exact same
implementation under a workspace-adapter name (spec §25's own sanctioned
substitution) — use it interchangeably with `sv0c --emit=exe` in any
example below when working inside this repo's own checkout.

## Single-file build

```console
$ sv0c --emit=exe hello.sv0
sv0c: built build/native/hello (backend=c, profile=dev, contracts=runtime)
$ ./build/native/hello
hello from sv0
$ echo $?
0
```

Equivalently, from inside this repo's own checkout (the workspace
adapter — same implementation, same normalized request, per §11.1):

```console
$ ./scripts/sv0 native-compile hello.sv0
sv0c: built build/native/hello (backend=c, profile=dev, contracts=runtime)
```

(Omitting `-o` triggers the default-naming rule — `<cwd>/build/native/<stem>`
— and creates the `build/native/` directory automatically.)


## Explicit output path

```console
$ sv0c --emit=exe -o dist/hello hello.sv0
```

`-o`'s parent directory must already exist for an *explicit* output path —
missing parents are a hard error, never silently created (ART-007). The
*default* `build/native/` directory (used when `-o` is omitted) is created
automatically.

## Project mode

```console
$ sv0c --emit=exe --project calculator
```

Project composition — recursive `.sv0` discovery, `module`/`use`
resolution, deterministic file ordering, duplicate-definition diagnostics —
is entirely the core compiler's job (`--project <dir>` mode); the driver
never concatenates sources or infers an entry file itself.

## Contract modes

```console
$ sv0c --emit=exe --contract-mode=runtime   file.sv0   # keep all checks
$ sv0c --emit=exe --contract-mode=verified  file.sv0   # strip proven `ensures` (needs z3)
$ sv0c --emit=exe --contract-mode=disabled  file.sv0   # strip every check
```

`verified` mode degrades soundly when `z3` isn't on `PATH`: nothing is
proven, so nothing is stripped — a missing solver can never turn into a
false "verified" claim.

## Manual run

The build command's own exit status reports *build* success or failure —
never the compiled program's. Run the program separately and inspect its
own exit status:

```console
$ sv0c --emit=exe answer.sv0
sv0c: built build/native/answer (backend=c, profile=dev, contracts=runtime)
$ ./build/native/answer
$ echo $?
42
```

## `.sv0b` is never converted to an executable

This feature builds an executable **from C emitted by the C backend only**.
The bytecode backend remains `.sv0 -> .sv0b`, and `sv0vm` remains the sole
executor for `.sv0b`. `--target=vm` combined with executable output, or a
`.sv0b` file given as input, is a usage error (exit 2) before any tool
runs — never a silent wrapper around the VM.

## Supported hosts and current limitations

Through R0, this feature supports a **hosted, POSIX, GCC/Clang-compatible**
development-profile build. It does not claim strict C99 conformance —
`sv0_runtime.h` uses GCC/Clang statement-expression extensions — or native
Windows support (the runtime and driver depend on POSIX APIs). WSL runs the
Linux build and produces a Linux executable, not a Windows-native one.

`--profile=release` (NEX-051), `--message-format=json` (NEX-041/059),
build records via `--build-record` (NEX-042/059), and `--keep-c`
(NEX-040/059) are all implemented and wired through both CLI surfaces —
see [`native-executable-stable.md`](native-executable-stable.md) for the
R1 stable-surface detail (profiles table, build-record schema,
reproducibility/sanitizer/performance gates).

**Still genuinely unwired, stated honestly rather than silently left for
a reader to discover**: `sv0.toml` project-configuration discovery
(`native_exe_config.py`, NEX-043) is fully built and tested in isolation,
but `native_exe_request.NativeBuildRequest.config_path` is hardcoded to
`None` in `normalize_request` — no CLI flag or auto-discovery path feeds
a real `sv0.toml` into an actual build yet. `--emit=c`
(`native_exe_emit_c.emit_c_only`, NEX-039 — write C atomically, never
invoke the host compiler) is similarly fully built but unreachable from
either CLI surface: `native_exe_cli.py`'s grammar explicitly rejects
`--emit=c` today (`"only --emit=exe is accepted here"`). Both are real,
scoped follow-up tasks, not aspirational claims.

Filesystem host I/O (`read_file`/`write_file`/`read_dir`) has no
dedicated fixture yet in the runtime-feature test suite — nothing in the
current behavior corpus exercises it, so it isn't a proven part of this
feature's tested surface.

## R0 host matrix and required CI jobs

Spec §20.1 requires evidence on at least:

| OS | Architecture | Host compiler | Profile |
|---|---|---|---|
| Ubuntu 22.04 (or the repo's pinned Linux image) | x86_64 | default GCC-compatible `cc` | dev |
| Ubuntu pinned image | x86_64 | a supported Clang | dev |
| macOS pinned image | arm64 | Apple Clang | dev |

**Current CI reality** (`.github/workflows/ci.yml`, updated by NEX-051d):
two jobs on Ubuntu 22.04 — `verify` (default `cc`, the full
`./scripts/sv0 test` suite plus the sanitizer/release-parity native-exe
checks) and `native-exe-clang` (`CC=clang`, scoped to the native-exe
checks specifically: behavior corpus, dev-vs-release parity, sanitizer
sweep — not the full test suite, to keep this leg's own SML/NJ
re-bootstrap cost proportionate to what it actually adds). Verified
locally before landing by running the exact three `native_exe_*.py`
commands with `CC=clang` set: 114/114 behavior-corpus programs, 114/114
dev-vs-release-parity programs (1 known-divergent), 114/114 sanitizer-sweep
programs (1 known/expected finding) — all passing. That local run is on
this dev machine's own Apple Clang (its `cc` and `clang` are the same
binary), so it proves the *mechanism* (the `CC` env var is honored end to
end) but not an actual GCC-vs-Clang behavioral difference; the CI job
itself, on a Linux runner where `cc` defaults to GCC, is the first place
this leg exercises a genuinely different compiler family.

**Still not applied**: the macOS Apple Clang arm64 leg. It's continuously
exercised only on developer machines (this doc, and the whole
`native_exe_*` test suite, was authored and verified on macOS arm64 with
Apple Clang) — not in the hosted CI matrix. Adding it means a job along
these lines:

```yaml
  native-exe-macos-arm64:
    runs-on: macos-14  # arm64 runner
    steps:
      - uses: actions/checkout@v4
        with: { submodules: recursive }
      - run: brew install z3
      - run: ./scripts/sv0 test-guards
```

Deliberately deferred rather than applied speculatively: it needs its own
full SML/NJ bootstrap on a runner type this project has never used in CI
before (unverified `ci-install-smlnj.sh` compatibility, unverified billed
minutes), and — unlike the Clang leg — nothing about it could be dry-run
locally in a way that adds real evidence beyond what developer-machine use
already provides. macOS x86_64 is not required for R0 (spec §20.1: "MAY be
added when runner access exists"); R1 either certifies arm64 in CI or
documents arm64-only macOS support with rationale (spec §20.2).
