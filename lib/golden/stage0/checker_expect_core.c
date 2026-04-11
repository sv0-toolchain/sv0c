#include "sv0_runtime.h"

static int expect_ok(int got_tag, int want_tag);
static int expect_error_code(void);
static int loop_depth_enter(int current);
static int loop_depth_exit(int current);
static int in_loop(int depth);
static int type_param_scope_size(int outer, int new_params);
static int type_param_restore(int outer);
static int unify_primitives(int a, int b);
static int unify_with_var(int is_var);

static int expect_ok(int got_tag, int want_tag) {
  int _sv0t0 = (got_tag == want_tag);
  return _sv0t0;
}

static int expect_error_code(void) {
  return 400;
}

static int loop_depth_enter(int current) {
  int _sv0t0 = (current + 1);
  return _sv0t0;
}

static int loop_depth_exit(int current) {
  if ((current <= 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = (current - 1);
  return _sv0t0;
}

static int in_loop(int depth) {
  int _sv0t0 = (depth > 0);
  return _sv0t0;
}

static int type_param_scope_size(int outer, int new_params) {
  int _sv0t0 = (outer + new_params);
  return _sv0t0;
}

static int type_param_restore(int outer) {
  return outer;
}

static int unify_primitives(int a, int b) {
  int _sv0t0 = (a == b);
  return _sv0t0;
}

static int unify_with_var(int is_var) {
  return is_var;
}

int main(void) {
  int _sv0t0 = expect_ok(1, 1);
  if ((!_sv0t0)) {
    return 1;
  } else {
  }
  int _sv0t1 = expect_ok(1, 2);
  if (_sv0t1) {
    return 2;
  } else {
  }
  int _sv0t2 = expect_error_code();
  if ((_sv0t2 != 400)) {
    return 3;
  } else {
  }
  int _sv0t3 = loop_depth_enter(0);
  if ((_sv0t3 != 1)) {
    return 10;
  } else {
  }
  int _sv0t4 = loop_depth_enter(2);
  if ((_sv0t4 != 3)) {
    return 11;
  } else {
  }
  int _sv0t5 = loop_depth_exit(1);
  if ((_sv0t5 != 0)) {
    return 12;
  } else {
  }
  int _sv0t6 = loop_depth_exit(0);
  if ((_sv0t6 != 0)) {
    return 13;
  } else {
  }
  int _sv0t7 = in_loop(0);
  if (_sv0t7) {
    return 20;
  } else {
  }
  int _sv0t8 = in_loop(1);
  if ((!_sv0t8)) {
    return 21;
  } else {
  }
  int _sv0t9 = in_loop(3);
  if ((!_sv0t9)) {
    return 22;
  } else {
  }
  int _sv0t10 = type_param_scope_size(0, 2);
  if ((_sv0t10 != 2)) {
    return 30;
  } else {
  }
  int _sv0t11 = type_param_scope_size(3, 1);
  if ((_sv0t11 != 4)) {
    return 31;
  } else {
  }
  int _sv0t12 = type_param_restore(3);
  if ((_sv0t12 != 3)) {
    return 32;
  } else {
  }
  int _sv0t13 = unify_primitives(5, 5);
  if ((!_sv0t13)) {
    return 40;
  } else {
  }
  int _sv0t14 = unify_primitives(5, 6);
  if (_sv0t14) {
    return 41;
  } else {
  }
  int _sv0t15 = unify_with_var(1);
  if ((!_sv0t15)) {
    return 50;
  } else {
  }
  int _sv0t16 = unify_with_var(0);
  if (_sv0t16) {
    return 51;
  } else {
  }
  return 0;
}

