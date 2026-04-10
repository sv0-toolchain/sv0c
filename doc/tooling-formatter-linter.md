# sv0-formatter and sv0-linter (design sketch)

This note records a **workspace-level** direction for two future tools: **`sv0-formatter`** (pretty-print **sv0** source) and **`sv0-linter`** (static checks on **sv0** source). It is **not** normative language semantics; see **`sv0doc/`** for grammar and typing rules.

## Relationship to what exists today

| Concern | Today | Target tool |
|---------|--------|----------------|
| Trailing whitespace + final newline on **`.sv0`** | **`scripts/fmt_sv0.py`**, invoked as **`./scripts/sv0 fmt`** (see **`task/sv0-toolchain-milestone-2-prep.Rmd`**) | Absorb or supersede with **`sv0-formatter`** **phase 0** (same contract: conservative, no AST reflow until specified) |
| Deep layout (indent, line breaks, import/`use` order) | Not standardized in tooling | **`sv0-formatter`** **phase 1+**: AST-driven printer locked to **`sv0doc/grammar/`** + golden **`.sv0`** corpus |
| “Obvious” hygiene (nested **`/* */`**, UTF-8 assumptions) | Python guards under **`./scripts/sv0 test`** / **`test-guards`** | **`sv0-linter`** **phase 0**: same rules as **library** checks + stable exit codes for CI |
| Semantic rules (**E0xxx**), borrow/contract clauses | SML bootstrap compiler diagnostics | **`sv0-linter`** **phase 1+**: reuse compiler front-end (lexer → resolver/checker) as a **library**; emit **JSON** or **GNU-style** locations for editors |

## sv0-formatter — proposed shape

1. **CLI** (stable contract): `sv0-formatter [--check] [--write] [paths...]`; **`--check`** exits **1** if any file would change (matches **`fmt_sv0.py --check`** behavior for phase 0).
2. **Phases**
   - **Phase 0 (whitespace)**: keep behavior aligned with **`scripts/fmt_sv0.py`** so CI can swap the implementation without churning every **`.sv0`** file.
   - **Phase 1 (AST printer)**: parse with the same parser as **`sv0c`**, print canonical spacing from a **style profile** (documented in-repo, versioned). Add **golden** “format round-trip” tests on **`sv0c/lib/`** seeds and **`sv0doc`** examples.
3. **Implementation home**: start as **Python** or **Rust** CLI in **`sv0-toolchain/scripts/`** or a small **`sv0c/tools/`** crate—decision belongs in **`task/sv0-toolchain-milestone-2-prep.Rmd`** when the slice is scheduled; long term the **self-hosted** compiler may own **`fmt`** once **M3** exposes a stable API.

## sv0-linter — proposed shape

1. **Rule tiers**
   - **Tier A — text/hygiene**: block-comment nesting, disallowed control bytes, max line length (optional), **UTF-8** validation.
   - **Tier B — syntax**: parse-only; classify “parse error” vs “unsupported in bootstrap”.
   - **Tier C — semantic** (optional flags): unused locals, unreachable code, **style-only** naming (off by default until policy exists in **`sv0doc/`** or maintainer guide).
2. **Diagnostics**
   - Reuse **`sv0c`** **diagnostic** codes where the rule maps 1:1 to compiler errors.
   - For **lint-only** rules, use a dedicated **`L####`** namespace documented in **`sv0c/doc/`** so **`sv0-linter`** never collides silently with compiler **E-codes**.
3. **Editor integration**: machine-readable stream (**`{file,span,code,message}`** per line) on stdout; human default mirrors **`rustc`**/**`clang`** `file:line:col` for local runs.

## Validation and rollout

- **Formatter phase 0**: diff-empty run on **`sv0c/`** + **`sv0vm/`** trees vs **`fmt_sv0.py`** before replacing the script.
- **Linter tier A/B**: run on **`lib/bootstrap-sources.list`** in CI without failing the compiler test harness; gate **Tier C** behind **`--deny`** flags until rules are agreed in **`sv0doc/`** or a **`task/*.Rmd`**.

## Related

- **`task/sv0-toolchain-milestone-2-prep.Rmd`** — milestone-2 **`fmt`** / **`doc`** expectations.
- **`scripts/fmt_sv0.py`** — current whitespace formatter.
- **`task/sv0-toolchain-milestone-3-self-host.Rmd`** — when the compiler is in **sv0**, **`sv0-formatter`** may call the same AST types as **`fmt`** today calls only text.
