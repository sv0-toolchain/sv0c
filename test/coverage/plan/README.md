# Coverage planner tests (sv0cov CV-107)

`run_plan.py` exercises `lib/coverage_plan.sv0` through the compiler. The
planner runs after resolve/check and before lowering; the internal
`plan-dump` mode (`SV0_COVERAGE_REQUEST="plan-dump\n<ignored>"`) prints the
plan instead of C:

```text
source  <index> <logical path> <sha256>
entity  <index> function <qualified name> <source> <span> <owner>
point   <index> function_entry <entity> entry <source> <span>
```

`<span>` is start byte, end byte (exclusive), start line, start column, end
line, end column, file-relative, with LF lines and Unicode-scalar columns.

The script checks:

- the seven hand-reviewed sv0cov semantic fixtures
  (`sv0cov/tests/fixtures/semantic/*`): sources, function entities, and
  `function_entry` points equal their `expected-map.json`;
- synthetic programs anchored to their own text: inherent methods
  (`Type::name`), trait default methods (`Trait::name`), `#[extern_c]`
  declarations (not entities), `pub`, array parameters (a `;` inside the
  signature), parenthesized contracts, a `module` name prefix, CRLF, tabs,
  multibyte text, and project source order (sorted by logical path);
- that the native compiler and the native VM emitter, which share the
  planner, print identical plans;
- that coverage fails closed with exit 9 for an `include`d file (the
  compiled source is not the file's bytes), an unknown mode, and `map` /
  `instrument` (refused until map emission and hit placement land).

`./scripts/sv0 test` runs it. Trait impls and bodyless trait methods do not
pass the native checker yet, so they are not covered here.
