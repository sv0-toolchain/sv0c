# Textual `include` for transliteration sources

The bootstrap compiler expands **top-level** lines of the form:

```text
include "relative/path.sv0";
```

**before** lexing and parsing. This is a **toolchain convenience** for self-hosting seeds (shared token stand-ins), not part of the normative sv0 language specification in **sv0doc**.

## Rules

- The path is **relative to the directory of the file** containing the `include` line (like C include paths from the current file).
- `..` is **not** allowed in the path. Absolute paths (leading `/`) are rejected.
- **Cycles** are rejected with **E0323**.
- Only lines that match exactly (after trimming whitespace and an optional trailing `\r`) are expanded; other lines are left unchanged.
- Included fragments must **not** define `fn main`; exactly one entry `main` must remain in the root file for standalone bootstrap programs.

## Block comments in included text

The lexer treats `/*` … `*/` as block comments and tracks **nesting**: a `/` immediately followed by `*` opens another level; `*/` closes one level. A path fragment like `parser/**` inside a comment contains that `/*` pair, so the next `*/` may only close the inner level and leave the rest of the file (including real code) inside an unclosed comment—producing an empty parse and trivial C output. Prefer wording such as **`parser`** without a slash before `*`, or use line comments, when documenting paths in transliteration headers.

Bold markdown that places a slash between closing and opening stars (for example two emphasis spans written as `**a**/**b**`) produces the same `/*` pair and should be avoided; use **`a` and `b`** or separate words instead.

## Validation

`./scripts/sv0 test` runs **`scripts/check_sv0_block_comment_nesting.py`** over **`*.sv0`** under each present repo root among **sv0c/**, **sv0doc/**, **sv0vm/**, and **sv0-mcp/** (strings and `//` comments skipped) so nested `/*` inside a block comment fails the suite early. The same script rejects lines containing **`**/**`**, which usually trigger the same lexer bug inside block comments.

## Reference

Implementation: **sml/include_expand.sml**, invoked from **sml/main.sml** (single-file compile) and **sml/link/link.sml** (`parseFile` for `--project`).
