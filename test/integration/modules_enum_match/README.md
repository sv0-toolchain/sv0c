# modules_enum_match — cross-module enum `match` (PC-2a failing fixture)

**Epic 2 / PC-2a** (post-M3 Phase C; `sv0c/doc/post-m3-phase-c-plan.md`,
`task/sv0-toolchain-milestone-3-checklist.Rmd` § Post-M3 Phase C slice backlog).

**What it exercises.** Module `lib` defines `enum Signal { On(i32), Off() }` and
a `make() -> Signal` constructor helper. The importing module (`main.sv0`)
obtains a `Signal` via `make()` (no local type annotation — deliberately isolates
this from the imported-struct-**local-type** gap, Gap 1 / Epic 1) and `match`es
on it with cross-module variant paths `Signal::On(v)` / `Signal::Off()`.

**Expected (target):** exit **42** (`7 + 35`).

**Current (gap, verified 2026-08-05):** `sv0c error: E0400: type mismatch` via
`--project`. The identical code **within a single module compiles** (control:
`modules_types` `color_code()` and a one-file repro both emit C) — so the gap is
**specifically cross-module**.

**Reproduce:**
```
cd sv0c && echo 'CM.make "sources.cm"; Main.main ((), ["--project", "test/integration/modules_enum_match"]);' | sml
```

**Likely fix loci (for PC-2b / PC-2c / PC-2d):**
- `linkProjectDir` mangles the enum **type** to `lib__Signal`, but the match-arm
  variant path in the importer stays `Signal::On` — the mangle/alias for
  cross-module **variant constructor paths** is the resolver gap (PC-2b;
  `lib/resolver.sv0` ~L788, enum variant-ctor alias plumbing).
- Checker variant env registers variants under `pathKey [enumName, variantName]`
  (`sml-legacy/type_checker/checker.sml` ~L1050/1072); the match-arm unify that
  raises E0400 is ~L777. The cross-module scrutinee type (`lib__Signal`) must
  resolve to the same variant-env entry the pattern path looks up (PC-2d;
  `lib/checker.sv0`).
- Lowering: `match_scrut_cty` must resolve the scrutinee to the precise (mangled)
  enum typedef via the threaded fn/param type table (PC-2c; `lib/lowering.sv0`).

**Wiring:** NOT yet in the pass harness (`task/sv0c-milestone-1/02-integration-test.sh`
`run_case … 42`) — it currently fails to compile. Wire it in as `run_case
"modules_enum_match (cross-module enum match)" project "$IT/modules_enum_match" 42`
when PC-2d lands and it compiles+runs.
