#include "sv0_runtime.h"

static int item_is_fn(int item_kind);
static int fn_has_explicit_return(int ret_tag);
static int fn_param_count(int p0, int p1, int p2, int p3);
static int fn_ty_tag(void);
static int fn_ty_error_no_return(void);
static int fn_ty_error_unknown_type(void);
static int env_find_result(int found, int ty_tag);
static int env_find_is_some(int val);
static int mod_env_fn_entry(int has_name, int has_type);

static int item_is_fn(int item_kind) {
  int _sv0t0 = (item_kind == 1);
  return _sv0t0;
}

static int fn_has_explicit_return(int ret_tag) {
  int _sv0t0 = (ret_tag != 0);
  return _sv0t0;
}

static int fn_param_count(int p0, int p1, int p2, int p3) {
  int count = 0;
  if (p0) {
    count = (count + 1);
  } else {
  }
  if (p1) {
    count = (count + 1);
  } else {
  }
  if (p2) {
    count = (count + 1);
  } else {
  }
  if (p3) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int fn_ty_tag(void) {
  return 30;
}

static int fn_ty_error_no_return(void) {
  return 409;
}

static int fn_ty_error_unknown_type(void) {
  return 406;
}

static int env_find_result(int found, int ty_tag) {
  if (found) {
    return ty_tag;
  } else {
  }
  return 0;
}

static int env_find_is_some(int val) {
  int _sv0t0 = (val != 0);
  return _sv0t0;
}

static int mod_env_fn_entry(int has_name, int has_type) {
  int _sv0t0 = (has_name && has_type);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = item_is_fn(1);
  if ((!_sv0t0)) {
    return 1;
  } else {
  }
  int _sv0t1 = item_is_fn(2);
  if (_sv0t1) {
    return 2;
  } else {
  }
  int _sv0t2 = fn_has_explicit_return(5);
  if ((!_sv0t2)) {
    return 10;
  } else {
  }
  int _sv0t3 = fn_has_explicit_return(0);
  if (_sv0t3) {
    return 11;
  } else {
  }
  int _sv0t4 = fn_param_count(1, 1, 0, 0);
  if ((_sv0t4 != 2)) {
    return 20;
  } else {
  }
  int _sv0t5 = fn_param_count(1, 1, 1, 1);
  if ((_sv0t5 != 4)) {
    return 21;
  } else {
  }
  int _sv0t6 = fn_param_count(0, 0, 0, 0);
  if ((_sv0t6 != 0)) {
    return 22;
  } else {
  }
  int _sv0t7 = fn_ty_tag();
  if ((_sv0t7 != 30)) {
    return 30;
  } else {
  }
  int _sv0t8 = fn_ty_error_no_return();
  if ((_sv0t8 != 409)) {
    return 40;
  } else {
  }
  int _sv0t9 = fn_ty_error_unknown_type();
  if ((_sv0t9 != 406)) {
    return 41;
  } else {
  }
  int _sv0t10 = env_find_result(1, 5);
  int r1 = _sv0t10;
  if ((r1 != 5)) {
    return 50;
  } else {
  }
  int _sv0t11 = env_find_result(0, 5);
  int r2 = _sv0t11;
  if ((r2 != 0)) {
    return 51;
  } else {
  }
  int _sv0t12 = env_find_is_some(5);
  if ((!_sv0t12)) {
    return 60;
  } else {
  }
  int _sv0t13 = env_find_is_some(0);
  if (_sv0t13) {
    return 61;
  } else {
  }
  int _sv0t14 = mod_env_fn_entry(1, 1);
  if ((!_sv0t14)) {
    return 70;
  } else {
  }
  int _sv0t15 = mod_env_fn_entry(0, 1);
  if (_sv0t15) {
    return 71;
  } else {
  }
  int _sv0t16 = mod_env_fn_entry(1, 0);
  if (_sv0t16) {
    return 72;
  } else {
  }
  return 0;
}

