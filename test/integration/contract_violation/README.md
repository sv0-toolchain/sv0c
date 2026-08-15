# contract_violation — runtime contract abort (BH-10c)

`f(-5)` violates `requires(x > 0)`. On the VM this used to raise an uncaught SML
`Fail` exception; it now aborts cleanly — prints `sv0 contract violation: ...` to
stderr and exits 1 (`vm_exit:1`), matching the C runtime's `exit(1)`. Gated on the
sv0vm integration harness. (native->C still drops the check until BH-10a.)
