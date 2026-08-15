# contract_violation — runtime contract abort (BH-10c)

`f(-5)` violates `requires(x > 0)`. On the VM this used to raise an uncaught SML
`Fail` exception; it now aborts cleanly — prints `sv0 contract violation: ...` to
stderr and exits 1 (`vm_exit:1`), matching the C runtime's `exit(1)`.

Gated on the sv0vm integration harness (VM → vm_exit:1, BH-10c) **and** the
native pc3b6 acceptance (native → exit 1 + "contract violation", BH-10a/BH-10b):
the native compose main now routes requires/ensures through lowering, so the
check is enforced there too — SML→C, native→C, and the VM all abort on the
violation.
