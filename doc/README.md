# sv0c documentation

Start with the repo [`README.md`](../README.md) for the big picture (the two
implementations, the pipeline, quickstart). This index covers the deeper notes.

## understand the compiler

| doc | what it covers |
|---|---|
| [`compiler-passes.md`](compiler-passes.md) | pass-by-pass walkthrough — lexer → parser → resolver → checker → contracts → lowering → codegen, including structs/enums/`match` |
| [`driver-pipeline-composition.md`](driver-pipeline-composition.md) | how the single-file driver and the multi-module (`--project`) path compose the passes |
| [`bootstrap-compiler-workarounds.md`](bootstrap-compiler-workarounds.md) | sv0-language gotchas the compiler sources work around (reserved words, missing `Vec` ops, etc.) — read before editing `lib/*.sv0` |

## build, self-host, and operate

| doc | what it covers |
|---|---|
| [`self-host-sv0-loop.md`](self-host-sv0-loop.md) | the sv0→sv0 self-hosting loop and its behavioral-parity acceptance |
| [`native-self-host-compiler-recipe.md`](native-self-host-compiler-recipe.md) | building the native mega-TU compiler from `lib/*.sv0` |
| [`cold-bootstrap-recovery.md`](cold-bootstrap-recovery.md) | recovering the toolchain if self-host breaks |
| [`cli-parity.md`](cli-parity.md) | CLI flag/behavior parity between the native compiler and the SML reference |
| [`tooling-formatter-linter.md`](tooling-formatter-linter.md) | the sv0 formatter + linter used by the pre-commit hooks |
| [`link-g6-blockers.md`](link-g6-blockers.md) | multi-module link contract (referenced by the M3 G6 CI guard) |
| [`sml-retirement-cutover-checklist.md`](sml-retirement-cutover-checklist.md) | the (human-gated, still-pending) SML retirement cutover procedure |

## the language specification

Grammar, type rules, contracts, memory model, and keywords live in
[`sv0doc/`](../../sv0doc/) — the normative source of truth the compiler
implements against.

## archive — historical M3 working notes

[`archive/`](archive/) holds the working notes from milestone 3 (self-hosting)
and the post-M3 hardening. They are **preserved for history and completeness**;
they are not needed to understand or use the current compiler, and some
`lib/*.sv0` source comments still point at their pre-archive paths.

| doc | era / topic |
|---|---|
| [`archive/bug-hunt-findings.md`](archive/bug-hunt-findings.md) | the 13-finding correctness audit (12 fixed, 1 deferred) — closed |
| [`archive/l0-closure-roadmap.md`](archive/l0-closure-roadmap.md) | the phased L0 engineering path (Phase C = post-M3 hardening, now complete) |
| [`archive/post-m3-phase-c-plan.md`](archive/post-m3-phase-c-plan.md) | post-M3 whole-language-parity plan (all slices done) |
| [`archive/m3-closure-evidence.md`](archive/m3-closure-evidence.md) | milestone-3 completion evidence map |
| [`archive/megatu-corpus-roadmap.md`](archive/megatu-corpus-roadmap.md) | mega-TU native-compose corpus roadmap |
| [`archive/pc3b-linkprojectdir-scoping.md`](archive/pc3b-linkprojectdir-scoping.md) | the `linkProjectDir` / `--project` scoping deep-dive (§5a–§5j) |
| [`archive/pc3b1-arena-reloc-semantics.md`](archive/pc3b1-arena-reloc-semantics.md) | arena relocation semantics for the merge path |
| [`archive/native-compose-tradeoffs.md`](archive/native-compose-tradeoffs.md) | why native emit is behaviorally (not byte) equal to SML |
| [`archive/bh8-native-diagnostics-plan.md`](archive/bh8-native-diagnostics-plan.md) | plan for the native checker diagnostics (BH-8) |
| [`archive/transliteration-plan.md`](archive/transliteration-plan.md) | the SML→sv0 transliteration plan |
| [`archive/transliteration-include.md`](archive/transliteration-include.md) | `include` transliteration note |
| [`archive/m3-g1-track-c.md`](archive/m3-g1-track-c.md) | M3 G1 track-C slice notes |
| [`archive/lib-LAYOUT.md`](archive/lib-LAYOUT.md) | the transliteration-era `lib/` directory + seed layout plan |
