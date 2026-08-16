# M3 G1 (Track C) — mechanical `raise` audit log

Companion to the meta-repo task **`task/sv0-toolchain-milestone-3-self-host.Rmd`**, section **M3 G1 slice status (Track C)**.

Each **M3-S-00x** subsection records a scoped search for the **`raise`** keyword on the owning module paths listed in the milestone backlog. The regex class matches **`scripts/verify_compiler_sv0_no_raise.py`** / **`scripts/verify_m3_g1_track_c.py`**. **Mechanical closure** (no `raise` in these paths) is **necessary** for G1 but **not sufficient** for full **`Result<CompileError>`** API threading — see **`sv0doc/compiler/bootstrap-diagnostics.md`**.

## M3-S-007 — checker batch A

- **Paths:** `lib/checker.sv0`
- **Check:** `rg '\braise(\s|\()' lib/checker.sv0` from **`sv0c/`** root
- **Outcome:** no matches (`rg` exit **1** = zero hits), 2026-04-27

## M3-S-008 — checker batch B

- **Paths:** `lib/checker.sv0` (second tranche / remainder — same file as M3-S-007)
- **Check:** `rg '\braise(\s|\()' lib/checker.sv0` from **`sv0c/`** root
- **Outcome:** no matches (`rg` exit **1**), 2026-04-27

## M3-S-009 — lowering

- **Paths:** `lib/lowering.sv0`
- **Check:** `rg '\braise(\s|\()' lib/lowering.sv0` from **`sv0c/`** root
- **Outcome:** no matches (`rg` exit **1**), 2026-04-27

## M3-S-010 — resolver

- **Paths:** `lib/resolver.sv0`
- **Check:** `rg '\braise(\s|\()' lib/resolver.sv0` from **`sv0c/`** root
- **Outcome:** no matches (`rg` exit **1**), 2026-04-27

## M3-S-011 — VM codegen

- **Paths:** `lib/vm_codegen.sv0`
- **Check:** `rg '\braise(\s|\()' lib/vm_codegen.sv0` from **`sv0c/`** root
- **Outcome:** no matches (`rg` exit **1**), 2026-04-27

## M3-S-012 — C codegen / bytecode / env

- **Paths:** `lib/codegen.sv0`, `lib/bytecode.sv0`, `lib/env.sv0`
- **Check:** `rg '\braise(\s|\()' lib/codegen.sv0 lib/bytecode.sv0 lib/env.sv0` from **`sv0c/`** root
- **Outcome:** no matches (`rg` exit **1**), 2026-04-27

## M3-S-013 — link / lexer / include_expand / unify / diagnostic

- **Paths:** `lib/link.sv0`, `lib/lexer.sv0`, `lib/include_expand.sv0`, `lib/unify.sv0`, `lib/diagnostic.sv0`
- **Check:** `rg '\braise(\s|\()' lib/link.sv0 lib/lexer.sv0 lib/include_expand.sv0 lib/unify.sv0 lib/diagnostic.sv0` from **`sv0c/`** root
- **Outcome:** no matches (`rg` exit **1**), 2026-04-27

## M3-S-014 — Track C closure (full compiler `.sv0` scan)

- **Paths:** all **`*.sv0`** under **`lib/`**, **`lexer/`**, and **`parser/`** (same roots as **`scripts/verify_compiler_sv0_no_raise.py`** in the meta-repo).
- **Check:** **`python3 scripts/verify_compiler_sv0_no_raise.py --root <meta>`** and **`python3 scripts/verify_m3_g1_track_c.py --root <meta>`** with **`scripts/m3_g1_active_slices.txt`** listing **M3-S-004** … **M3-S-014**.
- **Outcome:** both scripts **exit 0** on **2026-04-27** (mechanical closure — no **`raise`** token in those trees).
