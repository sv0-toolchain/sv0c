#include "sv0_runtime.h"

static int BINOP_ARITH(void);
static int BINOP_LOGIC(void);
static int BINOP_CMP(void);
static int binop_class(int tag);
static int is_arith_binop(int tag);
static int is_logic_binop(int tag);
static int is_cmp_binop(int tag);
static int binop_returns_bool(int tag);
static int is_integral_ty(int ty_tag);
static int is_bool_ty(int ty_tag);
static int is_unit_ty(int ty_tag);
static int is_string_ty(int ty_tag);
static int is_numeric_ty(int ty_tag);
static int assign_lhs_is_var(int class);
static int assign_lhs_is_field(int class);
static int TY_UNIT(void);
static int TY_BOOL(void);
static int TY_CHAR(void);
static int TY_INT(void);
static int TY_UINT(void);
static int TY_ISIZE(void);
static int TY_USIZE(void);
static int TY_FLOAT(void);
static int TY_STRING(void);
static int TY_STRUCT(void);
static int TY_ENUM(void);
static int TY_FN(void);
static int TY_REF(void);
static int TY_REFMUT(void);
static int TY_TUPLE(void);
static int TY_NAMED(void);
static int TY_VAR(void);
static int TY_UNKNOWN(void);
static int ast_type_name_to_tag(const char* name);
static int is_primitive_type_name(const char* name);
static int int_width_from_name(const char* name);
static int ast_ty_to_types_tag(const char* name);
static int AL_VAR(void);
static int AL_FIELD(void);
static int AL_INVALID(void);
static int classify_assign_lhs(int expr_tag);
static int stmt_returns(int stmt_tag, int inner_expr_tag);
static int last_stmt_returns(int count, int last_stmt_tag, int last_expr_tag);
static const char* path_key_2(const char* a, const char* b);
static const char* path_key_3(const char* a, const char* b, const char* c);
static const char* path_key_vec(const char* source, int starts, int ends, int segs);
static int is_contract_param(int params, int name);
static int struct_names_count(int struct_table, int stride);
static int struct_name_at(int struct_table, int stride, int idx);
static int enum_names_count(int enum_table, int stride);
static int enum_name_at(int enum_table, int stride, int idx);
static int name_in_table(int table, int stride, int name);
static int alias_lookup(int names, int targets, int name);
static int canon_ty_import(int names, int targets, int name);
static int env_lookup(int env_names, int env_types, int name);
static int env_extend(int env_names, int env_types, int env_muts, int name, int ty, int is_mut);
static int env_is_mut(int env_muts, int idx);
static int VS_UNIT(void);
static int VS_TUPLE(void);
static int VS_STRUCT(void);
static int ctor_arity(int shape, int field_count);
static int ctor_is_unit(int shape);
static int ctor_is_tuple(int shape);
static int is_result_enum(int has_ok, int has_err);
static int is_option_enum(int has_some, int has_none);
static int try_success_shape_valid(int shape, int field_count);
static const char* error_type_mismatch(void);
static const char* error_unbound_value(void);
static const char* error_stmt_no_effect(void);
static const char* error_unknown_type(void);
static const char* error_no_return_type(void);
static const char* error_duplicate_struct_enum(void);
static const char* error_unknown_struct(void);
static const char* error_unknown_variant(void);
static const char* error_unknown_enum(void);
static const char* error_enum_tuple_arity(void);
static const char* error_unknown_field(void);
static const char* error_try_payload(void);
static const char* error_immutable_assign(void);
static const char* error_bad_assign_lhs(void);
static int struct_field_index(int field_names, int name);
static int struct_has_field(int field_names, int name);
static int expect(int got_tag, int want_tag);
static int fields_of_struct(int struct_names, int struct_stride, int target_name);
static int variant_shape_of(int enum_names, int enum_stride, int enum_name, int variant_name, int variant_tags, int variant_enum_ids);
static int assign_lhs_bad(void);
static int in_loop(int depth);
static int enter_loop(int depth);
static int exit_loop(int depth);
static int ctor_ty_arity(int shape, int field_count);
static int struct_field_ty(int field_names, int field_types, int name);
static int infer_lit(int lit_tag);
static int dup_variant_names(int names);
static int struct_enum_name_clash(int struct_names, int enum_names);
static int named_only_ty(int struct_names, int enum_names, int name);
static int fields_of_struct_count(int struct_names, int struct_field_counts, int name);
static int stmt_returns_arena(int et, int ed1, int idx);
static int last_stmt_returns_arena(int et, int ed1, int first_stmt, int stmt_count);
static const char* chk_tok_str(const char* source, int starts, int ends, int pos);
static int expr_references_result(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp);
static int type_param_push(int type_params, int new_names, int new_tags);
static int type_param_lookup(int type_params, int limit, int name);
static int resolve_field_ty_tag(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int pp, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int idx);
static int contract_expr_new(void);
static int contract_expr_enter(int flag);
static int contract_expr_exit(int flag, int saved);
static int contract_expr_active(int flag);
static int ty_import_alias_push(int aliases, int from_h, int to_h);
static int alias_lookup_bounded(int names, int targets, int limit, int name);
static int ctor_ty_tag(int shape, int param_count);
static int test_binop_class(void);
static int test_integral_ty(void);
static int test_ast_type_names(void);
static int test_assign_lhs(void);
static int test_stmt_returns(void);
static int test_path_key(void);
static int test_alias_lookup(void);
static int test_env_helpers(void);
static int test_variant_shape(void);
static int test_try_helpers(void);
static int test_error_codes(void);
static int test_struct_field(void);
static int test_loop_depth(void);
static int test_extra_classifiers(void);
static int test_path_key_vec(void);
static int test_contract_param(void);
static int test_struct_enum_names(void);
static int test_expect(void);
static int test_fields_of_struct(void);
static int test_variant_shape_of(void);
static int test_assign_lhs_bad(void);
static int test_ctor_ty_arity(void);
static int test_struct_field_ty(void);
static int test_infer_lit(void);
static int test_dup_variant_names(void);
static int test_struct_enum_clash(void);
static int test_named_only_ty(void);
static int test_fields_of_struct_count(void);
static int test_stmt_returns_arena(void);
static int test_last_stmt_returns_arena(void);
static int test_expr_references_result(void);
static int test_ast_ty_types_tag(void);
static int test_type_params(void);
static int test_resolve_field_ty(void);
static int test_contract_expr_flag(void);
static int test_ty_import_alias_push(void);
static int test_ctor_ty_tag(void);

static int BINOP_ARITH(void) {
  return 0;
}

static int BINOP_LOGIC(void) {
  return 1;
}

static int BINOP_CMP(void) {
  return 2;
}

static int binop_class(int tag) {
  if ((tag <= 4)) {
    return 0;
  } else {
  }
  if ((tag >= 5)) {
    if ((tag <= 10)) {
      return 2;
    } else {
    }
  } else {
  }
  if ((tag == 11)) {
    return 1;
  } else {
  }
  if ((tag == 12)) {
    return 1;
  } else {
  }
  if ((tag >= 13)) {
    if ((tag <= 17)) {
      return 0;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_arith_binop(int tag) {
  int _sv0t0 = binop_class(tag);
  int _sv0t1 = (_sv0t0 == 0);
  return _sv0t1;
}

static int is_logic_binop(int tag) {
  int _sv0t0 = binop_class(tag);
  int _sv0t1 = (_sv0t0 == 1);
  return _sv0t1;
}

static int is_cmp_binop(int tag) {
  int _sv0t0 = binop_class(tag);
  int _sv0t1 = (_sv0t0 == 2);
  return _sv0t1;
}

static int binop_returns_bool(int tag) {
  int _sv0t0 = is_cmp_binop(tag);
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = is_logic_binop(tag);
  return _sv0t1;
}

static int is_integral_ty(int ty_tag) {
  if ((ty_tag == 0)) {
    return 1;
  } else {
  }
  if ((ty_tag == 1)) {
    return 1;
  } else {
  }
  if ((ty_tag == 8)) {
    return 1;
  } else {
  }
  if ((ty_tag == 9)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_bool_ty(int ty_tag) {
  int _sv0t0 = (ty_tag == 3);
  return _sv0t0;
}

static int is_unit_ty(int ty_tag) {
  int _sv0t0 = (ty_tag == 2);
  return _sv0t0;
}

static int is_string_ty(int ty_tag) {
  int _sv0t0 = (ty_tag == 5);
  return _sv0t0;
}

static int is_numeric_ty(int ty_tag) {
  int _sv0t0 = is_integral_ty(ty_tag);
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = (ty_tag == 10);
  return _sv0t1;
}

static int assign_lhs_is_var(int class) {
  int _sv0t0 = (class == 0);
  return _sv0t0;
}

static int assign_lhs_is_field(int class) {
  int _sv0t0 = (class == 1);
  return _sv0t0;
}

static int TY_UNIT(void) {
  return 2;
}

static int TY_BOOL(void) {
  return 3;
}

static int TY_CHAR(void) {
  return 4;
}

static int TY_INT(void) {
  return 0;
}

static int TY_UINT(void) {
  return 1;
}

static int TY_ISIZE(void) {
  return 8;
}

static int TY_USIZE(void) {
  return 9;
}

static int TY_FLOAT(void) {
  return 10;
}

static int TY_STRING(void) {
  return 5;
}

static int TY_STRUCT(void) {
  return 6;
}

static int TY_ENUM(void) {
  return 7;
}

static int TY_FN(void) {
  return 11;
}

static int TY_REF(void) {
  return 12;
}

static int TY_REFMUT(void) {
  return 13;
}

static int TY_TUPLE(void) {
  return 14;
}

static int TY_NAMED(void) {
  return 15;
}

static int TY_VAR(void) {
  return 16;
}

static int TY_UNKNOWN(void) {
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int ast_type_name_to_tag(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "unit");
  if (_sv0t0) {
    return 2;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "bool");
  if (_sv0t1) {
    return 3;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "char");
  if (_sv0t2) {
    return 4;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "i32");
  if (_sv0t3) {
    return 0;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "u32");
  if (_sv0t4) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "i64");
  if (_sv0t5) {
    return 0;
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "u64");
  if (_sv0t6) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "i128");
  if (_sv0t7) {
    return 0;
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "u128");
  if (_sv0t8) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "i8");
  if (_sv0t9) {
    return 0;
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "u8");
  if (_sv0t10) {
    return 1;
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "i16");
  if (_sv0t11) {
    return 0;
  } else {
  }
  int _sv0t12 = sv0_string_eq(name, "u16");
  if (_sv0t12) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_string_eq(name, "isize");
  if (_sv0t13) {
    return 8;
  } else {
  }
  int _sv0t14 = sv0_string_eq(name, "usize");
  if (_sv0t14) {
    return 9;
  } else {
  }
  int _sv0t15 = sv0_string_eq(name, "f32");
  if (_sv0t15) {
    return 10;
  } else {
  }
  int _sv0t16 = sv0_string_eq(name, "f64");
  if (_sv0t16) {
    return 10;
  } else {
  }
  int _sv0t17 = sv0_string_eq(name, "str");
  if (_sv0t17) {
    return 5;
  } else {
  }
  int _sv0t18 = sv0_string_eq(name, "string");
  if (_sv0t18) {
    return 5;
  } else {
  }
  int _sv0t19 = sv0_string_eq(name, "String");
  if (_sv0t19) {
    return 5;
  } else {
  }
  int _sv0t20 = (0 - 1);
  return _sv0t20;
}

static int is_primitive_type_name(const char* name) {
  int _sv0t0 = ast_type_name_to_tag(name);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int int_width_from_name(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "i8");
  if (_sv0t0) {
    return 8;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "u8");
  if (_sv0t1) {
    return 8;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "i16");
  if (_sv0t2) {
    return 16;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "u16");
  if (_sv0t3) {
    return 16;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "i32");
  if (_sv0t4) {
    return 32;
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "u32");
  if (_sv0t5) {
    return 32;
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "i64");
  if (_sv0t6) {
    return 64;
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "u64");
  if (_sv0t7) {
    return 64;
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "i128");
  if (_sv0t8) {
    return 128;
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "u128");
  if (_sv0t9) {
    return 128;
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "f32");
  if (_sv0t10) {
    return 32;
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "f64");
  if (_sv0t11) {
    return 64;
  } else {
  }
  return 0;
}

static int ast_ty_to_types_tag(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "unit");
  if (_sv0t0) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "bool");
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "char");
  if (_sv0t2) {
    return 2;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "str");
  if (_sv0t3) {
    return 3;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "string");
  if (_sv0t4) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "String");
  if (_sv0t5) {
    return 3;
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "isize");
  if (_sv0t6) {
    return 4;
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "usize");
  if (_sv0t7) {
    return 5;
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "i8");
  if (_sv0t8) {
    return 7;
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "i16");
  if (_sv0t9) {
    return 7;
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "i32");
  if (_sv0t10) {
    return 7;
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "i64");
  if (_sv0t11) {
    return 7;
  } else {
  }
  int _sv0t12 = sv0_string_eq(name, "i128");
  if (_sv0t12) {
    return 7;
  } else {
  }
  int _sv0t13 = sv0_string_eq(name, "u8");
  if (_sv0t13) {
    return 8;
  } else {
  }
  int _sv0t14 = sv0_string_eq(name, "u16");
  if (_sv0t14) {
    return 8;
  } else {
  }
  int _sv0t15 = sv0_string_eq(name, "u32");
  if (_sv0t15) {
    return 8;
  } else {
  }
  int _sv0t16 = sv0_string_eq(name, "u64");
  if (_sv0t16) {
    return 8;
  } else {
  }
  int _sv0t17 = sv0_string_eq(name, "u128");
  if (_sv0t17) {
    return 8;
  } else {
  }
  int _sv0t18 = sv0_string_eq(name, "f32");
  if (_sv0t18) {
    return 9;
  } else {
  }
  int _sv0t19 = sv0_string_eq(name, "f64");
  if (_sv0t19) {
    return 9;
  } else {
  }
  int _sv0t20 = (0 - 1);
  return _sv0t20;
}

static int AL_VAR(void) {
  return 0;
}

static int AL_FIELD(void) {
  return 1;
}

static int AL_INVALID(void) {
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int classify_assign_lhs(int expr_tag) {
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

static int stmt_returns(int stmt_tag, int inner_expr_tag) {
  if ((stmt_tag == 0)) {
    if ((inner_expr_tag == 14)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((stmt_tag == 1)) {
    if ((inner_expr_tag == 14)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int last_stmt_returns(int count, int last_stmt_tag, int last_expr_tag) {
  if ((count == 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = stmt_returns(last_stmt_tag, last_expr_tag);
  return _sv0t0;
}

static const char* path_key_2(const char* a, const char* b) {
  const char* _sv0t0 = sv0_string_concat(a, "::");
  const char* _sv0t1 = sv0_string_concat(_sv0t0, b);
  return _sv0t1;
}

static const char* path_key_3(const char* a, const char* b, const char* c) {
  const char* _sv0t0 = path_key_2(a, b);
  const char* _sv0t1 = sv0_string_concat("::", c);
  const char* _sv0t2 = sv0_string_concat(_sv0t0, _sv0t1);
  return _sv0t2;
}

static const char* path_key_vec(const char* source, int starts, int ends, int segs) {
  int _sv0t0 = sv0_vec_len(segs);
  int n = _sv0t0;
  if ((n == 0)) {
    return "";
  } else {
  }
  int _sv0t1 = sv0_vec_get(segs, 0);
  int p0 = _sv0t1;
  int _sv0t2 = sv0_vec_get(starts, p0);
  int s0 = _sv0t2;
  int _sv0t3 = sv0_vec_get(ends, p0);
  int e0 = _sv0t3;
  int _sv0t4 = (e0 - s0);
  const char* _sv0t5 = sv0_string_substr(source, s0, _sv0t4);
  const char* out;
  out = _sv0t5;
  int i = 1;
  while ((i < n)) {
    const char* _sv0t6 = sv0_string_concat(out, "::");
    out = _sv0t6;
    int _sv0t7 = sv0_vec_get(segs, i);
    int p = _sv0t7;
    int _sv0t8 = sv0_vec_get(starts, p);
    int s = _sv0t8;
    int _sv0t9 = sv0_vec_get(ends, p);
    int e = _sv0t9;
    int _sv0t10 = (e - s);
    const char* _sv0t11 = sv0_string_substr(source, s, _sv0t10);
    const char* _sv0t12 = sv0_string_concat(out, _sv0t11);
    out = _sv0t12;
    i = (i + 1);
  }
  return out;
}

static int is_contract_param(int params, int name) {
  int _sv0t0 = sv0_vec_len(params);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(params, i);
    if ((_sv0t1 == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int struct_names_count(int struct_table, int stride) {
  int _sv0t0 = sv0_vec_len(struct_table);
  int _sv0t1 = (_sv0t0 / stride);
  return _sv0t1;
}

static int struct_name_at(int struct_table, int stride, int idx) {
  int _sv0t0 = (idx * stride);
  int _sv0t1 = sv0_vec_get(struct_table, _sv0t0);
  return _sv0t1;
}

static int enum_names_count(int enum_table, int stride) {
  int _sv0t0 = sv0_vec_len(enum_table);
  int _sv0t1 = (_sv0t0 / stride);
  return _sv0t1;
}

static int enum_name_at(int enum_table, int stride, int idx) {
  int _sv0t0 = (idx * stride);
  int _sv0t1 = sv0_vec_get(enum_table, _sv0t0);
  return _sv0t1;
}

static int name_in_table(int table, int stride, int name) {
  int _sv0t0 = sv0_vec_len(table);
  int n = (_sv0t0 / stride);
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = (i * stride);
    int _sv0t2 = sv0_vec_get(table, _sv0t1);
    if ((_sv0t2 == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int alias_lookup(int names, int targets, int name) {
  int _sv0t0 = sv0_vec_len(names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(names, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = sv0_vec_get(targets, i);
      return _sv0t2;
    } else {
    }
    i = (i + 1);
  }
  return name;
}

static int canon_ty_import(int names, int targets, int name) {
  int _sv0t0 = alias_lookup(names, targets, name);
  int resolved = _sv0t0;
  if ((resolved == name)) {
    return name;
  } else {
  }
  int _sv0t1 = canon_ty_import(names, targets, resolved);
  return _sv0t1;
}

static int env_lookup(int env_names, int env_types, int name) {
  int _sv0t0 = sv0_vec_len(env_names);
  int n = _sv0t0;
  int i = (n - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(env_names, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = sv0_vec_get(env_types, i);
      return _sv0t2;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int env_extend(int env_names, int env_types, int env_muts, int name, int ty, int is_mut) {
  sv0_vec_push(env_names, name);
  sv0_vec_push(env_types, ty);
  sv0_vec_push(env_muts, is_mut);
  int _sv0t0 = sv0_vec_len(env_names);
  int _sv0t1 = (_sv0t0 - 1);
  return _sv0t1;
}

static int env_is_mut(int env_muts, int idx) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(env_muts, idx);
  int _sv0t1 = (_sv0t0 != 0);
  return _sv0t1;
}

static int VS_UNIT(void) {
  return 0;
}

static int VS_TUPLE(void) {
  return 1;
}

static int VS_STRUCT(void) {
  return 2;
}

static int ctor_arity(int shape, int field_count) {
  if ((shape == 0)) {
    return 0;
  } else {
  }
  return field_count;
}

static int ctor_is_unit(int shape) {
  int _sv0t0 = (shape == 0);
  return _sv0t0;
}

static int ctor_is_tuple(int shape) {
  int _sv0t0 = (shape == 1);
  return _sv0t0;
}

static int is_result_enum(int has_ok, int has_err) {
  if (has_ok) {
    if (has_err) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_option_enum(int has_some, int has_none) {
  if (has_some) {
    if (has_none) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int try_success_shape_valid(int shape, int field_count) {
  if ((shape == 1)) {
    if ((field_count == 1)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static const char* error_type_mismatch(void) {
  return "E0400";
}

static const char* error_unbound_value(void) {
  return "E0401";
}

static const char* error_stmt_no_effect(void) {
  return "E0405";
}

static const char* error_unknown_type(void) {
  return "E0406";
}

static const char* error_no_return_type(void) {
  return "E0409";
}

static const char* error_duplicate_struct_enum(void) {
  return "E0417";
}

static const char* error_unknown_struct(void) {
  return "E0420";
}

static const char* error_unknown_variant(void) {
  return "E0421";
}

static const char* error_unknown_enum(void) {
  return "E0422";
}

static const char* error_enum_tuple_arity(void) {
  return "E0426";
}

static const char* error_unknown_field(void) {
  return "E0429";
}

static const char* error_try_payload(void) {
  return "E0441";
}

static const char* error_immutable_assign(void) {
  return "E0448";
}

static const char* error_bad_assign_lhs(void) {
  return "E0449";
}

static int struct_field_index(int field_names, int name) {
  int _sv0t0 = sv0_vec_len(field_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(field_names, i);
    if ((_sv0t1 == name)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int struct_has_field(int field_names, int name) {
  int _sv0t0 = struct_field_index(field_names, name);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int expect(int got_tag, int want_tag) {
  int _sv0t0 = (got_tag == want_tag);
  return _sv0t0;
}

static int fields_of_struct(int struct_names, int struct_stride, int target_name) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int n = (_sv0t0 / struct_stride);
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = (i * struct_stride);
    int _sv0t2 = sv0_vec_get(struct_names, _sv0t1);
    if ((_sv0t2 == target_name)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int variant_shape_of(int enum_names, int enum_stride, int enum_name, int variant_name, int variant_tags, int variant_enum_ids) {
  int _sv0t0 = sv0_vec_len(enum_names);
  int ne = (_sv0t0 / enum_stride);
  int found_enum = 0;
  int i = 0;
  while ((i < ne)) {
    int _sv0t1 = (i * enum_stride);
    int _sv0t2 = sv0_vec_get(enum_names, _sv0t1);
    if ((_sv0t2 == enum_name)) {
      found_enum = 1;
      i = ne;
    } else {
    }
    i = (i + 1);
  }
  if ((found_enum == 0)) {
    int _sv0t3 = (0 - 2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = sv0_vec_len(variant_tags);
  int nv = _sv0t4;
  int j = 0;
  while ((j < nv)) {
    int _sv0t5 = sv0_vec_get(variant_enum_ids, j);
    if ((_sv0t5 == enum_name)) {
      int _sv0t6 = sv0_vec_get(variant_tags, j);
      if ((_sv0t6 == variant_name)) {
        return j;
      } else {
      }
    } else {
    }
    j = (j + 1);
  }
  int _sv0t7 = (0 - 1);
  return _sv0t7;
}

static int assign_lhs_bad(void) {
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int in_loop(int depth) {
  int _sv0t0 = (depth > 0);
  return _sv0t0;
}

static int enter_loop(int depth) {
  int _sv0t0 = (depth + 1);
  return _sv0t0;
}

static int exit_loop(int depth) {
  if ((depth <= 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = (depth - 1);
  return _sv0t0;
}

static int ctor_ty_arity(int shape, int field_count) {
  if ((shape == 0)) {
    return 0;
  } else {
  }
  return field_count;
}

static int struct_field_ty(int field_names, int field_types, int name) {
  int _sv0t0 = struct_field_index(field_names, name);
  int idx = _sv0t0;
  if ((idx < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(field_types, idx);
  return _sv0t2;
}

static int infer_lit(int lit_tag) {
  if ((lit_tag == 0)) {
    return 0;
  } else {
  }
  if ((lit_tag == 1)) {
    return 2;
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
  if ((lit_tag == 4)) {
    return 1;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int dup_variant_names(int names) {
  int _sv0t0 = sv0_vec_len(names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int j = 0;
    while ((j < i)) {
      int _sv0t1 = sv0_vec_get(names, j);
      int _sv0t2 = sv0_vec_get(names, i);
      if ((_sv0t1 == _sv0t2)) {
        return i;
      } else {
      }
      j = (j + 1);
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int struct_enum_name_clash(int struct_names, int enum_names) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_len(enum_names);
  int en = _sv0t1;
  int i = 0;
  while ((i < sn)) {
    int _sv0t2 = sv0_vec_get(struct_names, i);
    int s = _sv0t2;
    int j = 0;
    while ((j < en)) {
      int _sv0t3 = sv0_vec_get(enum_names, j);
      if ((_sv0t3 == s)) {
        return s;
      } else {
      }
      j = (j + 1);
    }
    i = (i + 1);
  }
  int _sv0t4 = (0 - 1);
  return _sv0t4;
}

static int named_only_ty(int struct_names, int enum_names, int name) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int sn = _sv0t0;
  int i = 0;
  while ((i < sn)) {
    int _sv0t1 = sv0_vec_get(struct_names, i);
    if ((_sv0t1 == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = sv0_vec_len(enum_names);
  int en = _sv0t2;
  int j = 0;
  while ((j < en)) {
    int _sv0t3 = sv0_vec_get(enum_names, j);
    if ((_sv0t3 == name)) {
      return 2;
    } else {
    }
    j = (j + 1);
  }
  return 0;
}

static int fields_of_struct_count(int struct_names, int struct_field_counts, int name) {
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
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int stmt_returns_arena(int et, int ed1, int idx) {
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 28)) {
    int _sv0t1 = sv0_vec_get(ed1, idx);
    int inner = _sv0t1;
    if ((inner >= 0)) {
      int _sv0t2 = sv0_vec_get(et, inner);
      int _sv0t3 = (_sv0t2 == 15);
      return _sv0t3;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 15)) {
    return 1;
  } else {
  }
  return 0;
}

static int last_stmt_returns_arena(int et, int ed1, int first_stmt, int stmt_count) {
  if ((stmt_count == 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = (first_stmt + stmt_count);
  int _sv0t1 = (_sv0t0 - 1);
  int _sv0t2 = stmt_returns_arena(et, ed1, _sv0t1);
  return _sv0t2;
}

static const char* chk_tok_str(const char* source, int starts, int ends, int pos) {
  int _sv0t0 = sv0_vec_get(starts, pos);
  int s = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, pos);
  int e = _sv0t1;
  int _sv0t2 = (e - s);
  const char* _sv0t3 = sv0_string_substr(source, s, _sv0t2);
  return _sv0t3;
}

static int expr_references_result(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp) {
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
    if ((ppc == 1)) {
      int _sv0t3 = sv0_vec_get(pp, pps);
      int tok = _sv0t3;
      const char* _sv0t4 = chk_tok_str(source, starts, ends, tok);
      const char* name;
      name = _sv0t4;
      int _sv0t5 = sv0_string_eq(name, "result");
      return _sv0t5;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t6 = sv0_vec_get(ed2, idx);
    int _sv0t7 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t6, source, starts, ends, pp);
    return _sv0t7;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t8 = sv0_vec_get(ed2, idx);
    int _sv0t9 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t8, source, starts, ends, pp);
    if (_sv0t9) {
      return 1;
    } else {
    }
    int _sv0t10 = sv0_vec_get(ed3, idx);
    int _sv0t11 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t10, source, starts, ends, pp);
    return _sv0t11;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t12 = sv0_vec_get(ed1, idx);
    int callee = _sv0t12;
    int _sv0t13 = expr_references_result(et, ed1, ed2, ed3, ed4, callee, source, starts, ends, pp);
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
      int _sv0t17 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t16, source, starts, ends, pp);
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
    int _sv0t19 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t18, source, starts, ends, pp);
    return _sv0t19;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t20 = sv0_vec_get(ed1, idx);
    int _sv0t21 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t20, source, starts, ends, pp);
    if (_sv0t21) {
      return 1;
    } else {
    }
    int _sv0t22 = sv0_vec_get(ed2, idx);
    int _sv0t23 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t22, source, starts, ends, pp);
    if (_sv0t23) {
      return 1;
    } else {
    }
    int _sv0t24 = sv0_vec_get(ed3, idx);
    int else_idx = _sv0t24;
    if ((else_idx >= 0)) {
      int _sv0t25 = expr_references_result(et, ed1, ed2, ed3, ed4, else_idx, source, starts, ends, pp);
      return _sv0t25;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t26 = sv0_vec_get(ed1, idx);
    int scrut = _sv0t26;
    int _sv0t27 = expr_references_result(et, ed1, ed2, ed3, ed4, scrut, source, starts, ends, pp);
    if (_sv0t27) {
      return 1;
    } else {
    }
    int _sv0t28 = sv0_vec_get(ed2, idx);
    int arms_f = _sv0t28;
    int _sv0t29 = sv0_vec_get(ed3, idx);
    int arms_c = _sv0t29;
    int ami = 0;
    while ((ami < arms_c)) {
      int arm_idx = (arms_f + ami);
      int _sv0t30 = sv0_vec_get(ed3, arm_idx);
      int guard = _sv0t30;
      int _sv0t31 = sv0_vec_get(ed4, arm_idx);
      int body = _sv0t31;
      if ((guard >= 0)) {
        int _sv0t32 = expr_references_result(et, ed1, ed2, ed3, ed4, guard, source, starts, ends, pp);
        if (_sv0t32) {
          return 1;
        } else {
        }
      } else {
      }
      int _sv0t33 = expr_references_result(et, ed1, ed2, ed3, ed4, body, source, starts, ends, pp);
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
    int _sv0t35 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t34, source, starts, ends, pp);
    if (_sv0t35) {
      return 1;
    } else {
    }
    int _sv0t36 = sv0_vec_get(ed3, idx);
    int inv_f = _sv0t36;
    int _sv0t37 = sv0_vec_get(ed4, idx);
    int inv_c = _sv0t37;
    int ii = 0;
    while ((ii < inv_c)) {
      int _sv0t38 = (inv_f + ii);
      int _sv0t39 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t38, source, starts, ends, pp);
      if (_sv0t39) {
        return 1;
      } else {
      }
      ii = (ii + 1);
    }
    int _sv0t40 = sv0_vec_get(ed2, idx);
    int _sv0t41 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t40, source, starts, ends, pp);
    return _sv0t41;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t42 = sv0_vec_get(ed2, idx);
    int _sv0t43 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t42, source, starts, ends, pp);
    if (_sv0t43) {
      return 1;
    } else {
    }
    int _sv0t44 = sv0_vec_get(ed3, idx);
    int _sv0t45 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t44, source, starts, ends, pp);
    return _sv0t45;
  } else {
  }
  if ((tag == 14)) {
    int _sv0t46 = sv0_vec_get(ed1, idx);
    int _sv0t47 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t46, source, starts, ends, pp);
    return _sv0t47;
  } else {
  }
  if ((tag == 16)) {
    int _sv0t48 = sv0_vec_get(ed1, idx);
    int bv = _sv0t48;
    if ((bv >= 0)) {
      int _sv0t49 = expr_references_result(et, ed1, ed2, ed3, ed4, bv, source, starts, ends, pp);
      return _sv0t49;
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
    int _sv0t50 = sv0_vec_get(ed1, idx);
    int _sv0t51 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t50, source, starts, ends, pp);
    if (_sv0t51) {
      return 1;
    } else {
    }
    int _sv0t52 = sv0_vec_get(ed2, idx);
    int _sv0t53 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t52, source, starts, ends, pp);
    return _sv0t53;
  } else {
  }
  if ((tag == 19)) {
    int _sv0t54 = sv0_vec_get(ed2, idx);
    int _sv0t55 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t54, source, starts, ends, pp);
    if (_sv0t55) {
      return 1;
    } else {
    }
    int _sv0t56 = sv0_vec_get(ed3, idx);
    int _sv0t57 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t56, source, starts, ends, pp);
    return _sv0t57;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t58 = sv0_vec_get(ed1, idx);
    int _sv0t59 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t58, source, starts, ends, pp);
    return _sv0t59;
  } else {
  }
  if ((tag == 22)) {
    int _sv0t60 = sv0_vec_get(ed1, idx);
    int _sv0t61 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t60, source, starts, ends, pp);
    return _sv0t61;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t62 = sv0_vec_get(ed3, idx);
    int sfc = _sv0t62;
    int sfi = 0;
    while ((sfi < sfc)) {
      int _sv0t63 = (idx - sfc);
      int _sv0t64 = (_sv0t63 + sfi);
      int _sv0t65 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t64, source, starts, ends, pp);
      if (_sv0t65) {
        return 1;
      } else {
      }
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
      int _sv0t69 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t68, source, starts, ends, pp);
      if (_sv0t69) {
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

static int type_param_push(int type_params, int new_names, int new_tags) {
  int _sv0t0 = sv0_vec_len(type_params);
  int saved = _sv0t0;
  int _sv0t1 = sv0_vec_len(new_names);
  int count = _sv0t1;
  int i = 0;
  while ((i < count)) {
    int _sv0t2 = sv0_vec_get(new_names, i);
    sv0_vec_push(type_params, _sv0t2);
    int _sv0t3 = sv0_vec_get(new_tags, i);
    sv0_vec_push(type_params, _sv0t3);
    i = (i + 1);
  }
  return saved;
}

static int type_param_lookup(int type_params, int limit, int name) {
  int i = (limit - 2);
  while ((i >= 0)) {
    int _sv0t0 = sv0_vec_get(type_params, i);
    if ((_sv0t0 == name)) {
      int _sv0t1 = (i + 1);
      int _sv0t2 = sv0_vec_get(type_params, _sv0t1);
      return _sv0t2;
    } else {
    }
    i = (i - 2);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int resolve_field_ty_tag(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int pp, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int idx) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(ty_tags, idx);
  int tag = _sv0t1;
  if ((tag == 6)) {
    int _sv0t2 = TY_UNIT();
    return _sv0t2;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t3 = TY_REF();
    return _sv0t3;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t4 = TY_REFMUT();
    return _sv0t4;
  } else {
  }
  if ((tag == 7)) {
    int _sv0t5 = TY_TUPLE();
    return _sv0t5;
  } else {
  }
  if ((tag == 0)) {
    int _sv0t6 = sv0_vec_get(ty_d2, idx);
    int seg_count = _sv0t6;
    if ((seg_count == 1)) {
      int _sv0t7 = sv0_vec_get(ty_d1, idx);
      int pps = _sv0t7;
      int _sv0t8 = sv0_vec_get(pp, pps);
      int tok = _sv0t8;
      int _sv0t9 = sv0_vec_get(starts, tok);
      int s = _sv0t9;
      int _sv0t10 = sv0_vec_get(ends, tok);
      int e = _sv0t10;
      int _sv0t11 = (e - s);
      const char* _sv0t12 = sv0_string_substr(source, s, _sv0t11);
      const char* name;
      name = _sv0t12;
      int _sv0t13 = ast_ty_to_types_tag(name);
      int prim = _sv0t13;
      if ((prim >= 0)) {
        return prim;
      } else {
      }
      int ni = 0;
      int _sv0t14 = sv0_vec_len(struct_names);
      while ((ni < _sv0t14)) {
        int _sv0t15 = sv0_vec_get(struct_names, ni);
        if ((_sv0t15 == tok)) {
          int _sv0t16 = TY_STRUCT();
          return _sv0t16;
        } else {
        }
        ni = (ni + 1);
      }
      int ei = 0;
      int _sv0t17 = sv0_vec_len(enum_names);
      while ((ei < _sv0t17)) {
        int _sv0t18 = sv0_vec_get(enum_names, ei);
        if ((_sv0t18 == tok)) {
          int _sv0t19 = TY_ENUM();
          return _sv0t19;
        } else {
        }
        ei = (ei + 1);
      }
      int _sv0t20 = type_param_lookup(type_params, tp_limit, tok);
      int tp = _sv0t20;
      if ((tp >= 0)) {
        int _sv0t21 = TY_VAR();
        return _sv0t21;
      } else {
      }
      int _sv0t22 = (0 - 1);
      return _sv0t22;
    } else {
    }
    int _sv0t23 = (0 - 1);
    return _sv0t23;
  } else {
  }
  int _sv0t24 = (0 - 1);
  return _sv0t24;
}

static int contract_expr_new(void) {
  int _sv0t0 = sv0_vec_new();
  int v = _sv0t0;
  sv0_vec_push(v, 0);
  return v;
}

static int contract_expr_enter(int flag) {
  int _sv0t0 = sv0_vec_get(flag, 0);
  int saved = _sv0t0;
  sv0_vec_push(flag, 1);
  return saved;
}

static int contract_expr_exit(int flag, int saved) {
  sv0_vec_push(flag, saved);
  return 0;
}

static int contract_expr_active(int flag) {
  int _sv0t0 = sv0_vec_len(flag);
  int n = _sv0t0;
  int _sv0t1 = (n - 1);
  int _sv0t2 = sv0_vec_get(flag, _sv0t1);
  int _sv0t3 = (_sv0t2 == 1);
  return _sv0t3;
}

static int ty_import_alias_push(int aliases, int from_h, int to_h) {
  int _sv0t0 = sv0_vec_len(aliases);
  int saved = _sv0t0;
  sv0_vec_push(aliases, from_h);
  sv0_vec_push(aliases, to_h);
  return saved;
}

static int alias_lookup_bounded(int names, int targets, int limit, int name) {
  int n = limit;
  int i = 0;
  while ((i < n)) {
    int _sv0t0 = sv0_vec_get(names, i);
    if ((_sv0t0 == name)) {
      int _sv0t1 = sv0_vec_get(targets, i);
      return _sv0t1;
    } else {
    }
    i = (i + 1);
  }
  return name;
}

static int ctor_ty_tag(int shape, int param_count) {
  if ((shape == 0)) {
    int _sv0t0 = TY_ENUM();
    return _sv0t0;
  } else {
  }
  if ((param_count > 0)) {
    int _sv0t1 = TY_FN();
    return _sv0t1;
  } else {
  }
  int _sv0t2 = TY_ENUM();
  return _sv0t2;
}

static int test_binop_class(void) {
  int _sv0t0 = binop_class(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = binop_class(1);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = binop_class(4);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = binop_class(5);
  if ((_sv0t3 != 2)) {
    return 4;
  } else {
  }
  int _sv0t4 = binop_class(10);
  if ((_sv0t4 != 2)) {
    return 5;
  } else {
  }
  int _sv0t5 = binop_class(11);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = binop_class(12);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = binop_class(13);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = binop_class(17);
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  int _sv0t9 = is_arith_binop(0);
  if ((_sv0t9 != 1)) {
    return 10;
  } else {
  }
  int _sv0t10 = is_logic_binop(11);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = is_cmp_binop(5);
  if ((_sv0t11 != 1)) {
    return 12;
  } else {
  }
  return 0;
}

static int test_integral_ty(void) {
  int _sv0t0 = is_integral_ty(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_integral_ty(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_integral_ty(8);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_integral_ty(9);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_integral_ty(3);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_integral_ty(5);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_ast_type_names(void) {
  int _sv0t0 = ast_type_name_to_tag("i32");
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = ast_type_name_to_tag("u32");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = ast_type_name_to_tag("bool");
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = ast_type_name_to_tag("unit");
  if ((_sv0t3 != 2)) {
    return 4;
  } else {
  }
  int _sv0t4 = ast_type_name_to_tag("string");
  if ((_sv0t4 != 5)) {
    return 5;
  } else {
  }
  int _sv0t5 = ast_type_name_to_tag("String");
  if ((_sv0t5 != 5)) {
    return 6;
  } else {
  }
  int _sv0t6 = ast_type_name_to_tag("isize");
  if ((_sv0t6 != 8)) {
    return 7;
  } else {
  }
  int _sv0t7 = ast_type_name_to_tag("f64");
  if ((_sv0t7 != 10)) {
    return 8;
  } else {
  }
  int _sv0t8 = ast_type_name_to_tag("MyStruct");
  int _sv0t9 = (0 - 1);
  if ((_sv0t8 != _sv0t9)) {
    return 9;
  } else {
  }
  int _sv0t10 = is_primitive_type_name("i32");
  if ((_sv0t10 != 1)) {
    return 10;
  } else {
  }
  int _sv0t11 = is_primitive_type_name("Foo");
  if ((_sv0t11 != 0)) {
    return 11;
  } else {
  }
  int _sv0t12 = int_width_from_name("i32");
  if ((_sv0t12 != 32)) {
    return 12;
  } else {
  }
  int _sv0t13 = int_width_from_name("u8");
  if ((_sv0t13 != 8)) {
    return 13;
  } else {
  }
  int _sv0t14 = int_width_from_name("f64");
  if ((_sv0t14 != 64)) {
    return 14;
  } else {
  }
  int _sv0t15 = int_width_from_name("bool");
  if ((_sv0t15 != 0)) {
    return 15;
  } else {
  }
  return 0;
}

static int test_assign_lhs(void) {
  int _sv0t0 = classify_assign_lhs(2);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = classify_assign_lhs(11);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = classify_assign_lhs(99);
  int _sv0t3 = (0 - 1);
  if ((_sv0t2 != _sv0t3)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_stmt_returns(void) {
  int _sv0t0 = stmt_returns(0, 14);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = stmt_returns(1, 14);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = stmt_returns(0, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = last_stmt_returns(0, 0, 0);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = last_stmt_returns(1, 0, 14);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_path_key(void) {
  const char* _sv0t0 = path_key_2("Foo", "bar");
  const char* r;
  r = _sv0t0;
  int _sv0t1 = sv0_string_eq(r, "Foo::bar");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = path_key_3("a", "b", "c");
  const char* r3;
  r3 = _sv0t2;
  int _sv0t3 = sv0_string_eq(r3, "a::b::c");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_alias_lookup(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int targets = _sv0t1;
  sv0_vec_push(names, 10);
  sv0_vec_push(targets, 20);
  sv0_vec_push(names, 30);
  sv0_vec_push(targets, 40);
  int _sv0t2 = alias_lookup(names, targets, 10);
  if ((_sv0t2 != 20)) {
    return 1;
  } else {
  }
  int _sv0t3 = alias_lookup(names, targets, 30);
  if ((_sv0t3 != 40)) {
    return 2;
  } else {
  }
  int _sv0t4 = alias_lookup(names, targets, 99);
  if ((_sv0t4 != 99)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_env_helpers(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int et = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int em = _sv0t2;
  int _sv0t3 = env_extend(en, et, em, 10, 100, 0);
  int _sv0t4 = env_extend(en, et, em, 20, 200, 1);
  int _sv0t5 = env_lookup(en, et, 10);
  if ((_sv0t5 != 100)) {
    return 1;
  } else {
  }
  int _sv0t6 = env_lookup(en, et, 20);
  if ((_sv0t6 != 200)) {
    return 2;
  } else {
  }
  int _sv0t7 = env_lookup(en, et, 99);
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 3;
  } else {
  }
  int _sv0t9 = env_is_mut(em, 0);
  if ((_sv0t9 != 0)) {
    return 4;
  } else {
  }
  int _sv0t10 = env_is_mut(em, 1);
  if ((_sv0t10 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_variant_shape(void) {
  int _sv0t0 = ctor_arity(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = ctor_arity(1, 3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = ctor_arity(2, 2);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = ctor_is_unit(0);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = ctor_is_tuple(1);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_try_helpers(void) {
  int _sv0t0 = is_result_enum(1, 1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_result_enum(1, 0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_option_enum(1, 1);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = try_success_shape_valid(1, 1);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = try_success_shape_valid(0, 0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = try_success_shape_valid(1, 2);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_error_codes(void) {
  const char* _sv0t0 = error_type_mismatch();
  int _sv0t1 = sv0_string_eq(_sv0t0, "E0400");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = error_unbound_value();
  int _sv0t3 = sv0_string_eq(_sv0t2, "E0401");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = error_unknown_struct();
  int _sv0t5 = sv0_string_eq(_sv0t4, "E0420");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = error_bad_assign_lhs();
  int _sv0t7 = sv0_string_eq(_sv0t6, "E0449");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_struct_field(void) {
  int _sv0t0 = sv0_vec_new();
  int fnames = _sv0t0;
  sv0_vec_push(fnames, 10);
  sv0_vec_push(fnames, 20);
  sv0_vec_push(fnames, 30);
  int _sv0t1 = struct_field_index(fnames, 10);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = struct_field_index(fnames, 20);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = struct_field_index(fnames, 99);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 3;
  } else {
  }
  int _sv0t5 = struct_has_field(fnames, 20);
  if ((_sv0t5 != 1)) {
    return 4;
  } else {
  }
  int _sv0t6 = struct_has_field(fnames, 99);
  if ((_sv0t6 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_loop_depth(void) {
  int _sv0t0 = in_loop(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = in_loop(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = in_loop(3);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = enter_loop(0);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = enter_loop(2);
  if ((_sv0t4 != 3)) {
    return 5;
  } else {
  }
  int _sv0t5 = exit_loop(1);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = exit_loop(3);
  if ((_sv0t6 != 2)) {
    return 7;
  } else {
  }
  int _sv0t7 = exit_loop(0);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_extra_classifiers(void) {
  int _sv0t0 = binop_returns_bool(5);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = binop_returns_bool(11);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = binop_returns_bool(0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_bool_ty(3);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_bool_ty(0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_unit_ty(2);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_unit_ty(0);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_string_ty(5);
  if ((_sv0t7 != 1)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_string_ty(0);
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  int _sv0t9 = is_numeric_ty(0);
  if ((_sv0t9 != 1)) {
    return 10;
  } else {
  }
  int _sv0t10 = is_numeric_ty(10);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = is_numeric_ty(3);
  if ((_sv0t11 != 0)) {
    return 12;
  } else {
  }
  int _sv0t12 = assign_lhs_is_var(0);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  int _sv0t13 = assign_lhs_is_field(1);
  if ((_sv0t13 != 1)) {
    return 14;
  } else {
  }
  int _sv0t14 = assign_lhs_is_var(1);
  if ((_sv0t14 != 0)) {
    return 15;
  } else {
  }
  return 0;
}

static int test_path_key_vec(void) {
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
  int segs = _sv0t2;
  sv0_vec_push(segs, 0);
  sv0_vec_push(segs, 1);
  sv0_vec_push(segs, 2);
  const char* _sv0t3 = path_key_vec(src, starts, ends, segs);
  const char* k;
  k = _sv0t3;
  int _sv0t4 = sv0_string_eq(k, "Foo::Bar::Baz");
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int one = _sv0t5;
  sv0_vec_push(one, 0);
  const char* _sv0t6 = path_key_vec(src, starts, ends, one);
  int _sv0t7 = sv0_string_eq(_sv0t6, "Foo");
  if ((_sv0t7 != 1)) {
    return 2;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int empty = _sv0t8;
  const char* _sv0t9 = path_key_vec(src, starts, ends, empty);
  int _sv0t10 = sv0_string_eq(_sv0t9, "");
  if ((_sv0t10 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_contract_param(void) {
  int _sv0t0 = sv0_vec_new();
  int params = _sv0t0;
  sv0_vec_push(params, 10);
  sv0_vec_push(params, 20);
  sv0_vec_push(params, 30);
  int _sv0t1 = is_contract_param(params, 10);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = is_contract_param(params, 20);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = is_contract_param(params, 99);
  if ((_sv0t3 != 0)) {
    return 3;
  } else {
  }
  int _sv0t4 = sv0_vec_new();
  int empty = _sv0t4;
  int _sv0t5 = is_contract_param(empty, 10);
  if ((_sv0t5 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_struct_enum_names(void) {
  int _sv0t0 = sv0_vec_new();
  int tbl = _sv0t0;
  sv0_vec_push(tbl, 100);
  sv0_vec_push(tbl, 3);
  sv0_vec_push(tbl, 200);
  sv0_vec_push(tbl, 5);
  int _sv0t1 = struct_names_count(tbl, 2);
  if ((_sv0t1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t2 = struct_name_at(tbl, 2, 0);
  if ((_sv0t2 != 100)) {
    return 2;
  } else {
  }
  int _sv0t3 = struct_name_at(tbl, 2, 1);
  if ((_sv0t3 != 200)) {
    return 3;
  } else {
  }
  int _sv0t4 = name_in_table(tbl, 2, 100);
  if ((_sv0t4 != 1)) {
    return 4;
  } else {
  }
  int _sv0t5 = name_in_table(tbl, 2, 200);
  if ((_sv0t5 != 1)) {
    return 5;
  } else {
  }
  int _sv0t6 = name_in_table(tbl, 2, 999);
  if ((_sv0t6 != 0)) {
    return 6;
  } else {
  }
  int _sv0t7 = enum_names_count(tbl, 2);
  if ((_sv0t7 != 2)) {
    return 7;
  } else {
  }
  int _sv0t8 = enum_name_at(tbl, 2, 0);
  if ((_sv0t8 != 100)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_expect(void) {
  int _sv0t0 = expect(5, 5);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = expect(5, 6);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = expect(0, 0);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_fields_of_struct(void) {
  int _sv0t0 = sv0_vec_new();
  int tbl = _sv0t0;
  sv0_vec_push(tbl, 100);
  sv0_vec_push(tbl, 3);
  sv0_vec_push(tbl, 200);
  sv0_vec_push(tbl, 5);
  int _sv0t1 = fields_of_struct(tbl, 2, 100);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = fields_of_struct(tbl, 2, 200);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = fields_of_struct(tbl, 2, 999);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_variant_shape_of(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  sv0_vec_push(en, 10);
  sv0_vec_push(en, 0);
  int _sv0t1 = sv0_vec_new();
  int vtags = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int veids = _sv0t2;
  sv0_vec_push(vtags, 50);
  sv0_vec_push(veids, 10);
  sv0_vec_push(vtags, 60);
  sv0_vec_push(veids, 10);
  int _sv0t3 = variant_shape_of(en, 2, 10, 50, vtags, veids);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = variant_shape_of(en, 2, 10, 60, vtags, veids);
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  int _sv0t5 = variant_shape_of(en, 2, 10, 99, vtags, veids);
  int _sv0t6 = (0 - 1);
  if ((_sv0t5 != _sv0t6)) {
    return 3;
  } else {
  }
  int _sv0t7 = variant_shape_of(en, 2, 999, 50, vtags, veids);
  int _sv0t8 = (0 - 2);
  if ((_sv0t7 != _sv0t8)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_assign_lhs_bad(void) {
  int _sv0t0 = assign_lhs_bad();
  int _sv0t1 = (0 - 1);
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_ctor_ty_arity(void) {
  int _sv0t0 = ctor_ty_arity(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = ctor_ty_arity(0, 3);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = ctor_ty_arity(1, 3);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = ctor_ty_arity(2, 2);
  if ((_sv0t3 != 2)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_struct_field_ty(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int types = _sv0t1;
  sv0_vec_push(names, 10);
  sv0_vec_push(types, 0);
  sv0_vec_push(names, 20);
  sv0_vec_push(types, 5);
  sv0_vec_push(names, 30);
  sv0_vec_push(types, 3);
  int _sv0t2 = struct_field_ty(names, types, 10);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = struct_field_ty(names, types, 20);
  if ((_sv0t3 != 5)) {
    return 2;
  } else {
  }
  int _sv0t4 = struct_field_ty(names, types, 30);
  if ((_sv0t4 != 3)) {
    return 3;
  } else {
  }
  int _sv0t5 = struct_field_ty(names, types, 99);
  int _sv0t6 = (0 - 1);
  if ((_sv0t5 != _sv0t6)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_infer_lit(void) {
  int _sv0t0 = infer_lit(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = infer_lit(1);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = infer_lit(2);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  int _sv0t3 = infer_lit(3);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  int _sv0t4 = infer_lit(4);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = infer_lit(99);
  int _sv0t6 = (0 - 1);
  if ((_sv0t5 != _sv0t6)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_dup_variant_names(void) {
  int _sv0t0 = sv0_vec_new();
  int v = _sv0t0;
  sv0_vec_push(v, 10);
  sv0_vec_push(v, 20);
  sv0_vec_push(v, 30);
  int _sv0t1 = dup_variant_names(v);
  int _sv0t2 = (0 - 1);
  if ((_sv0t1 != _sv0t2)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int v2 = _sv0t3;
  sv0_vec_push(v2, 10);
  sv0_vec_push(v2, 20);
  sv0_vec_push(v2, 10);
  int _sv0t4 = dup_variant_names(v2);
  if ((_sv0t4 != 2)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int v3 = _sv0t5;
  int _sv0t6 = dup_variant_names(v3);
  int _sv0t7 = (0 - 1);
  if ((_sv0t6 != _sv0t7)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_struct_enum_clash(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(sn, 10);
  sv0_vec_push(sn, 20);
  sv0_vec_push(en, 30);
  sv0_vec_push(en, 40);
  int _sv0t2 = struct_enum_name_clash(sn, en);
  int _sv0t3 = (0 - 1);
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  sv0_vec_push(en, 20);
  int _sv0t4 = struct_enum_name_clash(sn, en);
  if ((_sv0t4 != 20)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int e2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int s2 = _sv0t6;
  int _sv0t7 = struct_enum_name_clash(s2, e2);
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_named_only_ty(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(sn, 10);
  sv0_vec_push(en, 20);
  int _sv0t2 = named_only_ty(sn, en, 10);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = named_only_ty(sn, en, 20);
  if ((_sv0t3 != 2)) {
    return 2;
  } else {
  }
  int _sv0t4 = named_only_ty(sn, en, 99);
  if ((_sv0t4 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_fields_of_struct_count(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int counts = _sv0t1;
  sv0_vec_push(names, 50);
  sv0_vec_push(counts, 3);
  sv0_vec_push(names, 60);
  sv0_vec_push(counts, 2);
  int _sv0t2 = fields_of_struct_count(names, counts, 50);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = fields_of_struct_count(names, counts, 60);
  if ((_sv0t3 != 2)) {
    return 2;
  } else {
  }
  int _sv0t4 = fields_of_struct_count(names, counts, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_stmt_returns_arena(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  sv0_vec_push(et, 15);
  int _sv0t2 = (0 - 1);
  sv0_vec_push(ed1, _sv0t2);
  sv0_vec_push(et, 28);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(et, 28);
  sv0_vec_push(ed1, 2);
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, 0);
  int _sv0t3 = stmt_returns_arena(et, ed1, 1);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = stmt_returns_arena(et, ed1, 3);
  if ((_sv0t4 != 0)) {
    return 2;
  } else {
  }
  int _sv0t5 = stmt_returns_arena(et, ed1, 4);
  if ((_sv0t5 != 0)) {
    return 3;
  } else {
  }
  int _sv0t6 = stmt_returns_arena(et, ed1, 0);
  if ((_sv0t6 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_last_stmt_returns_arena(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(et, 28);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(et, 15);
  int _sv0t2 = (0 - 1);
  sv0_vec_push(ed1, _sv0t2);
  sv0_vec_push(et, 28);
  sv0_vec_push(ed1, 2);
  int _sv0t3 = last_stmt_returns_arena(et, ed1, 0, 0);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = last_stmt_returns_arena(et, ed1, 1, 1);
  if ((_sv0t4 != 0)) {
    return 2;
  } else {
  }
  int _sv0t5 = last_stmt_returns_arena(et, ed1, 3, 1);
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_expr_references_result(void) {
  const char* source;
  source = "result  x  y";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  int _sv0t2 = sv0_vec_new();
  int pp = _sv0t2;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(pp, 2);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 3);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 2);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 3);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 2);
  sv0_vec_push(ed4, 0);
  int _sv0t8 = expr_references_result(et, ed1, ed2, ed3, ed4, 0, source, starts, ends, pp);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = expr_references_result(et, ed1, ed2, ed3, ed4, 1, source, starts, ends, pp);
  if ((_sv0t9 != 0)) {
    return 2;
  } else {
  }
  int _sv0t10 = expr_references_result(et, ed1, ed2, ed3, ed4, 2, source, starts, ends, pp);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  int _sv0t11 = expr_references_result(et, ed1, ed2, ed3, ed4, 3, source, starts, ends, pp);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = expr_references_result(et, ed1, ed2, ed3, ed4, 4, source, starts, ends, pp);
  if ((_sv0t12 != 0)) {
    return 5;
  } else {
  }
  int _sv0t13 = (0 - 1);
  int _sv0t14 = expr_references_result(et, ed1, ed2, ed3, ed4, _sv0t13, source, starts, ends, pp);
  if ((_sv0t14 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_ast_ty_types_tag(void) {
  int _sv0t0 = ast_ty_to_types_tag("unit");
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = ast_ty_to_types_tag("bool");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = ast_ty_to_types_tag("char");
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = ast_ty_to_types_tag("str");
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = ast_ty_to_types_tag("string");
  if ((_sv0t4 != 3)) {
    return 5;
  } else {
  }
  int _sv0t5 = ast_ty_to_types_tag("String");
  if ((_sv0t5 != 3)) {
    return 6;
  } else {
  }
  int _sv0t6 = ast_ty_to_types_tag("isize");
  if ((_sv0t6 != 4)) {
    return 7;
  } else {
  }
  int _sv0t7 = ast_ty_to_types_tag("usize");
  if ((_sv0t7 != 5)) {
    return 8;
  } else {
  }
  int _sv0t8 = ast_ty_to_types_tag("i32");
  if ((_sv0t8 != 7)) {
    return 9;
  } else {
  }
  int _sv0t9 = ast_ty_to_types_tag("u32");
  if ((_sv0t9 != 8)) {
    return 10;
  } else {
  }
  int _sv0t10 = ast_ty_to_types_tag("f64");
  if ((_sv0t10 != 9)) {
    return 11;
  } else {
  }
  int _sv0t11 = ast_ty_to_types_tag("Foo");
  int _sv0t12 = (0 - 1);
  if ((_sv0t11 != _sv0t12)) {
    return 12;
  } else {
  }
  int _sv0t13 = ast_ty_to_types_tag("Vec");
  int _sv0t14 = (0 - 1);
  if ((_sv0t13 != _sv0t14)) {
    return 13;
  } else {
  }
  int _sv0t15 = int_width_from_name("i32");
  if ((_sv0t15 != 32)) {
    return 14;
  } else {
  }
  int _sv0t16 = int_width_from_name("u64");
  if ((_sv0t16 != 64)) {
    return 15;
  } else {
  }
  return 0;
}

static int test_type_params(void) {
  int _sv0t0 = sv0_vec_new();
  int tp = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int names = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int tags = _sv0t2;
  sv0_vec_push(names, 10);
  sv0_vec_push(tags, 16);
  sv0_vec_push(names, 20);
  sv0_vec_push(tags, 16);
  int _sv0t3 = type_param_push(tp, names, tags);
  int saved = _sv0t3;
  if ((saved != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_len(tp);
  if ((_sv0t4 != 4)) {
    return 2;
  } else {
  }
  int _sv0t5 = type_param_lookup(tp, 4, 10);
  if ((_sv0t5 != 16)) {
    return 3;
  } else {
  }
  int _sv0t6 = type_param_lookup(tp, 4, 20);
  if ((_sv0t6 != 16)) {
    return 4;
  } else {
  }
  int _sv0t7 = type_param_lookup(tp, 4, 99);
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 5;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int n2 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int t2 = _sv0t10;
  sv0_vec_push(n2, 30);
  sv0_vec_push(t2, 16);
  int _sv0t11 = type_param_push(tp, n2, t2);
  int saved2 = _sv0t11;
  if ((saved2 != 4)) {
    return 6;
  } else {
  }
  int _sv0t12 = sv0_vec_len(tp);
  if ((_sv0t12 != 6)) {
    return 7;
  } else {
  }
  int _sv0t13 = type_param_lookup(tp, 6, 30);
  if ((_sv0t13 != 16)) {
    return 8;
  } else {
  }
  int _sv0t14 = type_param_lookup(tp, saved2, 30);
  int _sv0t15 = (0 - 1);
  if ((_sv0t14 != _sv0t15)) {
    return 9;
  } else {
  }
  int _sv0t16 = type_param_lookup(tp, saved2, 10);
  if ((_sv0t16 != 16)) {
    return 10;
  } else {
  }
  int _sv0t17 = type_param_lookup(tp, saved, 10);
  int _sv0t18 = (0 - 1);
  if ((_sv0t17 != _sv0t18)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_resolve_field_ty(void) {
  const char* source;
  source = "i32 bool Foo";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 12);
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int td1 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int td2 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int td3 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pp = _sv0t6;
  sv0_vec_push(tt, 0);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 0);
  sv0_vec_push(pp, 0);
  sv0_vec_push(tt, 6);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 0);
  sv0_vec_push(td3, 0);
  sv0_vec_push(tt, 0);
  sv0_vec_push(td1, 1);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 0);
  sv0_vec_push(pp, 1);
  int _sv0t7 = sv0_vec_new();
  int sn = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int en = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int tp = _sv0t9;
  int _sv0t10 = resolve_field_ty_tag(tt, td1, td2, td3, pp, source, starts, ends, sn, en, tp, 0, 0);
  int r0 = _sv0t10;
  if ((r0 != 7)) {
    return 1;
  } else {
  }
  int _sv0t11 = resolve_field_ty_tag(tt, td1, td2, td3, pp, source, starts, ends, sn, en, tp, 0, 1);
  int r1 = _sv0t11;
  if ((r1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t12 = resolve_field_ty_tag(tt, td1, td2, td3, pp, source, starts, ends, sn, en, tp, 0, 2);
  int r2 = _sv0t12;
  if ((r2 != 1)) {
    return 3;
  } else {
  }
  sv0_vec_push(tt, 0);
  sv0_vec_push(td1, 2);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 0);
  sv0_vec_push(pp, 2);
  sv0_vec_push(sn, 2);
  int _sv0t13 = resolve_field_ty_tag(tt, td1, td2, td3, pp, source, starts, ends, sn, en, tp, 0, 3);
  int r3 = _sv0t13;
  if ((r3 != 6)) {
    return 4;
  } else {
  }
  int _sv0t14 = sv0_vec_new();
  int en2 = _sv0t14;
  sv0_vec_push(en2, 2);
  int _sv0t15 = sv0_vec_new();
  int sn2 = _sv0t15;
  int _sv0t16 = resolve_field_ty_tag(tt, td1, td2, td3, pp, source, starts, ends, sn2, en2, tp, 0, 3);
  int r4 = _sv0t16;
  if ((r4 != 7)) {
    return 5;
  } else {
  }
  int _sv0t17 = (0 - 1);
  int _sv0t18 = resolve_field_ty_tag(tt, td1, td2, td3, pp, source, starts, ends, sn, en, tp, 0, _sv0t17);
  int r_bad = _sv0t18;
  int _sv0t19 = (0 - 1);
  if ((r_bad != _sv0t19)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_contract_expr_flag(void) {
  int _sv0t0 = contract_expr_new();
  int flag = _sv0t0;
  int _sv0t1 = contract_expr_active(flag);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = contract_expr_enter(flag);
  int saved = _sv0t2;
  int _sv0t3 = contract_expr_active(flag);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  int _sv0t4 = contract_expr_exit(flag, saved);
  int x = _sv0t4;
  int _sv0t5 = contract_expr_active(flag);
  if ((_sv0t5 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_ty_import_alias_push(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int targets = _sv0t1;
  sv0_vec_push(names, 10);
  sv0_vec_push(targets, 20);
  int _sv0t2 = sv0_vec_len(names);
  int saved = _sv0t2;
  sv0_vec_push(names, 30);
  sv0_vec_push(targets, 40);
  int _sv0t3 = alias_lookup_bounded(names, targets, saved, 30);
  int r1 = _sv0t3;
  if ((r1 != 30)) {
    return 1;
  } else {
  }
  int _sv0t4 = alias_lookup_bounded(names, targets, 2, 30);
  int r2 = _sv0t4;
  if ((r2 != 40)) {
    return 2;
  } else {
  }
  int _sv0t5 = alias_lookup_bounded(names, targets, 0, 10);
  int r3 = _sv0t5;
  if ((r3 != 10)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_ctor_ty_tag(void) {
  int _sv0t0 = ctor_ty_tag(0, 0);
  int _sv0t1 = TY_ENUM();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = ctor_ty_tag(1, 3);
  int _sv0t3 = TY_FN();
  if ((_sv0t2 != _sv0t3)) {
    return 2;
  } else {
  }
  int _sv0t4 = ctor_ty_tag(2, 2);
  int _sv0t5 = TY_FN();
  if ((_sv0t4 != _sv0t5)) {
    return 3;
  } else {
  }
  int _sv0t6 = ctor_ty_tag(1, 0);
  int _sv0t7 = TY_ENUM();
  if ((_sv0t6 != _sv0t7)) {
    return 4;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_binop_class();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_integral_ty();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (20 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_ast_type_names();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (30 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_assign_lhs();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (50 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_stmt_returns();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (60 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_path_key();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (70 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_alias_lookup();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (80 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_env_helpers();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (90 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_variant_shape();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (100 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_try_helpers();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (110 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_error_codes();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (120 + r11);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_struct_field();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (130 + r12);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_loop_depth();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (140 + r13);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_extra_classifiers();
  int r14 = _sv0t25;
  if ((r14 != 0)) {
    int _sv0t26 = (150 + r14);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = test_path_key_vec();
  int r15 = _sv0t27;
  if ((r15 != 0)) {
    int _sv0t28 = (160 + r15);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = test_contract_param();
  int r16 = _sv0t29;
  if ((r16 != 0)) {
    int _sv0t30 = (170 + r16);
    return _sv0t30;
  } else {
  }
  int _sv0t31 = test_struct_enum_names();
  int r17 = _sv0t31;
  if ((r17 != 0)) {
    int _sv0t32 = (180 + r17);
    return _sv0t32;
  } else {
  }
  int _sv0t33 = test_expect();
  int r18 = _sv0t33;
  if ((r18 != 0)) {
    int _sv0t34 = (190 + r18);
    return _sv0t34;
  } else {
  }
  int _sv0t35 = test_fields_of_struct();
  int r19 = _sv0t35;
  if ((r19 != 0)) {
    int _sv0t36 = (200 + r19);
    return _sv0t36;
  } else {
  }
  int _sv0t37 = test_variant_shape_of();
  int r20 = _sv0t37;
  if ((r20 != 0)) {
    int _sv0t38 = (210 + r20);
    return _sv0t38;
  } else {
  }
  int _sv0t39 = test_assign_lhs_bad();
  int r21 = _sv0t39;
  if ((r21 != 0)) {
    int _sv0t40 = (220 + r21);
    return _sv0t40;
  } else {
  }
  int _sv0t41 = test_infer_lit();
  int r22 = _sv0t41;
  if ((r22 != 0)) {
    int _sv0t42 = (230 + r22);
    return _sv0t42;
  } else {
  }
  int _sv0t43 = test_ctor_ty_arity();
  int r23 = _sv0t43;
  if ((r23 != 0)) {
    int _sv0t44 = (240 + r23);
    return _sv0t44;
  } else {
  }
  int _sv0t45 = test_struct_field_ty();
  int r24 = _sv0t45;
  if ((r24 != 0)) {
    int _sv0t46 = (250 + r24);
    return _sv0t46;
  } else {
  }
  int _sv0t47 = test_dup_variant_names();
  int r25 = _sv0t47;
  if ((r25 != 0)) {
    int _sv0t48 = (260 + r25);
    return _sv0t48;
  } else {
  }
  int _sv0t49 = test_struct_enum_clash();
  int r26 = _sv0t49;
  if ((r26 != 0)) {
    int _sv0t50 = (270 + r26);
    return _sv0t50;
  } else {
  }
  int _sv0t51 = test_named_only_ty();
  int r27 = _sv0t51;
  if ((r27 != 0)) {
    int _sv0t52 = (280 + r27);
    return _sv0t52;
  } else {
  }
  int _sv0t53 = test_fields_of_struct_count();
  int r28 = _sv0t53;
  if ((r28 != 0)) {
    int _sv0t54 = (290 + r28);
    return _sv0t54;
  } else {
  }
  int _sv0t55 = test_stmt_returns_arena();
  int r29 = _sv0t55;
  if ((r29 != 0)) {
    int _sv0t56 = (300 + r29);
    return _sv0t56;
  } else {
  }
  int _sv0t57 = test_last_stmt_returns_arena();
  int r30 = _sv0t57;
  if ((r30 != 0)) {
    int _sv0t58 = (310 + r30);
    return _sv0t58;
  } else {
  }
  int _sv0t59 = test_expr_references_result();
  int r31 = _sv0t59;
  if ((r31 != 0)) {
    int _sv0t60 = (320 + r31);
    return _sv0t60;
  } else {
  }
  int _sv0t61 = test_ast_ty_types_tag();
  int r32 = _sv0t61;
  if ((r32 != 0)) {
    int _sv0t62 = (330 + r32);
    return _sv0t62;
  } else {
  }
  int _sv0t63 = test_type_params();
  int r33 = _sv0t63;
  if ((r33 != 0)) {
    int _sv0t64 = (340 + r33);
    return _sv0t64;
  } else {
  }
  int _sv0t65 = test_resolve_field_ty();
  int r34 = _sv0t65;
  if ((r34 != 0)) {
    int _sv0t66 = (360 + r34);
    return _sv0t66;
  } else {
  }
  int _sv0t67 = test_contract_expr_flag();
  int r35 = _sv0t67;
  if ((r35 != 0)) {
    int _sv0t68 = (370 + r35);
    return _sv0t68;
  } else {
  }
  int _sv0t69 = test_ty_import_alias_push();
  int r36 = _sv0t69;
  if ((r36 != 0)) {
    int _sv0t70 = (380 + r36);
    return _sv0t70;
  } else {
  }
  int _sv0t71 = test_ctor_ty_tag();
  int r37 = _sv0t71;
  if ((r37 != 0)) {
    int _sv0t72 = (390 + r37);
    return _sv0t72;
  } else {
  }
  return 0;
}

