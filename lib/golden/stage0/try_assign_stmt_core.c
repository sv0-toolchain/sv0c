#include "sv0_runtime.h"

static int tok_ident(void);
static int tok_star(void);
static int tok_assign_eq(void);
static int tok_pluseq(void);
static int tok_minuseq(void);
static int tok_gtgteq(void);
static int tok_rhs_atom(void);
static int tok_semi(void);
static int tok_lhs_dot(void);
static int tok_op_eqeq(void);
static int tok_binop_plus(void);
static int rhs_stub_ok_with_semi(int t0, int t1, int t2, int t3);
static int assign_binop_ast_tag(int t);
static int assign_op_follows_lhs(int t);
static int try_assign_linear_4(int t0, int t1, int t2, int t3);
static int try_assign_linear_5_deref(int t0, int t1, int t2, int t3, int t4);
static int try_assign_linear_6_field(int t0, int t1, int t2, int t3, int t4, int t5);
static int try_assign_id_op_rhs_stub(int op, int t0, int t1, int t2, int t3);
static int try_assign_deref_op_rhs_stub(int t_star, int t_id, int op, int t0, int t1, int t2, int t3);

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

static int tok_minuseq(void) {
  return 212;
}

static int tok_gtgteq(void) {
  return 220;
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

static int tok_binop_plus(void) {
  return 20;
}

static int rhs_stub_ok_with_semi(int t0, int t1, int t2, int t3) {
  int _sv0t0;
  int _sv0t1;
  if ((t0 == 40)) {
    int _sv0t2;
    if ((t1 == 15)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t1 == 20)) {
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
        _sv0t3 = _sv0t4;
      } else {
        return 0;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    int _sv0t6;
    if ((t0 == 73)) {
      int _sv0t7;
      if ((t1 == 15)) {
        return 1;
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
    _sv0t1 = _sv0t6;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int assign_binop_ast_tag(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 211)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 212)) {
      return 2;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 213)) {
        return 3;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((t == 214)) {
          return 4;
          _sv0t4 = 0;
        } else {
          int _sv0t5;
          if ((t == 215)) {
            return 5;
            _sv0t5 = 0;
          } else {
            int _sv0t6;
            if ((t == 216)) {
              return 6;
              _sv0t6 = 0;
            } else {
              int _sv0t7;
              if ((t == 217)) {
                return 7;
                _sv0t7 = 0;
              } else {
                int _sv0t8;
                if ((t == 218)) {
                  return 8;
                  _sv0t8 = 0;
                } else {
                  int _sv0t9;
                  if ((t == 219)) {
                    return 9;
                    _sv0t9 = 0;
                  } else {
                    int _sv0t10;
                    if ((t == 220)) {
                      return 10;
                      _sv0t10 = 0;
                    } else {
                      return 0;
                      _sv0t10 = 0;
                    }
                    _sv0t9 = _sv0t10;
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

static int assign_op_follows_lhs(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 19)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t3 = assign_binop_ast_tag(t);
    int _sv0t2;
    if ((_sv0t3 > 0)) {
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
    int _sv0t3 = assign_op_follows_lhs(t1);
    int _sv0t2;
    if ((_sv0t3 == 1)) {
      int _sv0t4 = rhs_stub_ok_with_semi(t2, t3, 0, 0);
      return _sv0t4;
      _sv0t2 = 0;
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
      int _sv0t4 = assign_op_follows_lhs(t2);
      int _sv0t3;
      if ((_sv0t4 == 1)) {
        int _sv0t5 = rhs_stub_ok_with_semi(t3, t4, 0, 0);
        return _sv0t5;
        _sv0t3 = 0;
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

static int try_assign_linear_6_field(int t0, int t1, int t2, int t3, int t4, int t5) {
  int _sv0t0;
  int _sv0t1;
  if ((t0 == 73)) {
    int _sv0t2;
    if ((t1 == 18)) {
      int _sv0t3;
      if ((t2 == 73)) {
        int _sv0t5 = assign_op_follows_lhs(t3);
        int _sv0t4;
        if ((_sv0t5 == 1)) {
          int _sv0t6 = rhs_stub_ok_with_semi(t4, t5, 0, 0);
          return _sv0t6;
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

static int try_assign_id_op_rhs_stub(int op, int t0, int t1, int t2, int t3) {
  int _sv0t2 = assign_op_follows_lhs(op);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    int _sv0t3 = rhs_stub_ok_with_semi(t0, t1, t2, t3);
    return _sv0t3;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int try_assign_deref_op_rhs_stub(int t_star, int t_id, int op, int t0, int t1, int t2, int t3) {
  int _sv0t0;
  int _sv0t1;
  if ((t_star == 22)) {
    int _sv0t2;
    if ((t_id == 73)) {
      int _sv0t4 = assign_op_follows_lhs(op);
      int _sv0t3;
      if ((_sv0t4 == 1)) {
        int _sv0t5 = rhs_stub_ok_with_semi(t0, t1, t2, t3);
        return _sv0t5;
        _sv0t3 = 0;
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
  int _sv0t1 = assign_op_follows_lhs(_sv0t0);
  int e0 = (_sv0t1 - 1);
  int _sv0t2 = tok_pluseq();
  int _sv0t3 = assign_op_follows_lhs(_sv0t2);
  int e1 = (_sv0t3 - 1);
  int _sv0t4 = tok_op_eqeq();
  int _sv0t5 = assign_op_follows_lhs(_sv0t4);
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
  int _sv0t50 = tok_ident();
  int _sv0t51 = tok_minuseq();
  int _sv0t52 = tok_rhs_atom();
  int _sv0t53 = tok_semi();
  int _sv0t54 = try_assign_linear_4(_sv0t50, _sv0t51, _sv0t52, _sv0t53);
  int e11 = (_sv0t54 - 1);
  int _sv0t55 = tok_ident();
  int _sv0t56 = tok_gtgteq();
  int _sv0t57 = tok_rhs_atom();
  int _sv0t58 = tok_semi();
  int _sv0t59 = try_assign_linear_4(_sv0t55, _sv0t56, _sv0t57, _sv0t58);
  int e12 = (_sv0t59 - 1);
  int _sv0t60 = tok_ident();
  int _sv0t61 = tok_lhs_dot();
  int _sv0t62 = tok_ident();
  int _sv0t63 = tok_minuseq();
  int _sv0t64 = tok_rhs_atom();
  int _sv0t65 = tok_semi();
  int _sv0t66 = try_assign_linear_6_field(_sv0t60, _sv0t61, _sv0t62, _sv0t63, _sv0t64, _sv0t65);
  int e13 = (_sv0t66 - 1);
  int _sv0t67 = tok_assign_eq();
  int _sv0t68 = tok_rhs_atom();
  int _sv0t69 = tok_binop_plus();
  int _sv0t70 = tok_rhs_atom();
  int _sv0t71 = tok_semi();
  int _sv0t72 = try_assign_id_op_rhs_stub(_sv0t67, _sv0t68, _sv0t69, _sv0t70, _sv0t71);
  int e14 = (_sv0t72 - 1);
  int _sv0t73 = tok_pluseq();
  int _sv0t74 = tok_rhs_atom();
  int _sv0t75 = tok_binop_plus();
  int _sv0t76 = tok_rhs_atom();
  int _sv0t77 = tok_semi();
  int _sv0t78 = try_assign_id_op_rhs_stub(_sv0t73, _sv0t74, _sv0t75, _sv0t76, _sv0t77);
  int e15 = (_sv0t78 - 1);
  int _sv0t79 = tok_assign_eq();
  int _sv0t80 = tok_rhs_atom();
  int _sv0t81 = tok_binop_plus();
  int _sv0t82 = tok_semi();
  int _sv0t83 = try_assign_id_op_rhs_stub(_sv0t79, _sv0t80, _sv0t81, _sv0t82, 0);
  int e16 = _sv0t83;
  int _sv0t84 = tok_ident();
  int _sv0t85 = tok_assign_eq();
  int _sv0t86 = tok_ident();
  int _sv0t87 = tok_semi();
  int _sv0t88 = try_assign_linear_4(_sv0t84, _sv0t85, _sv0t86, _sv0t87);
  int e17 = (_sv0t88 - 1);
  int _sv0t89 = tok_star();
  int _sv0t90 = tok_ident();
  int _sv0t91 = tok_assign_eq();
  int _sv0t92 = tok_rhs_atom();
  int _sv0t93 = tok_binop_plus();
  int _sv0t94 = tok_rhs_atom();
  int _sv0t95 = tok_semi();
  int _sv0t96 = try_assign_deref_op_rhs_stub(_sv0t89, _sv0t90, _sv0t91, _sv0t92, _sv0t93, _sv0t94, _sv0t95);
  int e18 = (_sv0t96 - 1);
  int _sv0t97 = tok_star();
  int _sv0t98 = tok_ident();
  int _sv0t99 = tok_assign_eq();
  int _sv0t100 = tok_rhs_atom();
  int _sv0t101 = tok_binop_plus();
  int _sv0t102 = tok_semi();
  int _sv0t103 = try_assign_deref_op_rhs_stub(_sv0t97, _sv0t98, _sv0t99, _sv0t100, _sv0t101, _sv0t102, 0);
  int e19 = _sv0t103;
  int _sv0t104 = tok_ident();
  int _sv0t105 = tok_ident();
  int _sv0t106 = tok_assign_eq();
  int _sv0t107 = tok_rhs_atom();
  int _sv0t108 = tok_binop_plus();
  int _sv0t109 = tok_rhs_atom();
  int _sv0t110 = tok_semi();
  int _sv0t111 = try_assign_deref_op_rhs_stub(_sv0t104, _sv0t105, _sv0t106, _sv0t107, _sv0t108, _sv0t109, _sv0t110);
  int e20 = _sv0t111;
  int _sv0t112 = (e0 + e1);
  int _sv0t113 = (_sv0t112 + e2);
  int _sv0t114 = (_sv0t113 + e3);
  int _sv0t115 = (_sv0t114 + e4);
  int _sv0t116 = (_sv0t115 + e5);
  int _sv0t117 = (_sv0t116 + e6);
  int _sv0t118 = (_sv0t117 + e7);
  int _sv0t119 = (_sv0t118 + e8);
  int _sv0t120 = (_sv0t119 + e9);
  int _sv0t121 = (_sv0t120 + e10);
  int _sv0t122 = (_sv0t121 + e11);
  int _sv0t123 = (_sv0t122 + e12);
  int _sv0t124 = (_sv0t123 + e13);
  int _sv0t125 = (_sv0t124 + e14);
  int _sv0t126 = (_sv0t125 + e15);
  int _sv0t127 = (_sv0t126 + e16);
  int _sv0t128 = (_sv0t127 + e17);
  int _sv0t129 = (_sv0t128 + e18);
  int _sv0t130 = (_sv0t129 + e19);
  int _sv0t131 = (_sv0t130 + e20);
  return _sv0t131;
}

