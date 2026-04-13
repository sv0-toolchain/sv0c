#include "sv0_runtime.h"

static const char* path_join2(const char* a, const char* b);
static const char* mangle_use_target(const char* mod_name, const char* item_name);
static int is_intrinsic(const char* name);
static int intrinsic_arity(const char* name);
static int intrinsic_count(void);
static int is_string_intrinsic(const char* name);
static int is_vec_intrinsic(const char* name);
static int is_box_intrinsic(const char* name);
static int item_registers_fn(int tag);
static int item_registers_type(int tag);
static int item_registers_variants(int tag);
static int item_is_use(int tag);
static int item_is_module(int tag);
static int item_needs_registration(int tag);
static int enum_variant_arity(int variant_kind, int tuple_fields, int struct_fields);
static const char* enum_variant_qualified_path(const char* enum_name, const char* variant_name);
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
  return 0;
}

