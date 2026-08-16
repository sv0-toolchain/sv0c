# sv0c — progress

**Live status is tracked in one place:** the parent workspace's
[`task/sv0-toolchain-progress.md`](../task/sv0-toolchain-progress.md) (run log +
per-milestone rollup). When this tree is the `sv0c/` submodule of
**sv0-toolchain**, that file is authoritative; consult it rather than this stub.

## current state (2026-08-16)

- **Self-hosting compiler complete.** The native sv0-built compiler
  (`build/sv0-megatu-compiler-native`, from `lib/*.sv0`) is the **default**
  `SV0_SELF_HOST_COMPILER`; `sml-legacy/` is the retired reference. Milestone 3
  is closed and post-M3 hardening is complete (see the parent progress rollup and
  `doc/archive/` for the milestone-3 working notes).
- **Where to read next:** [`README.md`](README.md) (architecture + quickstart),
  [`doc/README.md`](doc/README.md) (documentation index),
  [`doc/compiler-passes.md`](doc/compiler-passes.md) (pass-by-pass walkthrough).

_Historical note: earlier revisions of this file carried a detailed run log and a
milestone-3 transliteration checklist (`C-1…C-6`). That history lives in git and
in `doc/archive/`; it is not reproduced here to keep a single source of truth._
