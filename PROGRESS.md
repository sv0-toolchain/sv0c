# sv0c — progress (submodule)

**Meta-repo rollup:** when this tree is the `sv0c/` submodule of **sv0-toolchain**, the parent copies this file’s **`%`** into `task/sv0-toolchain-progress.md`. **Standalone clone:** keep this file authoritative here; reconcile on the next meta-repo integration.

**Last updated:** 2026-04-09 (America/Denver)

## Checklist (local source of truth)

| ID | Item | Done (0/1) |
|----|------|------------|
| C-1 | `lib/bootstrap-sources.list` + `lib/golden/stage0/` discipline documented and CI-enforced (`./scripts/sv0 test`) | 1 |
| C-2 | Self-host pilot list `lib/self-host-sv0-loop.list` tracked; loop recipe in `doc/self-host-sv0-loop.md` | 1 |
| C-3 | VM parity v1 manifest + goldens (`test/vm-parity/`) aligned with milestone allowlist policy | 1 |
| C-4 | Transliteration plan current (`doc/transliteration-plan.md`, `lib/LAYOUT.md`) | 1 |
| C-5 | Native semantic self-host (`SV0_SELF_HOST_COMPILER`) path documented vs pilot `emit-c` harness | 1 |
| C-6 | SML retirement criteria (`bootstrap-sml-final`, `sml-legacy/`) **not** claimed until `task/sv0-toolchain-milestone-3-self-host.Rmd` completion criteria | 1 |

**Optional metrics** (not in `%` unless promoted to checklist rows): non-comment lines in `lib/bootstrap-sources.list`, count of `self-host-sv0-loop.list` entries, vm-parity manifest size — copy into the meta **run log** when they change.

## Completion

- **Done:** count rows with `Done = 1` above.
- **Total:** row count of the checklist.
- **%:** `Done / Total * 100`.

## Notes

Point to the active vertical slice (SML module, harness name, or milestone sub-step). Link `task/sv0-toolchain-milestone-3-checklist.Rmd` for row-level transliteration inventory.

**Checklist closure notes:** **C-1** — `lib/LAYOUT.md` + `task/sv0-toolchain-milestone-3-self-host.Rmd` document the lists and stage0 goldens; parent **`./scripts/sv0 test`** runs **`bootstrap-build`**, stage0 **`diff`**, and **`verify_vm_parity_manifest_bootstrap`** (subset guard). **C-2** — `lib/self-host-sv0-loop.list` + `doc/self-host-sv0-loop.md` are the tracked pilot surface; **`./scripts/sv0 test`** runs **`self-host-sv0-loop`**. **C-3** — `test/vm-parity/manifest.txt` is the curated allowlist (**101** programs); `test/vm-parity/golden/sml/*.sv0b` are SML reference bytecode; **`./scripts/sv0 test`** **`cmp`**s post-**`bootstrap-build`** emit vs goldens; **`./scripts/sv0 test-guards`** / **`verify_vm_parity_manifest_bootstrap.py`** enforce **`manifest.txt` ⊆ `lib/bootstrap-sources.list`**; policy and growth expectations are spelled out in **`test/vm-parity/README.md`** and **`task/sv0-toolchain-milestone-3-self-host.Rmd`**. **C-4** — `doc/transliteration-plan.md` and `lib/LAYOUT.md` are the **paired** living map; both now document a **maintainer contract** (update together on seed churn), a **metrics snapshot** (~138 bootstrap lines, **55** pilots, **65** stage0 golden files, **101** vm-parity paths), and pointers to **`task/sv0-toolchain-milestone-3-checklist.Rmd`** for row-level inventory; no new CI gate — correctness is editorial discipline plus existing **`./scripts/sv0 test`** on list changes. **C-5** — `doc/self-host-sv0-loop.md` documents **pilot** third leg (**`sv0-self-host-emit-c.sh`** = SML stand-in) vs **native** third leg (**`SV0_SELF_HOST_COMPILER`**, same stdout contract); **`scripts/sv0-self-host-emit-c.sh`** header comments the contract; **`SV0_SKIP_SELF_HOST_COMPILER_DIFF=1`** documented for skipping the **`diff`**; **semantic** loop closure remains **`task/sv0-toolchain-milestone-3-self-host.Rmd`** bar — this checklist item is **documentation**, not claiming native self-host is done. **C-6** — **`bootstrap-sml-final`** / **`sml/`** → **`sml-legacy/`** are **not** claimed in this ledger; `doc/self-host-sv0-loop.md` **§ SML retirement guardrail** + **`task/sv0-toolchain-milestone-3-self-host.Rmd`** + **`.cursor/rules/28-sml-retirement-and-self-host-bar.mdc`** + **`./scripts/sv0 milestone-orient show M3`** make the bar explicit; **C-6 done = policy documented**, not SML retired.

**2026-04-09:** added **`lib/lower_req_instrs_core.sv0`**; pilots **47**. **2026-04-10:** **`lib/lower_ens_glue_inject_core.sv0`**; pilots **48**. **2026-04-11:** **`lib/lower_store_value_to_slot_core.sv0`** (**`storeValueToSlot`**); pilots **49**; checklist **2/6** on transliteration seeds. **2026-04-09 (later):** **`lib/lower_return_body_core.sv0`** (**`lowerReturn`** / **`lower_body_block_tail_len`**); pilots **50**, stage0 goldens **60**, bootstrap-listed lines **133** (see **`doc/transliteration-plan.md`** snapshot). **`lib/lower_fn_prefix_core.sv0`** (**`lowerFn`** **`decl` @ `oldInstrs` @ `reqInstrs`** prefix); pilots **51**, stage0 **61**, bootstrap **134**. **`lib/lower_body_dispatch_core.sv0`** (**`lowerBody`** / **`lowerBlock`**); pilots **52**, stage0 **62**, bootstrap **135**. **`lib/lower_stmt_dispatch_core.sv0`** (**`lowerStmt`** return vs **`lowerExprForEffect`**); pilots **53**, stage0 **63**, bootstrap **136**. **`lib/lower_stmt_let_init_core.sv0`** (**`StmtLet`** **`PatBind`** init arms); pilots **54**, stage0 **64**, bootstrap **137**. **`lib/lower_expr_wi_tv_cluster_core.sv0`** (**`lowerExprWithInstrs`** / **`lowerExprToValue`** tag cluster); pilots **55**, stage0 **65**, bootstrap **138**. **C-3 (vm-parity v1):** manifest + SML goldens + **`manifest ⊆ bootstrap`** guard were already CI-enforced; closure recorded in this **`PROGRESS.md`** revision — checklist **3/6**. **C-4 (transliteration map):** paired **`doc/transliteration-plan.md`** + **`lib/LAYOUT.md`** maintenance contract and snapshot — checklist **4/6**. **C-5 (native vs pilot self-host):** explicit table in **`doc/self-host-sv0-loop.md`** — checklist **5/6**. **C-6 (SML retirement guardrail):** non-claim policy + links in **`doc/self-host-sv0-loop.md`** — checklist **6/6** (still **not** asserting **`bootstrap-sml-final`** or **`sml-legacy/`**).
