# Native full-compose: (A) mega-TU vs (B) multi-unit link — trade-off analysis

**Date:** 2026-07-23. **Purpose:** decide how to run the *real* sv0 pipeline modules
(`lib/lexer.sv0` … `lib/vm_codegen.sv0`) together as one **native** compiler, so the
sv0 pipeline can be diffed against SML. This is the **critical-path** L0 work: it
unblocks Phase C validation (`linkProjectDir`/resolver/checker/lowering parity),
full P1 (composed driver), P2 (native full self-host), and the preferred P4 route.
Supersedes the (A)/(B) framing in `driver-pipeline-composition.md` with concrete
evidence.

## The fundamental constraint

sv0-as-bootstrapped has **no cross-unit linking mechanism**: no `extern`, no
import-symbol syntax, and each `lib/*.sv0` compiles **standalone** (referencing a
symbol from another module is `E0300: unbound`; verified — `link.sv0` cannot call
`tokenize`/`parse_program`). The `use`/`module` mangling in `link.sv0` is for the
SML **project** mode (source concatenation), not native object linking. So the real
modules cannot call each other as written; any composition must **make all phase
functions visible to one `main`**.

Reference: the SML stage-0 compiles **20** units into **one heap** via `sources.cm`
(`span → diagnostic → lexer → ast → parser → env → resolver → types → unify →
checker → analyzer → ir → lowering → codegen → bytecode → vm_codegen → link →
include_expand → main`). "Compose natively" = reproduce that single-artifact result
with a `cc` binary.

## Evidence (sv0 pipeline, 18 modules ≈ 31k lines)

| Fact | Value | Implication |
|---|---|---|
| Total top-level `fn` decls | 1651 | scale of a mega-TU |
| Distinct names | 1612 | ~99% already unique |
| Colliding names (>1 module) | **21** | small, bounded dedup |
| — `main` | 18-way | test harnesses; keep exactly one |
| — `test_*` | 8 | tests, excluded from a real compiler |
| — genuine logic collisions | **~14** | `emit_expr`/`emit_value`/`emit_program`/`emit_instr(s)` (codegen vs vm_codegen — different backends → rename `c_`/`vm_`); shared accessors `ty_tag`/`expr_tag`/`value_tag`/`instr_tag`/`is_void_cty`/`handle_to_str`/`digit_char`/`find_last_slash`/`find_last_dot` (likely dedupable) |
| C codegen linkage | `static` for every non-`main` fn (+ forward decls) | B's linking blocker (a policy, `emit_fn_proto is_static`) |
| `extern`/import-symbol syntax | **none** | B needs a new language feature |
| 2-module concat (span+types, one `main`) compiles | **yes** (SML, 645 lines C) | A assembly is viable |
| `driver.sv0` (self-contained full-pipeline TU) | 98/98 native parity | the (A) *shape* already works natively |
| Native driver expands `include` | **no** | A assembly is a build-step concat, not compiler `include` |

## Option A — mega-TU (assemble the real modules into one translation unit)

**Mechanism.** A build step concatenates the phase modules into one `.sv0`, keeping
exactly one `main`; that `main` (a "compose driver") threads the phases —
`tokenize → parse_program → resolve → check → lower → codegen/vm_codegen` — reusing
the real functions (now all visible in the one TU). Compile that TU SML→C→`cc` into
the native `SV0_SELF_HOST_COMPILER`.

**Work required.** (1) Build-time assembler: concatenate modules, strip the 18 test
`main`s + `test_*`, resolve the collisions. Deterministic, scriptable —
`scripts/assemble-sv0-megaTU.py` (`./scripts/sv0 assemble-megatu`). (2) Write the
compose `main` threading the real phase signatures (e.g. `parse_program`'s 36
arenas) — the core new logic, but it is exactly what `driver.sv0`'s
`drv_compile_file` does, using the *real* modules instead of the `drv_` subset.
(3) No language or codegen changes.

**Collision surface — bigger than the `fn` count, but mechanically resolvable
(A1 result).** Beyond the ~14 logic-`fn` collisions, the core **types** collide and
are **not identical**: `Value` has two shapes (`ir`/`vm_codegen` use
`VBool(bool)`+`VFloat`; `lowering`/`codegen` use `VBoolTrue`/`VBoolFalse`), and
`Expr` (5 modules), `Instr` (4), `Ty` (3) similarly diverge — so types can't be
deduped, they must be **namespaced per module**. The assembler does this
**automatically**: any top-level name a module defines that an earlier module
already claimed is renamed `<stem>_<name>` (definition + every word-boundary use
within that module). Because standalone modules never cross-reference, this is safe
and self-contained. **Result: all 18 pipeline modules assemble into one TU that
compiles (SML→C ~34k lines → cc, binary runs)** — `./scripts/sv0 assemble-megatu
--check`. The divergent types coexist (`Value` + `lowering_Value` +
`codegen_Value` + `vm_codegen_Value`; five `Expr`s), proving (A)'s assembly step is
done. **A2 (in progress):** the assembler appends `sv0c/lib/megaTU-main.sv0` as the
compose `main`; `assemble-megatu --check` compiles **and runs** it. It now threads
**four real phases — `tokenize → parse_program → resolve_program → check_program`**
— all returning success on a valid program in the composed TU. The phase
**boundaries are arena-based** (`lower(...)` and `emit_program(typedefs, blocks:
Vec<i32>)` take/produce `Vec<i32>`), so the compose main threads arenas and the
per-module namespaced IR types stay internal.

**Key A2 finding — the drift is SEMANTIC, not arena-format.** The pipeline modules
were transliterated and unit-tested **standalone, never actually composed**, so
gaps surface only when phases run together. The parser→checker boundary looked like
a format mismatch (`check_program` returned `-1` on a valid program) but was an
**incomplete transliteration**: the checker's own test only used *tail-expression*
bodies (`{ 42 }`), never a *return-statement* body, so `synth_expr` typed a tailless
block `{ return 0; }` as `TY_UNIT` and the strict `expect` (equality) rejected it
against `-> i32`. **Fix (faithful to SML):** a tailless block whose last statement
`return`s now types as `TY_VAR` (SML `freshVar`), and `expect` treats `TY_VAR` as a
wildcard (SML unification) — `sv0c/lib/checker.sv0`. checker self-tests + self-host
98/98 stay green (stage0 + vm-parity goldens refreshed). **Remaining:** reconcile
`lower` then `emit_program` the same way, validated by native-vs-SML behavioral
parity once emit lands.

**Pros.** Sidesteps the missing cross-unit mechanism entirely; reuses the
**proven** self-contained-TU model (`driver.sv0` at 98/98); assembly is a
deterministic script; smallest new-capability surface (no compiler-feature work).

**Cons.** One ~31k-line TU: all-or-nothing compile (any module error fails the
whole build); the assembler/dedup is maintenance overhead and diverges from the
clean module layout; collision renames must be kept in sync as modules evolve;
large generated C.

## Option B — multi-unit C link (compile each module to `.c`, link them)

**Mechanism.** Emit each module's cross-referenced functions as **non-`static`**
(extern) with a shared header of forward decls, then `cc`-link the objects + a
driver that calls across modules.

**Work required.** (1) A **language feature**: `extern`/import-symbol so a module
can *declare and call* another module's function in sv0 source (today `E0300`
blocks it) — this touches lexer + parser + resolver + checker + codegen of the
bootstrap compiler. (2) Change codegen's `static` policy for exported symbols +
emit a shared header. (3) A linking/packaging convention. Without (1), pure-sv0
modules simply cannot reference each other, so B **cannot be expressed** today.

**Pros.** Preserves the modular structure (mirrors `sources.cm`); incremental
compile per module; the maintainable long-term end state the composition doc favors.

**Cons.** Requires adding a **new language feature to the compiler being
bootstrapped** — large, high-risk, and self-referential (the extern feature must
itself round-trip through self-host). Much bigger than A; no part of it is proven
today.

## Option C (baseline) — hand-written self-contained subset = `driver.sv0`

The status quo: a hand-written single TU implementing the *essential subset*,
proven at 98/98 native behavioral parity. Not full-language (no cross-module types,
limited features) — it is the (A) *shape* without the real modules' full fidelity.

## Recommendation

**Pursue (A) mega-TU, assembled by a build script.** Evidence-based reasoning:

- (A)'s prerequisites are all **met or proven**: concatenation compiles, collisions
  are small (~14 real), and `driver.sv0` shows a self-contained full-pipeline TU
  runs natively at 98/98. The only genuinely new work is the compose `main`.
- (B)'s prerequisite is a **new `extern` language feature** in the bootstrap
  compiler — a large, self-referential, unproven addition. This updates the earlier
  "(B) for maintainability" lean in `driver-pipeline-composition.md`: (B) is the
  cleaner *destination* but is gated on a language feature we do not have, so it is
  not the right *first* move.
- (A) also directly unblocks **Phase C validation** and **P4** soonest, since a
  working native full compiler is what lets the real modules be diffed against SML.

**Sequence.** (A1) build-time assembler (concat + strip mains/tests + rename the ~14
collisions) with a guard that the assembled TU compiles; (A2) compose `main`
threading the real phases for `--emit-c`, validated by diffing native-vs-SML on the
`self-host-sv0-loop` corpus (behavioral parity, as in `self-host-native-parity`);
(A3) extend to `--target=vm` (feeds P4); (A4) treat (B) as a later refactor **iff**
an `extern` feature is added for other reasons. Revisit if the compose `main`'s
data-threading proves larger than `driver.sv0`'s.

## Related

- `driver-pipeline-composition.md` — original (A)/(B) framing + `driver.sv0` self-contained rationale
- `l0-closure-roadmap.md` — Phase A/B/C/D sequencing; composition is the critical path
- `native-self-host-compiler-recipe.md` — `SV0_SELF_HOST_COMPILER` contract
- `self-host-sv0-loop.md` — the corpus + behavioral-parity check the compose driver must pass
