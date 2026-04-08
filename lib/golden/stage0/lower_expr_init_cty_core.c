#include "sv0_runtime.h"

static int ec_int(void);
static int ec_struct_name(void);
static int ec_enum_ty(void);
static int ec_callee_ret(void);
static int expr_init_cty_class(int expr_shape_tag, int inner_shape);

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

int main(void) {
  int _sv0t0 = expr_init_cty_class(1, 0);
  int _sv0t1 = ec_struct_name();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = expr_init_cty_class(2, 0);
  int _sv0t3 = ec_enum_ty();
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = expr_init_cty_class(3, 0);
  int _sv0t5 = ec_callee_ret();
  if ((_sv0t4 != _sv0t5)) {
    return 1;
  } else {
  }
  int _sv0t6 = expr_init_cty_class(4, 0);
  int _sv0t7 = ec_enum_ty();
  if ((_sv0t6 != _sv0t7)) {
    return 1;
  } else {
  }
  int _sv0t8 = expr_init_cty_class(5, 7);
  int _sv0t9 = ec_int();
  if ((_sv0t8 != _sv0t9)) {
    return 1;
  } else {
  }
  int _sv0t10 = expr_init_cty_class(5, 1);
  int _sv0t11 = ec_struct_name();
  if ((_sv0t10 != _sv0t11)) {
    return 1;
  } else {
  }
  int _sv0t12 = expr_init_cty_class(6, 0);
  int _sv0t13 = ec_int();
  if ((_sv0t12 != _sv0t13)) {
    return 1;
  } else {
  }
  int _sv0t14 = expr_init_cty_class(7, 0);
  int _sv0t15 = ec_int();
  if ((_sv0t14 != _sv0t15)) {
    return 1;
  } else {
  }
  int _sv0t16 = expr_init_cty_class(99, 0);
  int _sv0t17 = ec_int();
  if ((_sv0t16 != _sv0t17)) {
    return 1;
  } else {
  }
  return 0;
}

