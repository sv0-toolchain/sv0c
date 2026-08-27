# Release signing workflow (NEX-056b, TEST-009)

Implements TEST-009's policy half
(`~/Documents/project-specs/sv0c-runtime-executable/SPEC.md`): "release
candidate SHALL be built from a signed, clean revision and retain
checksums/evidence." `scripts/native_exe_release_evidence.py` (NEX-056a)
assembles the *evidence* a release candidate carries; this document is the
policy for the one step that module deliberately does not automate: the
actual cryptographic signature.

## Why signing itself is not scripted

Real cryptographic signing needs a private key and, for a project this
size, a human decision about which revision is actually being released —
neither belongs in an automated Python module. Fabricating a signature
programmatically (or skipping the step and calling a build "signed"
without one) would be exactly the kind of "false verification claim" this
project's own risk register (spec §32) warns against for contract proofs,
applied here to release provenance instead.

## The workflow

1. Confirm `scripts/native_exe_release_evidence.py`'s evidence bundle for
   the candidate revision is well-formed and its `clean_tree` field is
   `true` (the module itself refuses to proceed on a dirty tree, raising
   `DirtyTreeError` — this is not a manual double-check, it's enforced).
2. Tag the exact revision the evidence bundle names, using a **signed**
   Git tag:

   ```console
   git tag -s v<version> -m "sv0c native-exe release v<version>"
   ```

   `-s` requires a configured GPG (or `gpg.format = ssh`) signing key;
   `git tag -v v<version>` later verifies the signature.
3. Attach the release-evidence JSON (from step 1) as a release artifact
   alongside the tag — the evidence bundle's own `revision` field must
   match the tag's target commit exactly; a mismatch means the evidence
   was computed against the wrong revision and step 1 must be redone.
4. Push the signed tag (`git push origin v<version>`) only after both the
   tag's signature and the evidence bundle's revision match are confirmed.

## What this does NOT cover

- **Key management** (generating, rotating, or storing the signing key)
  is an infrastructure/ops concern outside this driver's scope entirely.
- **Publishing** the signed artifact (a GitHub release, a package
  registry, etc.) is a separate step with its own review, not implied by
  signing alone.
- **CI-automated signing** (a CI job holding the signing key and tagging
  automatically) is explicitly not adopted here — the human confirmation
  in steps 1–2 is the deliberate control point; automating it away would
  remove the one place a human actually looks at what's being released.

## Retained evidence

Per spec §28.4 ("retained artifacts"), the release-evidence JSON bundle
(NEX-056a) plus the signed tag together constitute the retained evidence
for a stable release: revision, clean-tree confirmation, build record
(artifact digest, compiler identity, profile), benchmark report, and
reproducibility classification — everything a later auditor needs to
confirm what was actually shipped, without needing to trust an unsigned
claim.
