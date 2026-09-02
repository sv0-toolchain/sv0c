/*
 * sv0_runtime.h -- sv0 runtime support for C backend
 *
 * Provides panic, contract checking, string/vec/box operations,
 * and memory management primitives used by sv0c-generated C code.
 */
#ifndef SV0_RUNTIME_H
#define SV0_RUNTIME_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* hosted entry-point initialization (native executable ABI; NEX-016 of
   ~/Documents/project-specs/sv0c-runtime-executable/SPEC.md).
   Currently a no-op -- its slot is reserved so future runtime
   initialization does not require changing every executable entry
   contract (spec S14.3, S15.5). */
static inline void sv0_runtime_init(int argc, char **argv) {
  (void)argc;
  (void)argv;
}

/* panic and assertion */
static inline void sv0_panic(const char *msg) {
  fprintf(stderr, "sv0 panic: %s\n", msg);
  exit(1);
}

static inline void sv0_assert(bool cond, const char *msg) {
  if (!cond)
    sv0_panic(msg);
}

/* contract checking (runtime mode) */
static inline void sv0_requires(bool cond, const char *fn_name) {
  if (!cond) {
    fprintf(stderr, "sv0 contract violation: requires failed in %s\n", fn_name);
    exit(1);
  }
}

static inline void sv0_ensures(bool cond, const char *fn_name) {
  if (!cond) {
    fprintf(stderr, "sv0 contract violation: ensures failed in %s\n", fn_name);
    exit(1);
  }
}

/* Phase 9: pointer inequality (runtime approximation of no_alias) */
static inline bool sv0_no_alias(const void *a, const void *b) { return a != b; }

/* I/O (Phase 6): single string argument, newline appended */
static inline void sv0_println(const char *s) { printf("%s\n", s); }

/* String API (T0-1): intrinsic string operations */
static inline int sv0_string_len(const char *s) { return (int)strlen(s); }

static inline int sv0_string_eq(const char *a, const char *b) {
  return strcmp(a, b) == 0;
}

static inline const char *sv0_string_concat(const char *a, const char *b) {
  size_t la = strlen(a), lb = strlen(b);
  char *r = (char *)malloc(la + lb + 1);
  memcpy(r, a, la);
  memcpy(r + la, b, lb + 1);
  return r;
}

static inline int sv0_string_char_at(const char *s, int i) {
  return (int)(unsigned char)s[i];
}

static inline const char *sv0_string_substr(const char *s, int start, int len) {
  char *r = (char *)malloc((size_t)len + 1);
  memcpy(r, s + start, (size_t)len);
  r[len] = '\0';
  return r;
}

/* Owned `string` — length-bearing (SS-U02b, sv0-strings Track U).
 *
 * `sv0doc/type-system/rules.md` §1.3.1 is the normative *target* ABI: a
 * by-value three-field record `{ data: non-null *u8, len: usize, cap: usize }`
 * (`cap >= len`), content = exactly `len` bytes of UTF-8, an embedded `\0` is
 * content, no implicit terminator. This bootstrap runtime realizes it the
 * same way it realizes `Vec<T>` and slices — indirectly, through an `int`
 * handle into `sv0_str_table`, whose entries hold exactly that triple. A real
 * by-value string lands with a non-bootstrap backend.
 *
 * These `sv0_str_*` helpers are the length-based replacements for the
 * `strlen`/`strcmp`-based `sv0_string_*` above (SS-009's embedded-NUL red
 * corpus fails on those): length and comparison never scan for a terminator
 * (UP-002), and byte access / substring bounds-check before any read (UP-003)
 * and fail closed via `sv0_panic`. Allocation failure and `len + other.len`
 * overflow in concat are checked before the value is observable, with the
 * inputs left unchanged. The compiler front end still emits the old API; the
 * migration (literals carrying a decoded byte length, `ast_ty_to_c_string`
 * `string -> sv0_str`, `sv0_println`) is the rest of SS-U02b. */
#define SV0_STR_MAX 262144

typedef struct {
  uint8_t *data;
  int32_t len;
  int32_t cap;
} sv0_str;

static sv0_str sv0_str_table[SV0_STR_MAX];
static int32_t sv0_str_count = 0;
static uint8_t sv0_str_empty_base[1] = {0}; /* non-null base for len == 0 */

static inline int32_t sv0_str_slot(void) {
  if (sv0_str_count >= SV0_STR_MAX)
    sv0_panic("string: too many strings");
  return sv0_str_count++;
}

/* Store an already-owned heap buffer (or set up the empty string). */
static inline int32_t sv0_str_adopt(uint8_t *buf, int32_t len) {
  int32_t h = sv0_str_slot();
  if (len <= 0) {
    if (buf)
      free(buf);
    sv0_str_table[h].data = sv0_str_empty_base;
    sv0_str_table[h].len = 0;
    sv0_str_table[h].cap = 0;
    return h;
  }
  sv0_str_table[h].data = buf;
  sv0_str_table[h].len = len;
  sv0_str_table[h].cap = len;
  return h;
}

static inline int32_t sv0_str_intern(const uint8_t *bytes, int32_t len) {
  if (len <= 0)
    return sv0_str_adopt((uint8_t *)0, 0);
  uint8_t *buf = (uint8_t *)malloc((size_t)len);
  if (!buf)
    sv0_panic("string: allocation failed");
  memcpy(buf, bytes, (size_t)len);
  return sv0_str_adopt(buf, len);
}

/* String literal: raw bytes with a compile-time length (embedded `\0`
   included) — never `strlen`. */
static inline int32_t sv0_str_lit(const char *bytes, int32_t len) {
  return sv0_str_intern((const uint8_t *)bytes, len);
}

static inline int32_t sv0_str_len(int32_t h) { return sv0_str_table[h].len; }

static inline int sv0_str_eq(int32_t a, int32_t b) {
  sv0_str x = sv0_str_table[a];
  sv0_str y = sv0_str_table[b];
  if (x.len != y.len)
    return 0;
  return memcmp(x.data, y.data, (size_t)x.len) == 0;
}

static inline int32_t sv0_str_concat(int32_t a, int32_t b) {
  sv0_str x = sv0_str_table[a];
  sv0_str y = sv0_str_table[b];
  if (x.len < 0 || y.len < 0 || x.len > 0x7fffffff - y.len)
    sv0_panic("string: length overflow in concat");
  int32_t n = x.len + y.len;
  if (n == 0)
    return sv0_str_adopt((uint8_t *)0, 0);
  uint8_t *buf = (uint8_t *)malloc((size_t)n);
  if (!buf)
    sv0_panic("string: allocation failed");
  memcpy(buf, x.data, (size_t)x.len);
  memcpy(buf + x.len, y.data, (size_t)y.len);
  return sv0_str_adopt(buf, n);
}

static inline int sv0_str_char_at(int32_t h, int32_t i) {
  sv0_str s = sv0_str_table[h];
  if (i < 0 || i >= s.len)
    sv0_panic("string: index out of bounds");
  return (int)s.data[i];
}

static inline int32_t sv0_str_substr(int32_t h, int32_t start, int32_t len) {
  sv0_str s = sv0_str_table[h];
  if (start < 0 || len < 0 || start > s.len - len)
    sv0_panic("string: substring out of bounds");
  return sv0_str_intern(s.data + start, len);
}

static inline void sv0_str_println(int32_t h) {
  sv0_str s = sv0_str_table[h];
  if (s.len > 0)
    fwrite(s.data, 1u, (size_t)s.len, stdout);
  fputc('\n', stdout);
}

/* Vec<T> (T0-4): handle-based dynamic array for bootstrap.
 * Each vec_new() returns an int handle into a global table.
 * Elements are stored as intptr_t (word-sized); works for i32, bool, pointers.
 */
#define SV0_VEC_MAX 262144

static struct {
  intptr_t *data;
  int32_t len;
  int32_t cap;
} sv0_vec_table[SV0_VEC_MAX];

static int32_t sv0_vec_count = 0;

static inline int32_t sv0_vec_new(void) {
  if (sv0_vec_count >= SV0_VEC_MAX)
    sv0_panic("vec: too many vectors");
  int32_t h = sv0_vec_count++;
  sv0_vec_table[h].cap = 8;
  sv0_vec_table[h].len = 0;
  sv0_vec_table[h].data = (intptr_t *)malloc(8 * sizeof(intptr_t));
  return h;
}

static inline void sv0_vec_push(int32_t h, intptr_t elem) {
  if (sv0_vec_table[h].len >= sv0_vec_table[h].cap) {
    sv0_vec_table[h].cap *= 2;
    sv0_vec_table[h].data = (intptr_t *)realloc(
        sv0_vec_table[h].data, (size_t)sv0_vec_table[h].cap * sizeof(intptr_t));
  }
  sv0_vec_table[h].data[sv0_vec_table[h].len++] = elem;
}

static inline int32_t sv0_vec_len(int32_t h) { return sv0_vec_table[h].len; }

static inline intptr_t sv0_vec_get(int32_t h, int32_t idx) {
  if (idx < 0 || idx >= sv0_vec_table[h].len)
    sv0_panic("vec: index out of bounds");
  return sv0_vec_table[h].data[idx];
}

static inline void sv0_vec_set(int32_t h, int32_t idx, intptr_t val) {
  if (idx < 0 || idx >= sv0_vec_table[h].len)
    sv0_panic("vec: index out of bounds");
  sv0_vec_table[h].data[idx] = val;
}

/* Slice `&[T]` / `&mut [T]` (SS-U03b, sv0-strings Track U).
 *
 * `sv0doc/type-system/rules.md` §2.2.1 (`T-SLICE-ABI-001`) is the normative
 * *target* ABI: a by-value two-word record `{ data: non-null *T, len }`,
 * `data` addressing element 0 of a contiguous run, `len` the element count.
 * This bootstrap runtime realizes it the same way it realizes `Vec<T>` —
 * indirectly, through a handle into a side table whose entries hold exactly
 * that `{ data, len }` pair in that field order. A real by-value slice lands
 * with a non-bootstrap backend; here a slice is an `int` handle so it needs
 * no new C type in codegen and no per-local type tracking in lowering.
 *
 * The handle carries `SV0_SLICE_TAG` in a high bit so `sv0_idx_get` /
 * `sv0_idx_set` can accept either a slice handle or a plain Vec/array handle
 * (`e[i]` sugar lowers to those and must work for both). `data` points
 * straight into the source Vec's buffer — no copy, no realloc; the borrow
 * rules (enforced pre-lowering, SS-U03d) keep the source pinned so `data`
 * cannot dangle across a `sv0_vec_push` realloc.
 *
 * `sv0_slice_from_vec` range-checks `0 <= lo <= hi <= len` at the slicing
 * site (never an out-of-range pair); `sv0_slice_get` / `sv0_slice_set`
 * bounds-check `i < len` BEFORE touching memory (UP-003) — the same
 * fail-closed panic class the VM backend must mirror (SS-U03c). */
typedef struct {
  intptr_t *data;
  int32_t len;
} sv0_slice;

#define SV0_SLICE_MAX 262144
#define SV0_SLICE_TAG 0x40000000

static sv0_slice sv0_slice_table[SV0_SLICE_MAX];
static int32_t sv0_slice_count = 0;

static inline int32_t sv0_slice_intern(intptr_t *data, int32_t len) {
  if (sv0_slice_count >= SV0_SLICE_MAX)
    sv0_panic("slice: too many slices");
  int32_t s = sv0_slice_count++;
  sv0_slice_table[s].data = data;
  sv0_slice_table[s].len = len;
  return s | SV0_SLICE_TAG;
}

/* Base pointer + element count of a Vec/array handle OR a slice handle —
   so `&e[..]` / `&e[a..b]` work whether `e` is a collection or already a
   slice (re-slicing), without lowering needing to know which. */
static inline intptr_t *sv0_view_data(int32_t h) {
  if (h & SV0_SLICE_TAG)
    return sv0_slice_table[h & ~SV0_SLICE_TAG].data;
  return sv0_vec_table[h].data;
}

static inline int32_t sv0_view_len(int32_t h) {
  if (h & SV0_SLICE_TAG)
    return sv0_slice_table[h & ~SV0_SLICE_TAG].len;
  return sv0_vec_table[h].len;
}

static inline int32_t sv0_slice_full_vec(int32_t h) {
  return sv0_slice_intern(sv0_view_data(h), sv0_view_len(h));
}

static inline int32_t sv0_slice_from_vec(int32_t h, int32_t lo, int32_t hi) {
  int32_t n = sv0_view_len(h);
  if (lo < 0 || hi < lo || hi > n)
    sv0_panic("slice: range out of bounds");
  return sv0_slice_intern(sv0_view_data(h) + lo, hi - lo);
}

static inline int32_t sv0_slice_subslice(int32_t sh, int32_t lo, int32_t hi) {
  return sv0_slice_from_vec(sh, lo, hi);
}

static inline int32_t sv0_slice_len(int32_t sh) {
  return sv0_slice_table[sh & ~SV0_SLICE_TAG].len;
}

static inline intptr_t sv0_slice_get(int32_t sh, int32_t idx) {
  sv0_slice s = sv0_slice_table[sh & ~SV0_SLICE_TAG];
  if (idx < 0 || idx >= s.len)
    sv0_panic("slice: index out of bounds");
  return s.data[idx];
}

static inline void sv0_slice_set(int32_t sh, int32_t idx, intptr_t val) {
  sv0_slice s = sv0_slice_table[sh & ~SV0_SLICE_TAG];
  if (idx < 0 || idx >= s.len)
    sv0_panic("slice: index out of bounds");
  s.data[idx] = val;
}

/* `e[i]` / `e[i] = v` sugar: `e` may be a Vec/array handle or a slice
   handle — dispatch on the tag bit. Explicit `vec_get`/`vec_set` builtin
   calls keep going straight to `sv0_vec_*`. */
static inline intptr_t sv0_idx_get(int32_t h, int32_t idx) {
  if (h & SV0_SLICE_TAG)
    return sv0_slice_get(h, idx);
  return sv0_vec_get(h, idx);
}

static inline void sv0_idx_set(int32_t h, int32_t idx, intptr_t val) {
  if (h & SV0_SLICE_TAG)
    sv0_slice_set(h, idx, val);
  else
    sv0_vec_set(h, idx, val);
}

/* Box<T> (T0-6): handle-based heap indirection for recursive types.
 * Each box_alloc(nwords) returns a handle into a flat word pool.
 * Used for recursive enum variants like Box<Expr> in AST definitions.
 * Boxes are never freed (arena-style), so the pool must fit a whole run. The
 * native-compose mega-TU compiler box_new's every IR instruction/value while
 * lowering a module, so compiling a large library module (parser/checker/
 * lowering, thousands of lines) needs millions of words; sized generously — it
 * is demand-paged BSS, so unused capacity costs nothing.
 */
#define SV0_BOX_POOL_SIZE (16 * 1024 * 1024)

static intptr_t sv0_box_pool[SV0_BOX_POOL_SIZE];
static int32_t sv0_box_next = 0;

static inline int32_t sv0_box_alloc(int32_t nwords) {
  if (sv0_box_next + nwords > SV0_BOX_POOL_SIZE)
    sv0_panic("box: pool exhausted");
  int32_t h = sv0_box_next;
  sv0_box_next += nwords;
  return h;
}

static inline void sv0_box_store(int32_t h, int32_t off, intptr_t val) {
  sv0_box_pool[h + off] = val;
}

static inline intptr_t sv0_box_load(int32_t h, int32_t off) {
  return sv0_box_pool[h + off];
}

/* sv0__box_new_raw / sv0__box_deref_raw: generic box<T> helpers.
 * box_new(val) stores val's bytes in the pool, returns handle (int).
 * box_deref(h, T) reads bytes back as type T.
 * Uses GCC/Clang __typeof__ + statement expressions to handle rvalue args. */
static inline int32_t sv0__box_new_raw_impl(const void* data, int nbytes) {
  int nwords = (nbytes + (int)sizeof(intptr_t) - 1) / (int)sizeof(intptr_t);
  int32_t h = sv0_box_alloc(nwords);
  memcpy(&sv0_box_pool[h], data, (size_t)nbytes);
  return h;
}
#define sv0__box_new_raw(val) __extension__({ \
  __typeof__(val) _sv0_box_tmp = (val); \
  sv0__box_new_raw_impl(&_sv0_box_tmp, (int)sizeof(_sv0_box_tmp)); \
})
/* KC-004 (native-executable-ub-audit.md, Site 1): the original definition
 * `(*(T *)(void *)&sv0_box_pool[(h)])` reads through a pointer whose
 * pointed-to type (T) generally differs from the pool's declared element
 * type (intptr_t) — a real strict-aliasing violation under C's effective-type
 * rules, previously mitigated only by building with `-fno-strict-aliasing`.
 * Fixed for real here: every emission site (megaTU-main.sv0's Call codegen,
 * confirmed by direct reading) uses this macro's expansion strictly as an
 * rvalue -- `T dst = sv0__box_deref_raw(h, T);` -- never as an assignment
 * target, so rewriting it as a statement expression that `memcpy`s the bytes
 * into a same-typed local (well-defined regardless of strict-aliasing, per
 * the same reasoning already used for sv0__box_new_raw above) is a drop-in,
 * behavior-preserving replacement that needs no `-fno-strict-aliasing`. */
#define sv0__box_deref_raw(h, T) __extension__({ \
  T _sv0_box_deref_tmp; \
  memcpy(&_sv0_box_deref_tmp, &sv0_box_pool[(h)], sizeof(T)); \
  _sv0_box_deref_tmp; \
})

/* Host filesystem (T0-8 / M3 G2): see sv0doc/compiler/bootstrap-host-io.md */
const char *sv0_read_file(const char *path);
void sv0_write_file(const char *path, const char *contents);
const char *sv0_read_dir(const char *dir);

/* getenv (NEX-055c, REL-004's minimal fix): returns the named environment
   variable's value, or "" if unset -- NEVER panics on absence, unlike
   sv0_read_file's panic-on-missing-file contract, since a caller (the
   reentrant core-compiler entry point) needs to distinguish "no
   per-invocation channel present" from a real I/O failure. The returned
   pointer is owned by the process environment block (libc getenv's own
   contract), not malloc'd -- unlike every other string-returning host
   builtin here, this one is never freed by the caller. */
const char *sv0_getenv(const char *name);

/* write_bytes: raw binary write of a Vec<i32>'s byte values (low 8 bits each),
   NUL-safe (writes the vec length, not strlen) — for .sv0b bytecode emission.
   MUST be static inline in this header (not a .c function): the vec table is
   `static` (per-TU), so a .c definition would read the runtime.c TU's empty
   table instead of the emitted program's. Here it compiles into the caller's TU. */
static inline void sv0_write_bytes(const char *path, int32_t vec) {
  FILE *f = fopen(path, "wb");
  if (!f)
    sv0_panic("write_bytes: fopen failed");
  int32_t n = sv0_vec_len(vec);
  int32_t i;
  for (i = 0; i < n; i++) {
    unsigned char b = (unsigned char)(sv0_vec_get(vec, i) & 0xFF);
    if (fwrite(&b, 1u, 1u, f) != 1u) {
      fclose(f);
      sv0_panic("write_bytes: fwrite failed");
    }
  }
  if (fclose(f) != 0)
    sv0_panic("write_bytes: fclose failed");
}

/* Host-I/O shims for the length-bearing `sv0_str` world (SS-U02b p0b).
 * The underlying host builtins take/return NUL-terminated `const char *`;
 * these wrap them so codegen can stay uniform once `string` becomes an
 * `sv0_str` handle. Host paths / dir listings / env values are NUL-free,
 * so `strlen` on them is exact. Unreferenced until the SS-U02b codegen
 * flip (p1/p2) retargets the emitted names. */

/* NUL-terminated copy of a handle's bytes (for passing to a `const char *`
   host API). The handle's own bytes may contain interior NUL; a path that
   does is simply not found by the OS -- same as today. Caller owns the
   returned buffer. */
static inline char *sv0_str_cstr(int32_t h) {
  sv0_str s = sv0_str_table[h];
  char *r = (char *)malloc((size_t)s.len + 1u);
  if (!r)
    sv0_panic("string: allocation failed");
  if (s.len > 0)
    memcpy(r, s.data, (size_t)s.len);
  r[s.len] = '\0';
  return r;
}

/* `const char *` (NUL-terminated) -> owned sv0_str handle. */
static inline int32_t sv0_str_from_cstr(const char *s) {
  return sv0_str_intern((const uint8_t *)s, (int32_t)strlen(s));
}

/* NB: sv0_read_file returns a `const char *` with no length, so file content
   is measured with strlen -- an interior NUL truncates the result. That is
   sv0_read_file's own ABI limitation, not new here; the self-hosted compiler
   only reads NUL-free `.sv0` source. A length-returning read_file is a
   separate host-ABI change, out of SS-U02b's scope (the `string` *type*). */
static inline int32_t sv0_str_read_file(int32_t path_h) {
  char *p = sv0_str_cstr(path_h);
  const char *body = sv0_read_file(p);
  free(p);
  return sv0_str_from_cstr(body);
}

static inline void sv0_str_write_file(int32_t path_h, int32_t contents_h) {
  char *p = sv0_str_cstr(path_h);
  char *c = sv0_str_cstr(contents_h); /* NUL-safe: sv0_write_file uses strlen,
                                         so interior NUL would truncate -- same
                                         limitation as the pre-sv0_str backend;
                                         the .sv0b path uses sv0_write_bytes. */
  sv0_write_file(p, c);
  free(p);
  free(c);
}

static inline int32_t sv0_str_read_dir(int32_t dir_h) {
  char *d = sv0_str_cstr(dir_h);
  const char *listing = sv0_read_dir(d);
  free(d);
  return sv0_str_from_cstr(listing);
}

static inline int32_t sv0_str_getenv(int32_t name_h) {
  char *n = sv0_str_cstr(name_h);
  const char *v = sv0_getenv(n); /* "" when unset -- never NULL (its contract) */
  free(n);
  return sv0_str_from_cstr(v);
}

static inline void sv0_str_write_bytes(int32_t path_h, int32_t vec) {
  char *p = sv0_str_cstr(path_h);
  sv0_write_bytes(p, vec);
  free(p);
}

#endif /* SV0_RUNTIME_H */
