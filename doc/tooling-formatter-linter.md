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

## Style profile (formatter) — proposed rules

**Authority:** pretty-printing must **not** invent syntax; anything that changes token order or spellings belongs in **`sv0doc/`** (grammar) first. The **style profile** is a *layout* layer on top of an already-valid parse tree.

Document the profile in-repo (e.g. **`sv0c/doc/style-profile.md`** plus a machine-readable file such as **`sv0fmt.toml`** at the workspace or package root) with a **version** field so CI can pin “format vN”.

### Layout defaults (decision: project-configurable; defer hard defaults)

**Stakeholder choice:** indent width, tab vs spaces, max line length, and brace layout are **not** fixed at the toolchain level yet. The **first formatter implementation PR** picks discovery rules (where the profile lives), proposes **starter defaults** for **`sv0c/`** + **`sv0vm/`** corpora, and documents them in the profile file—avoid baking **100 vs 120** columns or **4 vs 2** spaces into **`sv0doc/`** unless a task explicitly promotes a style to normative policy.

| Area | Phase 0 (today) | Phase 1+ (AST printer) |
|------|-----------------|-------------------------|
| **Trailing whitespace** | Strip each line; ensure single **`\n`** at EOF | Same invariants (may remain outside profile or as profile **`version: 0`**) |
| **Indent unit** | N/A | **Profile-driven** (e.g. `indent_style = spaces`, `indent_width = N`); linter **L0006** applies only if the active profile **disallows** tabs |
| **Brace / block layout** | N/A | **Profile-driven** brace / `else` attachment rules, constrained by grammar samples in **`sv0doc/`** (formatter must not invent elisions the grammar forbids) |
| **Max line width** | N/A | **Profile-driven** optional `max_width`; soft warn vs hard wrap only where the grammar already allows line breaks (e.g. after `,` in lists) |
| **`use` / `mod` / `pub` ordering** | N/A | Stable **lexicographic** within a file for **`use`** groups *when* the profile enables import sorting; **`pub`** placement follows profile + corpus conventions; **never** reorder across **items** if it changes name resolution |
| **Blank lines** | N/A | Profile-controlled spacing between **top-level** items; optional rules for blanks inside functions |
| **Comments** | Preserve text; only trim trailing spaces on comment lines | **Block** `/* */`: preserve interior newlines; do not reflow nested block comments (same hazard as **`check_sv0_block_comment_nesting.py`**) |
| **String literals** | No change inside quotes | Escape style: prefer **minimal** escapes consistent with lexer; long strings: optional **break** only if a future spec allows implicit concatenation (otherwise leave as one line + optional width warn from profile) |

**Idempotence:** `format(format(ast)) == format(ast)` on the golden corpus. **Round-trip:** parse → print → parse succeeds and yields **structurally equivalent** AST (define “equivalent” as same resolved names + same spans optional for phase 1).

## Lint rules — catalog and severities

**Severities:** **`deny`** (exit 1 in CI), **`warn`** (exit 0 default; **`--deny-warnings`** for strict), **`allow`** (opt-in rules).

### Tier A — text and encoding (no parser)

| Code | Rule | Default |
|------|------|---------|
| **L0001** | UTF-8 well-formedness; reject **NUL** | deny |
| **L0002** | Disallowed control bytes (except **TAB** / **LF** / **CR** if spec allows **CR**) | deny |
| **L0003** | Nested **`/*`** inside block comments (mirror **`scripts/check_sv0_block_comment_nesting.py`**) | deny |
| **L0004** | Trailing whitespace on lines | warn (or align with **fmt** phase 0: auto-fix vs warn) |
| **L0005** | Missing final newline | warn |
| **L0006** | Tab characters in **`.sv0`** when the active **format profile** disallows tabs | warn |

### Tier B — parse surface

| Code | Rule | Default |
|------|------|---------|
| **L0100** | Parse error (any) | deny |
| **L0101** | Construct accepted by parser but explicitly **unsupported** in bootstrap (if distinguishable) | warn |

### Tier C — semantic / policy (compiler reuse or thin wrappers)

Reuse **compiler `E0xxx`** when the check is **identical** to the type checker or resolver. **Lint-only** rules use **`L1xxx`** so editors never confuse “compiler error” with “style”.

| Code | Example | Default |
|------|---------|---------|
| **L1001** | Unused local binding | warn |
| **L1002** | Unreachable statement after **`return`** / **`break`** / **`continue`** | warn |
| **L1003** | **`mut`** not required (could be **`let`**) | allow |
| **L1004** | Naming: **`snake_case`** for **`fn`** / locals (Rust-like) | allow |

**Bootstrap constraint:** Tier **C** must not require running **SML** forever; long term it runs on the **sv0** compiler library once **M3** exposes a **`check`** API suitable for embedding.

## Decision log

- **2026-04-18 — Layout defaults (option C):** Indent, line width, tabs, and brace attachment are **project-configurable** via a checked-in profile (name and discovery TBD in the first formatter PR). The meta-repo **does not** fix **100 vs 120** columns or **2 vs 4** spaces as global policy here; the first PR that lands **`sv0-formatter`** proposes starter values for **`sv0c/`** + **`sv0vm/`** trees and records them in that profile.

## Conflicts and precedence

1. **`sv0doc/`** grammar and semantics **win** over formatter heuristics.
2. **`fmt_sv0.py`** behavior is the **compatibility** baseline for **phase 0** until a replacement proves **diff-empty** on **`sv0c/`** + **`sv0vm/`** trees.
3. **`L####`** rules are documented only in **`sv0c/doc/`** until a task promotes selected warnings into **`sv0doc/`** (normative) or **`task/*.Rmd`** (policy).

## CI integration (sketch)

| Stage | Command | Gate |
|-------|---------|------|
| Fast | **`./scripts/sv0 fmt --check`** (or **`sv0-formatter --check`**) | Same as today’s whitespace contract |
| Medium | **`sv0-linter --tier=a,b`** on **`lib/bootstrap-sources.list`** paths | deny on **A**/**B** defaults |
| Strict (optional job) | **`sv0-linter --tier=c --deny-warnings`** | informational until rules stabilize |

## Related

- **`task/sv0-toolchain-milestone-2-prep.Rmd`** — milestone-2 **`fmt`** / **`doc`** expectations.
- **`scripts/fmt_sv0.py`** — current whitespace formatter.
- **`task/sv0-toolchain-milestone-3-self-host.Rmd`** — when the compiler is in **sv0**, **`sv0-formatter`** may call the same AST types as **`fmt`** today calls only text.
