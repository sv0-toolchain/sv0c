/* SS-U11 / NEW-003 (task/sv0c-non-eliding-write.Rmd): proof that
 * sv0_fill_explicit's stores survive dead-code elimination where a naive
 * (pre-SS-U11) fill lowering would not.
 *
 * `naive_fill`/`naive_fill_wrapper` mirror EXACTLY what sv0c's C backend
 * would emit for a fill without the SS-U11 intrinsic: a bare element-store
 * loop, no `volatile` qualifier, no compiler barrier, no
 * `explicit_bzero`/`memset_explicit` call -- the shape
 * docs/fill-explicit-blocked.md's own SS-108 evidence described. `noinline`
 * keeps each wrapper's compiled body a clean, isolated unit for
 * scripts/verify_fill_explicit_dce.py to inspect in the -O2 assembly.
 *
 * The claim under test: at -O2, `naive_fill_wrapper` (whose only effect is
 * writing to a buffer immediately freed and never read, through a raw
 * pointer with no intervening indirection) is fully eligible for
 * whole-call dead-code elimination -- a real, unforced compiler decision,
 * confirmed empirically below, not a hypothetical -- while
 * `explicit_fill_wrapper`'s call to `sv0_fill_explicit` retains its
 * stores despite an identically-dead destination.
 *
 * HONEST CAVEAT (found while building this fixture, worth keeping): on
 * the compiler this was verified against, `sv0_fill_explicit`'s writes
 * survive for TWO independent reasons layered together, not one -- (1)
 * the `volatile`-qualified pointer, the portable, standards-mandated
 * guarantee this intrinsic exists to provide, and (2) incidentally, the
 * write already goes through `sv0_slice_table` (a global array every
 * sv0 slice/Vec handle is represented through), which this compiler's
 * escape analysis could not see through to prove the store dead even
 * with `volatile` removed (checked by hand). That does NOT make
 * `volatile` redundant here -- a different compiler, a future
 * whole-program/LTO analysis pass, or a change to how slices are
 * represented could plausibly see through the indirection someday, and
 * `volatile` is what keeps the guarantee true regardless. It does mean
 * this fixture's naive-vs-explicit comparison is against a genuinely
 * unprotected RAW POINTER (matching the exact vulnerability class
 * docs/fill-explicit-blocked.md's own SS-108 evidence describes: "bare
 * element stores... no volatile qualifier, no compiler barrier"), not a
 * claim that removing sv0_fill_explicit's own `volatile` would
 * presently produce an observable regression on every compiler.
 */
#include "sv0_runtime.h"
#include <stdint.h>
#include <stdlib.h>

__attribute__((noinline))
static void naive_fill(intptr_t *data, int32_t len, intptr_t value) {
  for (int32_t i = 0; i < len; i++) {
    data[i] = value;
  }
}

__attribute__((noinline))
static void naive_fill_wrapper(int32_t len, intptr_t value) {
  intptr_t *buf = malloc(sizeof(intptr_t) * (size_t)len);
  if (!buf)
    return;
  naive_fill(buf, len, value);
  free(buf);
}

__attribute__((noinline))
static void explicit_fill_wrapper(int32_t len, int32_t value) {
  intptr_t *buf = malloc(sizeof(intptr_t) * (size_t)len);
  if (!buf)
    return;
  int32_t sh = sv0_slice_intern(buf, len);
  sv0_fill_explicit(sh, value);
  free(buf);
}

int main(void) {
  naive_fill_wrapper(8, 0x41);
  explicit_fill_wrapper(8, 0x41);
  return 0;
}
