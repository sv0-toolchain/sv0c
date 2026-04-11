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
#define SV0_VEC_MAX 4096

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
 */
#define SV0_BOX_POOL_SIZE (64 * 1024)

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

#endif /* SV0_RUNTIME_H */
