#include "sv0_runtime.h"

static int c_ty_void(void);
static int c_ty_int(void);
static int c_ty_passthru_name(void);
static int param_ty_cstring_class(int ty_shape_tag);
static int callee_ret_cty_class(int prog_has_fn, int fn_has_explicit_ret, int ret_ty_shape);

static int c_ty_void(void) {
  return 1;
}

static int c_ty_int(void) {
  return 2;
}

static int c_ty_passthru_name(void) {
  return 11;
}

static int param_ty_cstring_class(int ty_shape_tag) {
  int _sv0t0;
  int _sv0t1;
  if ((ty_shape_tag == 0)) {
    int _sv0t2 = c_ty_void();
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((ty_shape_tag == 1)) {
      int _sv0t4 = c_ty_void();
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      int _sv0t5;
      if ((ty_shape_tag == 2)) {
        int _sv0t6 = c_ty_int();
        return _sv0t6;
        _sv0t5 = 0;
      } else {
        int _sv0t7;
        if ((ty_shape_tag == 3)) {
          int _sv0t8 = c_ty_int();
          return _sv0t8;
          _sv0t7 = 0;
        } else {
          int _sv0t9;
          if ((ty_shape_tag == 13)) {
            int _sv0t10 = c_ty_passthru_name();
            return _sv0t10;
            _sv0t9 = 0;
          } else {
            int _sv0t11 = c_ty_int();
            return _sv0t11;
            _sv0t9 = 0;
          }
          _sv0t7 = _sv0t9;
        }
        _sv0t5 = _sv0t7;
      }
      _sv0t3 = _sv0t5;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int callee_ret_cty_class(int prog_has_fn, int fn_has_explicit_ret, int ret_ty_shape) {
  int _sv0t0;
  int _sv0t1;
  if ((prog_has_fn == 0)) {
    int _sv0t2 = c_ty_int();
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((fn_has_explicit_ret == 0)) {
      int _sv0t4 = c_ty_int();
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      int _sv0t5 = param_ty_cstring_class(ret_ty_shape);
      return _sv0t5;
      _sv0t3 = 0;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = callee_ret_cty_class(0, 0, 0);
  int _sv0t1 = c_ty_int();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = callee_ret_cty_class(0, 1, 2);
  int _sv0t3 = c_ty_int();
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = callee_ret_cty_class(1, 0, 0);
  int _sv0t5 = c_ty_int();
  if ((_sv0t4 != _sv0t5)) {
    return 1;
  } else {
  }
  int _sv0t6 = callee_ret_cty_class(1, 1, 2);
  int _sv0t7 = c_ty_int();
  if ((_sv0t6 != _sv0t7)) {
    return 1;
  } else {
  }
  int _sv0t8 = callee_ret_cty_class(1, 1, 0);
  int _sv0t9 = c_ty_void();
  if ((_sv0t8 != _sv0t9)) {
    return 1;
  } else {
  }
  int _sv0t10 = callee_ret_cty_class(1, 1, 13);
  int _sv0t11 = c_ty_passthru_name();
  if ((_sv0t10 != _sv0t11)) {
    return 1;
  } else {
  }
  return 0;
}

