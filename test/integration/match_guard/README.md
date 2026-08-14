# match_guard — match guards + top-level bind patterns (BH-13)

Regression for bug-hunt finding #13. A match arm with a guard (`x if x > 10 => …`)
was mis-lowered on BOTH C backends: the guard test was emitted BEFORE the pattern
variable was bound, so the guard referenced an undeclared identifier (invalid C on
SML; native additionally rejected it at check time — the native checker never bound
a top-level PatBind at all, so even a plain catch-all `x => …` arm failed to check).

Fixed: (1) the native checker binds a top-level bind pattern (`arm_pt == 1`) into
the env with the scrutinee's type; (2) both lowerings (SML `lowerMatchArms`, native
`lower_match_arms`) emit the pattern binding BEFORE the guard test, so the arm reads
`bind x; if (guard) { body } else { next-arm }`. Expected **exit 42**:
`classify(40)` = 42 (guard `x + 2`), `classify(8)` = 8 (catch-all bind `y`),
`42 + 8 - 8`. Runs on native `--project`, native single-file, and SML.
