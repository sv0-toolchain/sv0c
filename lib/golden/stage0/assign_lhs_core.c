#include "sv0_runtime.h"

static int tok_lhs_dot(void);
static int tok_assign_eq(void);
static int tok_pluseq(void);
static int tok_star(void);
static int tok_ident(void);
static int tok_int_lit(void);
static int tok_lbracket(void);
static int tok_rbracket(void);
static int tok_semi(void);
static int try_lhs_starts(int t);
static int assign_binop_tag(int t);
static int is_assign_follow_tok(int t);
static int is_assign_tok(int t);
static int assign_target_depth_if_assign_eq(int t0, int t1, int t2, int t3);
static int assign_target_depth_star_star_ident_eq(int t0, int t1, int t2, int t3);
static int assign_target_depth_index_with_eq(int t0, int t1, int t2, int t3, int t4);
static int try_lhs_ident_tail_is_assign_follow(int tail);

static int tok_lhs_dot(void) {
  return 18;
}

static int tok_assign_eq(void) {
  return 19;
}

static int tok_pluseq(void) {
  return 211;
}

static int tok_star(void) {
  return 22;
}

static int tok_ident(void) {
  return 73;
}

static int tok_int_lit(void) {
  return 40;
}

static int tok_lbracket(void) {
  return 16;
}

static int tok_rbracket(void) {
  return 17;
}

static int tok_semi(void) {
  return 15;
}

static int try_lhs_starts(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 22)) {
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

static int assign_binop_tag(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 211)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int is_assign_follow_tok(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 19)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t3 = assign_binop_tag(t);
    int _sv0t2;
    if ((_sv0t3 == 1)) {
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

static int is_assign_tok(int t) {
  int _sv0t0 = is_assign_follow_tok(t);
  return _sv0t0;
}

static int assign_target_depth_if_assign_eq(int t0, int t1, int t2, int t3) {
  int _sv0t0;
  int _sv0t1;
  if ((t0 == 73)) {
    int _sv0t2;
    if ((t1 == 19)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t1 == 18)) {
        int _sv0t4;
        if ((t2 == 73)) {
          int _sv0t5;
          if ((t3 == 19)) {
            return 2;
            _sv0t5 = 0;
          } else {
            return 0;
            _sv0t5 = 0;
          }
          _sv0t4 = _sv0t5;
        } else {
          int _sv0t6;
          if ((t2 == 40)) {
            int _sv0t7;
            if ((t3 == 19)) {
              return 2;
              _sv0t7 = 0;
            } else {
              return 0;
              _sv0t7 = 0;
            }
            _sv0t6 = _sv0t7;
          } else {
            return 0;
            _sv0t6 = 0;
          }
          _sv0t4 = _sv0t6;
        }
        _sv0t3 = _sv0t4;
      } else {
        return 0;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    int _sv0t8;
    if ((t0 == 22)) {
      int _sv0t9;
      if ((t1 == 73)) {
        int _sv0t10;
        if ((t2 == 19)) {
          return 2;
          _sv0t10 = 0;
        } else {
          return 0;
          _sv0t10 = 0;
        }
        _sv0t9 = _sv0t10;
      } else {
        return 0;
        _sv0t9 = 0;
      }
      _sv0t8 = _sv0t9;
    } else {
      return 0;
      _sv0t8 = 0;
    }
    _sv0t1 = _sv0t8;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int assign_target_depth_star_star_ident_eq(int t0, int t1, int t2, int t3) {
  int _sv0t0;
  int _sv0t1;
  if ((t0 == 22)) {
    int _sv0t2;
    if ((t1 == 22)) {
      int _sv0t3;
      if ((t2 == 73)) {
        int _sv0t4;
        if ((t3 == 19)) {
          return 3;
          _sv0t4 = 0;
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

static int assign_target_depth_index_with_eq(int t0, int t1, int t2, int t3, int t4) {
  int _sv0t0;
  int _sv0t1;
  if ((t0 == 73)) {
    int _sv0t2;
    if ((t1 == 16)) {
      int _sv0t3;
      if ((t2 == 40)) {
        int _sv0t4;
        if ((t3 == 17)) {
          int _sv0t5;
          if ((t4 == 19)) {
            return 2;
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

static int try_lhs_ident_tail_is_assign_follow(int tail) {
  int _sv0t2 = try_lhs_starts(73);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    int _sv0t4 = is_assign_follow_tok(tail);
    int _sv0t3;
    if ((_sv0t4 == 1)) {
      return 1;
      _sv0t3 = 0;
    } else {
      return 0;
      _sv0t3 = 0;
    }
    _sv0t1 = _sv0t3;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = tok_star();
  int _sv0t1 = try_lhs_starts(_sv0t0);
  int e0 = (_sv0t1 - 1);
  int _sv0t2 = tok_ident();
  int _sv0t3 = try_lhs_starts(_sv0t2);
  int e1 = (_sv0t3 - 1);
  int _sv0t4 = try_lhs_starts(99);
  int e2 = _sv0t4;
  int _sv0t5 = tok_assign_eq();
  int _sv0t6 = is_assign_follow_tok(_sv0t5);
  int e3 = (_sv0t6 - 1);
  int _sv0t7 = tok_pluseq();
  int _sv0t8 = is_assign_follow_tok(_sv0t7);
  int e4 = (_sv0t8 - 1);
  int _sv0t9 = is_assign_follow_tok(24);
  int e5 = _sv0t9;
  int _sv0t10 = tok_pluseq();
  int _sv0t11 = assign_binop_tag(_sv0t10);
  int e6 = (_sv0t11 - 1);
  int _sv0t12 = tok_assign_eq();
  int _sv0t13 = assign_binop_tag(_sv0t12);
  int e7 = _sv0t13;
  int _sv0t14 = tok_assign_eq();
  int _sv0t15 = is_assign_tok(_sv0t14);
  int e8 = (_sv0t15 - 1);
  int _sv0t16 = tok_pluseq();
  int _sv0t17 = is_assign_tok(_sv0t16);
  int e9 = (_sv0t17 - 1);
  int _sv0t18 = tok_ident();
  int _sv0t19 = tok_assign_eq();
  int _sv0t20 = assign_target_depth_if_assign_eq(_sv0t18, _sv0t19, 0, 0);
  int e10 = (_sv0t20 - 1);
  int _sv0t21 = tok_star();
  int _sv0t22 = tok_ident();
  int _sv0t23 = tok_assign_eq();
  int _sv0t24 = assign_target_depth_if_assign_eq(_sv0t21, _sv0t22, _sv0t23, 0);
  int e11 = (_sv0t24 - 2);
  int _sv0t25 = tok_ident();
  int _sv0t26 = tok_lhs_dot();
  int _sv0t27 = tok_ident();
  int _sv0t28 = tok_assign_eq();
  int _sv0t29 = assign_target_depth_if_assign_eq(_sv0t25, _sv0t26, _sv0t27, _sv0t28);
  int e12 = (_sv0t29 - 2);
  int _sv0t30 = tok_ident();
  int _sv0t31 = tok_lhs_dot();
  int _sv0t32 = tok_int_lit();
  int _sv0t33 = tok_assign_eq();
  int _sv0t34 = assign_target_depth_if_assign_eq(_sv0t30, _sv0t31, _sv0t32, _sv0t33);
  int e13 = (_sv0t34 - 2);
  int _sv0t35 = tok_ident();
  int _sv0t36 = tok_lbracket();
  int _sv0t37 = tok_int_lit();
  int _sv0t38 = tok_rbracket();
  int _sv0t39 = tok_assign_eq();
  int _sv0t40 = assign_target_depth_index_with_eq(_sv0t35, _sv0t36, _sv0t37, _sv0t38, _sv0t39);
  int e14 = (_sv0t40 - 2);
  int _sv0t41 = tok_star();
  int _sv0t42 = tok_star();
  int _sv0t43 = tok_ident();
  int _sv0t44 = tok_assign_eq();
  int _sv0t45 = assign_target_depth_star_star_ident_eq(_sv0t41, _sv0t42, _sv0t43, _sv0t44);
  int e15 = (_sv0t45 - 3);
  int _sv0t46 = tok_ident();
  int _sv0t47 = tok_semi();
  int _sv0t48 = assign_target_depth_if_assign_eq(_sv0t46, _sv0t47, 0, 0);
  int e16 = _sv0t48;
  int _sv0t49 = tok_assign_eq();
  int _sv0t50 = try_lhs_ident_tail_is_assign_follow(_sv0t49);
  int e17 = (_sv0t50 - 1);
  int _sv0t51 = tok_semi();
  int _sv0t52 = try_lhs_ident_tail_is_assign_follow(_sv0t51);
  int e18 = _sv0t52;
  int _sv0t53 = (e0 + e1);
  int _sv0t54 = (_sv0t53 + e2);
  int _sv0t55 = (_sv0t54 + e3);
  int _sv0t56 = (_sv0t55 + e4);
  int _sv0t57 = (_sv0t56 + e5);
  int _sv0t58 = (_sv0t57 + e6);
  int _sv0t59 = (_sv0t58 + e7);
  int _sv0t60 = (_sv0t59 + e8);
  int _sv0t61 = (_sv0t60 + e9);
  int _sv0t62 = (_sv0t61 + e10);
  int _sv0t63 = (_sv0t62 + e11);
  int _sv0t64 = (_sv0t63 + e12);
  int _sv0t65 = (_sv0t64 + e13);
  int _sv0t66 = (_sv0t65 + e14);
  int _sv0t67 = (_sv0t66 + e15);
  int _sv0t68 = (_sv0t67 + e16);
  int _sv0t69 = (_sv0t68 + e17);
  int _sv0t70 = (_sv0t69 + e18);
  return _sv0t70;
}

