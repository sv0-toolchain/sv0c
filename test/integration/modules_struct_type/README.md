# modules_struct_type — imported-struct local type (PC-1 fixture)

**Epic 1 / PC-1** (post-M3 Phase C; `sv0c/doc/post-m3-phase-c-plan.md`,
`task/sv0-toolchain-milestone-3-checklist.Rmd` § Post-M3 Phase C slice backlog).

**What it exercises.** Module `lib` defines `struct Point { x, y }` + `origin()`.
The importing module declares a local with a **use-imported struct type**:
`let p: Point = origin(); p.x + p.y`.

**Expected:** exit **42** (`3 + 4 + 35`).

**Status: FIXED (2026-08-05, sml-legacy) — exit 42, wired into the pass harness.**
Originally: SML `--project` type-checked (exit 0) but codegen emitted `int p =
_sv0t0;` (should be `lib__Point p`) and `p.x` on an `int` → **cc error**. The
local's declared type lost the cross-module alias. Closed by:
- `resolver.sml` — record the bare struct type alias (`Point → lib__Point`).
- `lowering.sml` `astTyToCString` — canonicalize the `TyName` via `canonTyName`.

sv0 transliteration of the same fix is PC-2e (gated on Epic 3).

**Reproduce (pre-fix):**
```
cd sv0c && sml @SMLload=build/sv0c --project test/integration/modules_struct_type
```
