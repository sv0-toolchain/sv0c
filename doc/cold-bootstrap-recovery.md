# Cold bootstrap recovery (M3-S-053)

Use this when the **sv0-only** default toolchain fails (buggy native compiler, broken self-host loop, or corrupted workspace) **after** SML has been moved to **`sml-legacy/`** and/or the **`bootstrap-sml-final`** tag exists.

## Goals

1. Restore a **known-good SML bootstrap** compiler tree.
2. Rebuild the **heap image** (`build/sv0c`) or use documented CM build.
3. Return to normal **`./scripts/sv0 emit-c`** / **`self-host-sv0-loop`** workflows.

## Recovery path A — checkout tagged **`bootstrap-sml-final`** (**sv0c**)

When **`sv0c`** has tag **`bootstrap-sml-final`** pointing at the last **pre-retirement** or **post-tag-pre-move** layout (stakeholder-defined):

```bash
cd sv0c
git fetch origin
git checkout bootstrap-sml-final
# Or: git checkout -b recover/bootstrap tags/bootstrap-sml-final
```

Then follow **`README.md`** **`make heap`** and meta-repo **`./scripts/sv0 check`**.

If that tag reflects **`sml-legacy/`** (not yet renamed), **`sources.cm`** matches **`sml-legacy/...`** — no **`sml-legacy/`** edits needed.

## Recovery path B — **`sml-legacy/`** tree on **main**

If **`main`** already renamed **`sml-legacy/`** → **`sml-legacy/`** and **`sources.cm`** was updated:

```bash
cd sv0c
git checkout main   # or stable branch
make clean
make heap
```

Invoke compiler via **`sml @SMLload=build/sv0c`** as in **`README.md`**.

## Recovery path C — submodule/meta-repo out of sync

From **sv0-toolchain** root:

```bash
git submodule update --init --recursive
cd sv0c && git status && make heap
```

If **`sv0c`** is detached or behind, align submodule gitlink to a known-good SHA from **`README.md`** pinned row or stakeholder release notes.

## Verification

- **`./scripts/sv0 check`** (meta-repo root)
- Single-file probe: **`./scripts/sv0 emit-c lib/span.sv0`** | head (expect **`#include`**)

## Related

- **`doc/archive/sml-retirement-cutover-checklist.md`** — forward cutover order
- **`doc/self-host-sv0-loop.md`** — pilot vs native semantics
