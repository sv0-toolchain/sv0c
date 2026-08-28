/*
 * sv0_runtime.c -- sv0 runtime support (non-inline functions)
 *
 * Most runtime functions are inline in the header. This file will
 * contain heap allocator, string, Vec, and Box implementations
 * as the C backend is developed.
 */
#include "sv0_runtime.h"
#include <dirent.h>
#include <sys/stat.h>

typedef struct {
  char **items;
  size_t len;
  size_t cap;
} sv0_path_list;

/* Self-contained C string duplicate: `strdup` is POSIX, not C99, so a strict
   `-std=c99` compile (as the corpus/acceptance harnesses use) leaves it undeclared
   on glibc. An implicit `int strdup()` then TRUNCATES the returned 64-bit pointer to
   32 bits -> a garbage `char*` -> SIGSEGV in the qsort comparator. macOS declares
   strdup even in c99 mode, hiding the bug. Use only C89 <string.h>/<stdlib.h> funcs. */
static char *sv0_dup_cstr(const char *s) {
  size_t n = strlen(s) + 1u;
  char *r = (char *)malloc(n);
  if (r)
    memcpy(r, s, n);
  return r;
}

static void sv0_path_list_push(sv0_path_list *pl, const char *s) {
  if (pl->len >= pl->cap) {
    pl->cap = pl->cap ? pl->cap * 2 : 16u;
    pl->items = (char **)realloc(pl->items, pl->cap * sizeof(char *));
    if (!pl->items)
      sv0_panic("read_dir: realloc failed");
  }
  pl->items[pl->len++] = sv0_dup_cstr(s);
  if (!pl->items[pl->len - 1])
    sv0_panic("read_dir: strdup failed");
}

static int sv0_path_cmp(const void *a, const void *b) {
  return strcmp(*(const char *const *)a, *(const char *const *)b);
}

static char *sv0_path_join2(const char *dir, const char *name) {
  size_t ld = strlen(dir), ln = strlen(name);
  int need_sep = (ld > 0 && dir[ld - 1] != '/');
  char *r = (char *)malloc(ld + ln + (need_sep ? 2u : 1u));
  if (!r)
    sv0_panic("read_dir: malloc failed");
  memcpy(r, dir, ld);
  if (need_sep) {
    r[ld] = '/';
    memcpy(r + ld + 1, name, ln + 1);
  } else {
    memcpy(r + ld, name, ln + 1);
  }
  return r;
}

static int sv0_endswith_sv0(const char *name) {
  size_t n = strlen(name);
  return n >= 4u && strcmp(name + n - 4u, ".sv0") == 0;
}

static void sv0_collect_sv0_r(const char *dir, sv0_path_list *pl) {
  DIR *d = opendir(dir);
  if (!d)
    sv0_panic("read_dir: opendir failed");
  for (;;) {
    struct dirent *e = readdir(d);
    if (!e)
      break;
    if (e->d_name[0] == '.')
      continue;
    char *full = sv0_path_join2(dir, e->d_name);
    struct stat st;
    if (stat(full, &st) != 0) {
      free(full);
      closedir(d);
      sv0_panic("read_dir: stat failed");
    }
    if (S_ISDIR(st.st_mode))
      sv0_collect_sv0_r(full, pl);
    else if (S_ISREG(st.st_mode) && sv0_endswith_sv0(e->d_name))
      sv0_path_list_push(pl, full);
    free(full);
  }
  closedir(d);
}

static char *sv0_join_paths_newlines(char **paths, size_t n) {
  if (n == 0u) {
    char *e = (char *)malloc(1u);
    if (!e)
      sv0_panic("read_dir: malloc failed");
    e[0] = '\0';
    return e;
  }
  size_t total = 0u;
  size_t i;
  for (i = 0u; i < n; i++)
    total += strlen(paths[i]) + 1u;
  char *out = (char *)malloc(total);
  if (!out)
    sv0_panic("read_dir: malloc failed");
  char *p = out;
  for (i = 0u; i < n; i++) {
    if (i > 0u)
      *p++ = '\n';
    const char *s = paths[i];
    size_t len = strlen(s);
    memcpy(p, s, len);
    p += len;
  }
  *p = '\0';
  return out;
}

const char *sv0_read_file(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f)
    sv0_panic("read_file: fopen failed");
  if (fseek(f, 0, SEEK_END) != 0) {
    fclose(f);
    sv0_panic("read_file: fseek failed");
  }
  long sz = ftell(f);
  if (sz < 0) {
    fclose(f);
    sv0_panic("read_file: ftell failed");
  }
  if (fseek(f, 0, SEEK_SET) != 0) {
    fclose(f);
    sv0_panic("read_file: rewind failed");
  }
  char *buf = (char *)malloc((size_t)sz + 1u);
  if (!buf) {
    fclose(f);
    sv0_panic("read_file: malloc failed");
  }
  if (sz > 0 && fread(buf, 1u, (size_t)sz, f) != (size_t)sz) {
    fclose(f);
    free(buf);
    sv0_panic("read_file: fread failed");
  }
  buf[sz] = '\0';
  fclose(f);
  return buf;
}

void sv0_write_file(const char *path, const char *contents) {
  FILE *f = fopen(path, "wb");
  if (!f)
    sv0_panic("write_file: fopen failed");
  size_t len = strlen(contents);
  if (len > 0u && fwrite(contents, 1u, len, f) != len) {
    fclose(f);
    sv0_panic("write_file: fwrite failed");
  }
  if (fclose(f) != 0)
    sv0_panic("write_file: fclose failed");
}

const char *sv0_getenv(const char *name) {
  const char *v = getenv(name);
  return v != NULL ? v : "";
}

const char *sv0_read_dir(const char *dir) {
  sv0_path_list pl = {NULL, 0u, 0u};
  sv0_collect_sv0_r(dir, &pl);
  qsort(pl.items, pl.len, sizeof(char *), sv0_path_cmp);
  char *joined = sv0_join_paths_newlines(pl.items, pl.len);
  size_t i;
  for (i = 0u; i < pl.len; i++)
    free(pl.items[i]);
  free(pl.items);
  return joined;
}
