#include "sv0_runtime.h"

static int c_ty_void(void);
static int c_ty_int(void);
static int explicit_payload_ast_ty_class(int ty_shape_tag);
static int ec_int(void);
static int ec_struct_name(void);
static int ec_enum_ty(void);
static int ec_callee_ret(void);
static int expr_init_cty_class(int expr_shape_tag, int inner_shape);
static int scan_lets_binding_packed(int stmt_let_patbind_with_init, int has_explicit_ty, int ty_shape_tag, int expr_shape_tag, int expr_inner_shape);

static int c_ty_void(void) {
  return 1;
}

static int c_ty_int(void) {
  return 2;
}

static int explicit_payload_ast_ty_class(int ty_shape_tag) {
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
        int _sv0t7 = c_ty_int();
        return _sv0t7;
        _sv0t5 = 0;
      }
      _sv0t3 = _sv0t5;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ec_int(void) {
  return 1;
}

static int ec_struct_name(void) {
  return 2;
}

static int ec_enum_ty(void) {
  return 3;
}

static int ec_callee_ret(void) {
  return 4;
}

static int expr_init_cty_class(int expr_shape_tag, int inner_shape) {
  int _sv0t0;
  int _sv0t1;
  if ((expr_shape_tag == 1)) {
    int _sv0t2 = ec_struct_name();
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((expr_shape_tag == 2)) {
      int _sv0t4 = ec_enum_ty();
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      int _sv0t5;
      if ((expr_shape_tag == 3)) {
        int _sv0t6 = ec_callee_ret();
        return _sv0t6;
        _sv0t5 = 0;
      } else {
        int _sv0t7;
        if ((expr_shape_tag == 4)) {
          int _sv0t8 = ec_enum_ty();
          return _sv0t8;
          _sv0t7 = 0;
        } else {
          int _sv0t9;
          if ((expr_shape_tag == 5)) {
            int _sv0t10 = expr_init_cty_class(inner_shape, 0);
            return _sv0t10;
            _sv0t9 = 0;
          } else {
            int _sv0t11;
            if ((expr_shape_tag == 6)) {
              int _sv0t12 = ec_int();
              return _sv0t12;
              _sv0t11 = 0;
            } else {
              int _sv0t13;
              if ((expr_shape_tag == 7)) {
                int _sv0t14 = ec_int();
                return _sv0t14;
                _sv0t13 = 0;
              } else {
                int _sv0t15 = ec_int();
                return _sv0t15;
                _sv0t13 = 0;
              }
              _sv0t11 = _sv0t13;
            }
            _sv0t9 = _sv0t11;
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

static int scan_lets_binding_packed(int stmt_let_patbind_with_init, int has_explicit_ty, int ty_shape_tag, int expr_shape_tag, int expr_inner_shape) {
  int _sv0t0;
  int _sv0t1;
  if ((stmt_let_patbind_with_init == 0)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((has_explicit_ty != 0)) {
      int _sv0t3 = explicit_payload_ast_ty_class(ty_shape_tag);
      int _sv0t4 = (1000 + _sv0t3);
      return _sv0t4;
      _sv0t2 = 0;
    } else {
      int _sv0t5 = expr_init_cty_class(expr_shape_tag, expr_inner_shape);
      int _sv0t6 = (2000 + _sv0t5);
      return _sv0t6;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = scan_lets_binding_packed(0, 0, 2, 7, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = scan_lets_binding_packed(1, 1, 2, 0, 0);
  if ((_sv0t1 != 1002)) {
    return 1;
  } else {
  }
  int _sv0t2 = scan_lets_binding_packed(1, 1, 0, 0, 0);
  if ((_sv0t2 != 1001)) {
    return 1;
  } else {
  }
  int _sv0t3 = scan_lets_binding_packed(1, 0, 0, 7, 0);
  if ((_sv0t3 != 2001)) {
    return 1;
  } else {
  }
  int _sv0t4 = scan_lets_binding_packed(1, 0, 0, 1, 0);
  if ((_sv0t4 != 2002)) {
    return 1;
  } else {
  }
  int _sv0t5 = scan_lets_binding_packed(1, 0, 0, 3, 0);
  if ((_sv0t5 != 2004)) {
    return 1;
  } else {
  }
  int _sv0t6 = scan_lets_binding_packed(1, 0, 0, 5, 1);
  if ((_sv0t6 != 2002)) {
    return 1;
  } else {
  }
  return 0;
}

