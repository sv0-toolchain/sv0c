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
static int should_allow_self(int item_tag);
static const char* error_code_unbound_ident(void);
static const char* error_code_unknown_type(void);
static const char* error_code_pat_or_unsupported(void);
static const char* error_code_wrong_arity(void);
static const char* error_code_use_clause(void);
static int arity_matches(int expected, int actual);
static int check_call_arity(int expected, int actual);
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
  int _sv0t17 = (0 - 1);
  return _sv0t17;
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
  return 0;
}

