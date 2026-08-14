# int_min — the `2147483648` (2³¹) literal wraps to INT_MIN on every backend (BH-7)

Bug-hunt finding #7: the literal `2147483648` (one past `i32::MAX`) was handled
three different ways. `let a: i32 = 0 - 2147483648;` then `a < 0`:

- **SML→C:** emitted the literal verbatim → C computes INT_MIN → `a < 0` true → 42 (already correct).
- **native→C:** the literal overflow-wrapped to INT_MIN during parse, but
  `int_to_string(INT_MIN)` did `0 - n` (which overflows back to a negative) and
  emitted the bogus `-?` → **cc failure**. Fixed: `int_to_string` special-cases
  INT_MIN (`lib/codegen.sv0`).
- **SML→VM:** `VmCodegen.int32` did `Int32.fromLarge` on `2147483648` → uncaught
  **Overflow** → emit exit 1. Fixed: wrap through `Word32`
  (`sml-legacy/backend/vm/vm_codegen.sml`), so `2³¹` → INT_MIN, matching the C
  wrap; the VM interpreter's Word32 arithmetic (BH-2) then evaluates
  `0 - INT_MIN` to INT_MIN at run time.

All three now → **exit 42**. Gated on native `--project` (pc3b6), SML `one`
mode, and sv0vm (`--target=vm` + run_sv0b).
