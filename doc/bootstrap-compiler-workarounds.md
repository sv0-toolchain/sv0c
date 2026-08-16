# Bootstrap compiler workarounds

The SML/NJ bootstrap compiler imposes constraints on `.sv0` source files that go beyond normal language semantics. These constraints arise from SML/NJ's recursive AST processing, the generated C code's stack model, and the C backend's while-condition hoisting. This document catalogs the known constraints and the coding patterns that avoid them.

## 1. Stack depth and expression complexity

**Symptom:** `sml @SMLload=build/sv0c <file>` exits silently with code 1 and no output.

**Root cause:** SML/NJ uses the C stack for recursive AST walks (parsing, type checking, lowering, code emission). Deeply nested expressions in a single function produce deep recursion that can overflow the OS stack. The failure is silent because SML/NJ does not print a diagnostic for stack overflow — the process simply exits.

**What triggers it:** Deeply nested function calls in a single expression, especially chained `string_concat`:

```sv0
// BAD — 4 levels of nesting; can crash the SML compiler on large files
return string_concat(a, string_concat(b, string_concat(c, string_concat(d, e))));
```

**What does NOT trigger it:** High function count alone. `parser.sv0` has 288 functions and compiles fine. The limit is per-function expression depth, not total function count.

### Mitigation: sequential assignment

Flatten nested calls into sequential assignments so each expression is one level deep:

```sv0
// GOOD — each expression is flat
let mut r: string = a;
r = string_concat(r, b);
r = string_concat(r, c);
r = string_concat(r, d);
r = string_concat(r, e);
return r;
```

### Mitigation: raise OS stack limit

`scripts/sv0` raises the soft stack limit to the OS hard limit (`ulimit -s "$(ulimit -Hs)"`) before SML invocations — typically ~64 MB on macOS vs the 8 MB default. This buys room for moderately nested expressions but is not a substitute for keeping expressions flat.

## 2. Per-tag helper decomposition (large function pattern)

Large SML functions like `lowerExprToValue` (237 lines, one big `case` expression) cannot be transliterated as a single sv0 function — the resulting expression tree is too deep for the bootstrap compiler's recursive processing.

### Pattern: dispatch + per-tag helpers

Break the monolithic `case` into a thin dispatcher that calls per-tag helper functions:

```sv0
fn lower_expr_to_value(/* shared args */) -> i32 {
    let tag: i32 = vec_get(et, idx);
    if tag == 1 { return lower_expr_path(/* args */); }
    if tag == 3 { return lower_expr_binop(/* args */); }
    if tag == 4 { return lower_expr_call(/* args */); }
    if tag == 5 { return lower_expr_field(/* args */); }
    if tag == 8 { return lower_expr_if(/* args */); }
    if tag == 10 { return lower_expr_match(/* args */); }
    // ... one line per tag
    return 0;
}

fn lower_expr_path(/* args */) -> i32 {
    // 10–30 lines of flat logic for one tag
}

fn lower_expr_call(/* args */) -> i32 {
    // 10–30 lines of flat logic for one tag
}
```

**Why this works:** Each helper function has a shallow expression tree. The dispatcher is just a chain of flat `if` + `return` — no nesting. The SML compiler processes each function independently, so the per-function recursion depth stays low.

**When to apply:** Any SML function with more than ~15 `case` arms or more than ~80 lines of body should be decomposed this way during transliteration.

## 3. Generated C binary stack overflow (SIGKILL 137)

**Symptom:** The `.sv0` file compiles to C, the C compiles, but the resulting binary crashes immediately with exit code 137 (SIGKILL from the OS — out of memory or stack overflow).

**Root cause:** Each `Vec<i32>` local variable in sv0 becomes a stack-allocated struct in the generated C code. Functions with ~20+ vector locals can exhaust the C runtime stack, especially when combined with deep call chains.

**Mitigation:** Keep functions — especially test functions — to fewer than ~10 `Vec<i32>` locals. Split vector setup into helper functions so each stack frame is smaller:

```sv0
// BAD — too many Vec locals in one function
fn test_big() -> i32 {
    let v1: Vec<i32> = vec_new();
    let v2: Vec<i32> = vec_new();
    // ... 18 more vectors ...
    return 0;
}

// GOOD — split into focused helpers
fn make_test_ast() -> Vec<i32> {
    let et: Vec<i32> = vec_new();
    // build and return
    return et;
}

fn test_small() -> i32 {
    let et: Vec<i32> = make_test_ast();
    // test with fewer locals per frame
    return 0;
}
```

## 4. Separate compilation units

Each `.sv0` file is compiled independently by the bootstrap compiler. Functions from one file are not visible to another at compile time. This means:

- **Large modules can be split across files.** If a file grows too large or complex, move loosely-coupled functions to a new `.sv0` file. Both files compile independently and can be linked at the C level.
- **Shared helpers must be duplicated** (or placed in a shared `include` file — see `doc/archive/transliteration-include.md`). If `checker.sv0` and `lowering.sv0` both need `expr_references_result`, each must have its own copy or both must `include` a shared fragment.
- **When to split:** Consider splitting when a file exceeds ~120 functions or when adding new functions triggers bootstrap compiler issues despite following the flat-expression pattern.

### Current split points

| File | Purpose | Why separate |
|------|---------|-------------|
| `bytecode.sv0` | `.sv0b` encoding/decoding | Decoupled from VM codegen; I/O-facing |
| `vm_codegen.sv0` | VM backend code generation | Large file; separate from C backend |
| `codegen.sv0` | C backend code generation | Large file; separate from VM backend |
| `ir.sv0` | IR data types | Shared by both backends and lowering |
| `lowering.sv0` | AST → IR lowering | Largest remaining transliteration target |

## 5. C backend while-condition hoisting

**Symptom:** Infinite loop in the generated C binary.

**Root cause:** The C backend evaluates `while` conditions containing function calls once before the loop starts. The condition variable is never re-evaluated.

**Mitigation:** Documented in `.cursor/rules/41-sml-process-and-vm-hang-prevention.mdc`. Use a mutable flag pattern:

```sv0
let mut done: bool = false;
while done != true {
    if f(x) != val {
        done = true;
    } else {
        // loop body
    }
}
```

## Related

- `.cursor/rules/41-sml-process-and-vm-hang-prevention.mdc` — agent-facing hang prevention rules
- `doc/archive/transliteration-include.md` — textual include mechanism
- `doc/archive/transliteration-plan.md` — overall transliteration roadmap
- `doc/compiler-passes.md` — SML compiler phase structure
