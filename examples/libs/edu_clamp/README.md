# `edu_clamp` — educational `i32` bounding helpers

## Purpose

This package shows how a **tiny utility library** might look in sv0 before a real **`core::num`** (or similar) exists. The code is intentionally small so you can read it in one sitting and map each function to ideas from the [vision doc](http://development.sasankvishnubhatla.net/tcowmbh/task/sv0-compiler-vision-and-design.html): **explicit behavior**, **contracts as documentation**, and **no hidden surprises**.

## API (module `lib`)

| Function | Behavior |
|----------|-----------|
| `clamp(x, lo, hi)` | If `lo <= hi`, returns `x` limited to `[lo, hi]`. If `lo > hi`, the current implementation still returns *some* value in a predictable way — **see comment in source**; a production library would **`requires(lo <= hi)`** or return **`Option`**. |
| `min_i32(a, b)` | Returns the smaller of `a` and `b`. |
| `max_i32(a, b)` | Returns the larger of `a` and `b`. |

## Teaching points

1. **Preconditions vs types** — “`lo <= hi`” is a *relationship between arguments*. The type system knows each is `i32`; it does not know ordering. That is classic **contract** territory (`requires(lo <= hi)` in full sv0).
2. **Postconditions** — You could add `ensures(result >= lo && result <= hi)` on `clamp` once the bootstrap checker supports what you need on nested expressions; until then, **tests** and **comments** carry the specification.
3. **Shipping story** — To ship something like this with the toolchain later, you would: freeze API under **`sv0doc/`**-linked semantics, add **vm-parity / integration** coverage, and version the library — none of which is required for this **example** tree.

## Demo

`main.sv0` returns **`10`**: `clamp(100, 0, 10) == 10`. Run:

```bash
# repo root
./scripts/sv0 vm-project-compile examples/libs/edu_clamp
./scripts/sv0 vm-run sv0c/build/vm/main.sv0b
# expect: vm_exit:10
```

## Potential future `std` placement

If this grew into a supported library, a plausible home would be something like **`std::num::saturating`** or **`core::cmp`**, depending on how the language splits **freestanding** vs **hosted** tiers — follow **`sv0doc/`** when those module names are normative.
