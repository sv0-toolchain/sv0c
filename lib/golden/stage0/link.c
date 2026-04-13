#include "sv0_runtime.h"

static int is_sv0(const char* name);
static int is_hidden(const char* name);
static int find_last_slash(const char* path);
static int find_last_dot(const char* s, int start);
static const char* file_stem(const char* path);
static const char* mangle(const char* mod_id, const char* x);
static int is_top_defining(int tag);
static int collect_tops_count(int item_tags);
static int is_link_directive(int tag);
static int strip_directives_count(int item_tags);
static int has_module_prefix(int first_tag);
static int in_tops(int tops, int name);
static int path_seg_needs_mangle(int tops, int name);
static int test_is_sv0(void);
static int test_is_hidden(void);
static int test_file_stem(void);
static int test_mangle(void);
static int test_is_top_defining(void);
static int test_collect_tops_count(void);
static int test_is_link_directive(void);
static int test_strip_directives_count(void);
static int test_has_module_prefix(void);
static int test_in_tops(void);
static int test_path_seg_needs_mangle(void);

static int is_sv0(const char* name) {
  int _sv0t0 = sv0_string_len(name);
  int len = _sv0t0;
  if ((len < 4)) {
    return 0;
  } else {
  }
  int _sv0t1 = (len - 4);
  const char* _sv0t2 = sv0_string_substr(name, _sv0t1, 4);
  const char* suffix;
  suffix = _sv0t2;
  int _sv0t3 = sv0_string_eq(suffix, ".sv0");
  return _sv0t3;
}

static int is_hidden(const char* name) {
  int _sv0t0 = sv0_string_len(name);
  int len = _sv0t0;
  if ((len == 0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_string_char_at(name, 0);
  int first = _sv0t1;
  int _sv0t2 = (first == 46);
  return _sv0t2;
}

static int find_last_slash(const char* path) {
  int _sv0t0 = sv0_string_len(path);
  int len = _sv0t0;
  int i = (len - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_string_char_at(path, i);
    int c = _sv0t1;
    if ((c == 47)) {
      return i;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int find_last_dot(const char* s, int start) {
  int _sv0t0 = sv0_string_len(s);
  int len = _sv0t0;
  int i = (len - 1);
  while ((i >= start)) {
    int _sv0t1 = sv0_string_char_at(s, i);
    int c = _sv0t1;
    if ((c == 46)) {
      return i;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static const char* file_stem(const char* path) {
  int _sv0t0 = sv0_string_len(path);
  int len = _sv0t0;
  int _sv0t1 = find_last_slash(path);
  int slash = _sv0t1;
  int base_start = (slash + 1);
  int _sv0t2 = find_last_dot(path, base_start);
  int dot = _sv0t2;
  if ((dot <= base_start)) {
    int _sv0t3 = (len - base_start);
    const char* _sv0t4 = sv0_string_substr(path, base_start, _sv0t3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = (dot - base_start);
  const char* _sv0t6 = sv0_string_substr(path, base_start, _sv0t5);
  return _sv0t6;
}

static const char* mangle(const char* mod_id, const char* x) {
  const char* _sv0t0 = sv0_string_concat(mod_id, "__");
  const char* a;
  a = _sv0t0;
  const char* _sv0t1 = sv0_string_concat(a, x);
  return _sv0t1;
}

static int is_top_defining(int tag) {
  if ((tag == 0)) {
    return 1;
  } else {
  }
  if ((tag == 1)) {
    return 1;
  } else {
  }
  if ((tag == 2)) {
    return 1;
  } else {
  }
  if ((tag == 3)) {
    return 1;
  } else {
  }
  if ((tag == 7)) {
    return 1;
  } else {
  }
  return 0;
}

static int collect_tops_count(int item_tags) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int len = _sv0t0;
  int count = 0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = is_top_defining(tag);
    if (_sv0t2) {
      count = (count + 1);
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int is_link_directive(int tag) {
  if ((tag == 5)) {
    return 1;
  } else {
  }
  if ((tag == 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int strip_directives_count(int item_tags) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int len = _sv0t0;
  int count = 0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = is_link_directive(tag);
    if ((_sv0t2 != 1)) {
      count = (count + 1);
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int has_module_prefix(int first_tag) {
  int _sv0t0 = (first_tag == 6);
  return _sv0t0;
}

static int in_tops(int tops, int name) {
  int _sv0t0 = sv0_vec_len(tops);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(tops, i);
    int t = _sv0t1;
    if ((t == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int path_seg_needs_mangle(int tops, int name) {
  int _sv0t0 = in_tops(tops, name);
  return _sv0t0;
}

static int test_is_sv0(void) {
  int _sv0t0 = is_sv0("hello.sv0");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_sv0("parser.sv0");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_sv0(".sv0");
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_sv0("hello.txt");
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_sv0("sv0");
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_sv0("");
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_is_hidden(void) {
  int _sv0t0 = is_hidden(".git");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_hidden(".");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_hidden("..");
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_hidden(".hidden");
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_hidden("visible");
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_hidden("src");
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_file_stem(void) {
  const char* _sv0t0 = file_stem("test.sv0");
  int _sv0t1 = sv0_string_eq(_sv0t0, "test");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = file_stem("src/foo.sv0");
  int _sv0t3 = sv0_string_eq(_sv0t2, "foo");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = file_stem("bar");
  int _sv0t5 = sv0_string_eq(_sv0t4, "bar");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = file_stem("/a/b/c.txt");
  int _sv0t7 = sv0_string_eq(_sv0t6, "c");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = file_stem("noext");
  int _sv0t9 = sv0_string_eq(_sv0t8, "noext");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_mangle(void) {
  const char* _sv0t0 = mangle("mymod", "foo");
  int _sv0t1 = sv0_string_eq(_sv0t0, "mymod__foo");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = mangle("a", "b");
  int _sv0t3 = sv0_string_eq(_sv0t2, "a__b");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_is_top_defining(void) {
  int _sv0t0 = is_top_defining(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_top_defining(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_top_defining(2);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_top_defining(3);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_top_defining(7);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_top_defining(4);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_top_defining(5);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_top_defining(6);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_collect_tops_count(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  sv0_vec_push(tags, 0);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 1);
  sv0_vec_push(tags, 6);
  sv0_vec_push(tags, 7);
  int _sv0t1 = collect_tops_count(tags);
  if ((_sv0t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_vec_new();
  int empty = _sv0t2;
  int _sv0t3 = collect_tops_count(empty);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_is_link_directive(void) {
  int _sv0t0 = is_link_directive(5);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_link_directive(6);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_link_directive(0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_link_directive(7);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_strip_directives_count(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  sv0_vec_push(tags, 0);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 1);
  sv0_vec_push(tags, 6);
  sv0_vec_push(tags, 2);
  int _sv0t1 = strip_directives_count(tags);
  if ((_sv0t1 != 3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_has_module_prefix(void) {
  int _sv0t0 = has_module_prefix(6);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = has_module_prefix(0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = has_module_prefix(5);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_in_tops(void) {
  int _sv0t0 = sv0_vec_new();
  int tops = _sv0t0;
  sv0_vec_push(tops, 10);
  sv0_vec_push(tops, 20);
  sv0_vec_push(tops, 30);
  int _sv0t1 = in_tops(tops, 20);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = in_tops(tops, 99);
  if ((_sv0t2 != 0)) {
    return 2;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int empty = _sv0t3;
  int _sv0t4 = in_tops(empty, 1);
  if ((_sv0t4 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_path_seg_needs_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int tops = _sv0t0;
  sv0_vec_push(tops, 100);
  sv0_vec_push(tops, 200);
  int _sv0t1 = path_seg_needs_mangle(tops, 100);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = path_seg_needs_mangle(tops, 999);
  if ((_sv0t2 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_is_sv0();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_is_hidden();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_file_stem();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_mangle();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_is_top_defining();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_collect_tops_count();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_is_link_directive();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (60 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_strip_directives_count();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (70 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_has_module_prefix();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (80 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_in_tops();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (90 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_path_seg_needs_mangle();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (100 + r11);
    return _sv0t20;
  } else {
  }
  return 0;
}

