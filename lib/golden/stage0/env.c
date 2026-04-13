#include "sv0_runtime.h"

static int is_prelude_type(const char* name);
static int vec_contains(int v, int name);
static int env_new_mod_vals(void);
static int env_new_mod_tys(void);
static int env_new_frames(void);
static int dup_mod_val(int name, int mod_vals);
static int dup_mod_ty(int name, int mod_tys);
static int register_module_value(int mod_vals, int name);
static int register_module_type(int mod_tys, int name);
static int type_is_registered_by_handle(int name_h, int mod_tys);
static int env_new_fn_arities(void);
static int dup_fn_arity(int fn_arities, int name);
static int register_fn_arity(int fn_arities, int name, int arity);
static int lookup_fn_arity(int fn_arities, int name);
static int env_new_ty_aliases(void);
static int has_ty_alias_name(int ty_aliases, int from);
static int find_ty_alias_target(int ty_aliases, int from);
static int resolve_canonical_ty(int ty_aliases, int name);
static int register_type_alias(int ty_aliases, int from, int to);
static int register_value_alias(int mod_vals, int fn_arities, int local_name, int target_name);
static int lookup_type(int mod_tys, int ty_aliases, int name_h, const char* name_str, int allow_self, int self_h);
static int enter_scope(int frames);
static int exit_scope(int frames);
static int bind_local(int frames, int name);
static int in_frame_vec(int name, int frame);
static int lookup_in_frames(int frames, int name);
static int lookup_value(int mod_vals, int frames, int name);
static int test_prelude_types(void);
static int test_vec_contains(void);
static int test_dup_mod_val(void);
static int test_register_module_value(void);
static int test_scope_enter_exit(void);
static int test_bind_and_lookup(void);
static int test_fn_arity(void);
static int test_ty_alias(void);
static int test_value_alias(void);
static int test_lookup_type(void);

static int is_prelude_type(const char* name) {
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

static int vec_contains(int v, int name) {
  int _sv0t0 = sv0_vec_len(v);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(v, i);
    int n = _sv0t1;
    if ((n == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int env_new_mod_vals(void) {
  int _sv0t0 = sv0_vec_new();
  return _sv0t0;
}

static int env_new_mod_tys(void) {
  int _sv0t0 = sv0_vec_new();
  return _sv0t0;
}

static int env_new_frames(void) {
  int _sv0t0 = sv0_vec_new();
  return _sv0t0;
}

static int dup_mod_val(int name, int mod_vals) {
  int _sv0t0 = vec_contains(mod_vals, name);
  return _sv0t0;
}

static int dup_mod_ty(int name, int mod_tys) {
  int _sv0t0 = vec_contains(mod_tys, name);
  return _sv0t0;
}

static int register_module_value(int mod_vals, int name) {
  sv0_vec_push(mod_vals, name);
  return 0;
}

static int register_module_type(int mod_tys, int name) {
  sv0_vec_push(mod_tys, name);
  return 0;
}

static int type_is_registered_by_handle(int name_h, int mod_tys) {
  int _sv0t0 = vec_contains(mod_tys, name_h);
  return _sv0t0;
}

static int env_new_fn_arities(void) {
  int _sv0t0 = sv0_vec_new();
  return _sv0t0;
}

static int dup_fn_arity(int fn_arities, int name) {
  int _sv0t0 = sv0_vec_len(fn_arities);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(fn_arities, i);
    if ((_sv0t1 == name)) {
      return 1;
    } else {
    }
    i = (i + 2);
  }
  return 0;
}

static int register_fn_arity(int fn_arities, int name, int arity) {
  sv0_vec_push(fn_arities, name);
  sv0_vec_push(fn_arities, arity);
  return 0;
}

static int lookup_fn_arity(int fn_arities, int name) {
  int _sv0t0 = sv0_vec_len(fn_arities);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(fn_arities, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = (i + 1);
      int _sv0t3 = sv0_vec_get(fn_arities, _sv0t2);
      return _sv0t3;
    } else {
    }
    i = (i + 2);
  }
  int _sv0t4 = (-1);
  return _sv0t4;
}

static int env_new_ty_aliases(void) {
  int _sv0t0 = sv0_vec_new();
  return _sv0t0;
}

static int has_ty_alias_name(int ty_aliases, int from) {
  int _sv0t0 = sv0_vec_len(ty_aliases);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(ty_aliases, i);
    if ((_sv0t1 == from)) {
      return 1;
    } else {
    }
    i = (i + 2);
  }
  return 0;
}

static int find_ty_alias_target(int ty_aliases, int from) {
  int _sv0t0 = sv0_vec_len(ty_aliases);
  int len = _sv0t0;
  int i = 0;
  while ((i < len)) {
    int _sv0t1 = sv0_vec_get(ty_aliases, i);
    if ((_sv0t1 == from)) {
      int _sv0t2 = (i + 1);
      int _sv0t3 = sv0_vec_get(ty_aliases, _sv0t2);
      return _sv0t3;
    } else {
    }
    i = (i + 2);
  }
  int _sv0t4 = (-1);
  return _sv0t4;
}

static int resolve_canonical_ty(int ty_aliases, int name) {
  int _sv0t0 = find_ty_alias_target(ty_aliases, name);
  int target = _sv0t0;
  int _sv0t1 = (-1);
  if ((target == _sv0t1)) {
    return name;
  } else {
  }
  if ((target == name)) {
    return name;
  } else {
  }
  int _sv0t2 = resolve_canonical_ty(ty_aliases, target);
  return _sv0t2;
}

static int register_type_alias(int ty_aliases, int from, int to) {
  sv0_vec_push(ty_aliases, from);
  sv0_vec_push(ty_aliases, to);
  return 0;
}

static int register_value_alias(int mod_vals, int fn_arities, int local_name, int target_name) {
  int _sv0t0 = lookup_fn_arity(fn_arities, target_name);
  int ar = _sv0t0;
  int _sv0t1 = (-1);
  if ((ar == _sv0t1)) {
    int _sv0t2 = (-1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = register_module_value(mod_vals, local_name);
  int _sv0t4 = register_fn_arity(fn_arities, local_name, ar);
  return 0;
}

static int lookup_type(int mod_tys, int ty_aliases, int name_h, const char* name_str, int allow_self, int self_h) {
  if ((name_h == self_h)) {
    int _sv0t0 = (allow_self == 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = resolve_canonical_ty(ty_aliases, name_h);
  int canon = _sv0t1;
  if ((canon != name_h)) {
    int _sv0t2 = vec_contains(mod_tys, canon);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = is_prelude_type(name_str);
  if (_sv0t3) {
    return 1;
  } else {
  }
  int _sv0t4 = vec_contains(mod_tys, name_h);
  return _sv0t4;
}

static int enter_scope(int frames) {
  int _sv0t0 = sv0_vec_new();
  int new_frame = _sv0t0;
  sv0_vec_push(frames, new_frame);
  return 0;
}

static int exit_scope(int frames) {
  int _sv0t0 = sv0_vec_len(frames);
  int len = _sv0t0;
  if ((len == 0)) {
    return 0;
  } else {
  }
  int _sv0t1 = (len - 1);
  sv0_vec_set(frames, _sv0t1, 0);
  return 0;
}

static int bind_local(int frames, int name) {
  int _sv0t0 = sv0_vec_len(frames);
  int len = _sv0t0;
  if ((len == 0)) {
    int _sv0t1 = sv0_vec_new();
    int new_frame = _sv0t1;
    sv0_vec_push(new_frame, name);
    sv0_vec_push(frames, new_frame);
    return 0;
  } else {
  }
  int _sv0t2 = (len - 1);
  int _sv0t3 = sv0_vec_get(frames, _sv0t2);
  int top = _sv0t3;
  sv0_vec_push(top, name);
  return 0;
}

static int in_frame_vec(int name, int frame) {
  int _sv0t0 = vec_contains(frame, name);
  return _sv0t0;
}

static int lookup_in_frames(int frames, int name) {
  int _sv0t0 = sv0_vec_len(frames);
  int len = _sv0t0;
  int i = (len - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(frames, i);
    int frame = _sv0t1;
    int _sv0t2 = in_frame_vec(name, frame);
    if (_sv0t2) {
      return 1;
    } else {
    }
    i = (i - 1);
  }
  return 0;
}

static int lookup_value(int mod_vals, int frames, int name) {
  int _sv0t0 = lookup_in_frames(frames, name);
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = vec_contains(mod_vals, name);
  return _sv0t1;
}

static int test_prelude_types(void) {
  int _sv0t0 = is_prelude_type("i32");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_prelude_type("bool");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_prelude_type("Vec");
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_prelude_type("Box");
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_prelude_type("String");
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_prelude_type("FooBar");
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_prelude_type("");
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_vec_contains(void) {
  int _sv0t0 = sv0_vec_new();
  int v = _sv0t0;
  int _sv0t1 = vec_contains(v, 42);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  sv0_vec_push(v, 10);
  sv0_vec_push(v, 20);
  sv0_vec_push(v, 30);
  int _sv0t2 = vec_contains(v, 20);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = vec_contains(v, 99);
  if ((_sv0t3 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_dup_mod_val(void) {
  int _sv0t0 = sv0_vec_new();
  int mv = _sv0t0;
  sv0_vec_push(mv, 100);
  sv0_vec_push(mv, 200);
  int _sv0t1 = dup_mod_val(100, mv);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = dup_mod_val(300, mv);
  if ((_sv0t2 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_register_module_value(void) {
  int _sv0t0 = sv0_vec_new();
  int mv = _sv0t0;
  int _sv0t1 = register_module_value(mv, 42);
  int _sv0t2 = sv0_vec_len(mv);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = vec_contains(mv, 42);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  int _sv0t4 = register_module_value(mv, 43);
  int _sv0t5 = sv0_vec_len(mv);
  if ((_sv0t5 != 2)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_scope_enter_exit(void) {
  int _sv0t0 = sv0_vec_new();
  int frames = _sv0t0;
  int _sv0t1 = enter_scope(frames);
  int _sv0t2 = sv0_vec_len(frames);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = enter_scope(frames);
  int _sv0t4 = sv0_vec_len(frames);
  if ((_sv0t4 != 2)) {
    return 2;
  } else {
  }
  int _sv0t5 = exit_scope(frames);
  return 0;
}

static int test_bind_and_lookup(void) {
  int _sv0t0 = sv0_vec_new();
  int mv = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int frames = _sv0t1;
  int _sv0t2 = register_module_value(mv, 100);
  int _sv0t3 = lookup_value(mv, frames, 100);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = lookup_value(mv, frames, 200);
  if ((_sv0t4 != 0)) {
    return 2;
  } else {
  }
  int _sv0t5 = enter_scope(frames);
  int _sv0t6 = bind_local(frames, 200);
  int _sv0t7 = lookup_value(mv, frames, 200);
  if ((_sv0t7 != 1)) {
    return 3;
  } else {
  }
  int _sv0t8 = enter_scope(frames);
  int _sv0t9 = bind_local(frames, 300);
  int _sv0t10 = lookup_value(mv, frames, 300);
  if ((_sv0t10 != 1)) {
    return 4;
  } else {
  }
  int _sv0t11 = lookup_value(mv, frames, 200);
  if ((_sv0t11 != 1)) {
    return 5;
  } else {
  }
  int _sv0t12 = lookup_value(mv, frames, 999);
  if ((_sv0t12 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_fn_arity(void) {
  int _sv0t0 = env_new_fn_arities();
  int fa = _sv0t0;
  int _sv0t1 = lookup_fn_arity(fa, 10);
  int _sv0t2 = (-1);
  if ((_sv0t1 != _sv0t2)) {
    return 1;
  } else {
  }
  int _sv0t3 = register_fn_arity(fa, 10, 2);
  int _sv0t4 = register_fn_arity(fa, 20, 3);
  int _sv0t5 = lookup_fn_arity(fa, 10);
  if ((_sv0t5 != 2)) {
    return 2;
  } else {
  }
  int _sv0t6 = lookup_fn_arity(fa, 20);
  if ((_sv0t6 != 3)) {
    return 3;
  } else {
  }
  int _sv0t7 = lookup_fn_arity(fa, 99);
  int _sv0t8 = (-1);
  if ((_sv0t7 != _sv0t8)) {
    return 4;
  } else {
  }
  int _sv0t9 = dup_fn_arity(fa, 10);
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  int _sv0t10 = dup_fn_arity(fa, 99);
  if ((_sv0t10 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_ty_alias(void) {
  int _sv0t0 = env_new_ty_aliases();
  int ta = _sv0t0;
  int _sv0t1 = has_ty_alias_name(ta, 100);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = register_type_alias(ta, 100, 200);
  int _sv0t3 = has_ty_alias_name(ta, 100);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  int _sv0t4 = find_ty_alias_target(ta, 100);
  if ((_sv0t4 != 200)) {
    return 3;
  } else {
  }
  int _sv0t5 = find_ty_alias_target(ta, 999);
  int _sv0t6 = (-1);
  if ((_sv0t5 != _sv0t6)) {
    return 4;
  } else {
  }
  int _sv0t7 = resolve_canonical_ty(ta, 100);
  if ((_sv0t7 != 200)) {
    return 5;
  } else {
  }
  int _sv0t8 = resolve_canonical_ty(ta, 999);
  if ((_sv0t8 != 999)) {
    return 6;
  } else {
  }
  int _sv0t9 = register_type_alias(ta, 200, 300);
  int _sv0t10 = resolve_canonical_ty(ta, 100);
  if ((_sv0t10 != 300)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_value_alias(void) {
  int _sv0t0 = env_new_mod_vals();
  int mv = _sv0t0;
  int _sv0t1 = env_new_fn_arities();
  int fa = _sv0t1;
  int _sv0t2 = register_fn_arity(fa, 50, 4);
  int _sv0t3 = register_value_alias(mv, fa, 60, 50);
  int rc = _sv0t3;
  if ((rc != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = vec_contains(mv, 60);
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  int _sv0t5 = lookup_fn_arity(fa, 60);
  if ((_sv0t5 != 4)) {
    return 3;
  } else {
  }
  int _sv0t6 = register_value_alias(mv, fa, 70, 999);
  int bad = _sv0t6;
  int _sv0t7 = (-1);
  if ((bad != _sv0t7)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_lookup_type(void) {
  int _sv0t0 = env_new_mod_tys();
  int mt = _sv0t0;
  int _sv0t1 = env_new_ty_aliases();
  int ta = _sv0t1;
  int _sv0t2 = lookup_type(mt, ta, 1, "i32", 0, 0);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = lookup_type(mt, ta, 2, "Foo", 0, 0);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  int _sv0t4 = register_module_type(mt, 3);
  int _sv0t5 = lookup_type(mt, ta, 3, "MyStruct", 0, 0);
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  int self_h = 77;
  int _sv0t6 = lookup_type(mt, ta, self_h, "Self", 1, self_h);
  if ((_sv0t6 != 1)) {
    return 4;
  } else {
  }
  int _sv0t7 = lookup_type(mt, ta, self_h, "Self", 0, self_h);
  if ((_sv0t7 != 0)) {
    return 5;
  } else {
  }
  int _sv0t8 = register_type_alias(ta, 500, 3);
  int _sv0t9 = lookup_type(mt, ta, 500, "Alias", 0, 0);
  if ((_sv0t9 != 1)) {
    return 6;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_prelude_types();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_vec_contains();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_dup_mod_val();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_register_module_value();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_scope_enter_exit();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_bind_and_lookup();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_fn_arity();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (60 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_ty_alias();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (70 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_value_alias();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (80 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_lookup_type();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (90 + r10);
    return _sv0t18;
  } else {
  }
  return 0;
}

