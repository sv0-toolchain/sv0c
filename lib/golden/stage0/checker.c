#include "sv0_runtime.h"

static int BINOP_ARITH(void);
static int BINOP_LOGIC(void);
static int BINOP_CMP(void);
static int binop_class(int tag);
static int is_arith_binop(int tag);
static int is_logic_binop(int tag);
static int is_cmp_binop(int tag);
static int binop_returns_bool(int tag);
static int binop_result_ty_tag(int op_tag);
static int unop_result_ty_tag(int op_tag);
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
static int env_entry_index(int env_names, int name);
static int fn_table_add(int fn_names, int fn_param_counts, int fn_ret_types, int fn_param_offsets, int fn_param_types_flat, int name, int ret_ty, int param_tags, int param_count);
static int fn_table_lookup(int fn_names, int name);
static int fn_table_ret_type(int fn_ret_types, int idx);
static int fn_table_param_count(int fn_param_counts, int idx);
static int fn_table_param_type(int fn_param_offsets, int fn_param_types_flat, int fn_idx, int param_idx);
static int fn_table_lookup_str(int fn_names, const char* source, int starts, int ends, const char* name_str);
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
static int dup_variant_names_str(const char* source, int starts, int ends, int name_toks);
static int struct_enum_name_clash(int struct_names, int enum_names);
static int struct_enum_name_clash_str(const char* source, int starts, int ends, int struct_names, int enum_names);
static int named_only_ty(int struct_names, int enum_names, int name);
static int fields_of_struct_count(int struct_names, int struct_field_counts, int name);
static int stmt_returns_arena(int et, int ed1, int idx);
static int last_stmt_returns_arena(int et, int ed1, int first_stmt, int stmt_count);
static const char* chk_tok_str(const char* source, int starts, int ends, int pos);
static int expr_references_result(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp);
static int type_param_push(int type_params, int new_names, int new_tags);
static int type_param_lookup(int type_params, int limit, int name);
static int resolve_field_ty_tag(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int pp, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int idx);
static int ast_ty_to_ty_payload(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int pp, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int idx, int out_payload);
static int enum_ctor_count(int variant_enum_ids, int enum_name);
static int item_fn_ret_ty_tag(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int pp, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int ret_idx);
static int scan_struct_field_names(int tok_tags, int name_tok_pos, int field_count, int out_field_names);
static int scan_enum_variant_names(int tok_tags, int name_tok_pos, int variant_count, int out_variant_names);
static int type_param_lookup_str(int type_params, int limit, const char* source, int starts, int ends, const char* name_str);
static int scan_type_tag_at(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int pos, int out_tag);
static int scan_struct_field_type_tags(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int field_name_positions, int out_field_types);
static int scan_fn_param_type_tags(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int name_tok_pos, int param_count, int out_param_types);
static int scan_fn_ret_type_tag(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int name_tok_pos, int has_ret);
static int item_fn_ty_to_table(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int id1, int id2, int id3, int fn_names, int fn_param_counts, int fn_ret_types, int fn_param_offsets, int fn_param_types_flat, int item_idx);
static int register_all_item_fns(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int it, int id1, int id2, int id3, int fn_names, int fn_param_counts, int fn_ret_types, int fn_param_offsets, int fn_param_types_flat);
static int scan_fn_param_names(int tok_tags, int name_tok_pos, int param_count, int out_param_name_pos, int out_param_muts);
static int check_fn_body(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, const char* source, int starts, int ends, int mod_env_names, int mod_env_types, int mod_env_muts, int param_name_pos, int param_type_tags, int param_muts, int param_count, int ret_ty, int body_root_idx, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int fn_names, int fn_ret_types);
static int process_item_use(const char* source, int starts, int ends, int pp, int id1, int id2, int item_idx, int fn_names, int fn_ret_types, int sdef_names, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int env_names, int env_types, int env_muts, int aliases, int alias_targets);
static int build_mod_env(const char* source, int starts, int ends, int pp, int it, int id1, int id2, int id3, int id4, int fn_names, int fn_ret_types, int sdef_names, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int env_names, int env_types, int env_muts, int aliases, int alias_targets);
static int check_program(int tok_tags, const char* source, int starts, int ends, int pp, int it, int id1, int id2, int id3, int id4, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int type_params, int tp_limit);
static int contract_expr_new(void);
static int contract_expr_enter(int flag);
static int contract_expr_exit(int flag, int saved);
static int contract_expr_active(int flag);
static int ty_import_alias_push(int aliases, int from_h, int to_h);
static int alias_lookup_bounded(int names, int targets, int limit, int name);
static int ctor_ty_tag(int shape, int param_count);
static int try_success_validate(int variant_tags, int variant_enum_ids, int variant_shapes, int variant_field_counts, int enum_name, const char* source, int starts, int ends, int out_idx);
static int scan_enum_variant_shapes(int tok_tags, int variant_name_positions, int out_shapes);
static int init_struct_defs(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int it, int id1, int id2, int sdef_names, int sdef_field_offsets, int sdef_field_counts, int sdef_fnames_flat, int sdef_ftypes_flat);
static int init_enum_defs(int tok_tags, const char* source, int starts, int ends, int it, int id1, int id2, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat);
static int struct_def_field_ty_str(int sdef_names, int sdef_field_offsets, int sdef_field_counts, int sdef_fnames_flat, int sdef_ftypes_flat, const char* source, int starts, int ends, int struct_name_pos, const char* field_name_str);
static int enum_def_variant_shape_str(int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, const char* source, int starts, int ends, int enum_name_pos, const char* variant_name_str);
static int BUILTIN_COUNT(void);
static int builtin_fn_lookup(const char* name_str);
static int builtin_fn_ret_type(int bid);
static int builtin_fn_param_count(int bid);
static int builtin_fn_param_type(int bid, int idx);
static int resolve_fn_call(int fn_names, const char* source, int starts, int ends, const char* name_str, int is_builtin_out);
static int edef_name_lookup_str(int edef_names, const char* source, int starts, int ends, const char* enum_name_str);
static int ctor_type_from_edef(int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, const char* source, int starts, int ends, int enum_name_pos, const char* variant_name_str);
static int resolve_ctor_path_ty(int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, const char* source, int starts, int ends, int seg1_tok_pos, int seg2_tok_pos);
static int synth_expr_call_simple(int et, int ed1, int ed2, int ed3, int ed4, int pp, const char* source, int starts, int ends, int env_names, int env_types, int env_muts, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int fn_names, int fn_ret_types, int ret_ty, int loop_depth, int callee_idx, int args_first, int args_count);
static int sdef_name_lookup_str(int sdef_names, const char* source, int starts, int ends, const char* struct_name_str);
static int env_lookup_str(int env_names, int env_types, const char* source, int starts, int ends, const char* target_str);
static int synth_call_ret_type(int fn_names, int fn_ret_types, const char* source, int starts, int ends, const char* callee_name_str, int is_builtin_out);
static int block_stmt_index(int pp, int stmts_first, int sidecar, int si);
static int check_stmt_in_block(int et, int ed1, int ed2, int ed3, int ed4, int pp, const char* source, int starts, int ends, int env_names, int env_types, int env_muts, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int fn_names, int fn_ret_types, int ret_ty, int loop_depth, int stmt_idx);
static int synth_expr(int et, int ed1, int ed2, int ed3, int ed4, int pp, const char* source, int starts, int ends, int env_names, int env_types, int env_muts, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int fn_names, int fn_ret_types, int ret_ty, int loop_depth, int idx);
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
static int test_try_success_validate(void);
static int test_binop_unop_result_ty(void);
static int test_int_width_from_name(void);
static int test_ast_ty_payload(void);
static int test_enum_ctor_count(void);
static int test_env_entry_index(void);
static int test_fn_table(void);
static int test_scan_struct_field_names(void);
static int test_scan_enum_variant_names(void);
static int test_scan_type_tag_at(void);
static int test_scan_struct_field_type_tags(void);
static int test_scan_fn_param_type_tags(void);
static int test_fn_table_lookup_str(void);
static int test_scan_fn_ret_type_tag(void);
static int test_register_all_item_fns(void);
static int test_scan_enum_variant_shapes(void);
static int test_init_struct_defs(void);
static int test_init_enum_defs(void);
static int test_init_enum_defs_dup_variants(void);
static int test_builtin_fn_lookup(void);
static int test_resolve_fn_call(void);
static int test_enum_ctor_resolution(void);
static int test_synth_expr(void);
static int test_scan_fn_param_names(void);
static int test_check_fn_body(void);
static int test_check_program(void);
static int test_process_item_use(void);
static int test_build_mod_env(void);
static int test_synth_building_blocks(void);
static int test_named_only_ty_basic(void);

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

static int binop_result_ty_tag(int op_tag) {
  int _sv0t0 = is_arith_binop(op_tag);
  if (_sv0t0) {
    int _sv0t1 = TY_INT();
    return _sv0t1;
  } else {
  }
  int _sv0t2 = TY_BOOL();
  return _sv0t2;
}

static int unop_result_ty_tag(int op_tag) {
  if ((op_tag == 0)) {
    int _sv0t0 = TY_INT();
    return _sv0t0;
  } else {
  }
  if ((op_tag == 1)) {
    int _sv0t1 = TY_BOOL();
    return _sv0t1;
  } else {
  }
  if ((op_tag == 2)) {
    int _sv0t2 = TY_INT();
    return _sv0t2;
  } else {
  }
  int _sv0t3 = TY_UNKNOWN();
  return _sv0t3;
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

static int env_entry_index(int env_names, int name) {
  int _sv0t0 = sv0_vec_len(env_names);
  int n = _sv0t0;
  int i = (n - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(env_names, i);
    if ((_sv0t1 == name)) {
      return i;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int fn_table_add(int fn_names, int fn_param_counts, int fn_ret_types, int fn_param_offsets, int fn_param_types_flat, int name, int ret_ty, int param_tags, int param_count) {
  int _sv0t0 = sv0_vec_len(fn_param_types_flat);
  int offset = _sv0t0;
  sv0_vec_push(fn_names, name);
  sv0_vec_push(fn_param_counts, param_count);
  sv0_vec_push(fn_ret_types, ret_ty);
  sv0_vec_push(fn_param_offsets, offset);
  int j = 0;
  while ((j < param_count)) {
    int _sv0t1 = sv0_vec_get(param_tags, j);
    sv0_vec_push(fn_param_types_flat, _sv0t1);
    j = (j + 1);
  }
  int _sv0t2 = sv0_vec_len(fn_names);
  int _sv0t3 = (_sv0t2 - 1);
  return _sv0t3;
}

static int fn_table_lookup(int fn_names, int name) {
  int _sv0t0 = sv0_vec_len(fn_names);
  int n = _sv0t0;
  int i = (n - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(fn_names, i);
    if ((_sv0t1 == name)) {
      return i;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int fn_table_ret_type(int fn_ret_types, int idx) {
  if ((idx < 0)) {
    int _sv0t0 = TY_UNKNOWN();
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(fn_ret_types, idx);
  return _sv0t1;
}

static int fn_table_param_count(int fn_param_counts, int idx) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(fn_param_counts, idx);
  return _sv0t1;
}

static int fn_table_param_type(int fn_param_offsets, int fn_param_types_flat, int fn_idx, int param_idx) {
  if ((fn_idx < 0)) {
    int _sv0t0 = TY_UNKNOWN();
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(fn_param_offsets, fn_idx);
  int off = _sv0t1;
  int _sv0t2 = (off + param_idx);
  int _sv0t3 = sv0_vec_get(fn_param_types_flat, _sv0t2);
  return _sv0t3;
}

static int fn_table_lookup_str(int fn_names, const char* source, int starts, int ends, const char* name_str) {
  int _sv0t0 = sv0_vec_len(fn_names);
  int n = _sv0t0;
  int i = (n - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(fn_names, i);
    int pos = _sv0t1;
    int _sv0t2 = sv0_vec_get(starts, pos);
    int s = _sv0t2;
    int _sv0t3 = sv0_vec_get(ends, pos);
    int e = _sv0t3;
    int _sv0t4 = (e - s);
    const char* _sv0t5 = sv0_string_substr(source, s, _sv0t4);
    const char* nm;
    nm = _sv0t5;
    int _sv0t6 = sv0_string_eq(nm, name_str);
    if (_sv0t6) {
      return i;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t7 = (0 - 1);
  return _sv0t7;
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
  int _sv0t0 = TY_VAR();
  if ((got_tag == _sv0t0)) {
    return 1;
  } else {
  }
  int _sv0t1 = TY_VAR();
  if ((want_tag == _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = (got_tag == want_tag);
  return _sv0t2;
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
    int _sv0t0 = TY_INT();
    return _sv0t0;
  } else {
  }
  if ((lit_tag == 1)) {
    int _sv0t1 = TY_BOOL();
    return _sv0t1;
  } else {
  }
  if ((lit_tag == 2)) {
    int _sv0t2 = TY_UNIT();
    return _sv0t2;
  } else {
  }
  if ((lit_tag == 3)) {
    int _sv0t3 = TY_STRING();
    return _sv0t3;
  } else {
  }
  if ((lit_tag == 4)) {
    int _sv0t4 = TY_CHAR();
    return _sv0t4;
  } else {
  }
  int _sv0t5 = (0 - 1);
  return _sv0t5;
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

static int dup_variant_names_str(const char* source, int starts, int ends, int name_toks) {
  int _sv0t0 = sv0_vec_len(name_toks);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(name_toks, i);
    int ti = _sv0t1;
    int _sv0t2 = sv0_vec_get(starts, ti);
    int si = _sv0t2;
    int _sv0t3 = sv0_vec_get(ends, ti);
    int ei = _sv0t3;
    int _sv0t4 = (ei - si);
    const char* _sv0t5 = sv0_string_substr(source, si, _sv0t4);
    const char* stri;
    stri = _sv0t5;
    int j = 0;
    while ((j < i)) {
      int _sv0t6 = sv0_vec_get(name_toks, j);
      int tj = _sv0t6;
      int _sv0t7 = sv0_vec_get(starts, tj);
      int sj = _sv0t7;
      int _sv0t8 = sv0_vec_get(ends, tj);
      int ej = _sv0t8;
      int _sv0t9 = (ej - sj);
      const char* _sv0t10 = sv0_string_substr(source, sj, _sv0t9);
      const char* strj;
      strj = _sv0t10;
      int _sv0t11 = sv0_string_eq(stri, strj);
      if (_sv0t11) {
        return i;
      } else {
      }
      j = (j + 1);
    }
    i = (i + 1);
  }
  int _sv0t12 = (0 - 1);
  return _sv0t12;
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

static int struct_enum_name_clash_str(const char* source, int starts, int ends, int struct_names, int enum_names) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_len(enum_names);
  int en = _sv0t1;
  int i = 0;
  while ((i < sn)) {
    int _sv0t2 = sv0_vec_get(struct_names, i);
    int st = _sv0t2;
    int _sv0t3 = sv0_vec_get(starts, st);
    int ss = _sv0t3;
    int _sv0t4 = sv0_vec_get(ends, st);
    int se = _sv0t4;
    int _sv0t5 = (se - ss);
    const char* _sv0t6 = sv0_string_substr(source, ss, _sv0t5);
    const char* sstr;
    sstr = _sv0t6;
    int j = 0;
    while ((j < en)) {
      int _sv0t7 = sv0_vec_get(enum_names, j);
      int et = _sv0t7;
      int _sv0t8 = sv0_vec_get(starts, et);
      int es = _sv0t8;
      int _sv0t9 = sv0_vec_get(ends, et);
      int ee = _sv0t9;
      int _sv0t10 = (ee - es);
      const char* _sv0t11 = sv0_string_substr(source, es, _sv0t10);
      const char* estr;
      estr = _sv0t11;
      int _sv0t12 = sv0_string_eq(sstr, estr);
      if (_sv0t12) {
        return st;
      } else {
      }
      j = (j + 1);
    }
    i = (i + 1);
  }
  int _sv0t13 = (0 - 1);
  return _sv0t13;
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
      while (1) {
        int _sv0t14 = sv0_vec_len(struct_names);
        int _sv0t17 = (ni < _sv0t14);
        if ((!_sv0t17)) {
          break;
        } else {
        }
        int _sv0t15 = sv0_vec_get(struct_names, ni);
        if ((_sv0t15 == tok)) {
          int _sv0t16 = TY_STRUCT();
          return _sv0t16;
        } else {
        }
        ni = (ni + 1);
      }
      int ei = 0;
      while (1) {
        int _sv0t18 = sv0_vec_len(enum_names);
        int _sv0t21 = (ei < _sv0t18);
        if ((!_sv0t21)) {
          break;
        } else {
        }
        int _sv0t19 = sv0_vec_get(enum_names, ei);
        if ((_sv0t19 == tok)) {
          int _sv0t20 = TY_ENUM();
          return _sv0t20;
        } else {
        }
        ei = (ei + 1);
      }
      int _sv0t22 = type_param_lookup(type_params, tp_limit, tok);
      int tp = _sv0t22;
      if ((tp >= 0)) {
        int _sv0t23 = TY_VAR();
        return _sv0t23;
      } else {
      }
      int _sv0t24 = (0 - 1);
      return _sv0t24;
    } else {
    }
    int _sv0t25 = (0 - 1);
    return _sv0t25;
  } else {
  }
  int _sv0t26 = (0 - 1);
  return _sv0t26;
}

static int ast_ty_to_ty_payload(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int pp, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int idx, int out_payload) {
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
      int _sv0t13 = ast_type_name_to_tag(name);
      int prim = _sv0t13;
      if ((prim >= 0)) {
        int _sv0t14 = int_width_from_name(name);
        int w = _sv0t14;
        if ((w > 0)) {
          sv0_vec_push(out_payload, w);
        } else {
        }
        return prim;
      } else {
      }
      int ni = 0;
      while (1) {
        int _sv0t15 = sv0_vec_len(struct_names);
        int _sv0t18 = (ni < _sv0t15);
        if ((!_sv0t18)) {
          break;
        } else {
        }
        int _sv0t16 = sv0_vec_get(struct_names, ni);
        if ((_sv0t16 == tok)) {
          sv0_vec_push(out_payload, tok);
          int _sv0t17 = TY_STRUCT();
          return _sv0t17;
        } else {
        }
        ni = (ni + 1);
      }
      int ei = 0;
      while (1) {
        int _sv0t19 = sv0_vec_len(enum_names);
        int _sv0t22 = (ei < _sv0t19);
        if ((!_sv0t22)) {
          break;
        } else {
        }
        int _sv0t20 = sv0_vec_get(enum_names, ei);
        if ((_sv0t20 == tok)) {
          sv0_vec_push(out_payload, tok);
          int _sv0t21 = TY_ENUM();
          return _sv0t21;
        } else {
        }
        ei = (ei + 1);
      }
      int _sv0t23 = type_param_lookup(type_params, tp_limit, tok);
      int tp = _sv0t23;
      if ((tp >= 0)) {
        sv0_vec_push(out_payload, tp);
        int _sv0t24 = TY_VAR();
        return _sv0t24;
      } else {
      }
      int _sv0t25 = (0 - 1);
      return _sv0t25;
    } else {
    }
    int _sv0t26 = (0 - 1);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = (0 - 1);
  return _sv0t27;
}

static int enum_ctor_count(int variant_enum_ids, int enum_name) {
  int _sv0t0 = sv0_vec_len(variant_enum_ids);
  int nv = _sv0t0;
  int count = 0;
  int j = 0;
  while ((j < nv)) {
    int _sv0t1 = sv0_vec_get(variant_enum_ids, j);
    if ((_sv0t1 == enum_name)) {
      count = (count + 1);
    } else {
    }
    j = (j + 1);
  }
  return count;
}

static int item_fn_ret_ty_tag(int ty_tags, int ty_d1, int ty_d2, int ty_d3, int pp, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int ret_idx) {
  if ((ret_idx < 0)) {
    int _sv0t0 = TY_UNKNOWN();
    return _sv0t0;
  } else {
  }
  int _sv0t1 = resolve_field_ty_tag(ty_tags, ty_d1, ty_d2, ty_d3, pp, source, starts, ends, struct_names, enum_names, type_params, tp_limit, ret_idx);
  return _sv0t1;
}

static int scan_struct_field_names(int tok_tags, int name_tok_pos, int field_count, int out_field_names) {
  int p = (name_tok_pos + 1);
  while (1) {
    int _sv0t0 = sv0_vec_get(tok_tags, p);
    int _sv0t3 = (_sv0t0 != 8);
    if ((!_sv0t3)) {
      break;
    } else {
    }
    p = (p + 1);
    int _sv0t1 = sv0_vec_len(tok_tags);
    if ((p >= _sv0t1)) {
      int _sv0t2 = (0 - 1);
      return _sv0t2;
    } else {
    }
  }
  p = (p + 1);
  int found = 0;
  while ((found < field_count)) {
    int _sv0t4 = sv0_vec_get(tok_tags, p);
    if ((_sv0t4 == 87)) {
      p = (p + 1);
    } else {
    }
    int _sv0t5 = sv0_vec_get(tok_tags, p);
    if ((_sv0t5 != 5)) {
      int _sv0t6 = (0 - 1);
      return _sv0t6;
    } else {
    }
    sv0_vec_push(out_field_names, p);
    found = (found + 1);
    p = (p + 1);
    int _sv0t7 = sv0_vec_get(tok_tags, p);
    if ((_sv0t7 != 14)) {
      int _sv0t8 = (0 - 1);
      return _sv0t8;
    } else {
    }
    p = (p + 1);
    int depth = 0;
    int scanning_type = 1;
    while (scanning_type) {
      int _sv0t9 = sv0_vec_get(tok_tags, p);
      int t = _sv0t9;
      if ((t == 8)) {
        depth = (depth + 1);
        p = (p + 1);
      } else {
        if ((t == 10)) {
          depth = (depth + 1);
          p = (p + 1);
        } else {
          if ((t == 9)) {
            if ((depth > 0)) {
              depth = (depth - 1);
              p = (p + 1);
            } else {
              scanning_type = 0;
            }
          } else {
            if ((t == 11)) {
              if ((depth > 0)) {
                depth = (depth - 1);
                p = (p + 1);
              } else {
                scanning_type = 0;
              }
            } else {
              if ((t == 12)) {
                if ((depth == 0)) {
                  scanning_type = 0;
                  p = (p + 1);
                } else {
                  p = (p + 1);
                }
              } else {
                p = (p + 1);
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

static int scan_enum_variant_names(int tok_tags, int name_tok_pos, int variant_count, int out_variant_names) {
  int p = (name_tok_pos + 1);
  while (1) {
    int _sv0t0 = sv0_vec_get(tok_tags, p);
    int _sv0t3 = (_sv0t0 != 8);
    if ((!_sv0t3)) {
      break;
    } else {
    }
    p = (p + 1);
    int _sv0t1 = sv0_vec_len(tok_tags);
    if ((p >= _sv0t1)) {
      int _sv0t2 = (0 - 1);
      return _sv0t2;
    } else {
    }
  }
  p = (p + 1);
  int found = 0;
  while ((found < variant_count)) {
    int _sv0t4 = sv0_vec_get(tok_tags, p);
    if ((_sv0t4 != 5)) {
      int _sv0t5 = (0 - 1);
      return _sv0t5;
    } else {
    }
    sv0_vec_push(out_variant_names, p);
    found = (found + 1);
    p = (p + 1);
    int _sv0t6 = sv0_vec_get(tok_tags, p);
    int t = _sv0t6;
    if ((t == 6)) {
      int depth = 1;
      p = (p + 1);
      while ((depth > 0)) {
        int _sv0t7 = sv0_vec_get(tok_tags, p);
        int u = _sv0t7;
        if ((u == 6)) {
          depth = (depth + 1);
        } else {
        }
        if ((u == 7)) {
          depth = (depth - 1);
        } else {
        }
        p = (p + 1);
      }
    } else {
    }
    if ((t == 10)) {
      int depth = 1;
      p = (p + 1);
      while ((depth > 0)) {
        int _sv0t8 = sv0_vec_get(tok_tags, p);
        int u = _sv0t8;
        if ((u == 10)) {
          depth = (depth + 1);
        } else {
        }
        if ((u == 11)) {
          depth = (depth - 1);
        } else {
        }
        p = (p + 1);
      }
    } else {
    }
    if ((t == 8)) {
      int depth = 1;
      p = (p + 1);
      while ((depth > 0)) {
        int _sv0t9 = sv0_vec_get(tok_tags, p);
        int u = _sv0t9;
        if ((u == 8)) {
          depth = (depth + 1);
        } else {
        }
        if ((u == 9)) {
          depth = (depth - 1);
        } else {
        }
        p = (p + 1);
      }
    } else {
    }
    int _sv0t10 = sv0_vec_get(tok_tags, p);
    if ((_sv0t10 == 12)) {
      p = (p + 1);
    } else {
    }
  }
  return 0;
}

static int type_param_lookup_str(int type_params, int limit, const char* source, int starts, int ends, const char* name_str) {
  int i = (limit - 2);
  while ((i >= 0)) {
    int _sv0t0 = sv0_vec_get(type_params, i);
    int tp_pos = _sv0t0;
    int _sv0t1 = sv0_vec_get(starts, tp_pos);
    int tps = _sv0t1;
    int _sv0t2 = sv0_vec_get(ends, tp_pos);
    int tpe = _sv0t2;
    int _sv0t3 = (tpe - tps);
    const char* _sv0t4 = sv0_string_substr(source, tps, _sv0t3);
    const char* tp_name;
    tp_name = _sv0t4;
    int _sv0t5 = sv0_string_eq(tp_name, name_str);
    if (_sv0t5) {
      int _sv0t6 = (i + 1);
      int _sv0t7 = sv0_vec_get(type_params, _sv0t6);
      return _sv0t7;
    } else {
    }
    i = (i - 2);
  }
  int _sv0t8 = (0 - 1);
  return _sv0t8;
}

static int scan_type_tag_at(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int pos, int out_tag) {
  int _sv0t0 = sv0_vec_get(tok_tags, pos);
  int t = _sv0t0;
  if ((t == 5)) {
    int _sv0t1 = (pos + 1);
    int _sv0t2 = sv0_vec_get(tok_tags, _sv0t1);
    if ((_sv0t2 == 38)) {
      int depth = 1;
      int p = (pos + 2);
      while ((depth > 0)) {
        int _sv0t3 = sv0_vec_get(tok_tags, p);
        int u = _sv0t3;
        if ((u == 38)) {
          depth = (depth + 1);
        } else {
        }
        if ((u == 39)) {
          depth = (depth - 1);
        } else {
        }
        p = (p + 1);
      }
      sv0_vec_push(out_tag, 15);
      return p;
    } else {
    }
    int _sv0t4 = sv0_vec_get(starts, pos);
    int s = _sv0t4;
    int _sv0t5 = sv0_vec_get(ends, pos);
    int e = _sv0t5;
    int _sv0t6 = (e - s);
    const char* _sv0t7 = sv0_string_substr(source, s, _sv0t6);
    const char* name;
    name = _sv0t7;
    int _sv0t8 = ast_type_name_to_tag(name);
    int prim = _sv0t8;
    if ((prim >= 0)) {
      sv0_vec_push(out_tag, prim);
      int _sv0t9 = (pos + 1);
      return _sv0t9;
    } else {
    }
    int ni = 0;
    while (1) {
      int _sv0t10 = sv0_vec_len(struct_names);
      int _sv0t18 = (ni < _sv0t10);
      if ((!_sv0t18)) {
        break;
      } else {
      }
      int _sv0t11 = sv0_vec_get(struct_names, ni);
      int sn = _sv0t11;
      int _sv0t12 = sv0_vec_get(starts, sn);
      int ss = _sv0t12;
      int _sv0t13 = sv0_vec_get(ends, sn);
      int se = _sv0t13;
      int _sv0t14 = (se - ss);
      const char* _sv0t15 = sv0_string_substr(source, ss, _sv0t14);
      const char* sname;
      sname = _sv0t15;
      int _sv0t16 = sv0_string_eq(name, sname);
      if (_sv0t16) {
        sv0_vec_push(out_tag, 6);
        int _sv0t17 = (pos + 1);
        return _sv0t17;
      } else {
      }
      ni = (ni + 1);
    }
    int ei = 0;
    while (1) {
      int _sv0t19 = sv0_vec_len(enum_names);
      int _sv0t27 = (ei < _sv0t19);
      if ((!_sv0t27)) {
        break;
      } else {
      }
      int _sv0t20 = sv0_vec_get(enum_names, ei);
      int en_pos = _sv0t20;
      int _sv0t21 = sv0_vec_get(starts, en_pos);
      int es = _sv0t21;
      int _sv0t22 = sv0_vec_get(ends, en_pos);
      int ee = _sv0t22;
      int _sv0t23 = (ee - es);
      const char* _sv0t24 = sv0_string_substr(source, es, _sv0t23);
      const char* ename;
      ename = _sv0t24;
      int _sv0t25 = sv0_string_eq(name, ename);
      if (_sv0t25) {
        sv0_vec_push(out_tag, 7);
        int _sv0t26 = (pos + 1);
        return _sv0t26;
      } else {
      }
      ei = (ei + 1);
    }
    int _sv0t28 = type_param_lookup_str(type_params, tp_limit, source, starts, ends, name);
    int tp = _sv0t28;
    if ((tp >= 0)) {
      sv0_vec_push(out_tag, 16);
      int _sv0t29 = (pos + 1);
      return _sv0t29;
    } else {
    }
    int _sv0t30 = (0 - 1);
    sv0_vec_push(out_tag, _sv0t30);
    int _sv0t31 = (pos + 1);
    return _sv0t31;
  } else {
  }
  if ((t == 6)) {
    int _sv0t32 = (pos + 1);
    int _sv0t33 = sv0_vec_get(tok_tags, _sv0t32);
    if ((_sv0t33 == 7)) {
      sv0_vec_push(out_tag, 2);
      int _sv0t34 = (pos + 2);
      return _sv0t34;
    } else {
    }
    int depth = 1;
    int p = (pos + 1);
    while ((depth > 0)) {
      int _sv0t35 = sv0_vec_get(tok_tags, p);
      int u = _sv0t35;
      if ((u == 6)) {
        depth = (depth + 1);
      } else {
      }
      if ((u == 7)) {
        depth = (depth - 1);
      } else {
      }
      p = (p + 1);
    }
    sv0_vec_push(out_tag, 14);
    return p;
  } else {
  }
  if ((t == 27)) {
    int _sv0t36 = (pos + 1);
    int _sv0t37 = sv0_vec_get(tok_tags, _sv0t36);
    if ((_sv0t37 == 77)) {
      int _sv0t38 = sv0_vec_new();
      int inner_out = _sv0t38;
      int _sv0t39 = (pos + 2);
      int _sv0t40 = scan_type_tag_at(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, _sv0t39, inner_out);
      int p = _sv0t40;
      sv0_vec_push(out_tag, 13);
      return p;
    } else {
    }
    int _sv0t41 = sv0_vec_new();
    int inner_out = _sv0t41;
    int _sv0t42 = (pos + 1);
    int _sv0t43 = scan_type_tag_at(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, _sv0t42, inner_out);
    int p = _sv0t43;
    sv0_vec_push(out_tag, 12);
    return p;
  } else {
  }
  int _sv0t44 = (0 - 1);
  sv0_vec_push(out_tag, _sv0t44);
  int _sv0t45 = (pos + 1);
  return _sv0t45;
}

static int scan_struct_field_type_tags(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int field_name_positions, int out_field_types) {
  int _sv0t0 = sv0_vec_len(field_name_positions);
  int count = _sv0t0;
  int i = 0;
  while ((i < count)) {
    int _sv0t1 = sv0_vec_get(field_name_positions, i);
    int name_pos = _sv0t1;
    int type_start = (name_pos + 2);
    int _sv0t2 = scan_type_tag_at(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, type_start, out_field_types);
    int _end = _sv0t2;
    i = (i + 1);
  }
  return 0;
}

static int scan_fn_param_type_tags(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int name_tok_pos, int param_count, int out_param_types) {
  if ((param_count == 0)) {
    return 0;
  } else {
  }
  int p = (name_tok_pos + 1);
  int _sv0t0 = sv0_vec_get(tok_tags, p);
  if ((_sv0t0 == 38)) {
    int depth = 1;
    p = (p + 1);
    while ((depth > 0)) {
      int _sv0t1 = sv0_vec_get(tok_tags, p);
      int u = _sv0t1;
      if ((u == 38)) {
        depth = (depth + 1);
      } else {
      }
      if ((u == 39)) {
        depth = (depth - 1);
      } else {
      }
      p = (p + 1);
    }
  } else {
  }
  int _sv0t2 = sv0_vec_get(tok_tags, p);
  if ((_sv0t2 != 6)) {
    int _sv0t3 = (0 - 1);
    return _sv0t3;
  } else {
  }
  p = (p + 1);
  int found = 0;
  while ((found < param_count)) {
    int _sv0t4 = sv0_vec_get(tok_tags, p);
    if ((_sv0t4 == 77)) {
      p = (p + 1);
    } else {
    }
    int _sv0t5 = sv0_vec_get(tok_tags, p);
    int pt = _sv0t5;
    if ((pt != 5)) {
      if ((pt != 86)) {
        int _sv0t6 = (0 - 1);
        return _sv0t6;
      } else {
      }
    } else {
    }
    p = (p + 1);
    int _sv0t7 = sv0_vec_get(tok_tags, p);
    if ((_sv0t7 != 14)) {
      int _sv0t8 = (0 - 1);
      return _sv0t8;
    } else {
    }
    p = (p + 1);
    int _sv0t9 = scan_type_tag_at(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, p, out_param_types);
    p = _sv0t9;
    if ((p < 0)) {
      int _sv0t10 = (0 - 1);
      return _sv0t10;
    } else {
    }
    found = (found + 1);
    int _sv0t11 = sv0_vec_get(tok_tags, p);
    if ((_sv0t11 == 12)) {
      p = (p + 1);
    } else {
    }
  }
  return 0;
}

static int scan_fn_ret_type_tag(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int name_tok_pos, int has_ret) {
  if ((has_ret == 0)) {
    int _sv0t0 = TY_UNIT();
    return _sv0t0;
  } else {
  }
  int p = (name_tok_pos + 1);
  int _sv0t1 = sv0_vec_get(tok_tags, p);
  if ((_sv0t1 == 38)) {
    int depth = 1;
    p = (p + 1);
    while ((depth > 0)) {
      int _sv0t2 = sv0_vec_get(tok_tags, p);
      int u = _sv0t2;
      if ((u == 38)) {
        depth = (depth + 1);
      } else {
      }
      if ((u == 39)) {
        depth = (depth - 1);
      } else {
      }
      p = (p + 1);
    }
  } else {
  }
  int _sv0t3 = sv0_vec_get(tok_tags, p);
  if ((_sv0t3 != 6)) {
    int _sv0t4 = TY_UNKNOWN();
    return _sv0t4;
  } else {
  }
  int depth = 1;
  p = (p + 1);
  while ((depth > 0)) {
    int _sv0t5 = sv0_vec_get(tok_tags, p);
    int u = _sv0t5;
    if ((u == 6)) {
      depth = (depth + 1);
    } else {
    }
    if ((u == 7)) {
      depth = (depth - 1);
    } else {
    }
    p = (p + 1);
  }
  int _sv0t6 = sv0_vec_get(tok_tags, p);
  if ((_sv0t6 != 19)) {
    int _sv0t7 = TY_UNKNOWN();
    return _sv0t7;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  int _sv0t9 = (p + 1);
  int _sv0t10 = scan_type_tag_at(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, _sv0t9, out);
  int _end = _sv0t10;
  int _sv0t11 = sv0_vec_len(out);
  if ((_sv0t11 == 0)) {
    int _sv0t12 = TY_UNKNOWN();
    return _sv0t12;
  } else {
  }
  int _sv0t13 = sv0_vec_get(out, 0);
  return _sv0t13;
}

static int item_fn_ty_to_table(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int id1, int id2, int id3, int fn_names, int fn_param_counts, int fn_ret_types, int fn_param_offsets, int fn_param_types_flat, int item_idx) {
  int _sv0t0 = sv0_vec_get(id1, item_idx);
  int name_pos = _sv0t0;
  int _sv0t1 = sv0_vec_get(id2, item_idx);
  int has_ret = _sv0t1;
  int _sv0t2 = sv0_vec_get(id3, item_idx);
  int param_count = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int param_types = _sv0t3;
  int _sv0t4 = scan_fn_param_type_tags(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, name_pos, param_count, param_types);
  int r1 = _sv0t4;
  if ((r1 < 0)) {
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = scan_fn_ret_type_tag(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, name_pos, has_ret);
  int ret_tag = _sv0t6;
  int _sv0t7 = fn_table_add(fn_names, fn_param_counts, fn_ret_types, fn_param_offsets, fn_param_types_flat, name_pos, ret_tag, param_types, param_count);
  int _idx = _sv0t7;
  return 0;
}

static int register_all_item_fns(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int it, int id1, int id2, int id3, int fn_names, int fn_param_counts, int fn_ret_types, int fn_param_offsets, int fn_param_types_flat) {
  int _sv0t0 = sv0_vec_len(it);
  int n = _sv0t0;
  int count = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(it, i);
    if ((_sv0t1 == 0)) {
      int _sv0t2 = item_fn_ty_to_table(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, id1, id2, id3, fn_names, fn_param_counts, fn_ret_types, fn_param_offsets, fn_param_types_flat, i);
      int r = _sv0t2;
      if ((r == 0)) {
        count = (count + 1);
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int scan_fn_param_names(int tok_tags, int name_tok_pos, int param_count, int out_param_name_pos, int out_param_muts) {
  if ((param_count == 0)) {
    return 0;
  } else {
  }
  int p = (name_tok_pos + 1);
  int _sv0t0 = sv0_vec_get(tok_tags, p);
  if ((_sv0t0 == 38)) {
    int depth = 1;
    p = (p + 1);
    while ((depth > 0)) {
      int _sv0t1 = sv0_vec_get(tok_tags, p);
      int u = _sv0t1;
      if ((u == 38)) {
        depth = (depth + 1);
      } else {
      }
      if ((u == 39)) {
        depth = (depth - 1);
      } else {
      }
      p = (p + 1);
    }
  } else {
  }
  int _sv0t2 = sv0_vec_get(tok_tags, p);
  if ((_sv0t2 != 6)) {
    int _sv0t3 = (0 - 1);
    return _sv0t3;
  } else {
  }
  p = (p + 1);
  int found = 0;
  while ((found < param_count)) {
    int is_mut = 0;
    int _sv0t4 = sv0_vec_get(tok_tags, p);
    if ((_sv0t4 == 77)) {
      is_mut = 1;
      p = (p + 1);
    } else {
    }
    int _sv0t5 = sv0_vec_get(tok_tags, p);
    int pt = _sv0t5;
    if ((pt != 5)) {
      if ((pt != 86)) {
        int _sv0t6 = (0 - 1);
        return _sv0t6;
      } else {
      }
    } else {
    }
    sv0_vec_push(out_param_name_pos, p);
    sv0_vec_push(out_param_muts, is_mut);
    p = (p + 1);
    int _sv0t7 = sv0_vec_get(tok_tags, p);
    if ((_sv0t7 != 14)) {
      int _sv0t8 = (0 - 1);
      return _sv0t8;
    } else {
    }
    p = (p + 1);
    int tdepth = 0;
    int done = 0;
    while ((done != 1)) {
      int _sv0t9 = sv0_vec_get(tok_tags, p);
      int tt = _sv0t9;
      if ((tt == 38)) {
        tdepth = (tdepth + 1);
        p = (p + 1);
      } else {
        if ((tt == 39)) {
          tdepth = (tdepth - 1);
          p = (p + 1);
        } else {
          if ((tt == 6)) {
            tdepth = (tdepth + 1);
            p = (p + 1);
          } else {
            if ((tt == 7)) {
              if ((tdepth > 0)) {
                tdepth = (tdepth - 1);
                p = (p + 1);
              } else {
                done = 1;
              }
            } else {
              if ((tt == 12)) {
                if ((tdepth == 0)) {
                  done = 1;
                } else {
                  p = (p + 1);
                }
              } else {
                p = (p + 1);
              }
            }
          }
        }
      }
    }
    found = (found + 1);
    int _sv0t10 = sv0_vec_get(tok_tags, p);
    if ((_sv0t10 == 12)) {
      p = (p + 1);
    } else {
    }
  }
  return 0;
}

static int check_fn_body(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, const char* source, int starts, int ends, int mod_env_names, int mod_env_types, int mod_env_muts, int param_name_pos, int param_type_tags, int param_muts, int param_count, int ret_ty, int body_root_idx, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int fn_names, int fn_ret_types) {
  int _sv0t0 = sv0_vec_new();
  int env_n = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int env_t = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int env_m = _sv0t2;
  int _sv0t3 = sv0_vec_len(mod_env_names);
  int me_len = _sv0t3;
  int j = 0;
  while ((j < me_len)) {
    int _sv0t4 = sv0_vec_get(mod_env_names, j);
    sv0_vec_push(env_n, _sv0t4);
    int _sv0t5 = sv0_vec_get(mod_env_types, j);
    sv0_vec_push(env_t, _sv0t5);
    int _sv0t6 = sv0_vec_get(mod_env_muts, j);
    sv0_vec_push(env_m, _sv0t6);
    j = (j + 1);
  }
  int pi = 0;
  while ((pi < param_count)) {
    int _sv0t7 = sv0_vec_get(param_name_pos, pi);
    int _sv0t8 = sv0_vec_get(param_type_tags, pi);
    int _sv0t9 = sv0_vec_get(param_muts, pi);
    int _sv0t10 = env_extend(env_n, env_t, env_m, _sv0t7, _sv0t8, _sv0t9);
    pi = (pi + 1);
  }
  int body_root = body_root_idx;
  if ((body_root < 0)) {
    int _sv0t11 = (0 - 1);
    return _sv0t11;
  } else {
  }
  int _sv0t12 = sv0_vec_len(bet);
  if ((body_root >= _sv0t12)) {
    int _sv0t13 = (0 - 1);
    return _sv0t13;
  } else {
  }
  int _sv0t14 = synth_expr(bet, bed1, bed2, bed3, bed4, bpp, source, starts, ends, env_n, env_t, env_m, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, 0, body_root);
  int body_ty = _sv0t14;
  if ((body_ty < 0)) {
    int _sv0t15 = (0 - 1);
    return _sv0t15;
  } else {
  }
  int _sv0t16 = expect(body_ty, ret_ty);
  if (_sv0t16) {
    return 0;
  } else {
  }
  int _sv0t17 = TY_UNIT();
  if ((ret_ty == _sv0t17)) {
    return 0;
  } else {
  }
  int _sv0t18 = (0 - 1);
  return _sv0t18;
}

static int process_item_use(const char* source, int starts, int ends, int pp, int id1, int id2, int item_idx, int fn_names, int fn_ret_types, int sdef_names, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int env_names, int env_types, int env_muts, int aliases, int alias_targets) {
  int _sv0t0 = sv0_vec_get(id1, item_idx);
  int pps = _sv0t0;
  int _sv0t1 = sv0_vec_get(id2, item_idx);
  int ppc = _sv0t1;
  if ((ppc != 2)) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_vec_get(pp, pps);
  int m_tok = _sv0t2;
  int _sv0t3 = (pps + 1);
  int _sv0t4 = sv0_vec_get(pp, _sv0t3);
  int nm_tok = _sv0t4;
  int _sv0t5 = sv0_vec_get(starts, m_tok);
  int ms = _sv0t5;
  int _sv0t6 = sv0_vec_get(ends, m_tok);
  int me = _sv0t6;
  int _sv0t7 = (me - ms);
  const char* _sv0t8 = sv0_string_substr(source, ms, _sv0t7);
  const char* m_str;
  m_str = _sv0t8;
  int _sv0t9 = sv0_vec_get(starts, nm_tok);
  int ns = _sv0t9;
  int _sv0t10 = sv0_vec_get(ends, nm_tok);
  int ne = _sv0t10;
  int _sv0t11 = (ne - ns);
  const char* _sv0t12 = sv0_string_substr(source, ns, _sv0t11);
  const char* nm_str;
  nm_str = _sv0t12;
  const char* _sv0t13 = sv0_string_concat(m_str, "__");
  const char* _sv0t14 = sv0_string_concat(_sv0t13, nm_str);
  const char* q;
  q = _sv0t14;
  int _sv0t15 = fn_table_lookup_str(fn_names, source, starts, ends, q);
  int fi = _sv0t15;
  if ((fi >= 0)) {
    int _sv0t16 = TY_FN();
    int _sv0t17 = env_extend(env_names, env_types, env_muts, nm_tok, _sv0t16, 0);
    return 0;
  } else {
  }
  int _sv0t18 = sdef_name_lookup_str(sdef_names, source, starts, ends, q);
  int si = _sv0t18;
  if ((si >= 0)) {
    sv0_vec_push(aliases, nm_tok);
    sv0_vec_push(alias_targets, m_tok);
    return 0;
  } else {
  }
  int _sv0t19 = edef_name_lookup_str(edef_names, source, starts, ends, q);
  int ei = _sv0t19;
  if ((ei >= 0)) {
    sv0_vec_push(aliases, nm_tok);
    sv0_vec_push(alias_targets, m_tok);
    int _sv0t20 = sv0_vec_get(edef_variant_offsets, ei);
    int voff = _sv0t20;
    int _sv0t21 = sv0_vec_get(edef_variant_counts, ei);
    int vc = _sv0t21;
    int vi = 0;
    while ((vi < vc)) {
      int _sv0t22 = (voff + vi);
      int _sv0t23 = sv0_vec_get(edef_vshapes_flat, _sv0t22);
      int vshape = _sv0t23;
      int _sv0t24 = ctor_ty_tag(vshape, 1);
      int cty = _sv0t24;
      int _sv0t25 = (voff + vi);
      int _sv0t26 = sv0_vec_get(edef_vnames_flat, _sv0t25);
      int vn_tok = _sv0t26;
      int _sv0t27 = env_extend(env_names, env_types, env_muts, vn_tok, cty, 0);
      vi = (vi + 1);
    }
    return 0;
  } else {
  }
  return 0;
}

static int build_mod_env(const char* source, int starts, int ends, int pp, int it, int id1, int id2, int id3, int id4, int fn_names, int fn_ret_types, int sdef_names, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int env_names, int env_types, int env_muts, int aliases, int alias_targets) {
  int _sv0t0 = sv0_vec_len(it);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(it, i);
    int tag = _sv0t1;
    if ((tag == 0)) {
      int _sv0t2 = sv0_vec_get(id1, i);
      int np = _sv0t2;
      int _sv0t3 = TY_FN();
      int _sv0t4 = env_extend(env_names, env_types, env_muts, np, _sv0t3, 0);
    } else {
    }
    i = (i + 1);
  }
  i = 0;
  while ((i < n)) {
    int _sv0t5 = sv0_vec_get(it, i);
    int tag = _sv0t5;
    if ((tag == 5)) {
      int _sv0t6 = process_item_use(source, starts, ends, pp, id1, id2, i, fn_names, fn_ret_types, sdef_names, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, env_names, env_types, env_muts, aliases, alias_targets);
      int _r = _sv0t6;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int check_program(int tok_tags, const char* source, int starts, int ends, int pp, int it, int id1, int id2, int id3, int id4, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int type_params, int tp_limit) {
  int _sv0t0 = sv0_vec_new();
  int struct_names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int enum_names = _sv0t1;
  int _sv0t2 = sv0_vec_len(it);
  int nitems = _sv0t2;
  int ni = 0;
  while ((ni < nitems)) {
    int _sv0t3 = sv0_vec_get(it, ni);
    int tg = _sv0t3;
    if ((tg == 1)) {
      int _sv0t4 = sv0_vec_get(id1, ni);
      sv0_vec_push(struct_names, _sv0t4);
    } else {
    }
    if ((tg == 2)) {
      int _sv0t5 = sv0_vec_get(id1, ni);
      sv0_vec_push(enum_names, _sv0t5);
    } else {
    }
    ni = (ni + 1);
  }
  int _sv0t6 = struct_enum_name_clash_str(source, starts, ends, struct_names, enum_names);
  int clash_se = _sv0t6;
  int _sv0t7 = (0 - 1);
  if ((clash_se != _sv0t7)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int sdef_names = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int sdef_field_offsets = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int sdef_field_counts = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int sdef_fnames_flat = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int sdef_ftypes_flat = _sv0t13;
  int _sv0t14 = init_struct_defs(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, it, id1, id2, sdef_names, sdef_field_offsets, sdef_field_counts, sdef_fnames_flat, sdef_ftypes_flat);
  int _sc = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int edef_names = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int edef_variant_offsets = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int edef_variant_counts = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int edef_vnames_flat = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int edef_vshapes_flat = _sv0t19;
  int _sv0t20 = init_enum_defs(tok_tags, source, starts, ends, it, id1, id2, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat);
  int _ec = _sv0t20;
  if ((_ec < 0)) {
    int _sv0t21 = (0 - 1);
    return _sv0t21;
  } else {
  }
  int _sv0t22 = sv0_vec_new();
  int fn_names = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int fn_param_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int fn_ret_types = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int fn_param_offsets = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int fn_param_types_flat = _sv0t26;
  int _sv0t27 = register_all_item_fns(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, it, id1, id2, id3, fn_names, fn_param_counts, fn_ret_types, fn_param_offsets, fn_param_types_flat);
  int _fc = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int mod_env_names = _sv0t28;
  int _sv0t29 = sv0_vec_new();
  int mod_env_types = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int mod_env_muts = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int aliases = _sv0t31;
  int _sv0t32 = sv0_vec_new();
  int alias_targets = _sv0t32;
  int _sv0t33 = build_mod_env(source, starts, ends, pp, it, id1, id2, id3, id4, fn_names, fn_ret_types, sdef_names, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, mod_env_names, mod_env_types, mod_env_muts, aliases, alias_targets);
  int br = _sv0t33;
  if ((br != 0)) {
    int _sv0t34 = (0 - 1);
    return _sv0t34;
  } else {
  }
  int bpp = pp;
  int ii = 0;
  while ((ii < nitems)) {
    int _sv0t35 = sv0_vec_get(it, ii);
    if ((_sv0t35 == 0)) {
      int _sv0t36 = sv0_vec_get(id1, ii);
      int name_pos = _sv0t36;
      int _sv0t37 = sv0_vec_get(id3, ii);
      int param_count = _sv0t37;
      int _sv0t38 = sv0_vec_get(id4, ii);
      int body_root_idx = _sv0t38;
      int _sv0t39 = sv0_vec_new();
      int param_types = _sv0t39;
      int _sv0t40 = scan_fn_param_type_tags(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, name_pos, param_count, param_types);
      int rpt = _sv0t40;
      if ((rpt < 0)) {
        int _sv0t41 = (0 - 1);
        return _sv0t41;
      } else {
      }
      int _sv0t42 = sv0_vec_new();
      int param_name_pos = _sv0t42;
      int _sv0t43 = sv0_vec_new();
      int param_muts = _sv0t43;
      int _sv0t44 = scan_fn_param_names(tok_tags, name_pos, param_count, param_name_pos, param_muts);
      int rpn = _sv0t44;
      if ((rpn < 0)) {
        int _sv0t45 = (0 - 1);
        return _sv0t45;
      } else {
      }
      int _sv0t46 = fn_table_lookup(fn_names, name_pos);
      int fidx = _sv0t46;
      if ((fidx < 0)) {
        int _sv0t47 = (0 - 1);
        return _sv0t47;
      } else {
      }
      int _sv0t48 = fn_table_ret_type(fn_ret_types, fidx);
      int ret_ty = _sv0t48;
      int _sv0t49 = check_fn_body(body_et, body_ed1, body_ed2, body_ed3, body_ed4, bpp, source, starts, ends, mod_env_names, mod_env_types, mod_env_muts, param_name_pos, param_types, param_muts, param_count, ret_ty, body_root_idx, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types);
      int chk = _sv0t49;
      if ((chk != 0)) {
        int _sv0t50 = (0 - 1);
        return _sv0t50;
      } else {
      }
    } else {
    }
    ii = (ii + 1);
  }
  return 0;
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

static int try_success_validate(int variant_tags, int variant_enum_ids, int variant_shapes, int variant_field_counts, int enum_name, const char* source, int starts, int ends, int out_idx) {
  int _sv0t0 = sv0_vec_len(variant_tags);
  int nv = _sv0t0;
  int has_ok = 0;
  int has_err = 0;
  int has_some = 0;
  int has_none = 0;
  int ok_idx = (0 - 1);
  int some_idx = (0 - 1);
  int j = 0;
  while ((j < nv)) {
    int _sv0t1 = sv0_vec_get(variant_enum_ids, j);
    if ((_sv0t1 == enum_name)) {
      int _sv0t2 = sv0_vec_get(variant_tags, j);
      int vh = _sv0t2;
      int _sv0t3 = sv0_vec_get(starts, vh);
      int vs = _sv0t3;
      int _sv0t4 = sv0_vec_get(ends, vh);
      int ve = _sv0t4;
      int _sv0t5 = (ve - vs);
      const char* _sv0t6 = sv0_string_substr(source, vs, _sv0t5);
      const char* vn;
      vn = _sv0t6;
      int _sv0t7 = sv0_string_eq(vn, "Ok");
      if (_sv0t7) {
        has_ok = 1;
        ok_idx = j;
      } else {
      }
      int _sv0t8 = sv0_string_eq(vn, "Err");
      if (_sv0t8) {
        has_err = 1;
      } else {
      }
      int _sv0t9 = sv0_string_eq(vn, "Some");
      if (_sv0t9) {
        has_some = 1;
        some_idx = j;
      } else {
      }
      int _sv0t10 = sv0_string_eq(vn, "None");
      if (_sv0t10) {
        has_none = 1;
      } else {
      }
    } else {
    }
    j = (j + 1);
  }
  if (has_ok) {
    if (has_err) {
      int _sv0t11 = sv0_vec_get(variant_shapes, ok_idx);
      int sh = _sv0t11;
      int _sv0t12 = sv0_vec_get(variant_field_counts, ok_idx);
      int fc = _sv0t12;
      int _sv0t13 = try_success_shape_valid(sh, fc);
      if (_sv0t13) {
        sv0_vec_push(out_idx, ok_idx);
        return 1;
      } else {
      }
      int _sv0t14 = (0 - 1);
      return _sv0t14;
    } else {
    }
  } else {
  }
  if (has_some) {
    if (has_none) {
      int _sv0t15 = sv0_vec_get(variant_shapes, some_idx);
      int sh2 = _sv0t15;
      int _sv0t16 = sv0_vec_get(variant_field_counts, some_idx);
      int fc2 = _sv0t16;
      int _sv0t17 = try_success_shape_valid(sh2, fc2);
      if (_sv0t17) {
        sv0_vec_push(out_idx, some_idx);
        return 2;
      } else {
      }
      int _sv0t18 = (0 - 1);
      return _sv0t18;
    } else {
    }
  } else {
  }
  return 0;
}

static int scan_enum_variant_shapes(int tok_tags, int variant_name_positions, int out_shapes) {
  int _sv0t0 = sv0_vec_len(variant_name_positions);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(variant_name_positions, i);
    int vp = _sv0t1;
    int _sv0t2 = (vp + 1);
    int _sv0t3 = sv0_vec_get(tok_tags, _sv0t2);
    int next = _sv0t3;
    if ((next == 6)) {
      sv0_vec_push(out_shapes, 1);
    } else {
      if ((next == 8)) {
        sv0_vec_push(out_shapes, 2);
      } else {
        sv0_vec_push(out_shapes, 0);
      }
    }
    i = (i + 1);
  }
  return 0;
}

static int init_struct_defs(int tok_tags, const char* source, int starts, int ends, int struct_names, int enum_names, int type_params, int tp_limit, int it, int id1, int id2, int sdef_names, int sdef_field_offsets, int sdef_field_counts, int sdef_fnames_flat, int sdef_ftypes_flat) {
  int _sv0t0 = sv0_vec_len(it);
  int n = _sv0t0;
  int count = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(it, i);
    if ((_sv0t1 == 1)) {
      int _sv0t2 = sv0_vec_get(id1, i);
      int name_pos = _sv0t2;
      int _sv0t3 = sv0_vec_get(id2, i);
      int field_count = _sv0t3;
      sv0_vec_push(sdef_names, name_pos);
      int _sv0t4 = sv0_vec_len(sdef_fnames_flat);
      sv0_vec_push(sdef_field_offsets, _sv0t4);
      sv0_vec_push(sdef_field_counts, field_count);
      if ((field_count > 0)) {
        int _sv0t5 = sv0_vec_new();
        int fnames = _sv0t5;
        int _sv0t6 = scan_struct_field_names(tok_tags, name_pos, field_count, fnames);
        int r0 = _sv0t6;
        if ((r0 == 0)) {
          int _sv0t7 = sv0_vec_new();
          int ftypes = _sv0t7;
          int _sv0t8 = scan_struct_field_type_tags(tok_tags, source, starts, ends, struct_names, enum_names, type_params, tp_limit, fnames, ftypes);
          int r1 = _sv0t8;
          int j = 0;
          while ((j < field_count)) {
            int _sv0t9 = sv0_vec_get(fnames, j);
            sv0_vec_push(sdef_fnames_flat, _sv0t9);
            int _sv0t10 = sv0_vec_get(ftypes, j);
            sv0_vec_push(sdef_ftypes_flat, _sv0t10);
            j = (j + 1);
          }
        } else {
        }
      } else {
      }
      count = (count + 1);
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int init_enum_defs(int tok_tags, const char* source, int starts, int ends, int it, int id1, int id2, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat) {
  int _sv0t0 = sv0_vec_len(it);
  int n = _sv0t0;
  int count = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(it, i);
    if ((_sv0t1 == 2)) {
      int _sv0t2 = sv0_vec_get(id1, i);
      int name_pos = _sv0t2;
      int _sv0t3 = sv0_vec_get(id2, i);
      int variant_count = _sv0t3;
      sv0_vec_push(edef_names, name_pos);
      int _sv0t4 = sv0_vec_len(edef_vnames_flat);
      sv0_vec_push(edef_variant_offsets, _sv0t4);
      sv0_vec_push(edef_variant_counts, variant_count);
      if ((variant_count > 0)) {
        int _sv0t5 = sv0_vec_new();
        int vnames = _sv0t5;
        int _sv0t6 = scan_enum_variant_names(tok_tags, name_pos, variant_count, vnames);
        int r0 = _sv0t6;
        if ((r0 == 0)) {
          int _sv0t7 = dup_variant_names_str(source, starts, ends, vnames);
          int dup_v = _sv0t7;
          int _sv0t8 = (0 - 1);
          if ((dup_v != _sv0t8)) {
            int _sv0t9 = (0 - 1);
            return _sv0t9;
          } else {
          }
          int _sv0t10 = sv0_vec_new();
          int vshapes = _sv0t10;
          int _sv0t11 = scan_enum_variant_shapes(tok_tags, vnames, vshapes);
          int r1 = _sv0t11;
          int j = 0;
          while ((j < variant_count)) {
            int _sv0t12 = sv0_vec_get(vnames, j);
            sv0_vec_push(edef_vnames_flat, _sv0t12);
            int _sv0t13 = sv0_vec_get(vshapes, j);
            sv0_vec_push(edef_vshapes_flat, _sv0t13);
            j = (j + 1);
          }
        } else {
        }
      } else {
      }
      count = (count + 1);
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int struct_def_field_ty_str(int sdef_names, int sdef_field_offsets, int sdef_field_counts, int sdef_fnames_flat, int sdef_ftypes_flat, const char* source, int starts, int ends, int struct_name_pos, const char* field_name_str) {
  int _sv0t0 = sv0_vec_len(sdef_names);
  int ns = _sv0t0;
  int si = (0 - 1);
  int k = 0;
  while ((k < ns)) {
    int _sv0t1 = sv0_vec_get(sdef_names, k);
    if ((_sv0t1 == struct_name_pos)) {
      si = k;
      k = ns;
    } else {
    }
    k = (k + 1);
  }
  if ((si < 0)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(sdef_field_offsets, si);
  int off = _sv0t3;
  int _sv0t4 = sv0_vec_get(sdef_field_counts, si);
  int fc = _sv0t4;
  int f = 0;
  while ((f < fc)) {
    int _sv0t5 = (off + f);
    int _sv0t6 = sv0_vec_get(sdef_fnames_flat, _sv0t5);
    int fnp = _sv0t6;
    int _sv0t7 = sv0_vec_get(starts, fnp);
    int s = _sv0t7;
    int _sv0t8 = sv0_vec_get(ends, fnp);
    int e = _sv0t8;
    int _sv0t9 = (e - s);
    const char* _sv0t10 = sv0_string_substr(source, s, _sv0t9);
    const char* fname;
    fname = _sv0t10;
    int _sv0t11 = sv0_string_eq(fname, field_name_str);
    if (_sv0t11) {
      int _sv0t12 = (off + f);
      int _sv0t13 = sv0_vec_get(sdef_ftypes_flat, _sv0t12);
      return _sv0t13;
    } else {
    }
    f = (f + 1);
  }
  int _sv0t14 = (0 - 1);
  return _sv0t14;
}

static int enum_def_variant_shape_str(int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, const char* source, int starts, int ends, int enum_name_pos, const char* variant_name_str) {
  int _sv0t0 = sv0_vec_len(edef_names);
  int ne = _sv0t0;
  int ei = (0 - 1);
  int k = 0;
  while ((k < ne)) {
    int _sv0t1 = sv0_vec_get(edef_names, k);
    if ((_sv0t1 == enum_name_pos)) {
      ei = k;
      k = ne;
    } else {
    }
    k = (k + 1);
  }
  if ((ei < 0)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(edef_variant_offsets, ei);
  int off = _sv0t3;
  int _sv0t4 = sv0_vec_get(edef_variant_counts, ei);
  int vc = _sv0t4;
  int v = 0;
  while ((v < vc)) {
    int _sv0t5 = (off + v);
    int _sv0t6 = sv0_vec_get(edef_vnames_flat, _sv0t5);
    int vnp = _sv0t6;
    int _sv0t7 = sv0_vec_get(starts, vnp);
    int s = _sv0t7;
    int _sv0t8 = sv0_vec_get(ends, vnp);
    int e = _sv0t8;
    int _sv0t9 = (e - s);
    const char* _sv0t10 = sv0_string_substr(source, s, _sv0t9);
    const char* vname;
    vname = _sv0t10;
    int _sv0t11 = sv0_string_eq(vname, variant_name_str);
    if (_sv0t11) {
      int _sv0t12 = (off + v);
      int _sv0t13 = sv0_vec_get(edef_vshapes_flat, _sv0t12);
      return _sv0t13;
    } else {
    }
    v = (v + 1);
  }
  int _sv0t14 = (0 - 1);
  return _sv0t14;
}

static int BUILTIN_COUNT(void) {
  return 16;
}

static int builtin_fn_lookup(const char* name_str) {
  int _sv0t0 = sv0_string_eq(name_str, "println");
  if (_sv0t0) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name_str, "string_len");
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name_str, "string_eq");
  if (_sv0t2) {
    return 2;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name_str, "string_concat");
  if (_sv0t3) {
    return 3;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name_str, "string_char_at");
  if (_sv0t4) {
    return 4;
  } else {
  }
  int _sv0t5 = sv0_string_eq(name_str, "string_substr");
  if (_sv0t5) {
    return 5;
  } else {
  }
  int _sv0t6 = sv0_string_eq(name_str, "vec_new");
  if (_sv0t6) {
    return 6;
  } else {
  }
  int _sv0t7 = sv0_string_eq(name_str, "vec_push");
  if (_sv0t7) {
    return 7;
  } else {
  }
  int _sv0t8 = sv0_string_eq(name_str, "vec_len");
  if (_sv0t8) {
    return 8;
  } else {
  }
  int _sv0t9 = sv0_string_eq(name_str, "vec_get");
  if (_sv0t9) {
    return 9;
  } else {
  }
  int _sv0t10 = sv0_string_eq(name_str, "vec_set");
  if (_sv0t10) {
    return 10;
  } else {
  }
  int _sv0t11 = sv0_string_eq(name_str, "box_new");
  if (_sv0t11) {
    return 11;
  } else {
  }
  int _sv0t12 = sv0_string_eq(name_str, "box_deref");
  if (_sv0t12) {
    return 12;
  } else {
  }
  int _sv0t13 = sv0_string_eq(name_str, "read_file");
  if (_sv0t13) {
    return 13;
  } else {
  }
  int _sv0t14 = sv0_string_eq(name_str, "write_file");
  if (_sv0t14) {
    return 14;
  } else {
  }
  int _sv0t15 = sv0_string_eq(name_str, "read_dir");
  if (_sv0t15) {
    return 15;
  } else {
  }
  int _sv0t16 = (0 - 1);
  return _sv0t16;
}

static int builtin_fn_ret_type(int bid) {
  if ((bid == 0)) {
    int _sv0t0 = TY_UNIT();
    return _sv0t0;
  } else {
  }
  if ((bid == 1)) {
    int _sv0t1 = TY_INT();
    return _sv0t1;
  } else {
  }
  if ((bid == 2)) {
    int _sv0t2 = TY_BOOL();
    return _sv0t2;
  } else {
  }
  if ((bid == 3)) {
    int _sv0t3 = TY_STRING();
    return _sv0t3;
  } else {
  }
  if ((bid == 4)) {
    int _sv0t4 = TY_INT();
    return _sv0t4;
  } else {
  }
  if ((bid == 5)) {
    int _sv0t5 = TY_STRING();
    return _sv0t5;
  } else {
  }
  if ((bid == 6)) {
    int _sv0t6 = TY_NAMED();
    return _sv0t6;
  } else {
  }
  if ((bid == 7)) {
    int _sv0t7 = TY_UNIT();
    return _sv0t7;
  } else {
  }
  if ((bid == 8)) {
    int _sv0t8 = TY_INT();
    return _sv0t8;
  } else {
  }
  if ((bid == 9)) {
    int _sv0t9 = TY_VAR();
    return _sv0t9;
  } else {
  }
  if ((bid == 10)) {
    int _sv0t10 = TY_UNIT();
    return _sv0t10;
  } else {
  }
  if ((bid == 11)) {
    int _sv0t11 = TY_NAMED();
    return _sv0t11;
  } else {
  }
  if ((bid == 12)) {
    int _sv0t12 = TY_VAR();
    return _sv0t12;
  } else {
  }
  if ((bid == 13)) {
    int _sv0t13 = TY_STRING();
    return _sv0t13;
  } else {
  }
  if ((bid == 14)) {
    int _sv0t14 = TY_UNIT();
    return _sv0t14;
  } else {
  }
  if ((bid == 15)) {
    int _sv0t15 = TY_STRING();
    return _sv0t15;
  } else {
  }
  int _sv0t16 = (0 - 1);
  return _sv0t16;
}

static int builtin_fn_param_count(int bid) {
  if ((bid == 0)) {
    return 1;
  } else {
  }
  if ((bid == 1)) {
    return 1;
  } else {
  }
  if ((bid == 2)) {
    return 2;
  } else {
  }
  if ((bid == 3)) {
    return 2;
  } else {
  }
  if ((bid == 4)) {
    return 2;
  } else {
  }
  if ((bid == 5)) {
    return 3;
  } else {
  }
  if ((bid == 6)) {
    return 0;
  } else {
  }
  if ((bid == 7)) {
    return 2;
  } else {
  }
  if ((bid == 8)) {
    return 1;
  } else {
  }
  if ((bid == 9)) {
    return 2;
  } else {
  }
  if ((bid == 10)) {
    return 3;
  } else {
  }
  if ((bid == 11)) {
    return 1;
  } else {
  }
  if ((bid == 12)) {
    return 1;
  } else {
  }
  if ((bid == 13)) {
    return 1;
  } else {
  }
  if ((bid == 14)) {
    return 2;
  } else {
  }
  if ((bid == 15)) {
    return 1;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int builtin_fn_param_type(int bid, int idx) {
  if ((bid == 0)) {
    int _sv0t0 = TY_STRING();
    return _sv0t0;
  } else {
  }
  if ((bid == 1)) {
    int _sv0t1 = TY_STRING();
    return _sv0t1;
  } else {
  }
  if ((bid == 2)) {
    int _sv0t2 = TY_STRING();
    return _sv0t2;
  } else {
  }
  if ((bid == 3)) {
    int _sv0t3 = TY_STRING();
    return _sv0t3;
  } else {
  }
  if ((bid == 4)) {
    if ((idx == 0)) {
      int _sv0t4 = TY_STRING();
      return _sv0t4;
    } else {
    }
    int _sv0t5 = TY_INT();
    return _sv0t5;
  } else {
  }
  if ((bid == 5)) {
    if ((idx == 0)) {
      int _sv0t6 = TY_STRING();
      return _sv0t6;
    } else {
    }
    int _sv0t7 = TY_INT();
    return _sv0t7;
  } else {
  }
  if ((bid == 7)) {
    if ((idx == 0)) {
      int _sv0t8 = TY_NAMED();
      return _sv0t8;
    } else {
    }
    int _sv0t9 = TY_VAR();
    return _sv0t9;
  } else {
  }
  if ((bid == 8)) {
    int _sv0t10 = TY_NAMED();
    return _sv0t10;
  } else {
  }
  if ((bid == 9)) {
    if ((idx == 0)) {
      int _sv0t11 = TY_NAMED();
      return _sv0t11;
    } else {
    }
    int _sv0t12 = TY_INT();
    return _sv0t12;
  } else {
  }
  if ((bid == 10)) {
    if ((idx == 0)) {
      int _sv0t13 = TY_NAMED();
      return _sv0t13;
    } else {
    }
    if ((idx == 1)) {
      int _sv0t14 = TY_INT();
      return _sv0t14;
    } else {
    }
    int _sv0t15 = TY_VAR();
    return _sv0t15;
  } else {
  }
  if ((bid == 11)) {
    int _sv0t16 = TY_VAR();
    return _sv0t16;
  } else {
  }
  if ((bid == 12)) {
    int _sv0t17 = TY_NAMED();
    return _sv0t17;
  } else {
  }
  if ((bid == 13)) {
    int _sv0t18 = TY_STRING();
    return _sv0t18;
  } else {
  }
  if ((bid == 14)) {
    if ((idx == 0)) {
      int _sv0t19 = TY_STRING();
      return _sv0t19;
    } else {
    }
    int _sv0t20 = TY_STRING();
    return _sv0t20;
  } else {
  }
  if ((bid == 15)) {
    int _sv0t21 = TY_STRING();
    return _sv0t21;
  } else {
  }
  int _sv0t22 = (0 - 1);
  return _sv0t22;
}

static int resolve_fn_call(int fn_names, const char* source, int starts, int ends, const char* name_str, int is_builtin_out) {
  int _sv0t0 = fn_table_lookup_str(fn_names, source, starts, ends, name_str);
  int idx = _sv0t0;
  if ((idx >= 0)) {
    sv0_vec_push(is_builtin_out, 0);
    return idx;
  } else {
  }
  int _sv0t1 = builtin_fn_lookup(name_str);
  int bid = _sv0t1;
  if ((bid >= 0)) {
    sv0_vec_push(is_builtin_out, 1);
    return bid;
  } else {
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int edef_name_lookup_str(int edef_names, const char* source, int starts, int ends, const char* enum_name_str) {
  int _sv0t0 = sv0_vec_len(edef_names);
  int ne = _sv0t0;
  int k = 0;
  while ((k < ne)) {
    int _sv0t1 = sv0_vec_get(edef_names, k);
    int np = _sv0t1;
    int _sv0t2 = sv0_vec_get(starts, np);
    int s = _sv0t2;
    int _sv0t3 = sv0_vec_get(ends, np);
    int e = _sv0t3;
    int _sv0t4 = (e - s);
    const char* _sv0t5 = sv0_string_substr(source, s, _sv0t4);
    const char* nm;
    nm = _sv0t5;
    int _sv0t6 = sv0_string_eq(nm, enum_name_str);
    if (_sv0t6) {
      return k;
    } else {
    }
    k = (k + 1);
  }
  int _sv0t7 = (0 - 1);
  return _sv0t7;
}

static int ctor_type_from_edef(int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, const char* source, int starts, int ends, int enum_name_pos, const char* variant_name_str) {
  int _sv0t0 = enum_def_variant_shape_str(edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, source, starts, ends, enum_name_pos, variant_name_str);
  int sh = _sv0t0;
  if ((sh < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  if ((sh == 0)) {
    int _sv0t2 = TY_ENUM();
    return _sv0t2;
  } else {
  }
  int _sv0t3 = TY_FN();
  return _sv0t3;
}

static int resolve_ctor_path_ty(int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, const char* source, int starts, int ends, int seg1_tok_pos, int seg2_tok_pos) {
  int _sv0t0 = sv0_vec_get(starts, seg1_tok_pos);
  int s1 = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, seg1_tok_pos);
  int e1 = _sv0t1;
  int _sv0t2 = (e1 - s1);
  const char* _sv0t3 = sv0_string_substr(source, s1, _sv0t2);
  const char* enum_str;
  enum_str = _sv0t3;
  int _sv0t4 = edef_name_lookup_str(edef_names, source, starts, ends, enum_str);
  int eidx = _sv0t4;
  if ((eidx < 0)) {
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = sv0_vec_get(starts, seg2_tok_pos);
  int s2 = _sv0t6;
  int _sv0t7 = sv0_vec_get(ends, seg2_tok_pos);
  int e2 = _sv0t7;
  int _sv0t8 = (e2 - s2);
  const char* _sv0t9 = sv0_string_substr(source, s2, _sv0t8);
  const char* variant_str;
  variant_str = _sv0t9;
  int _sv0t10 = sv0_vec_get(edef_names, eidx);
  int enp = _sv0t10;
  int _sv0t11 = ctor_type_from_edef(edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, source, starts, ends, enp, variant_str);
  return _sv0t11;
}

static int synth_expr_call_simple(int et, int ed1, int ed2, int ed3, int ed4, int pp, const char* source, int starts, int ends, int env_names, int env_types, int env_muts, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int fn_names, int fn_ret_types, int ret_ty, int loop_depth, int callee_idx, int args_first, int args_count) {
  int _sv0t0 = sv0_vec_get(et, callee_idx);
  int callee_tag = _sv0t0;
  if ((callee_tag != 1)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(ed1, callee_idx);
  int pps = _sv0t2;
  int _sv0t3 = sv0_vec_get(ed2, callee_idx);
  int ppc = _sv0t3;
  if ((ppc == 1)) {
    int _sv0t4 = sv0_vec_get(pp, pps);
    int tok_pos = _sv0t4;
    int _sv0t5 = sv0_vec_get(starts, tok_pos);
    int s = _sv0t5;
    int _sv0t6 = sv0_vec_get(ends, tok_pos);
    int e = _sv0t6;
    int _sv0t7 = (e - s);
    const char* _sv0t8 = sv0_string_substr(source, s, _sv0t7);
    const char* nm;
    nm = _sv0t8;
    int _sv0t9 = sv0_vec_new();
    int ibo = _sv0t9;
    int _sv0t10 = synth_call_ret_type(fn_names, fn_ret_types, source, starts, ends, nm, ibo);
    int rt = _sv0t10;
    if ((rt < 0)) {
      int _sv0t11 = (0 - 1);
      return _sv0t11;
    } else {
    }
    int ai = 0;
    while ((ai < args_count)) {
      int _sv0t12 = (args_first + ai);
      int _sv0t13 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, _sv0t12);
      int arg_ty = _sv0t13;
      if ((arg_ty < 0)) {
        int _sv0t14 = (0 - 1);
        return _sv0t14;
      } else {
      }
      ai = (ai + 1);
    }
    return rt;
  } else {
  }
  if ((ppc == 2)) {
    int _sv0t15 = sv0_vec_get(pp, pps);
    int seg1 = _sv0t15;
    int _sv0t16 = (pps + 1);
    int _sv0t17 = sv0_vec_get(pp, _sv0t16);
    int seg2 = _sv0t17;
    int _sv0t18 = resolve_ctor_path_ty(edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, source, starts, ends, seg1, seg2);
    int ctor_ty = _sv0t18;
    if ((ctor_ty >= 0)) {
      int ai = 0;
      while ((ai < args_count)) {
        int _sv0t19 = (args_first + ai);
        int _sv0t20 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, _sv0t19);
        int arg_ty = _sv0t20;
        if ((arg_ty < 0)) {
          int _sv0t21 = (0 - 1);
          return _sv0t21;
        } else {
        }
        ai = (ai + 1);
      }
      int _sv0t22 = TY_ENUM();
      return _sv0t22;
    } else {
    }
  } else {
  }
  int _sv0t23 = (0 - 1);
  return _sv0t23;
}

static int sdef_name_lookup_str(int sdef_names, const char* source, int starts, int ends, const char* struct_name_str) {
  int _sv0t0 = sv0_vec_len(sdef_names);
  int ns = _sv0t0;
  int k = 0;
  while ((k < ns)) {
    int _sv0t1 = sv0_vec_get(sdef_names, k);
    int np = _sv0t1;
    int _sv0t2 = sv0_vec_get(starts, np);
    int s = _sv0t2;
    int _sv0t3 = sv0_vec_get(ends, np);
    int e = _sv0t3;
    int _sv0t4 = (e - s);
    const char* _sv0t5 = sv0_string_substr(source, s, _sv0t4);
    const char* nm;
    nm = _sv0t5;
    int _sv0t6 = sv0_string_eq(nm, struct_name_str);
    if (_sv0t6) {
      return k;
    } else {
    }
    k = (k + 1);
  }
  int _sv0t7 = (0 - 1);
  return _sv0t7;
}

static int env_lookup_str(int env_names, int env_types, const char* source, int starts, int ends, const char* target_str) {
  int _sv0t0 = sv0_vec_len(env_names);
  int n = _sv0t0;
  int i = (n - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(env_names, i);
    int np = _sv0t1;
    int _sv0t2 = sv0_vec_get(starts, np);
    int s = _sv0t2;
    int _sv0t3 = sv0_vec_get(ends, np);
    int e = _sv0t3;
    int _sv0t4 = (e - s);
    const char* _sv0t5 = sv0_string_substr(source, s, _sv0t4);
    const char* nm;
    nm = _sv0t5;
    int _sv0t6 = sv0_string_eq(nm, target_str);
    if (_sv0t6) {
      int _sv0t7 = sv0_vec_get(env_types, i);
      return _sv0t7;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t8 = (0 - 1);
  return _sv0t8;
}

static int synth_call_ret_type(int fn_names, int fn_ret_types, const char* source, int starts, int ends, const char* callee_name_str, int is_builtin_out) {
  int _sv0t0 = resolve_fn_call(fn_names, source, starts, ends, callee_name_str, is_builtin_out);
  int fid = _sv0t0;
  if ((fid < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(is_builtin_out);
  int _sv0t3 = (_sv0t2 - 1);
  int _sv0t4 = sv0_vec_get(is_builtin_out, _sv0t3);
  int is_b = _sv0t4;
  if ((is_b != 0)) {
    int _sv0t5 = builtin_fn_ret_type(fid);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = sv0_vec_get(fn_ret_types, fid);
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

static int check_stmt_in_block(int et, int ed1, int ed2, int ed3, int ed4, int pp, const char* source, int starts, int ends, int env_names, int env_types, int env_muts, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int fn_names, int fn_ret_types, int ret_ty, int loop_depth, int stmt_idx) {
  int _sv0t0 = sv0_vec_get(et, stmt_idx);
  int stag = _sv0t0;
  if ((stag == 27)) {
    int _sv0t1 = sv0_vec_get(ed1, stmt_idx);
    int name_tok = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed3, stmt_idx);
    int init_idx = _sv0t2;
    int _sv0t3 = sv0_vec_get(ed4, stmt_idx);
    int is_mut = _sv0t3;
    if ((init_idx < 0)) {
      int _sv0t4 = (0 - 1);
      return _sv0t4;
    } else {
    }
    int _sv0t5 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, init_idx);
    int ty_init = _sv0t5;
    if ((ty_init < 0)) {
      int _sv0t6 = (0 - 1);
      return _sv0t6;
    } else {
    }
    int _sv0t7 = env_extend(env_names, env_types, env_muts, name_tok, ty_init, is_mut);
    return 0;
  } else {
  }
  if ((stag == 28)) {
    int _sv0t8 = sv0_vec_get(ed1, stmt_idx);
    int expr_idx = _sv0t8;
    int _sv0t9 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, expr_idx);
    int discard = _sv0t9;
    if ((discard < 0)) {
      int _sv0t10 = (0 - 1);
      return _sv0t10;
    } else {
    }
    return 0;
  } else {
  }
  int _sv0t11 = (0 - 1);
  return _sv0t11;
}

static int synth_expr(int et, int ed1, int ed2, int ed3, int ed4, int pp, const char* source, int starts, int ends, int env_names, int env_types, int env_muts, int edef_names, int edef_variant_offsets, int edef_variant_counts, int edef_vnames_flat, int edef_vshapes_flat, int fn_names, int fn_ret_types, int ret_ty, int loop_depth, int idx) {
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 0)) {
    int _sv0t1 = sv0_vec_get(ed1, idx);
    int lit_tag = _sv0t1;
    int _sv0t2 = infer_lit(lit_tag);
    return _sv0t2;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t3 = sv0_vec_get(ed1, idx);
    int pps = _sv0t3;
    int _sv0t4 = sv0_vec_get(ed2, idx);
    int ppc = _sv0t4;
    if ((ppc == 1)) {
      int _sv0t5 = sv0_vec_get(pp, pps);
      int tok_pos = _sv0t5;
      int _sv0t6 = sv0_vec_get(starts, tok_pos);
      int s = _sv0t6;
      int _sv0t7 = sv0_vec_get(ends, tok_pos);
      int e = _sv0t7;
      int _sv0t8 = (e - s);
      const char* _sv0t9 = sv0_string_substr(source, s, _sv0t8);
      const char* nm;
      nm = _sv0t9;
      int _sv0t10 = env_lookup_str(env_names, env_types, source, starts, ends, nm);
      return _sv0t10;
    } else {
    }
    if ((ppc == 2)) {
      int _sv0t11 = sv0_vec_get(pp, pps);
      int seg1 = _sv0t11;
      int _sv0t12 = (pps + 1);
      int _sv0t13 = sv0_vec_get(pp, _sv0t12);
      int seg2 = _sv0t13;
      int _sv0t14 = resolve_ctor_path_ty(edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, source, starts, ends, seg1, seg2);
      return _sv0t14;
    } else {
    }
    int _sv0t15 = (0 - 1);
    return _sv0t15;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t16 = sv0_vec_get(ed1, idx);
    int unop_tag = _sv0t16;
    int _sv0t17 = sv0_vec_get(ed2, idx);
    int operand_idx = _sv0t17;
    int _sv0t18 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, operand_idx);
    int op_ty = _sv0t18;
    if ((op_ty < 0)) {
      int _sv0t19 = (0 - 1);
      return _sv0t19;
    } else {
    }
    int _sv0t20 = unop_result_ty_tag(unop_tag);
    return _sv0t20;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t21 = sv0_vec_get(ed1, idx);
    int binop_tag = _sv0t21;
    int _sv0t22 = sv0_vec_get(ed2, idx);
    int lhs_idx = _sv0t22;
    int _sv0t23 = sv0_vec_get(ed3, idx);
    int rhs_idx = _sv0t23;
    int _sv0t24 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, lhs_idx);
    int lhs_ty = _sv0t24;
    if ((lhs_ty < 0)) {
      int _sv0t25 = (0 - 1);
      return _sv0t25;
    } else {
    }
    int _sv0t26 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, rhs_idx);
    int rhs_ty = _sv0t26;
    if ((rhs_ty < 0)) {
      int _sv0t27 = (0 - 1);
      return _sv0t27;
    } else {
    }
    int _sv0t28 = binop_result_ty_tag(binop_tag);
    return _sv0t28;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t29 = sv0_vec_get(ed1, idx);
    int callee_idx = _sv0t29;
    int _sv0t30 = sv0_vec_get(ed2, idx);
    int args_first = _sv0t30;
    int _sv0t31 = sv0_vec_get(ed3, idx);
    int args_count = _sv0t31;
    int _sv0t32 = synth_expr_call_simple(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, callee_idx, args_first, args_count);
    return _sv0t32;
  } else {
  }
  if ((tag == 5)) {
    int _sv0t33 = sv0_vec_get(ed1, idx);
    int mc_obj = _sv0t33;
    int _sv0t34 = sv0_vec_get(ed3, idx);
    int mc_args_first = _sv0t34;
    int _sv0t35 = sv0_vec_get(ed4, idx);
    int mc_args_count = _sv0t35;
    int _sv0t36 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, mc_obj);
    int mc_obj_ty = _sv0t36;
    if ((mc_obj_ty < 0)) {
      int _sv0t37 = (0 - 1);
      return _sv0t37;
    } else {
    }
    int mc_i = 0;
    while ((mc_i < mc_args_count)) {
      int _sv0t38 = (mc_args_first + mc_i);
      int _sv0t39 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, _sv0t38);
      int mc_arg_ty = _sv0t39;
      if ((mc_arg_ty < 0)) {
        int _sv0t40 = (0 - 1);
        return _sv0t40;
      } else {
      }
      mc_i = (mc_i + 1);
    }
    int _sv0t41 = TY_INT();
    return _sv0t41;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t42 = sv0_vec_get(ed1, idx);
    int fld_obj = _sv0t42;
    int _sv0t43 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, fld_obj);
    int fld_obj_ty = _sv0t43;
    if ((fld_obj_ty < 0)) {
      int _sv0t44 = (0 - 1);
      return _sv0t44;
    } else {
    }
    int _sv0t45 = TY_INT();
    return _sv0t45;
  } else {
  }
  if ((tag == 7)) {
    int _sv0t46 = sv0_vec_get(ed1, idx);
    int tf_obj = _sv0t46;
    int _sv0t47 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, tf_obj);
    int tf_obj_ty = _sv0t47;
    if ((tf_obj_ty < 0)) {
      int _sv0t48 = (0 - 1);
      return _sv0t48;
    } else {
    }
    int _sv0t49 = TY_INT();
    return _sv0t49;
  } else {
  }
  if ((tag == 8)) {
    int _sv0t50 = sv0_vec_get(ed1, idx);
    int obj_idx = _sv0t50;
    int _sv0t51 = sv0_vec_get(ed2, idx);
    int index_idx = _sv0t51;
    int _sv0t52 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, obj_idx);
    int obj_ty = _sv0t52;
    if ((obj_ty < 0)) {
      int _sv0t53 = (0 - 1);
      return _sv0t53;
    } else {
    }
    int _sv0t54 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, index_idx);
    int idx_ty = _sv0t54;
    if ((idx_ty < 0)) {
      int _sv0t55 = (0 - 1);
      return _sv0t55;
    } else {
    }
    int _sv0t56 = TY_INT();
    return _sv0t56;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t57 = sv0_vec_get(ed1, idx);
    int cond_idx = _sv0t57;
    int _sv0t58 = sv0_vec_get(ed2, idx);
    int then_idx = _sv0t58;
    int _sv0t59 = sv0_vec_get(ed3, idx);
    int else_idx = _sv0t59;
    int _sv0t60 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, cond_idx);
    int cond_ty = _sv0t60;
    if ((cond_ty < 0)) {
      int _sv0t61 = (0 - 1);
      return _sv0t61;
    } else {
    }
    int _sv0t62 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, then_idx);
    int then_ty = _sv0t62;
    if ((then_ty < 0)) {
      int _sv0t63 = (0 - 1);
      return _sv0t63;
    } else {
    }
    if ((else_idx < 0)) {
      int _sv0t64 = TY_UNIT();
      return _sv0t64;
    } else {
    }
    int _sv0t65 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, else_idx);
    int else_ty = _sv0t65;
    if ((else_ty < 0)) {
      int _sv0t66 = (0 - 1);
      return _sv0t66;
    } else {
    }
    if ((then_ty == else_ty)) {
      return then_ty;
    } else {
    }
    int _sv0t67 = TY_UNIT();
    return _sv0t67;
  } else {
  }
  if ((tag == 15)) {
    int _sv0t68 = sv0_vec_get(ed1, idx);
    int val_idx = _sv0t68;
    if ((val_idx < 0)) {
      return ret_ty;
    } else {
    }
    int _sv0t69 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, val_idx);
    int val_ty = _sv0t69;
    if ((val_ty < 0)) {
      int _sv0t70 = (0 - 1);
      return _sv0t70;
    } else {
    }
    return ret_ty;
  } else {
  }
  if ((tag == 16)) {
    if ((loop_depth <= 0)) {
      int _sv0t71 = (0 - 1);
      return _sv0t71;
    } else {
    }
    int _sv0t72 = TY_UNIT();
    return _sv0t72;
  } else {
  }
  if ((tag == 17)) {
    if ((loop_depth <= 0)) {
      int _sv0t73 = (0 - 1);
      return _sv0t73;
    } else {
    }
    int _sv0t74 = TY_UNIT();
    return _sv0t74;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t75 = sv0_vec_get(ed1, idx);
    int scrut_idx = _sv0t75;
    int _sv0t76 = sv0_vec_get(ed2, idx);
    int arms_first = _sv0t76;
    int _sv0t77 = sv0_vec_get(ed3, idx);
    int arms_count = _sv0t77;
    int _sv0t78 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, scrut_idx);
    int scrut_ty = _sv0t78;
    if ((scrut_ty < 0)) {
      int _sv0t79 = (0 - 1);
      return _sv0t79;
    } else {
    }
    int _sv0t80 = sv0_vec_get(ed4, idx);
    int arms_sidecar = _sv0t80;
    int _sv0t81 = TY_INT();
    int first_body_ty = _sv0t81;
    int ai = 0;
    while ((ai < arms_count)) {
      int _sv0t82 = block_stmt_index(pp, arms_first, arms_sidecar, ai);
      int arm_idx = _sv0t82;
      int _sv0t83 = sv0_vec_get(ed3, arm_idx);
      int guard_idx = _sv0t83;
      int _sv0t84 = sv0_vec_get(ed4, arm_idx);
      int body_idx = _sv0t84;
      int _sv0t85 = sv0_vec_get(ed1, arm_idx);
      int arm_pt = _sv0t85;
      if ((arm_pt == 5)) {
        int _sv0t86 = sv0_vec_get(ed2, arm_idx);
        int apx = _sv0t86;
        int _sv0t87 = sv0_vec_get(ed3, apx);
        int apc = _sv0t87;
        if ((apc > 0)) {
          int apm = (apx - 1);
          int _sv0t88 = sv0_vec_get(et, apm);
          if ((_sv0t88 == 33)) {
            int _sv0t89 = sv0_vec_get(ed1, apm);
            int apac = _sv0t89;
            int _sv0t90 = sv0_vec_get(ed2, apm);
            int apfe = _sv0t90;
            int apj = 0;
            while ((apj < apac)) {
              int apei = (apfe + apj);
              int _sv0t91 = sv0_vec_get(et, apei);
              if ((_sv0t91 == 31)) {
                int _sv0t92 = sv0_vec_get(ed4, apei);
                if ((_sv0t92 == 1)) {
                  int _sv0t93 = sv0_vec_get(ed1, apei);
                  int _sv0t94 = TY_INT();
                  int _sv0t95 = env_extend(env_names, env_types, env_muts, _sv0t93, _sv0t94, 0);
                } else {
                }
              } else {
              }
              apj = (apj + 1);
            }
          } else {
          }
        } else {
        }
      } else {
      }
      if ((guard_idx >= 0)) {
        int _sv0t96 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, guard_idx);
        int g_ty = _sv0t96;
        if ((g_ty < 0)) {
          int _sv0t97 = (0 - 1);
          return _sv0t97;
        } else {
        }
      } else {
      }
      int _sv0t98 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, body_idx);
      int b_ty = _sv0t98;
      if ((b_ty < 0)) {
        int _sv0t99 = (0 - 1);
        return _sv0t99;
      } else {
      }
      if ((ai == 0)) {
        first_body_ty = b_ty;
      } else {
      }
      ai = (ai + 1);
    }
    return first_body_ty;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t100 = sv0_vec_get(ed1, idx);
    int cond_idx = _sv0t100;
    int _sv0t101 = sv0_vec_get(ed2, idx);
    int body_idx = _sv0t101;
    int _sv0t102 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, cond_idx);
    int cond_ty = _sv0t102;
    if ((cond_ty < 0)) {
      int _sv0t103 = (0 - 1);
      return _sv0t103;
    } else {
    }
    int _sv0t104 = (loop_depth + 1);
    int _sv0t105 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, _sv0t104, body_idx);
    int body_ty = _sv0t105;
    if ((body_ty < 0)) {
      int _sv0t106 = (0 - 1);
      return _sv0t106;
    } else {
    }
    int _sv0t107 = TY_UNIT();
    return _sv0t107;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t108 = sv0_vec_get(ed1, idx);
    int pat_tok = _sv0t108;
    int _sv0t109 = sv0_vec_get(ed2, idx);
    int iter_idx = _sv0t109;
    int _sv0t110 = sv0_vec_get(ed3, idx);
    int fbody_idx = _sv0t110;
    int _sv0t111 = sv0_vec_get(et, iter_idx);
    int rtag = _sv0t111;
    if ((rtag != 21)) {
      int _sv0t112 = (0 - 1);
      return _sv0t112;
    } else {
    }
    int _sv0t113 = sv0_vec_get(ed1, iter_idx);
    int lo_idx = _sv0t113;
    int _sv0t114 = sv0_vec_get(ed2, iter_idx);
    int hi_idx = _sv0t114;
    if ((lo_idx >= 0)) {
      int _sv0t115 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, lo_idx);
      int lo_ty = _sv0t115;
      if ((lo_ty < 0)) {
        int _sv0t116 = (0 - 1);
        return _sv0t116;
      } else {
      }
    } else {
    }
    if ((hi_idx >= 0)) {
      int _sv0t117 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, hi_idx);
      int hi_ty = _sv0t117;
      if ((hi_ty < 0)) {
        int _sv0t118 = (0 - 1);
        return _sv0t118;
      } else {
      }
    } else {
    }
    int _sv0t119 = TY_INT();
    int _sv0t120 = env_extend(env_names, env_types, env_muts, pat_tok, _sv0t119, 0);
    int _sv0t121 = (loop_depth + 1);
    int _sv0t122 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, _sv0t121, fbody_idx);
    int fbody_ty = _sv0t122;
    if ((fbody_ty < 0)) {
      int _sv0t123 = (0 - 1);
      return _sv0t123;
    } else {
    }
    int _sv0t124 = TY_UNIT();
    return _sv0t124;
  } else {
  }
  if ((tag == 14)) {
    int _sv0t125 = sv0_vec_get(ed1, idx);
    int body_idx = _sv0t125;
    int _sv0t126 = (loop_depth + 1);
    int _sv0t127 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, _sv0t126, body_idx);
    int body_ty = _sv0t127;
    if ((body_ty < 0)) {
      int _sv0t128 = (0 - 1);
      return _sv0t128;
    } else {
    }
    int _sv0t129 = TY_UNIT();
    return _sv0t129;
  } else {
  }
  if ((tag == 18)) {
    int _sv0t130 = sv0_vec_get(ed1, idx);
    int lhs_idx = _sv0t130;
    int _sv0t131 = sv0_vec_get(ed2, idx);
    int rhs_idx = _sv0t131;
    int _sv0t132 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, lhs_idx);
    int lhs_ty = _sv0t132;
    if ((lhs_ty < 0)) {
      int _sv0t133 = (0 - 1);
      return _sv0t133;
    } else {
    }
    int _sv0t134 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, rhs_idx);
    int rhs_ty = _sv0t134;
    if ((rhs_ty < 0)) {
      int _sv0t135 = (0 - 1);
      return _sv0t135;
    } else {
    }
    int _sv0t136 = TY_UNIT();
    return _sv0t136;
  } else {
  }
  if ((tag == 19)) {
    int _sv0t137 = sv0_vec_get(ed2, idx);
    int lhs_idx = _sv0t137;
    int _sv0t138 = sv0_vec_get(ed3, idx);
    int rhs_idx = _sv0t138;
    int _sv0t139 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, lhs_idx);
    int lhs_ty = _sv0t139;
    if ((lhs_ty < 0)) {
      int _sv0t140 = (0 - 1);
      return _sv0t140;
    } else {
    }
    int _sv0t141 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, rhs_idx);
    int rhs_ty = _sv0t141;
    if ((rhs_ty < 0)) {
      int _sv0t142 = (0 - 1);
      return _sv0t142;
    } else {
    }
    int _sv0t143 = TY_UNIT();
    return _sv0t143;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t144 = sv0_vec_get(ed1, idx);
    int cast_src = _sv0t144;
    int _sv0t145 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, cast_src);
    int cast_ty = _sv0t145;
    if ((cast_ty < 0)) {
      int _sv0t146 = (0 - 1);
      return _sv0t146;
    } else {
    }
    int _sv0t147 = TY_INT();
    return _sv0t147;
  } else {
  }
  if ((tag == 21)) {
    int _sv0t148 = sv0_vec_get(ed1, idx);
    int rlo = _sv0t148;
    int _sv0t149 = sv0_vec_get(ed2, idx);
    int rhi = _sv0t149;
    if ((rlo >= 0)) {
      int _sv0t150 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, rlo);
      int rlo_ty = _sv0t150;
      if ((rlo_ty < 0)) {
        int _sv0t151 = (0 - 1);
        return _sv0t151;
      } else {
      }
    } else {
    }
    if ((rhi >= 0)) {
      int _sv0t152 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, rhi);
      int rhi_ty = _sv0t152;
      if ((rhi_ty < 0)) {
        int _sv0t153 = (0 - 1);
        return _sv0t153;
      } else {
      }
    } else {
    }
    int _sv0t154 = TY_INT();
    return _sv0t154;
  } else {
  }
  if ((tag == 22)) {
    int _sv0t155 = sv0_vec_get(ed1, idx);
    int try_src = _sv0t155;
    int _sv0t156 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, try_src);
    int try_ty = _sv0t156;
    if ((try_ty < 0)) {
      int _sv0t157 = (0 - 1);
      return _sv0t157;
    } else {
    }
    int _sv0t158 = TY_INT();
    return _sv0t158;
  } else {
  }
  if ((tag == 23)) {
    int _sv0t159 = sv0_vec_get(ed1, idx);
    int expr_idx = _sv0t159;
    int _sv0t160 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, expr_idx);
    int expr_ty = _sv0t160;
    if ((expr_ty < 0)) {
      int _sv0t161 = (0 - 1);
      return _sv0t161;
    } else {
    }
    int _sv0t162 = TY_UNIT();
    return _sv0t162;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t163 = TY_INT();
    return _sv0t163;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t164 = sv0_vec_get(ed1, idx);
    int first_idx = _sv0t164;
    int _sv0t165 = sv0_vec_get(ed2, idx);
    int count = _sv0t165;
    if ((count == 1)) {
      int _sv0t166 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, first_idx);
      return _sv0t166;
    } else {
    }
    int _sv0t167 = (0 - 1);
    return _sv0t167;
  } else {
  }
  if ((tag == 26)) {
    int _sv0t168 = TY_INT();
    return _sv0t168;
  } else {
  }
  if ((tag == 9)) {
    int _sv0t169 = sv0_vec_get(ed1, idx);
    int stmts_first = _sv0t169;
    int _sv0t170 = sv0_vec_get(ed2, idx);
    int stmts_count = _sv0t170;
    int _sv0t171 = sv0_vec_get(ed3, idx);
    int tail_idx = _sv0t171;
    int _sv0t172 = sv0_vec_get(ed4, idx);
    int sidecar = _sv0t172;
    int si = 0;
    while ((si < stmts_count)) {
      int _sv0t173 = block_stmt_index(pp, stmts_first, sidecar, si);
      int _sv0t174 = check_stmt_in_block(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, _sv0t173);
      int rc = _sv0t174;
      if ((rc < 0)) {
        int _sv0t175 = (0 - 1);
        return _sv0t175;
      } else {
      }
      si = (si + 1);
    }
    if ((tail_idx >= 0)) {
      int _sv0t176 = synth_expr(et, ed1, ed2, ed3, ed4, pp, source, starts, ends, env_names, env_types, env_muts, edef_names, edef_variant_offsets, edef_variant_counts, edef_vnames_flat, edef_vshapes_flat, fn_names, fn_ret_types, ret_ty, loop_depth, tail_idx);
      return _sv0t176;
    } else {
    }
    if ((stmts_count > 0)) {
      int _sv0t177 = (stmts_count - 1);
      int _sv0t178 = block_stmt_index(pp, stmts_first, sidecar, _sv0t177);
      int last_idx = _sv0t178;
      int _sv0t179 = stmt_returns_arena(et, ed1, last_idx);
      if (_sv0t179) {
        int _sv0t180 = TY_VAR();
        return _sv0t180;
      } else {
      }
    } else {
    }
    int _sv0t181 = TY_UNIT();
    return _sv0t181;
  } else {
  }
  if ((tag == 30)) {
    int _sv0t182 = TY_UNIT();
    return _sv0t182;
  } else {
  }
  if ((tag == 31)) {
    int _sv0t183 = TY_UNIT();
    return _sv0t183;
  } else {
  }
  if ((tag == 32)) {
    int _sv0t184 = TY_UNIT();
    return _sv0t184;
  } else {
  }
  if ((tag == 33)) {
    int _sv0t185 = TY_UNIT();
    return _sv0t185;
  } else {
  }
  int _sv0t186 = (0 - 1);
  return _sv0t186;
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
  int _sv0t1 = TY_INT();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = infer_lit(1);
  int _sv0t3 = TY_BOOL();
  if ((_sv0t2 != _sv0t3)) {
    return 2;
  } else {
  }
  int _sv0t4 = infer_lit(2);
  int _sv0t5 = TY_UNIT();
  if ((_sv0t4 != _sv0t5)) {
    return 3;
  } else {
  }
  int _sv0t6 = infer_lit(3);
  int _sv0t7 = TY_STRING();
  if ((_sv0t6 != _sv0t7)) {
    return 4;
  } else {
  }
  int _sv0t8 = infer_lit(4);
  int _sv0t9 = TY_CHAR();
  if ((_sv0t8 != _sv0t9)) {
    return 5;
  } else {
  }
  int _sv0t10 = infer_lit(99);
  int _sv0t11 = (0 - 1);
  if ((_sv0t10 != _sv0t11)) {
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
  const char* src_dv;
  src_dv = "Red Blue Red";
  int _sv0t8 = sv0_vec_new();
  int st_dv = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int en_dv = _sv0t9;
  sv0_vec_push(st_dv, 0);
  sv0_vec_push(en_dv, 3);
  sv0_vec_push(st_dv, 4);
  sv0_vec_push(en_dv, 8);
  sv0_vec_push(st_dv, 9);
  sv0_vec_push(en_dv, 12);
  int _sv0t10 = sv0_vec_new();
  int vn_dv = _sv0t10;
  sv0_vec_push(vn_dv, 0);
  sv0_vec_push(vn_dv, 1);
  sv0_vec_push(vn_dv, 2);
  int _sv0t11 = dup_variant_names_str(src_dv, st_dv, en_dv, vn_dv);
  if ((_sv0t11 != 2)) {
    return 4;
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
  const char* src_se;
  src_se = "Point Point";
  int _sv0t9 = sv0_vec_new();
  int st_se = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_se = _sv0t10;
  sv0_vec_push(st_se, 0);
  sv0_vec_push(en_se, 5);
  sv0_vec_push(st_se, 6);
  sv0_vec_push(en_se, 11);
  int _sv0t11 = sv0_vec_new();
  int sns = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int ens = _sv0t12;
  sv0_vec_push(sns, 0);
  sv0_vec_push(ens, 1);
  int _sv0t13 = struct_enum_name_clash_str(src_se, st_se, en_se, sns, ens);
  int _sv0t14 = (0 - 1);
  if ((_sv0t13 == _sv0t14)) {
    return 4;
  } else {
  }
  const char* src_nc;
  src_nc = "Point Color";
  int _sv0t15 = sv0_vec_new();
  int st_nc = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int en_nc = _sv0t16;
  sv0_vec_push(st_nc, 0);
  sv0_vec_push(en_nc, 5);
  sv0_vec_push(st_nc, 6);
  sv0_vec_push(en_nc, 11);
  int _sv0t17 = sv0_vec_new();
  int sns_nc = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int ens_nc = _sv0t18;
  sv0_vec_push(sns_nc, 0);
  sv0_vec_push(ens_nc, 1);
  int _sv0t19 = struct_enum_name_clash_str(src_nc, st_nc, en_nc, sns_nc, ens_nc);
  int _sv0t20 = (0 - 1);
  if ((_sv0t19 != _sv0t20)) {
    return 5;
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

static int test_try_success_validate(void) {
  int _sv0t0 = sv0_vec_new();
  int vt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int vei = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int vs = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int vfc = _sv0t3;
  const char* src;
  src = "OkErrSomeNoneFoo";
  int _sv0t4 = sv0_vec_new();
  int st = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int en = _sv0t5;
  sv0_vec_push(st, 0);
  sv0_vec_push(en, 2);
  sv0_vec_push(st, 2);
  sv0_vec_push(en, 5);
  sv0_vec_push(st, 5);
  sv0_vec_push(en, 9);
  sv0_vec_push(st, 9);
  sv0_vec_push(en, 13);
  sv0_vec_push(st, 13);
  sv0_vec_push(en, 16);
  sv0_vec_push(vt, 0);
  sv0_vec_push(vei, 100);
  sv0_vec_push(vs, 1);
  sv0_vec_push(vfc, 1);
  sv0_vec_push(vt, 1);
  sv0_vec_push(vei, 100);
  sv0_vec_push(vs, 1);
  sv0_vec_push(vfc, 1);
  int _sv0t6 = sv0_vec_new();
  int oi1 = _sv0t6;
  int _sv0t7 = try_success_validate(vt, vei, vs, vfc, 100, src, st, en, oi1);
  int r1 = _sv0t7;
  if ((r1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_len(oi1);
  if ((_sv0t8 != 1)) {
    return 2;
  } else {
  }
  int _sv0t9 = sv0_vec_get(oi1, 0);
  if ((_sv0t9 != 0)) {
    return 3;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int vt2 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int vei2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int vs2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int vfc2 = _sv0t13;
  sv0_vec_push(vt2, 2);
  sv0_vec_push(vei2, 200);
  sv0_vec_push(vs2, 1);
  sv0_vec_push(vfc2, 1);
  sv0_vec_push(vt2, 3);
  sv0_vec_push(vei2, 200);
  sv0_vec_push(vs2, 0);
  sv0_vec_push(vfc2, 0);
  int _sv0t14 = sv0_vec_new();
  int oi2 = _sv0t14;
  int _sv0t15 = try_success_validate(vt2, vei2, vs2, vfc2, 200, src, st, en, oi2);
  int r2 = _sv0t15;
  if ((r2 != 2)) {
    return 4;
  } else {
  }
  int _sv0t16 = sv0_vec_new();
  int vt3 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int vei3 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int vs3 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int vfc3 = _sv0t19;
  sv0_vec_push(vt3, 4);
  sv0_vec_push(vei3, 300);
  sv0_vec_push(vs3, 0);
  sv0_vec_push(vfc3, 0);
  int _sv0t20 = sv0_vec_new();
  int oi3 = _sv0t20;
  int _sv0t21 = try_success_validate(vt3, vei3, vs3, vfc3, 300, src, st, en, oi3);
  int r3 = _sv0t21;
  if ((r3 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_binop_unop_result_ty(void) {
  int _sv0t0 = binop_result_ty_tag(0);
  int _sv0t1 = TY_INT();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = binop_result_ty_tag(1);
  int _sv0t3 = TY_INT();
  if ((_sv0t2 != _sv0t3)) {
    return 2;
  } else {
  }
  int _sv0t4 = binop_result_ty_tag(11);
  int _sv0t5 = TY_BOOL();
  if ((_sv0t4 != _sv0t5)) {
    return 3;
  } else {
  }
  int _sv0t6 = binop_result_ty_tag(5);
  int _sv0t7 = TY_BOOL();
  if ((_sv0t6 != _sv0t7)) {
    return 4;
  } else {
  }
  int _sv0t8 = unop_result_ty_tag(0);
  int _sv0t9 = TY_INT();
  if ((_sv0t8 != _sv0t9)) {
    return 5;
  } else {
  }
  int _sv0t10 = unop_result_ty_tag(1);
  int _sv0t11 = TY_BOOL();
  if ((_sv0t10 != _sv0t11)) {
    return 6;
  } else {
  }
  int _sv0t12 = unop_result_ty_tag(2);
  int _sv0t13 = TY_INT();
  if ((_sv0t12 != _sv0t13)) {
    return 7;
  } else {
  }
  int _sv0t14 = unop_result_ty_tag(99);
  int _sv0t15 = TY_UNKNOWN();
  if ((_sv0t14 != _sv0t15)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_int_width_from_name(void) {
  int _sv0t0 = int_width_from_name("i32");
  if ((_sv0t0 != 32)) {
    return 1;
  } else {
  }
  int _sv0t1 = int_width_from_name("u64");
  if ((_sv0t1 != 64)) {
    return 2;
  } else {
  }
  int _sv0t2 = int_width_from_name("i8");
  if ((_sv0t2 != 8)) {
    return 3;
  } else {
  }
  int _sv0t3 = int_width_from_name("f64");
  if ((_sv0t3 != 64)) {
    return 4;
  } else {
  }
  int _sv0t4 = int_width_from_name("bool");
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = int_width_from_name("string");
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = int_width_from_name("i128");
  if ((_sv0t6 != 128)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_ast_ty_payload(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int td1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int td2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int td3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  const char* src;
  src = "i32MyStruct";
  int _sv0t5 = sv0_vec_new();
  int st = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int en = _sv0t6;
  sv0_vec_push(st, 0);
  sv0_vec_push(en, 3);
  sv0_vec_push(st, 3);
  sv0_vec_push(en, 11);
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(tt, 0);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 0);
  int _sv0t7 = sv0_vec_new();
  int sn = _sv0t7;
  sv0_vec_push(sn, 1);
  int _sv0t8 = sv0_vec_new();
  int enn = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int tp = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int op1 = _sv0t10;
  int _sv0t11 = ast_ty_to_ty_payload(tt, td1, td2, td3, pp, src, st, en, sn, enn, tp, 0, 0, op1);
  int r1 = _sv0t11;
  int _sv0t12 = TY_INT();
  if ((r1 != _sv0t12)) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_vec_len(op1);
  if ((_sv0t13 != 1)) {
    return 2;
  } else {
  }
  int _sv0t14 = sv0_vec_get(op1, 0);
  if ((_sv0t14 != 32)) {
    return 3;
  } else {
  }
  int _sv0t15 = sv0_vec_new();
  int op2 = _sv0t15;
  sv0_vec_push(tt, 0);
  sv0_vec_push(td1, 1);
  sv0_vec_push(td2, 1);
  sv0_vec_push(td3, 0);
  int _sv0t16 = ast_ty_to_ty_payload(tt, td1, td2, td3, pp, src, st, en, sn, enn, tp, 0, 1, op2);
  int r2 = _sv0t16;
  int _sv0t17 = TY_STRUCT();
  if ((r2 != _sv0t17)) {
    return 4;
  } else {
  }
  int _sv0t18 = sv0_vec_len(op2);
  if ((_sv0t18 != 1)) {
    return 5;
  } else {
  }
  int _sv0t19 = sv0_vec_get(op2, 0);
  if ((_sv0t19 != 1)) {
    return 6;
  } else {
  }
  int _sv0t20 = sv0_vec_new();
  int op3 = _sv0t20;
  sv0_vec_push(tt, 6);
  sv0_vec_push(td1, 0);
  sv0_vec_push(td2, 0);
  sv0_vec_push(td3, 0);
  int _sv0t21 = ast_ty_to_ty_payload(tt, td1, td2, td3, pp, src, st, en, sn, enn, tp, 0, 2, op3);
  int r3 = _sv0t21;
  int _sv0t22 = TY_UNIT();
  if ((r3 != _sv0t22)) {
    return 7;
  } else {
  }
  int _sv0t23 = sv0_vec_len(op3);
  if ((_sv0t23 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_enum_ctor_count(void) {
  int _sv0t0 = sv0_vec_new();
  int vei = _sv0t0;
  sv0_vec_push(vei, 100);
  sv0_vec_push(vei, 100);
  sv0_vec_push(vei, 200);
  sv0_vec_push(vei, 100);
  int _sv0t1 = enum_ctor_count(vei, 100);
  if ((_sv0t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t2 = enum_ctor_count(vei, 200);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = enum_ctor_count(vei, 999);
  if ((_sv0t3 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_env_entry_index(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 10);
  sv0_vec_push(names, 20);
  sv0_vec_push(names, 30);
  int _sv0t1 = env_entry_index(names, 10);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = env_entry_index(names, 20);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = env_entry_index(names, 30);
  if ((_sv0t3 != 2)) {
    return 3;
  } else {
  }
  int _sv0t4 = env_entry_index(names, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 4;
  } else {
  }
  sv0_vec_push(names, 10);
  int _sv0t6 = env_entry_index(names, 10);
  if ((_sv0t6 != 3)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_fn_table(void) {
  int _sv0t0 = sv0_vec_new();
  int fnames = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int fpcs = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int frets = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int foffs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int fptf = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int params1 = _sv0t5;
  int _sv0t6 = TY_STRING();
  sv0_vec_push(params1, _sv0t6);
  int _sv0t7 = TY_UNIT();
  int _sv0t8 = fn_table_add(fnames, fpcs, frets, foffs, fptf, 100, _sv0t7, params1, 1);
  int x1 = _sv0t8;
  if ((x1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = fn_table_lookup(fnames, 100);
  if ((_sv0t9 != 0)) {
    return 2;
  } else {
  }
  int _sv0t10 = fn_table_ret_type(frets, 0);
  int _sv0t11 = TY_UNIT();
  if ((_sv0t10 != _sv0t11)) {
    return 3;
  } else {
  }
  int _sv0t12 = fn_table_param_count(fpcs, 0);
  if ((_sv0t12 != 1)) {
    return 4;
  } else {
  }
  int _sv0t13 = fn_table_param_type(foffs, fptf, 0, 0);
  int _sv0t14 = TY_STRING();
  if ((_sv0t13 != _sv0t14)) {
    return 5;
  } else {
  }
  int _sv0t15 = sv0_vec_new();
  int params2 = _sv0t15;
  int _sv0t16 = TY_INT();
  sv0_vec_push(params2, _sv0t16);
  int _sv0t17 = TY_INT();
  sv0_vec_push(params2, _sv0t17);
  int _sv0t18 = TY_BOOL();
  int _sv0t19 = fn_table_add(fnames, fpcs, frets, foffs, fptf, 200, _sv0t18, params2, 2);
  int x2 = _sv0t19;
  if ((x2 != 1)) {
    return 6;
  } else {
  }
  int _sv0t20 = fn_table_param_count(fpcs, 1);
  if ((_sv0t20 != 2)) {
    return 7;
  } else {
  }
  int _sv0t21 = fn_table_param_type(foffs, fptf, 1, 0);
  int _sv0t22 = TY_INT();
  if ((_sv0t21 != _sv0t22)) {
    return 8;
  } else {
  }
  int _sv0t23 = fn_table_param_type(foffs, fptf, 1, 1);
  int _sv0t24 = TY_INT();
  if ((_sv0t23 != _sv0t24)) {
    return 9;
  } else {
  }
  int _sv0t25 = fn_table_ret_type(frets, 1);
  int _sv0t26 = TY_BOOL();
  if ((_sv0t25 != _sv0t26)) {
    return 10;
  } else {
  }
  int _sv0t27 = fn_table_lookup(fnames, 999);
  int _sv0t28 = (0 - 1);
  if ((_sv0t27 != _sv0t28)) {
    return 11;
  } else {
  }
  int _sv0t29 = (0 - 1);
  int _sv0t30 = fn_table_ret_type(frets, _sv0t29);
  int _sv0t31 = TY_UNKNOWN();
  if ((_sv0t30 != _sv0t31)) {
    return 12;
  } else {
  }
  int _sv0t32 = sv0_vec_new();
  int params0 = _sv0t32;
  int _sv0t33 = TY_INT();
  int _sv0t34 = fn_table_add(fnames, fpcs, frets, foffs, fptf, 300, _sv0t33, params0, 0);
  int x3 = _sv0t34;
  int _sv0t35 = fn_table_param_count(fpcs, x3);
  if ((_sv0t35 != 0)) {
    return 13;
  } else {
  }
  int _sv0t36 = fn_table_ret_type(frets, x3);
  int _sv0t37 = TY_INT();
  if ((_sv0t36 != _sv0t37)) {
    return 14;
  } else {
  }
  return 0;
}

static int test_scan_struct_field_names(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  sv0_vec_push(tt, 89);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 9);
  int _sv0t1 = sv0_vec_new();
  int out = _sv0t1;
  int _sv0t2 = scan_struct_field_names(tt, 1, 2, out);
  int r = _sv0t2;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(out);
  if ((_sv0t3 != 2)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(out, 0);
  if ((_sv0t4 != 3)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(out, 1);
  if ((_sv0t5 != 7)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int out0 = _sv0t6;
  int _sv0t7 = scan_struct_field_names(tt, 1, 0, out0);
  int r0 = _sv0t7;
  if ((r0 != 0)) {
    return 5;
  } else {
  }
  int _sv0t8 = sv0_vec_len(out0);
  if ((_sv0t8 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_scan_enum_variant_names(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  sv0_vec_push(tt, 62);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 10);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 11);
  sv0_vec_push(tt, 9);
  int _sv0t1 = sv0_vec_new();
  int out = _sv0t1;
  int _sv0t2 = scan_enum_variant_names(tt, 1, 2, out);
  int r = _sv0t2;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(out);
  if ((_sv0t3 != 2)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(out, 0);
  if ((_sv0t4 != 3)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(out, 1);
  if ((_sv0t5 != 5)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int out0 = _sv0t6;
  int _sv0t7 = scan_enum_variant_names(tt, 1, 0, out0);
  int r0 = _sv0t7;
  if ((r0 != 0)) {
    return 5;
  } else {
  }
  int _sv0t8 = sv0_vec_len(out0);
  if ((_sv0t8 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_scan_type_tag_at(void) {
  const char* source;
  source = "i32 bool Foo Bar T Vec < i32 > ( ) & i32 i32";
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
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 16);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 22);
  sv0_vec_push(starts, 23);
  sv0_vec_push(ends, 24);
  sv0_vec_push(starts, 25);
  sv0_vec_push(ends, 28);
  sv0_vec_push(starts, 29);
  sv0_vec_push(ends, 30);
  sv0_vec_push(starts, 31);
  sv0_vec_push(ends, 32);
  sv0_vec_push(starts, 33);
  sv0_vec_push(ends, 34);
  sv0_vec_push(starts, 35);
  sv0_vec_push(ends, 36);
  sv0_vec_push(starts, 37);
  sv0_vec_push(ends, 40);
  sv0_vec_push(starts, 41);
  sv0_vec_push(ends, 44);
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 38);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 39);
  sv0_vec_push(tt, 6);
  sv0_vec_push(tt, 7);
  sv0_vec_push(tt, 27);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 5);
  int _sv0t3 = sv0_vec_new();
  int sn = _sv0t3;
  sv0_vec_push(sn, 2);
  int _sv0t4 = sv0_vec_new();
  int en = _sv0t4;
  sv0_vec_push(en, 3);
  int _sv0t5 = sv0_vec_new();
  int tp = _sv0t5;
  sv0_vec_push(tp, 4);
  sv0_vec_push(tp, 99);
  int _sv0t6 = sv0_vec_new();
  int out = _sv0t6;
  int _sv0t7 = scan_type_tag_at(tt, source, starts, ends, sn, en, tp, 2, 0, out);
  int p0 = _sv0t7;
  if ((p0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_get(out, 0);
  if ((_sv0t8 != 0)) {
    return 2;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int out1 = _sv0t9;
  int _sv0t10 = scan_type_tag_at(tt, source, starts, ends, sn, en, tp, 2, 1, out1);
  int p1 = _sv0t10;
  if ((p1 != 2)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out1, 0);
  if ((_sv0t11 != 3)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int out2 = _sv0t12;
  int _sv0t13 = scan_type_tag_at(tt, source, starts, ends, sn, en, tp, 2, 2, out2);
  int p2 = _sv0t13;
  if ((p2 != 3)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(out2, 0);
  if ((_sv0t14 != 6)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_new();
  int out3 = _sv0t15;
  int _sv0t16 = scan_type_tag_at(tt, source, starts, ends, sn, en, tp, 2, 3, out3);
  int p3 = _sv0t16;
  if ((p3 != 4)) {
    return 7;
  } else {
  }
  int _sv0t17 = sv0_vec_get(out3, 0);
  if ((_sv0t17 != 7)) {
    return 8;
  } else {
  }
  int _sv0t18 = sv0_vec_new();
  int out4 = _sv0t18;
  int _sv0t19 = scan_type_tag_at(tt, source, starts, ends, sn, en, tp, 2, 4, out4);
  int p4 = _sv0t19;
  if ((p4 != 5)) {
    return 9;
  } else {
  }
  int _sv0t20 = sv0_vec_get(out4, 0);
  if ((_sv0t20 != 16)) {
    return 10;
  } else {
  }
  int _sv0t21 = sv0_vec_new();
  int out5 = _sv0t21;
  int _sv0t22 = scan_type_tag_at(tt, source, starts, ends, sn, en, tp, 2, 5, out5);
  int p5 = _sv0t22;
  if ((p5 != 9)) {
    return 11;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out5, 0);
  if ((_sv0t23 != 15)) {
    return 12;
  } else {
  }
  int _sv0t24 = sv0_vec_new();
  int out6 = _sv0t24;
  int _sv0t25 = scan_type_tag_at(tt, source, starts, ends, sn, en, tp, 2, 9, out6);
  int p6 = _sv0t25;
  if ((p6 != 11)) {
    return 13;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out6, 0);
  if ((_sv0t26 != 2)) {
    return 14;
  } else {
  }
  int _sv0t27 = sv0_vec_new();
  int out7 = _sv0t27;
  int _sv0t28 = scan_type_tag_at(tt, source, starts, ends, sn, en, tp, 2, 11, out7);
  int p7 = _sv0t28;
  if ((p7 != 13)) {
    return 15;
  } else {
  }
  int _sv0t29 = sv0_vec_get(out7, 0);
  if ((_sv0t29 != 12)) {
    return 16;
  } else {
  }
  return 0;
}

static int test_scan_struct_field_type_tags(void) {
  const char* source;
  source = "struct Pt { x : i32 , y : bool }";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 9);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 15);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 19);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 23);
  sv0_vec_push(starts, 24);
  sv0_vec_push(ends, 25);
  sv0_vec_push(starts, 26);
  sv0_vec_push(ends, 30);
  sv0_vec_push(starts, 31);
  sv0_vec_push(ends, 32);
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  sv0_vec_push(tt, 89);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 9);
  int _sv0t3 = sv0_vec_new();
  int fnames = _sv0t3;
  int _sv0t4 = scan_struct_field_names(tt, 1, 2, fnames);
  int fr = _sv0t4;
  if ((fr != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int sn = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int en = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int tp = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ftypes = _sv0t8;
  int _sv0t9 = scan_struct_field_type_tags(tt, source, starts, ends, sn, en, tp, 0, fnames, ftypes);
  int r = _sv0t9;
  if ((r != 0)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_len(ftypes);
  if ((_sv0t10 != 2)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(ftypes, 0);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ftypes, 1);
  if ((_sv0t12 != 3)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_scan_fn_param_type_tags(void) {
  const char* source;
  source = "fn foo ( x : i32 , y : bool ) -> i32";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 16);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 20);
  sv0_vec_push(starts, 21);
  sv0_vec_push(ends, 22);
  sv0_vec_push(starts, 23);
  sv0_vec_push(ends, 27);
  sv0_vec_push(starts, 28);
  sv0_vec_push(ends, 29);
  sv0_vec_push(starts, 30);
  sv0_vec_push(ends, 32);
  sv0_vec_push(starts, 33);
  sv0_vec_push(ends, 36);
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  sv0_vec_push(tt, 65);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 6);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 7);
  sv0_vec_push(tt, 19);
  sv0_vec_push(tt, 5);
  int _sv0t3 = sv0_vec_new();
  int sn = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int en = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int tp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ptypes = _sv0t6;
  int _sv0t7 = scan_fn_param_type_tags(tt, source, starts, ends, sn, en, tp, 0, 1, 2, ptypes);
  int r = _sv0t7;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_len(ptypes);
  if ((_sv0t8 != 2)) {
    return 2;
  } else {
  }
  int _sv0t9 = sv0_vec_get(ptypes, 0);
  if ((_sv0t9 != 0)) {
    return 3;
  } else {
  }
  int _sv0t10 = sv0_vec_get(ptypes, 1);
  if ((_sv0t10 != 3)) {
    return 4;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int ptypes0 = _sv0t11;
  int _sv0t12 = scan_fn_param_type_tags(tt, source, starts, ends, sn, en, tp, 0, 1, 0, ptypes0);
  int r0 = _sv0t12;
  if ((r0 != 0)) {
    return 5;
  } else {
  }
  int _sv0t13 = sv0_vec_len(ptypes0);
  if ((_sv0t13 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_fn_table_lookup_str(void) {
  const char* source;
  source = "foo bar baz";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 7);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 11);
  int _sv0t2 = sv0_vec_new();
  int fnames = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int fpcs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int frets = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int foffs = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fptf = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int p0 = _sv0t7;
  int _sv0t8 = TY_INT();
  sv0_vec_push(p0, _sv0t8);
  int _sv0t9 = TY_BOOL();
  int _sv0t10 = fn_table_add(fnames, fpcs, frets, foffs, fptf, 0, _sv0t9, p0, 1);
  int x0 = _sv0t10;
  if ((x0 < 0)) {
    return 99;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int p1 = _sv0t11;
  int _sv0t12 = TY_STRING();
  int _sv0t13 = fn_table_add(fnames, fpcs, frets, foffs, fptf, 1, _sv0t12, p1, 0);
  int x1 = _sv0t13;
  if ((x1 < 0)) {
    return 99;
  } else {
  }
  int _sv0t14 = fn_table_lookup_str(fnames, source, starts, ends, "foo");
  int r0 = _sv0t14;
  if ((r0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t15 = fn_table_lookup_str(fnames, source, starts, ends, "bar");
  int r1 = _sv0t15;
  if ((r1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t16 = fn_table_lookup_str(fnames, source, starts, ends, "baz");
  int r2 = _sv0t16;
  int _sv0t17 = (0 - 1);
  if ((r2 != _sv0t17)) {
    return 3;
  } else {
  }
  int _sv0t18 = fn_table_ret_type(frets, r0);
  int _sv0t19 = TY_BOOL();
  if ((_sv0t18 != _sv0t19)) {
    return 4;
  } else {
  }
  int _sv0t20 = fn_table_ret_type(frets, r1);
  int _sv0t21 = TY_STRING();
  if ((_sv0t20 != _sv0t21)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_scan_fn_ret_type_tag(void) {
  const char* source;
  source = "fn foo ( x : i32 ) -> bool { }";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 16);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 21);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 26);
  sv0_vec_push(starts, 27);
  sv0_vec_push(ends, 28);
  sv0_vec_push(starts, 29);
  sv0_vec_push(ends, 30);
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  sv0_vec_push(tt, 65);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 6);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 7);
  sv0_vec_push(tt, 19);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 9);
  int _sv0t3 = sv0_vec_new();
  int sn = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int en = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int tp = _sv0t5;
  int _sv0t6 = scan_fn_ret_type_tag(tt, source, starts, ends, sn, en, tp, 0, 1, 1);
  int r = _sv0t6;
  int _sv0t7 = TY_BOOL();
  if ((r != _sv0t7)) {
    return 1;
  } else {
  }
  int _sv0t8 = scan_fn_ret_type_tag(tt, source, starts, ends, sn, en, tp, 0, 1, 0);
  int r0 = _sv0t8;
  int _sv0t9 = TY_UNIT();
  if ((r0 != _sv0t9)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_register_all_item_fns(void) {
  const char* source;
  source = "fn foo ( x : i32 ) -> bool { } fn bar ( ) -> i32 { }";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 16);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 21);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 26);
  sv0_vec_push(starts, 27);
  sv0_vec_push(ends, 28);
  sv0_vec_push(starts, 29);
  sv0_vec_push(ends, 30);
  sv0_vec_push(starts, 31);
  sv0_vec_push(ends, 33);
  sv0_vec_push(starts, 34);
  sv0_vec_push(ends, 37);
  sv0_vec_push(starts, 38);
  sv0_vec_push(ends, 39);
  sv0_vec_push(starts, 40);
  sv0_vec_push(ends, 41);
  sv0_vec_push(starts, 42);
  sv0_vec_push(ends, 44);
  sv0_vec_push(starts, 45);
  sv0_vec_push(ends, 48);
  sv0_vec_push(starts, 49);
  sv0_vec_push(ends, 50);
  sv0_vec_push(starts, 51);
  sv0_vec_push(ends, 52);
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  sv0_vec_push(tt, 65);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 6);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 7);
  sv0_vec_push(tt, 19);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 9);
  sv0_vec_push(tt, 65);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 6);
  sv0_vec_push(tt, 7);
  sv0_vec_push(tt, 19);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 9);
  int _sv0t3 = sv0_vec_new();
  int item_t = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int item_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int item_d2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int item_d3 = _sv0t6;
  sv0_vec_push(item_t, 0);
  sv0_vec_push(item_d1, 1);
  sv0_vec_push(item_d2, 1);
  sv0_vec_push(item_d3, 1);
  sv0_vec_push(item_t, 0);
  sv0_vec_push(item_d1, 12);
  sv0_vec_push(item_d2, 1);
  sv0_vec_push(item_d3, 0);
  int _sv0t7 = sv0_vec_new();
  int sn = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int en = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int tp = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int fnames = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int fpcs = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int frets = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int foffs = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int fptf = _sv0t14;
  int _sv0t15 = register_all_item_fns(tt, source, starts, ends, sn, en, tp, 0, item_t, item_d1, item_d2, item_d3, fnames, fpcs, frets, foffs, fptf);
  int count = _sv0t15;
  if ((count != 2)) {
    return 1;
  } else {
  }
  int _sv0t16 = sv0_vec_len(fnames);
  if ((_sv0t16 != 2)) {
    return 2;
  } else {
  }
  int _sv0t17 = fn_table_lookup_str(fnames, source, starts, ends, "foo");
  int idx0 = _sv0t17;
  if ((idx0 < 0)) {
    return 3;
  } else {
  }
  int _sv0t18 = fn_table_ret_type(frets, idx0);
  int _sv0t19 = TY_BOOL();
  if ((_sv0t18 != _sv0t19)) {
    return 4;
  } else {
  }
  int _sv0t20 = fn_table_param_count(fpcs, idx0);
  if ((_sv0t20 != 1)) {
    return 5;
  } else {
  }
  int _sv0t21 = fn_table_param_type(foffs, fptf, idx0, 0);
  int _sv0t22 = TY_INT();
  if ((_sv0t21 != _sv0t22)) {
    return 6;
  } else {
  }
  int _sv0t23 = fn_table_lookup_str(fnames, source, starts, ends, "bar");
  int idx1 = _sv0t23;
  if ((idx1 < 0)) {
    return 7;
  } else {
  }
  int _sv0t24 = fn_table_ret_type(frets, idx1);
  int _sv0t25 = TY_INT();
  if ((_sv0t24 != _sv0t25)) {
    return 8;
  } else {
  }
  int _sv0t26 = fn_table_param_count(fpcs, idx1);
  if ((_sv0t26 != 0)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_scan_enum_variant_shapes(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 6);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 7);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 9);
  int _sv0t1 = sv0_vec_new();
  int vnames = _sv0t1;
  sv0_vec_push(vnames, 0);
  sv0_vec_push(vnames, 2);
  sv0_vec_push(vnames, 7);
  int _sv0t2 = sv0_vec_new();
  int shapes = _sv0t2;
  int _sv0t3 = scan_enum_variant_shapes(tt, vnames, shapes);
  int r0 = _sv0t3;
  if ((r0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_len(shapes);
  if ((_sv0t4 != 3)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_get(shapes, 0);
  if ((_sv0t5 != 0)) {
    return 3;
  } else {
  }
  int _sv0t6 = sv0_vec_get(shapes, 1);
  if ((_sv0t6 != 1)) {
    return 4;
  } else {
  }
  int _sv0t7 = sv0_vec_get(shapes, 2);
  if ((_sv0t7 != 2)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_init_struct_defs(void) {
  const char* source;
  source = "struct Pt { x : i32 , y : bool }";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 9);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 15);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 19);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 23);
  sv0_vec_push(starts, 24);
  sv0_vec_push(ends, 25);
  sv0_vec_push(starts, 26);
  sv0_vec_push(ends, 30);
  sv0_vec_push(starts, 31);
  sv0_vec_push(ends, 32);
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  sv0_vec_push(tt, 89);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 9);
  int _sv0t3 = sv0_vec_new();
  int item_t = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int item_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int item_d2 = _sv0t5;
  sv0_vec_push(item_t, 1);
  sv0_vec_push(item_d1, 1);
  sv0_vec_push(item_d2, 2);
  int _sv0t6 = sv0_vec_new();
  int sn = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int en = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int tp = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int sd_names = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int sd_foffs = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int sd_fcounts = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int sd_fn_flat = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int sd_ft_flat = _sv0t13;
  int _sv0t14 = init_struct_defs(tt, source, starts, ends, sn, en, tp, 0, item_t, item_d1, item_d2, sd_names, sd_foffs, sd_fcounts, sd_fn_flat, sd_ft_flat);
  int c = _sv0t14;
  if ((c != 1)) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_vec_len(sd_names);
  if ((_sv0t15 != 1)) {
    return 2;
  } else {
  }
  int _sv0t16 = sv0_vec_get(sd_names, 0);
  if ((_sv0t16 != 1)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_get(sd_fcounts, 0);
  if ((_sv0t17 != 2)) {
    return 4;
  } else {
  }
  int _sv0t18 = sv0_vec_len(sd_fn_flat);
  if ((_sv0t18 != 2)) {
    return 5;
  } else {
  }
  int _sv0t19 = sv0_vec_len(sd_ft_flat);
  if ((_sv0t19 != 2)) {
    return 6;
  } else {
  }
  int _sv0t20 = sv0_vec_get(sd_ft_flat, 0);
  int _sv0t21 = TY_INT();
  if ((_sv0t20 != _sv0t21)) {
    return 7;
  } else {
  }
  int _sv0t22 = sv0_vec_get(sd_ft_flat, 1);
  int _sv0t23 = TY_BOOL();
  if ((_sv0t22 != _sv0t23)) {
    return 8;
  } else {
  }
  int _sv0t24 = struct_def_field_ty_str(sd_names, sd_foffs, sd_fcounts, sd_fn_flat, sd_ft_flat, source, starts, ends, 1, "x");
  int ft0 = _sv0t24;
  int _sv0t25 = TY_INT();
  if ((ft0 != _sv0t25)) {
    return 9;
  } else {
  }
  int _sv0t26 = struct_def_field_ty_str(sd_names, sd_foffs, sd_fcounts, sd_fn_flat, sd_ft_flat, source, starts, ends, 1, "y");
  int ft1 = _sv0t26;
  int _sv0t27 = TY_BOOL();
  if ((ft1 != _sv0t27)) {
    return 10;
  } else {
  }
  int _sv0t28 = struct_def_field_ty_str(sd_names, sd_foffs, sd_fcounts, sd_fn_flat, sd_ft_flat, source, starts, ends, 1, "z");
  int ft2 = _sv0t28;
  int _sv0t29 = (0 - 1);
  if ((ft2 != _sv0t29)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_init_enum_defs(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  sv0_vec_push(tt, 62);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 6);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 7);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 9);
  sv0_vec_push(tt, 9);
  const char* source;
  source = "enum Color { Red , Green ( i32 ) , Blue { x : i32 } }";
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 16);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 24);
  sv0_vec_push(starts, 25);
  sv0_vec_push(ends, 26);
  sv0_vec_push(starts, 27);
  sv0_vec_push(ends, 30);
  sv0_vec_push(starts, 31);
  sv0_vec_push(ends, 32);
  sv0_vec_push(starts, 33);
  sv0_vec_push(ends, 34);
  sv0_vec_push(starts, 35);
  sv0_vec_push(ends, 39);
  sv0_vec_push(starts, 40);
  sv0_vec_push(ends, 41);
  sv0_vec_push(starts, 42);
  sv0_vec_push(ends, 43);
  sv0_vec_push(starts, 44);
  sv0_vec_push(ends, 45);
  sv0_vec_push(starts, 46);
  sv0_vec_push(ends, 49);
  sv0_vec_push(starts, 50);
  sv0_vec_push(ends, 51);
  sv0_vec_push(starts, 52);
  sv0_vec_push(ends, 53);
  int _sv0t3 = sv0_vec_new();
  int item_t = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int item_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int item_d2 = _sv0t5;
  sv0_vec_push(item_t, 2);
  sv0_vec_push(item_d1, 1);
  sv0_vec_push(item_d2, 3);
  int _sv0t6 = sv0_vec_new();
  int ed_names = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed_voffs = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ed_vcounts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed_vn_flat = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed_vs_flat = _sv0t10;
  int _sv0t11 = init_enum_defs(tt, source, starts, ends, item_t, item_d1, item_d2, ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat);
  int c = _sv0t11;
  if ((c != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = sv0_vec_len(ed_names);
  if ((_sv0t12 != 1)) {
    return 2;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed_vcounts, 0);
  if ((_sv0t13 != 3)) {
    return 3;
  } else {
  }
  int _sv0t14 = sv0_vec_len(ed_vn_flat);
  if ((_sv0t14 != 3)) {
    return 4;
  } else {
  }
  int _sv0t15 = sv0_vec_len(ed_vs_flat);
  if ((_sv0t15 != 3)) {
    return 5;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed_vs_flat, 0);
  if ((_sv0t16 != 0)) {
    return 6;
  } else {
  }
  int _sv0t17 = sv0_vec_get(ed_vs_flat, 1);
  if ((_sv0t17 != 1)) {
    return 7;
  } else {
  }
  int _sv0t18 = sv0_vec_get(ed_vs_flat, 2);
  if ((_sv0t18 != 2)) {
    return 8;
  } else {
  }
  int _sv0t19 = enum_def_variant_shape_str(ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat, source, starts, ends, 1, "Red");
  int s0 = _sv0t19;
  if ((s0 != 0)) {
    return 9;
  } else {
  }
  int _sv0t20 = enum_def_variant_shape_str(ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat, source, starts, ends, 1, "Green");
  int s1 = _sv0t20;
  if ((s1 != 1)) {
    return 10;
  } else {
  }
  int _sv0t21 = enum_def_variant_shape_str(ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat, source, starts, ends, 1, "Blue");
  int s2 = _sv0t21;
  if ((s2 != 2)) {
    return 11;
  } else {
  }
  int _sv0t22 = enum_def_variant_shape_str(ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat, source, starts, ends, 1, "Yellow");
  int s3 = _sv0t22;
  int _sv0t23 = (0 - 1);
  if ((s3 != _sv0t23)) {
    return 12;
  } else {
  }
  return 0;
}

static int test_init_enum_defs_dup_variants(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  sv0_vec_push(tt, 62);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 9);
  const char* source;
  source = "enum E { Red , Red }";
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 12);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 14);
  sv0_vec_push(starts, 15);
  sv0_vec_push(ends, 18);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 20);
  int _sv0t3 = sv0_vec_new();
  int item_t = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int item_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int item_d2 = _sv0t5;
  sv0_vec_push(item_t, 2);
  sv0_vec_push(item_d1, 1);
  sv0_vec_push(item_d2, 2);
  int _sv0t6 = sv0_vec_new();
  int ed_names = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed_voffs = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ed_vcounts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed_vn_flat = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed_vs_flat = _sv0t10;
  int _sv0t11 = init_enum_defs(tt, source, starts, ends, item_t, item_d1, item_d2, ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat);
  int c = _sv0t11;
  int _sv0t12 = (0 - 1);
  if ((c != _sv0t12)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_builtin_fn_lookup(void) {
  int _sv0t0 = builtin_fn_lookup("println");
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = builtin_fn_lookup("string_len");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = builtin_fn_lookup("vec_new");
  if ((_sv0t2 != 6)) {
    return 3;
  } else {
  }
  int _sv0t3 = builtin_fn_lookup("vec_get");
  if ((_sv0t3 != 9)) {
    return 4;
  } else {
  }
  int _sv0t4 = builtin_fn_lookup("box_deref");
  if ((_sv0t4 != 12)) {
    return 5;
  } else {
  }
  int _sv0t5 = builtin_fn_lookup("unknown");
  int _sv0t6 = (0 - 1);
  if ((_sv0t5 != _sv0t6)) {
    return 6;
  } else {
  }
  int _sv0t7 = builtin_fn_ret_type(0);
  int _sv0t8 = TY_UNIT();
  if ((_sv0t7 != _sv0t8)) {
    return 7;
  } else {
  }
  int _sv0t9 = builtin_fn_ret_type(1);
  int _sv0t10 = TY_INT();
  if ((_sv0t9 != _sv0t10)) {
    return 8;
  } else {
  }
  int _sv0t11 = builtin_fn_ret_type(2);
  int _sv0t12 = TY_BOOL();
  if ((_sv0t11 != _sv0t12)) {
    return 9;
  } else {
  }
  int _sv0t13 = builtin_fn_ret_type(3);
  int _sv0t14 = TY_STRING();
  if ((_sv0t13 != _sv0t14)) {
    return 10;
  } else {
  }
  int _sv0t15 = builtin_fn_ret_type(9);
  int _sv0t16 = TY_VAR();
  if ((_sv0t15 != _sv0t16)) {
    return 11;
  } else {
  }
  int _sv0t17 = builtin_fn_param_count(0);
  if ((_sv0t17 != 1)) {
    return 12;
  } else {
  }
  int _sv0t18 = builtin_fn_param_count(5);
  if ((_sv0t18 != 3)) {
    return 13;
  } else {
  }
  int _sv0t19 = builtin_fn_param_count(6);
  if ((_sv0t19 != 0)) {
    return 14;
  } else {
  }
  int _sv0t20 = builtin_fn_param_type(0, 0);
  int _sv0t21 = TY_STRING();
  if ((_sv0t20 != _sv0t21)) {
    return 15;
  } else {
  }
  int _sv0t22 = builtin_fn_param_type(4, 0);
  int _sv0t23 = TY_STRING();
  if ((_sv0t22 != _sv0t23)) {
    return 16;
  } else {
  }
  int _sv0t24 = builtin_fn_param_type(4, 1);
  int _sv0t25 = TY_INT();
  if ((_sv0t24 != _sv0t25)) {
    return 17;
  } else {
  }
  int _sv0t26 = builtin_fn_param_type(9, 0);
  int _sv0t27 = TY_NAMED();
  if ((_sv0t26 != _sv0t27)) {
    return 18;
  } else {
  }
  int _sv0t28 = builtin_fn_param_type(9, 1);
  int _sv0t29 = TY_INT();
  if ((_sv0t28 != _sv0t29)) {
    return 19;
  } else {
  }
  return 0;
}

static int test_resolve_fn_call(void) {
  const char* source;
  source = "fn foo ( ) -> i32 { }";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 13);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 17);
  sv0_vec_push(starts, 18);
  sv0_vec_push(ends, 19);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  int _sv0t2 = sv0_vec_new();
  int fnames = _sv0t2;
  sv0_vec_push(fnames, 1);
  int _sv0t3 = sv0_vec_new();
  int ib0 = _sv0t3;
  int _sv0t4 = resolve_fn_call(fnames, source, starts, ends, "foo", ib0);
  int r0 = _sv0t4;
  if ((r0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_get(ib0, 0);
  if ((_sv0t5 != 0)) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int ib1 = _sv0t6;
  int _sv0t7 = resolve_fn_call(fnames, source, starts, ends, "println", ib1);
  int r1 = _sv0t7;
  if ((r1 != 0)) {
    return 3;
  } else {
  }
  int _sv0t8 = sv0_vec_get(ib1, 0);
  if ((_sv0t8 != 1)) {
    return 4;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int ib2 = _sv0t9;
  int _sv0t10 = resolve_fn_call(fnames, source, starts, ends, "nonexistent", ib2);
  int r2 = _sv0t10;
  int _sv0t11 = (0 - 1);
  if ((r2 != _sv0t11)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_enum_ctor_resolution(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  sv0_vec_push(tt, 62);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 6);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 7);
  sv0_vec_push(tt, 12);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 8);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 14);
  sv0_vec_push(tt, 5);
  sv0_vec_push(tt, 9);
  sv0_vec_push(tt, 9);
  const char* source;
  source = "enum Color { Red , Green ( i32 ) , Blue { x : i32 } }";
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 16);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 24);
  sv0_vec_push(starts, 25);
  sv0_vec_push(ends, 26);
  sv0_vec_push(starts, 27);
  sv0_vec_push(ends, 30);
  sv0_vec_push(starts, 31);
  sv0_vec_push(ends, 32);
  sv0_vec_push(starts, 33);
  sv0_vec_push(ends, 34);
  sv0_vec_push(starts, 35);
  sv0_vec_push(ends, 39);
  sv0_vec_push(starts, 40);
  sv0_vec_push(ends, 41);
  sv0_vec_push(starts, 42);
  sv0_vec_push(ends, 43);
  sv0_vec_push(starts, 44);
  sv0_vec_push(ends, 45);
  sv0_vec_push(starts, 46);
  sv0_vec_push(ends, 49);
  sv0_vec_push(starts, 50);
  sv0_vec_push(ends, 51);
  sv0_vec_push(starts, 52);
  sv0_vec_push(ends, 53);
  int _sv0t3 = sv0_vec_new();
  int item_t = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int item_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int item_d2 = _sv0t5;
  sv0_vec_push(item_t, 2);
  sv0_vec_push(item_d1, 1);
  sv0_vec_push(item_d2, 3);
  int _sv0t6 = sv0_vec_new();
  int ed_names = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed_voffs = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ed_vcounts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed_vn_flat = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed_vs_flat = _sv0t10;
  int _sv0t11 = init_enum_defs(tt, source, starts, ends, item_t, item_d1, item_d2, ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat);
  int c = _sv0t11;
  if ((c != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = edef_name_lookup_str(ed_names, source, starts, ends, "Color");
  int ei = _sv0t12;
  if ((ei != 0)) {
    return 2;
  } else {
  }
  int _sv0t13 = edef_name_lookup_str(ed_names, source, starts, ends, "Nope");
  int ei2 = _sv0t13;
  int _sv0t14 = (0 - 1);
  if ((ei2 != _sv0t14)) {
    return 3;
  } else {
  }
  int _sv0t15 = ctor_type_from_edef(ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat, source, starts, ends, 1, "Red");
  int ct0 = _sv0t15;
  int _sv0t16 = TY_ENUM();
  if ((ct0 != _sv0t16)) {
    return 4;
  } else {
  }
  int _sv0t17 = ctor_type_from_edef(ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat, source, starts, ends, 1, "Green");
  int ct1 = _sv0t17;
  int _sv0t18 = TY_FN();
  if ((ct1 != _sv0t18)) {
    return 5;
  } else {
  }
  int _sv0t19 = ctor_type_from_edef(ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat, source, starts, ends, 1, "Blue");
  int ct2 = _sv0t19;
  int _sv0t20 = TY_FN();
  if ((ct2 != _sv0t20)) {
    return 6;
  } else {
  }
  int _sv0t21 = resolve_ctor_path_ty(ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat, source, starts, ends, 1, 3);
  int rp = _sv0t21;
  int _sv0t22 = TY_ENUM();
  if ((rp != _sv0t22)) {
    return 7;
  } else {
  }
  int _sv0t23 = resolve_ctor_path_ty(ed_names, ed_voffs, ed_vcounts, ed_vn_flat, ed_vs_flat, source, starts, ends, 1, 5);
  int rp2 = _sv0t23;
  int _sv0t24 = TY_FN();
  if ((rp2 != _sv0t24)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_synth_expr(void) {
  const char* source;
  source = "x 42 true";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 4);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 9);
  int _sv0t2 = sv0_vec_new();
  int en = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ety = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int em = _sv0t4;
  int _sv0t5 = TY_INT();
  int _sv0t6 = env_extend(en, ety, em, 0, _sv0t5, 0);
  int _sv0t7 = sv0_vec_new();
  int et = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ed1 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ed2 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ed3 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int ed4v = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int ppool = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int edn = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int edvo = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int edvc = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int edvn = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int edvs = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int tfn = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int tfrt = _sv0t19;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t20 = TY_INT();
  int _sv0t21 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t20, 0, 0);
  int r0 = _sv0t21;
  int _sv0t22 = TY_INT();
  if ((r0 != _sv0t22)) {
    return 1;
  } else {
  }
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t23 = TY_INT();
  int _sv0t24 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t23, 0, 1);
  int r1 = _sv0t24;
  int _sv0t25 = TY_BOOL();
  if ((r1 != _sv0t25)) {
    return 2;
  } else {
  }
  sv0_vec_push(ppool, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t26 = TY_INT();
  int _sv0t27 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t26, 0, 2);
  int r2 = _sv0t27;
  int _sv0t28 = TY_INT();
  if ((r2 != _sv0t28)) {
    return 3;
  } else {
  }
  sv0_vec_push(et, 2);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t29 = TY_INT();
  int _sv0t30 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t29, 0, 3);
  int r3 = _sv0t30;
  int _sv0t31 = TY_INT();
  if ((r3 != _sv0t31)) {
    return 4;
  } else {
  }
  sv0_vec_push(et, 3);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t32 = TY_INT();
  int _sv0t33 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t32, 0, 4);
  int r4 = _sv0t33;
  int _sv0t34 = TY_INT();
  if ((r4 != _sv0t34)) {
    return 5;
  } else {
  }
  sv0_vec_push(et, 15);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t35 = TY_INT();
  int _sv0t36 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t35, 0, 5);
  int r5 = _sv0t36;
  int _sv0t37 = TY_INT();
  if ((r5 != _sv0t37)) {
    return 6;
  } else {
  }
  sv0_vec_push(et, 15);
  int _sv0t38 = (0 - 1);
  sv0_vec_push(ed1, _sv0t38);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t39 = TY_BOOL();
  int _sv0t40 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t39, 0, 6);
  int r6 = _sv0t40;
  int _sv0t41 = TY_BOOL();
  if ((r6 != _sv0t41)) {
    return 7;
  } else {
  }
  sv0_vec_push(et, 17);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t42 = TY_INT();
  int _sv0t43 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t42, 1, 7);
  int r7 = _sv0t43;
  int _sv0t44 = TY_UNIT();
  if ((r7 != _sv0t44)) {
    return 8;
  } else {
  }
  sv0_vec_push(et, 17);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t45 = TY_INT();
  int _sv0t46 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t45, 0, 8);
  int r8 = _sv0t46;
  int _sv0t47 = (0 - 1);
  if ((r8 != _sv0t47)) {
    return 9;
  } else {
  }
  sv0_vec_push(et, 10);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t48 = TY_INT();
  int _sv0t49 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t48, 0, 9);
  int r9 = _sv0t49;
  int _sv0t50 = TY_INT();
  if ((r9 != _sv0t50)) {
    return 10;
  } else {
  }
  sv0_vec_push(et, 10);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 0);
  int _sv0t51 = (0 - 1);
  sv0_vec_push(ed3, _sv0t51);
  sv0_vec_push(ed4v, 0);
  int _sv0t52 = TY_INT();
  int _sv0t53 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t52, 0, 10);
  int r10 = _sv0t53;
  int _sv0t54 = TY_UNIT();
  if ((r10 != _sv0t54)) {
    return 11;
  } else {
  }
  sv0_vec_push(et, 12);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t55 = TY_INT();
  int _sv0t56 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t55, 0, 11);
  int r11 = _sv0t56;
  int _sv0t57 = TY_UNIT();
  if ((r11 != _sv0t57)) {
    return 12;
  } else {
  }
  sv0_vec_push(et, 14);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t58 = TY_INT();
  int _sv0t59 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t58, 0, 12);
  int r12 = _sv0t59;
  int _sv0t60 = TY_UNIT();
  if ((r12 != _sv0t60)) {
    return 13;
  } else {
  }
  sv0_vec_push(et, 16);
  int _sv0t61 = (0 - 1);
  sv0_vec_push(ed1, _sv0t61);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t62 = TY_INT();
  int _sv0t63 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t62, 1, 13);
  int r13 = _sv0t63;
  int _sv0t64 = TY_UNIT();
  if ((r13 != _sv0t64)) {
    return 14;
  } else {
  }
  sv0_vec_push(et, 18);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t65 = TY_INT();
  int _sv0t66 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t65, 0, 14);
  int r14 = _sv0t66;
  int _sv0t67 = TY_UNIT();
  if ((r14 != _sv0t67)) {
    return 15;
  } else {
  }
  sv0_vec_push(et, 25);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t68 = TY_INT();
  int _sv0t69 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t68, 0, 15);
  int r15 = _sv0t69;
  int _sv0t70 = TY_INT();
  if ((r15 != _sv0t70)) {
    return 16;
  } else {
  }
  sv0_vec_push(et, 23);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t71 = TY_INT();
  int _sv0t72 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t71, 0, 16);
  int r16 = _sv0t72;
  int _sv0t73 = TY_UNIT();
  if ((r16 != _sv0t73)) {
    return 17;
  } else {
  }
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  const char* src2;
  src2 = "x 42 true y";
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, 3);
  int _sv0t74 = (0 - 1);
  sv0_vec_push(ed2, _sv0t74);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  sv0_vec_push(ppool, 3);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  sv0_vec_push(et, 9);
  sv0_vec_push(ed1, 17);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 18);
  sv0_vec_push(ed4v, 0);
  int _sv0t75 = TY_INT();
  int _sv0t76 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t75, 0, 19);
  int r17 = _sv0t76;
  int _sv0t77 = TY_INT();
  if ((r17 != _sv0t77)) {
    return 19;
  } else {
  }
  sv0_vec_push(et, 9);
  sv0_vec_push(ed1, 17);
  sv0_vec_push(ed2, 1);
  int _sv0t78 = (0 - 1);
  sv0_vec_push(ed3, _sv0t78);
  sv0_vec_push(ed4v, 0);
  int _sv0t79 = TY_INT();
  int _sv0t80 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t79, 0, 20);
  int r18 = _sv0t80;
  int _sv0t81 = TY_UNIT();
  if ((r18 != _sv0t81)) {
    return 20;
  } else {
  }
  sv0_vec_push(et, 21);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  sv0_vec_push(et, 13);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 21);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t82 = TY_INT();
  int _sv0t83 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t82, 0, 22);
  int r19 = _sv0t83;
  int _sv0t84 = TY_UNIT();
  if ((r19 != _sv0t84)) {
    return 21;
  } else {
  }
  sv0_vec_push(et, 20);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t85 = TY_INT();
  int _sv0t86 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t85, 0, 23);
  int r20 = _sv0t86;
  int _sv0t87 = TY_INT();
  if ((r20 != _sv0t87)) {
    return 22;
  } else {
  }
  sv0_vec_push(et, 22);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t88 = TY_INT();
  int _sv0t89 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t88, 0, 24);
  int r21 = _sv0t89;
  int _sv0t90 = TY_INT();
  if ((r21 != _sv0t90)) {
    return 23;
  } else {
  }
  sv0_vec_push(et, 8);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t91 = TY_INT();
  int _sv0t92 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t91, 0, 25);
  int r22 = _sv0t92;
  int _sv0t93 = TY_INT();
  if ((r22 != _sv0t93)) {
    return 24;
  } else {
  }
  sv0_vec_push(et, 21);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t94 = TY_INT();
  int _sv0t95 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, tfn, tfrt, _sv0t94, 0, 26);
  int r23 = _sv0t95;
  int _sv0t96 = TY_INT();
  if ((r23 != _sv0t96)) {
    return 25;
  } else {
  }
  int _sv0t97 = sv0_vec_new();
  int fnames = _sv0t97;
  int _sv0t98 = sv0_vec_new();
  int frtypes = _sv0t98;
  sv0_vec_push(fnames, 0);
  int _sv0t99 = TY_BOOL();
  sv0_vec_push(frtypes, _sv0t99);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  sv0_vec_push(et, 4);
  sv0_vec_push(ed1, 27);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 1);
  sv0_vec_push(ed4v, 0);
  int _sv0t100 = TY_INT();
  int _sv0t101 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, fnames, frtypes, _sv0t100, 0, 28);
  int r24 = _sv0t101;
  int _sv0t102 = TY_BOOL();
  if ((r24 != _sv0t102)) {
    return 26;
  } else {
  }
  sv0_vec_push(et, 6);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t103 = TY_INT();
  int _sv0t104 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, fnames, frtypes, _sv0t103, 0, 29);
  int r25 = _sv0t104;
  int _sv0t105 = TY_INT();
  if ((r25 != _sv0t105)) {
    return 27;
  } else {
  }
  sv0_vec_push(et, 7);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t106 = TY_INT();
  int _sv0t107 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, fnames, frtypes, _sv0t106, 0, 30);
  int r26 = _sv0t107;
  int _sv0t108 = TY_INT();
  if ((r26 != _sv0t108)) {
    return 28;
  } else {
  }
  sv0_vec_push(et, 5);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 1);
  int _sv0t109 = TY_INT();
  int _sv0t110 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, fnames, frtypes, _sv0t109, 0, 31);
  int r27 = _sv0t110;
  int _sv0t111 = TY_INT();
  if ((r27 != _sv0t111)) {
    return 29;
  } else {
  }
  sv0_vec_push(et, 29);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 0);
  int _sv0t112 = (0 - 1);
  sv0_vec_push(ed3, _sv0t112);
  sv0_vec_push(ed4v, 0);
  sv0_vec_push(et, 11);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 32);
  sv0_vec_push(ed3, 1);
  sv0_vec_push(ed4v, 0);
  int _sv0t113 = TY_INT();
  int _sv0t114 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, fnames, frtypes, _sv0t113, 0, 33);
  int r28 = _sv0t114;
  int _sv0t115 = TY_INT();
  if ((r28 != _sv0t115)) {
    return 30;
  } else {
  }
  sv0_vec_push(et, 24);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 1);
  sv0_vec_push(ed4v, 0);
  int _sv0t116 = TY_INT();
  int _sv0t117 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, fnames, frtypes, _sv0t116, 0, 34);
  int r29 = _sv0t117;
  int _sv0t118 = TY_INT();
  if ((r29 != _sv0t118)) {
    return 31;
  } else {
  }
  sv0_vec_push(et, 26);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 2);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4v, 0);
  int _sv0t119 = TY_INT();
  int _sv0t120 = synth_expr(et, ed1, ed2, ed3, ed4v, ppool, src2, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, fnames, frtypes, _sv0t119, 0, 35);
  int r30 = _sv0t120;
  int _sv0t121 = TY_INT();
  if ((r30 != _sv0t121)) {
    return 32;
  } else {
  }
  int _sv0t122 = TY_INT();
  int _sv0t123 = synth_expr_call_simple(et, ed1, ed2, ed3, ed4v, ppool, source, starts, ends, en, ety, em, edn, edvo, edvc, edvn, edvs, fnames, frtypes, _sv0t122, 0, 2, 0, 0);
  int rc = _sv0t123;
  int _sv0t124 = TY_BOOL();
  if ((rc != _sv0t124)) {
    return 18;
  } else {
  }
  return 0;
}

static int test_scan_fn_param_names(void) {
  int _sv0t0 = sv0_vec_new();
  int tok_tags = _sv0t0;
  sv0_vec_push(tok_tags, 65);
  sv0_vec_push(tok_tags, 5);
  sv0_vec_push(tok_tags, 6);
  sv0_vec_push(tok_tags, 77);
  sv0_vec_push(tok_tags, 5);
  sv0_vec_push(tok_tags, 14);
  sv0_vec_push(tok_tags, 5);
  sv0_vec_push(tok_tags, 12);
  sv0_vec_push(tok_tags, 5);
  sv0_vec_push(tok_tags, 14);
  sv0_vec_push(tok_tags, 5);
  sv0_vec_push(tok_tags, 7);
  sv0_vec_push(tok_tags, 19);
  sv0_vec_push(tok_tags, 5);
  sv0_vec_push(tok_tags, 8);
  sv0_vec_push(tok_tags, 1);
  sv0_vec_push(tok_tags, 9);
  int _sv0t1 = sv0_vec_new();
  int pn = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int pm = _sv0t2;
  int _sv0t3 = scan_fn_param_names(tok_tags, 1, 2, pn, pm);
  int r = _sv0t3;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_len(pn);
  if ((_sv0t4 != 2)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_get(pn, 0);
  if ((_sv0t5 != 4)) {
    return 3;
  } else {
  }
  int _sv0t6 = sv0_vec_get(pn, 1);
  if ((_sv0t6 != 8)) {
    return 4;
  } else {
  }
  int _sv0t7 = sv0_vec_get(pm, 0);
  if ((_sv0t7 != 1)) {
    return 5;
  } else {
  }
  int _sv0t8 = sv0_vec_get(pm, 1);
  if ((_sv0t8 != 0)) {
    return 6;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int pn0 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int pm0 = _sv0t10;
  int _sv0t11 = scan_fn_param_names(tok_tags, 1, 0, pn0, pm0);
  int r0 = _sv0t11;
  if ((r0 != 0)) {
    return 7;
  } else {
  }
  int _sv0t12 = sv0_vec_len(pn0);
  if ((_sv0t12 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_check_fn_body(void) {
  const char* source;
  source = "42";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  int _sv0t2 = sv0_vec_new();
  int bet = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int bed1 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int bed2 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int bed3 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int bed4 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int bpp = _sv0t7;
  sv0_vec_push(bet, 0);
  sv0_vec_push(bed1, 0);
  sv0_vec_push(bed2, 0);
  sv0_vec_push(bed3, 0);
  sv0_vec_push(bed4, 0);
  int _sv0t8 = sv0_vec_new();
  int men = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int met = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int mem = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int ppn = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int ppt = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int ppm = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int edn = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int edvo = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int edvc = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int edvn = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int edvs = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int fnn = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int fnrt = _sv0t20;
  int _sv0t21 = TY_INT();
  int _sv0t22 = check_fn_body(bet, bed1, bed2, bed3, bed4, bpp, source, starts, ends, men, met, mem, ppn, ppt, ppm, 0, _sv0t21, 0, edn, edvo, edvc, edvn, edvs, fnn, fnrt);
  int r1 = _sv0t22;
  if ((r1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t23 = TY_BOOL();
  int _sv0t24 = check_fn_body(bet, bed1, bed2, bed3, bed4, bpp, source, starts, ends, men, met, mem, ppn, ppt, ppm, 0, _sv0t23, 0, edn, edvo, edvc, edvn, edvs, fnn, fnrt);
  int r2 = _sv0t24;
  int _sv0t25 = (0 - 1);
  if ((r2 != _sv0t25)) {
    return 2;
  } else {
  }
  int _sv0t26 = TY_UNIT();
  int _sv0t27 = check_fn_body(bet, bed1, bed2, bed3, bed4, bpp, source, starts, ends, men, met, mem, ppn, ppt, ppm, 0, _sv0t26, 0, edn, edvo, edvc, edvn, edvs, fnn, fnrt);
  int r3 = _sv0t27;
  if ((r3 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_check_program(void) {
  const char* source_a;
  source_a = "fn foo ( ) -> i32 { }";
  int _sv0t0 = sv0_vec_new();
  int starts_a = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends_a = _sv0t1;
  sv0_vec_push(starts_a, 0);
  sv0_vec_push(ends_a, 2);
  sv0_vec_push(starts_a, 3);
  sv0_vec_push(ends_a, 6);
  sv0_vec_push(starts_a, 7);
  sv0_vec_push(ends_a, 8);
  sv0_vec_push(starts_a, 9);
  sv0_vec_push(ends_a, 10);
  sv0_vec_push(starts_a, 11);
  sv0_vec_push(ends_a, 13);
  sv0_vec_push(starts_a, 14);
  sv0_vec_push(ends_a, 17);
  sv0_vec_push(starts_a, 18);
  sv0_vec_push(ends_a, 19);
  sv0_vec_push(starts_a, 20);
  sv0_vec_push(ends_a, 21);
  int _sv0t2 = sv0_vec_new();
  int tt_a = _sv0t2;
  sv0_vec_push(tt_a, 65);
  sv0_vec_push(tt_a, 5);
  sv0_vec_push(tt_a, 6);
  sv0_vec_push(tt_a, 7);
  sv0_vec_push(tt_a, 19);
  sv0_vec_push(tt_a, 5);
  sv0_vec_push(tt_a, 8);
  sv0_vec_push(tt_a, 9);
  int _sv0t3 = sv0_vec_new();
  int it_a = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id1_a = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int id2_a = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id3_a = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id4_a = _sv0t7;
  sv0_vec_push(it_a, 0);
  sv0_vec_push(id1_a, 1);
  sv0_vec_push(id2_a, 1);
  sv0_vec_push(id3_a, 0);
  sv0_vec_push(id4_a, 0);
  int _sv0t8 = sv0_vec_new();
  int body_et_a = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int body_ed1_a = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int body_ed2_a = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int body_ed3_a = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int body_ed4_a = _sv0t12;
  sv0_vec_push(body_et_a, 0);
  sv0_vec_push(body_ed1_a, 0);
  sv0_vec_push(body_ed2_a, 0);
  sv0_vec_push(body_ed3_a, 0);
  sv0_vec_push(body_ed4_a, 0);
  int _sv0t13 = sv0_vec_new();
  int pp_a = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int tp_a = _sv0t14;
  int _sv0t15 = check_program(tt_a, source_a, starts_a, ends_a, pp_a, it_a, id1_a, id2_a, id3_a, id4_a, body_et_a, body_ed1_a, body_ed2_a, body_ed3_a, body_ed4_a, tp_a, 0);
  int ra = _sv0t15;
  if ((ra != 0)) {
    return 1;
  } else {
  }
  const char* source_b;
  source_b = "fn foo ( ) -> i32 { } fn bar ( ) -> i32 { }";
  int _sv0t16 = sv0_vec_new();
  int starts_b = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends_b = _sv0t17;
  sv0_vec_push(starts_b, 0);
  sv0_vec_push(ends_b, 2);
  sv0_vec_push(starts_b, 3);
  sv0_vec_push(ends_b, 6);
  sv0_vec_push(starts_b, 7);
  sv0_vec_push(ends_b, 8);
  sv0_vec_push(starts_b, 9);
  sv0_vec_push(ends_b, 10);
  sv0_vec_push(starts_b, 11);
  sv0_vec_push(ends_b, 13);
  sv0_vec_push(starts_b, 14);
  sv0_vec_push(ends_b, 17);
  sv0_vec_push(starts_b, 18);
  sv0_vec_push(ends_b, 19);
  sv0_vec_push(starts_b, 20);
  sv0_vec_push(ends_b, 21);
  sv0_vec_push(starts_b, 22);
  sv0_vec_push(ends_b, 24);
  sv0_vec_push(starts_b, 25);
  sv0_vec_push(ends_b, 28);
  sv0_vec_push(starts_b, 29);
  sv0_vec_push(ends_b, 30);
  sv0_vec_push(starts_b, 31);
  sv0_vec_push(ends_b, 32);
  sv0_vec_push(starts_b, 33);
  sv0_vec_push(ends_b, 35);
  sv0_vec_push(starts_b, 36);
  sv0_vec_push(ends_b, 39);
  sv0_vec_push(starts_b, 40);
  sv0_vec_push(ends_b, 41);
  sv0_vec_push(starts_b, 42);
  sv0_vec_push(ends_b, 43);
  int _sv0t18 = sv0_vec_new();
  int tt_b = _sv0t18;
  sv0_vec_push(tt_b, 65);
  sv0_vec_push(tt_b, 5);
  sv0_vec_push(tt_b, 6);
  sv0_vec_push(tt_b, 7);
  sv0_vec_push(tt_b, 19);
  sv0_vec_push(tt_b, 5);
  sv0_vec_push(tt_b, 8);
  sv0_vec_push(tt_b, 9);
  sv0_vec_push(tt_b, 65);
  sv0_vec_push(tt_b, 5);
  sv0_vec_push(tt_b, 6);
  sv0_vec_push(tt_b, 7);
  sv0_vec_push(tt_b, 19);
  sv0_vec_push(tt_b, 5);
  sv0_vec_push(tt_b, 8);
  sv0_vec_push(tt_b, 9);
  int _sv0t19 = sv0_vec_new();
  int it_b = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int id1_b = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int id2_b = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int id3_b = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int id4_b = _sv0t23;
  sv0_vec_push(it_b, 0);
  sv0_vec_push(id1_b, 1);
  sv0_vec_push(id2_b, 1);
  sv0_vec_push(id3_b, 0);
  sv0_vec_push(id4_b, 0);
  sv0_vec_push(it_b, 0);
  sv0_vec_push(id1_b, 9);
  sv0_vec_push(id2_b, 1);
  sv0_vec_push(id3_b, 0);
  sv0_vec_push(id4_b, 1);
  int _sv0t24 = sv0_vec_new();
  int body_et_b = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int body_ed1_b = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int body_ed2_b = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int body_ed3_b = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int body_ed4_b = _sv0t28;
  sv0_vec_push(body_et_b, 0);
  sv0_vec_push(body_ed1_b, 0);
  sv0_vec_push(body_ed2_b, 0);
  sv0_vec_push(body_ed3_b, 0);
  sv0_vec_push(body_ed4_b, 0);
  sv0_vec_push(body_et_b, 0);
  sv0_vec_push(body_ed1_b, 0);
  sv0_vec_push(body_ed2_b, 0);
  sv0_vec_push(body_ed3_b, 0);
  sv0_vec_push(body_ed4_b, 0);
  int _sv0t29 = sv0_vec_new();
  int pp_b = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int tp_b = _sv0t30;
  int _sv0t31 = check_program(tt_b, source_b, starts_b, ends_b, pp_b, it_b, id1_b, id2_b, id3_b, id4_b, body_et_b, body_ed1_b, body_ed2_b, body_ed3_b, body_ed4_b, tp_b, 0);
  int rb = _sv0t31;
  if ((rb != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_process_item_use(void) {
  const char* source;
  source = "use Mod :: bar fn Mod__bar ( ) -> i32 { 0 }";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 7);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 14);
  sv0_vec_push(starts, 15);
  sv0_vec_push(ends, 17);
  sv0_vec_push(starts, 18);
  sv0_vec_push(ends, 26);
  sv0_vec_push(starts, 27);
  sv0_vec_push(ends, 28);
  sv0_vec_push(starts, 29);
  sv0_vec_push(ends, 30);
  sv0_vec_push(starts, 31);
  sv0_vec_push(ends, 33);
  sv0_vec_push(starts, 34);
  sv0_vec_push(ends, 37);
  sv0_vec_push(starts, 38);
  sv0_vec_push(ends, 39);
  sv0_vec_push(starts, 40);
  sv0_vec_push(ends, 41);
  sv0_vec_push(starts, 42);
  sv0_vec_push(ends, 43);
  int _sv0t2 = sv0_vec_new();
  int pp = _sv0t2;
  sv0_vec_push(pp, 1);
  sv0_vec_push(pp, 3);
  int _sv0t3 = sv0_vec_new();
  int id1 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id2 = _sv0t4;
  sv0_vec_push(id1, 0);
  sv0_vec_push(id2, 2);
  int _sv0t5 = sv0_vec_new();
  int fnn = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fnrt = _sv0t6;
  sv0_vec_push(fnn, 5);
  int _sv0t7 = TY_INT();
  sv0_vec_push(fnrt, _sv0t7);
  int _sv0t8 = sv0_vec_new();
  int sdn = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int edn = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int edvo = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int edvc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int edvn = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int edvs = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int en = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int et = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int em = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int al = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int at = _sv0t18;
  int _sv0t19 = process_item_use(source, starts, ends, pp, id1, id2, 0, fnn, fnrt, sdn, edn, edvo, edvc, edvn, edvs, en, et, em, al, at);
  int r = _sv0t19;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t20 = sv0_vec_len(en);
  if ((_sv0t20 != 1)) {
    return 2;
  } else {
  }
  int _sv0t21 = sv0_vec_get(en, 0);
  if ((_sv0t21 != 3)) {
    return 3;
  } else {
  }
  int _sv0t22 = sv0_vec_get(et, 0);
  int _sv0t23 = TY_FN();
  if ((_sv0t22 != _sv0t23)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_build_mod_env(void) {
  const char* source;
  source = "fn foo ( ) { }";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 14);
  int _sv0t2 = sv0_vec_new();
  int pp = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int it = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int id2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id4 = _sv0t7;
  sv0_vec_push(it, 0);
  sv0_vec_push(id1, 1);
  sv0_vec_push(id2, 0);
  sv0_vec_push(id3, 0);
  sv0_vec_push(id4, 0);
  sv0_vec_push(it, 1);
  sv0_vec_push(id1, 1);
  sv0_vec_push(id2, 0);
  sv0_vec_push(id3, 0);
  sv0_vec_push(id4, 0);
  int _sv0t8 = sv0_vec_new();
  int fnn = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int fnrt = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int sdn = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int edn = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int edvo = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int edvc = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int edvn = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int edvs = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int en = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int et = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int em = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int al = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int at = _sv0t20;
  int _sv0t21 = build_mod_env(source, starts, ends, pp, it, id1, id2, id3, id4, fnn, fnrt, sdn, edn, edvo, edvc, edvn, edvs, en, et, em, al, at);
  int r = _sv0t21;
  if ((r != 0)) {
    return 1;
  } else {
  }
  int _sv0t22 = sv0_vec_len(en);
  if ((_sv0t22 != 1)) {
    return 2;
  } else {
  }
  int _sv0t23 = sv0_vec_get(en, 0);
  if ((_sv0t23 != 1)) {
    return 3;
  } else {
  }
  int _sv0t24 = sv0_vec_get(et, 0);
  int _sv0t25 = TY_FN();
  if ((_sv0t24 != _sv0t25)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_synth_building_blocks(void) {
  const char* source;
  source = "fn foo ( x : i32 ) -> bool { }";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 6);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 16);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 21);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 26);
  sv0_vec_push(starts, 27);
  sv0_vec_push(ends, 28);
  sv0_vec_push(starts, 29);
  sv0_vec_push(ends, 30);
  int _sv0t2 = sv0_vec_new();
  int en = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int em = _sv0t4;
  int _sv0t5 = TY_INT();
  int _sv0t6 = env_extend(en, et, em, 3, _sv0t5, 0);
  int _sv0t7 = env_lookup_str(en, et, source, starts, ends, "x");
  int r1 = _sv0t7;
  int _sv0t8 = TY_INT();
  if ((r1 != _sv0t8)) {
    return 1;
  } else {
  }
  int _sv0t9 = env_lookup_str(en, et, source, starts, ends, "y");
  int r2 = _sv0t9;
  int _sv0t10 = (0 - 1);
  if ((r2 != _sv0t10)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int sd_names = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int sd_foffs = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int sd_fcounts = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int sd_fn_flat = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int sd_ft_flat = _sv0t15;
  sv0_vec_push(sd_names, 1);
  sv0_vec_push(sd_foffs, 0);
  sv0_vec_push(sd_fcounts, 0);
  int _sv0t16 = sdef_name_lookup_str(sd_names, source, starts, ends, "foo");
  int si = _sv0t16;
  if ((si != 0)) {
    return 3;
  } else {
  }
  int _sv0t17 = sdef_name_lookup_str(sd_names, source, starts, ends, "bar");
  int si2 = _sv0t17;
  int _sv0t18 = (0 - 1);
  if ((si2 != _sv0t18)) {
    return 4;
  } else {
  }
  int _sv0t19 = sv0_vec_new();
  int fnames = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int frtypes = _sv0t20;
  sv0_vec_push(fnames, 1);
  int _sv0t21 = TY_BOOL();
  sv0_vec_push(frtypes, _sv0t21);
  int _sv0t22 = sv0_vec_new();
  int ibo = _sv0t22;
  int _sv0t23 = synth_call_ret_type(fnames, frtypes, source, starts, ends, "foo", ibo);
  int rt = _sv0t23;
  int _sv0t24 = TY_BOOL();
  if ((rt != _sv0t24)) {
    return 5;
  } else {
  }
  int _sv0t25 = sv0_vec_new();
  int ibo2 = _sv0t25;
  int _sv0t26 = synth_call_ret_type(fnames, frtypes, source, starts, ends, "println", ibo2);
  int rt2 = _sv0t26;
  int _sv0t27 = TY_UNIT();
  if ((rt2 != _sv0t27)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_named_only_ty_basic(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  sv0_vec_push(sn, 7);
  sv0_vec_push(sn, 8);
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(en, 9);
  int _sv0t2 = named_only_ty(sn, en, 7);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = named_only_ty(sn, en, 9);
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
  int _sv0t73 = test_try_success_validate();
  int r38 = _sv0t73;
  if ((r38 != 0)) {
    int _sv0t74 = (400 + r38);
    return _sv0t74;
  } else {
  }
  int _sv0t75 = test_int_width_from_name();
  int r39 = _sv0t75;
  if ((r39 != 0)) {
    int _sv0t76 = (410 + r39);
    return _sv0t76;
  } else {
  }
  int _sv0t77 = test_ast_ty_payload();
  int r40 = _sv0t77;
  if ((r40 != 0)) {
    int _sv0t78 = (420 + r40);
    return _sv0t78;
  } else {
  }
  int _sv0t79 = test_enum_ctor_count();
  int r41 = _sv0t79;
  if ((r41 != 0)) {
    int _sv0t80 = (430 + r41);
    return _sv0t80;
  } else {
  }
  int _sv0t81 = test_binop_unop_result_ty();
  int r42 = _sv0t81;
  if ((r42 != 0)) {
    int _sv0t82 = (440 + r42);
    return _sv0t82;
  } else {
  }
  int _sv0t83 = test_env_entry_index();
  int r43 = _sv0t83;
  if ((r43 != 0)) {
    int _sv0t84 = (450 + r43);
    return _sv0t84;
  } else {
  }
  int _sv0t85 = test_fn_table();
  int r44 = _sv0t85;
  if ((r44 != 0)) {
    int _sv0t86 = (460 + r44);
    return _sv0t86;
  } else {
  }
  int _sv0t87 = test_scan_struct_field_names();
  int r45 = _sv0t87;
  if ((r45 != 0)) {
    int _sv0t88 = (470 + r45);
    return _sv0t88;
  } else {
  }
  int _sv0t89 = test_scan_enum_variant_names();
  int r46 = _sv0t89;
  if ((r46 != 0)) {
    int _sv0t90 = (480 + r46);
    return _sv0t90;
  } else {
  }
  int _sv0t91 = test_scan_type_tag_at();
  int r47 = _sv0t91;
  if ((r47 != 0)) {
    int _sv0t92 = (490 + r47);
    return _sv0t92;
  } else {
  }
  int _sv0t93 = test_scan_struct_field_type_tags();
  int r48 = _sv0t93;
  if ((r48 != 0)) {
    int _sv0t94 = (500 + r48);
    return _sv0t94;
  } else {
  }
  int _sv0t95 = test_scan_fn_param_type_tags();
  int r49 = _sv0t95;
  if ((r49 != 0)) {
    int _sv0t96 = (510 + r49);
    return _sv0t96;
  } else {
  }
  int _sv0t97 = test_fn_table_lookup_str();
  int r50 = _sv0t97;
  if ((r50 != 0)) {
    int _sv0t98 = (520 + r50);
    return _sv0t98;
  } else {
  }
  int _sv0t99 = test_scan_fn_ret_type_tag();
  int r51 = _sv0t99;
  if ((r51 != 0)) {
    int _sv0t100 = (530 + r51);
    return _sv0t100;
  } else {
  }
  int _sv0t101 = test_register_all_item_fns();
  int r52 = _sv0t101;
  if ((r52 != 0)) {
    int _sv0t102 = (540 + r52);
    return _sv0t102;
  } else {
  }
  int _sv0t103 = test_scan_enum_variant_shapes();
  int r53 = _sv0t103;
  if ((r53 != 0)) {
    int _sv0t104 = (550 + r53);
    return _sv0t104;
  } else {
  }
  int _sv0t105 = test_init_struct_defs();
  int r54 = _sv0t105;
  if ((r54 != 0)) {
    int _sv0t106 = (560 + r54);
    return _sv0t106;
  } else {
  }
  int _sv0t107 = test_init_enum_defs();
  int r55 = _sv0t107;
  if ((r55 != 0)) {
    int _sv0t108 = (570 + r55);
    return _sv0t108;
  } else {
  }
  int _sv0t109 = test_init_enum_defs_dup_variants();
  int r55d = _sv0t109;
  if ((r55d != 0)) {
    int _sv0t110 = (574 + r55d);
    return _sv0t110;
  } else {
  }
  int _sv0t111 = test_builtin_fn_lookup();
  int r56 = _sv0t111;
  if ((r56 != 0)) {
    int _sv0t112 = (580 + r56);
    return _sv0t112;
  } else {
  }
  int _sv0t113 = test_resolve_fn_call();
  int r57 = _sv0t113;
  if ((r57 != 0)) {
    int _sv0t114 = (600 + r57);
    return _sv0t114;
  } else {
  }
  int _sv0t115 = test_enum_ctor_resolution();
  int r58 = _sv0t115;
  if ((r58 != 0)) {
    int _sv0t116 = (610 + r58);
    return _sv0t116;
  } else {
  }
  int _sv0t117 = test_synth_building_blocks();
  int r59 = _sv0t117;
  if ((r59 != 0)) {
    int _sv0t118 = (620 + r59);
    return _sv0t118;
  } else {
  }
  int _sv0t119 = test_synth_expr();
  int r60 = _sv0t119;
  if ((r60 != 0)) {
    int _sv0t120 = (630 + r60);
    return _sv0t120;
  } else {
  }
  int _sv0t121 = test_scan_fn_param_names();
  int r61 = _sv0t121;
  if ((r61 != 0)) {
    int _sv0t122 = (650 + r61);
    return _sv0t122;
  } else {
  }
  int _sv0t123 = test_check_fn_body();
  int r62 = _sv0t123;
  if ((r62 != 0)) {
    int _sv0t124 = (660 + r62);
    return _sv0t124;
  } else {
  }
  int _sv0t125 = test_process_item_use();
  int r63 = _sv0t125;
  if ((r63 != 0)) {
    int _sv0t126 = (670 + r63);
    return _sv0t126;
  } else {
  }
  int _sv0t127 = test_build_mod_env();
  int r64 = _sv0t127;
  if ((r64 != 0)) {
    int _sv0t128 = (680 + r64);
    return _sv0t128;
  } else {
  }
  int _sv0t129 = test_check_program();
  int r65 = _sv0t129;
  if ((r65 != 0)) {
    int _sv0t130 = (685 + r65);
    return _sv0t130;
  } else {
  }
  int _sv0t131 = test_named_only_ty_basic();
  int r66 = _sv0t131;
  if ((r66 != 0)) {
    int _sv0t132 = (690 + r66);
    return _sv0t132;
  } else {
  }
  return 0;
}

