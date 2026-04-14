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
  return 0;
}

