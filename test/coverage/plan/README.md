# Coverage planner and map tests (sv0cov CV-107..CV-111)

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
  compiled source is not the file's bytes), an unknown mode, `map` without
  its target and identity lines, and `instrument` (refused until hit
  placement lands).

`./scripts/sv0 test` runs it.

`run_determinism.py` (CV-111) builds the map of each fixture and of
`constructs.sv0` five times with the native compiler and once with the VM
emitter, from different working directories, map paths, and environment
noise, and requires identical bytes. It also requires `map` mode's generated
C and `.sv0b` to be byte-identical to an `off` build (map mode adds no hit
operations).
