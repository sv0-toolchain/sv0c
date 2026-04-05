#include "sv0_runtime.h"

static int tok_ident(void);
static int tok_star(void);
static int tok_assign_eq(void);
static int tok_pluseq(void);
static int tok_rhs_atom(void);
static int tok_semi(void);
static int tok_lhs_dot(void);
static int tok_op_eqeq(void);
static int assign_op_is_eq_or_pluseq(int t);
static int try_assign_linear_4(int t0, int t1, int t2, int t3);
static int try_assign_linear_5_deref(int t0, int t1, int t2, int t3, int t4);
static int try_assign_linear_6_field(int t0, int t1, int t2, int t3, int t4, int t5);

static int tok_ident(void) {
  return 73;
}

static int tok_star(void) {
  return 22;
}

static int tok_assign_eq(void) {
  return 19;
}

static int tok_pluseq(void) {
  return 211;
}

static int tok_rhs_atom(void) {
  return 40;
}

static int tok_semi(void) {
  return 15;
}

static int tok_lhs_dot(void) {
  return 18;
}

static int tok_op_eqeq(void) {
  return 24;
}

static int assign_op_is_eq_or_pluseq(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 19)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 211)) {
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

static int try_assign_linear_4(int t0, int t1, int t2, int t3) {
  int _sv0t0;
  int _sv0t1;
  if ((t0 == 73)) {
    int _sv0t3 = assign_op_is_eq_or_pluseq(t1);
    int _sv0t2;
    if ((_sv0t3 == 1)) {
      int _sv0t4;
      if ((t2 == 40)) {
        int _sv0t5;
        if ((t3 == 15)) {
          return 1;
          _sv0t5 = 0;
        } else {
          return 0;
          _sv0t5 = 0;
        }
        _sv0t4 = _sv0t5;
      } else {
        return 0;
        _sv0t4 = 0;
      }
      _sv0t2 = _sv0t4;
    } else {
      return 0;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int try_assign_linear_5_deref(int t0, int t1, int t2, int t3, int t4) {
  int _sv0t0;
  int _sv0t1;
  if ((t0 == 22)) {
    int _sv0t2;
    if ((t1 == 73)) {
      int _sv0t3;
      if ((t2 == 19)) {
        int _sv0t4;
        if ((t3 == 40)) {
          int _sv0t5;
          if ((t4 == 15)) {
            return 1;
            _sv0t5 = 0;
          } else {
            return 0;
            _sv0t5 = 0;
          }
          _sv0t4 = _sv0t5;
        } else {
          return 0;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        int _sv0t6;
        if ((t2 == 211)) {
          int _sv0t7;
          if ((t3 == 40)) {
            int _sv0t8;
            if ((t4 == 15)) {
              return 1;
              _sv0t8 = 0;
            } else {
              return 0;
              _sv0t8 = 0;
            }
            _sv0t7 = _sv0t8;
          } else {
            return 0;
            _sv0t7 = 0;
          }
          _sv0t6 = _sv0t7;
        } else {
          return 0;
          _sv0t6 = 0;
        }
        _sv0t3 = _sv0t6;
      }
      _sv0t2 = _sv0t3;
    } else {
      return 0;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int try_assign_linear_6_field(int t0, int t1, int t2, int t3, int t4, int t5) {
  int _sv0t0;
  int _sv0t1;
  if ((t0 == 73)) {
    int _sv0t2;
    if ((t1 == 18)) {
      int _sv0t3;
      if ((t2 == 73)) {
        int _sv0t4;
        if ((t3 == 19)) {
          int _sv0t5;
          if ((t4 == 40)) {
            int _sv0t6;
            if ((t5 == 15)) {
              return 1;
              _sv0t6 = 0;
            } else {
              return 0;
              _sv0t6 = 0;
            }
            _sv0t5 = _sv0t6;
          } else {
            return 0;
            _sv0t5 = 0;
          }
          _sv0t4 = _sv0t5;
        } else {
          return 0;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        return 0;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    } else {
      return 0;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = tok_assign_eq();
  int _sv0t1 = assign_op_is_eq_or_pluseq(_sv0t0);
  int e0 = (_sv0t1 - 1);
  int _sv0t2 = tok_pluseq();
  int _sv0t3 = assign_op_is_eq_or_pluseq(_sv0t2);
  int e1 = (_sv0t3 - 1);
  int _sv0t4 = tok_op_eqeq();
  int _sv0t5 = assign_op_is_eq_or_pluseq(_sv0t4);
  int e2 = _sv0t5;
  int _sv0t6 = tok_ident();
  int _sv0t7 = tok_assign_eq();
  int _sv0t8 = tok_rhs_atom();
  int _sv0t9 = tok_semi();
  int _sv0t10 = try_assign_linear_4(_sv0t6, _sv0t7, _sv0t8, _sv0t9);
  int e3 = (_sv0t10 - 1);
  int _sv0t11 = tok_ident();
  int _sv0t12 = tok_pluseq();
  int _sv0t13 = tok_rhs_atom();
  int _sv0t14 = tok_semi();
  int _sv0t15 = try_assign_linear_4(_sv0t11, _sv0t12, _sv0t13, _sv0t14);
  int e4 = (_sv0t15 - 1);
  int _sv0t16 = tok_star();
  int _sv0t17 = tok_ident();
  int _sv0t18 = tok_assign_eq();
  int _sv0t19 = tok_rhs_atom();
  int _sv0t20 = tok_semi();
  int _sv0t21 = try_assign_linear_5_deref(_sv0t16, _sv0t17, _sv0t18, _sv0t19, _sv0t20);
  int e5 = (_sv0t21 - 1);
  int _sv0t22 = tok_star();
  int _sv0t23 = tok_ident();
  int _sv0t24 = tok_pluseq();
  int _sv0t25 = tok_rhs_atom();
  int _sv0t26 = tok_semi();
  int _sv0t27 = try_assign_linear_5_deref(_sv0t22, _sv0t23, _sv0t24, _sv0t25, _sv0t26);
  int e6 = (_sv0t27 - 1);
  int _sv0t28 = tok_ident();
  int _sv0t29 = tok_lhs_dot();
  int _sv0t30 = tok_ident();
  int _sv0t31 = tok_assign_eq();
  int _sv0t32 = tok_rhs_atom();
  int _sv0t33 = tok_semi();
  int _sv0t34 = try_assign_linear_6_field(_sv0t28, _sv0t29, _sv0t30, _sv0t31, _sv0t32, _sv0t33);
  int e7 = (_sv0t34 - 1);
  int _sv0t35 = tok_ident();
  int _sv0t36 = tok_assign_eq();
  int _sv0t37 = tok_rhs_atom();
  int _sv0t38 = tok_ident();
  int _sv0t39 = try_assign_linear_4(_sv0t35, _sv0t36, _sv0t37, _sv0t38);
  int e8 = _sv0t39;
  int _sv0t40 = tok_ident();
  int _sv0t41 = tok_op_eqeq();
  int _sv0t42 = tok_rhs_atom();
  int _sv0t43 = tok_semi();
  int _sv0t44 = try_assign_linear_4(_sv0t40, _sv0t41, _sv0t42, _sv0t43);
  int e9 = _sv0t44;
  int _sv0t45 = tok_ident();
  int _sv0t46 = tok_assign_eq();
  int _sv0t47 = tok_semi();
  int _sv0t48 = tok_semi();
  int _sv0t49 = try_assign_linear_4(_sv0t45, _sv0t46, _sv0t47, _sv0t48);
  int e10 = _sv0t49;
  int _sv0t50 = (e0 + e1);
  int _sv0t51 = (_sv0t50 + e2);
  int _sv0t52 = (_sv0t51 + e3);
  int _sv0t53 = (_sv0t52 + e4);
  int _sv0t54 = (_sv0t53 + e5);
  int _sv0t55 = (_sv0t54 + e6);
  int _sv0t56 = (_sv0t55 + e7);
  int _sv0t57 = (_sv0t56 + e8);
  int _sv0t58 = (_sv0t57 + e9);
  int _sv0t59 = (_sv0t58 + e10);
  return _sv0t59;
}

