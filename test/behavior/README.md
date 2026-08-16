# Behavioral PASS corpus

The positive counterpart to the [diagnostics](../diagnostics/) (reject) corpus:
small, valid programs that must **compile and run to a specific exit code** on the
native default compiler. Covers language features and boundary values with
known answers.

## Layout

- **`manifest.txt`** — one entry per line: `path_relative_to_sv0c/ | expected_exit`.
- **`cases/*.sv0`** — a program whose `main` returns the expected exit code, with a
  header comment describing what it exercises.

Exit codes are the process exit (`main`'s return masked to 0–255), so keep
answers in 0–255.

## Validation

`./scripts/sv0 test` runs `scripts/verify_behavior_corpus_native.py`: it compiles
each program with `build/sv0-megatu-compiler-native`, `cc`s it, runs it, and
asserts the exit code. (Feature behavior on the VM backend is covered by the
`vm-parity` and integration suites.)

## Growing the corpus

Add a `.sv0` under `cases/` whose `main` returns a known value, then append a
`test/behavior/cases/<stem>.sv0|<exit>` line. Validate locally first —
compile+cc+run and confirm the exit — before committing.
