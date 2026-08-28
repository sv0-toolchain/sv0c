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
#define sv0__box_deref_raw(h, T) (*(T *)(void *)&sv0_box_pool[(h)])

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

#endif /* SV0_RUNTIME_H */
