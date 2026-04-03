#include "sv0_runtime.h"

static int item_arm_module(void);
static int item_arm_use(void);
static int item_arm_fn(void);
static int item_arm_struct(void);
static int item_arm_enum(void);
static int item_arm_trait(void);
static int item_arm_impl(void);
static int item_arm_type(void);
static int item_arm_newtype(void);
static int tok_item_module(void);
static int tok_item_use(void);
static int tok_item_struct(void);
static int tok_item_enum(void);
static int tok_item_trait(void);
static int tok_item_impl(void);
static int tok_item_type(void);
static int tok_item_newtype(void);
static int parse_item_dispatch(int t);
static int lexer_keyword_fn_to_item_arm(int kw);
static int item_arm_has_item_body(int arm);

static int item_arm_module(void) {
  return 1;
}

static int item_arm_use(void) {
  return 2;
}

static int item_arm_fn(void) {
  return 3;
}

static int item_arm_struct(void) {
  return 4;
}

static int item_arm_enum(void) {
  return 5;
}

static int item_arm_trait(void) {
  return 6;
}

static int item_arm_impl(void) {
  return 7;
}

static int item_arm_type(void) {
  return 8;
}

static int item_arm_newtype(void) {
  return 9;
}

static int tok_item_module(void) {
  return 180;
}

static int tok_item_use(void) {
  return 181;
}

static int tok_item_struct(void) {
  return 182;
}

static int tok_item_enum(void) {
  return 183;
}

static int tok_item_trait(void) {
  return 184;
}

static int tok_item_impl(void) {
  return 185;
}

static int tok_item_type(void) {
  return 186;
}

static int tok_item_newtype(void) {
  return 187;
}

static int parse_item_dispatch(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 180)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 181)) {
      return 2;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 1)) {
        return 3;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((t == 182)) {
          return 4;
          _sv0t4 = 0;
        } else {
          int _sv0t5;
          if ((t == 183)) {
            return 5;
            _sv0t5 = 0;
          } else {
            int _sv0t6;
            if ((t == 184)) {
              return 6;
              _sv0t6 = 0;
            } else {
              int _sv0t7;
              if ((t == 185)) {
                return 7;
                _sv0t7 = 0;
              } else {
                int _sv0t8;
                if ((t == 186)) {
                  return 8;
                  _sv0t8 = 0;
                } else {
                  int _sv0t9;
                  if ((t == 187)) {
                    return 9;
                    _sv0t9 = 0;
                  } else {
                    return 0;
                    _sv0t9 = 0;
                  }
                  _sv0t8 = _sv0t9;
                }
                _sv0t7 = _sv0t8;
              }
              _sv0t6 = _sv0t7;
            }
            _sv0t5 = _sv0t6;
          }
          _sv0t4 = _sv0t5;
        }
        _sv0t3 = _sv0t4;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int lexer_keyword_fn_to_item_arm(int kw) {
  int _sv0t0;
  int _sv0t1;
  if ((kw == 1)) {
    return 3;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int item_arm_has_item_body(int arm) {
  int _sv0t0;
  int _sv0t1;
  if ((arm == 3)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((arm == 4)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((arm == 5)) {
        return 1;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((arm == 6)) {
          return 1;
          _sv0t4 = 0;
        } else {
          int _sv0t5;
          if ((arm == 7)) {
            return 1;
            _sv0t5 = 0;
          } else {
            return 0;
            _sv0t5 = 0;
          }
          _sv0t4 = _sv0t5;
        }
        _sv0t3 = _sv0t4;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = tok_item_module();
  int _sv0t1 = parse_item_dispatch(_sv0t0);
  int _sv0t2 = item_arm_module();
  int f0 = (_sv0t1 - _sv0t2);
  int _sv0t3 = tok_item_use();
  int _sv0t4 = parse_item_dispatch(_sv0t3);
  int _sv0t5 = item_arm_use();
  int f1 = (_sv0t4 - _sv0t5);
  int _sv0t6 = parse_item_dispatch(1);
  int _sv0t7 = item_arm_fn();
  int f2 = (_sv0t6 - _sv0t7);
  int _sv0t8 = tok_item_struct();
  int _sv0t9 = parse_item_dispatch(_sv0t8);
  int _sv0t10 = item_arm_struct();
  int f3 = (_sv0t9 - _sv0t10);
  int _sv0t11 = tok_item_enum();
  int _sv0t12 = parse_item_dispatch(_sv0t11);
  int _sv0t13 = item_arm_enum();
  int f4 = (_sv0t12 - _sv0t13);
  int _sv0t14 = tok_item_trait();
  int _sv0t15 = parse_item_dispatch(_sv0t14);
  int _sv0t16 = item_arm_trait();
  int f5 = (_sv0t15 - _sv0t16);
  int _sv0t17 = tok_item_impl();
  int _sv0t18 = parse_item_dispatch(_sv0t17);
  int _sv0t19 = item_arm_impl();
  int f6 = (_sv0t18 - _sv0t19);
  int _sv0t20 = tok_item_type();
  int _sv0t21 = parse_item_dispatch(_sv0t20);
  int _sv0t22 = item_arm_type();
  int f7 = (_sv0t21 - _sv0t22);
  int _sv0t23 = tok_item_newtype();
  int _sv0t24 = parse_item_dispatch(_sv0t23);
  int _sv0t25 = item_arm_newtype();
  int f8 = (_sv0t24 - _sv0t25);
  int _sv0t26 = parse_item_dispatch(3);
  int f9 = _sv0t26;
  int _sv0t27 = lexer_keyword_fn_to_item_arm(1);
  int _sv0t28 = item_arm_fn();
  int f10 = (_sv0t27 - _sv0t28);
  int _sv0t29 = lexer_keyword_fn_to_item_arm(3);
  int f11 = _sv0t29;
  int _sv0t30 = item_arm_fn();
  int _sv0t31 = item_arm_has_item_body(_sv0t30);
  int f12 = (_sv0t31 - 1);
  int _sv0t32 = item_arm_use();
  int _sv0t33 = item_arm_has_item_body(_sv0t32);
  int f13 = _sv0t33;
  int _sv0t34 = (f0 + f1);
  int _sv0t35 = (_sv0t34 + f2);
  int _sv0t36 = (_sv0t35 + f3);
  int _sv0t37 = (_sv0t36 + f4);
  int _sv0t38 = (_sv0t37 + f5);
  int _sv0t39 = (_sv0t38 + f6);
  int _sv0t40 = (_sv0t39 + f7);
  int _sv0t41 = (_sv0t40 + f8);
  int _sv0t42 = (_sv0t41 + f9);
  int _sv0t43 = (_sv0t42 + f10);
  int _sv0t44 = (_sv0t43 + f11);
  int _sv0t45 = (_sv0t44 + f12);
  int _sv0t46 = (_sv0t45 + f13);
  return _sv0t46;
}

