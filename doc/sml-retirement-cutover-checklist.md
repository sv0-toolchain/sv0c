# SML bootstrap retirement — human cutover checklist (M3-S-050 / M3-S-051)

This is the **stakeholder-owned** procedure for **`bootstrap-sml-final`**, **`sml-legacy/` → `sml-legacy/`**, and follow-on default-build changes. **Do not execute** until **`task/sv0-toolchain-milestone-3-self-host.Rmd`** **## completion criteria** (L0) and explicit sign-off — see **`.cursor/rules/28-sml-retirement-and-self-host-bar.mdc`**.

## Preflight (blast radius)

Before editing **`sources.cm`**, generate a reference list (paths under **`sv0c/`**, **`scripts/`**, **`.github/`**, **`task/`**, **`.cursor/`**):

```bash
./scripts/sv0 sml-retirement-preflight
# or: python3 scripts/sml_retirement_preflight_scan.py --root .
```

Re-run after **`sml-legacy/` → `sml-legacy/`** until only intentional **`sml-legacy/`** references remain.

## Prerequisites (gate)

- [ ] **M3 L0** criteria in the owning milestone task are satisfied (feature parity, VM policy, native self-host story — **not** pilot-list growth alone).
- [ ] **`./scripts/sv0 test`** green on the integration branch that will receive the cutover.
- [ ] Submodule discipline reviewed: push **`sv0c`** first, then meta-repo gitlink — **`29-submodule-git-visibility.mdc`**.
- [ ] Announce freeze window; snapshot **`sv0doc/`** / **`task/`** revision if stakeholders require audit trail.

## Documentation pointers (pre-flight)

| Topic | Location |
|-------|----------|
| Pilot loop vs retirement | **`doc/self-host-sv0-loop.md`**, **`.cursor/rules/28-sml-retirement-and-self-host-bar.mdc`** |
| Recovery after cutover | **`doc/cold-bootstrap-recovery.md`** |
| Native compiler env | **`doc/native-self-host-compiler-recipe.md`** |

## M3-S-051 — Tag `bootstrap-sml-final` on **sv0c** only

**Human-owned.** Apply only on the **`sv0c`** repository (not necessarily the meta-repo root).

1. On **`sv0c`** **`main`** (or agreed release branch), verify clean tree and green **`./scripts/sv0 test`** from **meta-repo** root against current **`sv0c`** SHA.
2. Create annotated tag (exact message is stakeholder-owned), e.g.:

```bash
cd sv0c   # standalone clone or submodule checkout
git tag -a bootstrap-sml-final -m "Final SML bootstrap layout before sml-legacy move"
git push origin bootstrap-sml-final
```

3. Record tag SHA in **`README.md`** / **`task/sv0-toolchain-milestone-3-self-host.Rmd`** refinement log if policy requires.

## M3-S-051 — Rename **`sml-legacy/`** → **`sml-legacy/`**

**Human-owned.** This breaks **`sources.cm`** until paths are updated.

1. Rename directory at **`sv0c`** root: **`sml-legacy/`** → **`sml-legacy/`** (preserve history with **`git mv`**).
2. Edit **`sources.cm`**: replace every **`sml-legacy/`** prefix with **`sml-legacy/`** (see current file — **~60** member lines).
3. Grep **`sv0c/`** and **meta-repo** for hard-coded **`sml-legacy/`** paths (`Makefile`, **`scripts/`**, docs, CI); fix or add compatibility symlinks only if stakeholders explicitly allow (prefer explicit path updates).
4. Run **`./scripts/sv0 check`** and **`./scripts/sv0 test`** from meta-repo root.
5. Bump meta-repo **`sv0c`** submodule pointer after **`sv0c`** push.

## M3-S-052 — Default build sv0-only (`SV0_SELF_HOST_COMPILER`)

**Landed:** **`make check`** / **`./scripts/sv0 check`** smoke uses **`sv0c/scripts/smoke-self-host-compiler.sh`** (**`heap`** + one-file **`sml @SMLload`** emit — standalone-safe). Default **C emit** for **`emit-c`**, stage0 goldens, and **`scripts/sv0`** stages goes through **`SV0_SELF_HOST_COMPILER`** (**`build/sv0-self-host-compiler`** wrapper — **M3-S-047** recipe) where applicable.

**Still SML-backed today:** the wrapper delegates to **`scripts/sv0-self-host-emit-c.sh`** (**`sml @SMLload=build/sv0c`**). **`make heap`**, **`bootstrap-build`**, VM **`Main.main`** paths, **`sv0vm`** loads, and **`legacy-bootstrap-check`** (**full **`CM.make "sources.cm"`**) remain on **SML/NJ** — CI installs SML and runs **`legacy-bootstrap-check`** after **`./scripts/sv0 check`**.

**Makefile:** **`check`** = **`heap`** + smoke script; **`legacy-bootstrap-check`** = full **`CM.make`**; **`legacy-bootstrap-heap`** aliases **`heap`**.

## Post-cutover validation (minimum)

- **`./scripts/sv0 test-guards`**
- **`./scripts/sv0 test`** (or stakeholder-agreed subset until CI time budget updated)

## Related

- **`task/sv0-toolchain-milestone-3-self-host.Rmd`** — **## M3 G9 slice status**
- **`README.md`** (sv0c) — architecture table still references **`sml-legacy/`** until cutover edits land
