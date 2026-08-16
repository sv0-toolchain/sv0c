# Behavioral PASS corpus

The positive counterpart to the [diagnostics](../diagnostics/) (reject) corpus:
small, valid programs that must **compile and run to a specific exit code** on the
native default compiler. Covers language features and boundary values with
known answers.

## Layout

- **`manifest.txt`** — one entry per line:

  ```
  path_relative_to_sv0c/ | expected_exit [ | leg ]
  ```

  `leg` is optional and one of `both` (default) or `native`. A `native` row runs
  only on the C leg — used for programs the **VM backend does not yet support**.
- **`cases/*.sv0`** — a program whose `main` returns the expected exit code, with a
  header comment describing what it exercises.

Exit codes are the process exit (`main`'s return masked to 0–255), so keep
answers in 0–255.

## Validation — two legs, both run by `./scripts/sv0 test`

- **C leg** (`scripts/verify_behavior_corpus_native.py`): compiles each program
  with `build/sv0-megatu-compiler-native`, `cc`s it, runs it, asserts the exit.
  Runs every row.
- **VM leg** (`scripts/verify_behavior_corpus_vm.py`): compiles each program with
  the SML `--target=vm` emitter, runs the bytecode on sv0vm, and asserts
  `vm_exit & 0xFF` equals the expected exit. Skips `native`-tagged rows.

## VM backend gap

Programs using **chained field access** (e.g. `o.inner.v`) are tagged `native`:
the VM codegen currently rejects them with
`vm: FieldAccess needs VVar base in this slice` (simple one-level `o.x` works).
Native C handles them. This is a documented VM-backend limitation, a candidate
for a future fix; the C leg keeps them covered meanwhile.

## Growing the corpus

Add a `.sv0` under `cases/` whose `main` returns a known value, then append a
`test/behavior/cases/<stem>.sv0|<exit>` line. Validate locally first —
compile+cc+run and confirm the exit — before committing.
