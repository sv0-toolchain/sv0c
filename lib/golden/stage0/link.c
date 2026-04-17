#include "sv0_runtime.h"

static int is_sv0(const char* name);
static int is_hidden(const char* name);
static int find_last_slash(const char* path);
static int find_last_dot(const char* s, int start);
static const char* file_stem(const char* path);
static const char* mangle(const char* mod_id, const char* x);
static const char* fn_name_for_link(const char* mod_id, const char* name);
static const char* link_dir_from_entry(const char* entry_path);
static int is_top_defining(int tag);
static int collect_tops_count(int item_tags);
static int collect_top_names(int item_tags, int item_names, int out);
static int is_link_directive(int tag);
static int strip_directives_count(int item_tags);
static int has_module_prefix(int first_tag);
static int split_module_inner_count(int total_items, int has_wrapper);
static int map_ty_kind(int ty_tag);
static int needs_path_rewrite(int ty_tag);
static int needs_recursive_rewrite(int ty_tag);
static int needs_list_rewrite(int ty_tag);
static int is_leaf_ty(int ty_tag);
static int rewrite_depth(int ty_tag, int child_depth);
static int item_needs_rewrite(int item_kind);
static int item_has_body(int item_kind);
static int item_has_fields(int item_kind);
static int map_fn_components(int has_params, int has_body, int has_contracts);
static int in_tops(int tops, int name);
static int path_seg_needs_mangle(int tops, int name);
static int map_path_segs_needs_mangle(int tops, int seg_count, int first_seg);
static const char* map_path_segs_mangle_first(const char* mod_id, const char* first_seg_str);
static const char* map_path_segs_rewrite_1(int tops, const char* mod_id, int seg_handle, const char* seg_str);
static const char* map_path_segs_rewrite_2(int tops, const char* mod_id, int first_handle, const char* first_str);
static int strip_link_directives(int item_tags, int out_indices);
static int split_module_name(int item_tags, int item_d1, int pp, int default_name);
static int split_module_body_start(int item_tags);
static int test_is_sv0(void);
static int test_is_hidden(void);
static int test_file_stem(void);
static int test_mangle(void);
static int test_fn_name_for_link(void);
static int test_link_dir_from_entry(void);
static int test_is_top_defining(void);
static int test_collect_tops_count(void);
static int test_is_link_directive(void);
static int test_strip_directives_count(void);
static int test_has_module_prefix(void);
static int test_in_tops(void);
static int test_path_seg_needs_mangle(void);
static int test_split_module(void);
static int test_map_ty_kind(void);
static int test_item_rewrite(void);
static int test_collect_top_names(void);
static int test_map_path_segs_rewrite(void);
static int test_map_path_segs(void);
static int test_strip_link_directives(void);
static int test_split_module_name(void);

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
    if ((c == 92)) {
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

static const char* fn_name_for_link(const char* mod_id, const char* name) {
  int _sv0t0 = sv0_string_eq(name, "main");
  if (_sv0t0) {
    return "main";
  } else {
  }
  const char* _sv0t1 = mangle(mod_id, name);
  return _sv0t1;
}

static const char* link_dir_from_entry(const char* entry_path) {
  int _sv0t0 = find_last_slash(entry_path);
  int slash = _sv0t0;
  if ((slash < 0)) {
    return ".";
  } else {
  }
  if ((slash == 0)) {
    return "/";
  } else {
  }
  const char* _sv0t1 = sv0_string_substr(entry_path, 0, slash);
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

static int collect_top_names(int item_tags, int item_names, int out) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int len = _sv0t0;
  int count = 0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int _sv0t2 = is_top_defining(_sv0t1);
    if (_sv0t2) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      sv0_vec_push(out, _sv0t3);
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

static int split_module_inner_count(int total_items, int has_wrapper) {
  if (has_wrapper) {
    int _sv0t0 = (total_items - 1);
    return _sv0t0;
  } else {
  }
  return total_items;
}

static int map_ty_kind(int ty_tag) {
  if ((ty_tag == 0)) {
    return 1;
  } else {
  }
  if ((ty_tag == 1)) {
    return 2;
  } else {
  }
  if ((ty_tag == 2)) {
    return 2;
  } else {
  }
  if ((ty_tag == 3)) {
    return 2;
  } else {
  }
  if ((ty_tag == 4)) {
    return 2;
  } else {
  }
  if ((ty_tag == 5)) {
    return 3;
  } else {
  }
  if ((ty_tag == 6)) {
    return 0;
  } else {
  }
  if ((ty_tag == 7)) {
    return 0;
  } else {
  }
  return 0;
}

static int needs_path_rewrite(int ty_tag) {
  int _sv0t0 = map_ty_kind(ty_tag);
  int _sv0t1 = (_sv0t0 == 1);
  return _sv0t1;
}

static int needs_recursive_rewrite(int ty_tag) {
  int _sv0t0 = map_ty_kind(ty_tag);
  int _sv0t1 = (_sv0t0 == 2);
  return _sv0t1;
}

static int needs_list_rewrite(int ty_tag) {
  int _sv0t0 = map_ty_kind(ty_tag);
  int _sv0t1 = (_sv0t0 == 3);
  return _sv0t1;
}

static int is_leaf_ty(int ty_tag) {
  int _sv0t0 = map_ty_kind(ty_tag);
  int _sv0t1 = (_sv0t0 == 0);
  return _sv0t1;
}

static int rewrite_depth(int ty_tag, int child_depth) {
  int _sv0t0 = is_leaf_ty(ty_tag);
  if (_sv0t0) {
    return 0;
  } else {
  }
  int _sv0t1 = (1 + child_depth);
  return _sv0t1;
}

static int item_needs_rewrite(int item_kind) {
  if ((item_kind == 0)) {
    return 1;
  } else {
  }
  if ((item_kind == 3)) {
    return 1;
  } else {
  }
  if ((item_kind == 4)) {
    return 1;
  } else {
  }
  if ((item_kind == 5)) {
    return 1;
  } else {
  }
  if ((item_kind == 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int item_has_body(int item_kind) {
  if ((item_kind == 0)) {
    return 1;
  } else {
  }
  if ((item_kind == 5)) {
    return 1;
  } else {
  }
  if ((item_kind == 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int item_has_fields(int item_kind) {
  if ((item_kind == 3)) {
    return 1;
  } else {
  }
  if ((item_kind == 4)) {
    return 1;
  } else {
  }
  return 0;
}

static int map_fn_components(int has_params, int has_body, int has_contracts) {
  int parts = 1;
  if (has_params) {
    parts = (parts + 1);
  } else {
  }
  if (has_body) {
    parts = (parts + 1);
  } else {
  }
  if (has_contracts) {
    parts = (parts + 1);
  } else {
  }
  return parts;
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

static int map_path_segs_needs_mangle(int tops, int seg_count, int first_seg) {
  if ((seg_count == 1)) {
    int _sv0t0 = in_tops(tops, first_seg);
    return _sv0t0;
  } else {
  }
  if ((seg_count == 2)) {
    int _sv0t1 = in_tops(tops, first_seg);
    return _sv0t1;
  } else {
  }
  return 0;
}

static const char* map_path_segs_mangle_first(const char* mod_id, const char* first_seg_str) {
  const char* _sv0t0 = mangle(mod_id, first_seg_str);
  return _sv0t0;
}

static const char* map_path_segs_rewrite_1(int tops, const char* mod_id, int seg_handle, const char* seg_str) {
  int _sv0t0 = in_tops(tops, seg_handle);
  if (_sv0t0) {
    const char* _sv0t1 = mangle(mod_id, seg_str);
    return _sv0t1;
  } else {
  }
  return seg_str;
}

static const char* map_path_segs_rewrite_2(int tops, const char* mod_id, int first_handle, const char* first_str) {
  int _sv0t0 = in_tops(tops, first_handle);
  if (_sv0t0) {
    const char* _sv0t1 = mangle(mod_id, first_str);
    return _sv0t1;
  } else {
  }
  return first_str;
}

static int strip_link_directives(int item_tags, int out_indices) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = is_link_directive(tag);
    if ((_sv0t2 != 1)) {
      sv0_vec_push(out_indices, i);
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = sv0_vec_len(out_indices);
  return _sv0t3;
}

static int split_module_name(int item_tags, int item_d1, int pp, int default_name) {
  int _sv0t0 = sv0_vec_len(item_tags);
  if ((_sv0t0 == 0)) {
    return default_name;
  } else {
  }
  int _sv0t1 = sv0_vec_get(item_tags, 0);
  int first = _sv0t1;
  if ((first == 6)) {
    int _sv0t2 = sv0_vec_get(item_d1, 0);
    int pp_start = _sv0t2;
    int _sv0t3 = sv0_vec_get(pp, pp_start);
    return _sv0t3;
  } else {
  }
  return default_name;
}

static int split_module_body_start(int item_tags) {
  int _sv0t0 = sv0_vec_len(item_tags);
  if ((_sv0t0 == 0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(item_tags, 0);
  if ((_sv0t1 == 6)) {
    return 1;
  } else {
  }
  return 0;
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

static int test_fn_name_for_link(void) {
  const char* _sv0t0 = fn_name_for_link("mymod", "main");
  int _sv0t1 = sv0_string_eq(_sv0t0, "main");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = fn_name_for_link("mymod", "foo");
  int _sv0t3 = sv0_string_eq(_sv0t2, "mymod__foo");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = fn_name_for_link("utils", "helper");
  int _sv0t5 = sv0_string_eq(_sv0t4, "utils__helper");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_link_dir_from_entry(void) {
  const char* _sv0t0 = link_dir_from_entry("src/foo.sv0");
  int _sv0t1 = sv0_string_eq(_sv0t0, "src");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = link_dir_from_entry("foo.sv0");
  int _sv0t3 = sv0_string_eq(_sv0t2, ".");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = link_dir_from_entry("/a/b/c.sv0");
  int _sv0t5 = sv0_string_eq(_sv0t4, "/a/b");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = link_dir_from_entry("/foo.sv0");
  int _sv0t7 = sv0_string_eq(_sv0t6, "/");
  if ((_sv0t7 != 1)) {
    return 4;
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

static int test_split_module(void) {
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
  int _sv0t2 = split_module_inner_count(5, 1);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  int _sv0t3 = split_module_inner_count(5, 0);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_map_ty_kind(void) {
  int _sv0t0 = map_ty_kind(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = map_ty_kind(1);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = map_ty_kind(5);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = map_ty_kind(6);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = needs_path_rewrite(0);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = needs_recursive_rewrite(1);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = needs_list_rewrite(5);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_leaf_ty(6);
  if ((_sv0t7 != 1)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_leaf_ty(0);
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  int _sv0t9 = rewrite_depth(6, 0);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = rewrite_depth(0, 0);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = rewrite_depth(1, 2);
  if ((_sv0t11 != 3)) {
    return 12;
  } else {
  }
  return 0;
}

static int test_item_rewrite(void) {
  int _sv0t0 = item_needs_rewrite(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = item_needs_rewrite(4);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = item_needs_rewrite(7);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = item_needs_rewrite(2);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = item_has_body(0);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = item_has_body(3);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = item_has_body(6);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = item_has_fields(3);
  if ((_sv0t7 != 1)) {
    return 8;
  } else {
  }
  int _sv0t8 = item_has_fields(4);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = item_has_fields(0);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = map_fn_components(1, 1, 1);
  if ((_sv0t10 != 4)) {
    return 11;
  } else {
  }
  int _sv0t11 = map_fn_components(0, 1, 0);
  if ((_sv0t11 != 2)) {
    return 12;
  } else {
  }
  int _sv0t12 = map_fn_components(0, 0, 0);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  return 0;
}

static int test_collect_top_names(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int names = _sv0t1;
  sv0_vec_push(tags, 0);
  sv0_vec_push(names, 100);
  sv0_vec_push(tags, 5);
  sv0_vec_push(names, 200);
  sv0_vec_push(tags, 1);
  sv0_vec_push(names, 300);
  sv0_vec_push(tags, 6);
  sv0_vec_push(names, 400);
  sv0_vec_push(tags, 7);
  sv0_vec_push(names, 500);
  int _sv0t2 = sv0_vec_new();
  int out = _sv0t2;
  int _sv0t3 = collect_top_names(tags, names, out);
  int n = _sv0t3;
  if ((n != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_len(out);
  if ((_sv0t4 != 3)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_get(out, 0);
  if ((_sv0t5 != 100)) {
    return 3;
  } else {
  }
  int _sv0t6 = sv0_vec_get(out, 1);
  if ((_sv0t6 != 300)) {
    return 4;
  } else {
  }
  int _sv0t7 = sv0_vec_get(out, 2);
  if ((_sv0t7 != 500)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_map_path_segs_rewrite(void) {
  int _sv0t0 = sv0_vec_new();
  int tops = _sv0t0;
  sv0_vec_push(tops, 10);
  sv0_vec_push(tops, 20);
  const char* _sv0t1 = map_path_segs_rewrite_1(tops, "mod", 10, "Foo");
  const char* r1;
  r1 = _sv0t1;
  int _sv0t2 = sv0_string_eq(r1, "mod__Foo");
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t3 = map_path_segs_rewrite_1(tops, "mod", 99, "Bar");
  const char* r2;
  r2 = _sv0t3;
  int _sv0t4 = sv0_string_eq(r2, "Bar");
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t5 = map_path_segs_rewrite_2(tops, "mod", 20, "Baz");
  const char* r3;
  r3 = _sv0t5;
  int _sv0t6 = sv0_string_eq(r3, "mod__Baz");
  if ((_sv0t6 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t7 = map_path_segs_rewrite_2(tops, "mod", 99, "Qux");
  const char* r4;
  r4 = _sv0t7;
  int _sv0t8 = sv0_string_eq(r4, "Qux");
  if ((_sv0t8 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_map_path_segs(void) {
  int _sv0t0 = sv0_vec_new();
  int tops = _sv0t0;
  sv0_vec_push(tops, 10);
  sv0_vec_push(tops, 20);
  int _sv0t1 = map_path_segs_needs_mangle(tops, 1, 10);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = map_path_segs_needs_mangle(tops, 1, 99);
  if ((_sv0t2 != 0)) {
    return 2;
  } else {
  }
  int _sv0t3 = map_path_segs_needs_mangle(tops, 2, 10);
  if ((_sv0t3 != 1)) {
    return 3;
  } else {
  }
  int _sv0t4 = map_path_segs_needs_mangle(tops, 2, 99);
  if ((_sv0t4 != 0)) {
    return 4;
  } else {
  }
  int _sv0t5 = map_path_segs_needs_mangle(tops, 3, 10);
  if ((_sv0t5 != 0)) {
    return 5;
  } else {
  }
  int _sv0t6 = map_path_segs_needs_mangle(tops, 0, 10);
  if ((_sv0t6 != 0)) {
    return 6;
  } else {
  }
  const char* _sv0t7 = map_path_segs_mangle_first("mod", "Foo");
  int _sv0t8 = sv0_string_eq(_sv0t7, "mod__Foo");
  if ((_sv0t8 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_strip_link_directives(void) {
  int _sv0t0 = sv0_vec_new();
  int it = _sv0t0;
  sv0_vec_push(it, 0);
  sv0_vec_push(it, 5);
  sv0_vec_push(it, 1);
  sv0_vec_push(it, 6);
  sv0_vec_push(it, 2);
  int _sv0t1 = sv0_vec_new();
  int out = _sv0t1;
  int _sv0t2 = strip_link_directives(it, out);
  int n = _sv0t2;
  if ((n != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_get(out, 0);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(out, 1);
  if ((_sv0t4 != 2)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(out, 2);
  if ((_sv0t5 != 4)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int empty_it = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int empty_out = _sv0t7;
  int _sv0t8 = strip_link_directives(empty_it, empty_out);
  if ((_sv0t8 != 0)) {
    return 5;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int all_use = _sv0t9;
  sv0_vec_push(all_use, 5);
  sv0_vec_push(all_use, 6);
  int _sv0t10 = sv0_vec_new();
  int au_out = _sv0t10;
  int _sv0t11 = strip_link_directives(all_use, au_out);
  if ((_sv0t11 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_split_module_name(void) {
  int _sv0t0 = sv0_vec_new();
  int it = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int id1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int pp = _sv0t2;
  sv0_vec_push(it, 6);
  sv0_vec_push(id1, 0);
  sv0_vec_push(pp, 42);
  int _sv0t3 = split_module_name(it, id1, pp, 99);
  if ((_sv0t3 != 42)) {
    return 1;
  } else {
  }
  int _sv0t4 = split_module_body_start(it);
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int it2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id1b = _sv0t6;
  sv0_vec_push(it2, 0);
  sv0_vec_push(id1b, 10);
  int _sv0t7 = split_module_name(it2, id1b, pp, 99);
  if ((_sv0t7 != 99)) {
    return 3;
  } else {
  }
  int _sv0t8 = split_module_body_start(it2);
  if ((_sv0t8 != 0)) {
    return 4;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int empty = _sv0t9;
  int _sv0t10 = split_module_name(empty, id1, pp, 77);
  if ((_sv0t10 != 77)) {
    return 5;
  } else {
  }
  int _sv0t11 = split_module_body_start(empty);
  if ((_sv0t11 != 0)) {
    return 6;
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
  int _sv0t21 = test_split_module();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (110 + r12);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_map_ty_kind();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (120 + r13);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_item_rewrite();
  int r14 = _sv0t25;
  if ((r14 != 0)) {
    int _sv0t26 = (140 + r14);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = test_fn_name_for_link();
  int r15 = _sv0t27;
  if ((r15 != 0)) {
    int _sv0t28 = (150 + r15);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = test_link_dir_from_entry();
  int r16 = _sv0t29;
  if ((r16 != 0)) {
    int _sv0t30 = (160 + r16);
    return _sv0t30;
  } else {
  }
  int _sv0t31 = test_map_path_segs();
  int r17 = _sv0t31;
  if ((r17 != 0)) {
    int _sv0t32 = (170 + r17);
    return _sv0t32;
  } else {
  }
  int _sv0t33 = test_collect_top_names();
  int r18 = _sv0t33;
  if ((r18 != 0)) {
    int _sv0t34 = (180 + r18);
    return _sv0t34;
  } else {
  }
  int _sv0t35 = test_map_path_segs_rewrite();
  int r19 = _sv0t35;
  if ((r19 != 0)) {
    int _sv0t36 = (190 + r19);
    return _sv0t36;
  } else {
  }
  int _sv0t37 = test_strip_link_directives();
  int r20 = _sv0t37;
  if ((r20 != 0)) {
    int _sv0t38 = (200 + r20);
    return _sv0t38;
  } else {
  }
  int _sv0t39 = test_split_module_name();
  int r21 = _sv0t39;
  if ((r21 != 0)) {
    int _sv0t40 = (210 + r21);
    return _sv0t40;
  } else {
  }
  return 0;
}

