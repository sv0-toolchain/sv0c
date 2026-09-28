# Coverage planner, map, hit, and C emission tests (sv0cov CV-107..CV-113)

`run_plan.py` exercises `lib/coverage_plan.sv0` through the compiler. The
planner runs after resolve/check and before lowering; the internal
`plan-dump` mode (`SV0_COVERAGE_REQUEST="plan-dump\n<ignored>"`) prints the
plan instead of C:

```text
source  <index> <logical path> <sha256>
entity  <index> function <qualified name> <source> <span> <owner>
point   <index> function_entry <entity> entry <source> <span>
branch  <index> <kind> <entity> <source> <span> <outcome count> <outcome names>
point   <index> branch_outcome <entity> <branch kind> <source> <span> <ordinal>
region  <index> <kind> <entity> <source> <span> <line_contributing> <lines> <terms>
```

`<terms>` is the counter expression, e.g. `+1*entry:0,-1*branch:2:0`
(`branch:<branch index>:<outcome ordinal>`).

`<span>` is start byte, end byte (exclusive), start line, start column, end
line, end column, file-relative, with LF lines and Unicode-scalar columns.

The script checks:

- the seven hand-reviewed sv0cov semantic fixtures
  (`sv0cov/tests/fixtures/semantic/*`): sources, function entities,
  `function_entry` and `branch_outcome` points, branches with their outcome
  names, and regions (spans, line numbers,
  counter expressions) equal their `expected-map.json`;
- the canonical map (`--coverage=map`, `lib/coverage_map.sv0`) of each of
  those fixtures is byte-identical to its `expected-map.json`, from both the
  native compiler and the native VM emitter;
- the map of `constructs.sv0` passes sv0cov's own `validate_map` with its
  source bytes (run with the first Python >= 3.10 found; the summary line
  says when it was skipped);
- `constructs.sv0` against the hand-reviewed `constructs.expected`
  (else-if chains, `for`, `loop`, loop invariants, block match arms with
  `break`/`continue`, `+=`, conditional expressions, tail expressions,
  block expressions);
- synthetic programs anchored to their own text: inherent and trait impl
  methods (`Type::name`), trait default methods (`Trait::name`), bodyless
  trait methods and `#[extern_c]` declarations (not entities), `pub`, array
  parameters (a `;` inside the signature), parenthesized contracts, a
  `module` name prefix, CRLF, tabs, multibyte text, and project source order
  (sorted by logical path);
- that the native compiler and the native VM emitter, which share the
  planner, print identical plans;
- that coverage fails closed with exit 9 for an `include`d file (the
  compiled source is not the file's bytes), an unknown mode, and `map` or
  `instrument` without their target and identity lines.

`./scripts/sv0 test` runs it.

`run_determinism.py` (CV-111) builds the map of each fixture and of
`constructs.sv0` five times with the native compiler and once with the VM
emitter, from different working directories, map paths, and environment
noise, and requires identical bytes. It also requires `map` mode's generated
C and `.sv0b` to be byte-identical to an `off` build (map mode adds no hit
operations).

`run_hits.py` (CV-112) checks hit placement. For `--coverage=instrument`
lowering places `CovHit(counter)` IR instructions from the planner's node
table, and the compiler checks after lowering that every planned counter is
placed exactly once and nothing else (`COV1020` otherwise). The internal
`hit-dump` mode prints each function's hits in IR pre-order. The script
requires, for every fixture and `constructs.sv0` on both binaries, that the
hits are exactly counters `0..N-1` with each function's entry counter first,
and that a dropped, duplicated, or orphan hit (`SV0_COVERAGE_FAULT` test hook)
and any hit in map mode fail with `COV1020`.

`run_emit_c.py` (CV-113) checks the generated C of `--coverage=instrument`.
The C carries a registration prelude after the runtime include: the runtime
interface (`__sv0cov_start`, `__sv0cov_hit`), one descriptor per map
fragment, and the module record (protocol major 1, map ID, program counter
count, target, compiler identity, the module's counter slice). Every hit is
`__sv0cov_hit(&__sv0cov_module, <i>u);`, and the hosted `main` calls
`__sv0cov_start(__sv0cov_modules, 1u);` right after `sv0_runtime_init`,
before user code. The map is written once the C is. For every fixture and
`constructs.sv0` the script requires:

- the instrument map to be byte-identical to the map-mode map;
- the prelude to carry the map's ID, counter count, and fragment table, and
  the C to hold exactly one hit per counter;
- the C minus the prelude, hits, start call, and CV-112 loop rewrite to equal
  the `off` build's C;
- the program, linked with the real sv0cov runtime
  (`sv0cov/runtime/c/sv0cov_rt.c`, CV-114) and run under a valid
  `SV0COV_*` transport, to behave exactly as the `off` build (the runtime
  accepts sv0c's registration), and with a malformed `SV0COV_RUN_ID` in
  required mode to exit 1 with `COV2001` before any output;
- the program, linked with `stub_rt.c` (a test-only stand-in for the CV-114
  runtime that validates the registration and counts hits) and run without
  gcov, profiling, or debug flags, to exit as the `off` build does, with
  every point's count equal to the fixture's `expected-counts.json`;
- f0's instrumented C to equal `emit-f0.expected.c` (`--update` rewrites
  it);
- a program with no hosted `main` to be refused (exit 9) with no map left.

Until the runtime can publish a profile (CV-115), `sv0 native-compile
--coverage=instrument` refuses to link (exit 7) and `--emit=c` is the way to
get instrumented C; the VM emitter refuses `instrument` (exit 9) until
`COVER_HIT` emission (CV-117).
