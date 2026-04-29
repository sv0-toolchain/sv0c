# G6 remaining work — engineering blockers (link + driver)

This note explains why **`task/sv0-toolchain-milestone-3-self-host.Rmd`** **## M3 G6 slice status** still lists **M3-S-039** / **M3-S-040** / **M3-S-041** as **Partial** despite predicate-heavy **`lib/link.sv0`** and **`lib/main.sv0`** driver naming tests.

## M3-S-039 — `mapTy` / `mapExpr` / `mapPat` / `mapItem` arena rewrites

**SML** (`sv0c/sml/link/link.sml`) builds recursive **`Ast.*`** trees; `mapPathSegs` returns **new** path lists with **fresh** identifiers after `mangle`.

**sv0 bootstrap** stores paths as **indices into the lexer token stream** (`pp` pool + `starts`/`ends` into **`source`**). A **semantic** rewrite must either:

1. **Replace token indices** with handles whose span text equals `mod__Name` (requires **new** source text + **new** `(start,end)` rows — i.e. a **synthetic token append** API), or  
2. **Clone** the whole program into a side arena that owns copied strings (major pipeline change).

**Shipped toward closure**

- Predicate stack (`link_*_needs_mangle`, subtree counts) — verification-only.  
- **String preview (no mutation):** **`link_ty_tyname_first_seg_preview`** applies `map_path_segs_rewrite_1` / `_2` to the **text** of the first path segment for **`TyName`** rows — documents **what** the rewriter would emit before handles exist.

**Gate:** introduce **`parser`-level** (or link-local) **synthetic ident tokens** + mutation policy for `pp` / item arenas; until then **M3-S-039** stays **Partial**.

## M3-S-040 — `linkProjectDir` program merge

**SML:** `List.concat (map (fn p => mapProgramUnit p (parseFile p)) paths)` — **per-file** parse, **per-module** `mapItem`, then concatenate **AST** item lists.

**sv0:** `parse_program` consumes **one** token stream. Concatenating sources then lexing once is **not** equivalent (diagnostics paths, module boundaries, token indices). A faithful merge needs **per-file** `parse_program` plus **index relocation** / merged program-level arenas — blocked on the same handle strategy as **M3-S-039** for cross-module references.

## M3-S-041 — single bootstrap TU driver

Requires calling **lexer → parser → resolver → … → emit** inside **one** `.sv0` translation unit (or an agreed **`#include`** composition model). Today phases are split across **`lib/*.sv0`** roots compiled separately by the **SML** heap loader — **Partial** until multi-unit **sv0** linking or an explicit pipeline crate exists.

## Related

- **`sv0c/doc/transliteration-plan.md`** — Link row.  
- **`sv0c/doc/bootstrap-compiler-workarounds.md`** — synthetic names / arena limits.  
- **`42-transliteration-complexity-invariant.mdc`**.
