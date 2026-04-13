#include "sv0_runtime.h"

static int fresh_tmp_name(int counter);
static int next_tmp(int counter);
static int find_double_colon(const char* s, int start);
static int split_qname_count(const char* s);
static int has_qualified_name(const char* s);
static const char* ast_binop_to_c(int tag);
static const char* ast_unop_to_c(int tag);
static int is_supported_unop(int tag);
static const char* ast_ty_to_c_string(const char* name);
static int is_void_cty(const char* name);
static int ast_ty_is_unit(const char* name);
static const char* old_slot_prefix(void);
static int old_slot_name_len(int name_len);
static int ret_syntax_is_unit(int has_ret, const char* ret_ty_name);
static int use_ret_slot(int is_unit, int ens_mentions_result);
static int lower_lit_supported(int lit_tag);
static int SV_SKIP(void);
static int SV_STORE(void);
static int SV_LOAD(void);
static int store_value_class(int is_var, int var_name_eq_slot);
static int LOW_AL_VAR(void);
static int LOW_AL_FIELD(void);
static int LOW_AL_INVALID(void);
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
static int LIT_INT(void);
static int LIT_BOOL(void);
static int LIT_UNIT(void);
static int LIT_STRING(void);
static int LIT_UNSUPPORTED(void);
static int classify_literal(int lit_tag);
static int VAL_VAR(void);
static int VAL_OTHER(void);
static int value_to_expr_kind(int val_tag);
static int payload_field_name(int index);
static int enum_store_payload_count(int val_count);
static int enum_alloc_width(int max_payload);
static const char* ret_value_slot(void);
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

static int fresh_tmp_name(int counter) {
  return counter;
}

static int next_tmp(int counter) {
  int _sv0t0 = (counter + 1);
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

static int SV_SKIP(void) {
  return 0;
}

static int SV_STORE(void) {
  return 1;
}

static int SV_LOAD(void) {
  return 2;
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

static int LOW_AL_VAR(void) {
  return 0;
}

static int LOW_AL_FIELD(void) {
  return 1;
}

static int LOW_AL_INVALID(void) {
  int _sv0t0 = (0 - 1);
  return _sv0t0;
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

static int LIT_INT(void) {
  return 0;
}

static int LIT_BOOL(void) {
  return 1;
}

static int LIT_UNIT(void) {
  return 2;
}

static int LIT_STRING(void) {
  return 3;
}

static int LIT_UNSUPPORTED(void) {
  int _sv0t0 = (0 - 1);
  return _sv0t0;
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

static int VAL_VAR(void) {
  return 0;
}

static int VAL_OTHER(void) {
  return 1;
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
  int _sv0t4 = is_supported_unop(0);
  if ((_sv0t4 != 1)) {
    return 3;
  } else {
  }
  int _sv0t5 = is_supported_unop(1);
  if ((_sv0t5 != 1)) {
    return 4;
  } else {
  }
  int _sv0t6 = is_supported_unop(5);
  if ((_sv0t6 != 0)) {
    return 5;
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
  return 0;
}

