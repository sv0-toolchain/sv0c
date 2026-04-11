#include "sv0_runtime.h"

static int builtin_fn_count(void);
static int is_intrinsic_fn(int fn_tag);
static int intrinsic_arg_count(int fn_tag);
static int use_clause_kind(int has_module, int has_name);
static int ctor_env_entries(int variant_count);
static int total_env_entries(int builtins, int use_imports, int ctors, int fns);

static int builtin_fn_count(void) {
  return 11;
}

static int is_intrinsic_fn(int fn_tag) {
  int _sv0t0 = (fn_tag >= 0);
  int _sv0t1 = builtin_fn_count();
  int _sv0t2 = (fn_tag < _sv0t1);
  int _sv0t3 = (_sv0t0 && _sv0t2);
  return _sv0t3;
}

static int intrinsic_arg_count(int fn_tag) {
  if ((fn_tag == 0)) {
    return 1;
  } else {
  }
  if ((fn_tag == 1)) {
    return 1;
  } else {
  }
  if ((fn_tag == 2)) {
    return 2;
  } else {
  }
  if ((fn_tag == 3)) {
    return 2;
  } else {
  }
  if ((fn_tag == 4)) {
    return 2;
  } else {
  }
  if ((fn_tag == 5)) {
    return 3;
  } else {
  }
  if ((fn_tag == 6)) {
    return 0;
  } else {
  }
  if ((fn_tag == 7)) {
    return 2;
  } else {
  }
  if ((fn_tag == 8)) {
    return 1;
  } else {
  }
  if ((fn_tag == 9)) {
    return 2;
  } else {
  }
  if ((fn_tag == 10)) {
    return 3;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int use_clause_kind(int has_module, int has_name) {
  if ((has_module && has_name)) {
    return 0;
  } else {
  }
  if (has_module) {
    return 1;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int ctor_env_entries(int variant_count) {
  return variant_count;
}

static int total_env_entries(int builtins, int use_imports, int ctors, int fns) {
  int _sv0t0 = (builtins + use_imports);
  int _sv0t1 = (_sv0t0 + ctors);
  int _sv0t2 = (_sv0t1 + fns);
  return _sv0t2;
}

int main(void) {
  int _sv0t0 = builtin_fn_count();
  if ((_sv0t0 != 11)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_intrinsic_fn(0);
  if ((!_sv0t1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_intrinsic_fn(10);
  if ((!_sv0t2)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_intrinsic_fn(11);
  if (_sv0t3) {
    return 4;
  } else {
  }
  int _sv0t4 = (-1);
  int _sv0t5 = is_intrinsic_fn(_sv0t4);
  if (_sv0t5) {
    return 5;
  } else {
  }
  int _sv0t6 = intrinsic_arg_count(0);
  if ((_sv0t6 != 1)) {
    return 6;
  } else {
  }
  int _sv0t7 = intrinsic_arg_count(2);
  if ((_sv0t7 != 2)) {
    return 7;
  } else {
  }
  int _sv0t8 = intrinsic_arg_count(6);
  if ((_sv0t8 != 0)) {
    return 8;
  } else {
  }
  int _sv0t9 = intrinsic_arg_count(99);
  int _sv0t10 = (-1);
  if ((_sv0t9 != _sv0t10)) {
    return 9;
  } else {
  }
  int _sv0t11 = use_clause_kind(1, 1);
  if ((_sv0t11 != 0)) {
    return 10;
  } else {
  }
  int _sv0t12 = use_clause_kind(1, 0);
  if ((_sv0t12 != 1)) {
    return 11;
  } else {
  }
  int _sv0t13 = use_clause_kind(0, 0);
  int _sv0t14 = (-1);
  if ((_sv0t13 != _sv0t14)) {
    return 12;
  } else {
  }
  int _sv0t15 = ctor_env_entries(3);
  if ((_sv0t15 != 3)) {
    return 13;
  } else {
  }
  int _sv0t16 = ctor_env_entries(0);
  if ((_sv0t16 != 0)) {
    return 14;
  } else {
  }
  int _sv0t17 = total_env_entries(11, 2, 5, 3);
  if ((_sv0t17 != 21)) {
    return 15;
  } else {
  }
  int _sv0t18 = total_env_entries(0, 0, 0, 0);
  if ((_sv0t18 != 0)) {
    return 16;
  } else {
  }
  return 0;
}

