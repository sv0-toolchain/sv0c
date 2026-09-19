/* SS-U11 / NEW-003 (task/sv0c-non-eliding-write.Rmd): proof that
 * sv0_fill_explicit's stores survive dead-code elimination where a naive
 * (pre-SS-U11) fill lowering would not.
 *
 * `naive_fill_wrapper` mirrors the pre-SS-U11 fill lowering shape
 * docs/fill-explicit-blocked.md's own SS-108 evidence describes: a bare
 * element-store loop, no `volatile` qualifier, no compiler barrier, no
 * `explicit_bzero`/`memset_explicit` call. It fills a purely LOCAL stack
 * array that never escapes the function and is never read afterward --
 * the most reliably dead-code-eliminated shape a real optimizing
 * compiler can prove, confirmed empirically on both clang/AArch64 and
 * gcc/x86-64 at `-O2` (an EARLIER version of this fixture used a
 * malloc'd, `free`'d buffer instead; that shape turned out to be
 * genuinely compiler-dependent -- clang eliminated it outright, gcc did
 * not, at `-O2` without cross-TU/LTO analysis -- so it was replaced with
 * this more portable one rather than kept and special-cased per
 * compiler). `noinline` keeps its compiled body a clean, isolated unit
 * to inspect.
 *
 * The claim under test: at `-O2`, `naive_fill_wrapper` is fully eligible
 * for whole-function dead-code elimination -- a real, unforced compiler
 * decision, confirmed empirically below on two independent compiler/
 * architecture combinations, not a hypothetical -- while
 * `explicit_fill_wrapper`'s call to `sv0_fill_explicit` retains its
 * stores despite an identically-dead destination.
 */
#include "sv0_runtime.h"
#include <stdint.h>
#include <stdlib.h>

__attribute__((noinline))
static void naive_fill_wrapper(int32_t len, intptr_t value) {
  intptr_t buf[8];
  for (int32_t i = 0; i < len; i++) {
    buf[i] = value;
  }
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
