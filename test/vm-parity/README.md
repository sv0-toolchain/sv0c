# VM parity suite (milestone 3, v1)

**Goal:** **strict** bytecode parity between the **SML** bootstrap compiler and the future **sv0** compiler on a **curated corpus** (see `task/sv0-toolchain-milestone-3-self-host.Rmd`).

## v1 scope

- **`manifest.txt`** lists **`.sv0`** sources (paths relative to **`sv0c/`**), one per line. Lines starting with **`#`** are comments.
- **Corpus size (current):** **97** programs. Each entry has a checked-in **`golden/sml/<stem>.sv0b`**. After **`bootstrap-build`**, **`./scripts/sv0 test`** **`cmp`**s fresh SML **`--target=vm`** output (**`build/vm/<stem>.sv0b`**) against that golden for every manifest line — mismatch fails CI (same discipline as **stage0** C goldens).
- **Allowlist policy (milestone-aligned):** the manifest is the **curated M3 parity allowlist** (see **`task/sv0-toolchain-milestone-3-self-host.Rmd`** for the ~50–100 bar and growth rules). Every manifest path must also appear in **`lib/bootstrap-sources.list`** so **`bootstrap-build`** emits bytecode before the **`cmp`**. The meta-repo enforces **`manifest.txt` ⊆ `bootstrap-sources.list`** via **`scripts/verify_vm_parity_manifest_bootstrap.py`** (runs in **`./scripts/sv0 test-guards`** and at the start of **`./scripts/sv0 test`**).
- **Coverage shape:** **lib/** and **lexer/** / **parser/** seeds, **VM-supported** integration tests (**`hello`**, **`contracts`**, **`no_alias_requires`**, **`patterns`**, **`structs`**, **`field_assign`**, **`generics`**, **`call_arg_order`**, **`continue_loops`**, **`println_ok`**, golden passes such as **`forall_requires`** / **`exists_requires`** where the VM backend matches SML), plus **`test/vm-parity/programs/*.sv0`** arithmetic / control-flow smokes.
- **`no_alias` / `VAddrOf`:** VM encodes **`&param`** as the parameter’s **local base slot** (**`i32`**) and **`sv0_no_alias`** as **`CALL_BUILTIN` 1** (same pointer-inequality idea as C **`sv0_no_alias`**).
- Programs that still need VM work stay off **`bootstrap-sources.list`** and off **`manifest.txt`** until covered.

## Golden bytecode (SML reference)

- Checked-in files: **`golden/sml/<stem>.sv0b`** where **`<stem>`** is the basename of the source without **`.sv0`** (matches **`build/vm/<stem>.sv0b`** from **`--target=vm`**).
- **Regenerate** after intentional VM-backend or lowering changes:

```bash
# from sv0-toolchain repo root (requires SML/NJ)
./scripts/capture_vm_parity_goldens.sh
```

Then commit updated **`golden/sml/*.sv0b`** in **sv0c** together with any compiler change.

## CI

Parent **`./scripts/sv0 test`** runs **`bootstrap-build`** (SML → **`.sv0b`** + VM run), then **`cmp`**s each **`build/vm/<stem>.sv0b`** against **`test/vm-parity/golden/sml/<stem>.sv0b`** for every entry in **`manifest.txt`**. Mismatch fails the build (same pattern as **stage0** C goldens).

When the **sv0** compiler emits VM bytecode, add a second tier (e.g. **`golden/sv0/`** or a dedicated compare step) without dropping the SML reference tier.

## Tier 2 (M3-S-045) — policy subset + optional emitter compare

- **`tier2-manifest.txt`** lists a **subset** of **`manifest.txt`** paths. **`scripts/verify_vm_parity_tier2_policy.py`** ( **`./scripts/sv0 test-guards`** ) asserts each path is in **`manifest.txt`** and that **`golden/sml/<stem>.sv0b`** exists for each stem.
- **`SV0_VM_BYTECODE_EMITTER`** — optional. When set to an **executable** path, **`./scripts/sv0 test`** (after tier-1 parity) invokes it once per **`tier2-manifest.txt`** line with a single argument: the **`.sv0`** path **relative to `sv0c/`** (e.g. **`lib/span.sv0`**). The emitter must write **`sv0c/build/vm/<stem>.sv0b`**. The harness **`cmp`**s that file to **`test/vm-parity/golden/sml/<stem>.sv0b`**. **`SV0TOOLCHAIN_ROOT`** and **`SV0C_ROOT`** are exported for wrappers.
- The **native** mega-TU VM emitter (**`scripts/build-sv0-megatu-vm-native.sh`** → **`build/sv0-megatu-vm-native`**) implements the same emit contract and is **byte-identical** to the SML **`--target=vm`** golden for **all 18 mega-TU compiler modules** as of **P4/D2** (driver closed via `prepool_loop_seq`). **`scripts/sv0-vm-tier2-native-emitter.sh`** is the **`SV0_VM_BYTECODE_EMITTER`** wrapper for it (it builds the native emitter on demand, then re-emits one manifest path into **`build/vm/<stem>.sv0b`**). The former SML-heap surrogate (`sv0-vm-tier2-emit-bootstrap.sh`) was a tautological SML-vs-SML check and is **retired** — the native emitter is the real gate.
- **`./scripts/sv0 vm-parity-tier2-emit`** runs the byte-parity leg standalone: it **defaults** **`SV0_VM_BYTECODE_EMITTER`** to the native wrapper (no SML-heap fallback) and **`cmp`**s each manifest path to its golden. Grow **`tier2-manifest.txt`** toward full **`manifest.txt`** as more programs reach byte-parity.
- Workflow **`.github/workflows/vm-parity-tier2.yml`** (push/PR/dispatch): job **`tier2-policy`** runs the policy script (no SML); job **`tier2-byte-parity`** builds the native emitter and runs **`sv0 vm-parity-tier2-emit`** as a **byte-parity gate**.

## Tier 3 (VMF-###) — behavioral parity for f64 / wide-int

**Why a third tier:** tiers 1–2 are **byte-identical vs an SML `--target=vm` golden**.
The SML VM backend has **no float support at all** (`sml-legacy/backend/vm/vm_codegen.sml`
raises on `VFloat`) and emits `ADD_I32` unconditionally (no `ADD_I64`). So there is
**no SML reference** to byte-diff `f64` or wide-int (`i64`/`u64`) programs against, and
the native emitter emitting `ADD_I64` / `*_F64` would *by design* diverge from the
frozen goldens. The `sv0c-vm-float-parity` task (slices `VMF-###`) adds that surface
to the **native** VM emitter + the `sv0vm` interpreter.

**Acceptance oracle (behavioral, not byte):** for each fixture, run it **twice** —

1. **C backend:** native `emit-c` → `cc -std=c99` → execute; capture stdout + exit.
2. **VM backend:** native VM-emit (`build/sv0-megatu-vm-native`, via
   `SV0_DRV_REQUEST`) → `.sv0b` → `sv0vm` (`scripts/run_sv0b.sml`); capture stdout
   + exit.

Then compare:

- **integer / i64 / u64 / modular:** results MUST be **bit-identical** (COMPAT-001).
- **floating-point:** a fixture whose check is not exit-code-based would need a
  ULP compare (bit-pattern-ordering metric, `sv0-mathlib/docs/ulp_audit_harness.c`).
  None exist yet — every current fixture (and `sv0-mathlib`'s own `main.sv0`)
  reduces its result to an exit code, which already exercises the f64
  arithmetic / comparison paths.

**Status (2026-08-29): implemented.** `scripts/vm_behavioral_parity.py` reads
`behavioral-manifest.txt` (this dir) — one entry per line, a repo-root-relative
`.sv0` path or `--project <dir>` (an absent `--project` dir is skipped). Fixtures:
`programs/vmf_f64_*.sv0`, `programs/vmf_i64_*.sv0`, plus `int_min` / `contracts`
and `--project sv0-mathlib` (the `sv0-mathlib` submodule — present in every
recursive checkout). Run it via `./scripts/sv0 vm-behavioral-parity`; it is also a
step in `./scripts/sv0 test` (after tier-2) and in
`.github/workflows/vm-parity-tier2.yml`. C emit goes through
`native_exe_canonical_compile.compile_and_publish` (no manual `cc` recipe).

**`sv0-mathlib` also consumes this** — its own `scripts/ci` runs the same
harness (`--project sv0-mathlib`), closing COMPAT-001 / COMPAT-002 / TEST-005
for the exit-code surface. It also runs its own `scripts/run_fixture_parity.py`,
which generates one `fn main()` check per row of `test/fixtures/{rounding,trig}.csv`
and runs it through BOTH backends, asserting each result matches the fixture's
expected value and that the C and VM exit codes agree — the per-fixture surface.
The broad full-domain ULP sweep in `sv0-mathlib/docs/ulp_audit_harness.c` stays
C-backend-only by design (it needs `<math.h>` as its reference oracle).
