# sv0c native executable output (F0/R0)

Governing spec: `~/Documents/project-specs/sv0c-runtime-executable/SPEC.md`
(tracked in the parent `sv0-toolchain` repo as
`task/sv0c-runtime-executable.Rmd` / `-checklist.Rmd`; this file is itself
`NEX-038`, and its host-matrix section is `NEX-037` — slices `NEX-001`
through `NEX-036` of the R0 corpus/project/reliability band are done as of
this writing).

## Current status

**The engine is real and tested end to end; the public CLI command is not
wired up yet.** Every phase described below — entry validation, runtime
resolution, host-compiler selection and probing, core-compiler invocation,
staging, atomic publication — is implemented and covered by a real
`--selftest` corpus (including all 112 rows of
`sv0c/test/behavior/manifest.txt` compiled, linked, and run) in the parent
repo's `scripts/native_exe_*.py`. What doesn't exist yet is the installed
`sv0c --emit=exe` command surface itself, or the `./scripts/sv0
native-compile` workspace adapter — that CLI-entry-point wiring is later
work, once this band's reliability gates (this doc's own closure) land.

The examples below describe the **target design** (spec §25's worked
examples, using the spec's canonical spellings) alongside how to exercise
the same behavior **today**, directly through the tested engine.

## Single-file build (target CLI)

```console
$ sv0c --emit=exe hello.sv0
sv0c: built build/native/hello (backend=c, profile=dev, contracts=runtime)
$ ./build/native/hello
hello from sv0
$ echo $?
0
```

Today, the same build runs through the engine directly (from the parent
`sv0-toolchain` repo root, so `scripts/` is import-visible):

```console
$ PYTHONPATH=scripts python3 -c "
from native_exe_build import build_native_executable
r = build_native_executable('file', '/abs/path/hello.sv0', None, '.')
print(r.message)
"
sv0c: built ./build/native/hello (backend=c, profile=dev, contracts=runtime)
```

(`output_path=None` triggers the same default-naming rule as omitting `-o`
on the target CLI — `<cwd>/build/native/<stem>` — and creates the
`build/native/` directory automatically.)


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

`--profile=release`, `sv0.toml` configuration, `--message-format=json`,
build records, and `--keep-c` are not yet implemented (R0.1+). Filesystem
host I/O (`read_file`/`write_file`/`read_dir`) has no dedicated fixture yet
in the runtime-feature test suite — nothing in the current behavior corpus
exercises it, so it isn't a proven part of this feature's tested surface.

## R0 host matrix and required CI jobs

Spec §20.1 requires evidence on at least:

| OS | Architecture | Host compiler | Profile |
|---|---|---|---|
| Ubuntu 22.04 (or the repo's pinned Linux image) | x86_64 | default GCC-compatible `cc` | dev |
| Ubuntu pinned image | x86_64 | a supported Clang | dev |
| macOS pinned image | arm64 | Apple Clang | dev |

**Current CI reality** (`.github/workflows/ci.yml`): one job, Ubuntu 22.04,
default `cc`, with `z3` installed via `apt-get` for contract verification —
no Clang leg, no macOS job at all. The macOS Apple Clang arm64 leg above is
continuously exercised only on developer machines (this doc, and the whole
`native_exe_*` test suite, was authored and verified on macOS arm64 with
Apple Clang), not in the hosted CI matrix. Closing PORT-001 in CI (rather
than relying on developer-machine evidence) means adding jobs to
`.github/workflows/ci.yml` along these lines, matching the existing
z3-install pattern:

```yaml
jobs:
  native-exe-linux-gcc:
    runs-on: ubuntu-22.04
    steps:
      - uses: actions/checkout@v4
        with: { submodules: recursive }
      - run: sudo apt-get update && sudo apt-get install -y z3
      - run: ./scripts/sv0 test-guards

  native-exe-linux-clang:
    runs-on: ubuntu-22.04
    env:
      CC: clang
    steps:
      - uses: actions/checkout@v4
        with: { submodules: recursive }
      - run: sudo apt-get update && sudo apt-get install -y z3 clang
      - run: ./scripts/sv0 test-guards

  native-exe-macos-arm64:
    runs-on: macos-14  # arm64 runner
    steps:
      - uses: actions/checkout@v4
        with: { submodules: recursive }
      - run: brew install z3
      - run: ./scripts/sv0 test-guards
```

Adding these jobs is a CI-configuration change with its own review (it
changes billed CI minutes and the supported-platform claim), so it's
recorded here as the documented requirement rather than applied directly in
this doc-only slice. macOS x86_64 is not required for R0 (spec §20.1: "MAY
be added when runner access exists"); R1 either certifies it or documents
arm64-only macOS support with rationale (spec §20.2).
