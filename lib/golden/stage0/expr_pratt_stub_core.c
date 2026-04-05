#include "sv0_runtime.h"

static int tok_int_lit(void);
static int tok_ident(void);
static int tok_plus(void);
static int tok_star(void);
static int tok_minus(void);
static int tok_semi(void);
static int is_atom(int t);
static int pratt_from_atom(int a, int t1, int t2, int t3, int t4, int t5);
static int pratt_rhs_ok_with_semi(int t0, int t1, int t2, int t3, int t4, int t5);

static int tok_int_lit(void) {
  return 40;
}

static int tok_ident(void) {
  return 73;
}

static int tok_plus(void) {
  return 20;
}

static int tok_star(void) {
  return 22;
}

static int tok_minus(void) {
  return 21;
}

static int tok_semi(void) {
  return 15;
}

static int is_atom(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 40)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 73)) {
      return 1;
      _sv0t2 = 0;
    } else {
      return 0;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int pratt_from_atom(int a, int t1, int t2, int t3, int t4, int t5) {
  int _sv0t2 = is_atom(a);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 0)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((t1 == 15)) {
      return 1;
      _sv0t3 = 0;
    } else {
      int _sv0t4;
      if ((t1 == 20)) {
        int _sv0t6 = is_atom(t2);
        int _sv0t5;
        if ((_sv0t6 == 0)) {
          return 0;
          _sv0t5 = 0;
        } else {
          int _sv0t7;
          if ((t3 == 15)) {
            return 1;
            _sv0t7 = 0;
          } else {
            int _sv0t8;
            if ((t3 == 22)) {
              int _sv0t10 = is_atom(t4);
              int _sv0t9;
              if ((_sv0t10 == 0)) {
                return 0;
                _sv0t9 = 0;
              } else {
                int _sv0t11;
                if ((t5 == 15)) {
                  return 1;
                  _sv0t11 = 0;
                } else {
                  return 0;
                  _sv0t11 = 0;
                }
                _sv0t9 = _sv0t11;
              }
              _sv0t8 = _sv0t9;
            } else {
              return 0;
              _sv0t8 = 0;
            }
            _sv0t7 = _sv0t8;
          }
          _sv0t5 = _sv0t7;
        }
        _sv0t4 = _sv0t5;
      } else {
        int _sv0t12;
        if ((t1 == 22)) {
          int _sv0t14 = is_atom(t2);
          int _sv0t13;
          if ((_sv0t14 == 0)) {
            return 0;
            _sv0t13 = 0;
          } else {
            int _sv0t15;
            if ((t3 == 15)) {
              return 1;
              _sv0t15 = 0;
            } else {
              int _sv0t16;
              if ((t3 == 22)) {
                int _sv0t18 = is_atom(t4);
                int _sv0t17;
                if ((_sv0t18 == 0)) {
                  return 0;
                  _sv0t17 = 0;
                } else {
                  int _sv0t19;
                  if ((t5 == 15)) {
                    return 1;
                    _sv0t19 = 0;
                  } else {
                    return 0;
                    _sv0t19 = 0;
                  }
                  _sv0t17 = _sv0t19;
                }
                _sv0t16 = _sv0t17;
              } else {
                int _sv0t20;
                if ((t3 == 20)) {
                  int _sv0t22 = is_atom(t4);
                  int _sv0t21;
                  if ((_sv0t22 == 0)) {
                    return 0;
                    _sv0t21 = 0;
                  } else {
                    int _sv0t23;
                    if ((t5 == 15)) {
                      return 1;
                      _sv0t23 = 0;
                    } else {
                      return 0;
                      _sv0t23 = 0;
                    }
                    _sv0t21 = _sv0t23;
                  }
                  _sv0t20 = _sv0t21;
                } else {
                  return 0;
                  _sv0t20 = 0;
                }
                _sv0t16 = _sv0t20;
              }
              _sv0t15 = _sv0t16;
            }
            _sv0t13 = _sv0t15;
          }
          _sv0t12 = _sv0t13;
        } else {
          return 0;
          _sv0t12 = 0;
        }
        _sv0t4 = _sv0t12;
      }
      _sv0t3 = _sv0t4;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int pratt_rhs_ok_with_semi(int t0, int t1, int t2, int t3, int t4, int t5) {
  int _sv0t2 = is_atom(t0);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    int _sv0t3 = pratt_from_atom(t0, t1, t2, t3, t4, t5);
    return _sv0t3;
    _sv0t1 = 0;
  } else {
    int _sv0t4;
    if ((t0 == 21)) {
      int _sv0t6 = is_atom(t1);
      int _sv0t5;
      if ((_sv0t6 == 0)) {
        return 0;
        _sv0t5 = 0;
      } else {
        int _sv0t7 = pratt_from_atom(t1, t2, t3, t4, t5, 0);
        return _sv0t7;
        _sv0t5 = 0;
      }
      _sv0t4 = _sv0t5;
    } else {
      return 0;
      _sv0t4 = 0;
    }
    _sv0t1 = _sv0t4;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = tok_int_lit();
  int _sv0t1 = tok_semi();
  int _sv0t2 = pratt_rhs_ok_with_semi(_sv0t0, _sv0t1, 0, 0, 0, 0);
  int e0 = (_sv0t2 - 1);
  int _sv0t3 = tok_ident();
  int _sv0t4 = tok_semi();
  int _sv0t5 = pratt_rhs_ok_with_semi(_sv0t3, _sv0t4, 0, 0, 0, 0);
  int e1 = (_sv0t5 - 1);
  int _sv0t6 = tok_int_lit();
  int _sv0t7 = tok_plus();
  int _sv0t8 = tok_int_lit();
  int _sv0t9 = tok_semi();
  int _sv0t10 = pratt_rhs_ok_with_semi(_sv0t6, _sv0t7, _sv0t8, _sv0t9, 0, 0);
  int e2 = (_sv0t10 - 1);
  int _sv0t11 = tok_int_lit();
  int _sv0t12 = tok_star();
  int _sv0t13 = tok_int_lit();
  int _sv0t14 = tok_semi();
  int _sv0t15 = pratt_rhs_ok_with_semi(_sv0t11, _sv0t12, _sv0t13, _sv0t14, 0, 0);
  int e3 = (_sv0t15 - 1);
  int _sv0t16 = tok_int_lit();
  int _sv0t17 = tok_plus();
  int _sv0t18 = tok_int_lit();
  int _sv0t19 = tok_star();
  int _sv0t20 = tok_int_lit();
  int _sv0t21 = tok_semi();
  int _sv0t22 = pratt_rhs_ok_with_semi(_sv0t16, _sv0t17, _sv0t18, _sv0t19, _sv0t20, _sv0t21);
  int e4 = (_sv0t22 - 1);
  int _sv0t23 = tok_int_lit();
  int _sv0t24 = tok_star();
  int _sv0t25 = tok_int_lit();
  int _sv0t26 = tok_plus();
  int _sv0t27 = tok_int_lit();
  int _sv0t28 = tok_semi();
  int _sv0t29 = pratt_rhs_ok_with_semi(_sv0t23, _sv0t24, _sv0t25, _sv0t26, _sv0t27, _sv0t28);
  int e5 = (_sv0t29 - 1);
  int _sv0t30 = tok_int_lit();
  int _sv0t31 = tok_star();
  int _sv0t32 = tok_int_lit();
  int _sv0t33 = tok_star();
  int _sv0t34 = tok_int_lit();
  int _sv0t35 = tok_semi();
  int _sv0t36 = pratt_rhs_ok_with_semi(_sv0t30, _sv0t31, _sv0t32, _sv0t33, _sv0t34, _sv0t35);
  int e6 = (_sv0t36 - 1);
  int _sv0t37 = tok_int_lit();
  int _sv0t38 = tok_plus();
  int _sv0t39 = tok_int_lit();
  int _sv0t40 = tok_star();
  int _sv0t41 = tok_int_lit();
  int _sv0t42 = tok_ident();
  int _sv0t43 = pratt_rhs_ok_with_semi(_sv0t37, _sv0t38, _sv0t39, _sv0t40, _sv0t41, _sv0t42);
  int e7 = _sv0t43;
  int _sv0t44 = tok_int_lit();
  int _sv0t45 = tok_star();
  int _sv0t46 = tok_int_lit();
  int _sv0t47 = tok_star();
  int _sv0t48 = tok_int_lit();
  int _sv0t49 = pratt_rhs_ok_with_semi(_sv0t44, _sv0t45, _sv0t46, _sv0t47, _sv0t48, 0);
  int e8 = _sv0t49;
  int _sv0t50 = tok_minus();
  int _sv0t51 = tok_int_lit();
  int _sv0t52 = tok_semi();
  int _sv0t53 = pratt_rhs_ok_with_semi(_sv0t50, _sv0t51, _sv0t52, 0, 0, 0);
  int e9 = (_sv0t53 - 1);
  int _sv0t54 = tok_minus();
  int _sv0t55 = tok_int_lit();
  int _sv0t56 = tok_plus();
  int _sv0t57 = tok_int_lit();
  int _sv0t58 = tok_semi();
  int _sv0t59 = pratt_rhs_ok_with_semi(_sv0t54, _sv0t55, _sv0t56, _sv0t57, _sv0t58, 0);
  int e10 = (_sv0t59 - 1);
  int _sv0t60 = tok_minus();
  int _sv0t61 = tok_int_lit();
  int _sv0t62 = tok_star();
  int _sv0t63 = tok_int_lit();
  int _sv0t64 = tok_semi();
  int _sv0t65 = pratt_rhs_ok_with_semi(_sv0t60, _sv0t61, _sv0t62, _sv0t63, _sv0t64, 0);
  int e11 = (_sv0t65 - 1);
  int _sv0t66 = tok_minus();
  int _sv0t67 = tok_ident();
  int _sv0t68 = tok_semi();
  int _sv0t69 = pratt_rhs_ok_with_semi(_sv0t66, _sv0t67, _sv0t68, 0, 0, 0);
  int e12 = (_sv0t69 - 1);
  int _sv0t70 = tok_minus();
  int _sv0t71 = tok_int_lit();
  int _sv0t72 = tok_plus();
  int _sv0t73 = tok_int_lit();
  int _sv0t74 = tok_star();
  int _sv0t75 = tok_int_lit();
  int _sv0t76 = pratt_rhs_ok_with_semi(_sv0t70, _sv0t71, _sv0t72, _sv0t73, _sv0t74, _sv0t75);
  int e13 = _sv0t76;
  int _sv0t77 = tok_minus();
  int _sv0t78 = tok_minus();
  int _sv0t79 = tok_int_lit();
  int _sv0t80 = tok_semi();
  int _sv0t81 = pratt_rhs_ok_with_semi(_sv0t77, _sv0t78, _sv0t79, _sv0t80, 0, 0);
  int e14 = _sv0t81;
  int _sv0t82 = (e0 + e1);
  int _sv0t83 = (_sv0t82 + e2);
  int _sv0t84 = (_sv0t83 + e3);
  int _sv0t85 = (_sv0t84 + e4);
  int _sv0t86 = (_sv0t85 + e5);
  int _sv0t87 = (_sv0t86 + e6);
  int _sv0t88 = (_sv0t87 + e7);
  int _sv0t89 = (_sv0t88 + e8);
  int _sv0t90 = (_sv0t89 + e9);
  int _sv0t91 = (_sv0t90 + e10);
  int _sv0t92 = (_sv0t91 + e11);
  int _sv0t93 = (_sv0t92 + e12);
  int _sv0t94 = (_sv0t93 + e13);
  int _sv0t95 = (_sv0t94 + e14);
  return _sv0t95;
}

