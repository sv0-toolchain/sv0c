# `edu_bits` — tiny integer parity helpers (educational)

## Purpose

Bit-twiddling and **low-level numeric intuition** matter for systems programming and for crypto-adjacent work later in the [roadmap](http://development.sasankvishnubhatla.net/tcowmbh/task/sv0-compiler-vision-and-design.html). This package stays **tiny**: it only uses **`%`**, **`==`**, and **`bool`**, so it pairs well with early tutorials that avoid shifts and masks.

## API (module `lib`)

| Function | Behavior |
|----------|-----------|
| `is_even(n)` | `true` when `n % 2 == 0`. **Note:** in two’s complement, `%` for negative `n` follows the language definition — this example assumes teaching with **non-negative** demo values first. |
| `is_odd(n)` | `true` when `!is_even(n)`. |

## Teaching points

1. **`bool` vs `i32`** — `main` must return `i32`; the demo uses `if` to turn a boolean fact into an integer exit code. That mirrors how host processes consume exit statuses.
2. **Specification** — For production, parity on **all** `i32` values might be specified in **`sv0doc/`** (sign of `%`). Here we document the **pedagogical** subset in comments.
3. **Path to “real” std** — A future **`core::num`** might offer tested parity and wrapping helpers; this tree is only a **template**.

## Demo

`main.sv0` returns **`42`**: `40 + (if is_even(4) { 2 } else { 0 })`.

```bash
./scripts/sv0 vm-project-compile examples/libs/edu_bits
./scripts/sv0 vm-run sv0c/build/vm/main.sv0b
# expect: vm_exit:42
```

## Potential future `std` placement

Could evolve toward something like **`core::integer`** parity helpers or test-only utilities under **`std::testing`**, once module naming is frozen in **`sv0doc/`**.
