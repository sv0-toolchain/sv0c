#include "sv0_runtime.h"

static const char* path_join2(const char* a, const char* b);
static const char* mangle_use_target(const char* mod_name, const char* item_name);
static const char* path_join_vec(const char* source, int starts, int ends, int path_indices);
static int import_alias_new(void);
static int import_alias_add(int aliases, int from_handle, int to_handle);
static int import_alias_count(int aliases);
static int import_alias_lookup(int aliases, int name_handle);
static int is_intrinsic(const char* name);
static int intrinsic_arity(const char* name);
static int intrinsic_count(void);
static int is_string_intrinsic(const char* name);
static int is_vec_intrinsic(const char* name);
static int is_box_intrinsic(const char* name);
static int register_one_intrinsic(int fn_arities, int mod_values, int name, int arity);
static int register_all_intrinsics(int fn_arities, int mod_values, int println_h, int old_h, int forall_h, int exists_h, int no_alias_h, int str_len_h, int str_eq_h, int str_concat_h, int str_char_at_h, int str_substr_h, int vec_new_h, int vec_push_h, int vec_len_h, int vec_get_h, int vec_set_h, int box_new_h, int box_deref_h);
static int item_registers_fn(int tag);
static int item_registers_type(int tag);
static int item_registers_variants(int tag);
static int item_is_use(int tag);
static int item_is_module(int tag);
static int item_needs_registration(int tag);
static int enum_variant_arity(int variant_kind, int tuple_fields, int struct_fields);
static const char* enum_variant_qualified_path(const char* enum_name, const char* variant_name);
static int variant_stem(int variant_kind, int name_handle);
static const char* enum_variant_reg_path(const char* enum_name, const char* variant_name);
static int enum_variant_reg_arity(int variant_kind, int tuple_fields, int struct_fields);
static int pat_binds_count(int pat_tag, int child_count);
static int pat_is_or(int pat_tag);
static int pat_binds(int pat_tag, int name_handle, int out);
static int bind_pattern_locals(int binds, int env_locals);
static int should_allow_self(int item_tag);
static const char* error_code_unbound_ident(void);
static const char* error_code_unknown_type(void);
static const char* error_code_pat_or_unsupported(void);
static const char* error_code_wrong_arity(void);
static const char* error_code_use_clause(void);
static int diag_pack(int code, int tok);
static int diag_code_of(int packed);
static int diag_tok_of(int packed);
static int arity_matches(int expected, int actual);
static int check_call_arity(int expected, int actual);
static int register_item(int item_tag, int item_d1, int item_d2, int item_d3, int item_d4, int mod_vals, int mod_tys, int fn_arities);
static int register_items(int it, int id1, int id2, int id3, int id4, int mod_vals, int mod_tys, int fn_arities);
static const char* tok_str(const char* source, int starts, int ends, int pos);
static int res_is_prelude_type(const char* name);
static int res_value_exists(int mod_vals, const char* name_str, const char* source, int starts, int ends);
static int res_type_exists(int mod_tys, const char* name_str, const char* source, int starts, int ends);
static int resolve_pat_shape(int pt, int pd1, int pd2, int pd3, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int pp);
static int resolve_ty(int tt, int td1, int td2, int td3, int idx, const char* source, int starts, int ends, int mod_tys, int mod_vals, int pp);
static int res_enter_scope(int frames);
static int res_bind_local(int frames, int name_pos);
static int res_lookup_in_frames_str(int frames, const char* name_str, const char* source, int starts, int ends);
static int res_full_value_exists(int mod_vals, int frames, const char* name_str, const char* source, int starts, int ends);
static int res_lookup_fn_arity_str(int fn_arities, const char* name_str, const char* source, int starts, int ends);
static int block_stmt_index(int pp, int stmts_first, int sidecar, int si);
static int resolve_expr(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int frames, int pp);
static int resolve_stmt(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int frames, int pp);
static int resolve_contract(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int frames, int pp);
static int resolve_expr_cast_roots_program(int et, int ed2, int pty_tt, int pty_td1, int pty_td2, int pty_td3, const char* source, int starts, int ends, int mod_tys, int mod_vals, int pp);
static int apply_use_clause(int it, int id1, int id2, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int pp);
static int apply_use_clauses(int it, int id1, int id2, int id3, int id4, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int pp);
static int resolve_top_item(int it, int id1, int id2, int id3, int id4, int id5, int idx, int et, int ed1, int ed2, int ed3, int ed4, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int pp);
static int resolve_program(int et, int ed1, int ed2, int ed3, int ed4, int it, int id1, int id2, int id3, int id4, int id5, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int pp);
static int test_path_join(void);
static int test_mangle_use(void);
static int test_is_intrinsic(void);
static int test_intrinsic_arity(void);
static int test_intrinsic_classify(void);
static int test_item_classify(void);
static int test_variant_arity(void);
static int test_pat_binds(void);
static int test_context(void);
static int test_error_codes(void);
static int test_arity_check(void);
static int test_path_join_vec(void);
static int test_import_alias(void);
static int test_register_intrinsics(void);
static int test_variant_stem(void);
static int test_enum_variant_reg(void);
static int test_pat_binds_vec(void);
static int test_bind_pattern_locals(void);
static int test_register_item_fn(void);
static int test_register_item_struct(void);
static int test_register_items(void);
static int test_tok_str(void);
static int test_res_value_exists(void);
static int test_res_type_exists(void);
static int test_resolve_pat_shape_simple(void);
static int test_resolve_pat_enum(void);
static int test_resolve_ty_simple(void);
static int test_resolve_ty_array_len(void);
static int test_resolve_ty_ref(void);
static int test_scope_mgmt(void);
static int test_resolve_expr_lit(void);
static int test_resolve_expr_path(void);
static int test_resolve_expr_block(void);
static int test_resolve_expr_call_arity(void);
static int test_resolve_stmt_let(void);
static int test_apply_use_clause_value(void);
static int test_apply_use_clause_type(void);
static int test_apply_use_clause_unknown(void);
static int test_apply_use_clause_unmangled(void);
static int test_path_join_vec_three(void);
static int test_resolve_top_item_struct_field(void);
static int test_resolve_top_item_fn_signature_contracts(void);
static int test_resolve_top_item_enum_payload_type(void);
static int test_resolve_program(void);
static int test_resolve_program_cast_type_roots(void);

static const char* path_join2(const char* a, const char* b) {
  const char* _sv0t0 = sv0_string_concat(a, "::");
  const char* ab;
  ab = _sv0t0;
  const char* _sv0t1 = sv0_string_concat(ab, b);
  return _sv0t1;
}

static const char* mangle_use_target(const char* mod_name, const char* item_name) {
  const char* _sv0t0 = sv0_string_concat(mod_name, "__");
  const char* a;
  a = _sv0t0;
  const char* _sv0t1 = sv0_string_concat(a, item_name);
  return _sv0t1;
}

static const char* path_join_vec(const char* source, int starts, int ends, int path_indices) {
  int _sv0t0 = sv0_vec_len(path_indices);
  int n = _sv0t0;
  if ((n == 0)) {
    return "";
  } else {
  }
  int _sv0t1 = sv0_vec_get(path_indices, 0);
  int first_pos = _sv0t1;
  int _sv0t2 = sv0_vec_get(starts, first_pos);
  int fs = _sv0t2;
  int _sv0t3 = sv0_vec_get(ends, first_pos);
  int fe = _sv0t3;
  int _sv0t4 = (fe - fs);
  const char* _sv0t5 = sv0_string_substr(source, fs, _sv0t4);
  const char* out;
  out = _sv0t5;
  int i = 1;
  while ((i < n)) {
    const char* _sv0t6 = sv0_string_concat(out, "::");
    out = _sv0t6;
    int _sv0t7 = sv0_vec_get(path_indices, i);
    int pos = _sv0t7;
    int _sv0t8 = sv0_vec_get(starts, pos);
    int s = _sv0t8;
    int _sv0t9 = sv0_vec_get(ends, pos);
    int e = _sv0t9;
    int _sv0t10 = (e - s);
    const char* _sv0t11 = sv0_string_substr(source, s, _sv0t10);
    const char* seg;
    seg = _sv0t11;
    const char* _sv0t12 = sv0_string_concat(out, seg);
    out = _sv0t12;
    i = (i + 1);
  }
  return out;
}

static int import_alias_new(void) {
  int _sv0t0 = sv0_vec_new();
  return _sv0t0;
}

static int import_alias_add(int aliases, int from_handle, int to_handle) {
  sv0_vec_push(aliases, from_handle);
  sv0_vec_push(aliases, to_handle);
  int _sv0t0 = sv0_vec_len(aliases);
  int _sv0t1 = (_sv0t0 / 2);
  return _sv0t1;
}

static int import_alias_count(int aliases) {
  int _sv0t0 = sv0_vec_len(aliases);
  int _sv0t1 = (_sv0t0 / 2);
  return _sv0t1;
}

static int import_alias_lookup(int aliases, int name_handle) {
  int _sv0t0 = sv0_vec_len(aliases);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(aliases, i);
    if ((_sv0t1 == name_handle)) {
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

static int is_intrinsic(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "println");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "old");
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "forall");
  if (_sv0t2) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "exists");
  if (_sv0t3) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "no_alias");
  if (_sv0t4) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "string_len");
  if (_sv0t5) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "string_eq");
  if (_sv0t6) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "string_concat");
  if (_sv0t7) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "string_char_at");
  if (_sv0t8) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "string_substr");
  if (_sv0t9) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "vec_new");
  if (_sv0t10) {
    return 1;
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "vec_push");
  if (_sv0t11) {
    return 1;
  } else {
  }
  int _sv0t12 = sv0_string_eq(name, "vec_len");
  if (_sv0t12) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_string_eq(name, "vec_get");
  if (_sv0t13) {
    return 1;
  } else {
  }
  int _sv0t14 = sv0_string_eq(name, "vec_set");
  if (_sv0t14) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_string_eq(name, "box_new");
  if (_sv0t15) {
    return 1;
  } else {
  }
  int _sv0t16 = sv0_string_eq(name, "box_deref");
  if (_sv0t16) {
    return 1;
  } else {
  }
  int _sv0t17 = sv0_string_eq(name, "read_file");
  if (_sv0t17) {
    return 1;
  } else {
  }
  int _sv0t18 = sv0_string_eq(name, "write_file");
  if (_sv0t18) {
    return 1;
  } else {
  }
  int _sv0t19 = sv0_string_eq(name, "read_dir");
  if (_sv0t19) {
    return 1;
  } else {
  }
  int _sv0t20 = sv0_string_eq(name, "write_bytes");
  if (_sv0t20) {
    return 1;
  } else {
  }
  return 0;
}

static int intrinsic_arity(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "println");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "old");
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "forall");
  if (_sv0t2) {
    return 3;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "exists");
  if (_sv0t3) {
    return 3;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "no_alias");
  if (_sv0t4) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "string_len");
  if (_sv0t5) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "string_eq");
  if (_sv0t6) {
    return 2;
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "string_concat");
  if (_sv0t7) {
    return 2;
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "string_char_at");
  if (_sv0t8) {
    return 2;
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "string_substr");
  if (_sv0t9) {
    return 3;
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "vec_new");
  if (_sv0t10) {
    return 0;
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "vec_push");
  if (_sv0t11) {
    return 2;
  } else {
  }
  int _sv0t12 = sv0_string_eq(name, "vec_len");
  if (_sv0t12) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_string_eq(name, "vec_get");
  if (_sv0t13) {
    return 2;
  } else {
  }
  int _sv0t14 = sv0_string_eq(name, "vec_set");
  if (_sv0t14) {
    return 3;
  } else {
  }
  int _sv0t15 = sv0_string_eq(name, "box_new");
  if (_sv0t15) {
    return 1;
  } else {
  }
  int _sv0t16 = sv0_string_eq(name, "box_deref");
  if (_sv0t16) {
    return 1;
  } else {
  }
  int _sv0t17 = sv0_string_eq(name, "read_file");
  if (_sv0t17) {
    return 1;
  } else {
  }
  int _sv0t18 = sv0_string_eq(name, "write_file");
  if (_sv0t18) {
    return 2;
  } else {
  }
  int _sv0t19 = sv0_string_eq(name, "read_dir");
  if (_sv0t19) {
    return 1;
  } else {
  }
  int _sv0t20 = sv0_string_eq(name, "write_bytes");
  if (_sv0t20) {
    return 2;
  } else {
  }
  int _sv0t21 = (0 - 1);
  return _sv0t21;
}

static int intrinsic_count(void) {
  return 17;
}

static int is_string_intrinsic(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "string_len");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "string_eq");
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "string_concat");
  if (_sv0t2) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "string_char_at");
  if (_sv0t3) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "string_substr");
  if (_sv0t4) {
    return 1;
  } else {
  }
  return 0;
}

static int is_vec_intrinsic(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "vec_new");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "vec_push");
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "vec_len");
  if (_sv0t2) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "vec_get");
  if (_sv0t3) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "vec_set");
  if (_sv0t4) {
    return 1;
  } else {
  }
  return 0;
}

static int is_box_intrinsic(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "box_new");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "box_deref");
  if (_sv0t1) {
    return 1;
  } else {
  }
  return 0;
}

static int register_one_intrinsic(int fn_arities, int mod_values, int name, int arity) {
  sv0_vec_push(fn_arities, name);
  sv0_vec_push(fn_arities, arity);
  sv0_vec_push(mod_values, name);
  return 0;
}

static int register_all_intrinsics(int fn_arities, int mod_values, int println_h, int old_h, int forall_h, int exists_h, int no_alias_h, int str_len_h, int str_eq_h, int str_concat_h, int str_char_at_h, int str_substr_h, int vec_new_h, int vec_push_h, int vec_len_h, int vec_get_h, int vec_set_h, int box_new_h, int box_deref_h) {
  int _sv0t0 = register_one_intrinsic(fn_arities, mod_values, println_h, 1);
  int _sv0t1 = register_one_intrinsic(fn_arities, mod_values, old_h, 1);
  int _sv0t2 = register_one_intrinsic(fn_arities, mod_values, forall_h, 3);
  int _sv0t3 = register_one_intrinsic(fn_arities, mod_values, exists_h, 3);
  int _sv0t4 = register_one_intrinsic(fn_arities, mod_values, no_alias_h, 2);
  int _sv0t5 = register_one_intrinsic(fn_arities, mod_values, str_len_h, 1);
  int _sv0t6 = register_one_intrinsic(fn_arities, mod_values, str_eq_h, 2);
  int _sv0t7 = register_one_intrinsic(fn_arities, mod_values, str_concat_h, 2);
  int _sv0t8 = register_one_intrinsic(fn_arities, mod_values, str_char_at_h, 2);
  int _sv0t9 = register_one_intrinsic(fn_arities, mod_values, str_substr_h, 3);
  int _sv0t10 = register_one_intrinsic(fn_arities, mod_values, vec_new_h, 0);
  int _sv0t11 = register_one_intrinsic(fn_arities, mod_values, vec_push_h, 2);
  int _sv0t12 = register_one_intrinsic(fn_arities, mod_values, vec_len_h, 1);
  int _sv0t13 = register_one_intrinsic(fn_arities, mod_values, vec_get_h, 2);
  int _sv0t14 = register_one_intrinsic(fn_arities, mod_values, vec_set_h, 3);
  int _sv0t15 = register_one_intrinsic(fn_arities, mod_values, box_new_h, 1);
  int _sv0t16 = register_one_intrinsic(fn_arities, mod_values, box_deref_h, 1);
  return 17;
}

static int item_registers_fn(int tag) {
  int _sv0t0 = (tag == 0);
  return _sv0t0;
}

static int item_registers_type(int tag) {
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

static int item_registers_variants(int tag) {
  int _sv0t0 = (tag == 2);
  return _sv0t0;
}

static int item_is_use(int tag) {
  int _sv0t0 = (tag == 5);
  return _sv0t0;
}

static int item_is_module(int tag) {
  int _sv0t0 = (tag == 6);
  return _sv0t0;
}

static int item_needs_registration(int tag) {
  int _sv0t0 = item_registers_fn(tag);
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = item_registers_type(tag);
  if (_sv0t1) {
    return 1;
  } else {
  }
  return 0;
}

static int enum_variant_arity(int variant_kind, int tuple_fields, int struct_fields) {
  if ((variant_kind == 0)) {
    return 0;
  } else {
  }
  if ((variant_kind == 1)) {
    return tuple_fields;
  } else {
  }
  if ((variant_kind == 2)) {
    return struct_fields;
  } else {
  }
  return 0;
}

static const char* enum_variant_qualified_path(const char* enum_name, const char* variant_name) {
  const char* _sv0t0 = path_join2(enum_name, variant_name);
  return _sv0t0;
}

static int variant_stem(int variant_kind, int name_handle) {
  return name_handle;
}

static const char* enum_variant_reg_path(const char* enum_name, const char* variant_name) {
  const char* _sv0t0 = enum_variant_qualified_path(enum_name, variant_name);
  return _sv0t0;
}

static int enum_variant_reg_arity(int variant_kind, int tuple_fields, int struct_fields) {
  int _sv0t0 = enum_variant_arity(variant_kind, tuple_fields, struct_fields);
  return _sv0t0;
}

static int pat_binds_count(int pat_tag, int child_count) {
  if ((pat_tag == 0)) {
    return 0;
  } else {
  }
  if ((pat_tag == 1)) {
    return 1;
  } else {
  }
  if ((pat_tag == 2)) {
    return 0;
  } else {
  }
  if ((pat_tag == 3)) {
    return child_count;
  } else {
  }
  if ((pat_tag == 4)) {
    return child_count;
  } else {
  }
  if ((pat_tag == 5)) {
    return child_count;
  } else {
  }
  if ((pat_tag == 6)) {
    return 255;
  } else {
  }
  return 0;
}

static int pat_is_or(int pat_tag) {
  int _sv0t0 = (pat_tag == 6);
  return _sv0t0;
}

static int pat_binds(int pat_tag, int name_handle, int out) {
  if ((pat_tag == 1)) {
    sv0_vec_push(out, name_handle);
    return 1;
  } else {
  }
  return 0;
}

static int bind_pattern_locals(int binds, int env_locals) {
  int _sv0t0 = sv0_vec_len(binds);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(binds, i);
    sv0_vec_push(env_locals, _sv0t1);
    i = (i + 1);
  }
  return n;
}

static int should_allow_self(int item_tag) {
  if ((item_tag == 3)) {
    return 1;
  } else {
  }
  if ((item_tag == 4)) {
    return 1;
  } else {
  }
  return 0;
}

static const char* error_code_unbound_ident(void) {
  return "E0300";
}

static const char* error_code_unknown_type(void) {
  return "E0301";
}

static const char* error_code_pat_or_unsupported(void) {
  return "E0306";
}

static const char* error_code_wrong_arity(void) {
  return "E0307";
}

static const char* error_code_use_clause(void) {
  return "E0309";
}

static int diag_pack(int code, int tok) {
  int t = tok;
  if ((t < 0)) {
    t = (0 - 1);
  } else {
  }
  if ((t >= 1000000)) {
    t = (0 - 1);
  } else {
  }
  int _sv0t0 = (code * 1000000);
  int _sv0t1 = (_sv0t0 + t);
  int _sv0t2 = (_sv0t1 + 1);
  return _sv0t2;
}

static int diag_code_of(int packed) {
  if ((packed < 1000)) {
    return packed;
  } else {
  }
  int _sv0t0 = (packed / 1000000);
  return _sv0t0;
}

static int diag_tok_of(int packed) {
  if ((packed < 1000)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = (packed / 1000000);
  int _sv0t2 = (_sv0t1 * 1000000);
  int _sv0t3 = (packed - _sv0t2);
  int _sv0t4 = (_sv0t3 - 1);
  return _sv0t4;
}

static int arity_matches(int expected, int actual) {
  int _sv0t0 = (expected == actual);
  return _sv0t0;
}

static int check_call_arity(int expected, int actual) {
  if ((expected == actual)) {
    return 0;
  } else {
  }
  if ((expected < 0)) {
    return 0;
  } else {
  }
  return 1;
}

static int register_item(int item_tag, int item_d1, int item_d2, int item_d3, int item_d4, int mod_vals, int mod_tys, int fn_arities) {
  if ((item_tag == 0)) {
    sv0_vec_push(mod_vals, item_d1);
    sv0_vec_push(fn_arities, item_d1);
    sv0_vec_push(fn_arities, item_d3);
    return 0;
  } else {
  }
  if ((item_tag == 1)) {
    sv0_vec_push(mod_vals, item_d1);
    sv0_vec_push(mod_tys, item_d1);
    return 0;
  } else {
  }
  if ((item_tag == 2)) {
    sv0_vec_push(mod_vals, item_d1);
    sv0_vec_push(mod_tys, item_d1);
    return 0;
  } else {
  }
  if ((item_tag == 3)) {
    sv0_vec_push(mod_tys, item_d1);
    return 0;
  } else {
  }
  if ((item_tag == 7)) {
    sv0_vec_push(mod_tys, item_d1);
    return 0;
  } else {
  }
  return 0;
}

static int register_items(int it, int id1, int id2, int id3, int id4, int mod_vals, int mod_tys, int fn_arities) {
  int _sv0t0 = sv0_vec_len(it);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(it, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(id1, i);
    int d1 = _sv0t2;
    int _sv0t3 = sv0_vec_get(id2, i);
    int d2 = _sv0t3;
    int _sv0t4 = sv0_vec_get(id3, i);
    int d3 = _sv0t4;
    int _sv0t5 = sv0_vec_get(id4, i);
    int d4 = _sv0t5;
    int _sv0t6 = register_item(tag, d1, d2, d3, d4, mod_vals, mod_tys, fn_arities);
    i = (i + 1);
  }
  return 0;
}

static const char* tok_str(const char* source, int starts, int ends, int pos) {
  int _sv0t0 = sv0_vec_get(starts, pos);
  int s = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, pos);
  int e = _sv0t1;
  int _sv0t2 = (e - s);
  const char* _sv0t3 = sv0_string_substr(source, s, _sv0t2);
  return _sv0t3;
}

static int res_is_prelude_type(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "i8");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "i16");
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "i32");
  if (_sv0t2) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "i64");
  if (_sv0t3) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "i128");
  if (_sv0t4) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "u8");
  if (_sv0t5) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "u16");
  if (_sv0t6) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "u32");
  if (_sv0t7) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "u64");
  if (_sv0t8) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "u128");
  if (_sv0t9) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "isize");
  if (_sv0t10) {
    return 1;
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "usize");
  if (_sv0t11) {
    return 1;
  } else {
  }
  int _sv0t12 = sv0_string_eq(name, "f32");
  if (_sv0t12) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_string_eq(name, "f64");
  if (_sv0t13) {
    return 1;
  } else {
  }
  int _sv0t14 = sv0_string_eq(name, "bool");
  if (_sv0t14) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_string_eq(name, "char");
  if (_sv0t15) {
    return 1;
  } else {
  }
  int _sv0t16 = sv0_string_eq(name, "str");
  if (_sv0t16) {
    return 1;
  } else {
  }
  int _sv0t17 = sv0_string_eq(name, "string");
  if (_sv0t17) {
    return 1;
  } else {
  }
  int _sv0t18 = sv0_string_eq(name, "String");
  if (_sv0t18) {
    return 1;
  } else {
  }
  int _sv0t19 = sv0_string_eq(name, "unit");
  if (_sv0t19) {
    return 1;
  } else {
  }
  int _sv0t20 = sv0_string_eq(name, "Vec");
  if (_sv0t20) {
    return 1;
  } else {
  }
  int _sv0t21 = sv0_string_eq(name, "Box");
  if (_sv0t21) {
    return 1;
  } else {
  }
  return 0;
}

static int res_value_exists(int mod_vals, const char* name_str, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(mod_vals);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(mod_vals, i);
    int pos = _sv0t1;
    const char* _sv0t2 = tok_str(source, starts, ends, pos);
    const char* nm;
    nm = _sv0t2;
    int _sv0t3 = sv0_string_eq(nm, name_str);
    if (_sv0t3) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int res_type_exists(int mod_tys, const char* name_str, const char* source, int starts, int ends) {
  int _sv0t0 = res_is_prelude_type(name_str);
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_vec_len(mod_tys);
  int len = _sv0t1;
  int i = 0;
  while ((i < len)) {
    int _sv0t2 = sv0_vec_get(mod_tys, i);
    int pos = _sv0t2;
    const char* _sv0t3 = tok_str(source, starts, ends, pos);
    const char* nm;
    nm = _sv0t3;
    int _sv0t4 = sv0_string_eq(nm, name_str);
    if (_sv0t4) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int resolve_pat_shape(int pt, int pd1, int pd2, int pd3, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int pp) {
  int _sv0t0 = sv0_vec_get(pt, idx);
  int tag = _sv0t0;
  if ((tag == 0)) {
    return 0;
  } else {
  }
  if ((tag == 1)) {
    return 0;
  } else {
  }
  if ((tag == 2)) {
    return 0;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t1 = sv0_vec_get(pd1, idx);
    int first = _sv0t1;
    int _sv0t2 = sv0_vec_get(pd2, idx);
    int count = _sv0t2;
    int i = 0;
    while ((i < count)) {
      int _sv0t3 = (first + i);
      int _sv0t4 = resolve_pat_shape(pt, pd1, pd2, pd3, _sv0t3, source, starts, ends, mod_vals, mod_tys, pp);
      int r = _sv0t4;
      if ((r != 0)) {
        return r;
      } else {
      }
      i = (i + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t5 = sv0_vec_get(pd1, idx);
    int pps = _sv0t5;
    int _sv0t6 = sv0_vec_get(pd2, idx);
    int ppc = _sv0t6;
    int _sv0t7 = sv0_vec_get(pd3, idx);
    int fc = _sv0t7;
    int _sv0t8 = sv0_vec_new();
    int path = _sv0t8;
    int k = 0;
    while ((k < ppc)) {
      int _sv0t9 = (pps + k);
      int _sv0t10 = sv0_vec_get(pp, _sv0t9);
      sv0_vec_push(path, _sv0t10);
      k = (k + 1);
    }
    const char* _sv0t11 = path_join_vec(source, starts, ends, path);
    const char* ps;
    ps = _sv0t11;
    if ((ppc >= 2)) {
      int _sv0t12 = sv0_vec_get(path, 0);
      const char* _sv0t13 = tok_str(source, starts, ends, _sv0t12);
      const char* seg0;
      seg0 = _sv0t13;
      int _sv0t14 = res_type_exists(mod_tys, seg0, source, starts, ends);
      if ((_sv0t14 != 1)) {
        return 301;
      } else {
      }
    } else {
      int _sv0t15 = res_type_exists(mod_tys, ps, source, starts, ends);
      if ((_sv0t15 != 1)) {
        return 301;
      } else {
      }
    }
    int fi = 0;
    while ((fi < fc)) {
      int _sv0t16 = (idx - fc);
      int child_idx = (_sv0t16 + fi);
      int _sv0t17 = resolve_pat_shape(pt, pd1, pd2, pd3, child_idx, source, starts, ends, mod_vals, mod_tys, pp);
      int r = _sv0t17;
      if ((r != 0)) {
        return r;
      } else {
      }
      fi = (fi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 5)) {
    int _sv0t18 = sv0_vec_get(pd1, idx);
    int pps = _sv0t18;
    int _sv0t19 = sv0_vec_get(pd2, idx);
    int ppc = _sv0t19;
    int _sv0t20 = sv0_vec_get(pd3, idx);
    int ac = _sv0t20;
    int _sv0t21 = sv0_vec_new();
    int path = _sv0t21;
    int k = 0;
    while ((k < ppc)) {
      int _sv0t22 = (pps + k);
      int _sv0t23 = sv0_vec_get(pp, _sv0t22);
      sv0_vec_push(path, _sv0t23);
      k = (k + 1);
    }
    const char* _sv0t24 = path_join_vec(source, starts, ends, path);
    const char* ps;
    ps = _sv0t24;
    int _sv0t25 = res_value_exists(mod_vals, ps, source, starts, ends);
    if ((_sv0t25 != 1)) {
      return 300;
    } else {
    }
    int ai = 0;
    while ((ai < ac)) {
      int _sv0t26 = (idx - ac);
      int child_idx = (_sv0t26 + ai);
      int _sv0t27 = resolve_pat_shape(pt, pd1, pd2, pd3, child_idx, source, starts, ends, mod_vals, mod_tys, pp);
      int r = _sv0t27;
      if ((r != 0)) {
        return r;
      } else {
      }
      ai = (ai + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 6)) {
    return 306;
  } else {
  }
  return 0;
}

static int resolve_ty(int tt, int td1, int td2, int td3, int idx, const char* source, int starts, int ends, int mod_tys, int mod_vals, int pp) {
  int _sv0t0 = sv0_vec_get(tt, idx);
  int tag = _sv0t0;
  if ((tag == 6)) {
    return 0;
  } else {
  }
  if ((tag == 0)) {
    int _sv0t1 = sv0_vec_get(td1, idx);
    int pps = _sv0t1;
    int _sv0t2 = sv0_vec_get(td2, idx);
    int ppc = _sv0t2;
    int _sv0t3 = sv0_vec_get(td3, idx);
    int tac = _sv0t3;
    int _sv0t4 = sv0_vec_new();
    int path = _sv0t4;
    int k = 0;
    while ((k < ppc)) {
      int _sv0t5 = (pps + k);
      int _sv0t6 = sv0_vec_get(pp, _sv0t5);
      sv0_vec_push(path, _sv0t6);
      k = (k + 1);
    }
    const char* _sv0t7 = path_join_vec(source, starts, ends, path);
    const char* ps;
    ps = _sv0t7;
    int _sv0t8 = res_type_exists(mod_tys, ps, source, starts, ends);
    if ((_sv0t8 != 1)) {
      int _sv0t9 = res_is_prelude_type(ps);
      if ((_sv0t9 != 1)) {
        int _sv0t10 = sv0_vec_get(pp, pps);
        int _sv0t11 = diag_pack(301, _sv0t10);
        return _sv0t11;
      } else {
      }
    } else {
    }
    int ti = 0;
    while ((ti < tac)) {
      int _sv0t12 = (idx - tac);
      int child_idx = (_sv0t12 + ti);
      int _sv0t13 = resolve_ty(tt, td1, td2, td3, child_idx, source, starts, ends, mod_tys, mod_vals, pp);
      int r = _sv0t13;
      if ((r != 0)) {
        return r;
      } else {
      }
      ti = (ti + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t14 = sv0_vec_get(td1, idx);
    int _sv0t15 = resolve_ty(tt, td1, td2, td3, _sv0t14, source, starts, ends, mod_tys, mod_vals, pp);
    return _sv0t15;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t16 = sv0_vec_get(td1, idx);
    int _sv0t17 = resolve_ty(tt, td1, td2, td3, _sv0t16, source, starts, ends, mod_tys, mod_vals, pp);
    return _sv0t17;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t18 = sv0_vec_get(td3, idx);
    if ((_sv0t18 == 1)) {
      int _sv0t19 = sv0_vec_get(td2, idx);
      int len_tok = _sv0t19;
      const char* _sv0t20 = tok_str(source, starts, ends, len_tok);
      const char* ln;
      ln = _sv0t20;
      int _sv0t21 = sv0_vec_new();
      int _sv0t22 = res_full_value_exists(mod_vals, _sv0t21, ln, source, starts, ends);
      if ((_sv0t22 != 1)) {
        return 300;
      } else {
      }
    } else {
    }
    int _sv0t23 = sv0_vec_get(td1, idx);
    int _sv0t24 = resolve_ty(tt, td1, td2, td3, _sv0t23, source, starts, ends, mod_tys, mod_vals, pp);
    return _sv0t24;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t25 = sv0_vec_get(td1, idx);
    int _sv0t26 = resolve_ty(tt, td1, td2, td3, _sv0t25, source, starts, ends, mod_tys, mod_vals, pp);
    return _sv0t26;
  } else {
  }
  if ((tag == 5)) {
    int _sv0t27 = sv0_vec_get(td1, idx);
    int first = _sv0t27;
    int _sv0t28 = sv0_vec_get(td2, idx);
    int count = _sv0t28;
    int i = 0;
    while ((i < count)) {
      int _sv0t29 = (first + i);
      int _sv0t30 = resolve_ty(tt, td1, td2, td3, _sv0t29, source, starts, ends, mod_tys, mod_vals, pp);
      int r = _sv0t30;
      if ((r != 0)) {
        return r;
      } else {
      }
      i = (i + 1);
    }
    return 0;
  } else {
  }
  return 0;
}

static int res_enter_scope(int frames) {
  int _sv0t0 = sv0_vec_new();
  int f = _sv0t0;
  sv0_vec_push(frames, f);
  return 0;
}

static int res_bind_local(int frames, int name_pos) {
  int _sv0t0 = sv0_vec_len(frames);
  int len = _sv0t0;
  if ((len == 0)) {
    int _sv0t1 = sv0_vec_new();
    int f = _sv0t1;
    sv0_vec_push(f, name_pos);
    sv0_vec_push(frames, f);
    return 0;
  } else {
  }
  int _sv0t2 = (len - 1);
  int _sv0t3 = sv0_vec_get(frames, _sv0t2);
  int top = _sv0t3;
  sv0_vec_push(top, name_pos);
  return 0;
}

static int res_lookup_in_frames_str(int frames, const char* name_str, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(frames);
  int flen = _sv0t0;
  int fi = (flen - 1);
  while ((fi >= 0)) {
    int _sv0t1 = sv0_vec_get(frames, fi);
    int frame = _sv0t1;
    int _sv0t2 = sv0_vec_len(frame);
    int fv_len = _sv0t2;
    int j = 0;
    while ((j < fv_len)) {
      int _sv0t3 = sv0_vec_get(frame, j);
      int pos = _sv0t3;
      const char* _sv0t4 = tok_str(source, starts, ends, pos);
      const char* nm;
      nm = _sv0t4;
      int _sv0t5 = sv0_string_eq(nm, name_str);
      if (_sv0t5) {
        return 1;
      } else {
      }
      j = (j + 1);
    }
    fi = (fi - 1);
  }
  return 0;
}

static int res_full_value_exists(int mod_vals, int frames, const char* name_str, const char* source, int starts, int ends) {
  int _sv0t0 = res_lookup_in_frames_str(frames, name_str, source, starts, ends);
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = res_value_exists(mod_vals, name_str, source, starts, ends);
  return _sv0t1;
}

static int res_lookup_fn_arity_str(int fn_arities, const char* name_str, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(fn_arities);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(fn_arities, i);
    int pos = _sv0t1;
    const char* _sv0t2 = tok_str(source, starts, ends, pos);
    const char* nm;
    nm = _sv0t2;
    int _sv0t3 = sv0_string_eq(nm, name_str);
    if (_sv0t3) {
      int _sv0t4 = (i + 1);
      int _sv0t5 = sv0_vec_get(fn_arities, _sv0t4);
      return _sv0t5;
    } else {
    }
    i = (i + 2);
  }
  int _sv0t6 = (0 - 1);
  return _sv0t6;
}

static int block_stmt_index(int pp, int stmts_first, int sidecar, int si) {
  if ((sidecar > 0)) {
    int _sv0t0 = (sidecar - 1);
    int _sv0t1 = (_sv0t0 + si);
    int _sv0t2 = sv0_vec_get(pp, _sv0t1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = (stmts_first + si);
  return _sv0t3;
}

static int resolve_expr(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int frames, int pp) {
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
    int _sv0t1 = sv0_vec_get(ed1, idx);
    int pps = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed2, idx);
    int ppc = _sv0t2;
    int _sv0t3 = sv0_vec_new();
    int path = _sv0t3;
    int k = 0;
    while ((k < ppc)) {
      int _sv0t4 = (pps + k);
      int _sv0t5 = sv0_vec_get(pp, _sv0t4);
      sv0_vec_push(path, _sv0t5);
      k = (k + 1);
    }
    const char* _sv0t6 = path_join_vec(source, starts, ends, path);
    const char* ps;
    ps = _sv0t6;
    int _sv0t7 = res_full_value_exists(mod_vals, frames, ps, source, starts, ends);
    if (_sv0t7) {
      return 0;
    } else {
    }
    if ((ppc == 1)) {
      int _sv0t8 = is_intrinsic(ps);
      if (_sv0t8) {
        return 0;
      } else {
      }
      int _sv0t9 = sv0_string_eq(ps, "result");
      if (_sv0t9) {
        return 0;
      } else {
      }
    } else {
    }
    if ((ppc == 2)) {
      int _sv0t10 = sv0_vec_get(pp, pps);
      const char* _sv0t11 = tok_str(source, starts, ends, _sv0t10);
      const char* seg0;
      seg0 = _sv0t11;
      int _sv0t12 = res_type_exists(mod_tys, seg0, source, starts, ends);
      if (_sv0t12) {
        return 0;
      } else {
      }
    } else {
    }
    int _sv0t13 = sv0_vec_get(pp, pps);
    int _sv0t14 = diag_pack(300, _sv0t13);
    return _sv0t14;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t15 = sv0_vec_get(ed2, idx);
    int _sv0t16 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t15, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t16;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t17 = sv0_vec_get(ed2, idx);
    int _sv0t18 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t17, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int r1 = _sv0t18;
    if ((r1 != 0)) {
      return r1;
    } else {
    }
    int _sv0t19 = sv0_vec_get(ed3, idx);
    int _sv0t20 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t19, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t20;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t21 = sv0_vec_get(ed1, idx);
    int callee = _sv0t21;
    int _sv0t22 = sv0_vec_get(ed2, idx);
    int args_first = _sv0t22;
    int _sv0t23 = sv0_vec_get(ed3, idx);
    int args_count = _sv0t23;
    int _sv0t24 = sv0_vec_get(ed4, idx);
    int args_sidecar = _sv0t24;
    int _sv0t25 = sv0_vec_get(et, callee);
    if ((_sv0t25 == 1)) {
      int _sv0t26 = sv0_vec_get(ed1, callee);
      int cpps = _sv0t26;
      int _sv0t27 = sv0_vec_get(ed2, callee);
      int cppc = _sv0t27;
      int _sv0t28 = sv0_vec_new();
      int cpath = _sv0t28;
      int ck = 0;
      while ((ck < cppc)) {
        int _sv0t29 = (cpps + ck);
        int _sv0t30 = sv0_vec_get(pp, _sv0t29);
        sv0_vec_push(cpath, _sv0t30);
        ck = (ck + 1);
      }
      const char* _sv0t31 = path_join_vec(source, starts, ends, cpath);
      const char* cps;
      cps = _sv0t31;
      if ((cppc == 1)) {
        int _sv0t32 = sv0_string_eq(cps, "forall");
        if (_sv0t32) {
          if ((args_count == 3)) {
            int binder = args_first;
            int _sv0t33 = sv0_vec_get(et, binder);
            if ((_sv0t33 == 1)) {
              int _sv0t34 = sv0_vec_get(ed1, binder);
              int _sv0t35 = sv0_vec_get(pp, _sv0t34);
              int bpos = _sv0t35;
              int _sv0t36 = (args_first + 1);
              int _sv0t37 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t36, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
              int rd = _sv0t37;
              if ((rd != 0)) {
                return rd;
              } else {
              }
              int _sv0t38 = res_enter_scope(frames);
              int _sv0t39 = res_bind_local(frames, bpos);
              int _sv0t40 = (args_first + 2);
              int _sv0t41 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t40, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
              int rb = _sv0t41;
              return rb;
            } else {
            }
          } else {
          }
        } else {
        }
        int _sv0t42 = sv0_string_eq(cps, "exists");
        if (_sv0t42) {
          if ((args_count == 3)) {
            int binder = args_first;
            int _sv0t43 = sv0_vec_get(et, binder);
            if ((_sv0t43 == 1)) {
              int _sv0t44 = sv0_vec_get(ed1, binder);
              int _sv0t45 = sv0_vec_get(pp, _sv0t44);
              int bpos = _sv0t45;
              int _sv0t46 = (args_first + 1);
              int _sv0t47 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t46, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
              int rd = _sv0t47;
              if ((rd != 0)) {
                return rd;
              } else {
              }
              int _sv0t48 = res_enter_scope(frames);
              int _sv0t49 = res_bind_local(frames, bpos);
              int _sv0t50 = (args_first + 2);
              int _sv0t51 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t50, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
              int rb = _sv0t51;
              return rb;
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
      int _sv0t52 = res_lookup_fn_arity_str(fn_arities, cps, source, starts, ends);
      int arity = _sv0t52;
      if ((arity >= 0)) {
        if ((arity != args_count)) {
          int _sv0t53 = sv0_vec_get(pp, cpps);
          int _sv0t54 = diag_pack(307, _sv0t53);
          return _sv0t54;
        } else {
        }
      } else {
      }
    } else {
    }
    int _sv0t55 = resolve_expr(et, ed1, ed2, ed3, ed4, callee, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int rf = _sv0t55;
    if ((rf != 0)) {
      return rf;
    } else {
    }
    int ai = 0;
    while ((ai < args_count)) {
      int _sv0t56 = block_stmt_index(pp, args_first, args_sidecar, ai);
      int _sv0t57 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t56, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int ra = _sv0t57;
      if ((ra != 0)) {
        return ra;
      } else {
      }
      ai = (ai + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 5)) {
    int _sv0t58 = sv0_vec_get(ed1, idx);
    int recv = _sv0t58;
    int _sv0t59 = sv0_vec_get(ed3, idx);
    int margs_count = _sv0t59;
    int _sv0t60 = sv0_vec_get(ed4, idx);
    int margs_sidecar = _sv0t60;
    int _sv0t61 = resolve_expr(et, ed1, ed2, ed3, ed4, recv, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int rr = _sv0t61;
    if ((rr != 0)) {
      return rr;
    } else {
    }
    int mi = 0;
    while ((mi < margs_count)) {
      int _sv0t62 = block_stmt_index(pp, 0, margs_sidecar, mi);
      int _sv0t63 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t62, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int rm = _sv0t63;
      if ((rm != 0)) {
        return rm;
      } else {
      }
      mi = (mi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t64 = sv0_vec_get(ed1, idx);
    int _sv0t65 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t64, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t65;
  } else {
  }
  if ((tag == 7)) {
    int _sv0t66 = sv0_vec_get(ed1, idx);
    int _sv0t67 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t66, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t67;
  } else {
  }
  if ((tag == 8)) {
    int _sv0t68 = sv0_vec_get(ed1, idx);
    int _sv0t69 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t68, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int ra = _sv0t69;
    if ((ra != 0)) {
      return ra;
    } else {
    }
    int _sv0t70 = sv0_vec_get(ed2, idx);
    int _sv0t71 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t70, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t71;
  } else {
  }
  if ((tag == 9)) {
    int _sv0t72 = sv0_vec_get(ed1, idx);
    int sf = _sv0t72;
    int _sv0t73 = sv0_vec_get(ed2, idx);
    int sc = _sv0t73;
    int _sv0t74 = sv0_vec_get(ed3, idx);
    int tail = _sv0t74;
    int _sv0t75 = sv0_vec_get(ed4, idx);
    int sidecar = _sv0t75;
    int _sv0t76 = res_enter_scope(frames);
    int si = 0;
    while ((si < sc)) {
      int _sv0t77 = block_stmt_index(pp, sf, sidecar, si);
      int _sv0t78 = resolve_stmt(et, ed1, ed2, ed3, ed4, _sv0t77, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int rs = _sv0t78;
      if ((rs != 0)) {
        return rs;
      } else {
      }
      si = (si + 1);
    }
    if ((tail >= 0)) {
      int _sv0t79 = resolve_expr(et, ed1, ed2, ed3, ed4, tail, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      return _sv0t79;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t80 = sv0_vec_get(ed1, idx);
    int _sv0t81 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t80, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int rc = _sv0t81;
    if ((rc != 0)) {
      return rc;
    } else {
    }
    int _sv0t82 = sv0_vec_get(ed2, idx);
    int _sv0t83 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t82, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int rt = _sv0t83;
    if ((rt != 0)) {
      return rt;
    } else {
    }
    int _sv0t84 = sv0_vec_get(ed3, idx);
    int else_idx = _sv0t84;
    if ((else_idx >= 0)) {
      int _sv0t85 = resolve_expr(et, ed1, ed2, ed3, ed4, else_idx, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      return _sv0t85;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t86 = sv0_vec_get(ed1, idx);
    int scrut = _sv0t86;
    int _sv0t87 = sv0_vec_get(ed2, idx);
    int arms_f = _sv0t87;
    int _sv0t88 = sv0_vec_get(ed3, idx);
    int arms_c = _sv0t88;
    int _sv0t89 = sv0_vec_get(ed4, idx);
    int arms_sidecar = _sv0t89;
    int _sv0t90 = resolve_expr(et, ed1, ed2, ed3, ed4, scrut, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int rsc = _sv0t90;
    if ((rsc != 0)) {
      return rsc;
    } else {
    }
    int ami = 0;
    while ((ami < arms_c)) {
      int _sv0t91 = block_stmt_index(pp, arms_f, arms_sidecar, ami);
      int arm_idx = _sv0t91;
      int _sv0t92 = sv0_vec_get(ed1, arm_idx);
      int arm_pat_tag = _sv0t92;
      int _sv0t93 = sv0_vec_get(ed2, arm_idx);
      int arm_pat_d1 = _sv0t93;
      int _sv0t94 = sv0_vec_get(ed3, arm_idx);
      int guard = _sv0t94;
      int _sv0t95 = sv0_vec_get(ed4, arm_idx);
      int body = _sv0t95;
      int _sv0t96 = res_enter_scope(frames);
      if ((arm_pat_tag == 1)) {
        int _sv0t97 = res_bind_local(frames, arm_pat_d1);
      } else {
      }
      if ((arm_pat_tag == 5)) {
        int _sv0t98 = sv0_vec_get(ed3, arm_pat_d1);
        int argc_r = _sv0t98;
        if ((argc_r > 0)) {
          int meta_r = (arm_pat_d1 - 1);
          int _sv0t99 = sv0_vec_get(et, meta_r);
          if ((_sv0t99 == 33)) {
            int _sv0t100 = sv0_vec_get(ed1, meta_r);
            int ac_r = _sv0t100;
            int _sv0t101 = sv0_vec_get(ed2, meta_r);
            int first_r = _sv0t101;
            int ej_r = 0;
            while ((ej_r < ac_r)) {
              int eidx_r = (first_r + ej_r);
              int _sv0t102 = sv0_vec_get(et, eidx_r);
              if ((_sv0t102 == 31)) {
                int _sv0t103 = sv0_vec_get(ed4, eidx_r);
                if ((_sv0t103 == 1)) {
                  int _sv0t104 = sv0_vec_get(ed1, eidx_r);
                  int _sv0t105 = res_bind_local(frames, _sv0t104);
                } else {
                }
              } else {
              }
              ej_r = (ej_r + 1);
            }
          } else {
          }
        } else {
        }
      } else {
      }
      if ((arm_pat_tag == 4)) {
        int _sv0t106 = sv0_vec_get(ed3, arm_pat_d1);
        int fc_r = _sv0t106;
        if ((fc_r > 0)) {
          int meta_s = (arm_pat_d1 - 1);
          int _sv0t107 = sv0_vec_get(et, meta_s);
          if ((_sv0t107 == 32)) {
            int _sv0t108 = sv0_vec_get(ed1, meta_s);
            int mc_s = _sv0t108;
            int _sv0t109 = sv0_vec_get(ed2, meta_s);
            int first_s = _sv0t109;
            int ej_s = 0;
            while ((ej_s < mc_s)) {
              int eidx_s = (first_s + ej_s);
              int _sv0t110 = sv0_vec_get(et, eidx_s);
              if ((_sv0t110 == 31)) {
                int _sv0t111 = sv0_vec_get(ed4, eidx_s);
                if ((_sv0t111 == 1)) {
                  int _sv0t112 = sv0_vec_get(ed1, eidx_s);
                  int _sv0t113 = res_bind_local(frames, _sv0t112);
                } else {
                }
              } else {
              }
              ej_s = (ej_s + 1);
            }
          } else {
          }
        } else {
        }
      } else {
      }
      if ((guard >= 0)) {
        int _sv0t114 = resolve_expr(et, ed1, ed2, ed3, ed4, guard, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
        int rg = _sv0t114;
        if ((rg != 0)) {
          return rg;
        } else {
        }
      } else {
      }
      int _sv0t115 = resolve_expr(et, ed1, ed2, ed3, ed4, body, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int rb = _sv0t115;
      if ((rb != 0)) {
        return rb;
      } else {
      }
      ami = (ami + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t116 = sv0_vec_get(ed1, idx);
    int cond = _sv0t116;
    int _sv0t117 = sv0_vec_get(ed2, idx);
    int wbody = _sv0t117;
    int _sv0t118 = sv0_vec_get(ed3, idx);
    int inv_f = _sv0t118;
    int _sv0t119 = sv0_vec_get(ed4, idx);
    int inv_c = _sv0t119;
    int _sv0t120 = resolve_expr(et, ed1, ed2, ed3, ed4, cond, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int rwc = _sv0t120;
    if ((rwc != 0)) {
      return rwc;
    } else {
    }
    int ii = 0;
    while ((ii < inv_c)) {
      int _sv0t121 = (inv_f + ii);
      int _sv0t122 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t121, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int ri = _sv0t122;
      if ((ri != 0)) {
        return ri;
      } else {
      }
      ii = (ii + 1);
    }
    int _sv0t123 = resolve_expr(et, ed1, ed2, ed3, ed4, wbody, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t123;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t124 = sv0_vec_get(ed1, idx);
    int pat_tok = _sv0t124;
    int _sv0t125 = sv0_vec_get(ed2, idx);
    int iter = _sv0t125;
    int _sv0t126 = sv0_vec_get(ed3, idx);
    int fbody = _sv0t126;
    int _sv0t127 = resolve_expr(et, ed1, ed2, ed3, ed4, iter, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int rit = _sv0t127;
    if ((rit != 0)) {
      return rit;
    } else {
    }
    int _sv0t128 = res_enter_scope(frames);
    int _sv0t129 = res_bind_local(frames, pat_tok);
    int _sv0t130 = resolve_expr(et, ed1, ed2, ed3, ed4, fbody, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t130;
  } else {
  }
  if ((tag == 14)) {
    int _sv0t131 = sv0_vec_get(ed1, idx);
    int _sv0t132 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t131, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t132;
  } else {
  }
  if ((tag == 15)) {
    int _sv0t133 = sv0_vec_get(ed1, idx);
    int rv = _sv0t133;
    if ((rv >= 0)) {
      int _sv0t134 = resolve_expr(et, ed1, ed2, ed3, ed4, rv, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      return _sv0t134;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 16)) {
    int _sv0t135 = sv0_vec_get(ed1, idx);
    int bv = _sv0t135;
    if ((bv >= 0)) {
      int _sv0t136 = resolve_expr(et, ed1, ed2, ed3, ed4, bv, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      return _sv0t136;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 17)) {
    return 0;
  } else {
  }
  if ((tag == 18)) {
    int _sv0t137 = sv0_vec_get(ed1, idx);
    int _sv0t138 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t137, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int ra = _sv0t138;
    if ((ra != 0)) {
      return ra;
    } else {
    }
    int _sv0t139 = sv0_vec_get(ed2, idx);
    int _sv0t140 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t139, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t140;
  } else {
  }
  if ((tag == 19)) {
    int _sv0t141 = sv0_vec_get(ed2, idx);
    int _sv0t142 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t141, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    int ra = _sv0t142;
    if ((ra != 0)) {
      return ra;
    } else {
    }
    int _sv0t143 = sv0_vec_get(ed3, idx);
    int _sv0t144 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t143, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t144;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t145 = sv0_vec_get(ed1, idx);
    int _sv0t146 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t145, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t146;
  } else {
  }
  if ((tag == 21)) {
    int _sv0t147 = sv0_vec_get(ed1, idx);
    int lo = _sv0t147;
    int _sv0t148 = sv0_vec_get(ed2, idx);
    int hi = _sv0t148;
    if ((lo >= 0)) {
      int _sv0t149 = resolve_expr(et, ed1, ed2, ed3, ed4, lo, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int rl = _sv0t149;
      if ((rl != 0)) {
        return rl;
      } else {
      }
    } else {
    }
    if ((hi >= 0)) {
      int _sv0t150 = resolve_expr(et, ed1, ed2, ed3, ed4, hi, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      return _sv0t150;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 22)) {
    int _sv0t151 = sv0_vec_get(ed1, idx);
    int _sv0t152 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t151, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t152;
  } else {
  }
  if ((tag == 23)) {
    int _sv0t153 = sv0_vec_get(ed1, idx);
    int _sv0t154 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t153, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t154;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t155 = sv0_vec_get(ed1, idx);
    int spps = _sv0t155;
    int _sv0t156 = sv0_vec_get(ed2, idx);
    int sppc = _sv0t156;
    int _sv0t157 = sv0_vec_get(ed3, idx);
    int sfc = _sv0t157;
    int _sv0t158 = sv0_vec_new();
    int spath = _sv0t158;
    int sk = 0;
    while ((sk < sppc)) {
      int _sv0t159 = (spps + sk);
      int _sv0t160 = sv0_vec_get(pp, _sv0t159);
      sv0_vec_push(spath, _sv0t160);
      sk = (sk + 1);
    }
    const char* _sv0t161 = path_join_vec(source, starts, ends, spath);
    const char* sps;
    sps = _sv0t161;
    int _sv0t162 = res_type_exists(mod_tys, sps, source, starts, ends);
    if ((_sv0t162 != 1)) {
      return 301;
    } else {
    }
    int sfi = 0;
    while ((sfi < sfc)) {
      int _sv0t163 = (idx - sfc);
      int _sv0t164 = (_sv0t163 + sfi);
      int _sv0t165 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t164, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int rfe = _sv0t165;
      if ((rfe != 0)) {
        return rfe;
      } else {
      }
      sfi = (sfi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t166 = sv0_vec_get(ed1, idx);
    int tf = _sv0t166;
    int _sv0t167 = sv0_vec_get(ed2, idx);
    int tc = _sv0t167;
    int ti = 0;
    while ((ti < tc)) {
      int _sv0t168 = (tf + ti);
      int _sv0t169 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t168, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int rte = _sv0t169;
      if ((rte != 0)) {
        return rte;
      } else {
      }
      ti = (ti + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 26)) {
    int _sv0t170 = sv0_vec_get(ed1, idx);
    int af = _sv0t170;
    int _sv0t171 = sv0_vec_get(ed2, idx);
    int ac = _sv0t171;
    int ari = 0;
    while ((ari < ac)) {
      int _sv0t172 = (af + ari);
      int _sv0t173 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t172, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int rae = _sv0t173;
      if ((rae != 0)) {
        return rae;
      } else {
      }
      ari = (ari + 1);
    }
    return 0;
  } else {
  }
  return 0;
}

static int resolve_stmt(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int frames, int pp) {
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 27)) {
    int _sv0t1 = sv0_vec_get(ed3, idx);
    int init = _sv0t1;
    if ((init >= 0)) {
      int _sv0t2 = resolve_expr(et, ed1, ed2, ed3, ed4, init, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int ri = _sv0t2;
      if ((ri != 0)) {
        return ri;
      } else {
      }
    } else {
    }
    int _sv0t3 = sv0_vec_get(ed1, idx);
    int name_pos = _sv0t3;
    int _sv0t4 = res_bind_local(frames, name_pos);
    return 0;
  } else {
  }
  if ((tag == 28)) {
    int _sv0t5 = sv0_vec_get(ed1, idx);
    int _sv0t6 = resolve_expr(et, ed1, ed2, ed3, ed4, _sv0t5, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = resolve_expr(et, ed1, ed2, ed3, ed4, idx, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
  return _sv0t7;
}

static int resolve_contract(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int frames, int pp) {
  int _sv0t0 = resolve_expr(et, ed1, ed2, ed3, ed4, idx, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
  return _sv0t0;
}

static int resolve_expr_cast_roots_program(int et, int ed2, int pty_tt, int pty_td1, int pty_td2, int pty_td3, const char* source, int starts, int ends, int mod_tys, int mod_vals, int pp) {
  int i = 0;
  int _sv0t0 = sv0_vec_len(et);
  int n = _sv0t0;
  int _sv0t1 = sv0_vec_len(pty_tt);
  int tn = _sv0t1;
  while ((i < n)) {
    int _sv0t2 = sv0_vec_get(et, i);
    if ((_sv0t2 == 20)) {
      int _sv0t3 = sv0_vec_get(ed2, i);
      int tr = _sv0t3;
      if ((tr >= 0)) {
        if ((tr < tn)) {
          int _sv0t4 = resolve_ty(pty_tt, pty_td1, pty_td2, pty_td3, tr, source, starts, ends, mod_tys, mod_vals, pp);
          int rr = _sv0t4;
          if ((rr != 0)) {
            return rr;
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int apply_use_clause(int it, int id1, int id2, int idx, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int pp) {
  int _sv0t0 = sv0_vec_get(it, idx);
  int tag = _sv0t0;
  if ((tag != 5)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(id1, idx);
  int pps = _sv0t1;
  int _sv0t2 = sv0_vec_get(id2, idx);
  int ppc = _sv0t2;
  if ((ppc != 2)) {
    return 309;
  } else {
  }
  int _sv0t3 = sv0_vec_get(pp, pps);
  int m_tok = _sv0t3;
  int _sv0t4 = (pps + 1);
  int _sv0t5 = sv0_vec_get(pp, _sv0t4);
  int nm_tok = _sv0t5;
  const char* _sv0t6 = tok_str(source, starts, ends, m_tok);
  const char* m_str;
  m_str = _sv0t6;
  const char* _sv0t7 = tok_str(source, starts, ends, nm_tok);
  const char* nm_str;
  nm_str = _sv0t7;
  const char* _sv0t8 = mangle_use_target(m_str, nm_str);
  const char* tgt;
  tgt = _sv0t8;
  int _sv0t9 = res_lookup_fn_arity_str(fn_arities, tgt, source, starts, ends);
  int arity = _sv0t9;
  if ((arity >= 0)) {
    sv0_vec_push(mod_vals, nm_tok);
    sv0_vec_push(fn_arities, nm_tok);
    sv0_vec_push(fn_arities, arity);
    return 0;
  } else {
  }
  int _sv0t10 = res_type_exists(mod_tys, tgt, source, starts, ends);
  if (_sv0t10) {
    sv0_vec_push(mod_tys, nm_tok);
    return 0;
  } else {
  }
  int _sv0t11 = res_lookup_fn_arity_str(fn_arities, nm_str, source, starts, ends);
  int uarity = _sv0t11;
  if ((uarity >= 0)) {
    sv0_vec_push(mod_vals, nm_tok);
    sv0_vec_push(fn_arities, nm_tok);
    sv0_vec_push(fn_arities, uarity);
    return 0;
  } else {
  }
  int _sv0t12 = res_type_exists(mod_tys, nm_str, source, starts, ends);
  if (_sv0t12) {
    sv0_vec_push(mod_tys, nm_tok);
    return 0;
  } else {
  }
  return 309;
}

static int apply_use_clauses(int it, int id1, int id2, int id3, int id4, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int pp) {
  int _sv0t0 = sv0_vec_len(it);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(it, i);
    if ((_sv0t1 == 5)) {
      int _sv0t2 = apply_use_clause(it, id1, id2, i, source, starts, ends, mod_vals, mod_tys, fn_arities, pp);
      int r = _sv0t2;
      if ((r != 0)) {
        return r;
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int resolve_top_item(int it, int id1, int id2, int id3, int id4, int id5, int idx, int et, int ed1, int ed2, int ed3, int ed4, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int pp) {
  int _sv0t0 = sv0_vec_get(it, idx);
  int tag = _sv0t0;
  if ((tag == 5)) {
    return 0;
  } else {
  }
  if ((tag == 6)) {
    return 0;
  } else {
  }
  if ((tag == 0)) {
    int _sv0t1 = sv0_vec_get(id4, idx);
    int body_root = _sv0t1;
    int _sv0t2 = sv0_vec_get(id3, idx);
    int param_count = _sv0t2;
    int _sv0t3 = sv0_vec_get(id5, idx);
    int pbase = _sv0t3;
    int _sv0t4 = sv0_vec_new();
    int frames = _sv0t4;
    int pti = 0;
    while ((pti < param_count)) {
      int _sv0t5 = (pbase + pti);
      int _sv0t6 = sv0_vec_get(fn_param_ty_root, _sv0t5);
      int tr = _sv0t6;
      int _sv0t7 = resolve_ty(pty_tt, pty_td1, pty_td2, pty_td3, tr, source, starts, ends, mod_tys, mod_vals, pp);
      int rty = _sv0t7;
      if ((rty != 0)) {
        return rty;
      } else {
      }
      pti = (pti + 1);
    }
    int _sv0t8 = sv0_vec_get(fn_ret_ty_root_by_item, idx);
    int ret_root = _sv0t8;
    if ((ret_root >= 0)) {
      int _sv0t9 = resolve_ty(pty_tt, pty_td1, pty_td2, pty_td3, ret_root, source, starts, ends, mod_tys, mod_vals, pp);
      int rr = _sv0t9;
      if ((rr != 0)) {
        return rr;
      } else {
      }
    } else {
    }
    int _sv0t10 = res_enter_scope(frames);
    int pi = 0;
    while ((pi < param_count)) {
      int _sv0t11 = (pbase + pi);
      int _sv0t12 = sv0_vec_get(fn_param_name_toks, _sv0t11);
      int tok = _sv0t12;
      if ((tok >= 0)) {
        int _sv0t13 = res_bind_local(frames, tok);
      } else {
      }
      pi = (pi + 1);
    }
    int _sv0t14 = sv0_vec_get(fn_contract_base_by_item, idx);
    int cbase = _sv0t14;
    int _sv0t15 = sv0_vec_get(id2, idx);
    int ccount = (_sv0t15 / 2);
    int ci = 0;
    while ((ci < ccount)) {
      int _sv0t16 = (cbase + ci);
      int _sv0t17 = sv0_vec_get(fn_contract_root, _sv0t16);
      int cr = _sv0t17;
      int _sv0t18 = resolve_contract(et, ed1, ed2, ed3, ed4, cr, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
      int rc = _sv0t18;
      if ((rc != 0)) {
        return rc;
      } else {
      }
      ci = (ci + 1);
    }
    int _sv0t19 = resolve_expr(et, ed1, ed2, ed3, ed4, body_root, source, starts, ends, mod_vals, mod_tys, fn_arities, frames, pp);
    return _sv0t19;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t20 = sv0_vec_get(id2, idx);
    int fc = _sv0t20;
    int _sv0t21 = sv0_vec_get(id3, idx);
    int sfb = _sv0t21;
    int fi = 0;
    while ((fi < fc)) {
      int _sv0t22 = (sfb + fi);
      int _sv0t23 = sv0_vec_get(struct_field_ty_root, _sv0t22);
      int tr = _sv0t23;
      int _sv0t24 = resolve_ty(pty_tt, pty_td1, pty_td2, pty_td3, tr, source, starts, ends, mod_tys, mod_vals, pp);
      int rr = _sv0t24;
      if ((rr != 0)) {
        return rr;
      } else {
      }
      fi = (fi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t25 = sv0_vec_get(enum_variant_payload_base_by_item, idx);
    int base = _sv0t25;
    int _sv0t26 = sv0_vec_get(enum_variant_payload_count_by_item, idx);
    int cnt = _sv0t26;
    int vi = 0;
    while ((vi < cnt)) {
      int _sv0t27 = (base + vi);
      int _sv0t28 = sv0_vec_get(enum_variant_payload_ty_root, _sv0t27);
      int tr = _sv0t28;
      int _sv0t29 = resolve_ty(pty_tt, pty_td1, pty_td2, pty_td3, tr, source, starts, ends, mod_tys, mod_vals, pp);
      int rr = _sv0t29;
      if ((rr != 0)) {
        return rr;
      } else {
      }
      vi = (vi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 7)) {
    int _sv0t30 = sv0_vec_get(id2, idx);
    int alias_root = _sv0t30;
    int _sv0t31 = resolve_ty(pty_tt, pty_td1, pty_td2, pty_td3, alias_root, source, starts, ends, mod_tys, mod_vals, pp);
    return _sv0t31;
  } else {
  }
  return 0;
}

static int resolve_program(int et, int ed1, int ed2, int ed3, int ed4, int it, int id1, int id2, int id3, int id4, int id5, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root, const char* source, int starts, int ends, int mod_vals, int mod_tys, int fn_arities, int pp) {
  int _sv0t0 = register_items(it, id1, id2, id3, id4, mod_vals, mod_tys, fn_arities);
  int _sv0t1 = apply_use_clauses(it, id1, id2, id3, id4, source, starts, ends, mod_vals, mod_tys, fn_arities, pp);
  int ru = _sv0t1;
  if ((ru != 0)) {
    return ru;
  } else {
  }
  int _sv0t2 = resolve_expr_cast_roots_program(et, ed2, pty_tt, pty_td1, pty_td2, pty_td3, source, starts, ends, mod_tys, mod_vals, pp);
  int rcast = _sv0t2;
  if ((rcast != 0)) {
    return rcast;
  } else {
  }
  int _sv0t3 = sv0_vec_len(it);
  int len = _sv0t3;
  int i = 0;
  while ((i < len)) {
    int _sv0t4 = resolve_top_item(it, id1, id2, id3, id4, id5, i, et, ed1, ed2, ed3, ed4, fn_param_name_toks, fn_param_ty_root, fn_ret_ty_root_by_item, fn_contract_base_by_item, fn_contract_root, enum_variant_payload_ty_root, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3, struct_field_ty_root, source, starts, ends, mod_vals, mod_tys, fn_arities, pp);
    int r = _sv0t4;
    if ((r != 0)) {
      return r;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int test_path_join(void) {
  const char* _sv0t0 = path_join2("Foo", "Bar");
  int _sv0t1 = sv0_string_eq(_sv0t0, "Foo::Bar");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = path_join2("mod", "fn");
  int _sv0t3 = sv0_string_eq(_sv0t2, "mod::fn");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_mangle_use(void) {
  const char* _sv0t0 = mangle_use_target("mymod", "foo");
  int _sv0t1 = sv0_string_eq(_sv0t0, "mymod__foo");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = mangle_use_target("a", "b");
  int _sv0t3 = sv0_string_eq(_sv0t2, "a__b");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_is_intrinsic(void) {
  int _sv0t0 = is_intrinsic("println");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_intrinsic("vec_new");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_intrinsic("box_deref");
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_intrinsic("foo");
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_intrinsic("main");
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_intrinsic_arity(void) {
  int _sv0t0 = intrinsic_arity("println");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = intrinsic_arity("forall");
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = intrinsic_arity("vec_new");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = intrinsic_arity("string_substr");
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = intrinsic_arity("unknown");
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_intrinsic_classify(void) {
  int _sv0t0 = is_string_intrinsic("string_len");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_string_intrinsic("string_substr");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_string_intrinsic("println");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_vec_intrinsic("vec_new");
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_vec_intrinsic("vec_set");
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_vec_intrinsic("println");
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_box_intrinsic("box_new");
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_box_intrinsic("vec_new");
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = intrinsic_count();
  if ((_sv0t8 != 17)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_item_classify(void) {
  int _sv0t0 = item_registers_fn(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = item_registers_fn(1);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = item_registers_type(1);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = item_registers_type(2);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = item_registers_type(3);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = item_registers_type(7);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = item_registers_type(0);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = item_registers_type(4);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = item_registers_variants(2);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = item_registers_variants(1);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = item_is_use(5);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = item_is_use(0);
  if ((_sv0t11 != 0)) {
    return 12;
  } else {
  }
  int _sv0t12 = item_is_module(6);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  int _sv0t13 = item_needs_registration(0);
  if ((_sv0t13 != 1)) {
    return 14;
  } else {
  }
  int _sv0t14 = item_needs_registration(5);
  if ((_sv0t14 != 0)) {
    return 15;
  } else {
  }
  return 0;
}

static int test_variant_arity(void) {
  int _sv0t0 = enum_variant_arity(0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = enum_variant_arity(1, 3, 0);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = enum_variant_arity(2, 0, 5);
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  const char* _sv0t3 = enum_variant_qualified_path("Color", "Red");
  const char* qp;
  qp = _sv0t3;
  int _sv0t4 = sv0_string_eq(qp, "Color::Red");
  if ((_sv0t4 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_pat_binds(void) {
  int _sv0t0 = pat_binds_count(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = pat_binds_count(1, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = pat_binds_count(2, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = pat_binds_count(3, 3);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = pat_binds_count(5, 2);
  if ((_sv0t4 != 2)) {
    return 5;
  } else {
  }
  int _sv0t5 = pat_binds_count(6, 0);
  if ((_sv0t5 != 255)) {
    return 6;
  } else {
  }
  int _sv0t6 = pat_is_or(6);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = pat_is_or(1);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_context(void) {
  int _sv0t0 = should_allow_self(3);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = should_allow_self(4);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = should_allow_self(0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = should_allow_self(1);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_error_codes(void) {
  const char* _sv0t0 = error_code_unbound_ident();
  int _sv0t1 = sv0_string_eq(_sv0t0, "E0300");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = error_code_unknown_type();
  int _sv0t3 = sv0_string_eq(_sv0t2, "E0301");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = error_code_wrong_arity();
  int _sv0t5 = sv0_string_eq(_sv0t4, "E0307");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = error_code_use_clause();
  int _sv0t7 = sv0_string_eq(_sv0t6, "E0309");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_arity_check(void) {
  int _sv0t0 = arity_matches(3, 3);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = arity_matches(3, 2);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = check_call_arity(3, 3);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = check_call_arity(3, 2);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = (0 - 1);
  int _sv0t5 = check_call_arity(_sv0t4, 5);
  if ((_sv0t5 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_path_join_vec(void) {
  const char* src;
  src = "Foo::Bar::Baz";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 13);
  int _sv0t2 = sv0_vec_new();
  int indices = _sv0t2;
  sv0_vec_push(indices, 0);
  sv0_vec_push(indices, 1);
  sv0_vec_push(indices, 2);
  const char* _sv0t3 = path_join_vec(src, starts, ends, indices);
  const char* joined;
  joined = _sv0t3;
  int _sv0t4 = sv0_string_eq(joined, "Foo::Bar::Baz");
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int single = _sv0t5;
  sv0_vec_push(single, 0);
  const char* _sv0t6 = path_join_vec(src, starts, ends, single);
  const char* r2;
  r2 = _sv0t6;
  int _sv0t7 = sv0_string_eq(r2, "Foo");
  if ((_sv0t7 != 1)) {
    return 2;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int empty = _sv0t8;
  const char* _sv0t9 = path_join_vec(src, starts, ends, empty);
  const char* r3;
  r3 = _sv0t9;
  int _sv0t10 = sv0_string_eq(r3, "");
  if ((_sv0t10 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_import_alias(void) {
  int _sv0t0 = import_alias_new();
  int aliases = _sv0t0;
  int _sv0t1 = import_alias_count(aliases);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = import_alias_lookup(aliases, 10);
  int _sv0t3 = (0 - 1);
  if ((_sv0t2 != _sv0t3)) {
    return 2;
  } else {
  }
  int _sv0t4 = import_alias_add(aliases, 10, 20);
  int _sv0t5 = import_alias_add(aliases, 30, 40);
  int _sv0t6 = import_alias_count(aliases);
  if ((_sv0t6 != 2)) {
    return 3;
  } else {
  }
  int _sv0t7 = import_alias_lookup(aliases, 10);
  if ((_sv0t7 != 20)) {
    return 4;
  } else {
  }
  int _sv0t8 = import_alias_lookup(aliases, 30);
  if ((_sv0t8 != 40)) {
    return 5;
  } else {
  }
  int _sv0t9 = import_alias_lookup(aliases, 50);
  int _sv0t10 = (0 - 1);
  if ((_sv0t9 != _sv0t10)) {
    return 6;
  } else {
  }
  int _sv0t11 = import_alias_new();
  int aliases2 = _sv0t11;
  int _sv0t12 = import_alias_count(aliases2);
  if ((_sv0t12 != 0)) {
    return 7;
  } else {
  }
  int _sv0t13 = import_alias_lookup(aliases2, 10);
  int _sv0t14 = (0 - 1);
  if ((_sv0t13 != _sv0t14)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_register_intrinsics(void) {
  int _sv0t0 = sv0_vec_new();
  int fn_ar = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int mod_v = _sv0t1;
  int _sv0t2 = register_all_intrinsics(fn_ar, mod_v, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116);
  int count = _sv0t2;
  if ((count != 17)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(fn_ar);
  if ((_sv0t3 != 34)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_len(mod_v);
  if ((_sv0t4 != 17)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(fn_ar, 0);
  if ((_sv0t5 != 100)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_get(fn_ar, 1);
  if ((_sv0t6 != 1)) {
    return 5;
  } else {
  }
  int _sv0t7 = sv0_vec_get(fn_ar, 4);
  if ((_sv0t7 != 102)) {
    return 6;
  } else {
  }
  int _sv0t8 = sv0_vec_get(fn_ar, 5);
  if ((_sv0t8 != 3)) {
    return 7;
  } else {
  }
  int _sv0t9 = sv0_vec_get(mod_v, 0);
  if ((_sv0t9 != 100)) {
    return 8;
  } else {
  }
  int _sv0t10 = sv0_vec_get(mod_v, 16);
  if ((_sv0t10 != 116)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_variant_stem(void) {
  int _sv0t0 = variant_stem(0, 42);
  if ((_sv0t0 != 42)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_stem(1, 99);
  if ((_sv0t1 != 99)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_stem(2, 7);
  if ((_sv0t2 != 7)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_enum_variant_reg(void) {
  const char* _sv0t0 = enum_variant_reg_path("Option", "Some");
  const char* p;
  p = _sv0t0;
  int _sv0t1 = sv0_string_eq(p, "Option::Some");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = enum_variant_reg_arity(0, 0, 0);
  if ((_sv0t2 != 0)) {
    return 2;
  } else {
  }
  int _sv0t3 = enum_variant_reg_arity(1, 3, 0);
  if ((_sv0t3 != 3)) {
    return 3;
  } else {
  }
  int _sv0t4 = enum_variant_reg_arity(2, 0, 2);
  if ((_sv0t4 != 2)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_pat_binds_vec(void) {
  int _sv0t0 = sv0_vec_new();
  int out = _sv0t0;
  int _sv0t1 = pat_binds(0, 10, out);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(out);
  if ((_sv0t2 != 0)) {
    return 2;
  } else {
  }
  int _sv0t3 = pat_binds(1, 42, out);
  if ((_sv0t3 != 1)) {
    return 3;
  } else {
  }
  int _sv0t4 = sv0_vec_len(out);
  if ((_sv0t4 != 1)) {
    return 4;
  } else {
  }
  int _sv0t5 = sv0_vec_get(out, 0);
  if ((_sv0t5 != 42)) {
    return 5;
  } else {
  }
  int _sv0t6 = pat_binds(2, 99, out);
  if ((_sv0t6 != 0)) {
    return 6;
  } else {
  }
  int _sv0t7 = sv0_vec_len(out);
  if ((_sv0t7 != 1)) {
    return 7;
  } else {
  }
  int _sv0t8 = pat_binds(1, 77, out);
  if ((_sv0t8 != 1)) {
    return 8;
  } else {
  }
  int _sv0t9 = sv0_vec_len(out);
  if ((_sv0t9 != 2)) {
    return 9;
  } else {
  }
  int _sv0t10 = sv0_vec_get(out, 1);
  if ((_sv0t10 != 77)) {
    return 10;
  } else {
  }
  return 0;
}

static int test_bind_pattern_locals(void) {
  int _sv0t0 = sv0_vec_new();
  int binds = _sv0t0;
  sv0_vec_push(binds, 10);
  sv0_vec_push(binds, 20);
  sv0_vec_push(binds, 30);
  int _sv0t1 = sv0_vec_new();
  int env = _sv0t1;
  int _sv0t2 = bind_pattern_locals(binds, env);
  int n = _sv0t2;
  if ((n != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(env);
  if ((_sv0t3 != 3)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(env, 0);
  if ((_sv0t4 != 10)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(env, 2);
  if ((_sv0t5 != 30)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int empty = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int env2 = _sv0t7;
  int _sv0t8 = bind_pattern_locals(empty, env2);
  if ((_sv0t8 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_register_item_fn(void) {
  int _sv0t0 = sv0_vec_new();
  int mv = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int mt = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int fa = _sv0t2;
  int _sv0t3 = register_item(0, 42, 1, 3, 0, mv, mt, fa);
  int _sv0t4 = sv0_vec_len(mv);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_get(mv, 0);
  if ((_sv0t5 != 42)) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_vec_len(mt);
  if ((_sv0t6 != 0)) {
    return 3;
  } else {
  }
  int _sv0t7 = sv0_vec_len(fa);
  if ((_sv0t7 != 2)) {
    return 4;
  } else {
  }
  int _sv0t8 = sv0_vec_get(fa, 0);
  if ((_sv0t8 != 42)) {
    return 5;
  } else {
  }
  int _sv0t9 = sv0_vec_get(fa, 1);
  if ((_sv0t9 != 3)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_register_item_struct(void) {
  int _sv0t0 = sv0_vec_new();
  int mv = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int mt = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int fa = _sv0t2;
  int _sv0t3 = register_item(1, 10, 2, 0, 0, mv, mt, fa);
  int _sv0t4 = sv0_vec_len(mv);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_get(mv, 0);
  if ((_sv0t5 != 10)) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_vec_len(mt);
  if ((_sv0t6 != 1)) {
    return 3;
  } else {
  }
  int _sv0t7 = sv0_vec_get(mt, 0);
  if ((_sv0t7 != 10)) {
    return 4;
  } else {
  }
  int _sv0t8 = sv0_vec_len(fa);
  if ((_sv0t8 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_register_items(void) {
  int _sv0t0 = sv0_vec_new();
  int it = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int id1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int id2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int id3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id4 = _sv0t4;
  sv0_vec_push(it, 0);
  sv0_vec_push(id1, 5);
  sv0_vec_push(id2, 1);
  sv0_vec_push(id3, 2);
  sv0_vec_push(id4, 0);
  sv0_vec_push(it, 1);
  sv0_vec_push(id1, 10);
  sv0_vec_push(id2, 3);
  sv0_vec_push(id3, 0);
  sv0_vec_push(id4, 0);
  sv0_vec_push(it, 3);
  sv0_vec_push(id1, 20);
  sv0_vec_push(id2, 1);
  sv0_vec_push(id3, 0);
  sv0_vec_push(id4, 0);
  int _sv0t5 = sv0_vec_new();
  int mv = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int mt = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int fa = _sv0t7;
  int _sv0t8 = register_items(it, id1, id2, id3, id4, mv, mt, fa);
  int _sv0t9 = sv0_vec_len(mv);
  if ((_sv0t9 != 2)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(mv, 0);
  if ((_sv0t10 != 5)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(mv, 1);
  if ((_sv0t11 != 10)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_len(mt);
  if ((_sv0t12 != 2)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(mt, 0);
  if ((_sv0t13 != 10)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(mt, 1);
  if ((_sv0t14 != 20)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_len(fa);
  if ((_sv0t15 != 2)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(fa, 0);
  if ((_sv0t16 != 5)) {
    return 8;
  } else {
  }
  int _sv0t17 = sv0_vec_get(fa, 1);
  if ((_sv0t17 != 2)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_tok_str(void) {
  const char* source;
  source = "fn foo bar";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 10);
  const char* _sv0t2 = tok_str(source, starts, ends, 0);
  int _sv0t3 = sv0_string_eq(_sv0t2, "fn");
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t4 = tok_str(source, starts, ends, 1);
  int _sv0t5 = sv0_string_eq(_sv0t4, "foo");
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t6 = tok_str(source, starts, ends, 2);
  int _sv0t7 = sv0_string_eq(_sv0t6, "bar");
  if ((_sv0t7 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_res_value_exists(void) {
  const char* source;
  source = "alpha beta gamma";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 16);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  sv0_vec_push(mv, 0);
  sv0_vec_push(mv, 2);
  int _sv0t3 = res_value_exists(mv, "alpha", source, starts, ends);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = res_value_exists(mv, "gamma", source, starts, ends);
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  int _sv0t5 = res_value_exists(mv, "beta", source, starts, ends);
  if ((_sv0t5 != 0)) {
    return 3;
  } else {
  }
  int _sv0t6 = res_value_exists(mv, "nope", source, starts, ends);
  if ((_sv0t6 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_res_type_exists(void) {
  const char* source;
  source = "MyStruct i32";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 12);
  int _sv0t2 = sv0_vec_new();
  int mt = _sv0t2;
  sv0_vec_push(mt, 0);
  int _sv0t3 = res_type_exists(mt, "MyStruct", source, starts, ends);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = res_type_exists(mt, "i32", source, starts, ends);
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  int _sv0t5 = res_type_exists(mt, "bool", source, starts, ends);
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  int _sv0t6 = res_type_exists(mt, "Nope", source, starts, ends);
  if ((_sv0t6 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_resolve_pat_shape_simple(void) {
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
  const char* source;
  source = "_";
  int _sv0t4 = sv0_vec_new();
  int starts = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ends = _sv0t5;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  int _sv0t6 = sv0_vec_new();
  int mv = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int mt = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = resolve_pat_shape(pt, pd1, pd2, pd3, 0, source, starts, ends, mv, mt, pp);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  sv0_vec_push(pt, 1);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 0);
  sv0_vec_push(pd3, 0);
  int _sv0t10 = resolve_pat_shape(pt, pd1, pd2, pd3, 1, source, starts, ends, mv, mt, pp);
  if ((_sv0t10 != 0)) {
    return 2;
  } else {
  }
  sv0_vec_push(pt, 6);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 0);
  sv0_vec_push(pd3, 0);
  int _sv0t11 = resolve_pat_shape(pt, pd1, pd2, pd3, 2, source, starts, ends, mv, mt, pp);
  if ((_sv0t11 != 306)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_resolve_pat_enum(void) {
  const char* source;
  source = "Option Some Option::Some x";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 11);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 24);
  sv0_vec_push(starts, 25);
  sv0_vec_push(ends, 26);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  sv0_vec_push(mv, 2);
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  int _sv0t5 = sv0_vec_new();
  int pt = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd1 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pd2 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pd3 = _sv0t8;
  sv0_vec_push(pt, 1);
  sv0_vec_push(pd1, 3);
  sv0_vec_push(pd2, 0);
  sv0_vec_push(pd3, 0);
  sv0_vec_push(pt, 5);
  sv0_vec_push(pd1, 0);
  sv0_vec_push(pd2, 2);
  sv0_vec_push(pd3, 1);
  int _sv0t9 = resolve_pat_shape(pt, pd1, pd2, pd3, 1, source, starts, ends, mv, mt, pp);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int mv2 = _sv0t10;
  int _sv0t11 = resolve_pat_shape(pt, pd1, pd2, pd3, 1, source, starts, ends, mv2, mt, pp);
  if ((_sv0t11 != 300)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_resolve_ty_simple(void) {
  const char* source;
  source = "i32 MyType";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 10);
  int _sv0t2 = sv0_vec_new();
  int mt = _sv0t2;
  sv0_vec_push(mt, 1);
  int _sv0t3 = sv0_vec_new();
  int pp = _sv0t3;
  sv0_vec_push(pp, 0);
  int _sv0t4 = sv0_vec_new();
  int tt = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int td1 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int td2 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int td3 = _sv0t7;
  sv0_vec_push(tt, 0);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 0);
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = resolve_ty(tt, td1, td2, td3, 0, source, starts, ends, mt, _sv0t8, pp);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  sv0_vec_push(pp, 1);
  sv0_vec_push(tt, 0);
  sv0_vec_push(td1, 1);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 0);
  int _sv0t10 = sv0_vec_new();
  int _sv0t11 = resolve_ty(tt, td1, td2, td3, 1, source, starts, ends, mt, _sv0t10, pp);
  if ((_sv0t11 != 0)) {
    return 2;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int mt2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int _sv0t14 = resolve_ty(tt, td1, td2, td3, 1, source, starts, ends, mt2, _sv0t13, pp);
  int _sv0t15 = diag_code_of(_sv0t14);
  if ((_sv0t15 != 301)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_resolve_ty_array_len(void) {
  const char* source;
  source = "i32 N";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  int _sv0t2 = sv0_vec_new();
  int mt = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pp = _sv0t3;
  sv0_vec_push(pp, 0);
  int _sv0t4 = sv0_vec_new();
  int tt = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int td1 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int td2 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int td3 = _sv0t7;
  sv0_vec_push(tt, 0);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 0);
  sv0_vec_push(tt, 3);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 1);
  int _sv0t8 = sv0_vec_new();
  int mv_empty = _sv0t8;
  int _sv0t9 = resolve_ty(tt, td1, td2, td3, 1, source, starts, ends, mt, mv_empty, pp);
  if ((_sv0t9 != 300)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int mv_bound = _sv0t10;
  sv0_vec_push(mv_bound, 1);
  int _sv0t11 = resolve_ty(tt, td1, td2, td3, 1, source, starts, ends, mt, mv_bound, pp);
  if ((_sv0t11 != 0)) {
    return 2;
  } else {
  }
  sv0_vec_set(td3, 1, 0);
  int _sv0t12 = resolve_ty(tt, td1, td2, td3, 1, source, starts, ends, mt, mv_empty, pp);
  if ((_sv0t12 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_resolve_ty_ref(void) {
  const char* source;
  source = "i32";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  int _sv0t2 = sv0_vec_new();
  int mt = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pp = _sv0t3;
  sv0_vec_push(pp, 0);
  int _sv0t4 = sv0_vec_new();
  int tt = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int td1 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int td2 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int td3 = _sv0t7;
  sv0_vec_push(tt, 0);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 0);
  sv0_vec_push(tt, 1);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 0);
  sv0_vec_push(td3, 0);
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = resolve_ty(tt, td1, td2, td3, 1, source, starts, ends, mt, _sv0t8, pp);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  sv0_vec_push(tt, 6);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 0);
  sv0_vec_push(td3, 0);
  int _sv0t10 = sv0_vec_new();
  int _sv0t11 = resolve_ty(tt, td1, td2, td3, 2, source, starts, ends, mt, _sv0t10, pp);
  if ((_sv0t11 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_scope_mgmt(void) {
  const char* source;
  source = "x y z";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  int _sv0t2 = sv0_vec_new();
  int frames = _sv0t2;
  int _sv0t3 = res_enter_scope(frames);
  int _sv0t4 = res_bind_local(frames, 0);
  int _sv0t5 = res_lookup_in_frames_str(frames, "x", source, starts, ends);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = res_lookup_in_frames_str(frames, "y", source, starts, ends);
  if ((_sv0t6 != 0)) {
    return 2;
  } else {
  }
  int _sv0t7 = res_bind_local(frames, 1);
  int _sv0t8 = res_lookup_in_frames_str(frames, "y", source, starts, ends);
  if ((_sv0t8 != 1)) {
    return 3;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int mv = _sv0t9;
  sv0_vec_push(mv, 2);
  int _sv0t10 = res_full_value_exists(mv, frames, "z", source, starts, ends);
  if ((_sv0t10 != 1)) {
    return 4;
  } else {
  }
  int _sv0t11 = res_full_value_exists(mv, frames, "x", source, starts, ends);
  if ((_sv0t11 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_resolve_expr_lit(void) {
  const char* source;
  source = "42";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  int _sv0t2 = sv0_vec_new();
  int et = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed1 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed2 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed3 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed4 = _sv0t6;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t7 = sv0_vec_new();
  int mv = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int mt = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int fa = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int frames = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int pp = _sv0t11;
  int _sv0t12 = resolve_expr(et, ed1, ed2, ed3, ed4, 0, source, starts, ends, mv, mt, fa, frames, pp);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_resolve_expr_path(void) {
  const char* source;
  source = "foo bar";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 7);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  sv0_vec_push(mv, 0);
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int frames = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pp = _sv0t6;
  sv0_vec_push(pp, 0);
  int _sv0t7 = sv0_vec_new();
  int et = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ed1 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed2 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed3 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int ed4 = _sv0t11;
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t12 = resolve_expr(et, ed1, ed2, ed3, ed4, 0, source, starts, ends, mv, mt, fa, frames, pp);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  sv0_vec_push(pp, 1);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t13 = resolve_expr(et, ed1, ed2, ed3, ed4, 1, source, starts, ends, mv, mt, fa, frames, pp);
  int _sv0t14 = diag_code_of(_sv0t13);
  if ((_sv0t14 != 300)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_resolve_expr_block(void) {
  const char* source;
  source = "x 10";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 4);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int frames = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pp = _sv0t6;
  sv0_vec_push(pp, 0);
  int _sv0t7 = sv0_vec_new();
  int et = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ed1 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed2 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed3 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int ed4 = _sv0t11;
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, 0);
  int _sv0t12 = (0 - 1);
  sv0_vec_push(ed2, _sv0t12);
  int _sv0t13 = (0 - 1);
  sv0_vec_push(ed3, _sv0t13);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 9);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 1);
  sv0_vec_push(ed4, 0);
  int _sv0t14 = resolve_expr(et, ed1, ed2, ed3, ed4, 2, source, starts, ends, mv, mt, fa, frames, pp);
  if ((_sv0t14 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_resolve_expr_call_arity(void) {
  const char* source;
  source = "foo 1 2";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  sv0_vec_push(mv, 0);
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  sv0_vec_push(fa, 0);
  sv0_vec_push(fa, 2);
  int _sv0t5 = sv0_vec_new();
  int frames = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pp = _sv0t6;
  sv0_vec_push(pp, 0);
  int _sv0t7 = sv0_vec_new();
  int et = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ed1 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed2 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed3 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int ed4 = _sv0t11;
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 2);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 4);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 2);
  sv0_vec_push(ed4, 0);
  int _sv0t12 = resolve_expr(et, ed1, ed2, ed3, ed4, 3, source, starts, ends, mv, mt, fa, frames, pp);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  sv0_vec_push(et, 4);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 1);
  sv0_vec_push(ed4, 0);
  int _sv0t13 = resolve_expr(et, ed1, ed2, ed3, ed4, 4, source, starts, ends, mv, mt, fa, frames, pp);
  int _sv0t14 = diag_code_of(_sv0t13);
  if ((_sv0t14 != 307)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_resolve_stmt_let(void) {
  const char* source;
  source = "a b";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int frames = _sv0t5;
  int _sv0t6 = res_enter_scope(frames);
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  sv0_vec_push(pp, 0);
  int _sv0t8 = sv0_vec_new();
  int et = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed1 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed2 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int ed3 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int ed4 = _sv0t12;
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, 0);
  int _sv0t13 = (0 - 1);
  sv0_vec_push(ed2, _sv0t13);
  int _sv0t14 = (0 - 1);
  sv0_vec_push(ed3, _sv0t14);
  sv0_vec_push(ed4, 0);
  int _sv0t15 = resolve_stmt(et, ed1, ed2, ed3, ed4, 0, source, starts, ends, mv, mt, fa, frames, pp);
  if ((_sv0t15 != 0)) {
    return 1;
  } else {
  }
  int _sv0t16 = res_lookup_in_frames_str(frames, "a", source, starts, ends);
  if ((_sv0t16 != 1)) {
    return 2;
  } else {
  }
  sv0_vec_push(pp, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t17 = resolve_expr(et, ed1, ed2, ed3, ed4, 1, source, starts, ends, mv, mt, fa, frames, pp);
  if ((_sv0t17 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_apply_use_clause_value(void) {
  const char* source;
  source = "mymod mymod__bar bar x";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 16);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 20);
  sv0_vec_push(starts, 21);
  sv0_vec_push(ends, 22);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  sv0_vec_push(mv, 1);
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  sv0_vec_push(fa, 1);
  sv0_vec_push(fa, 2);
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 2);
  int _sv0t6 = sv0_vec_new();
  int it = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id1 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int id2 = _sv0t8;
  sv0_vec_push(it, 5);
  sv0_vec_push(id1, 0);
  sv0_vec_push(id2, 2);
  int _sv0t9 = apply_use_clause(it, id1, id2, 0, source, starts, ends, mv, mt, fa, pp);
  int r = _sv0t9;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int _sv0t11 = res_full_value_exists(mv, _sv0t10, "bar", source, starts, ends);
  if ((_sv0t11 != 1)) {
    return 2;
  } else {
  }
  int _sv0t12 = res_lookup_fn_arity_str(fa, "bar", source, starts, ends);
  if ((_sv0t12 != 2)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_apply_use_clause_type(void) {
  const char* source;
  source = "mymod mymod__Point Point";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 18);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 24);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  sv0_vec_push(mt, 1);
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 2);
  int _sv0t6 = sv0_vec_new();
  int it = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id1 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int id2 = _sv0t8;
  sv0_vec_push(it, 5);
  sv0_vec_push(id1, 0);
  sv0_vec_push(id2, 2);
  int _sv0t9 = apply_use_clause(it, id1, id2, 0, source, starts, ends, mv, mt, fa, pp);
  int r = _sv0t9;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = res_type_exists(mt, "Point", source, starts, ends);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_apply_use_clause_unknown(void) {
  const char* source;
  source = "mymod unknown";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 13);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  int _sv0t6 = sv0_vec_new();
  int it = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id1 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int id2 = _sv0t8;
  sv0_vec_push(it, 5);
  sv0_vec_push(id1, 0);
  sv0_vec_push(id2, 2);
  int _sv0t9 = apply_use_clause(it, id1, id2, 0, source, starts, ends, mv, mt, fa, pp);
  int r = _sv0t9;
  int _sv0t10 = diag_code_of(r);
  if ((_sv0t10 != 309)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_apply_use_clause_unmangled(void) {
  const char* source;
  source = "mymod bar Sig";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 9);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 13);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  sv0_vec_push(mt, 2);
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  sv0_vec_push(fa, 1);
  sv0_vec_push(fa, 3);
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  int _sv0t6 = sv0_vec_new();
  int it = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id1 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int id2 = _sv0t8;
  sv0_vec_push(it, 5);
  sv0_vec_push(id1, 0);
  sv0_vec_push(id2, 2);
  int _sv0t9 = apply_use_clause(it, id1, id2, 0, source, starts, ends, mv, mt, fa, pp);
  int r = _sv0t9;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = res_lookup_fn_arity_str(fa, "bar", source, starts, ends);
  if ((_sv0t10 != 3)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int pp2 = _sv0t11;
  sv0_vec_push(pp2, 0);
  sv0_vec_push(pp2, 2);
  int _sv0t12 = sv0_vec_new();
  int it2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int id1b = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int id2b = _sv0t14;
  sv0_vec_push(it2, 5);
  sv0_vec_push(id1b, 0);
  sv0_vec_push(id2b, 2);
  int _sv0t15 = apply_use_clause(it2, id1b, id2b, 0, source, starts, ends, mv, mt, fa, pp2);
  int r2 = _sv0t15;
  if ((r2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_path_join_vec_three(void) {
  const char* source;
  source = "a b c";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  int _sv0t2 = sv0_vec_new();
  int path = _sv0t2;
  sv0_vec_push(path, 0);
  sv0_vec_push(path, 1);
  sv0_vec_push(path, 2);
  const char* _sv0t3 = path_join_vec(source, starts, ends, path);
  const char* out;
  out = _sv0t3;
  int _sv0t4 = sv0_string_eq(out, "a::b::c");
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_resolve_top_item_struct_field(void) {
  const char* source;
  source = "i32";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  int _sv0t2 = sv0_vec_new();
  int mt = _sv0t2;
  sv0_vec_push(mt, 99);
  int _sv0t3 = sv0_vec_new();
  int mv = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 0);
  int _sv0t6 = sv0_vec_new();
  int pty_tt = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pty_td1 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pty_td2 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int pty_td3 = _sv0t9;
  sv0_vec_push(pty_tt, 0);
  sv0_vec_push(pty_td1, 0);
  sv0_vec_push(pty_td2, 1);
  sv0_vec_push(pty_td3, 0);
  int _sv0t10 = sv0_vec_new();
  int it = _sv0t10;
  sv0_vec_push(it, 1);
  int _sv0t11 = sv0_vec_new();
  int id1 = _sv0t11;
  sv0_vec_push(id1, 0);
  int _sv0t12 = sv0_vec_new();
  int id2 = _sv0t12;
  sv0_vec_push(id2, 1);
  int _sv0t13 = sv0_vec_new();
  int id3 = _sv0t13;
  sv0_vec_push(id3, 0);
  int _sv0t14 = sv0_vec_new();
  int id4 = _sv0t14;
  sv0_vec_push(id4, 1);
  int _sv0t15 = sv0_vec_new();
  int id5 = _sv0t15;
  int _sv0t16 = (0 - 1);
  sv0_vec_push(id5, _sv0t16);
  int _sv0t17 = sv0_vec_new();
  int et = _sv0t17;
  sv0_vec_push(et, 0);
  int _sv0t18 = sv0_vec_new();
  int ed1 = _sv0t18;
  sv0_vec_push(ed1, 0);
  int _sv0t19 = sv0_vec_new();
  int ed2 = _sv0t19;
  sv0_vec_push(ed2, 0);
  int _sv0t20 = sv0_vec_new();
  int ed3 = _sv0t20;
  sv0_vec_push(ed3, 0);
  int _sv0t21 = sv0_vec_new();
  int ed4 = _sv0t21;
  sv0_vec_push(ed4, 0);
  int _sv0t22 = sv0_vec_new();
  int fn_ptoks = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int fn_p_ty_root = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int fn_ret_root = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int fn_contract_base = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int fn_contract_root = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int enum_payload_ty_root = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int enum_payload_base = _sv0t28;
  int _sv0t29 = sv0_vec_new();
  int enum_payload_count = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int sf_root = _sv0t30;
  sv0_vec_push(sf_root, 0);
  int _sv0t31 = (0 - 1);
  sv0_vec_push(fn_ret_root, _sv0t31);
  int _sv0t32 = (0 - 1);
  sv0_vec_push(fn_contract_base, _sv0t32);
  int _sv0t33 = (0 - 1);
  sv0_vec_push(enum_payload_base, _sv0t33);
  sv0_vec_push(enum_payload_count, 0);
  int _sv0t34 = resolve_top_item(it, id1, id2, id3, id4, id5, 0, et, ed1, ed2, ed3, ed4, fn_ptoks, fn_p_ty_root, fn_ret_root, fn_contract_base, fn_contract_root, enum_payload_ty_root, enum_payload_base, enum_payload_count, pty_tt, pty_td1, pty_td2, pty_td3, sf_root, source, starts, ends, mv, mt, fa, pp);
  int r = _sv0t34;
  if ((r != 0)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_resolve_top_item_fn_signature_contracts(void) {
  const char* source;
  source = "x i32";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 5);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  int _sv0t6 = sv0_vec_new();
  int it = _sv0t6;
  sv0_vec_push(it, 0);
  int _sv0t7 = sv0_vec_new();
  int id1 = _sv0t7;
  sv0_vec_push(id1, 0);
  int _sv0t8 = sv0_vec_new();
  int id2 = _sv0t8;
  sv0_vec_push(id2, 3);
  int _sv0t9 = sv0_vec_new();
  int id3 = _sv0t9;
  sv0_vec_push(id3, 1);
  int _sv0t10 = sv0_vec_new();
  int id4 = _sv0t10;
  sv0_vec_push(id4, 1);
  int _sv0t11 = sv0_vec_new();
  int id5 = _sv0t11;
  sv0_vec_push(id5, 0);
  int _sv0t12 = sv0_vec_new();
  int et = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int ed1 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int ed2 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ed3 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int ed4 = _sv0t16;
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t17 = sv0_vec_new();
  int fn_ptoks = _sv0t17;
  sv0_vec_push(fn_ptoks, 0);
  int _sv0t18 = sv0_vec_new();
  int fn_p_ty_root = _sv0t18;
  sv0_vec_push(fn_p_ty_root, 0);
  int _sv0t19 = sv0_vec_new();
  int fn_ret_root = _sv0t19;
  sv0_vec_push(fn_ret_root, 1);
  int _sv0t20 = sv0_vec_new();
  int fn_contract_base = _sv0t20;
  sv0_vec_push(fn_contract_base, 0);
  int _sv0t21 = sv0_vec_new();
  int fn_contract_root = _sv0t21;
  sv0_vec_push(fn_contract_root, 0);
  int _sv0t22 = sv0_vec_new();
  int enum_payload_ty_root = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int enum_payload_base = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_payload_count = _sv0t24;
  int _sv0t25 = (0 - 1);
  sv0_vec_push(enum_payload_base, _sv0t25);
  sv0_vec_push(enum_payload_count, 0);
  int _sv0t26 = sv0_vec_new();
  int pty_tt = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int pty_td1 = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int pty_td2 = _sv0t28;
  int _sv0t29 = sv0_vec_new();
  int pty_td3 = _sv0t29;
  sv0_vec_push(pty_tt, 0);
  sv0_vec_push(pty_td1, 1);
  sv0_vec_push(pty_td2, 1);
  sv0_vec_push(pty_td3, 0);
  sv0_vec_push(pty_tt, 0);
  sv0_vec_push(pty_td1, 1);
  sv0_vec_push(pty_td2, 1);
  sv0_vec_push(pty_td3, 0);
  int _sv0t30 = sv0_vec_new();
  int sf_root = _sv0t30;
  int _sv0t31 = resolve_top_item(it, id1, id2, id3, id4, id5, 0, et, ed1, ed2, ed3, ed4, fn_ptoks, fn_p_ty_root, fn_ret_root, fn_contract_base, fn_contract_root, enum_payload_ty_root, enum_payload_base, enum_payload_count, pty_tt, pty_td1, pty_td2, pty_td3, sf_root, source, starts, ends, mv, mt, fa, pp);
  int r = _sv0t31;
  if ((r != 0)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_resolve_top_item_enum_payload_type(void) {
  const char* source;
  source = "Foo";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 0);
  int _sv0t6 = sv0_vec_new();
  int it = _sv0t6;
  sv0_vec_push(it, 2);
  int _sv0t7 = sv0_vec_new();
  int id1 = _sv0t7;
  sv0_vec_push(id1, 0);
  int _sv0t8 = sv0_vec_new();
  int id2 = _sv0t8;
  sv0_vec_push(id2, 1);
  int _sv0t9 = sv0_vec_new();
  int id3 = _sv0t9;
  sv0_vec_push(id3, 0);
  int _sv0t10 = sv0_vec_new();
  int id4 = _sv0t10;
  sv0_vec_push(id4, 1);
  int _sv0t11 = sv0_vec_new();
  int id5 = _sv0t11;
  int _sv0t12 = (0 - 1);
  sv0_vec_push(id5, _sv0t12);
  int _sv0t13 = sv0_vec_new();
  int et = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int ed1 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ed2 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int ed3 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ed4 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int fn_ptoks = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int fn_p_ty_root = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int fn_ret_root = _sv0t20;
  int _sv0t21 = (0 - 1);
  sv0_vec_push(fn_ret_root, _sv0t21);
  int _sv0t22 = sv0_vec_new();
  int fn_contract_base = _sv0t22;
  int _sv0t23 = (0 - 1);
  sv0_vec_push(fn_contract_base, _sv0t23);
  int _sv0t24 = sv0_vec_new();
  int fn_contract_root = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_payload_ty_root = _sv0t25;
  sv0_vec_push(enum_payload_ty_root, 0);
  int _sv0t26 = sv0_vec_new();
  int enum_payload_base = _sv0t26;
  sv0_vec_push(enum_payload_base, 0);
  int _sv0t27 = sv0_vec_new();
  int enum_payload_count = _sv0t27;
  sv0_vec_push(enum_payload_count, 1);
  int _sv0t28 = sv0_vec_new();
  int pty_tt = _sv0t28;
  int _sv0t29 = sv0_vec_new();
  int pty_td1 = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int pty_td2 = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int pty_td3 = _sv0t31;
  sv0_vec_push(pty_tt, 0);
  sv0_vec_push(pty_td1, 0);
  sv0_vec_push(pty_td2, 1);
  sv0_vec_push(pty_td3, 0);
  int _sv0t32 = sv0_vec_new();
  int sf_root = _sv0t32;
  int _sv0t33 = resolve_top_item(it, id1, id2, id3, id4, id5, 0, et, ed1, ed2, ed3, ed4, fn_ptoks, fn_p_ty_root, fn_ret_root, fn_contract_base, fn_contract_root, enum_payload_ty_root, enum_payload_base, enum_payload_count, pty_tt, pty_td1, pty_td2, pty_td3, sf_root, source, starts, ends, mv, mt, fa, pp);
  int r = _sv0t33;
  int _sv0t34 = diag_code_of(r);
  if ((_sv0t34 != 301)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_resolve_program(void) {
  const char* source;
  source = "main i32";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 8);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 1);
  int _sv0t6 = sv0_vec_new();
  int et = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed1 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ed2 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed3 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed4 = _sv0t10;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t11 = sv0_vec_new();
  int it = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int id1 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int id2 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int id3 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int id4 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int id5 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int fn_ptoks = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int fn_p_ty_root = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int fn_ret_root = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int fn_contract_base = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int fn_contract_root = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int enum_payload_ty_root = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int enum_payload_base = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_payload_count = _sv0t24;
  sv0_vec_push(it, 0);
  sv0_vec_push(id1, 0);
  sv0_vec_push(id2, 0);
  sv0_vec_push(id3, 1);
  sv0_vec_push(id4, 0);
  sv0_vec_push(id5, 0);
  sv0_vec_push(fn_ptoks, 0);
  sv0_vec_push(fn_p_ty_root, 0);
  int _sv0t25 = (0 - 1);
  sv0_vec_push(fn_ret_root, _sv0t25);
  int _sv0t26 = (0 - 1);
  sv0_vec_push(fn_contract_base, _sv0t26);
  int _sv0t27 = (0 - 1);
  sv0_vec_push(enum_payload_base, _sv0t27);
  sv0_vec_push(enum_payload_count, 0);
  int _sv0t28 = sv0_vec_new();
  int pty_tt = _sv0t28;
  int _sv0t29 = sv0_vec_new();
  int pty_td1 = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int pty_td2 = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int pty_td3 = _sv0t31;
  sv0_vec_push(pty_tt, 0);
  sv0_vec_push(pty_td1, 0);
  sv0_vec_push(pty_td2, 1);
  sv0_vec_push(pty_td3, 0);
  int _sv0t32 = sv0_vec_new();
  int sf_tr = _sv0t32;
  int _sv0t33 = resolve_program(et, ed1, ed2, ed3, ed4, it, id1, id2, id3, id4, id5, fn_ptoks, fn_p_ty_root, fn_ret_root, fn_contract_base, fn_contract_root, enum_payload_ty_root, enum_payload_base, enum_payload_count, pty_tt, pty_td1, pty_td2, pty_td3, sf_tr, source, starts, ends, mv, mt, fa, pp);
  int r = _sv0t33;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t34 = sv0_vec_new();
  int _sv0t35 = res_full_value_exists(mv, _sv0t34, "main", source, starts, ends);
  if ((_sv0t35 != 1)) {
    return 2;
  } else {
  }
  int _sv0t36 = res_lookup_fn_arity_str(fa, "main", source, starts, ends);
  if ((_sv0t36 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_resolve_program_cast_type_roots(void) {
  const char* source;
  source = "main NoSuch";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 11);
  int _sv0t2 = sv0_vec_new();
  int mv = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int mt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fa = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  int _sv0t6 = sv0_vec_new();
  int et = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed1 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ed2 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed3 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed4 = _sv0t10;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 20);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t11 = sv0_vec_new();
  int it = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int id1 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int id2 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int id3 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int id4 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int id5 = _sv0t16;
  sv0_vec_push(it, 0);
  sv0_vec_push(id1, 0);
  sv0_vec_push(id2, 0);
  sv0_vec_push(id3, 0);
  sv0_vec_push(id4, 1);
  int _sv0t17 = (0 - 1);
  sv0_vec_push(id5, _sv0t17);
  int _sv0t18 = sv0_vec_new();
  int fn_ptoks = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int fn_p_ty_root = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int fn_ret_root = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int fn_contract_base = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int fn_contract_root = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int enum_payload_ty_root = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_payload_base = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_payload_count = _sv0t25;
  int _sv0t26 = (0 - 1);
  sv0_vec_push(fn_ret_root, _sv0t26);
  int _sv0t27 = (0 - 1);
  sv0_vec_push(fn_contract_base, _sv0t27);
  int _sv0t28 = (0 - 1);
  sv0_vec_push(enum_payload_base, _sv0t28);
  sv0_vec_push(enum_payload_count, 0);
  int _sv0t29 = sv0_vec_new();
  int pty_tt = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int pty_td1 = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int pty_td2 = _sv0t31;
  int _sv0t32 = sv0_vec_new();
  int pty_td3 = _sv0t32;
  sv0_vec_push(pty_tt, 0);
  sv0_vec_push(pty_td1, 1);
  sv0_vec_push(pty_td2, 1);
  sv0_vec_push(pty_td3, 0);
  int _sv0t33 = sv0_vec_new();
  int sf_tr = _sv0t33;
  int _sv0t34 = resolve_program(et, ed1, ed2, ed3, ed4, it, id1, id2, id3, id4, id5, fn_ptoks, fn_p_ty_root, fn_ret_root, fn_contract_base, fn_contract_root, enum_payload_ty_root, enum_payload_base, enum_payload_count, pty_tt, pty_td1, pty_td2, pty_td3, sf_tr, source, starts, ends, mv, mt, fa, pp);
  int r1 = _sv0t34;
  int _sv0t35 = diag_code_of(r1);
  if ((_sv0t35 != 301)) {
    return 1;
  } else {
  }
  sv0_vec_push(mt, 1);
  int _sv0t36 = resolve_program(et, ed1, ed2, ed3, ed4, it, id1, id2, id3, id4, id5, fn_ptoks, fn_p_ty_root, fn_ret_root, fn_contract_base, fn_contract_root, enum_payload_ty_root, enum_payload_base, enum_payload_count, pty_tt, pty_td1, pty_td2, pty_td3, sf_tr, source, starts, ends, mv, mt, fa, pp);
  int r2 = _sv0t36;
  if ((r2 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_path_join();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_mangle_use();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_is_intrinsic();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_intrinsic_arity();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_intrinsic_classify();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_item_classify();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_variant_arity();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (70 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_pat_binds();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (80 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_context();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (90 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_error_codes();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (100 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_arity_check();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (110 + r11);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_path_join_vec();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (120 + r12);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_import_alias();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (130 + r13);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_register_intrinsics();
  int r14 = _sv0t25;
  if ((r14 != 0)) {
    int _sv0t26 = (140 + r14);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = test_variant_stem();
  int r15 = _sv0t27;
  if ((r15 != 0)) {
    int _sv0t28 = (150 + r15);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = test_enum_variant_reg();
  int r16 = _sv0t29;
  if ((r16 != 0)) {
    int _sv0t30 = (160 + r16);
    return _sv0t30;
  } else {
  }
  int _sv0t31 = test_pat_binds_vec();
  int r17 = _sv0t31;
  if ((r17 != 0)) {
    int _sv0t32 = (170 + r17);
    return _sv0t32;
  } else {
  }
  int _sv0t33 = test_bind_pattern_locals();
  int r18 = _sv0t33;
  if ((r18 != 0)) {
    int _sv0t34 = (180 + r18);
    return _sv0t34;
  } else {
  }
  int _sv0t35 = test_register_item_fn();
  int r19 = _sv0t35;
  if ((r19 != 0)) {
    int _sv0t36 = (190 + r19);
    return _sv0t36;
  } else {
  }
  int _sv0t37 = test_register_item_struct();
  int r20 = _sv0t37;
  if ((r20 != 0)) {
    int _sv0t38 = (200 + r20);
    return _sv0t38;
  } else {
  }
  int _sv0t39 = test_register_items();
  int r21 = _sv0t39;
  if ((r21 != 0)) {
    int _sv0t40 = (210 + r21);
    return _sv0t40;
  } else {
  }
  int _sv0t41 = test_tok_str();
  int r22 = _sv0t41;
  if ((r22 != 0)) {
    int _sv0t42 = (220 + r22);
    return _sv0t42;
  } else {
  }
  int _sv0t43 = test_res_value_exists();
  int r23 = _sv0t43;
  if ((r23 != 0)) {
    int _sv0t44 = (230 + r23);
    return _sv0t44;
  } else {
  }
  int _sv0t45 = test_res_type_exists();
  int r24 = _sv0t45;
  if ((r24 != 0)) {
    int _sv0t46 = (240 + r24);
    return _sv0t46;
  } else {
  }
  int _sv0t47 = test_resolve_pat_shape_simple();
  int r25 = _sv0t47;
  if ((r25 != 0)) {
    int _sv0t48 = (250 + r25);
    return _sv0t48;
  } else {
  }
  int _sv0t49 = test_resolve_pat_enum();
  int r26 = _sv0t49;
  if ((r26 != 0)) {
    int _sv0t50 = (260 + r26);
    return _sv0t50;
  } else {
  }
  int _sv0t51 = test_resolve_ty_simple();
  int r27 = _sv0t51;
  if ((r27 != 0)) {
    int _sv0t52 = (270 + r27);
    return _sv0t52;
  } else {
  }
  int _sv0t53 = test_resolve_ty_ref();
  int r28 = _sv0t53;
  if ((r28 != 0)) {
    int _sv0t54 = (280 + r28);
    return _sv0t54;
  } else {
  }
  int _sv0t55 = test_resolve_ty_array_len();
  int r28a = _sv0t55;
  if ((r28a != 0)) {
    int _sv0t56 = (285 + r28a);
    return _sv0t56;
  } else {
  }
  int _sv0t57 = test_scope_mgmt();
  int r29 = _sv0t57;
  if ((r29 != 0)) {
    int _sv0t58 = (3 + r29);
    return _sv0t58;
  } else {
  }
  int _sv0t59 = test_resolve_expr_lit();
  int r30 = _sv0t59;
  if ((r30 != 0)) {
    int _sv0t60 = (60 + r30);
    return _sv0t60;
  } else {
  }
  int _sv0t61 = test_resolve_expr_path();
  int r31 = _sv0t61;
  if ((r31 != 0)) {
    int _sv0t62 = (63 + r31);
    return _sv0t62;
  } else {
  }
  int _sv0t63 = test_resolve_expr_block();
  int r32 = _sv0t63;
  if ((r32 != 0)) {
    int _sv0t64 = (66 + r32);
    return _sv0t64;
  } else {
  }
  int _sv0t65 = test_resolve_expr_call_arity();
  int r33 = _sv0t65;
  if ((r33 != 0)) {
    int _sv0t66 = (136 + r33);
    return _sv0t66;
  } else {
  }
  int _sv0t67 = test_resolve_stmt_let();
  int r34 = _sv0t67;
  if ((r34 != 0)) {
    int _sv0t68 = (146 + r34);
    return _sv0t68;
  } else {
  }
  int _sv0t69 = test_apply_use_clause_value();
  int r35 = _sv0t69;
  if ((r35 != 0)) {
    int _sv0t70 = (152 + r35);
    return _sv0t70;
  } else {
  }
  int _sv0t71 = test_apply_use_clause_type();
  int r36 = _sv0t71;
  if ((r36 != 0)) {
    int _sv0t72 = (156 + r36);
    return _sv0t72;
  } else {
  }
  int _sv0t73 = test_apply_use_clause_unknown();
  int r37 = _sv0t73;
  if ((r37 != 0)) {
    int _sv0t74 = (159 + r37);
    return _sv0t74;
  } else {
  }
  int _sv0t75 = test_apply_use_clause_unmangled();
  int r37e = _sv0t75;
  if ((r37e != 0)) {
    int _sv0t76 = (159 + r37e);
    return _sv0t76;
  } else {
  }
  int _sv0t77 = test_resolve_top_item_struct_field();
  int r37b = _sv0t77;
  if ((r37b != 0)) {
    int _sv0t78 = (161 + r37b);
    return _sv0t78;
  } else {
  }
  int _sv0t79 = test_resolve_top_item_fn_signature_contracts();
  int r37c = _sv0t79;
  if ((r37c != 0)) {
    int _sv0t80 = (162 + r37c);
    return _sv0t80;
  } else {
  }
  int _sv0t81 = test_resolve_top_item_enum_payload_type();
  int r37d = _sv0t81;
  if ((r37d != 0)) {
    int _sv0t82 = (163 + r37d);
    return _sv0t82;
  } else {
  }
  int _sv0t83 = test_resolve_program();
  int r38 = _sv0t83;
  if ((r38 != 0)) {
    int _sv0t84 = (164 + r38);
    return _sv0t84;
  } else {
  }
  int _sv0t85 = test_resolve_program_cast_type_roots();
  int r38a = _sv0t85;
  if ((r38a != 0)) {
    int _sv0t86 = (166 + r38a);
    return _sv0t86;
  } else {
  }
  int _sv0t87 = test_path_join_vec_three();
  int r39 = _sv0t87;
  if ((r39 != 0)) {
    int _sv0t88 = (167 + r39);
    return _sv0t88;
  } else {
  }
  return 0;
}

