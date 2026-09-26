# Coverage source-unit table tests (sv0cov CV-105)

`run_source_table.py` exercises `lib/coverage_source.sv0` on native and VM
against a Python reference:

- logical-path normalization (SPEC 10.1): root stripping, `.`/`..`, escapes,
  absolute paths, drive designators, backslashes, case and UTF-8 kept exact;
- per-unit SHA-256 digests of exact bytes (via `lib/sha256.sv0`);
- concat offsets using link's rule (files joined with one LF) and the concat
  digest itself;
- offset lookup to (unit, file-relative byte), including the joining newline;
- LF-split lines and Unicode-scalar columns, with offsets inside a UTF-8
  sequence rejected;
- projects: multi-file, same basename, multibyte paths and text, CRLF without
  a final newline, a normalized-path collision, and a root escape.

`./scripts/sv0 test` runs it. Project-mode name mangling
(`link_u15_mangle_source`) can insert text; until it records its edits,
callers compare `cov_src_concat` with the compiled concat and refuse coverage
when they differ.
