# sv0c native executable: stable surface (R1, NEX-057)

Implements DOC-005
(`~/Documents/project-specs/sv0c-runtime-executable/SPEC.md`): "runtime
ABI, build-record schema, profiles, and compatibility policy SHALL be
published." This page indexes and links the material each of those four
areas already has, rather than duplicating it — every claim below is
checked against a real, already-landed script or test, matching
[`native-executable.md`](native-executable.md)'s (NEX-038) own
verified-example discipline.

For the engine's day-to-day usage (build examples, contract modes, host
matrix), see [`native-executable.md`](native-executable.md); this page
covers the R1 stable-release surface specifically: what's versioned, what
a build record contains, what the two profiles are, and how compatibility
is governed going forward.

## Runtime and entry ABI

Two independent, versioned ABI surfaces ship with every native build:

- **Runtime bundle ABI** (`native_exe_runtime_manifest.ABI_VERSION`,
  currently `1`): the `sv0_runtime.h`/`.c` header/source hashes an
  installed compiler checks against at build time
  (`native_exe_runtime_manifest.verify_manifest`).
- **Entry ABI** (`native_exe_entry_abi.ENTRY_ABI_VERSION`, currently `1`):
  the reserved-symbol set and hosted `main`-adapter shapes. Full detail in
  [`entry-abi.md`](entry-abi.md) (NEX-054).

Both are snapshotted in the installed runtime bundle
(`sv0c/runtime/runtime-manifest.json`, `sv0c/runtime/entry-abi-manifest.json`)
and verified on every build (`verify_manifest`, `verify_entry_abi_compat`)
— an installed runtime at a version this compiler doesn't support fails
the build closed, never silently.

## Build-record schema

Every build can produce a build-record JSON document
(`scripts/native_exe_build_record.py`, NEX-042, Appendix C of the spec):
artifact identity (kind/path/sha256/size, computed from the actual
published bytes — never trusted from a caller), input source hashes,
`sv0c`/runtime/host/compiler identity, profile, both contract modes, a
`hermetic` flag, and (NEX-053b) a `reproducibility` classification
(`"byte-identical"` / `"semantic-only"` / `"divergent"`, from
`native_exe_repro_harness.py`) recorded verbatim, never recomputed by the
record itself.

## Profiles

Two build profiles exist, both in `scripts/native_exe_argv_builder.py`:

| Profile | Optimization | Extra flags | Notes |
|---|---|---|---|
| `dev` (default) | `-O0` | `-g` | Byte-identical to R0's original argv shape. |
| `release` | `-O2` | `-fno-strict-aliasing`, `-g` | Never `-O3`, LTO, `-ffast-math`, or `NDEBUG`-based contract stripping (§16.5). |

`-fno-strict-aliasing` is not a generic safety margin — it's the specific,
audited mitigation for two real strict-aliasing violations found in
[`native-executable-ub-audit.md`](native-executable-ub-audit.md) (NEX-048a,
Sites 1/2: the box-pool pointer-cast deref, and a cross-reinterpretation
inside the bootstrap compiler's own generated C). Behavioral parity
between the two profiles is proven on the full 114-fixture behavior corpus
by `native_exe_release_parity.py` (NEX-051b) — the one documented,
expected exception is `overflow_wrap_mask.sv0`, which deliberately
exercises real signed-overflow UB whose exact manifestation is
optimization-level-dependent by definition.

`release` is wired into `build_native_executable` via a `profile`
parameter (NEX-051c); an unrecognized profile value is a hard usage error,
never a silent fallback to `dev` (§11.4).

## Sanitizer and warning gates

Every build in the full behavior corpus passes cleanly under `-Wall
-Wextra` (`native_exe_warning_policy.py`/`native_exe_warning_report.py`,
NEX-049) and under `-fsanitize=address,undefined`
(`native_exe_sanitizer_corpus.py`, NEX-050c) — both wired into CI. Known,
narrow exceptions (a handful of stylistic warnings, one fixture
deliberately exercising documented UB) are recorded with rationale in
those modules' own source, not silently suppressed wholesale.

## Reproducibility

Byte-reproducibility is verified where the platform supports it, and
honestly classified as semantic-only where it doesn't
(`native_exe_repro_harness.py`, NEX-053a) — on macOS/`ld64`, the linker's
`LC_UUID` load command prevents true byte identity, confirmed by direct
observation rather than assumed.

## Performance

A benchmark baseline (`sv0c/doc/perf-baseline.json`, NEX-055a) and a 20%
release-to-release regression budget (`native_exe_perf_regression.py`)
gate performance; concurrent distinct-output builds achieve real
wall-clock parallelism, measured directly rather than assumed
(`native_exe_concurrent_perf.py`, NEX-055b — 1.7×–6.6× speedup observed
for 2–4 concurrent builds).

## Release-candidate evidence

A signed release candidate carries one evidence bundle (revision, clean
working tree, build record, benchmark report, reproducibility
classification — `native_exe_release_evidence.py`, NEX-056a) and follows a
documented signing workflow (never a fabricated signature —
[`release-signing-workflow.md`](release-signing-workflow.md), NEX-056b).

## Compatibility policy (GOV-009)

Public CLI surface, runtime ABI, entry ABI, and build-record schema
changes all follow versioned change control:

- A runtime-ABI or entry-ABI-breaking change requires bumping the
  relevant version constant, regenerating its manifest snapshot, and
  documenting the change (this page + the relevant per-area doc).
- A build-record schema change requires bumping
  `native_exe_build_record.SCHEMA_VERSION` and updating this page's
  "Build-record schema" section.
- Public CLI surface changes (once `sv0c --emit=exe` itself ships) follow
  ordinary release review — this project has no separate CLI-versioning
  scheme beyond the toolchain's own release process.
- None of these are silent: every version-gated check in this codebase
  fails closed on an unrecognized/unsupported version rather than
  guessing compatibility.
