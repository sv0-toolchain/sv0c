#include "sv0_runtime.h"

static int fresh_tmp_name(int counter);
static int next_tmp(int counter);
static const char* lower_digit(int d);
static const char* lower_int_to_str(int n);
static const char* fresh_tmp_str(int counter);
static int lower_alias_new(void);
static int lower_alias_add(int aliases, int from_h, int to_h);
static int lower_alias_lookup(int aliases, int name_h);
static int resolve_fn_callee(int aliases, int name_h);
static int resolve_enum_ctor_path(int ctor_from_enum, int ctor_from_var, int ctor_to_enum, int ctor_to_var, int enum_h, int variant_h);
static int resolved_ctor_enum(int ctor_to_enum, int idx, int default_enum);
static int resolved_ctor_variant(int ctor_to_var, int idx, int default_var);
static int find_double_colon(const char* s, int start);
static int split_qname_count(const char* s);
static int has_qualified_name(const char* s);
static const char* ast_binop_to_c(int tag);
static const char* ast_unop_to_c(int tag);
static int is_supported_unop(int tag);
static const char* ast_ty_to_c_string(const char* name);
static const char* ast_ty_to_c_string_with_user(const char* name, int name_handle, int struct_names, int enum_names);
static int is_void_cty(const char* name);
static int ast_ty_is_unit(const char* name);
static const char* old_slot_prefix(void);
static int old_slot_name_len(int name_len);
static const char* old_slot_name(const char* name);
static int param_name_tag(int pat_tag);
static int param_name(int pat_tag, int name_handle);
static int lower_field_index(int field_names, int target);
static int old_name_seen(int seen, int name);
static int old_names_add_unique(int seen, int name);
static int uniq_old_names(int names);
static int ret_syntax_is_unit(int has_ret, const char* ret_ty_name);
static int use_ret_slot(int is_unit, int ens_mentions_result);
static int lower_lit_supported(int lit_tag);
static int store_value_class(int is_var, int var_name_eq_slot);
static int low_classify_assign_lhs(int expr_tag);
static int enum_tag_lookup(int enum_names, int enum_tags_flat, int enum_tag_offsets, int enum_tag_counts, int enum_name, int variant_name);
static int struct_fields_lookup(int struct_names, int struct_field_offsets, int struct_field_counts, int struct_fields_flat, int name);
static int struct_field_count(int struct_names, int struct_field_counts, int name);
static int is_enum_cty(int enum_names, int name);
static int is_struct_cty(int struct_names, int name);
static int variant_slot_count(int shape_tag, int field_count);
static int enum_max_payload_lookup(int enum_names, int enum_max_payloads, int name);
static const char* try_variant_success(int has_ok, int has_some);
static const char* try_variant_failure(int has_err, int has_none);
static int is_try_enum(int has_ok, int has_err, int has_some, int has_none);
static int try_variant_names_lookup(int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, const char* source, int starts, int ends, int enum_name, int out_handles);
static int find_variant_in_items(int item_tags, int item_names, int item_field_counts, int variant_names_flat, int enum_name, int variant_name);
static int classify_literal(int lit_tag);
static int lower_lit_to_ir_tag(int lit_tag);
static const char* fn_ret_cty(int has_ret, const char* ret_cty);
static int value_to_expr_kind(int val_tag);
static int payload_field_name(int index);
static int enum_store_payload_count(int val_count);
static int enum_alloc_width(int max_payload);
static const char* ret_value_slot(void);
static int find_fn_index(int fn_names, int name_h);
static int callee_fn_index(int fn_names, int aliases, int name_h);
static int scrut_local_lookup(int scrut_names, int name_h);
static int param_ty_lookup(int param_names, int name_h);
static const char* handle_to_str(int h, const char* source, int starts, int ends);
static const char* emit_struct_td(int name_h, int fnames, int ftypes, int foff, int fcount, const char* source, int starts, int ends, int sn, int en);
static const char* emit_enum_td(int name_h, int max_payload, const char* source, int starts, int ends);
static int build_struct_order(int item_tags, int item_names, int item_field_counts, int struct_fnames_flat, int out_names, int out_offsets, int out_counts, int out_flat);
static int build_enum_variants_data(int item_tags, int item_names, int item_field_counts, int item_vmax, int enum_vnames_flat, int out_names, int out_tag_offsets, int out_tag_counts, int out_tags_flat, int out_max_payloads);
static const char* collect_typedefs_str(int item_tags, int item_names, int item_field_counts, int item_vmax, int sf_names, int sf_types, const char* source, int starts, int ends, int sn, int en);
static int mentions_result(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp);
static int all_old_names_expr(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp, int out);
static int expr_init_cty(int et, int ed1, int ed2, int ed3, int ed4, int idx, int pp, int fn_names, int fn_ret_ctys, int aliases);
static int scan_lets(int et, int ed1, int ed2, int ed3, int ed4, int block_idx, int pp, int fn_names, int fn_ret_ctys, int aliases, int out_names, int out_ctys);
static int test_fresh_tmp(void);
static int test_split_qname(void);
static int test_binop_to_c(void);
static int test_unop_to_c(void);
static int test_ty_to_c(void);
static int test_old_slot(void);
static int test_ret_syntax(void);
static int test_store_value_class(void);
static int test_assign_lhs(void);
static int test_enum_tag(void);
static int test_struct_fields(void);
static int test_type_detection(void);
static int test_variant_slots(void);
static int test_try_variants(void);
static int test_literals(void);
static int test_value_to_expr(void);
static int test_enum_alloc(void);
static int test_ret_value_slot(void);
static int test_use_ret_slot(void);
static int test_lit_supported(void);
static int test_param_name_tag(void);
static int test_uniq_old_names(void);
static int test_fresh_tmp_str(void);
static int test_lower_lit_ir(void);
static int test_fn_ret_cty(void);
static int test_lower_alias(void);
static int test_resolve_fn_callee(void);
static int test_resolve_ctor_path(void);
static int test_param_name(void);
static int test_lower_field_index(void);
static int test_ast_ty_c_string_user(void);
static int test_find_fn_index(void);
static int test_callee_fn_index(void);
static int test_scrut_local(void);
static int test_param_ty(void);
static int test_mentions_result(void);
static int test_all_old_names(void);
static int test_typedef_builders(void);
static int test_expr_init_cty(void);
static int test_try_variant_lookup(void);
static int test_find_variant_in_items(void);
static int test_scan_lets(void);

static int fresh_tmp_name(int counter) {
  return counter;
}

static int next_tmp(int counter) {
  int _sv0t0 = (counter + 1);
  return _sv0t0;
}

static const char* lower_digit(int d) {
  if ((d == 0)) {
    return "0";
  } else {
  }
  if ((d == 1)) {
    return "1";
  } else {
  }
  if ((d == 2)) {
    return "2";
  } else {
  }
  if ((d == 3)) {
    return "3";
  } else {
  }
  if ((d == 4)) {
    return "4";
  } else {
  }
  if ((d == 5)) {
    return "5";
  } else {
  }
  if ((d == 6)) {
    return "6";
  } else {
  }
  if ((d == 7)) {
    return "7";
  } else {
  }
  if ((d == 8)) {
    return "8";
  } else {
  }
  return "9";
}

static const char* lower_int_to_str(int n) {
  if ((n < 0)) {
    int _sv0t0 = (0 - n);
    const char* _sv0t1 = lower_int_to_str(_sv0t0);
    const char* _sv0t2 = sv0_string_concat("-", _sv0t1);
    return _sv0t2;
  } else {
  }
  if ((n < 10)) {
    const char* _sv0t3 = lower_digit(n);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = (n / 10);
  const char* _sv0t5 = lower_int_to_str(_sv0t4);
  const char* rest;
  rest = _sv0t5;
  int _sv0t6 = (n / 10);
  int _sv0t7 = (_sv0t6 * 10);
  int _sv0t8 = (n - _sv0t7);
  const char* _sv0t9 = lower_digit(_sv0t8);
  const char* last;
  last = _sv0t9;
  const char* _sv0t10 = sv0_string_concat(rest, last);
  return _sv0t10;
}

static const char* fresh_tmp_str(int counter) {
  const char* _sv0t0 = lower_int_to_str(counter);
  const char* _sv0t1 = sv0_string_concat("_sv0t", _sv0t0);
  return _sv0t1;
}

static int lower_alias_new(void) {
  int _sv0t0 = sv0_vec_new();
  return _sv0t0;
}

static int lower_alias_add(int aliases, int from_h, int to_h) {
  sv0_vec_push(aliases, from_h);
  sv0_vec_push(aliases, to_h);
  int _sv0t0 = sv0_vec_len(aliases);
  int _sv0t1 = (_sv0t0 / 2);
  return _sv0t1;
}

static int lower_alias_lookup(int aliases, int name_h) {
  int _sv0t0 = sv0_vec_len(aliases);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(aliases, i);
    if ((_sv0t1 == name_h)) {
      int _sv0t2 = (i + 1);
      int _sv0t3 = sv0_vec_get(aliases, _sv0t2);
      return _sv0t3;
    } else {
    }
    i = (i + 2);
  }
  int _sv0t4 = (0 - 1);
  return _sv0t4;
}

static int resolve_fn_callee(int aliases, int name_h) {
  int _sv0t0 = lower_alias_lookup(aliases, name_h);
  int target = _sv0t0;
  if ((target < 0)) {
    return name_h;
  } else {
  }
  return target;
}

static int resolve_enum_ctor_path(int ctor_from_enum, int ctor_from_var, int ctor_to_enum, int ctor_to_var, int enum_h, int variant_h) {
  int _sv0t0 = sv0_vec_len(ctor_from_enum);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(ctor_from_enum, i);
    if ((_sv0t1 == enum_h)) {
      int _sv0t2 = sv0_vec_get(ctor_from_var, i);
      if ((_sv0t2 == variant_h)) {
        return i;
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int resolved_ctor_enum(int ctor_to_enum, int idx, int default_enum) {
  if ((idx < 0)) {
    return default_enum;
  } else {
  }
  int _sv0t0 = sv0_vec_get(ctor_to_enum, idx);
  return _sv0t0;
}

static int resolved_ctor_variant(int ctor_to_var, int idx, int default_var) {
  if ((idx < 0)) {
    return default_var;
  } else {
  }
  int _sv0t0 = sv0_vec_get(ctor_to_var, idx);
  return _sv0t0;
}

static int find_double_colon(const char* s, int start) {
  int _sv0t0 = sv0_string_len(s);
  int len = _sv0t0;
  int limit = (len - 1);
  int i = start;
  while ((i < limit)) {
    int _sv0t1 = sv0_string_char_at(s, i);
    if ((_sv0t1 == 58)) {
      int _sv0t2 = (i + 1);
      int _sv0t3 = sv0_string_char_at(s, _sv0t2);
      if ((_sv0t3 == 58)) {
        return i;
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  int _sv0t4 = (0 - 1);
  return _sv0t4;
}

static int split_qname_count(const char* s) {
  int count = 1;
  int pos = 0;
  int _sv0t0 = find_double_colon(s, pos);
  int idx = _sv0t0;
  if ((idx < 0)) {
    return 1;
  } else {
  }
  count = (count + 1);
  pos = (idx + 2);
  int _sv0t1 = find_double_colon(s, pos);
  int idx2 = _sv0t1;
  if ((idx2 < 0)) {
    return count;
  } else {
  }
  count = (count + 1);
  pos = (idx2 + 2);
  int _sv0t2 = find_double_colon(s, pos);
  int idx3 = _sv0t2;
  if ((idx3 < 0)) {
    return count;
  } else {
  }
  int _sv0t3 = (count + 1);
  return _sv0t3;
}

static int has_qualified_name(const char* s) {
  int _sv0t0 = find_double_colon(s, 0);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static const char* ast_binop_to_c(int tag) {
  if ((tag == 0)) {
    return "+";
  } else {
  }
  if ((tag == 1)) {
    return "-";
  } else {
  }
  if ((tag == 2)) {
    return "*";
  } else {
  }
  if ((tag == 3)) {
    return "/";
  } else {
  }
  if ((tag == 4)) {
    return "%";
  } else {
  }
  if ((tag == 5)) {
    return "==";
  } else {
  }
  if ((tag == 6)) {
    return "!=";
  } else {
  }
  if ((tag == 7)) {
    return "<";
  } else {
  }
  if ((tag == 8)) {
    return ">";
  } else {
  }
  if ((tag == 9)) {
    return "<=";
  } else {
  }
  if ((tag == 10)) {
    return ">=";
  } else {
  }
  if ((tag == 11)) {
    return "&&";
  } else {
  }
  if ((tag == 12)) {
    return "||";
  } else {
  }
  if ((tag == 13)) {
    return "&";
  } else {
  }
  if ((tag == 14)) {
    return "|";
  } else {
  }
  if ((tag == 15)) {
    return "^";
  } else {
  }
  if ((tag == 16)) {
    return "<<";
  } else {
  }
  if ((tag == 17)) {
    return ">>";
  } else {
  }
  return "+";
}

static const char* ast_unop_to_c(int tag) {
  if ((tag == 0)) {
    return "-";
  } else {
  }
  if ((tag == 1)) {
    return "!";
  } else {
  }
  if ((tag == 2)) {
    return "~";
  } else {
  }
  if ((tag == 3)) {
    return "*";
  } else {
  }
  if ((tag == 4)) {
    return "&";
  } else {
  }
  if ((tag == 5)) {
    return "&";
  } else {
  }
  return "?";
}

static int is_supported_unop(int tag) {
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
  if ((tag == 4)) {
    return 1;
  } else {
  }
  if ((tag == 5)) {
    return 1;
  } else {
  }
  return 0;
}

static const char* ast_ty_to_c_string(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "unit");
  if (_sv0t0) {
    return "void";
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "i32");
  if (_sv0t1) {
    return "int";
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "bool");
  if (_sv0t2) {
    return "int";
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "i8");
  if (_sv0t3) {
    return "int8_t";
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "u8");
  if (_sv0t4) {
    return "uint8_t";
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "i16");
  if (_sv0t5) {
    return "int16_t";
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "u16");
  if (_sv0t6) {
    return "uint16_t";
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "i64");
  if (_sv0t7) {
    return "int64_t";
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "u64");
  if (_sv0t8) {
    return "uint64_t";
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "isize");
  if (_sv0t9) {
    return "intptr_t";
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "usize");
  if (_sv0t10) {
    return "uintptr_t";
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "string");
  if (_sv0t11) {
    return "const char*";
  } else {
  }
  int _sv0t12 = sv0_string_eq(name, "str");
  if (_sv0t12) {
    return "const char*";
  } else {
  }
  int _sv0t13 = sv0_string_eq(name, "String");
  if (_sv0t13) {
    return "const char*";
  } else {
  }
  int _sv0t14 = sv0_string_eq(name, "Vec");
  if (_sv0t14) {
    return "int";
  } else {
  }
  int _sv0t15 = sv0_string_eq(name, "Box");
  if (_sv0t15) {
    return "int";
  } else {
  }
  return "int";
}

static const char* ast_ty_to_c_string_with_user(const char* name, int name_handle, int struct_names, int enum_names) {
  const char* _sv0t0 = ast_ty_to_c_string(name);
  const char* prim;
  prim = _sv0t0;
  int _sv0t1 = sv0_string_eq(prim, "int");
  if ((_sv0t1 != 1)) {
    return prim;
  } else {
  }
  int _sv0t2 = is_struct_cty(struct_names, name_handle);
  if (_sv0t2) {
    return name;
  } else {
  }
  int _sv0t3 = is_enum_cty(enum_names, name_handle);
  if (_sv0t3) {
    return name;
  } else {
  }
  return "int";
}

static int is_void_cty(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "void");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "unit");
  if (_sv0t1) {
    return 1;
  } else {
  }
  return 0;
}

static int ast_ty_is_unit(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "unit");
  return _sv0t0;
}

static const char* old_slot_prefix(void) {
  return "_sv0old_";
}

static int old_slot_name_len(int name_len) {
  int _sv0t0 = (8 + name_len);
  return _sv0t0;
}

static const char* old_slot_name(const char* name) {
  const char* _sv0t0 = sv0_string_concat("_sv0old_", name);
  return _sv0t0;
}

static int param_name_tag(int pat_tag) {
  int _sv0t0 = (pat_tag == 1);
  return _sv0t0;
}

static int param_name(int pat_tag, int name_handle) {
  if ((pat_tag == 1)) {
    return name_handle;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int lower_field_index(int field_names, int target) {
  int _sv0t0 = sv0_vec_len(field_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(field_names, i);
    if ((_sv0t1 == target)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int old_name_seen(int seen, int name) {
  int _sv0t0 = sv0_vec_len(seen);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(seen, i);
    if ((_sv0t1 == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int old_names_add_unique(int seen, int name) {
  int _sv0t0 = old_name_seen(seen, name);
  if (_sv0t0) {
    return 0;
  } else {
  }
  sv0_vec_push(seen, name);
  return 1;
}

static int uniq_old_names(int names) {
  int _sv0t0 = sv0_vec_new();
  int out = _sv0t0;
  int _sv0t1 = sv0_vec_len(names);
  int n = _sv0t1;
  int i = 0;
  while ((i < n)) {
    int _sv0t2 = sv0_vec_get(names, i);
    int name = _sv0t2;
    int _sv0t3 = old_names_add_unique(out, name);
    i = (i + 1);
  }
  return out;
}

static int ret_syntax_is_unit(int has_ret, const char* ret_ty_name) {
  if ((has_ret == 0)) {
    return 1;
  } else {
  }
  int _sv0t0 = sv0_string_eq(ret_ty_name, "unit");
  if (_sv0t0) {
    return 1;
  } else {
  }
  return 0;
}

static int use_ret_slot(int is_unit, int ens_mentions_result) {
  if (is_unit) {
    return 0;
  } else {
  }
  return ens_mentions_result;
}

static int lower_lit_supported(int lit_tag) {
  int _sv0t0 = classify_literal(lit_tag);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int store_value_class(int is_var, int var_name_eq_slot) {
  if (is_var) {
    if (var_name_eq_slot) {
      return 0;
    } else {
    }
    return 2;
  } else {
  }
  return 1;
}

static int low_classify_assign_lhs(int expr_tag) {
  if ((expr_tag == 2)) {
    return 0;
  } else {
  }
  if ((expr_tag == 11)) {
    return 1;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int enum_tag_lookup(int enum_names, int enum_tags_flat, int enum_tag_offsets, int enum_tag_counts, int enum_name, int variant_name) {
  int _sv0t0 = sv0_vec_len(enum_names);
  int n = _sv0t0;
  int ei = 0;
  while ((ei < n)) {
    int _sv0t1 = sv0_vec_get(enum_names, ei);
    if ((_sv0t1 == enum_name)) {
      int _sv0t2 = sv0_vec_get(enum_tag_offsets, ei);
      int offset = _sv0t2;
      int _sv0t3 = sv0_vec_get(enum_tag_counts, ei);
      int count = _sv0t3;
      int vi = 0;
      while ((vi < count)) {
        int _sv0t4 = (vi * 2);
        int flat_idx = (offset + _sv0t4);
        int _sv0t5 = sv0_vec_get(enum_tags_flat, flat_idx);
        if ((_sv0t5 == variant_name)) {
          int _sv0t6 = (flat_idx + 1);
          int _sv0t7 = sv0_vec_get(enum_tags_flat, _sv0t6);
          return _sv0t7;
        } else {
        }
        vi = (vi + 1);
      }
      int _sv0t8 = (0 - 1);
      return _sv0t8;
    } else {
    }
    ei = (ei + 1);
  }
  int _sv0t9 = (0 - 2);
  return _sv0t9;
}

static int struct_fields_lookup(int struct_names, int struct_field_offsets, int struct_field_counts, int struct_fields_flat, int name) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(struct_names, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = sv0_vec_get(struct_field_offsets, i);
      return _sv0t2;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int struct_field_count(int struct_names, int struct_field_counts, int name) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(struct_names, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = sv0_vec_get(struct_field_counts, i);
      return _sv0t2;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int is_enum_cty(int enum_names, int name) {
  int _sv0t0 = sv0_vec_len(enum_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(enum_names, i);
    if ((_sv0t1 == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int is_struct_cty(int struct_names, int name) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(struct_names, i);
    if ((_sv0t1 == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int variant_slot_count(int shape_tag, int field_count) {
  if ((shape_tag == 0)) {
    return 0;
  } else {
  }
  return field_count;
}

static int enum_max_payload_lookup(int enum_names, int enum_max_payloads, int name) {
  int _sv0t0 = sv0_vec_len(enum_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(enum_names, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = sv0_vec_get(enum_max_payloads, i);
      return _sv0t2;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static const char* try_variant_success(int has_ok, int has_some) {
  if (has_ok) {
    return "Ok";
  } else {
  }
  if (has_some) {
    return "Some";
  } else {
  }
  return "";
}

static const char* try_variant_failure(int has_err, int has_none) {
  if (has_err) {
    return "Err";
  } else {
  }
  if (has_none) {
    return "None";
  } else {
  }
  return "";
}

static int is_try_enum(int has_ok, int has_err, int has_some, int has_none) {
  if (has_ok) {
    if (has_err) {
      return 1;
    } else {
    }
  } else {
  }
  if (has_some) {
    if (has_none) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int try_variant_names_lookup(int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, const char* source, int starts, int ends, int enum_name, int out_handles) {
  int _sv0t0 = sv0_vec_len(enum_names);
  int n = _sv0t0;
  int ei = 0;
  while ((ei < n)) {
    int _sv0t1 = sv0_vec_get(enum_names, ei);
    if ((_sv0t1 == enum_name)) {
      int _sv0t2 = sv0_vec_get(enum_tag_offsets, ei);
      int offset = _sv0t2;
      int _sv0t3 = sv0_vec_get(enum_tag_counts, ei);
      int count = _sv0t3;
      int has_ok = 0;
      int has_err = 0;
      int has_some = 0;
      int has_none = 0;
      int ok_h = 0;
      int err_h = 0;
      int some_h = 0;
      int none_h = 0;
      int vi = 0;
      while ((vi < count)) {
        int _sv0t4 = (vi * 2);
        int _sv0t5 = (offset + _sv0t4);
        int _sv0t6 = sv0_vec_get(enum_tags_flat, _sv0t5);
        int vn_h = _sv0t6;
        const char* _sv0t7 = handle_to_str(vn_h, source, starts, ends);
        const char* vn_s;
        vn_s = _sv0t7;
        int _sv0t8 = sv0_string_eq(vn_s, "Ok");
        if (_sv0t8) {
          has_ok = 1;
          ok_h = vn_h;
        } else {
        }
        int _sv0t9 = sv0_string_eq(vn_s, "Err");
        if (_sv0t9) {
          has_err = 1;
          err_h = vn_h;
        } else {
        }
        int _sv0t10 = sv0_string_eq(vn_s, "Some");
        if (_sv0t10) {
          has_some = 1;
          some_h = vn_h;
        } else {
        }
        int _sv0t11 = sv0_string_eq(vn_s, "None");
        if (_sv0t11) {
          has_none = 1;
          none_h = vn_h;
        } else {
        }
        vi = (vi + 1);
      }
      if (has_ok) {
        if (has_err) {
          sv0_vec_push(out_handles, ok_h);
          sv0_vec_push(out_handles, err_h);
          return 1;
        } else {
        }
      } else {
      }
      if (has_some) {
        if (has_none) {
          sv0_vec_push(out_handles, some_h);
          sv0_vec_push(out_handles, none_h);
          return 1;
        } else {
        }
      } else {
      }
      return 0;
    } else {
    }
    ei = (ei + 1);
  }
  return 0;
}

static int find_variant_in_items(int item_tags, int item_names, int item_field_counts, int variant_names_flat, int enum_name, int variant_name) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  int vn_offset = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(item_field_counts, i);
    int vc = _sv0t2;
    if ((tag == 2)) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      if ((_sv0t3 == enum_name)) {
        int vi = 0;
        while ((vi < vc)) {
          int _sv0t4 = (vn_offset + vi);
          int _sv0t5 = sv0_vec_get(variant_names_flat, _sv0t4);
          if ((_sv0t5 == variant_name)) {
            return vi;
          } else {
          }
          vi = (vi + 1);
        }
        int _sv0t6 = (0 - 2);
        return _sv0t6;
      } else {
      }
      vn_offset = (vn_offset + vc);
    } else {
    }
    i = (i + 1);
  }
  int _sv0t7 = (0 - 1);
  return _sv0t7;
}

static int classify_literal(int lit_tag) {
  if ((lit_tag == 0)) {
    return 0;
  } else {
  }
  if ((lit_tag == 1)) {
    return 1;
  } else {
  }
  if ((lit_tag == 2)) {
    return 2;
  } else {
  }
  if ((lit_tag == 3)) {
    return 3;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int lower_lit_to_ir_tag(int lit_tag) {
  if ((lit_tag == 0)) {
    return 0;
  } else {
  }
  if ((lit_tag == 1)) {
    return 1;
  } else {
  }
  if ((lit_tag == 2)) {
    return 4;
  } else {
  }
  if ((lit_tag == 3)) {
    return 5;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static const char* fn_ret_cty(int has_ret, const char* ret_cty) {
  if (has_ret) {
    return ret_cty;
  } else {
  }
  return "int";
}

static int value_to_expr_kind(int val_tag) {
  if ((val_tag == 0)) {
    return 0;
  } else {
  }
  return 1;
}

static int payload_field_name(int index) {
  return index;
}

static int enum_store_payload_count(int val_count) {
  return val_count;
}

static int enum_alloc_width(int max_payload) {
  int _sv0t0 = (1 + max_payload);
  return _sv0t0;
}

static const char* ret_value_slot(void) {
  return "_sv0ret";
}

static int find_fn_index(int fn_names, int name_h) {
  int _sv0t0 = sv0_vec_len(fn_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(fn_names, i);
    if ((_sv0t1 == name_h)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int callee_fn_index(int fn_names, int aliases, int name_h) {
  int _sv0t0 = resolve_fn_callee(aliases, name_h);
  int resolved = _sv0t0;
  int _sv0t1 = find_fn_index(fn_names, resolved);
  return _sv0t1;
}

static int scrut_local_lookup(int scrut_names, int name_h) {
  int _sv0t0 = sv0_vec_len(scrut_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(scrut_names, i);
    if ((_sv0t1 == name_h)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int param_ty_lookup(int param_names, int name_h) {
  int _sv0t0 = sv0_vec_len(param_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(param_names, i);
    if ((_sv0t1 == name_h)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static const char* handle_to_str(int h, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_get(starts, h);
  int s = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, h);
  int e = _sv0t1;
  int _sv0t2 = (e - s);
  const char* _sv0t3 = sv0_string_substr(source, s, _sv0t2);
  return _sv0t3;
}

static const char* emit_struct_td(int name_h, int fnames, int ftypes, int foff, int fcount, const char* source, int starts, int ends, int sn, int en) {
  const char* _sv0t0 = handle_to_str(name_h, source, starts, ends);
  const char* name;
  name = _sv0t0;
  const char* r;
  r = "typedef struct {\n";
  int i = 0;
  while ((i < fcount)) {
    int _sv0t1 = (foff + i);
    int _sv0t2 = sv0_vec_get(ftypes, _sv0t1);
    int th = _sv0t2;
    const char* _sv0t3 = handle_to_str(th, source, starts, ends);
    const char* tname;
    tname = _sv0t3;
    const char* _sv0t4 = ast_ty_to_c_string_with_user(tname, th, sn, en);
    const char* cty;
    cty = _sv0t4;
    int _sv0t5 = (foff + i);
    int _sv0t6 = sv0_vec_get(fnames, _sv0t5);
    const char* _sv0t7 = handle_to_str(_sv0t6, source, starts, ends);
    const char* fname;
    fname = _sv0t7;
    const char* _sv0t8 = sv0_string_concat(r, "  ");
    r = _sv0t8;
    const char* _sv0t9 = sv0_string_concat(r, cty);
    r = _sv0t9;
    const char* _sv0t10 = sv0_string_concat(r, " ");
    r = _sv0t10;
    const char* _sv0t11 = sv0_string_concat(r, fname);
    r = _sv0t11;
    const char* _sv0t12 = sv0_string_concat(r, ";\n");
    r = _sv0t12;
    i = (i + 1);
  }
  const char* _sv0t13 = sv0_string_concat(r, "} ");
  r = _sv0t13;
  const char* _sv0t14 = sv0_string_concat(r, name);
  r = _sv0t14;
  const char* _sv0t15 = sv0_string_concat(r, ";\n");
  r = _sv0t15;
  return r;
}

static const char* emit_enum_td(int name_h, int max_payload, const char* source, int starts, int ends) {
  const char* _sv0t0 = handle_to_str(name_h, source, starts, ends);
  const char* name;
  name = _sv0t0;
  const char* r;
  r = "typedef struct {\n  int tag;\n";
  int i = 0;
  while ((i < max_payload)) {
    const char* _sv0t1 = sv0_string_concat(r, "  int p");
    r = _sv0t1;
    const char* _sv0t2 = lower_int_to_str(i);
    const char* _sv0t3 = sv0_string_concat(r, _sv0t2);
    r = _sv0t3;
    const char* _sv0t4 = sv0_string_concat(r, ";\n");
    r = _sv0t4;
    i = (i + 1);
  }
  const char* _sv0t5 = sv0_string_concat(r, "} ");
  r = _sv0t5;
  const char* _sv0t6 = sv0_string_concat(r, name);
  r = _sv0t6;
  const char* _sv0t7 = sv0_string_concat(r, ";\n");
  r = _sv0t7;
  return r;
}

static int build_struct_order(int item_tags, int item_names, int item_field_counts, int struct_fnames_flat, int out_names, int out_offsets, int out_counts, int out_flat) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  int sf = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(item_field_counts, i);
    int fc = _sv0t2;
    if ((tag == 1)) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      sv0_vec_push(out_names, _sv0t3);
      int _sv0t4 = sv0_vec_len(out_flat);
      sv0_vec_push(out_offsets, _sv0t4);
      sv0_vec_push(out_counts, fc);
      int j = 0;
      while ((j < fc)) {
        int _sv0t5 = (sf + j);
        int _sv0t6 = sv0_vec_get(struct_fnames_flat, _sv0t5);
        sv0_vec_push(out_flat, _sv0t6);
        j = (j + 1);
      }
      sf = (sf + fc);
    } else {
    }
    i = (i + 1);
  }
  int _sv0t7 = sv0_vec_len(out_names);
  return _sv0t7;
}

static int build_enum_variants_data(int item_tags, int item_names, int item_field_counts, int item_vmax, int enum_vnames_flat, int out_names, int out_tag_offsets, int out_tag_counts, int out_tags_flat, int out_max_payloads) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  int ev = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(item_field_counts, i);
    int vc = _sv0t2;
    if ((tag == 2)) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      sv0_vec_push(out_names, _sv0t3);
      int _sv0t4 = sv0_vec_len(out_tags_flat);
      sv0_vec_push(out_tag_offsets, _sv0t4);
      sv0_vec_push(out_tag_counts, vc);
      int _sv0t5 = sv0_vec_get(item_vmax, i);
      sv0_vec_push(out_max_payloads, _sv0t5);
      int j = 0;
      while ((j < vc)) {
        int _sv0t6 = (ev + j);
        int _sv0t7 = sv0_vec_get(enum_vnames_flat, _sv0t6);
        sv0_vec_push(out_tags_flat, _sv0t7);
        sv0_vec_push(out_tags_flat, j);
        j = (j + 1);
      }
      ev = (ev + vc);
    } else {
    }
    i = (i + 1);
  }
  int _sv0t8 = sv0_vec_len(out_names);
  return _sv0t8;
}

static const char* collect_typedefs_str(int item_tags, int item_names, int item_field_counts, int item_vmax, int sf_names, int sf_types, const char* source, int starts, int ends, int sn, int en) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  const char* r;
  r = "";
  int i = 0;
  int sf = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(item_field_counts, i);
    int fc = _sv0t2;
    if ((tag == 1)) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      const char* _sv0t4 = emit_struct_td(_sv0t3, sf_names, sf_types, sf, fc, source, starts, ends, sn, en);
      const char* td;
      td = _sv0t4;
      const char* _sv0t5 = sv0_string_concat(r, td);
      r = _sv0t5;
      sf = (sf + fc);
    } else {
    }
    i = (i + 1);
  }
  i = 0;
  while ((i < n)) {
    int _sv0t6 = sv0_vec_get(item_tags, i);
    if ((_sv0t6 == 2)) {
      int _sv0t7 = sv0_vec_get(item_names, i);
      int _sv0t8 = sv0_vec_get(item_vmax, i);
      const char* _sv0t9 = emit_enum_td(_sv0t7, _sv0t8, source, starts, ends);
      const char* td2;
      td2 = _sv0t9;
      const char* _sv0t10 = sv0_string_concat(r, td2);
      r = _sv0t10;
    } else {
    }
    i = (i + 1);
  }
  return r;
}

static int mentions_result(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 0)) {
    return 0;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t1 = sv0_vec_get(ed2, idx);
    int ppc = _sv0t1;
    if ((ppc == 1)) {
      int _sv0t2 = sv0_vec_get(ed1, idx);
      int _sv0t3 = sv0_vec_get(pp, _sv0t2);
      int tok = _sv0t3;
      const char* _sv0t4 = handle_to_str(tok, source, starts, ends);
      int _sv0t5 = sv0_string_eq(_sv0t4, "result");
      return _sv0t5;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t6 = sv0_vec_get(ed2, idx);
    int _sv0t7 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t6, source, starts, ends, pp);
    return _sv0t7;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t8 = sv0_vec_get(ed2, idx);
    int _sv0t9 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t8, source, starts, ends, pp);
    if (_sv0t9) {
      return 1;
    } else {
    }
    int _sv0t10 = sv0_vec_get(ed3, idx);
    int _sv0t11 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t10, source, starts, ends, pp);
    return _sv0t11;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t12 = sv0_vec_get(ed1, idx);
    int _sv0t13 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t12, source, starts, ends, pp);
    if (_sv0t13) {
      return 1;
    } else {
    }
    int _sv0t14 = sv0_vec_get(ed2, idx);
    int af = _sv0t14;
    int _sv0t15 = sv0_vec_get(ed3, idx);
    int ac = _sv0t15;
    int ai = 0;
    while ((ai < ac)) {
      int _sv0t16 = (af + ai);
      int _sv0t17 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t16, source, starts, ends, pp);
      if (_sv0t17) {
        return 1;
      } else {
      }
      ai = (ai + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t18 = sv0_vec_get(ed1, idx);
    int _sv0t19 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t18, source, starts, ends, pp);
    return _sv0t19;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t20 = sv0_vec_get(ed1, idx);
    int _sv0t21 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t20, source, starts, ends, pp);
    if (_sv0t21) {
      return 1;
    } else {
    }
    int _sv0t22 = sv0_vec_get(ed2, idx);
    int _sv0t23 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t22, source, starts, ends, pp);
    if (_sv0t23) {
      return 1;
    } else {
    }
    int _sv0t24 = sv0_vec_get(ed3, idx);
    int _sv0t25 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t24, source, starts, ends, pp);
    return _sv0t25;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t26 = sv0_vec_get(ed1, idx);
    int _sv0t27 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t26, source, starts, ends, pp);
    if (_sv0t27) {
      return 1;
    } else {
    }
    int _sv0t28 = sv0_vec_get(ed2, idx);
    int amf = _sv0t28;
    int _sv0t29 = sv0_vec_get(ed3, idx);
    int amc = _sv0t29;
    int ami = 0;
    while ((ami < amc)) {
      int aidx = (amf + ami);
      int _sv0t30 = sv0_vec_get(ed3, aidx);
      int grd = _sv0t30;
      if ((grd >= 0)) {
        int _sv0t31 = mentions_result(et, ed1, ed2, ed3, ed4, grd, source, starts, ends, pp);
        if (_sv0t31) {
          return 1;
        } else {
        }
      } else {
      }
      int _sv0t32 = sv0_vec_get(ed4, aidx);
      int _sv0t33 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t32, source, starts, ends, pp);
      if (_sv0t33) {
        return 1;
      } else {
      }
      ami = (ami + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t34 = sv0_vec_get(ed1, idx);
    int _sv0t35 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t34, source, starts, ends, pp);
    if (_sv0t35) {
      return 1;
    } else {
    }
    int _sv0t36 = sv0_vec_get(ed3, idx);
    int invf = _sv0t36;
    int _sv0t37 = sv0_vec_get(ed4, idx);
    int invc = _sv0t37;
    int ii = 0;
    while ((ii < invc)) {
      int _sv0t38 = (invf + ii);
      int _sv0t39 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t38, source, starts, ends, pp);
      if (_sv0t39) {
        return 1;
      } else {
      }
      ii = (ii + 1);
    }
    int _sv0t40 = sv0_vec_get(ed2, idx);
    int _sv0t41 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t40, source, starts, ends, pp);
    return _sv0t41;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t42 = sv0_vec_get(ed2, idx);
    int _sv0t43 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t42, source, starts, ends, pp);
    if (_sv0t43) {
      return 1;
    } else {
    }
    int _sv0t44 = sv0_vec_get(ed3, idx);
    int _sv0t45 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t44, source, starts, ends, pp);
    return _sv0t45;
  } else {
  }
  if ((tag == 14)) {
    int _sv0t46 = sv0_vec_get(ed1, idx);
    int _sv0t47 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t46, source, starts, ends, pp);
    return _sv0t47;
  } else {
  }
  if ((tag == 16)) {
    int _sv0t48 = sv0_vec_get(ed1, idx);
    int _sv0t49 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t48, source, starts, ends, pp);
    return _sv0t49;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t50 = sv0_vec_get(ed1, idx);
    int _sv0t51 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t50, source, starts, ends, pp);
    return _sv0t51;
  } else {
  }
  if ((tag == 22)) {
    int _sv0t52 = sv0_vec_get(ed1, idx);
    int _sv0t53 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t52, source, starts, ends, pp);
    return _sv0t53;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t54 = sv0_vec_get(ed3, idx);
    int sfc = _sv0t54;
    int sfi = 0;
    while ((sfi < sfc)) {
      int _sv0t55 = (idx - sfc);
      int _sv0t56 = (_sv0t55 + sfi);
      int _sv0t57 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t56, source, starts, ends, pp);
      if (_sv0t57) {
        return 1;
      } else {
      }
      sfi = (sfi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t58 = sv0_vec_get(ed1, idx);
    int tf = _sv0t58;
    int _sv0t59 = sv0_vec_get(ed2, idx);
    int tc = _sv0t59;
    int ti = 0;
    while ((ti < tc)) {
      int _sv0t60 = (tf + ti);
      int _sv0t61 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t60, source, starts, ends, pp);
      if (_sv0t61) {
        return 1;
      } else {
      }
      ti = (ti + 1);
    }
    return 0;
  } else {
  }
  return 0;
}

static int all_old_names_expr(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp, int out) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 0)) {
    return 0;
  } else {
  }
  if ((tag == 1)) {
    return 0;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t1 = sv0_vec_get(ed1, idx);
    int callee = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed3, idx);
    int ac = _sv0t2;
    if ((ac == 1)) {
      int _sv0t3 = sv0_vec_get(et, callee);
      int ct = _sv0t3;
      if ((ct == 1)) {
        int _sv0t4 = sv0_vec_get(ed2, callee);
        int cppc = _sv0t4;
        if ((cppc == 1)) {
          int _sv0t5 = sv0_vec_get(ed1, callee);
          int _sv0t6 = sv0_vec_get(pp, _sv0t5);
          int ctok = _sv0t6;
          const char* _sv0t7 = handle_to_str(ctok, source, starts, ends);
          int _sv0t8 = sv0_string_eq(_sv0t7, "old");
          if (_sv0t8) {
            int _sv0t9 = sv0_vec_get(ed2, idx);
            int arg0 = _sv0t9;
            int _sv0t10 = sv0_vec_get(et, arg0);
            int at = _sv0t10;
            if ((at == 1)) {
              int _sv0t11 = sv0_vec_get(ed2, arg0);
              int appc = _sv0t11;
              if ((appc == 1)) {
                int _sv0t12 = sv0_vec_get(ed1, arg0);
                int _sv0t13 = sv0_vec_get(pp, _sv0t12);
                int ntok = _sv0t13;
                sv0_vec_push(out, ntok);
                return 1;
              } else {
              }
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    int _sv0t14 = all_old_names_expr(et, ed1, ed2, ed3, ed4, callee, source, starts, ends, pp, out);
    int _sv0t15 = sv0_vec_get(ed2, idx);
    int af = _sv0t15;
    int ai = 0;
    while ((ai < ac)) {
      int _sv0t16 = (af + ai);
      int _sv0t17 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t16, source, starts, ends, pp, out);
      ai = (ai + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t18 = sv0_vec_get(ed2, idx);
    int _sv0t19 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t18, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t20 = sv0_vec_get(ed2, idx);
    int _sv0t21 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t20, source, starts, ends, pp, out);
    int _sv0t22 = sv0_vec_get(ed3, idx);
    int _sv0t23 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t22, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t24 = sv0_vec_get(ed1, idx);
    int _sv0t25 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t24, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t26 = sv0_vec_get(ed1, idx);
    int _sv0t27 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t26, source, starts, ends, pp, out);
    int _sv0t28 = sv0_vec_get(ed2, idx);
    int _sv0t29 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t28, source, starts, ends, pp, out);
    int _sv0t30 = sv0_vec_get(ed3, idx);
    int _sv0t31 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t30, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t32 = sv0_vec_get(ed1, idx);
    int _sv0t33 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t32, source, starts, ends, pp, out);
    int _sv0t34 = sv0_vec_get(ed2, idx);
    int amf = _sv0t34;
    int _sv0t35 = sv0_vec_get(ed3, idx);
    int amc = _sv0t35;
    int ami = 0;
    while ((ami < amc)) {
      int aidx = (amf + ami);
      int _sv0t36 = sv0_vec_get(ed3, aidx);
      int grd = _sv0t36;
      if ((grd >= 0)) {
        int _sv0t37 = all_old_names_expr(et, ed1, ed2, ed3, ed4, grd, source, starts, ends, pp, out);
      } else {
      }
      int _sv0t38 = sv0_vec_get(ed4, aidx);
      int _sv0t39 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t38, source, starts, ends, pp, out);
      ami = (ami + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t40 = sv0_vec_get(ed1, idx);
    int _sv0t41 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t40, source, starts, ends, pp, out);
    int _sv0t42 = sv0_vec_get(ed3, idx);
    int invf = _sv0t42;
    int _sv0t43 = sv0_vec_get(ed4, idx);
    int invc = _sv0t43;
    int ii = 0;
    while ((ii < invc)) {
      int _sv0t44 = (invf + ii);
      int _sv0t45 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t44, source, starts, ends, pp, out);
      ii = (ii + 1);
    }
    int _sv0t46 = sv0_vec_get(ed2, idx);
    int _sv0t47 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t46, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t48 = sv0_vec_get(ed2, idx);
    int _sv0t49 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t48, source, starts, ends, pp, out);
    int _sv0t50 = sv0_vec_get(ed3, idx);
    int _sv0t51 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t50, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 14)) {
    int _sv0t52 = sv0_vec_get(ed1, idx);
    int _sv0t53 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t52, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t54 = sv0_vec_get(ed1, idx);
    int _sv0t55 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t54, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 21)) {
    int _sv0t56 = sv0_vec_get(ed1, idx);
    int _sv0t57 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t56, source, starts, ends, pp, out);
    int _sv0t58 = sv0_vec_get(ed2, idx);
    int _sv0t59 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t58, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 22)) {
    int _sv0t60 = sv0_vec_get(ed1, idx);
    int _sv0t61 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t60, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t62 = sv0_vec_get(ed3, idx);
    int sfc = _sv0t62;
    int sfi = 0;
    while ((sfi < sfc)) {
      int _sv0t63 = (idx - sfc);
      int _sv0t64 = (_sv0t63 + sfi);
      int _sv0t65 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t64, source, starts, ends, pp, out);
      sfi = (sfi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t66 = sv0_vec_get(ed1, idx);
    int tf = _sv0t66;
    int _sv0t67 = sv0_vec_get(ed2, idx);
    int tc = _sv0t67;
    int ti = 0;
    while ((ti < tc)) {
      int _sv0t68 = (tf + ti);
      int _sv0t69 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t68, source, starts, ends, pp, out);
      ti = (ti + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 9)) {
    int _sv0t70 = sv0_vec_get(ed1, idx);
    int sf = _sv0t70;
    int _sv0t71 = sv0_vec_get(ed2, idx);
    int sc = _sv0t71;
    int si = 0;
    while ((si < sc)) {
      int sidx = (sf + si);
      int _sv0t72 = sv0_vec_get(et, sidx);
      int stag = _sv0t72;
      if ((stag == 27)) {
        int _sv0t73 = sv0_vec_get(ed3, sidx);
        int ini = _sv0t73;
        if ((ini >= 0)) {
          int _sv0t74 = all_old_names_expr(et, ed1, ed2, ed3, ed4, ini, source, starts, ends, pp, out);
        } else {
        }
      } else {
      }
      if ((stag == 28)) {
        int _sv0t75 = sv0_vec_get(ed1, sidx);
        int _sv0t76 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t75, source, starts, ends, pp, out);
      } else {
      }
      si = (si + 1);
    }
    int _sv0t77 = sv0_vec_get(ed3, idx);
    int tail = _sv0t77;
    if ((tail >= 0)) {
      int _sv0t78 = all_old_names_expr(et, ed1, ed2, ed3, ed4, tail, source, starts, ends, pp, out);
    } else {
    }
    return 0;
  } else {
  }
  return 0;
}

static int expr_init_cty(int et, int ed1, int ed2, int ed3, int ed4, int idx, int pp, int fn_names, int fn_ret_ctys, int aliases) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(et, idx);
  int tag = _sv0t1;
  if ((tag == 24)) {
    int _sv0t2 = sv0_vec_get(ed1, idx);
    int pps = _sv0t2;
    int _sv0t3 = sv0_vec_get(pp, pps);
    return _sv0t3;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t4 = sv0_vec_get(ed1, idx);
    int pps2 = _sv0t4;
    int _sv0t5 = sv0_vec_get(ed2, idx);
    int ppc2 = _sv0t5;
    if ((ppc2 == 2)) {
      int _sv0t6 = sv0_vec_get(pp, pps2);
      return _sv0t6;
    } else {
    }
    int _sv0t7 = (0 - 1);
    return _sv0t7;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t8 = sv0_vec_get(ed1, idx);
    int callee = _sv0t8;
    if ((callee >= 0)) {
      int _sv0t9 = sv0_vec_get(et, callee);
      int ct = _sv0t9;
      if ((ct == 1)) {
        int _sv0t10 = sv0_vec_get(ed1, callee);
        int cpps = _sv0t10;
        int _sv0t11 = sv0_vec_get(ed2, callee);
        int cppc = _sv0t11;
        if ((cppc == 1)) {
          int _sv0t12 = sv0_vec_get(pp, cpps);
          int ftok = _sv0t12;
          int _sv0t13 = callee_fn_index(fn_names, aliases, ftok);
          int fi = _sv0t13;
          if ((fi >= 0)) {
            int _sv0t14 = sv0_vec_get(fn_ret_ctys, fi);
            return _sv0t14;
          } else {
          }
          int _sv0t15 = (0 - 1);
          return _sv0t15;
        } else {
        }
        if ((cppc == 2)) {
          int _sv0t16 = sv0_vec_get(pp, cpps);
          return _sv0t16;
        } else {
        }
      } else {
      }
    } else {
    }
    int _sv0t17 = (0 - 1);
    return _sv0t17;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t18 = sv0_vec_get(ed2, idx);
    int tc = _sv0t18;
    if ((tc == 1)) {
      int _sv0t19 = sv0_vec_get(ed1, idx);
      int first = _sv0t19;
      int _sv0t20 = expr_init_cty(et, ed1, ed2, ed3, ed4, first, pp, fn_names, fn_ret_ctys, aliases);
      return _sv0t20;
    } else {
    }
  } else {
  }
  int _sv0t21 = (0 - 1);
  return _sv0t21;
}

static int scan_lets(int et, int ed1, int ed2, int ed3, int ed4, int block_idx, int pp, int fn_names, int fn_ret_ctys, int aliases, int out_names, int out_ctys) {
  if ((block_idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(et, block_idx);
  int btag = _sv0t0;
  if ((btag != 9)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(ed1, block_idx);
  int sf = _sv0t1;
  int _sv0t2 = sv0_vec_get(ed2, block_idx);
  int sc = _sv0t2;
  int count = 0;
  int i = 0;
  while ((i < sc)) {
    int sidx = (sf + i);
    int _sv0t3 = sv0_vec_get(et, sidx);
    int stag = _sv0t3;
    if ((stag == 27)) {
      int _sv0t4 = sv0_vec_get(ed3, sidx);
      int init = _sv0t4;
      if ((init >= 0)) {
        int _sv0t5 = sv0_vec_get(ed1, sidx);
        int nh = _sv0t5;
        int _sv0t6 = expr_init_cty(et, ed1, ed2, ed3, ed4, init, pp, fn_names, fn_ret_ctys, aliases);
        int th = _sv0t6;
        sv0_vec_push(out_names, nh);
        sv0_vec_push(out_ctys, th);
        count = (count + 1);
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int test_fresh_tmp(void) {
  int _sv0t0 = fresh_tmp_name(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = next_tmp(0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = next_tmp(5);
  if ((_sv0t2 != 6)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_split_qname(void) {
  int _sv0t0 = find_double_colon("Foo::bar", 0);
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = find_double_colon("hello", 0);
  int _sv0t2 = (0 - 1);
  if ((_sv0t1 != _sv0t2)) {
    return 2;
  } else {
  }
  int _sv0t3 = find_double_colon("a::b::c", 0);
  if ((_sv0t3 != 1)) {
    return 3;
  } else {
  }
  int _sv0t4 = split_qname_count("hello");
  if ((_sv0t4 != 1)) {
    return 4;
  } else {
  }
  int _sv0t5 = split_qname_count("Foo::bar");
  if ((_sv0t5 != 2)) {
    return 5;
  } else {
  }
  int _sv0t6 = split_qname_count("a::b::c");
  if ((_sv0t6 != 3)) {
    return 6;
  } else {
  }
  int _sv0t7 = has_qualified_name("hello");
  if ((_sv0t7 != 0)) {
    return 7;
  } else {
  }
  int _sv0t8 = has_qualified_name("Foo::bar");
  if ((_sv0t8 != 1)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_binop_to_c(void) {
  const char* _sv0t0 = ast_binop_to_c(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "+");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = ast_binop_to_c(1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "-");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = ast_binop_to_c(2);
  int _sv0t5 = sv0_string_eq(_sv0t4, "*");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = ast_binop_to_c(3);
  int _sv0t7 = sv0_string_eq(_sv0t6, "/");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = ast_binop_to_c(4);
  int _sv0t9 = sv0_string_eq(_sv0t8, "%");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t10 = ast_binop_to_c(5);
  int _sv0t11 = sv0_string_eq(_sv0t10, "==");
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  const char* _sv0t12 = ast_binop_to_c(11);
  int _sv0t13 = sv0_string_eq(_sv0t12, "&&");
  if ((_sv0t13 != 1)) {
    return 7;
  } else {
  }
  const char* _sv0t14 = ast_binop_to_c(12);
  int _sv0t15 = sv0_string_eq(_sv0t14, "||");
  if ((_sv0t15 != 1)) {
    return 8;
  } else {
  }
  const char* _sv0t16 = ast_binop_to_c(13);
  int _sv0t17 = sv0_string_eq(_sv0t16, "&");
  if ((_sv0t17 != 1)) {
    return 9;
  } else {
  }
  const char* _sv0t18 = ast_binop_to_c(16);
  int _sv0t19 = sv0_string_eq(_sv0t18, "<<");
  if ((_sv0t19 != 1)) {
    return 10;
  } else {
  }
  const char* _sv0t20 = ast_binop_to_c(17);
  int _sv0t21 = sv0_string_eq(_sv0t20, ">>");
  if ((_sv0t21 != 1)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_unop_to_c(void) {
  const char* _sv0t0 = ast_unop_to_c(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "-");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = ast_unop_to_c(1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "!");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = ast_unop_to_c(2);
  int _sv0t5 = sv0_string_eq(_sv0t4, "~");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = ast_unop_to_c(3);
  int _sv0t7 = sv0_string_eq(_sv0t6, "*");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = ast_unop_to_c(4);
  int _sv0t9 = sv0_string_eq(_sv0t8, "&");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t10 = ast_unop_to_c(5);
  int _sv0t11 = sv0_string_eq(_sv0t10, "&");
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  int _sv0t12 = is_supported_unop(0);
  if ((_sv0t12 != 1)) {
    return 7;
  } else {
  }
  int _sv0t13 = is_supported_unop(1);
  if ((_sv0t13 != 1)) {
    return 8;
  } else {
  }
  int _sv0t14 = is_supported_unop(2);
  if ((_sv0t14 != 1)) {
    return 9;
  } else {
  }
  int _sv0t15 = is_supported_unop(4);
  if ((_sv0t15 != 1)) {
    return 10;
  } else {
  }
  int _sv0t16 = is_supported_unop(5);
  if ((_sv0t16 != 1)) {
    return 11;
  } else {
  }
  int _sv0t17 = is_supported_unop(3);
  if ((_sv0t17 != 0)) {
    return 12;
  } else {
  }
  return 0;
}

static int test_ty_to_c(void) {
  const char* _sv0t0 = ast_ty_to_c_string("i32");
  int _sv0t1 = sv0_string_eq(_sv0t0, "int");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = ast_ty_to_c_string("bool");
  int _sv0t3 = sv0_string_eq(_sv0t2, "int");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = ast_ty_to_c_string("unit");
  int _sv0t5 = sv0_string_eq(_sv0t4, "void");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = ast_ty_to_c_string("i64");
  int _sv0t7 = sv0_string_eq(_sv0t6, "int64_t");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = ast_ty_to_c_string("string");
  int _sv0t9 = sv0_string_eq(_sv0t8, "const char*");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t10 = ast_ty_to_c_string("String");
  int _sv0t11 = sv0_string_eq(_sv0t10, "const char*");
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  const char* _sv0t12 = ast_ty_to_c_string("Vec");
  int _sv0t13 = sv0_string_eq(_sv0t12, "int");
  if ((_sv0t13 != 1)) {
    return 7;
  } else {
  }
  const char* _sv0t14 = ast_ty_to_c_string("Box");
  int _sv0t15 = sv0_string_eq(_sv0t14, "int");
  if ((_sv0t15 != 1)) {
    return 8;
  } else {
  }
  const char* _sv0t16 = ast_ty_to_c_string("u8");
  int _sv0t17 = sv0_string_eq(_sv0t16, "uint8_t");
  if ((_sv0t17 != 1)) {
    return 9;
  } else {
  }
  const char* _sv0t18 = ast_ty_to_c_string("isize");
  int _sv0t19 = sv0_string_eq(_sv0t18, "intptr_t");
  if ((_sv0t19 != 1)) {
    return 10;
  } else {
  }
  int _sv0t20 = is_void_cty("void");
  if ((_sv0t20 != 1)) {
    return 11;
  } else {
  }
  int _sv0t21 = is_void_cty("unit");
  if ((_sv0t21 != 1)) {
    return 12;
  } else {
  }
  int _sv0t22 = is_void_cty("int");
  if ((_sv0t22 != 0)) {
    return 13;
  } else {
  }
  return 0;
}

static int test_old_slot(void) {
  const char* _sv0t0 = old_slot_prefix();
  int _sv0t1 = sv0_string_eq(_sv0t0, "_sv0old_");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = old_slot_name_len(3);
  if ((_sv0t2 != 11)) {
    return 2;
  } else {
  }
  const char* _sv0t3 = old_slot_name("x");
  int _sv0t4 = sv0_string_eq(_sv0t3, "_sv0old_x");
  if ((_sv0t4 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t5 = old_slot_name("count");
  int _sv0t6 = sv0_string_eq(_sv0t5, "_sv0old_count");
  if ((_sv0t6 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_ret_syntax(void) {
  int _sv0t0 = ret_syntax_is_unit(0, "");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = ret_syntax_is_unit(1, "unit");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = ret_syntax_is_unit(1, "i32");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_store_value_class(void) {
  int _sv0t0 = store_value_class(1, 1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = store_value_class(1, 0);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = store_value_class(0, 0);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_assign_lhs(void) {
  int _sv0t0 = low_classify_assign_lhs(2);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = low_classify_assign_lhs(11);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = low_classify_assign_lhs(99);
  int _sv0t3 = (0 - 1);
  if ((_sv0t2 != _sv0t3)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_enum_tag(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int tags_flat = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int offsets = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int counts = _sv0t3;
  sv0_vec_push(names, 100);
  sv0_vec_push(offsets, 0);
  sv0_vec_push(counts, 3);
  sv0_vec_push(tags_flat, 10);
  sv0_vec_push(tags_flat, 0);
  sv0_vec_push(tags_flat, 20);
  sv0_vec_push(tags_flat, 1);
  sv0_vec_push(tags_flat, 30);
  sv0_vec_push(tags_flat, 2);
  int _sv0t4 = enum_tag_lookup(names, tags_flat, offsets, counts, 100, 10);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = enum_tag_lookup(names, tags_flat, offsets, counts, 100, 20);
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  int _sv0t6 = enum_tag_lookup(names, tags_flat, offsets, counts, 100, 30);
  if ((_sv0t6 != 2)) {
    return 3;
  } else {
  }
  int _sv0t7 = enum_tag_lookup(names, tags_flat, offsets, counts, 100, 99);
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 4;
  } else {
  }
  int _sv0t9 = enum_tag_lookup(names, tags_flat, offsets, counts, 999, 10);
  int _sv0t10 = (0 - 2);
  if ((_sv0t9 != _sv0t10)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_struct_fields(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int offsets = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int counts = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int flat = _sv0t3;
  sv0_vec_push(names, 50);
  sv0_vec_push(offsets, 0);
  sv0_vec_push(counts, 2);
  sv0_vec_push(flat, 10);
  sv0_vec_push(flat, 20);
  int _sv0t4 = struct_fields_lookup(names, offsets, counts, flat, 50);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = struct_field_count(names, counts, 50);
  if ((_sv0t5 != 2)) {
    return 2;
  } else {
  }
  int _sv0t6 = struct_fields_lookup(names, offsets, counts, flat, 99);
  int _sv0t7 = (0 - 1);
  if ((_sv0t6 != _sv0t7)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_type_detection(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int sn = _sv0t1;
  sv0_vec_push(en, 10);
  sv0_vec_push(sn, 20);
  int _sv0t2 = is_enum_cty(en, 10);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = is_enum_cty(en, 99);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  int _sv0t4 = is_struct_cty(sn, 20);
  if ((_sv0t4 != 1)) {
    return 3;
  } else {
  }
  int _sv0t5 = is_struct_cty(sn, 99);
  if ((_sv0t5 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_variant_slots(void) {
  int _sv0t0 = variant_slot_count(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_slot_count(1, 3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_slot_count(2, 2);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_try_variants(void) {
  const char* _sv0t0 = try_variant_success(1, 0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "Ok");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = try_variant_success(0, 1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "Some");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = try_variant_failure(1, 0);
  int _sv0t5 = sv0_string_eq(_sv0t4, "Err");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = try_variant_failure(0, 1);
  int _sv0t7 = sv0_string_eq(_sv0t6, "None");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  int _sv0t8 = is_try_enum(1, 1, 0, 0);
  if ((_sv0t8 != 1)) {
    return 5;
  } else {
  }
  int _sv0t9 = is_try_enum(0, 0, 1, 1);
  if ((_sv0t9 != 1)) {
    return 6;
  } else {
  }
  int _sv0t10 = is_try_enum(1, 0, 0, 0);
  if ((_sv0t10 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_literals(void) {
  int _sv0t0 = classify_literal(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = classify_literal(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = classify_literal(2);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = classify_literal(3);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = classify_literal(99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_value_to_expr(void) {
  int _sv0t0 = value_to_expr_kind(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = value_to_expr_kind(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_enum_alloc(void) {
  int _sv0t0 = enum_alloc_width(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = enum_alloc_width(3);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_ret_value_slot(void) {
  const char* _sv0t0 = ret_value_slot();
  int _sv0t1 = sv0_string_eq(_sv0t0, "_sv0ret");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_use_ret_slot(void) {
  int _sv0t0 = use_ret_slot(1, 1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = use_ret_slot(1, 0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = use_ret_slot(0, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = use_ret_slot(0, 1);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_lit_supported(void) {
  int _sv0t0 = lower_lit_supported(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_lit_supported(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = lower_lit_supported(2);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = lower_lit_supported(3);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = lower_lit_supported(99);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_param_name_tag(void) {
  int _sv0t0 = param_name_tag(1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = param_name_tag(0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = param_name_tag(2);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_uniq_old_names(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 10);
  sv0_vec_push(names, 20);
  sv0_vec_push(names, 10);
  sv0_vec_push(names, 30);
  sv0_vec_push(names, 20);
  int _sv0t1 = uniq_old_names(names);
  int uniq = _sv0t1;
  int _sv0t2 = sv0_vec_len(uniq);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_get(uniq, 0);
  if ((_sv0t3 != 10)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(uniq, 1);
  if ((_sv0t4 != 20)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(uniq, 2);
  if ((_sv0t5 != 30)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int empty = _sv0t6;
  int _sv0t7 = uniq_old_names(empty);
  int uniq2 = _sv0t7;
  int _sv0t8 = sv0_vec_len(uniq2);
  if ((_sv0t8 != 0)) {
    return 5;
  } else {
  }
  int _sv0t9 = old_name_seen(uniq, 10);
  if ((_sv0t9 != 1)) {
    return 6;
  } else {
  }
  int _sv0t10 = old_name_seen(uniq, 99);
  if ((_sv0t10 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_fresh_tmp_str(void) {
  const char* _sv0t0 = fresh_tmp_str(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "_sv0t0");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = fresh_tmp_str(1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "_sv0t1");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = fresh_tmp_str(42);
  int _sv0t5 = sv0_string_eq(_sv0t4, "_sv0t42");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = fresh_tmp_str(100);
  int _sv0t7 = sv0_string_eq(_sv0t6, "_sv0t100");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_lower_lit_ir(void) {
  int _sv0t0 = lower_lit_to_ir_tag(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_lit_to_ir_tag(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = lower_lit_to_ir_tag(2);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  int _sv0t3 = lower_lit_to_ir_tag(3);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  int _sv0t4 = lower_lit_to_ir_tag(99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_fn_ret_cty(void) {
  const char* _sv0t0 = fn_ret_cty(0, "void");
  int _sv0t1 = sv0_string_eq(_sv0t0, "int");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = fn_ret_cty(1, "int32_t");
  int _sv0t3 = sv0_string_eq(_sv0t2, "int32_t");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = fn_ret_cty(1, "void");
  int _sv0t5 = sv0_string_eq(_sv0t4, "void");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_lower_alias(void) {
  int _sv0t0 = lower_alias_new();
  int a = _sv0t0;
  int _sv0t1 = lower_alias_add(a, 10, 20);
  int _sv0t2 = lower_alias_add(a, 30, 40);
  int _sv0t3 = lower_alias_lookup(a, 10);
  if ((_sv0t3 != 20)) {
    return 1;
  } else {
  }
  int _sv0t4 = lower_alias_lookup(a, 30);
  if ((_sv0t4 != 40)) {
    return 2;
  } else {
  }
  int _sv0t5 = lower_alias_lookup(a, 99);
  int _sv0t6 = (0 - 1);
  if ((_sv0t5 != _sv0t6)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_resolve_fn_callee(void) {
  int _sv0t0 = lower_alias_new();
  int a = _sv0t0;
  int _sv0t1 = lower_alias_add(a, 10, 20);
  int _sv0t2 = resolve_fn_callee(a, 10);
  if ((_sv0t2 != 20)) {
    return 1;
  } else {
  }
  int _sv0t3 = resolve_fn_callee(a, 99);
  if ((_sv0t3 != 99)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_resolve_ctor_path(void) {
  int _sv0t0 = sv0_vec_new();
  int fe = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int fv = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int te = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int tv = _sv0t3;
  sv0_vec_push(fe, 10);
  sv0_vec_push(fv, 20);
  sv0_vec_push(te, 30);
  sv0_vec_push(tv, 40);
  int _sv0t4 = resolve_enum_ctor_path(fe, fv, te, tv, 10, 20);
  int idx = _sv0t4;
  if ((idx != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = resolved_ctor_enum(te, idx, 10);
  if ((_sv0t5 != 30)) {
    return 2;
  } else {
  }
  int _sv0t6 = resolved_ctor_variant(tv, idx, 20);
  if ((_sv0t6 != 40)) {
    return 3;
  } else {
  }
  int _sv0t7 = resolve_enum_ctor_path(fe, fv, te, tv, 99, 88);
  int miss = _sv0t7;
  int _sv0t8 = (0 - 1);
  if ((miss != _sv0t8)) {
    return 4;
  } else {
  }
  int _sv0t9 = resolved_ctor_enum(te, miss, 99);
  if ((_sv0t9 != 99)) {
    return 5;
  } else {
  }
  int _sv0t10 = resolved_ctor_variant(tv, miss, 88);
  if ((_sv0t10 != 88)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_param_name(void) {
  int _sv0t0 = param_name(1, 42);
  if ((_sv0t0 != 42)) {
    return 1;
  } else {
  }
  int _sv0t1 = param_name(0, 42);
  int _sv0t2 = (0 - 1);
  if ((_sv0t1 != _sv0t2)) {
    return 2;
  } else {
  }
  int _sv0t3 = param_name(3, 99);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_lower_field_index(void) {
  int _sv0t0 = sv0_vec_new();
  int fields = _sv0t0;
  sv0_vec_push(fields, 10);
  sv0_vec_push(fields, 20);
  sv0_vec_push(fields, 30);
  int _sv0t1 = lower_field_index(fields, 10);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_field_index(fields, 20);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = lower_field_index(fields, 30);
  if ((_sv0t3 != 2)) {
    return 3;
  } else {
  }
  int _sv0t4 = lower_field_index(fields, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int empty = _sv0t6;
  int _sv0t7 = lower_field_index(empty, 10);
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_ast_ty_c_string_user(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(sn, 100);
  sv0_vec_push(en, 200);
  const char* _sv0t2 = ast_ty_to_c_string_with_user("i32", 50, sn, en);
  int _sv0t3 = sv0_string_eq(_sv0t2, "int");
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t4 = ast_ty_to_c_string_with_user("MyStruct", 100, sn, en);
  int _sv0t5 = sv0_string_eq(_sv0t4, "MyStruct");
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t6 = ast_ty_to_c_string_with_user("MyEnum", 200, sn, en);
  int _sv0t7 = sv0_string_eq(_sv0t6, "MyEnum");
  if ((_sv0t7 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t8 = ast_ty_to_c_string_with_user("Unknown", 999, sn, en);
  int _sv0t9 = sv0_string_eq(_sv0t8, "int");
  if ((_sv0t9 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t10 = ast_ty_to_c_string_with_user("bool", 999, sn, en);
  int _sv0t11 = sv0_string_eq(_sv0t10, "int");
  if ((_sv0t11 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_find_fn_index(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 10);
  sv0_vec_push(names, 20);
  sv0_vec_push(names, 30);
  int _sv0t1 = find_fn_index(names, 10);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = find_fn_index(names, 20);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = find_fn_index(names, 30);
  if ((_sv0t3 != 2)) {
    return 3;
  } else {
  }
  int _sv0t4 = find_fn_index(names, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_callee_fn_index(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 10);
  sv0_vec_push(names, 20);
  int _sv0t1 = sv0_vec_new();
  int aliases = _sv0t1;
  sv0_vec_push(aliases, 30);
  sv0_vec_push(aliases, 10);
  int _sv0t2 = callee_fn_index(names, aliases, 10);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = callee_fn_index(names, aliases, 30);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  int _sv0t4 = callee_fn_index(names, aliases, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_scrut_local(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 50);
  sv0_vec_push(names, 60);
  int _sv0t1 = scrut_local_lookup(names, 50);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = scrut_local_lookup(names, 60);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = scrut_local_lookup(names, 99);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_param_ty(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 70);
  sv0_vec_push(names, 80);
  int _sv0t1 = param_ty_lookup(names, 70);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = param_ty_lookup(names, 80);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = param_ty_lookup(names, 99);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_mentions_result(void) {
  int _sv0t0 = sv0_vec_new();
  int _sv0t1 = sv0_vec_new();
  int _sv0t2 = sv0_vec_new();
  int _sv0t3 = sv0_vec_new();
  int _sv0t4 = sv0_vec_new();
  int _sv0t5 = (0 - 1);
  int _sv0t6 = sv0_vec_new();
  int _sv0t7 = sv0_vec_new();
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = mentions_result(_sv0t0, _sv0t1, _sv0t2, _sv0t3, _sv0t4, _sv0t5, "", _sv0t6, _sv0t7, _sv0t8);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  const char* src;
  src = "result x old";
  int _sv0t10 = sv0_vec_new();
  int st = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int nd = _sv0t11;
  sv0_vec_push(st, 0);
  sv0_vec_push(nd, 6);
  sv0_vec_push(st, 7);
  sv0_vec_push(nd, 8);
  sv0_vec_push(st, 9);
  sv0_vec_push(nd, 12);
  int _sv0t12 = sv0_vec_new();
  int pp = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int et = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int d1 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int d2 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int d3 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int d4 = _sv0t17;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(et, 1);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(d4, 0);
  int _sv0t18 = mentions_result(et, d1, d2, d3, d4, 0, src, st, nd, pp);
  if ((_sv0t18 != 1)) {
    return 2;
  } else {
  }
  int _sv0t19 = sv0_vec_new();
  int et2 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int d1b = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int d2b = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int d3b = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int d4b = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int pp2 = _sv0t24;
  sv0_vec_push(pp2, 1);
  sv0_vec_push(et2, 1);
  sv0_vec_push(d1b, 0);
  sv0_vec_push(d2b, 1);
  sv0_vec_push(d3b, 0);
  sv0_vec_push(d4b, 0);
  int _sv0t25 = mentions_result(et2, d1b, d2b, d3b, d4b, 0, src, st, nd, pp2);
  if ((_sv0t25 != 0)) {
    return 3;
  } else {
  }
  int _sv0t26 = sv0_vec_new();
  int et3 = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int d1c = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int d2c = _sv0t28;
  int _sv0t29 = sv0_vec_new();
  int d3c = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int d4c = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int pp3 = _sv0t31;
  sv0_vec_push(pp3, 0);
  sv0_vec_push(pp3, 1);
  sv0_vec_push(et3, 1);
  sv0_vec_push(d1c, 1);
  sv0_vec_push(d2c, 1);
  sv0_vec_push(d3c, 0);
  sv0_vec_push(d4c, 0);
  sv0_vec_push(et3, 1);
  sv0_vec_push(d1c, 0);
  sv0_vec_push(d2c, 1);
  sv0_vec_push(d3c, 0);
  sv0_vec_push(d4c, 0);
  sv0_vec_push(et3, 3);
  sv0_vec_push(d1c, 0);
  sv0_vec_push(d2c, 0);
  sv0_vec_push(d3c, 1);
  sv0_vec_push(d4c, 0);
  int _sv0t32 = mentions_result(et3, d1c, d2c, d3c, d4c, 2, src, st, nd, pp3);
  if ((_sv0t32 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_all_old_names(void) {
  int _sv0t0 = sv0_vec_new();
  int out0 = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int _sv0t2 = sv0_vec_new();
  int _sv0t3 = sv0_vec_new();
  int _sv0t4 = sv0_vec_new();
  int _sv0t5 = sv0_vec_new();
  int _sv0t6 = (0 - 1);
  int _sv0t7 = sv0_vec_new();
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = sv0_vec_new();
  int _sv0t10 = all_old_names_expr(_sv0t1, _sv0t2, _sv0t3, _sv0t4, _sv0t5, _sv0t6, "", _sv0t7, _sv0t8, _sv0t9, out0);
  int _sv0t11 = sv0_vec_len(out0);
  if ((_sv0t11 != 0)) {
    return 1;
  } else {
  }
  const char* src;
  src = "old x y";
  int _sv0t12 = sv0_vec_new();
  int st = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int nd = _sv0t13;
  sv0_vec_push(st, 0);
  sv0_vec_push(nd, 3);
  sv0_vec_push(st, 4);
  sv0_vec_push(nd, 5);
  sv0_vec_push(st, 6);
  sv0_vec_push(nd, 7);
  int _sv0t14 = sv0_vec_new();
  int pp = _sv0t14;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(pp, 2);
  int _sv0t15 = sv0_vec_new();
  int et = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int d1 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int d2 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int d3 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int d4 = _sv0t19;
  sv0_vec_push(et, 1);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(d4, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(d1, 1);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(d4, 0);
  sv0_vec_push(et, 4);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 1);
  sv0_vec_push(d4, 0);
  int _sv0t20 = sv0_vec_new();
  int out = _sv0t20;
  int _sv0t21 = all_old_names_expr(et, d1, d2, d3, d4, 2, src, st, nd, pp, out);
  int _sv0t22 = sv0_vec_len(out);
  if ((_sv0t22 != 1)) {
    return 2;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out, 0);
  if ((_sv0t23 != 1)) {
    return 3;
  } else {
  }
  int _sv0t24 = sv0_vec_new();
  int et2 = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int e1 = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int e2 = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int e3 = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int e4 = _sv0t28;
  sv0_vec_push(et2, 1);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 1);
  sv0_vec_push(e1, 1);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 4);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 1);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 1);
  sv0_vec_push(e1, 2);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 4);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 3);
  sv0_vec_push(e3, 1);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 3);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 2);
  sv0_vec_push(e3, 4);
  sv0_vec_push(e4, 0);
  int _sv0t29 = sv0_vec_new();
  int out3 = _sv0t29;
  int _sv0t30 = all_old_names_expr(et2, e1, e2, e3, e4, 5, src, st, nd, pp, out3);
  int _sv0t31 = sv0_vec_len(out3);
  if ((_sv0t31 != 2)) {
    return 4;
  } else {
  }
  int _sv0t32 = sv0_vec_get(out3, 0);
  if ((_sv0t32 != 1)) {
    return 5;
  } else {
  }
  int _sv0t33 = sv0_vec_get(out3, 1);
  if ((_sv0t33 != 2)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_typedef_builders(void) {
  const char* src;
  src = "P x i32 y bool E";
  int _sv0t0 = sv0_vec_new();
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int nd = _sv0t1;
  sv0_vec_push(st, 0);
  sv0_vec_push(nd, 1);
  sv0_vec_push(st, 2);
  sv0_vec_push(nd, 3);
  sv0_vec_push(st, 4);
  sv0_vec_push(nd, 7);
  sv0_vec_push(st, 8);
  sv0_vec_push(nd, 9);
  sv0_vec_push(st, 10);
  sv0_vec_push(nd, 14);
  sv0_vec_push(st, 15);
  sv0_vec_push(nd, 16);
  const char* _sv0t2 = handle_to_str(0, src, st, nd);
  int _sv0t3 = sv0_string_eq(_sv0t2, "P");
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_new();
  int sn = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int en = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fv = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int tv = _sv0t7;
  sv0_vec_push(fv, 1);
  sv0_vec_push(fv, 3);
  sv0_vec_push(tv, 2);
  sv0_vec_push(tv, 4);
  const char* _sv0t8 = emit_struct_td(0, fv, tv, 0, 2, src, st, nd, sn, en);
  const char* td;
  td = _sv0t8;
  int _sv0t9 = sv0_string_eq(td, "typedef struct {\n  int x;\n  int y;\n} P;\n");
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t10 = emit_enum_td(5, 1, src, st, nd);
  const char* td2;
  td2 = _sv0t10;
  int _sv0t11 = sv0_string_eq(td2, "typedef struct {\n  int tag;\n  int p0;\n} E;\n");
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int tags = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int nms = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int fcs = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int vmx = _sv0t15;
  sv0_vec_push(tags, 1);
  sv0_vec_push(nms, 0);
  sv0_vec_push(fcs, 2);
  sv0_vec_push(vmx, 0);
  sv0_vec_push(tags, 2);
  sv0_vec_push(nms, 5);
  sv0_vec_push(fcs, 2);
  sv0_vec_push(vmx, 1);
  int _sv0t16 = sv0_vec_new();
  int sff = _sv0t16;
  sv0_vec_push(sff, 1);
  sv0_vec_push(sff, 3);
  int _sv0t17 = sv0_vec_new();
  int on = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int oo = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int oc = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int of1 = _sv0t20;
  int _sv0t21 = build_struct_order(tags, nms, fcs, sff, on, oo, oc, of1);
  if ((_sv0t21 != 1)) {
    return 4;
  } else {
  }
  int _sv0t22 = sv0_vec_get(oc, 0);
  if ((_sv0t22 != 2)) {
    return 5;
  } else {
  }
  int _sv0t23 = sv0_vec_new();
  int evn = _sv0t23;
  sv0_vec_push(evn, 10);
  sv0_vec_push(evn, 20);
  int _sv0t24 = sv0_vec_new();
  int en2 = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int eto = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int etc = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int etf = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int emp = _sv0t28;
  int _sv0t29 = build_enum_variants_data(tags, nms, fcs, vmx, evn, en2, eto, etc, etf, emp);
  if ((_sv0t29 != 1)) {
    return 6;
  } else {
  }
  int _sv0t30 = sv0_vec_get(emp, 0);
  if ((_sv0t30 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_expr_init_cty(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fnn = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int frc = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int al = _sv0t8;
  sv0_vec_push(pp, 100);
  sv0_vec_push(et, 24);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = expr_init_cty(et, ed1, ed2, ed3, ed4, 0, pp, fnn, frc, al);
  int r1 = _sv0t9;
  if ((r1 != 100)) {
    return 1;
  } else {
  }
  sv0_vec_push(pp, 200);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 2);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t10 = expr_init_cty(et, ed1, ed2, ed3, ed4, 1, pp, fnn, frc, al);
  int r2 = _sv0t10;
  if ((r2 != 100)) {
    return 2;
  } else {
  }
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t11 = expr_init_cty(et, ed1, ed2, ed3, ed4, 2, pp, fnn, frc, al);
  int r3 = _sv0t11;
  int _sv0t12 = (0 - 1);
  if ((r3 != _sv0t12)) {
    return 3;
  } else {
  }
  int _sv0t13 = (0 - 1);
  int _sv0t14 = expr_init_cty(et, ed1, ed2, ed3, ed4, _sv0t13, pp, fnn, frc, al);
  int r4 = _sv0t14;
  int _sv0t15 = (0 - 1);
  if ((r4 != _sv0t15)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_try_variant_lookup(void) {
  const char* src;
  src = "Ok Err Some None Other";
  int _sv0t0 = sv0_vec_new();
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int nd = _sv0t1;
  sv0_vec_push(st, 0);
  sv0_vec_push(nd, 2);
  sv0_vec_push(st, 3);
  sv0_vec_push(nd, 6);
  sv0_vec_push(st, 7);
  sv0_vec_push(nd, 11);
  sv0_vec_push(st, 12);
  sv0_vec_push(nd, 16);
  sv0_vec_push(st, 17);
  sv0_vec_push(nd, 22);
  int _sv0t2 = sv0_vec_new();
  int en = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int eto = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int etc = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int etf = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int emp = _sv0t6;
  sv0_vec_push(en, 100);
  sv0_vec_push(eto, 0);
  sv0_vec_push(etc, 2);
  sv0_vec_push(emp, 1);
  sv0_vec_push(etf, 0);
  sv0_vec_push(etf, 0);
  sv0_vec_push(etf, 1);
  sv0_vec_push(etf, 1);
  int _sv0t7 = sv0_vec_new();
  int out1 = _sv0t7;
  int _sv0t8 = try_variant_names_lookup(en, eto, etc, etf, src, st, nd, 100, out1);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(out1);
  if ((_sv0t9 != 2)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(out1, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out1, 1);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int en2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int eto2 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int etc2 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int etf2 = _sv0t15;
  sv0_vec_push(en2, 200);
  sv0_vec_push(eto2, 0);
  sv0_vec_push(etc2, 2);
  sv0_vec_push(etf2, 2);
  sv0_vec_push(etf2, 0);
  sv0_vec_push(etf2, 3);
  sv0_vec_push(etf2, 1);
  int _sv0t16 = sv0_vec_new();
  int out2 = _sv0t16;
  int _sv0t17 = try_variant_names_lookup(en2, eto2, etc2, etf2, src, st, nd, 200, out2);
  if ((_sv0t17 != 1)) {
    return 5;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out2, 0);
  if ((_sv0t18 != 2)) {
    return 6;
  } else {
  }
  int _sv0t19 = sv0_vec_get(out2, 1);
  if ((_sv0t19 != 3)) {
    return 7;
  } else {
  }
  int _sv0t20 = sv0_vec_new();
  int en3 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int eto3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int etc3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int etf3 = _sv0t23;
  sv0_vec_push(en3, 300);
  sv0_vec_push(eto3, 0);
  sv0_vec_push(etc3, 2);
  sv0_vec_push(etf3, 0);
  sv0_vec_push(etf3, 0);
  sv0_vec_push(etf3, 4);
  sv0_vec_push(etf3, 1);
  int _sv0t24 = sv0_vec_new();
  int out3 = _sv0t24;
  int _sv0t25 = try_variant_names_lookup(en3, eto3, etc3, etf3, src, st, nd, 300, out3);
  if ((_sv0t25 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_find_variant_in_items(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int nms = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int fcs = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int vnf = _sv0t3;
  sv0_vec_push(tags, 1);
  sv0_vec_push(nms, 10);
  sv0_vec_push(fcs, 2);
  sv0_vec_push(tags, 2);
  sv0_vec_push(nms, 20);
  sv0_vec_push(fcs, 3);
  sv0_vec_push(vnf, 30);
  sv0_vec_push(vnf, 31);
  sv0_vec_push(vnf, 32);
  int _sv0t4 = find_variant_in_items(tags, nms, fcs, vnf, 20, 30);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = find_variant_in_items(tags, nms, fcs, vnf, 20, 32);
  if ((_sv0t5 != 2)) {
    return 2;
  } else {
  }
  int _sv0t6 = find_variant_in_items(tags, nms, fcs, vnf, 20, 99);
  int _sv0t7 = (0 - 2);
  if ((_sv0t6 != _sv0t7)) {
    return 3;
  } else {
  }
  int _sv0t8 = find_variant_in_items(tags, nms, fcs, vnf, 99, 30);
  int _sv0t9 = (0 - 1);
  if ((_sv0t8 != _sv0t9)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_scan_lets(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fnn = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int frc = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int al = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 5);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, 42);
  int _sv0t9 = (0 - 1);
  sv0_vec_push(ed2, _sv0t9);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 9);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 1);
  int _sv0t10 = (0 - 1);
  sv0_vec_push(ed3, _sv0t10);
  sv0_vec_push(ed4, 0);
  int _sv0t11 = sv0_vec_new();
  int on = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int oc = _sv0t12;
  int _sv0t13 = scan_lets(et, ed1, ed2, ed3, ed4, 2, pp, fnn, frc, al, on, oc);
  int c = _sv0t13;
  if ((c != 1)) {
    return 1;
  } else {
  }
  int _sv0t14 = sv0_vec_get(on, 0);
  if ((_sv0t14 != 42)) {
    return 2;
  } else {
  }
  int _sv0t15 = sv0_vec_get(oc, 0);
  int _sv0t16 = (0 - 1);
  if ((_sv0t15 != _sv0t16)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_new();
  int on2 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int oc2 = _sv0t18;
  int _sv0t19 = (0 - 1);
  int _sv0t20 = scan_lets(et, ed1, ed2, ed3, ed4, _sv0t19, pp, fnn, frc, al, on2, oc2);
  int c2 = _sv0t20;
  if ((c2 != 0)) {
    return 4;
  } else {
  }
  int _sv0t21 = scan_lets(et, ed1, ed2, ed3, ed4, 0, pp, fnn, frc, al, on2, oc2);
  int c3 = _sv0t21;
  if ((c3 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_fresh_tmp();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_split_qname();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_binop_to_c();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (30 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_unop_to_c();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (50 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_ty_to_c();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (60 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_old_slot();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (80 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_ret_syntax();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (90 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_store_value_class();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (100 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_assign_lhs();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (110 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_enum_tag();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (120 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_struct_fields();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (130 + r11);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_type_detection();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (140 + r12);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_variant_slots();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (150 + r13);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_try_variants();
  int r14 = _sv0t25;
  if ((r14 != 0)) {
    int _sv0t26 = (160 + r14);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = test_literals();
  int r15 = _sv0t27;
  if ((r15 != 0)) {
    int _sv0t28 = (170 + r15);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = test_value_to_expr();
  int r16 = _sv0t29;
  if ((r16 != 0)) {
    int _sv0t30 = (180 + r16);
    return _sv0t30;
  } else {
  }
  int _sv0t31 = test_enum_alloc();
  int r17 = _sv0t31;
  if ((r17 != 0)) {
    int _sv0t32 = (190 + r17);
    return _sv0t32;
  } else {
  }
  int _sv0t33 = test_ret_value_slot();
  int r18 = _sv0t33;
  if ((r18 != 0)) {
    int _sv0t34 = (200 + r18);
    return _sv0t34;
  } else {
  }
  int _sv0t35 = test_use_ret_slot();
  int r19 = _sv0t35;
  if ((r19 != 0)) {
    int _sv0t36 = (210 + r19);
    return _sv0t36;
  } else {
  }
  int _sv0t37 = test_lit_supported();
  int r20 = _sv0t37;
  if ((r20 != 0)) {
    int _sv0t38 = (220 + r20);
    return _sv0t38;
  } else {
  }
  int _sv0t39 = test_param_name_tag();
  int r21 = _sv0t39;
  if ((r21 != 0)) {
    int _sv0t40 = (230 + r21);
    return _sv0t40;
  } else {
  }
  int _sv0t41 = test_uniq_old_names();
  int r22 = _sv0t41;
  if ((r22 != 0)) {
    int _sv0t42 = (240 + r22);
    return _sv0t42;
  } else {
  }
  int _sv0t43 = test_fresh_tmp_str();
  int r23 = _sv0t43;
  if ((r23 != 0)) {
    int _sv0t44 = (250 + r23);
    return _sv0t44;
  } else {
  }
  int _sv0t45 = test_lower_lit_ir();
  int r24 = _sv0t45;
  if ((r24 != 0)) {
    int _sv0t46 = (260 + r24);
    return _sv0t46;
  } else {
  }
  int _sv0t47 = test_fn_ret_cty();
  int r25 = _sv0t47;
  if ((r25 != 0)) {
    int _sv0t48 = (270 + r25);
    return _sv0t48;
  } else {
  }
  int _sv0t49 = test_lower_alias();
  int r26 = _sv0t49;
  if ((r26 != 0)) {
    int _sv0t50 = (280 + r26);
    return _sv0t50;
  } else {
  }
  int _sv0t51 = test_resolve_fn_callee();
  int r27 = _sv0t51;
  if ((r27 != 0)) {
    int _sv0t52 = (290 + r27);
    return _sv0t52;
  } else {
  }
  int _sv0t53 = test_resolve_ctor_path();
  int r28 = _sv0t53;
  if ((r28 != 0)) {
    int _sv0t54 = (300 + r28);
    return _sv0t54;
  } else {
  }
  int _sv0t55 = test_param_name();
  int r29 = _sv0t55;
  if ((r29 != 0)) {
    int _sv0t56 = (310 + r29);
    return _sv0t56;
  } else {
  }
  int _sv0t57 = test_lower_field_index();
  int r30 = _sv0t57;
  if ((r30 != 0)) {
    int _sv0t58 = (320 + r30);
    return _sv0t58;
  } else {
  }
  int _sv0t59 = test_ast_ty_c_string_user();
  int r31 = _sv0t59;
  if ((r31 != 0)) {
    int _sv0t60 = (330 + r31);
    return _sv0t60;
  } else {
  }
  int _sv0t61 = test_find_fn_index();
  int r32 = _sv0t61;
  if ((r32 != 0)) {
    int _sv0t62 = (340 + r32);
    return _sv0t62;
  } else {
  }
  int _sv0t63 = test_callee_fn_index();
  int r33 = _sv0t63;
  if ((r33 != 0)) {
    int _sv0t64 = (350 + r33);
    return _sv0t64;
  } else {
  }
  int _sv0t65 = test_scrut_local();
  int r34 = _sv0t65;
  if ((r34 != 0)) {
    int _sv0t66 = (360 + r34);
    return _sv0t66;
  } else {
  }
  int _sv0t67 = test_param_ty();
  int r35 = _sv0t67;
  if ((r35 != 0)) {
    int _sv0t68 = (370 + r35);
    return _sv0t68;
  } else {
  }
  int _sv0t69 = test_typedef_builders();
  int r36 = _sv0t69;
  if ((r36 != 0)) {
    int _sv0t70 = (380 + r36);
    return _sv0t70;
  } else {
  }
  int _sv0t71 = test_mentions_result();
  int r37 = _sv0t71;
  if ((r37 != 0)) {
    int _sv0t72 = (390 + r37);
    return _sv0t72;
  } else {
  }
  int _sv0t73 = test_all_old_names();
  int r38 = _sv0t73;
  if ((r38 != 0)) {
    int _sv0t74 = (400 + r38);
    return _sv0t74;
  } else {
  }
  int _sv0t75 = test_expr_init_cty();
  int r39 = _sv0t75;
  if ((r39 != 0)) {
    int _sv0t76 = (410 + r39);
    return _sv0t76;
  } else {
  }
  int _sv0t77 = test_scan_lets();
  int r40 = _sv0t77;
  if ((r40 != 0)) {
    int _sv0t78 = (420 + r40);
    return _sv0t78;
  } else {
  }
  int _sv0t79 = test_try_variant_lookup();
  int r41 = _sv0t79;
  if ((r41 != 0)) {
    int _sv0t80 = (430 + r41);
    return _sv0t80;
  } else {
  }
  int _sv0t81 = test_find_variant_in_items();
  int r42 = _sv0t81;
  if ((r42 != 0)) {
    int _sv0t82 = (440 + r42);
    return _sv0t82;
  } else {
  }
  return 0;
}

