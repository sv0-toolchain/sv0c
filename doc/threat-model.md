# Threat model and security contact (SEC-012)

Implements SEC-012
(`~/Documents/project-specs/sv0c-runtime-executable/SPEC.md`): "Threat
model and security contact SHALL be reviewed for each stable release."
Red test: "Release checklist." Like
[`release-signing-workflow.md`](release-signing-workflow.md) (NEX-056b),
this stays a documented *policy* + a real, checked-against-code threat
model — not a script that fabricates a "reviewed" claim. Every mitigation
named below is checked against a real, already-tested module, not
aspirational.

## Assets

- **The compiled native executable** a user's `.sv0` source produces.
- **The host build environment** the driver runs in (the invoking user's
  filesystem, environment variables, and any C compiler it invokes).
- **The runtime bundle** (`sv0c/runtime/sv0_runtime.h`/`.c` and their ABI
  manifest) shipped with an installed compiler.
- **Build provenance artifacts** (build records, release evidence) a
  downstream consumer might trust as an integrity/authenticity signal.

## Threat actors and vectors, and this project's actual mitigation

| Actor / vector | Real mitigation | Where |
|---|---|---|
| A hostile `.sv0` source or project tree (malicious filenames: spaces, Unicode, `;`, `$()`, backticks, leading hyphen) attempting shell injection via a filename that becomes a subprocess argument | Every subprocess invocation in this driver uses `shell=False` with an argv list, never a shell string — a hostile filename can never be reinterpreted as a shell command | `native_exe_subprocess.run_argv`, exercised end-to-end by `native_exe_hostile_paths.py` (6 hostile shapes, SEC-001/PORT-004) |
| A hostile output-path symlink attempting to redirect a build's final write to an attacker-controlled location | Publication validates the destination is not a symlink/directory/non-regular file before ever writing, and validates again immediately before the atomic rename | `native_exe_publish.validate_temp_output`/`publish_atomically`, `native_exe_output_path.validate_output_path` (ART-002…004, SEC-003) |
| A local attacker racing a shared scratch directory or guessing its path | Scratch directories are created with `tempfile.mkdtemp` (unpredictable leaf) and owner-only permissions; cleanup only ever removes the exact path created, never a glob/prefix scan | `native_exe_scratch.ScratchDir` (ART-009/010, SEC-005) |
| An attacker-controlled or accidentally-inherited environment variable redirecting the host compiler's include/library search path | The child compiler environment is built from an explicit allowlist (`PATH`/`LANG`/`LC_ALL`/`TMPDIR`/`SDKROOT`/`DEVELOPER_DIR` only) — everything else, including `CPATH`/`LD_LIBRARY_PATH`/proxy variables, is stripped even if the parent process set it | `native_exe_env.sanitized_child_env` (SEC-006, TOOL-007/008; also the real mitigation cited for SEC-008's network-deny concern) |
| A tampered or version-skewed runtime bundle (`sv0_runtime.h`/`.c`) silently miscompiling a program | Content hashes are verified against a manifest before every build; a mismatch fails closed with a diagnostic, never a "best effort" compile against untrusted/skewed sources | `native_exe_runtime_manifest.verify_manifest` (RT-004/005, SEC-010) |
| A build reading a runtime/config file from the current working directory or an untrusted parent directory instead of the real installation | Runtime resolution is always relative to the compiler's own installed location (`Path(__file__).resolve()`), never `cwd`, `CPATH`, or a project-relative search | `native_exe_runtime.resolve_runtime_dir` (RT-001…003, SEC-002) |
| Supply-chain risk in the host C compiler itself (a compromised or broken `cc`) | Out of scope for this driver to defend against directly — `native_exe_cc_probe.probe_compiler` verifies the selected compiler can actually compile+link+run a minimal hosted program (catches brokenness, not malice), and `native_exe_supported_compilers.py` (TOOL-013) declares which compiler families/versions this release claims support | `native_exe_cc_probe.py`, `native_exe_supported_compilers.py` |
| Network exfiltration or a build silently fetching remote content | No networking code (`socket`/`urllib`/`requests`/etc.) exists anywhere in the driver, confirmed by direct code review; the sanitized child environment also strips any proxy-redirection variable | `native_exe_env.py`; noted honestly in the checklist (`SEC-008` row) as absence + environment hardening, not an active network-deny test |

## Explicitly out of scope

- Protecting against a user who runs this driver with a source file they
  do not trust *and* deliberately disables its own safety checks (e.g.
  calling internal modules directly instead of the CLI entry point). This
  driver protects the *build*, not a user's own decision to run untrusted
  code they compiled.
- Denial-of-service via resource exhaustion (huge inputs, pathological
  compile times) — not addressed by this threat model; no claim is made
  about it.
- Anything upstream of this repository: the host operating system, the
  C compiler's own internal security, or the physical machine's
  integrity.

## Security contact

This project does not yet have a dedicated, published security contact
(e.g. a named maintainer email or a `SECURITY.md` with a disclosure
policy) — stated honestly rather than inventing one. Until one is
designated, the recommended channel for a security-relevant report is a
**private GitHub Security Advisory** on this repository (GitHub's
built-in mechanism for a confidential report, visible only to
maintainers until resolved), rather than a public issue.

## Review cadence

Per SEC-012's own wording ("reviewed for each stable release"), this
document SHALL be re-read against the actual code at each stable
(R1+) release, and updated if a mitigation's implementation has changed
or a new asset/vector has been introduced — not signed off once and
left stale. `scripts/native_exe_release_evidence.py`'s evidence bundle
does not currently enforce this review mechanically (unlike GOV-007's
`known_conflicts` gate, this is a documented policy step, unenforced by
code, matching this project's own precedent for TEST-009/NEX-056b: real
signing is a human decision, not something to fabricate a "reviewed"
flag for).
