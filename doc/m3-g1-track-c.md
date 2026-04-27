# M3 G1 (Track C) — mechanical `raise` audit log

Companion to the meta-repo task **`task/sv0-toolchain-milestone-3-self-host.Rmd`**, section **M3 G1 slice status (Track C)**.

Each **M3-S-00x** subsection records a scoped search for the **`raise`** keyword on the owning module paths listed in the milestone backlog. The regex class matches **`scripts/verify_compiler_sv0_no_raise.py`** / **`scripts/verify_m3_g1_track_c.py`**. **Mechanical closure** (no `raise` in these paths) is **necessary** for G1 but **not sufficient** for full **`Result<CompileError>`** API threading — see **`sv0doc/compiler/bootstrap-diagnostics.md`**.

## M3-S-007 — checker batch A

- **Paths:** `lib/checker.sv0`
- **Check:** `rg '\braise(\s|\()' lib/checker.sv0` from **`sv0c/`** root
- **Outcome:** no matches (`rg` exit **1** = zero hits), 2026-04-27
