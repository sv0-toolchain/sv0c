# sv0c — progress (submodule)

**Meta-repo rollup:** when this tree is the `sv0c/` submodule of **sv0-toolchain**, the parent copies this file’s **`%`** into `task/sv0-toolchain-progress.md`. **Standalone clone:** keep this file authoritative here; reconcile on the next meta-repo integration.

**Last updated:** 2026-04-09

## Checklist (local source of truth)

| ID | Item | Done (0/1) |
|----|------|------------|
| C-1 | `lib/bootstrap-sources.list` + `lib/golden/stage0/` discipline documented and CI-enforced (`./scripts/sv0 test`) | 1 |
| C-2 | Self-host pilot list `lib/self-host-sv0-loop.list` tracked; loop recipe in `doc/self-host-sv0-loop.md` | 1 |
| C-3 | VM parity v1 manifest + goldens (`test/vm-parity/`) aligned with milestone allowlist policy | 1 |
| C-4 | Transliteration plan current (`doc/transliteration-plan.md`, `lib/LAYOUT.md`) | 0 |
| C-5 | Native semantic self-host (`SV0_SELF_HOST_COMPILER`) path documented vs pilot `emit-c` harness | 0 |
| C-6 | SML retirement criteria (`bootstrap-sml-final`, `sml-legacy/`) **not** claimed until `task/sv0-toolchain-milestone-3-self-host.Rmd` completion criteria | 0 |

**Optional metrics** (not in `%` unless promoted to checklist rows): non-comment lines in `lib/bootstrap-sources.list`, count of `self-host-sv0-loop.list` entries, vm-parity manifest size — copy into the meta **run log** when they change.

## Completion

- **Done:** count rows with `Done = 1` above.
- **Total:** row count of the checklist.
- **%:** `Done / Total * 100`.

## Notes

Point to the active vertical slice (SML module, harness name, or milestone sub-step). Link `task/sv0-toolchain-milestone-3-checklist.Rmd` for row-level transliteration inventory.

**Checklist closure notes:** **C-1** — `lib/LAYOUT.md` + `task/sv0-toolchain-milestone-3-self-host.Rmd` document the lists and stage0 goldens; parent **`./scripts/sv0 test`** runs **`bootstrap-build`**, stage0 **`diff`**, and **`verify_vm_parity_manifest_bootstrap`** (subset guard). **C-2** — `lib/self-host-sv0-loop.list` + `doc/self-host-sv0-loop.md` are the tracked pilot surface; **`./scripts/sv0 test`** runs **`self-host-sv0-loop`**. **C-3** — `test/vm-parity/manifest.txt` is the curated allowlist (**101** programs); `test/vm-parity/golden/sml/*.sv0b` are SML reference bytecode; **`./scripts/sv0 test`** **`cmp`**s post-**`bootstrap-build`** emit vs goldens; **`./scripts/sv0 test-guards`** / **`verify_vm_parity_manifest_bootstrap.py`** enforce **`manifest.txt` ⊆ `lib/bootstrap-sources.list`**; policy and growth expectations are spelled out in **`test/vm-parity/README.md`** and **`task/sv0-toolchain-milestone-3-self-host.Rmd`**.

**2026-04-09:** added **`lib/lower_req_instrs_core.sv0`**; pilots **47**. **2026-04-10:** **`lib/lower_ens_glue_inject_core.sv0`**; pilots **48**. **2026-04-11:** **`lib/lower_store_value_to_slot_core.sv0`** (**`storeValueToSlot`**); pilots **49**; checklist **2/6** on transliteration seeds. **C-3 (vm-parity v1):** manifest + SML goldens + **`manifest ⊆ bootstrap`** guard were already CI-enforced; closure recorded in this **`PROGRESS.md`** revision — checklist **3/6**.
