# Native executable: entry ABI (NEX-054c)

Implements ENTRY-010's documentation half
(`~/Documents/project-specs/sv0c-runtime-executable/SPEC.md`): "the entry
ABI and reserved symbols SHALL be versioned and documented." The
versioning/enforcement machinery lives in `scripts/native_exe_entry_abi.py`
and `scripts/native_exe_entry_reserved.py`; this doc is the human-readable
statement of what that machinery enforces.

## Reserved symbols

The native-executable entry adapter reserves two C symbol names. A source
program that declares a top-level `fn` with either name is rejected at
build time (`native_exe_entry_reserved.validate_no_reserved_collisions`,
NEX-017) before the core compiler ever runs:

| Symbol | Role |
|---|---|
| `sv0_user_main` | The generated wrapper around the user's own `main` — the emitter renames the user's `fn main` to this symbol so the adapter below can call it. |
| `sv0_runtime_init` | Runtime initialization hook (`sv0_runtime.h`), called once before `sv0_user_main`. Currently a no-op — its slot is reserved so a future runtime initialization need never changes every executable's entry contract. |

## Hosted `main` adapter

The megaTU C emitter (`sv0c/lib/megaTU-main.sv0`) generates a hosted C
`main(int argc, char **argv)` that calls `sv0_runtime_init` then
`sv0_user_main`, in one of two exact shapes depending on the user's own
`main`'s declared return type:

**i32-returning user `main`** (`fn main() -> i32`):

```c
int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}
```

**unit-returning user `main`** (`fn main() -> ()` or no declared return):

```c
int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  sv0_user_main();
  return 0;
}
```

No other user `main` signature (a parameterized `main`, one returning any
other type) is accepted — entry-signature validation
(`native_exe_entry_signature.py`, NEX-015) rejects it before either shape
would ever be emitted.

## Versioning and compatibility

`ENTRY_ABI_VERSION` (`scripts/native_exe_entry_abi.py`), currently `1`, is
a version number over exactly the two things above: the reserved-symbol
set and both adapter shapes. It is a **separate, parallel concept** to the
runtime bundle's own `ABI_VERSION` (`native_exe_runtime_manifest.py`,
NEX-020) — that number versions the runtime header/source hashes, not the
entry contract.

Two independent checks enforce this:

- **Repo-hygiene drift check** (`verify_entry_abi_manifest`): at
  development time, confirms the live reserved-symbol set and adapter
  shapes in this checkout still hash-match the committed snapshot
  (`sv0c/runtime/entry-abi-manifest.json`) for the CURRENT
  `ENTRY_ABI_VERSION`. A change to either without a deliberate version
  bump fails this check — it exists to catch an accidental entry-contract
  change before it ships, not to gate a real build.
- **Installed-runtime compat check** (`verify_entry_abi_compat`): at build
  time, wired into `build_native_executable`'s own pipeline, confirms the
  `entry_abi_version` declared in the *installed* runtime bundle's
  `entry-abi-manifest.json` matches this compiled driver's own
  `ENTRY_ABI_VERSION`. A mismatch — an installed runtime built for a
  different entry-ABI generation than the driver currently running —
  fails the build closed with a clear diagnostic, mirroring
  `verify_manifest`'s existing behavior for the runtime bundle's own ABI
  version.

`entry-abi-manifest.json` ships as part of the installed runtime bundle
(`sv0c/runtime/`), alongside `runtime-manifest.json`,
`abi-version.txt`, and the bundle's license files
(`native_exe_runtime_bundle.py`'s required-members list).

## Compatibility policy going forward

A change to the reserved-symbol set or either adapter shape is a breaking
entry-ABI change and REQUIRES bumping `ENTRY_ABI_VERSION`, regenerating
`entry-abi-manifest.json` (`native_exe_entry_abi.py --write-manifest`),
and documenting the change in this file. An installed runtime bundle at an
older or newer `entry_abi_version` than the running driver is rejected
outright — there is no partial-compatibility or best-effort fallback
between entry-ABI generations.
