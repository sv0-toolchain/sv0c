# shadowing — `let` shadowing works on every backend (BH-12)

sv0 permits `let` shadowing, but a flat C function scope cannot redeclare a name
and the VM reused one slot per name. This fixture combines sequential same-block
shadowing (`x`) and nested-block shadowing (`a`, whose inner binding must not leak
past its block). Correct result is **42**. See doc/archive/bug-hunt-findings.md #12.

Native C: a shadowing-index pre-pass (megaTU-main.sv0) renames re-bound locals to
`x_N`. (SML->C and VM parts tracked alongside.)
