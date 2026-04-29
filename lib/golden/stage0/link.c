#include "sv0_runtime.h"

static int is_sv0(const char* name);
static int is_hidden(const char* name);
static int is_link_source_file(const char* name);
static const char* list_sv0(const char* dir);
static const char* walk_sv0(const char* dir);
static const char* parse_file(const char* path);
static int listing_find_newline(const char* listing, int start);
static const char* link_project_concat_sources_from_listing(const char* listing);
static int listing_count_nonempty_paths(const char* listing);
static const char* link_project_concat_sources_from_dir(const char* dir);
static const char* link_project_listing_from_entry(const char* entry_path);
static const char* link_project_concat_sources_from_entry(const char* entry_path);
static const char* link_project_dir(const char* dir);
static int LINK_ERR_NO_SV0_IN_DIR(void);
static int link_listing_nonempty(const char* listing);
static int link_project_guard_nonempty(const char* dir);
static int find_last_slash(const char* path);
static int find_last_dot(const char* s, int start);
static const char* file_stem(const char* path);
static const char* mangle(const char* mod_id, const char* x);
static const char* fn_name_for_link(const char* mod_id, const char* name);
static const char* link_dir_from_entry(const char* entry_path);
static const char* module_id_from_entry_path(const char* entry_path);
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
static int link_path_pool_prefix_needs_mangle(int pp, int tops, int pool_start, int seg_count);
static int link_ty_tyname_path_needs_mangle(int ty_tags, int ty_d1, int ty_d2, int pp, int tops, int idx);
static const char* link_tok_text(const char* source, int starts, int ends, int pos);
static const char* link_ty_tyname_first_seg_preview(int ty_tags, int ty_d1, int ty_d2, int pp, int tops, const char* mod_id, int starts, int ends, const char* source, int idx);
static int link_expr_path_needs_mangle(int expr_tags, int ed1, int ed2, int pp, int tops, int idx);
static int link_pat_subtree_node_count(int pat_tags, int pat_d1, int pat_d2, int pat_d3, int idx, int depth);
static int link_pat_root_needs_mangle(int pat_tags, int pat_d1, int pat_d2, int pat_d3, int pp, int tops, int idx, int depth);
static int link_expr_subtree_node_count(int expr_tags, int ed1, int ed2, int ed3, int ed4, int idx, int depth);
static int link_expr_find_root_covering_min(int expr_tags, int ed1, int ed2, int ed3, int ed4, int lo, int hi_excl);
static int link_expr_root_needs_mangle(int expr_tags, int ed1, int ed2, int ed3, int ed4, int pp, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int tops, int idx, int depth);
static int link_ty_subtree_node_count(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int idx, int depth);
static int link_ty_peel_ref_mut_slice(int ty_tags, int ty_d1, int idx);
static int link_ty_after_ref_slice_chain_tyname_needs_mangle(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int pp, int tops, int idx);
static int strip_link_directives(int item_tags, int out_indices);
static int split_module_name(int item_tags, int item_d1, int pp, int default_name);
static int split_module_body_start(int item_tags);
static int test_is_sv0(void);
static int test_is_hidden(void);
static int test_is_link_source_file(void);
static int test_file_stem(void);
static int test_mangle(void);
static int test_fn_name_for_link(void);
static int test_link_dir_from_entry(void);
static int test_module_id_from_entry_path(void);
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
static int test_g2_link_host_io_aliases(void);
static int test_link_g6_empty_listing(void);
static int test_link_project_concat_sources_from_listing(void);
static int test_listing_count_nonempty_paths(void);
static int test_link_ty_tyname_path_needs_mangle(void);
static int test_link_path_pool_prefix_needs_mangle(void);
static int test_link_expr_path_needs_mangle(void);
static int test_link_expr_subtree_node_count_binop(void);
static int test_link_expr_call_root_needs_mangle_two_args(void);
static int test_link_expr_if_needs_mangle(void);
static int test_link_expr_block_stmt_needs_mangle(void);
static int test_link_expr_match_arm_needs_mangle(void);
static int test_link_pat_subtree_node_count_wild(void);
static int test_link_pat_subtree_tuple_two_wild(void);
static int test_link_pat_struct_path_needs_mangle(void);
static int test_link_ty_ref_chain_tyname_mangle(void);
static int test_link_ty_array_tyname_mangle(void);
static int test_link_ty_tuple_two_tyname_mangle(void);
static int test_link_ty_generic_arg_path_mangle(void);
static int test_link_ty_tyname_first_seg_preview(void);
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

static int is_link_source_file(const char* name) {
  int _sv0t0 = is_hidden(name);
  if (_sv0t0) {
    return 0;
  } else {
  }
  int _sv0t1 = is_sv0(name);
  return _sv0t1;
}

static const char* list_sv0(const char* dir) {
  const char* _sv0t0 = sv0_read_dir(dir);
  return _sv0t0;
}

static const char* walk_sv0(const char* dir) {
  const char* _sv0t0 = sv0_read_dir(dir);
  return _sv0t0;
}

static const char* parse_file(const char* path) {
  const char* _sv0t0 = sv0_read_file(path);
  return _sv0t0;
}

static int listing_find_newline(const char* listing, int start) {
  int _sv0t0 = sv0_string_len(listing);
  int len = _sv0t0;
  int i = start;
  while ((i < len)) {
    int _sv0t1 = sv0_string_char_at(listing, i);
    if ((_sv0t1 == 10)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  return len;
}

static const char* link_project_concat_sources_from_listing(const char* listing) {
  int _sv0t0 = sv0_string_len(listing);
  int len = _sv0t0;
  int p = 0;
  const char* out;
  out = "";
  int first = 1;
  while ((p < len)) {
    int _sv0t1 = listing_find_newline(listing, p);
    int nl = _sv0t1;
    int lsz = (nl - p);
    if ((lsz > 0)) {
      const char* _sv0t2 = sv0_string_substr(listing, p, lsz);
      const char* path;
      path = _sv0t2;
      const char* _sv0t3 = parse_file(path);
      const char* src;
      src = _sv0t3;
      if (first) {
        out = src;
        first = 0;
      } else {
        const char* _sv0t4 = sv0_string_concat(out, "\n");
        out = _sv0t4;
        const char* _sv0t5 = sv0_string_concat(out, src);
        out = _sv0t5;
      }
    } else {
    }
    p = (nl + 1);
  }
  return out;
}

static int listing_count_nonempty_paths(const char* listing) {
  int _sv0t0 = sv0_string_len(listing);
  int len = _sv0t0;
  int p = 0;
  int n = 0;
  while ((p < len)) {
    int _sv0t1 = listing_find_newline(listing, p);
    int nl = _sv0t1;
    int lsz = (nl - p);
    if ((lsz > 0)) {
      n = (n + 1);
    } else {
    }
    p = (nl + 1);
  }
  return n;
}

static const char* link_project_concat_sources_from_dir(const char* dir) {
  const char* _sv0t0 = link_project_dir(dir);
  const char* listing;
  listing = _sv0t0;
  const char* _sv0t1 = link_project_concat_sources_from_listing(listing);
  return _sv0t1;
}

static const char* link_project_listing_from_entry(const char* entry_path) {
  const char* _sv0t0 = link_dir_from_entry(entry_path);
  const char* dir;
  dir = _sv0t0;
  const char* _sv0t1 = link_project_dir(dir);
  return _sv0t1;
}

static const char* link_project_concat_sources_from_entry(const char* entry_path) {
  const char* _sv0t0 = link_dir_from_entry(entry_path);
  const char* dir;
  dir = _sv0t0;
  const char* _sv0t1 = link_project_concat_sources_from_dir(dir);
  return _sv0t1;
}

static const char* link_project_dir(const char* dir) {
  const char* _sv0t0 = list_sv0(dir);
  return _sv0t0;
}

static int LINK_ERR_NO_SV0_IN_DIR(void) {
  return 321;
}

static int link_listing_nonempty(const char* listing) {
  int _sv0t0 = sv0_string_len(listing);
  int _sv0t1 = (_sv0t0 > 0);
  return _sv0t1;
}

static int link_project_guard_nonempty(const char* dir) {
  const char* _sv0t0 = link_project_dir(dir);
  const char* listing;
  listing = _sv0t0;
  int _sv0t1 = link_listing_nonempty(listing);
  if ((_sv0t1 != 1)) {
    int _sv0t2 = LINK_ERR_NO_SV0_IN_DIR();
    return _sv0t2;
  } else {
  }
  return 0;
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

static const char* module_id_from_entry_path(const char* entry_path) {
  const char* _sv0t0 = file_stem(entry_path);
  return _sv0t0;
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

static int link_path_pool_prefix_needs_mangle(int pp, int tops, int pool_start, int seg_count) {
  if ((seg_count == 1)) {
    int _sv0t0 = sv0_vec_get(pp, pool_start);
    int ft = _sv0t0;
    int _sv0t1 = map_path_segs_needs_mangle(tops, 1, ft);
    return _sv0t1;
  } else {
  }
  if ((seg_count == 2)) {
    int _sv0t2 = sv0_vec_get(pp, pool_start);
    int ft2 = _sv0t2;
    int _sv0t3 = map_path_segs_needs_mangle(tops, 2, ft2);
    return _sv0t3;
  } else {
  }
  return 0;
}

static int link_ty_tyname_path_needs_mangle(int ty_tags, int ty_d1, int ty_d2, int pp, int tops, int idx) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(ty_tags, idx);
  if ((_sv0t0 != 0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(ty_d1, idx);
  int ps = _sv0t1;
  int _sv0t2 = sv0_vec_get(ty_d2, idx);
  int sc = _sv0t2;
  int _sv0t3 = link_path_pool_prefix_needs_mangle(pp, tops, ps, sc);
  return _sv0t3;
}

static const char* link_tok_text(const char* source, int starts, int ends, int pos) {
  int _sv0t0 = sv0_vec_get(starts, pos);
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, pos);
  int en = _sv0t1;
  int len = (en - st);
  const char* _sv0t2 = sv0_string_substr(source, st, len);
  return _sv0t2;
}

static const char* link_ty_tyname_first_seg_preview(int ty_tags, int ty_d1, int ty_d2, int pp, int tops, const char* mod_id, int starts, int ends, const char* source, int idx) {
  if ((idx < 0)) {
    return "";
  } else {
  }
  int _sv0t0 = sv0_vec_get(ty_tags, idx);
  if ((_sv0t0 != 0)) {
    return "";
  } else {
  }
  int _sv0t1 = sv0_vec_get(ty_d1, idx);
  int ps = _sv0t1;
  int _sv0t2 = sv0_vec_get(ty_d2, idx);
  int sc = _sv0t2;
  if ((sc == 1)) {
    int _sv0t3 = sv0_vec_get(pp, ps);
    int h1 = _sv0t3;
    const char* _sv0t4 = link_tok_text(source, starts, ends, h1);
    const char* t1;
    t1 = _sv0t4;
    const char* _sv0t5 = map_path_segs_rewrite_1(tops, mod_id, h1, t1);
    return _sv0t5;
  } else {
  }
  if ((sc == 2)) {
    int _sv0t6 = sv0_vec_get(pp, ps);
    int hf = _sv0t6;
    const char* _sv0t7 = link_tok_text(source, starts, ends, hf);
    const char* tf;
    tf = _sv0t7;
    const char* _sv0t8 = map_path_segs_rewrite_2(tops, mod_id, hf, tf);
    return _sv0t8;
  } else {
  }
  return "";
}

static int link_expr_path_needs_mangle(int expr_tags, int ed1, int ed2, int pp, int tops, int idx) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(expr_tags, idx);
  if ((_sv0t0 != 1)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(ed1, idx);
  int ps2 = _sv0t1;
  int _sv0t2 = sv0_vec_get(ed2, idx);
  int sc2 = _sv0t2;
  int _sv0t3 = link_path_pool_prefix_needs_mangle(pp, tops, ps2, sc2);
  return _sv0t3;
}

static int link_pat_subtree_node_count(int pat_tags, int pat_d1, int pat_d2, int pat_d3, int idx, int depth) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  if ((depth > 96)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(pat_tags, idx);
  int t = _sv0t2;
  if ((t == 0)) {
    return 1;
  } else {
  }
  if ((t == 1)) {
    return 1;
  } else {
  }
  if ((t == 2)) {
    return 1;
  } else {
  }
  if ((t == 3)) {
    int _sv0t3 = sv0_vec_get(pat_d1, idx);
    int ef = _sv0t3;
    int _sv0t4 = sv0_vec_get(pat_d2, idx);
    int ec = _sv0t4;
    int cur = ef;
    int tot = 0;
    int j = 0;
    while ((j < ec)) {
      int _sv0t5 = (depth + 1);
      int _sv0t6 = link_pat_subtree_node_count(pat_tags, pat_d1, pat_d2, pat_d3, cur, _sv0t5);
      int part = _sv0t6;
      if ((part <= 0)) {
        int _sv0t7 = (0 - 1);
        return _sv0t7;
      } else {
      }
      tot = (tot + part);
      cur = (cur + part);
      j = (j + 1);
    }
    int _sv0t8 = (tot + 1);
    return _sv0t8;
  } else {
  }
  if ((t == 4)) {
    int _sv0t9 = sv0_vec_get(pat_d3, idx);
    int fc = _sv0t9;
    if ((fc <= 0)) {
      return 1;
    } else {
    }
    int first_f = (idx - fc);
    int cur2 = first_f;
    int tot2 = 0;
    int j2 = 0;
    while ((j2 < fc)) {
      int _sv0t10 = (depth + 1);
      int _sv0t11 = link_pat_subtree_node_count(pat_tags, pat_d1, pat_d2, pat_d3, cur2, _sv0t10);
      int part2 = _sv0t11;
      if ((part2 <= 0)) {
        int _sv0t12 = (0 - 1);
        return _sv0t12;
      } else {
      }
      tot2 = (tot2 + part2);
      cur2 = (cur2 + part2);
      j2 = (j2 + 1);
    }
    int _sv0t13 = (tot2 + 1);
    return _sv0t13;
  } else {
  }
  if ((t == 5)) {
    int _sv0t14 = sv0_vec_get(pat_d3, idx);
    int ac = _sv0t14;
    if ((ac <= 0)) {
      return 1;
    } else {
    }
    int first_a = (idx - ac);
    int cur3 = first_a;
    int tot3 = 0;
    int j3 = 0;
    while ((j3 < ac)) {
      int _sv0t15 = (depth + 1);
      int _sv0t16 = link_pat_subtree_node_count(pat_tags, pat_d1, pat_d2, pat_d3, cur3, _sv0t15);
      int part3 = _sv0t16;
      if ((part3 <= 0)) {
        int _sv0t17 = (0 - 1);
        return _sv0t17;
      } else {
      }
      tot3 = (tot3 + part3);
      cur3 = (cur3 + part3);
      j3 = (j3 + 1);
    }
    int _sv0t18 = (tot3 + 1);
    return _sv0t18;
  } else {
  }
  if ((t == 6)) {
    int _sv0t19 = sv0_vec_get(pat_d1, idx);
    int fo = _sv0t19;
    int _sv0t20 = (idx - fo);
    int span = (_sv0t20 + 1);
    return span;
  } else {
  }
  int _sv0t21 = (0 - 1);
  return _sv0t21;
}

static int link_pat_root_needs_mangle(int pat_tags, int pat_d1, int pat_d2, int pat_d3, int pp, int tops, int idx, int depth) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  if ((depth > 96)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(pat_tags, idx);
  int t = _sv0t0;
  if ((t == 0)) {
    return 0;
  } else {
  }
  if ((t == 1)) {
    return 0;
  } else {
  }
  if ((t == 2)) {
    return 0;
  } else {
  }
  if ((t == 3)) {
    int _sv0t1 = sv0_vec_get(pat_d1, idx);
    int ef = _sv0t1;
    int _sv0t2 = sv0_vec_get(pat_d2, idx);
    int ec = _sv0t2;
    int cur = ef;
    int j = 0;
    while ((j < ec)) {
      int _sv0t3 = (depth + 1);
      int _sv0t4 = link_pat_root_needs_mangle(pat_tags, pat_d1, pat_d2, pat_d3, pp, tops, cur, _sv0t3);
      if ((_sv0t4 == 1)) {
        return 1;
      } else {
      }
      int _sv0t5 = link_pat_subtree_node_count(pat_tags, pat_d1, pat_d2, pat_d3, cur, 0);
      int sp = _sv0t5;
      if ((sp <= 0)) {
        return 0;
      } else {
      }
      cur = (cur + sp);
      j = (j + 1);
    }
    return 0;
  } else {
  }
  if ((t == 4)) {
    int _sv0t6 = sv0_vec_get(pat_d1, idx);
    int ps = _sv0t6;
    int _sv0t7 = sv0_vec_get(pat_d2, idx);
    int sc = _sv0t7;
    int _sv0t8 = link_path_pool_prefix_needs_mangle(pp, tops, ps, sc);
    if ((_sv0t8 == 1)) {
      return 1;
    } else {
    }
    int _sv0t9 = sv0_vec_get(pat_d3, idx);
    int fc = _sv0t9;
    int fi = 0;
    while ((fi < fc)) {
      int _sv0t10 = (idx - fc);
      int fr = (_sv0t10 + fi);
      int _sv0t11 = (depth + 1);
      int _sv0t12 = link_pat_root_needs_mangle(pat_tags, pat_d1, pat_d2, pat_d3, pp, tops, fr, _sv0t11);
      if ((_sv0t12 == 1)) {
        return 1;
      } else {
      }
      fi = (fi + 1);
    }
    return 0;
  } else {
  }
  if ((t == 5)) {
    int _sv0t13 = sv0_vec_get(pat_d1, idx);
    int ps5 = _sv0t13;
    int _sv0t14 = sv0_vec_get(pat_d2, idx);
    int sc5 = _sv0t14;
    int _sv0t15 = link_path_pool_prefix_needs_mangle(pp, tops, ps5, sc5);
    if ((_sv0t15 == 1)) {
      return 1;
    } else {
    }
    int _sv0t16 = sv0_vec_get(pat_d3, idx);
    int ac = _sv0t16;
    int ai = 0;
    while ((ai < ac)) {
      int _sv0t17 = (idx - ac);
      int ar = (_sv0t17 + ai);
      int _sv0t18 = (depth + 1);
      int _sv0t19 = link_pat_root_needs_mangle(pat_tags, pat_d1, pat_d2, pat_d3, pp, tops, ar, _sv0t18);
      if ((_sv0t19 == 1)) {
        return 1;
      } else {
      }
      ai = (ai + 1);
    }
    return 0;
  } else {
  }
  if ((t == 6)) {
    int _sv0t20 = sv0_vec_get(pat_d1, idx);
    int fo = _sv0t20;
    int _sv0t21 = sv0_vec_get(pat_d2, idx);
    int cnt = _sv0t21;
    int cur = fo;
    int ai = 0;
    while ((ai < cnt)) {
      int _sv0t22 = (depth + 1);
      int _sv0t23 = link_pat_root_needs_mangle(pat_tags, pat_d1, pat_d2, pat_d3, pp, tops, cur, _sv0t22);
      if ((_sv0t23 == 1)) {
        return 1;
      } else {
      }
      int _sv0t24 = link_pat_subtree_node_count(pat_tags, pat_d1, pat_d2, pat_d3, cur, 0);
      int sp = _sv0t24;
      if ((sp <= 0)) {
        return 0;
      } else {
      }
      cur = (cur + sp);
      ai = (ai + 1);
    }
    return 0;
  } else {
  }
  return 0;
}

static int link_expr_subtree_node_count(int expr_tags, int ed1, int ed2, int ed3, int ed4, int idx, int depth) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  if ((depth > 96)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(expr_tags, idx);
  int tag = _sv0t2;
  if ((tag == 0)) {
    return 1;
  } else {
  }
  if ((tag == 1)) {
    return 1;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t3 = sv0_vec_get(ed2, idx);
    int op = _sv0t3;
    int _sv0t4 = (depth + 1);
    int _sv0t5 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, op, _sv0t4);
    int sc = _sv0t5;
    if ((sc <= 0)) {
      int _sv0t6 = (0 - 1);
      return _sv0t6;
    } else {
    }
    int _sv0t7 = (1 + sc);
    return _sv0t7;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t8 = sv0_vec_get(ed2, idx);
    int lhs = _sv0t8;
    int _sv0t9 = sv0_vec_get(ed3, idx);
    int rhs = _sv0t9;
    int _sv0t10 = (depth + 1);
    int _sv0t11 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, lhs, _sv0t10);
    int sl = _sv0t11;
    if ((sl <= 0)) {
      int _sv0t12 = (0 - 1);
      return _sv0t12;
    } else {
    }
    int _sv0t13 = (depth + 1);
    int _sv0t14 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, rhs, _sv0t13);
    int sr = _sv0t14;
    if ((sr <= 0)) {
      int _sv0t15 = (0 - 1);
      return _sv0t15;
    } else {
    }
    int _sv0t16 = (1 + sl);
    int _sv0t17 = (_sv0t16 + sr);
    return _sv0t17;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t18 = sv0_vec_get(ed1, idx);
    int ce = _sv0t18;
    int _sv0t19 = sv0_vec_get(ed3, idx);
    int span = _sv0t19;
    int _sv0t20 = (depth + 1);
    int _sv0t21 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ce, _sv0t20);
    int sc = _sv0t21;
    if ((sc <= 0)) {
      int _sv0t22 = (0 - 1);
      return _sv0t22;
    } else {
    }
    int _sv0t23 = (1 + sc);
    int _sv0t24 = (_sv0t23 + span);
    return _sv0t24;
  } else {
  }
  if ((tag == 5)) {
    int _sv0t25 = sv0_vec_get(ed1, idx);
    int rcv = _sv0t25;
    int _sv0t26 = sv0_vec_get(ed4, idx);
    int spanm = _sv0t26;
    int _sv0t27 = (depth + 1);
    int _sv0t28 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, rcv, _sv0t27);
    int sr = _sv0t28;
    if ((sr <= 0)) {
      int _sv0t29 = (0 - 1);
      return _sv0t29;
    } else {
    }
    int _sv0t30 = (1 + sr);
    int _sv0t31 = (_sv0t30 + spanm);
    return _sv0t31;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t32 = sv0_vec_get(ed1, idx);
    int ob = _sv0t32;
    int _sv0t33 = (depth + 1);
    int _sv0t34 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ob, _sv0t33);
    int sc = _sv0t34;
    if ((sc <= 0)) {
      int _sv0t35 = (0 - 1);
      return _sv0t35;
    } else {
    }
    int _sv0t36 = (1 + sc);
    return _sv0t36;
  } else {
  }
  if ((tag == 7)) {
    int _sv0t37 = sv0_vec_get(ed1, idx);
    int ob7 = _sv0t37;
    int _sv0t38 = (depth + 1);
    int _sv0t39 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ob7, _sv0t38);
    int sc7 = _sv0t39;
    if ((sc7 <= 0)) {
      int _sv0t40 = (0 - 1);
      return _sv0t40;
    } else {
    }
    int _sv0t41 = (1 + sc7);
    return _sv0t41;
  } else {
  }
  if ((tag == 8)) {
    int _sv0t42 = sv0_vec_get(ed1, idx);
    int o8 = _sv0t42;
    int _sv0t43 = sv0_vec_get(ed2, idx);
    int i8 = _sv0t43;
    int _sv0t44 = (depth + 1);
    int _sv0t45 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, o8, _sv0t44);
    int so = _sv0t45;
    if ((so <= 0)) {
      int _sv0t46 = (0 - 1);
      return _sv0t46;
    } else {
    }
    int _sv0t47 = (depth + 1);
    int _sv0t48 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, i8, _sv0t47);
    int si = _sv0t48;
    if ((si <= 0)) {
      int _sv0t49 = (0 - 1);
      return _sv0t49;
    } else {
    }
    int _sv0t50 = (1 + so);
    int _sv0t51 = (_sv0t50 + si);
    return _sv0t51;
  } else {
  }
  if ((tag == 9)) {
    int _sv0t52 = sv0_vec_get(ed1, idx);
    int sf = _sv0t52;
    int _sv0t53 = sv0_vec_get(ed2, idx);
    int scnt = _sv0t53;
    int _sv0t54 = sv0_vec_get(ed3, idx);
    int tail = _sv0t54;
    int sum9 = 1;
    int lo9 = sf;
    int si9 = 0;
    while ((si9 < scnt)) {
      int _sv0t55 = link_expr_find_root_covering_min(expr_tags, ed1, ed2, ed3, ed4, lo9, idx);
      int sr9 = _sv0t55;
      if ((sr9 < 0)) {
        int _sv0t56 = (0 - 1);
        return _sv0t56;
      } else {
      }
      int _sv0t57 = (depth + 1);
      int _sv0t58 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, sr9, _sv0t57);
      int sc9 = _sv0t58;
      if ((sc9 <= 0)) {
        int _sv0t59 = (0 - 1);
        return _sv0t59;
      } else {
      }
      sum9 = (sum9 + sc9);
      lo9 = (sr9 + 1);
      si9 = (si9 + 1);
    }
    if ((tail >= 0)) {
      int _sv0t60 = (depth + 1);
      int _sv0t61 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, tail, _sv0t60);
      int st9 = _sv0t61;
      if ((st9 <= 0)) {
        int _sv0t62 = (0 - 1);
        return _sv0t62;
      } else {
      }
      sum9 = (sum9 + st9);
    } else {
    }
    return sum9;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t63 = sv0_vec_get(ed1, idx);
    int cond = _sv0t63;
    int _sv0t64 = sv0_vec_get(ed2, idx);
    int th = _sv0t64;
    int _sv0t65 = sv0_vec_get(ed3, idx);
    int el = _sv0t65;
    int _sv0t66 = (depth + 1);
    int _sv0t67 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, cond, _sv0t66);
    int sc = _sv0t67;
    if ((sc <= 0)) {
      int _sv0t68 = (0 - 1);
      return _sv0t68;
    } else {
    }
    int _sv0t69 = (depth + 1);
    int _sv0t70 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, th, _sv0t69);
    int st = _sv0t70;
    if ((st <= 0)) {
      int _sv0t71 = (0 - 1);
      return _sv0t71;
    } else {
    }
    int _sv0t72 = (1 + sc);
    int sum10 = (_sv0t72 + st);
    if ((el >= 0)) {
      int _sv0t73 = (depth + 1);
      int _sv0t74 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, el, _sv0t73);
      int se = _sv0t74;
      if ((se <= 0)) {
        int _sv0t75 = (0 - 1);
        return _sv0t75;
      } else {
      }
      sum10 = (sum10 + se);
    } else {
    }
    return sum10;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t76 = sv0_vec_get(ed1, idx);
    int scr = _sv0t76;
    int _sv0t77 = sv0_vec_get(ed2, idx);
    int af = _sv0t77;
    int _sv0t78 = sv0_vec_get(ed3, idx);
    int ac = _sv0t78;
    int _sv0t79 = (depth + 1);
    int _sv0t80 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, scr, _sv0t79);
    int ss = _sv0t80;
    if ((ss <= 0)) {
      int _sv0t81 = (0 - 1);
      return _sv0t81;
    } else {
    }
    int sum11 = (1 + ss);
    int lo11 = af;
    int ai11 = 0;
    while ((ai11 < ac)) {
      int _sv0t82 = link_expr_find_root_covering_min(expr_tags, ed1, ed2, ed3, ed4, lo11, idx);
      int ar11 = _sv0t82;
      if ((ar11 < 0)) {
        int _sv0t83 = (0 - 1);
        return _sv0t83;
      } else {
      }
      int _sv0t84 = (depth + 1);
      int _sv0t85 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ar11, _sv0t84);
      int sa11 = _sv0t85;
      if ((sa11 <= 0)) {
        int _sv0t86 = (0 - 1);
        return _sv0t86;
      } else {
      }
      sum11 = (sum11 + sa11);
      lo11 = (ar11 + 1);
      ai11 = (ai11 + 1);
    }
    return sum11;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t87 = sv0_vec_get(ed1, idx);
    int cond12 = _sv0t87;
    int _sv0t88 = sv0_vec_get(ed2, idx);
    int body12 = _sv0t88;
    int _sv0t89 = sv0_vec_get(ed3, idx);
    int invf = _sv0t89;
    int _sv0t90 = sv0_vec_get(ed4, idx);
    int invc = _sv0t90;
    int _sv0t91 = (depth + 1);
    int _sv0t92 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, cond12, _sv0t91);
    int sc12 = _sv0t92;
    if ((sc12 <= 0)) {
      int _sv0t93 = (0 - 1);
      return _sv0t93;
    } else {
    }
    int _sv0t94 = (depth + 1);
    int _sv0t95 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, body12, _sv0t94);
    int sb12 = _sv0t95;
    if ((sb12 <= 0)) {
      int _sv0t96 = (0 - 1);
      return _sv0t96;
    } else {
    }
    int _sv0t97 = (1 + sc12);
    int sum12 = (_sv0t97 + sb12);
    if ((invf >= 0)) {
      int lo12 = invf;
      int ii12 = 0;
      while ((ii12 < invc)) {
        int _sv0t98 = link_expr_find_root_covering_min(expr_tags, ed1, ed2, ed3, ed4, lo12, idx);
        int ir12 = _sv0t98;
        if ((ir12 < 0)) {
          int _sv0t99 = (0 - 1);
          return _sv0t99;
        } else {
        }
        int _sv0t100 = (depth + 1);
        int _sv0t101 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ir12, _sv0t100);
        int si12 = _sv0t101;
        if ((si12 <= 0)) {
          int _sv0t102 = (0 - 1);
          return _sv0t102;
        } else {
        }
        sum12 = (sum12 + si12);
        lo12 = (ir12 + 1);
        ii12 = (ii12 + 1);
      }
    } else {
    }
    return sum12;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t103 = sv0_vec_get(ed2, idx);
    int iter13 = _sv0t103;
    int _sv0t104 = sv0_vec_get(ed3, idx);
    int body13 = _sv0t104;
    int _sv0t105 = (depth + 1);
    int _sv0t106 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, iter13, _sv0t105);
    int si13 = _sv0t106;
    if ((si13 <= 0)) {
      int _sv0t107 = (0 - 1);
      return _sv0t107;
    } else {
    }
    int _sv0t108 = (depth + 1);
    int _sv0t109 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, body13, _sv0t108);
    int sb13 = _sv0t109;
    if ((sb13 <= 0)) {
      int _sv0t110 = (0 - 1);
      return _sv0t110;
    } else {
    }
    int _sv0t111 = (1 + si13);
    int _sv0t112 = (_sv0t111 + sb13);
    return _sv0t112;
  } else {
  }
  if ((tag == 14)) {
    int _sv0t113 = sv0_vec_get(ed1, idx);
    int bd = _sv0t113;
    int _sv0t114 = (depth + 1);
    int _sv0t115 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, bd, _sv0t114);
    int sb = _sv0t115;
    if ((sb <= 0)) {
      int _sv0t116 = (0 - 1);
      return _sv0t116;
    } else {
    }
    int _sv0t117 = (1 + sb);
    return _sv0t117;
  } else {
  }
  if ((tag == 15)) {
    int _sv0t118 = sv0_vec_get(ed1, idx);
    int vi = _sv0t118;
    if ((vi < 0)) {
      return 1;
    } else {
    }
    int _sv0t119 = (depth + 1);
    int _sv0t120 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, vi, _sv0t119);
    int sv = _sv0t120;
    if ((sv <= 0)) {
      int _sv0t121 = (0 - 1);
      return _sv0t121;
    } else {
    }
    int _sv0t122 = (1 + sv);
    return _sv0t122;
  } else {
  }
  if ((tag == 16)) {
    int _sv0t123 = sv0_vec_get(ed1, idx);
    int vb = _sv0t123;
    if ((vb < 0)) {
      return 1;
    } else {
    }
    int _sv0t124 = (depth + 1);
    int _sv0t125 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, vb, _sv0t124);
    int sb = _sv0t125;
    if ((sb <= 0)) {
      int _sv0t126 = (0 - 1);
      return _sv0t126;
    } else {
    }
    int _sv0t127 = (1 + sb);
    return _sv0t127;
  } else {
  }
  if ((tag == 17)) {
    return 1;
  } else {
  }
  if ((tag == 27)) {
    int _sv0t128 = sv0_vec_get(ed3, idx);
    int init27 = _sv0t128;
    if ((init27 < 0)) {
      return 1;
    } else {
    }
    int _sv0t129 = (depth + 1);
    int _sv0t130 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, init27, _sv0t129);
    int si27 = _sv0t130;
    if ((si27 <= 0)) {
      int _sv0t131 = (0 - 1);
      return _sv0t131;
    } else {
    }
    int _sv0t132 = (1 + si27);
    return _sv0t132;
  } else {
  }
  if ((tag == 28)) {
    int _sv0t133 = sv0_vec_get(ed1, idx);
    int se28 = _sv0t133;
    int _sv0t134 = (depth + 1);
    int _sv0t135 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, se28, _sv0t134);
    int ss28 = _sv0t135;
    if ((ss28 <= 0)) {
      int _sv0t136 = (0 - 1);
      return _sv0t136;
    } else {
    }
    int _sv0t137 = (1 + ss28);
    return _sv0t137;
  } else {
  }
  if ((tag == 29)) {
    int _sv0t138 = sv0_vec_get(ed3, idx);
    int gd29 = _sv0t138;
    int _sv0t139 = sv0_vec_get(ed4, idx);
    int bd29 = _sv0t139;
    int _sv0t140 = (depth + 1);
    int _sv0t141 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, bd29, _sv0t140);
    int sb29 = _sv0t141;
    if ((sb29 <= 0)) {
      int _sv0t142 = (0 - 1);
      return _sv0t142;
    } else {
    }
    if ((gd29 < 0)) {
      int _sv0t143 = (1 + sb29);
      return _sv0t143;
    } else {
    }
    int _sv0t144 = (depth + 1);
    int _sv0t145 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, gd29, _sv0t144);
    int sg29 = _sv0t145;
    if ((sg29 <= 0)) {
      int _sv0t146 = (0 - 1);
      return _sv0t146;
    } else {
    }
    int _sv0t147 = (1 + sb29);
    int _sv0t148 = (_sv0t147 + sg29);
    return _sv0t148;
  } else {
  }
  if ((tag == 30)) {
    return 1;
  } else {
  }
  if ((tag == 31)) {
    return 1;
  } else {
  }
  if ((tag == 32)) {
    return 1;
  } else {
  }
  if ((tag == 33)) {
    return 1;
  } else {
  }
  if ((tag == 18)) {
    int _sv0t149 = sv0_vec_get(ed1, idx);
    int al = _sv0t149;
    int _sv0t150 = sv0_vec_get(ed2, idx);
    int ar = _sv0t150;
    int _sv0t151 = (depth + 1);
    int _sv0t152 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, al, _sv0t151);
    int sl = _sv0t152;
    if ((sl <= 0)) {
      int _sv0t153 = (0 - 1);
      return _sv0t153;
    } else {
    }
    int _sv0t154 = (depth + 1);
    int _sv0t155 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ar, _sv0t154);
    int sr = _sv0t155;
    if ((sr <= 0)) {
      int _sv0t156 = (0 - 1);
      return _sv0t156;
    } else {
    }
    int _sv0t157 = (1 + sl);
    int _sv0t158 = (_sv0t157 + sr);
    return _sv0t158;
  } else {
  }
  if ((tag == 19)) {
    int _sv0t159 = sv0_vec_get(ed2, idx);
    int al2 = _sv0t159;
    int _sv0t160 = sv0_vec_get(ed3, idx);
    int ar2 = _sv0t160;
    int _sv0t161 = (depth + 1);
    int _sv0t162 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, al2, _sv0t161);
    int sl2 = _sv0t162;
    if ((sl2 <= 0)) {
      int _sv0t163 = (0 - 1);
      return _sv0t163;
    } else {
    }
    int _sv0t164 = (depth + 1);
    int _sv0t165 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ar2, _sv0t164);
    int sr2 = _sv0t165;
    if ((sr2 <= 0)) {
      int _sv0t166 = (0 - 1);
      return _sv0t166;
    } else {
    }
    int _sv0t167 = (1 + sl2);
    int _sv0t168 = (_sv0t167 + sr2);
    return _sv0t168;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t169 = sv0_vec_get(ed1, idx);
    int cs = _sv0t169;
    int _sv0t170 = (depth + 1);
    int _sv0t171 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, cs, _sv0t170);
    int ss = _sv0t171;
    if ((ss <= 0)) {
      int _sv0t172 = (0 - 1);
      return _sv0t172;
    } else {
    }
    int _sv0t173 = (1 + ss);
    return _sv0t173;
  } else {
  }
  if ((tag == 21)) {
    int _sv0t174 = sv0_vec_get(ed1, idx);
    int slo = _sv0t174;
    int _sv0t175 = sv0_vec_get(ed2, idx);
    int shi = _sv0t175;
    int sum = 1;
    if ((slo >= 0)) {
      int _sv0t176 = (depth + 1);
      int _sv0t177 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, slo, _sv0t176);
      int sl = _sv0t177;
      if ((sl <= 0)) {
        int _sv0t178 = (0 - 1);
        return _sv0t178;
      } else {
      }
      sum = (sum + sl);
    } else {
    }
    if ((shi >= 0)) {
      int _sv0t179 = (depth + 1);
      int _sv0t180 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, shi, _sv0t179);
      int sr = _sv0t180;
      if ((sr <= 0)) {
        int _sv0t181 = (0 - 1);
        return _sv0t181;
      } else {
      }
      sum = (sum + sr);
    } else {
    }
    return sum;
  } else {
  }
  if ((tag == 22)) {
    int _sv0t182 = sv0_vec_get(ed1, idx);
    int te = _sv0t182;
    int _sv0t183 = (depth + 1);
    int _sv0t184 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, te, _sv0t183);
    int st = _sv0t184;
    if ((st <= 0)) {
      int _sv0t185 = (0 - 1);
      return _sv0t185;
    } else {
    }
    int _sv0t186 = (1 + st);
    return _sv0t186;
  } else {
  }
  if ((tag == 23)) {
    int _sv0t187 = sv0_vec_get(ed1, idx);
    int ae = _sv0t187;
    int _sv0t188 = (depth + 1);
    int _sv0t189 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ae, _sv0t188);
    int sa = _sv0t189;
    if ((sa <= 0)) {
      int _sv0t190 = (0 - 1);
      return _sv0t190;
    } else {
    }
    int _sv0t191 = (1 + sa);
    return _sv0t191;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t192 = sv0_vec_get(ed3, idx);
    int fc = _sv0t192;
    if ((fc <= 0)) {
      return 1;
    } else {
    }
    int sumf = 1;
    int fi = 0;
    while ((fi < fc)) {
      int _sv0t193 = (idx - fc);
      int fr = (_sv0t193 + fi);
      int _sv0t194 = (depth + 1);
      int _sv0t195 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, fr, _sv0t194);
      int sf = _sv0t195;
      if ((sf <= 0)) {
        int _sv0t196 = (0 - 1);
        return _sv0t196;
      } else {
      }
      sumf = (sumf + sf);
      fi = (fi + 1);
    }
    return sumf;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t197 = sv0_vec_get(ed1, idx);
    int first = _sv0t197;
    int _sv0t198 = sv0_vec_get(ed2, idx);
    int cnt = _sv0t198;
    int cur = first;
    int sumt = 1;
    int j = 0;
    while ((j < cnt)) {
      int _sv0t199 = (depth + 1);
      int _sv0t200 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, cur, _sv0t199);
      int se = _sv0t200;
      if ((se <= 0)) {
        int _sv0t201 = (0 - 1);
        return _sv0t201;
      } else {
      }
      sumt = (sumt + se);
      cur = (cur + se);
      j = (j + 1);
    }
    return sumt;
  } else {
  }
  if ((tag == 26)) {
    int _sv0t202 = sv0_vec_get(ed2, idx);
    int ec = _sv0t202;
    int suma = 1;
    int k = 0;
    while ((k < ec)) {
      int _sv0t203 = (idx - ec);
      int er = (_sv0t203 + k);
      int _sv0t204 = (depth + 1);
      int _sv0t205 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, er, _sv0t204);
      int se = _sv0t205;
      if ((se <= 0)) {
        int _sv0t206 = (0 - 1);
        return _sv0t206;
      } else {
      }
      suma = (suma + se);
      k = (k + 1);
    }
    return suma;
  } else {
  }
  int _sv0t207 = (0 - 1);
  return _sv0t207;
}

static int link_expr_find_root_covering_min(int expr_tags, int ed1, int ed2, int ed3, int ed4, int lo, int hi_excl) {
  int r = lo;
  while ((r < hi_excl)) {
    int _sv0t0 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, r, 0);
    int c = _sv0t0;
    if ((c > 0)) {
      int _sv0t1 = (r - c);
      int min_i = (_sv0t1 + 1);
      if ((min_i <= lo)) {
        return r;
      } else {
      }
    } else {
    }
    r = (r + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int link_expr_root_needs_mangle(int expr_tags, int ed1, int ed2, int ed3, int ed4, int pp, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int tops, int idx, int depth) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  if ((depth > 96)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(expr_tags, idx);
  int tag = _sv0t0;
  if ((tag == 1)) {
    int _sv0t1 = link_expr_path_needs_mangle(expr_tags, ed1, ed2, pp, tops, idx);
    return _sv0t1;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t2 = sv0_vec_get(ed2, idx);
    int op = _sv0t2;
    int _sv0t3 = (depth + 1);
    int _sv0t4 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, op, _sv0t3);
    return _sv0t4;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t5 = sv0_vec_get(ed2, idx);
    int lhs = _sv0t5;
    int _sv0t6 = sv0_vec_get(ed3, idx);
    int rhs = _sv0t6;
    int _sv0t7 = (depth + 1);
    int _sv0t8 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, lhs, _sv0t7);
    if ((_sv0t8 == 1)) {
      return 1;
    } else {
    }
    int _sv0t9 = (depth + 1);
    int _sv0t10 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, rhs, _sv0t9);
    return _sv0t10;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t11 = sv0_vec_get(ed1, idx);
    int ce = _sv0t11;
    int _sv0t12 = sv0_vec_get(ed2, idx);
    int af = _sv0t12;
    int _sv0t13 = sv0_vec_get(ed3, idx);
    int span = _sv0t13;
    int _sv0t14 = (depth + 1);
    int _sv0t15 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, ce, _sv0t14);
    if ((_sv0t15 == 1)) {
      return 1;
    } else {
    }
    int hi = (af + span);
    int lo = af;
    while ((lo < hi)) {
      int _sv0t16 = link_expr_find_root_covering_min(expr_tags, ed1, ed2, ed3, ed4, lo, hi);
      int ar = _sv0t16;
      if ((ar < 0)) {
        return 0;
      } else {
      }
      int _sv0t17 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ar, 0);
      int ac = _sv0t17;
      if ((ac <= 0)) {
        return 0;
      } else {
      }
      int _sv0t18 = (depth + 1);
      int _sv0t19 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, ar, _sv0t18);
      if ((_sv0t19 == 1)) {
        return 1;
      } else {
      }
      lo = (ar + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 5)) {
    int _sv0t20 = sv0_vec_get(ed1, idx);
    int rcv = _sv0t20;
    int _sv0t21 = sv0_vec_get(ed3, idx);
    int afm = _sv0t21;
    int _sv0t22 = sv0_vec_get(ed4, idx);
    int acm = _sv0t22;
    int _sv0t23 = (depth + 1);
    int _sv0t24 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, rcv, _sv0t23);
    if ((_sv0t24 == 1)) {
      return 1;
    } else {
    }
    int hi5 = (afm + acm);
    int lo5 = afm;
    while ((lo5 < hi5)) {
      int _sv0t25 = link_expr_find_root_covering_min(expr_tags, ed1, ed2, ed3, ed4, lo5, hi5);
      int ar5 = _sv0t25;
      if ((ar5 < 0)) {
        return 0;
      } else {
      }
      int _sv0t26 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, ar5, 0);
      int ac5 = _sv0t26;
      if ((ac5 <= 0)) {
        return 0;
      } else {
      }
      int _sv0t27 = (depth + 1);
      int _sv0t28 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, ar5, _sv0t27);
      if ((_sv0t28 == 1)) {
        return 1;
      } else {
      }
      lo5 = (ar5 + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t29 = sv0_vec_get(ed1, idx);
    int ob = _sv0t29;
    int _sv0t30 = (depth + 1);
    int _sv0t31 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, ob, _sv0t30);
    return _sv0t31;
  } else {
  }
  if ((tag == 7)) {
    int _sv0t32 = sv0_vec_get(ed1, idx);
    int ob7 = _sv0t32;
    int _sv0t33 = (depth + 1);
    int _sv0t34 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, ob7, _sv0t33);
    return _sv0t34;
  } else {
  }
  if ((tag == 8)) {
    int _sv0t35 = sv0_vec_get(ed1, idx);
    int o8 = _sv0t35;
    int _sv0t36 = sv0_vec_get(ed2, idx);
    int i8 = _sv0t36;
    int _sv0t37 = (depth + 1);
    int _sv0t38 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, o8, _sv0t37);
    if ((_sv0t38 == 1)) {
      return 1;
    } else {
    }
    int _sv0t39 = (depth + 1);
    int _sv0t40 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, i8, _sv0t39);
    return _sv0t40;
  } else {
  }
  if ((tag == 9)) {
    int _sv0t41 = sv0_vec_get(ed1, idx);
    int sf9 = _sv0t41;
    int _sv0t42 = sv0_vec_get(ed2, idx);
    int sc9 = _sv0t42;
    int _sv0t43 = sv0_vec_get(ed3, idx);
    int tail9 = _sv0t43;
    int lo9 = sf9;
    int si9 = 0;
    while ((si9 < sc9)) {
      int _sv0t44 = link_expr_find_root_covering_min(expr_tags, ed1, ed2, ed3, ed4, lo9, idx);
      int sr9 = _sv0t44;
      if ((sr9 < 0)) {
        return 0;
      } else {
      }
      int _sv0t45 = (depth + 1);
      int _sv0t46 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, sr9, _sv0t45);
      if ((_sv0t46 == 1)) {
        return 1;
      } else {
      }
      lo9 = (sr9 + 1);
      si9 = (si9 + 1);
    }
    if ((tail9 >= 0)) {
      int _sv0t47 = (depth + 1);
      int _sv0t48 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, tail9, _sv0t47);
      return _sv0t48;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t49 = sv0_vec_get(ed1, idx);
    int c10 = _sv0t49;
    int _sv0t50 = sv0_vec_get(ed2, idx);
    int t10 = _sv0t50;
    int _sv0t51 = sv0_vec_get(ed3, idx);
    int e10 = _sv0t51;
    int _sv0t52 = (depth + 1);
    int _sv0t53 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, c10, _sv0t52);
    if ((_sv0t53 == 1)) {
      return 1;
    } else {
    }
    int _sv0t54 = (depth + 1);
    int _sv0t55 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, t10, _sv0t54);
    if ((_sv0t55 == 1)) {
      return 1;
    } else {
    }
    if ((e10 >= 0)) {
      int _sv0t56 = (depth + 1);
      int _sv0t57 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, e10, _sv0t56);
      return _sv0t57;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t58 = sv0_vec_get(ed1, idx);
    int scr11 = _sv0t58;
    int _sv0t59 = sv0_vec_get(ed2, idx);
    int af11 = _sv0t59;
    int _sv0t60 = sv0_vec_get(ed3, idx);
    int ac11 = _sv0t60;
    int _sv0t61 = (depth + 1);
    int _sv0t62 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, scr11, _sv0t61);
    if ((_sv0t62 == 1)) {
      return 1;
    } else {
    }
    int lo11 = af11;
    int ai11 = 0;
    while ((ai11 < ac11)) {
      int _sv0t63 = link_expr_find_root_covering_min(expr_tags, ed1, ed2, ed3, ed4, lo11, idx);
      int ar11 = _sv0t63;
      if ((ar11 < 0)) {
        return 0;
      } else {
      }
      int _sv0t64 = (depth + 1);
      int _sv0t65 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, ar11, _sv0t64);
      if ((_sv0t65 == 1)) {
        return 1;
      } else {
      }
      lo11 = (ar11 + 1);
      ai11 = (ai11 + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t66 = sv0_vec_get(ed1, idx);
    int c12 = _sv0t66;
    int _sv0t67 = sv0_vec_get(ed2, idx);
    int b12 = _sv0t67;
    int _sv0t68 = sv0_vec_get(ed3, idx);
    int invf12 = _sv0t68;
    int _sv0t69 = sv0_vec_get(ed4, idx);
    int invc12 = _sv0t69;
    int _sv0t70 = (depth + 1);
    int _sv0t71 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, c12, _sv0t70);
    if ((_sv0t71 == 1)) {
      return 1;
    } else {
    }
    int _sv0t72 = (depth + 1);
    int _sv0t73 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, b12, _sv0t72);
    if ((_sv0t73 == 1)) {
      return 1;
    } else {
    }
    if ((invf12 >= 0)) {
      int lo12 = invf12;
      int ii12 = 0;
      while ((ii12 < invc12)) {
        int _sv0t74 = link_expr_find_root_covering_min(expr_tags, ed1, ed2, ed3, ed4, lo12, idx);
        int ir12 = _sv0t74;
        if ((ir12 < 0)) {
          return 0;
        } else {
        }
        int _sv0t75 = (depth + 1);
        int _sv0t76 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, ir12, _sv0t75);
        if ((_sv0t76 == 1)) {
          return 1;
        } else {
        }
        lo12 = (ir12 + 1);
        ii12 = (ii12 + 1);
      }
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t77 = sv0_vec_get(ed2, idx);
    int it13 = _sv0t77;
    int _sv0t78 = sv0_vec_get(ed3, idx);
    int bd13 = _sv0t78;
    int _sv0t79 = (depth + 1);
    int _sv0t80 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, it13, _sv0t79);
    if ((_sv0t80 == 1)) {
      return 1;
    } else {
    }
    int _sv0t81 = (depth + 1);
    int _sv0t82 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, bd13, _sv0t81);
    return _sv0t82;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t83 = sv0_vec_get(ed1, idx);
    int cs = _sv0t83;
    int _sv0t84 = sv0_vec_get(ed2, idx);
    int tyi = _sv0t84;
    int _sv0t85 = (depth + 1);
    int _sv0t86 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, cs, _sv0t85);
    if ((_sv0t86 == 1)) {
      return 1;
    } else {
    }
    int _sv0t87 = sv0_vec_len(ty_tags);
    int _sv0t88 = (_sv0t87 > tyi);
    int _sv0t89 = (tyi >= 0);
    if ((_sv0t88 && _sv0t89)) {
      int _sv0t90 = link_ty_after_ref_slice_chain_tyname_needs_mangle(ty_tags, ty_d1, ty_d2, ty_d3, pp, tops, tyi);
      if ((_sv0t90 == 1)) {
        return 1;
      } else {
      }
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 27)) {
    int _sv0t91 = sv0_vec_get(ed3, idx);
    int init27 = _sv0t91;
    if ((init27 < 0)) {
      return 0;
    } else {
    }
    int _sv0t92 = (depth + 1);
    int _sv0t93 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, init27, _sv0t92);
    return _sv0t93;
  } else {
  }
  if ((tag == 28)) {
    int _sv0t94 = sv0_vec_get(ed1, idx);
    int s28 = _sv0t94;
    int _sv0t95 = (depth + 1);
    int _sv0t96 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, s28, _sv0t95);
    return _sv0t96;
  } else {
  }
  if ((tag == 29)) {
    int _sv0t97 = sv0_vec_get(ed3, idx);
    int g29 = _sv0t97;
    int _sv0t98 = sv0_vec_get(ed4, idx);
    int b29 = _sv0t98;
    if ((g29 >= 0)) {
      int _sv0t99 = (depth + 1);
      int _sv0t100 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, g29, _sv0t99);
      if ((_sv0t100 == 1)) {
        return 1;
      } else {
      }
    } else {
    }
    int _sv0t101 = (depth + 1);
    int _sv0t102 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, b29, _sv0t101);
    return _sv0t102;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t103 = sv0_vec_get(ed3, idx);
    int fc = _sv0t103;
    int fi = 0;
    while ((fi < fc)) {
      int _sv0t104 = (idx - fc);
      int fr = (_sv0t104 + fi);
      int _sv0t105 = (depth + 1);
      int _sv0t106 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, fr, _sv0t105);
      if ((_sv0t106 == 1)) {
        return 1;
      } else {
      }
      fi = (fi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t107 = sv0_vec_get(ed1, idx);
    int first = _sv0t107;
    int _sv0t108 = sv0_vec_get(ed2, idx);
    int cnt = _sv0t108;
    int cur = first;
    int j = 0;
    while ((j < cnt)) {
      int _sv0t109 = (depth + 1);
      int _sv0t110 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, cur, _sv0t109);
      if ((_sv0t110 == 1)) {
        return 1;
      } else {
      }
      int _sv0t111 = link_expr_subtree_node_count(expr_tags, ed1, ed2, ed3, ed4, cur, 0);
      int sc = _sv0t111;
      if ((sc <= 0)) {
        return 0;
      } else {
      }
      cur = (cur + sc);
      j = (j + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 26)) {
    int _sv0t112 = sv0_vec_get(ed2, idx);
    int ec = _sv0t112;
    int k = 0;
    while ((k < ec)) {
      int _sv0t113 = (idx - ec);
      int er = (_sv0t113 + k);
      int _sv0t114 = (depth + 1);
      int _sv0t115 = link_expr_root_needs_mangle(expr_tags, ed1, ed2, ed3, ed4, pp, ty_tags, ty_d1, ty_d2, ty_d3, tops, er, _sv0t114);
      if ((_sv0t115 == 1)) {
        return 1;
      } else {
      }
      k = (k + 1);
    }
    return 0;
  } else {
  }
  return 0;
}

static int link_ty_subtree_node_count(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int idx, int depth) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  if ((depth > 64)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(ty_tags, idx);
  int t = _sv0t2;
  if ((t == 6)) {
    return 1;
  } else {
  }
  if ((t == 0)) {
    int _sv0t3 = sv0_vec_get(ty_d3, idx);
    int nargs = _sv0t3;
    if ((nargs == 0)) {
      return 1;
    } else {
    }
    int pos = (idx - 1);
    int sum = 1;
    int i = 0;
    while ((i < nargs)) {
      if ((pos < 0)) {
        int _sv0t4 = (0 - 1);
        return _sv0t4;
      } else {
      }
      int _sv0t5 = (depth + 1);
      int _sv0t6 = link_ty_subtree_node_count(ty_tags, ty_d1, ty_d2, ty_d3, pos, _sv0t5);
      int sc = _sv0t6;
      if ((sc <= 0)) {
        int _sv0t7 = (0 - 1);
        return _sv0t7;
      } else {
      }
      sum = (sum + sc);
      pos = (pos - sc);
      i = (i + 1);
    }
    return sum;
  } else {
  }
  if ((t == 1)) {
    int _sv0t8 = sv0_vec_get(ty_d1, idx);
    int inn = _sv0t8;
    int _sv0t9 = (depth + 1);
    int _sv0t10 = link_ty_subtree_node_count(ty_tags, ty_d1, ty_d2, ty_d3, inn, _sv0t9);
    int sc = _sv0t10;
    if ((sc <= 0)) {
      int _sv0t11 = (0 - 1);
      return _sv0t11;
    } else {
    }
    int _sv0t12 = (1 + sc);
    return _sv0t12;
  } else {
  }
  if ((t == 2)) {
    int _sv0t13 = sv0_vec_get(ty_d1, idx);
    int inn2 = _sv0t13;
    int _sv0t14 = (depth + 1);
    int _sv0t15 = link_ty_subtree_node_count(ty_tags, ty_d1, ty_d2, ty_d3, inn2, _sv0t14);
    int sc2 = _sv0t15;
    if ((sc2 <= 0)) {
      int _sv0t16 = (0 - 1);
      return _sv0t16;
    } else {
    }
    int _sv0t17 = (1 + sc2);
    return _sv0t17;
  } else {
  }
  if ((t == 3)) {
    int _sv0t18 = sv0_vec_get(ty_d1, idx);
    int el = _sv0t18;
    int _sv0t19 = (depth + 1);
    int _sv0t20 = link_ty_subtree_node_count(ty_tags, ty_d1, ty_d2, ty_d3, el, _sv0t19);
    int sc3 = _sv0t20;
    if ((sc3 <= 0)) {
      int _sv0t21 = (0 - 1);
      return _sv0t21;
    } else {
    }
    int _sv0t22 = (1 + sc3);
    return _sv0t22;
  } else {
  }
  if ((t == 4)) {
    int _sv0t23 = sv0_vec_get(ty_d1, idx);
    int sl = _sv0t23;
    int _sv0t24 = (depth + 1);
    int _sv0t25 = link_ty_subtree_node_count(ty_tags, ty_d1, ty_d2, ty_d3, sl, _sv0t24);
    int sc4 = _sv0t25;
    if ((sc4 <= 0)) {
      int _sv0t26 = (0 - 1);
      return _sv0t26;
    } else {
    }
    int _sv0t27 = (1 + sc4);
    return _sv0t27;
  } else {
  }
  if ((t == 5)) {
    int _sv0t28 = sv0_vec_get(ty_d1, idx);
    int first = _sv0t28;
    int _sv0t29 = sv0_vec_get(ty_d2, idx);
    int cnt = _sv0t29;
    int cur = first;
    int sumt = 1;
    int j = 0;
    while ((j < cnt)) {
      int _sv0t30 = (depth + 1);
      int _sv0t31 = link_ty_subtree_node_count(ty_tags, ty_d1, ty_d2, ty_d3, cur, _sv0t30);
      int sce = _sv0t31;
      if ((sce <= 0)) {
        int _sv0t32 = (0 - 1);
        return _sv0t32;
      } else {
      }
      sumt = (sumt + sce);
      cur = (cur + sce);
      j = (j + 1);
    }
    return sumt;
  } else {
  }
  int _sv0t33 = (0 - 1);
  return _sv0t33;
}

static int link_ty_peel_ref_mut_slice(int ty_tags, int ty_d1, int idx) {
  int cur = idx;
  int guard = 0;
  while ((guard < 64)) {
    int _sv0t0 = sv0_vec_get(ty_tags, cur);
    int tg = _sv0t0;
    if ((tg == 1)) {
      int _sv0t1 = sv0_vec_get(ty_d1, cur);
      cur = _sv0t1;
      guard = (guard + 1);
    } else {
      if ((tg == 2)) {
        int _sv0t2 = sv0_vec_get(ty_d1, cur);
        cur = _sv0t2;
        guard = (guard + 1);
      } else {
        if ((tg == 4)) {
          int _sv0t3 = sv0_vec_get(ty_d1, cur);
          cur = _sv0t3;
          guard = (guard + 1);
        } else {
          return cur;
        }
      }
    }
  }
  return cur;
}

static int link_ty_after_ref_slice_chain_tyname_needs_mangle(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int pp, int tops, int idx) {
  int cur = idx;
  int arr_guard = 0;
  while ((arr_guard < 32)) {
    int _sv0t0 = link_ty_peel_ref_mut_slice(ty_tags, ty_d1, cur);
    int base = _sv0t0;
    int _sv0t1 = sv0_vec_get(ty_tags, base);
    int bt = _sv0t1;
    if ((bt == 0)) {
      int _sv0t2 = link_ty_tyname_path_needs_mangle(ty_tags, ty_d1, ty_d2, pp, tops, base);
      if ((_sv0t2 == 1)) {
        return 1;
      } else {
      }
      int _sv0t3 = sv0_vec_get(ty_d3, base);
      int nargs = _sv0t3;
      if ((nargs == 0)) {
        return 0;
      } else {
      }
      int pos = (base - 1);
      int ai = 0;
      while ((ai < nargs)) {
        int _sv0t4 = link_ty_after_ref_slice_chain_tyname_needs_mangle(ty_tags, ty_d1, ty_d2, ty_d3, pp, tops, pos);
        if ((_sv0t4 == 1)) {
          return 1;
        } else {
        }
        int _sv0t5 = link_ty_subtree_node_count(ty_tags, ty_d1, ty_d2, ty_d3, pos, 0);
        int sc = _sv0t5;
        if ((sc <= 0)) {
          return 0;
        } else {
        }
        pos = (pos - sc);
        ai = (ai + 1);
      }
      return 0;
    } else {
    }
    if ((bt == 3)) {
      int _sv0t6 = sv0_vec_get(ty_d1, base);
      cur = _sv0t6;
      arr_guard = (arr_guard + 1);
    } else {
      if ((bt == 5)) {
        int _sv0t7 = sv0_vec_get(ty_d1, base);
        int first = _sv0t7;
        int _sv0t8 = sv0_vec_get(ty_d2, base);
        int cnt = _sv0t8;
        int er = first;
        int ei = 0;
        while ((ei < cnt)) {
          int _sv0t9 = link_ty_after_ref_slice_chain_tyname_needs_mangle(ty_tags, ty_d1, ty_d2, ty_d3, pp, tops, er);
          if ((_sv0t9 == 1)) {
            return 1;
          } else {
          }
          int _sv0t10 = link_ty_subtree_node_count(ty_tags, ty_d1, ty_d2, ty_d3, er, 0);
          int se = _sv0t10;
          if ((se <= 0)) {
            return 0;
          } else {
          }
          er = (er + se);
          ei = (ei + 1);
        }
        return 0;
      } else {
      }
      return 0;
    }
  }
  return 0;
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

static int test_is_link_source_file(void) {
  int _sv0t0 = is_link_source_file("main.sv0");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_link_source_file("lib.sv0");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_link_source_file(".main.sv0");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_link_source_file("README.md");
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_link_source_file("sv0");
  if ((_sv0t4 != 0)) {
    return 5;
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
  const char* _sv0t4 = file_stem("src\\bar.sv0");
  int _sv0t5 = sv0_string_eq(_sv0t4, "bar");
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  const char* _sv0t6 = file_stem("bar");
  int _sv0t7 = sv0_string_eq(_sv0t6, "bar");
  if ((_sv0t7 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t8 = file_stem("/a/b/c.txt");
  int _sv0t9 = sv0_string_eq(_sv0t8, "c");
  if ((_sv0t9 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t10 = file_stem("noext");
  int _sv0t11 = sv0_string_eq(_sv0t10, "noext");
  if ((_sv0t11 != 1)) {
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
  const char* _sv0t2 = link_dir_from_entry("src\\foo.sv0");
  int _sv0t3 = sv0_string_eq(_sv0t2, "src");
  if ((_sv0t3 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t4 = link_dir_from_entry("foo.sv0");
  int _sv0t5 = sv0_string_eq(_sv0t4, ".");
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t6 = link_dir_from_entry("/a/b/c.sv0");
  int _sv0t7 = sv0_string_eq(_sv0t6, "/a/b");
  if ((_sv0t7 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t8 = link_dir_from_entry("/foo.sv0");
  int _sv0t9 = sv0_string_eq(_sv0t8, "/");
  if ((_sv0t9 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_module_id_from_entry_path(void) {
  const char* _sv0t0 = module_id_from_entry_path("src/foo.sv0");
  int _sv0t1 = sv0_string_eq(_sv0t0, "foo");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = module_id_from_entry_path("main.sv0");
  int _sv0t3 = sv0_string_eq(_sv0t2, "main");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = module_id_from_entry_path("/a/b/c.sv0");
  int _sv0t5 = sv0_string_eq(_sv0t4, "c");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = module_id_from_entry_path("plain");
  int _sv0t7 = sv0_string_eq(_sv0t6, "plain");
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

static int test_g2_link_host_io_aliases(void) {
  const char* p;
  p = "/tmp/sv0_g2_link_parse.sv0";
  sv0_write_file(p, "probe");
  const char* _sv0t0 = parse_file(p);
  const char* s;
  s = _sv0t0;
  int _sv0t1 = sv0_string_eq(s, "probe");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_link_g6_empty_listing(void) {
  int _sv0t0 = link_listing_nonempty("");
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = link_listing_nonempty("a.sv0\n");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = LINK_ERR_NO_SV0_IN_DIR();
  if ((_sv0t2 != 321)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_link_project_concat_sources_from_listing(void) {
  const char* p1;
  p1 = "/tmp/sv0_link_concat_a.sv0";
  const char* p2;
  p2 = "/tmp/sv0_link_concat_b.sv0";
  sv0_write_file(p1, "A");
  sv0_write_file(p2, "B");
  const char* listing;
  listing = p1;
  const char* _sv0t0 = sv0_string_concat(listing, "\n");
  listing = _sv0t0;
  const char* _sv0t1 = sv0_string_concat(listing, p2);
  listing = _sv0t1;
  const char* _sv0t2 = sv0_string_concat(listing, "\n");
  listing = _sv0t2;
  const char* _sv0t3 = link_project_concat_sources_from_listing(listing);
  const char* out;
  out = _sv0t3;
  int _sv0t4 = sv0_string_eq(out, "A\nB");
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t5 = link_project_concat_sources_from_listing("");
  const char* out2;
  out2 = _sv0t5;
  int _sv0t6 = sv0_string_eq(out2, "");
  if ((_sv0t6 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_listing_count_nonempty_paths(void) {
  int _sv0t0 = listing_count_nonempty_paths("");
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = listing_count_nonempty_paths("\n\n");
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = listing_count_nonempty_paths("a.sv0");
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = listing_count_nonempty_paths("a.sv0\nb.sv0");
  if ((_sv0t3 != 2)) {
    return 4;
  } else {
  }
  int _sv0t4 = listing_count_nonempty_paths("a.sv0\n\nb.sv0");
  if ((_sv0t4 != 2)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_link_ty_tyname_path_needs_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int d1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int d2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int d3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  sv0_vec_push(tt, 0);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(pp, 42);
  int _sv0t5 = sv0_vec_new();
  int tops_hit = _sv0t5;
  sv0_vec_push(tops_hit, 42);
  int _sv0t6 = link_ty_tyname_path_needs_mangle(tt, d1, d2, pp, tops_hit, 0);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int tops_miss = _sv0t7;
  int _sv0t8 = link_ty_tyname_path_needs_mangle(tt, d1, d2, pp, tops_miss, 0);
  if ((_sv0t8 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_path_pool_prefix_needs_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int pp = _sv0t0;
  sv0_vec_push(pp, 33);
  int _sv0t1 = sv0_vec_new();
  int tops_hit = _sv0t1;
  sv0_vec_push(tops_hit, 33);
  int _sv0t2 = link_path_pool_prefix_needs_mangle(pp, tops_hit, 0, 1);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int tops_miss = _sv0t3;
  int _sv0t4 = link_path_pool_prefix_needs_mangle(pp, tops_miss, 0, 1);
  if ((_sv0t4 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_expr_path_needs_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int e1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int e2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pp = _sv0t3;
  sv0_vec_push(et, 1);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(pp, 88);
  int _sv0t4 = sv0_vec_new();
  int tops_hit = _sv0t4;
  sv0_vec_push(tops_hit, 88);
  int _sv0t5 = link_expr_path_needs_mangle(et, e1, e2, pp, tops_hit, 0);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int tops_miss = _sv0t6;
  int _sv0t7 = link_expr_path_needs_mangle(et, e1, e2, pp, tops_miss, 0);
  if ((_sv0t7 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_expr_subtree_node_count_binop(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int e1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int e2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int e3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int e4 = _sv0t4;
  sv0_vec_push(et, 0);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 0);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 0);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 3);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 0);
  sv0_vec_push(e3, 1);
  sv0_vec_push(e4, 0);
  int _sv0t5 = link_expr_subtree_node_count(et, e1, e2, e3, e4, 2, 0);
  int c = _sv0t5;
  if ((c != 3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_link_expr_call_root_needs_mangle_two_args(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int e1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int e2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int e3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int e4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 7);
  sv0_vec_push(pp, 8);
  sv0_vec_push(et, 1);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(e1, 1);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 0);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 4);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 2);
  sv0_vec_push(e4, 0);
  int _sv0t6 = sv0_vec_new();
  int tops_hit = _sv0t6;
  sv0_vec_push(tops_hit, 8);
  int _sv0t7 = sv0_vec_new();
  int ty_e = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ty_d1 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ty_d2 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ty_d3 = _sv0t10;
  int _sv0t11 = link_expr_root_needs_mangle(et, e1, e2, e3, e4, pp, ty_e, ty_d1, ty_d2, ty_d3, tops_hit, 3, 0);
  if ((_sv0t11 != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int tops_miss = _sv0t12;
  int _sv0t13 = link_expr_root_needs_mangle(et, e1, e2, e3, e4, pp, ty_e, ty_d1, ty_d2, ty_d3, tops_miss, 3, 0);
  if ((_sv0t13 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_expr_if_needs_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int e1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int e2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int e3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int e4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 1);
  sv0_vec_push(pp, 2);
  sv0_vec_push(et, 1);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(e1, 1);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 10);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  int _sv0t6 = (0 - 1);
  sv0_vec_push(e3, _sv0t6);
  sv0_vec_push(e4, 0);
  int _sv0t7 = link_expr_subtree_node_count(et, e1, e2, e3, e4, 2, 0);
  int cnt = _sv0t7;
  if ((cnt != 3)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int tops = _sv0t8;
  sv0_vec_push(tops, 2);
  int _sv0t9 = sv0_vec_new();
  int tty = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int td1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int td2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int td3 = _sv0t12;
  int _sv0t13 = link_expr_root_needs_mangle(et, e1, e2, e3, e4, pp, tty, td1, td2, td3, tops, 2, 0);
  if ((_sv0t13 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_expr_block_stmt_needs_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int e1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int e2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int e3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int e4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 9);
  sv0_vec_push(et, 1);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 28);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 0);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 9);
  sv0_vec_push(e1, 1);
  sv0_vec_push(e2, 1);
  int _sv0t6 = (0 - 1);
  sv0_vec_push(e3, _sv0t6);
  sv0_vec_push(e4, 0);
  int _sv0t7 = link_expr_subtree_node_count(et, e1, e2, e3, e4, 2, 0);
  int cnt = _sv0t7;
  if ((cnt != 3)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int tops = _sv0t8;
  sv0_vec_push(tops, 9);
  int _sv0t9 = sv0_vec_new();
  int tty = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int td1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int td2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int td3 = _sv0t12;
  int _sv0t13 = link_expr_root_needs_mangle(et, e1, e2, e3, e4, pp, tty, td1, td2, td3, tops, 2, 0);
  if ((_sv0t13 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_expr_match_arm_needs_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int e1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int e2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int e3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int e4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 40);
  sv0_vec_push(pp, 41);
  sv0_vec_push(et, 1);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(e1, 1);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et, 29);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 0);
  int _sv0t6 = (0 - 1);
  sv0_vec_push(e3, _sv0t6);
  sv0_vec_push(e4, 1);
  sv0_vec_push(et, 11);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 2);
  sv0_vec_push(e3, 1);
  sv0_vec_push(e4, 0);
  int _sv0t7 = link_expr_subtree_node_count(et, e1, e2, e3, e4, 3, 0);
  int cnt = _sv0t7;
  if ((cnt != 4)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int tops = _sv0t8;
  sv0_vec_push(tops, 41);
  int _sv0t9 = sv0_vec_new();
  int tty = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int td1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int td2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int td3 = _sv0t12;
  int _sv0t13 = link_expr_root_needs_mangle(et, e1, e2, e3, e4, pp, tty, td1, td2, td3, tops, 3, 0);
  if ((_sv0t13 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_pat_subtree_node_count_wild(void) {
  int _sv0t0 = sv0_vec_new();
  int pt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int pd1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int pd2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pd3 = _sv0t3;
  sv0_vec_push(pt, 0);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 0);
  sv0_vec_push(pd3, 0);
  int _sv0t4 = link_pat_subtree_node_count(pt, pd1, pd2, pd3, 0, 0);
  int c = _sv0t4;
  if ((c != 1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_link_pat_subtree_tuple_two_wild(void) {
  int _sv0t0 = sv0_vec_new();
  int pt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int pd1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int pd2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pd3 = _sv0t3;
  sv0_vec_push(pt, 0);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 0);
  sv0_vec_push(pd3, 0);
  sv0_vec_push(pt, 0);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 0);
  sv0_vec_push(pd3, 0);
  sv0_vec_push(pt, 3);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 2);
  sv0_vec_push(pd3, 0);
  int _sv0t4 = link_pat_subtree_node_count(pt, pd1, pd2, pd3, 2, 0);
  int c = _sv0t4;
  if ((c != 3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_link_pat_struct_path_needs_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int pt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int pd1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int pd2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pd3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  sv0_vec_push(pp, 77);
  sv0_vec_push(pt, 0);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 0);
  sv0_vec_push(pd3, 0);
  sv0_vec_push(pt, 0);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 0);
  sv0_vec_push(pd3, 0);
  sv0_vec_push(pt, 4);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 1);
  sv0_vec_push(pd3, 2);
  int _sv0t5 = sv0_vec_new();
  int tops_hit = _sv0t5;
  sv0_vec_push(tops_hit, 77);
  int _sv0t6 = link_pat_root_needs_mangle(pt, pd1, pd2, pd3, pp, tops_hit, 2, 0);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int tops_miss = _sv0t7;
  int _sv0t8 = link_pat_root_needs_mangle(pt, pd1, pd2, pd3, pp, tops_miss, 2, 0);
  if ((_sv0t8 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_ty_ref_chain_tyname_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int d1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int d2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int d3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  sv0_vec_push(tt, 0);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(pp, 99);
  sv0_vec_push(tt, 1);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 0);
  sv0_vec_push(d3, 0);
  int _sv0t5 = sv0_vec_new();
  int tops_hit = _sv0t5;
  sv0_vec_push(tops_hit, 99);
  int _sv0t6 = link_ty_after_ref_slice_chain_tyname_needs_mangle(tt, d1, d2, d3, pp, tops_hit, 1);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int tops_miss = _sv0t7;
  int _sv0t8 = link_ty_after_ref_slice_chain_tyname_needs_mangle(tt, d1, d2, d3, pp, tops_miss, 1);
  if ((_sv0t8 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_ty_array_tyname_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int d1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int d2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int d3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  sv0_vec_push(tt, 0);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(pp, 77);
  sv0_vec_push(tt, 3);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 5);
  sv0_vec_push(d3, 0);
  sv0_vec_push(pp, 88);
  int _sv0t5 = sv0_vec_new();
  int tops_hit = _sv0t5;
  sv0_vec_push(tops_hit, 77);
  int _sv0t6 = link_ty_after_ref_slice_chain_tyname_needs_mangle(tt, d1, d2, d3, pp, tops_hit, 1);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int tops_miss = _sv0t7;
  int _sv0t8 = link_ty_after_ref_slice_chain_tyname_needs_mangle(tt, d1, d2, d3, pp, tops_miss, 1);
  if ((_sv0t8 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_ty_tuple_two_tyname_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int d1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int d2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int d3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  sv0_vec_push(tt, 0);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(pp, 40);
  sv0_vec_push(tt, 0);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(pp, 41);
  sv0_vec_push(tt, 5);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 2);
  sv0_vec_push(d3, 0);
  sv0_vec_push(pp, 0);
  int _sv0t5 = sv0_vec_new();
  int tops_hit = _sv0t5;
  sv0_vec_push(tops_hit, 40);
  int _sv0t6 = link_ty_after_ref_slice_chain_tyname_needs_mangle(tt, d1, d2, d3, pp, tops_hit, 2);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int tops_miss = _sv0t7;
  int _sv0t8 = link_ty_after_ref_slice_chain_tyname_needs_mangle(tt, d1, d2, d3, pp, tops_miss, 2);
  if ((_sv0t8 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_link_ty_generic_arg_path_mangle(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int d1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int d2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int d3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  sv0_vec_push(tt, 0);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(pp, 55);
  sv0_vec_push(tt, 0);
  sv0_vec_push(d1, 1);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 1);
  sv0_vec_push(pp, 66);
  int _sv0t5 = sv0_vec_new();
  int tops_hit = _sv0t5;
  sv0_vec_push(tops_hit, 55);
  int _sv0t6 = link_ty_after_ref_slice_chain_tyname_needs_mangle(tt, d1, d2, d3, pp, tops_hit, 1);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int tops_outer_only = _sv0t7;
  sv0_vec_push(tops_outer_only, 66);
  int _sv0t8 = link_ty_after_ref_slice_chain_tyname_needs_mangle(tt, d1, d2, d3, pp, tops_outer_only, 1);
  if ((_sv0t8 != 1)) {
    return 2;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int tops_none = _sv0t9;
  int _sv0t10 = link_ty_after_ref_slice_chain_tyname_needs_mangle(tt, d1, d2, d3, pp, tops_none, 1);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_link_ty_tyname_first_seg_preview(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int d1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int d2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pp = _sv0t3;
  sv0_vec_push(tt, 0);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(pp, 0);
  int _sv0t4 = sv0_vec_new();
  int starts = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ends = _sv0t5;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  int _sv0t6 = sv0_vec_new();
  int tops_hit = _sv0t6;
  sv0_vec_push(tops_hit, 0);
  const char* _sv0t7 = link_ty_tyname_first_seg_preview(tt, d1, d2, pp, tops_hit, "mod", starts, ends, "Foo", 0);
  const char* s1;
  s1 = _sv0t7;
  int _sv0t8 = sv0_string_eq(s1, "mod__Foo");
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int tops_miss = _sv0t9;
  sv0_vec_push(tops_miss, 99);
  const char* _sv0t10 = link_ty_tyname_first_seg_preview(tt, d1, d2, pp, tops_miss, "mod", starts, ends, "Foo", 0);
  const char* s2;
  s2 = _sv0t10;
  int _sv0t11 = sv0_string_eq(s2, "Foo");
  if ((_sv0t11 != 1)) {
    return 2;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int tt2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int d1b = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int d2b = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ppb = _sv0t15;
  sv0_vec_push(tt2, 0);
  sv0_vec_push(d1b, 0);
  sv0_vec_push(d2b, 2);
  sv0_vec_push(ppb, 0);
  sv0_vec_push(ppb, 1);
  int _sv0t16 = sv0_vec_new();
  int st2 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int en2 = _sv0t17;
  sv0_vec_push(st2, 0);
  sv0_vec_push(en2, 3);
  sv0_vec_push(st2, 5);
  sv0_vec_push(en2, 8);
  const char* src2;
  src2 = "Foo::Bar";
  int _sv0t18 = sv0_vec_new();
  int tops2 = _sv0t18;
  sv0_vec_push(tops2, 0);
  const char* _sv0t19 = link_ty_tyname_first_seg_preview(tt2, d1b, d2b, ppb, tops2, "m", st2, en2, src2, 0);
  const char* s3;
  s3 = _sv0t19;
  int _sv0t20 = sv0_string_eq(s3, "m__Foo");
  if ((_sv0t20 != 1)) {
    return 3;
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
  int _sv0t3 = test_is_link_source_file();
  int r2b = _sv0t3;
  if ((r2b != 0)) {
    int _sv0t4 = (15 + r2b);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_file_stem();
  int r3 = _sv0t5;
  if ((r3 != 0)) {
    int _sv0t6 = (20 + r3);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_mangle();
  int r4 = _sv0t7;
  if ((r4 != 0)) {
    int _sv0t8 = (30 + r4);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_is_top_defining();
  int r5 = _sv0t9;
  if ((r5 != 0)) {
    int _sv0t10 = (40 + r5);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_collect_tops_count();
  int r6 = _sv0t11;
  if ((r6 != 0)) {
    int _sv0t12 = (50 + r6);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_is_link_directive();
  int r7 = _sv0t13;
  if ((r7 != 0)) {
    int _sv0t14 = (60 + r7);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_strip_directives_count();
  int r8 = _sv0t15;
  if ((r8 != 0)) {
    int _sv0t16 = (70 + r8);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_has_module_prefix();
  int r9 = _sv0t17;
  if ((r9 != 0)) {
    int _sv0t18 = (80 + r9);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_in_tops();
  int r10 = _sv0t19;
  if ((r10 != 0)) {
    int _sv0t20 = (90 + r10);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_path_seg_needs_mangle();
  int r11 = _sv0t21;
  if ((r11 != 0)) {
    int _sv0t22 = (100 + r11);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_split_module();
  int r12 = _sv0t23;
  if ((r12 != 0)) {
    int _sv0t24 = (110 + r12);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_map_ty_kind();
  int r13 = _sv0t25;
  if ((r13 != 0)) {
    int _sv0t26 = (120 + r13);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = test_item_rewrite();
  int r14 = _sv0t27;
  if ((r14 != 0)) {
    int _sv0t28 = (140 + r14);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = test_fn_name_for_link();
  int r15 = _sv0t29;
  if ((r15 != 0)) {
    int _sv0t30 = (150 + r15);
    return _sv0t30;
  } else {
  }
  int _sv0t31 = test_link_dir_from_entry();
  int r16 = _sv0t31;
  if ((r16 != 0)) {
    int _sv0t32 = (160 + r16);
    return _sv0t32;
  } else {
  }
  int _sv0t33 = test_module_id_from_entry_path();
  int r16b = _sv0t33;
  if ((r16b != 0)) {
    int _sv0t34 = (165 + r16b);
    return _sv0t34;
  } else {
  }
  int _sv0t35 = test_map_path_segs();
  int r17 = _sv0t35;
  if ((r17 != 0)) {
    int _sv0t36 = (170 + r17);
    return _sv0t36;
  } else {
  }
  int _sv0t37 = test_collect_top_names();
  int r18 = _sv0t37;
  if ((r18 != 0)) {
    int _sv0t38 = (180 + r18);
    return _sv0t38;
  } else {
  }
  int _sv0t39 = test_map_path_segs_rewrite();
  int r19 = _sv0t39;
  if ((r19 != 0)) {
    int _sv0t40 = (190 + r19);
    return _sv0t40;
  } else {
  }
  int _sv0t41 = test_strip_link_directives();
  int r20 = _sv0t41;
  if ((r20 != 0)) {
    int _sv0t42 = (200 + r20);
    return _sv0t42;
  } else {
  }
  int _sv0t43 = test_split_module_name();
  int r21 = _sv0t43;
  if ((r21 != 0)) {
    int _sv0t44 = (210 + r21);
    return _sv0t44;
  } else {
  }
  int _sv0t45 = test_link_g6_empty_listing();
  int r21b = _sv0t45;
  if ((r21b != 0)) {
    int _sv0t46 = (215 + r21b);
    return _sv0t46;
  } else {
  }
  int _sv0t47 = test_link_project_concat_sources_from_listing();
  int r21b2 = _sv0t47;
  if ((r21b2 != 0)) {
    int _sv0t48 = (246 + r21b2);
    return _sv0t48;
  } else {
  }
  int _sv0t49 = test_listing_count_nonempty_paths();
  int r21b2a = _sv0t49;
  if ((r21b2a != 0)) {
    int _sv0t50 = (247 + r21b2a);
    return _sv0t50;
  } else {
  }
  int _sv0t51 = test_link_ty_tyname_path_needs_mangle();
  int r21c = _sv0t51;
  if ((r21c != 0)) {
    int _sv0t52 = (217 + r21c);
    return _sv0t52;
  } else {
  }
  int _sv0t53 = test_link_path_pool_prefix_needs_mangle();
  int r21c2 = _sv0t53;
  if ((r21c2 != 0)) {
    int _sv0t54 = (227 + r21c2);
    return _sv0t54;
  } else {
  }
  int _sv0t55 = test_link_expr_path_needs_mangle();
  int r21c3 = _sv0t55;
  if ((r21c3 != 0)) {
    int _sv0t56 = (231 + r21c3);
    return _sv0t56;
  } else {
  }
  int _sv0t57 = test_link_expr_subtree_node_count_binop();
  int r21c3a = _sv0t57;
  if ((r21c3a != 0)) {
    int _sv0t58 = (232 + r21c3a);
    return _sv0t58;
  } else {
  }
  int _sv0t59 = test_link_expr_call_root_needs_mangle_two_args();
  int r21c3b = _sv0t59;
  if ((r21c3b != 0)) {
    int _sv0t60 = (234 + r21c3b);
    return _sv0t60;
  } else {
  }
  int _sv0t61 = test_link_expr_if_needs_mangle();
  int r21c3c = _sv0t61;
  if ((r21c3c != 0)) {
    int _sv0t62 = (236 + r21c3c);
    return _sv0t62;
  } else {
  }
  int _sv0t63 = test_link_expr_block_stmt_needs_mangle();
  int r21c3d = _sv0t63;
  if ((r21c3d != 0)) {
    int _sv0t64 = (239 + r21c3d);
    return _sv0t64;
  } else {
  }
  int _sv0t65 = test_link_expr_match_arm_needs_mangle();
  int r21c3e = _sv0t65;
  if ((r21c3e != 0)) {
    int _sv0t66 = (242 + r21c3e);
    return _sv0t66;
  } else {
  }
  int _sv0t67 = test_link_pat_subtree_node_count_wild();
  int r21c3p = _sv0t67;
  if ((r21c3p != 0)) {
    int _sv0t68 = (248 + r21c3p);
    return _sv0t68;
  } else {
  }
  int _sv0t69 = test_link_pat_subtree_tuple_two_wild();
  int r21c3q = _sv0t69;
  if ((r21c3q != 0)) {
    int _sv0t70 = (251 + r21c3q);
    return _sv0t70;
  } else {
  }
  int _sv0t71 = test_link_pat_struct_path_needs_mangle();
  int r21c3r = _sv0t71;
  if ((r21c3r != 0)) {
    int _sv0t72 = (254 + r21c3r);
    return _sv0t72;
  } else {
  }
  int _sv0t73 = test_link_ty_ref_chain_tyname_mangle();
  int r21d = _sv0t73;
  if ((r21d != 0)) {
    int _sv0t74 = (219 + r21d);
    return _sv0t74;
  } else {
  }
  int _sv0t75 = test_link_ty_array_tyname_mangle();
  int r21e = _sv0t75;
  if ((r21e != 0)) {
    int _sv0t76 = (222 + r21e);
    return _sv0t76;
  } else {
  }
  int _sv0t77 = test_link_ty_tuple_two_tyname_mangle();
  int r21f = _sv0t77;
  if ((r21f != 0)) {
    int _sv0t78 = (226 + r21f);
    return _sv0t78;
  } else {
  }
  int _sv0t79 = test_link_ty_generic_arg_path_mangle();
  int r21g = _sv0t79;
  if ((r21g != 0)) {
    int _sv0t80 = (230 + r21g);
    return _sv0t80;
  } else {
  }
  int _sv0t81 = test_link_ty_tyname_first_seg_preview();
  int r21pv = _sv0t81;
  if ((r21pv != 0)) {
    int _sv0t82 = (260 + r21pv);
    return _sv0t82;
  } else {
  }
  int _sv0t83 = test_g2_link_host_io_aliases();
  int r22 = _sv0t83;
  if ((r22 != 0)) {
    int _sv0t84 = (220 + r22);
    return _sv0t84;
  } else {
  }
  return 0;
}

