# mcall_compound_arg — method call with a compound argument (BH-1)

Regression for bug-hunt finding #1: a method call `recv.m(arg)` whose argument is
compound (binop, call, nested method call) was silently miscompiled on the native
pipeline — the argument was replaced by its first sub-expression node
(`p.add(n + 17)` → `add(p, n)`). Root cause: the tag-5 (ExprMethodCall) node
discarded its arg-root sidecar and lowering/resolver/checker reconstructed args as
`args_first + i` (a contiguous single-node assumption wrong for compound args).

Fixed by giving tag-5 the same pp arg-root sidecar as tag-4 ExprCall
(`ed3 = argc`, `ed4 = block_stmt_sidecar_push`). Expected **exit 42**
(`p.add(n+17)=42`, `p.sum()=20`, `42 - 20 + 20`). Runs on native `--project`,
native single-file, and SML.
