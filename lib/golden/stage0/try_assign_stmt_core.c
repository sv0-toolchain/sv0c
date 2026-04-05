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
static int tok_lbracket(void);
static int tok_rbracket(void);
static int tok_op_eqeq(void);
static int tok_binop_plus(void);
static int tok_binop_star(void);
static int rhs_stub_ok_with_semi(int t0, int t1, int t2, int t3);
static int pratt_is_atom(int t);
static int pratt_rhs_ok_with_semi(int t0, int t1, int t2, int t3, int t4, int t5);
static int assign_binop_ast_tag(int t);
static int assign_op_follows_lhs(int t);
static int try_assign_linear_4(int t0, int t1, int t2, int t3);
static int try_assign_linear_5_deref(int t0, int t1, int t2, int t3, int t4);
static int try_assign_linear_6_field(int t0, int t1, int t2, int t3, int t4, int t5);
static int try_assign_id_op_rhs_stub(int op, int t0, int t1, int t2, int t3);
static int try_assign_id_op_pratt_rhs_stub(int op, int t0, int t1, int t2, int t3, int t4, int t5);
static int try_assign_deref_op_rhs_stub(int t_star, int t_id, int op, int t0, int t1, int t2, int t3);
static int try_assign_field_op_rhs_stub(int t_base, int t_dot, int t_field, int op, int r0, int r1, int r2, int r3);
static int try_assign_index_op_rhs_stub(int t_id, int t_lbr, int t_idx, int t_rbr, int op, int r0, int r1, int r2, int r3);

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

static int tok_lbracket(void) {
  return 16;
}

static int tok_rbracket(void) {
  return 17;
}

static int tok_op_eqeq(void) {
  return 24;
}

static int tok_binop_plus(void) {
  return 20;
}

static int tok_binop_star(void) {
  return 22;
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

static int pratt_is_atom(int t) {
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

static int pratt_rhs_ok_with_semi(int t0, int t1, int t2, int t3, int t4, int t5) {
  int _sv0t2 = pratt_is_atom(t0);
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
        int _sv0t6 = pratt_is_atom(t2);
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
              int _sv0t10 = pratt_is_atom(t4);
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
          int _sv0t14 = pratt_is_atom(t2);
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
                int _sv0t18 = pratt_is_atom(t4);
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
                  int _sv0t22 = pratt_is_atom(t4);
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

static int try_assign_id_op_pratt_rhs_stub(int op, int t0, int t1, int t2, int t3, int t4, int t5) {
  int _sv0t2 = assign_op_follows_lhs(op);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    int _sv0t3 = pratt_rhs_ok_with_semi(t0, t1, t2, t3, t4, t5);
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

static int try_assign_field_op_rhs_stub(int t_base, int t_dot, int t_field, int op, int r0, int r1, int r2, int r3) {
  int _sv0t0;
  int _sv0t1;
  if ((t_base == 73)) {
    int _sv0t2;
    if ((t_dot == 18)) {
      int _sv0t3;
      if ((t_field == 73)) {
        int _sv0t5 = assign_op_follows_lhs(op);
        int _sv0t4;
        if ((_sv0t5 == 1)) {
          int _sv0t6 = rhs_stub_ok_with_semi(r0, r1, r2, r3);
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

static int try_assign_index_op_rhs_stub(int t_id, int t_lbr, int t_idx, int t_rbr, int op, int r0, int r1, int r2, int r3) {
  int _sv0t0;
  int _sv0t1;
  if ((t_id == 73)) {
    int _sv0t2;
    if ((t_lbr == 16)) {
      int _sv0t3;
      if ((t_idx == 40)) {
        int _sv0t4;
        if ((t_rbr == 17)) {
          int _sv0t6 = assign_op_follows_lhs(op);
          int _sv0t5;
          if ((_sv0t6 == 1)) {
            int _sv0t7 = rhs_stub_ok_with_semi(r0, r1, r2, r3);
            return _sv0t7;
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
  int _sv0t112 = tok_ident();
  int _sv0t113 = tok_lhs_dot();
  int _sv0t114 = tok_ident();
  int _sv0t115 = tok_assign_eq();
  int _sv0t116 = tok_rhs_atom();
  int _sv0t117 = tok_binop_plus();
  int _sv0t118 = tok_rhs_atom();
  int _sv0t119 = tok_semi();
  int _sv0t120 = try_assign_field_op_rhs_stub(_sv0t112, _sv0t113, _sv0t114, _sv0t115, _sv0t116, _sv0t117, _sv0t118, _sv0t119);
  int e21 = (_sv0t120 - 1);
  int _sv0t121 = tok_ident();
  int _sv0t122 = tok_lhs_dot();
  int _sv0t123 = tok_ident();
  int _sv0t124 = tok_assign_eq();
  int _sv0t125 = tok_rhs_atom();
  int _sv0t126 = tok_binop_plus();
  int _sv0t127 = tok_semi();
  int _sv0t128 = try_assign_field_op_rhs_stub(_sv0t121, _sv0t122, _sv0t123, _sv0t124, _sv0t125, _sv0t126, _sv0t127, 0);
  int e22 = _sv0t128;
  int _sv0t129 = tok_ident();
  int _sv0t130 = tok_assign_eq();
  int _sv0t131 = tok_ident();
  int _sv0t132 = tok_assign_eq();
  int _sv0t133 = tok_rhs_atom();
  int _sv0t134 = tok_binop_plus();
  int _sv0t135 = tok_rhs_atom();
  int _sv0t136 = tok_semi();
  int _sv0t137 = try_assign_field_op_rhs_stub(_sv0t129, _sv0t130, _sv0t131, _sv0t132, _sv0t133, _sv0t134, _sv0t135, _sv0t136);
  int e23 = _sv0t137;
  int _sv0t138 = tok_ident();
  int _sv0t139 = tok_lbracket();
  int _sv0t140 = tok_rhs_atom();
  int _sv0t141 = tok_rbracket();
  int _sv0t142 = tok_assign_eq();
  int _sv0t143 = tok_rhs_atom();
  int _sv0t144 = tok_binop_plus();
  int _sv0t145 = tok_rhs_atom();
  int _sv0t146 = tok_semi();
  int _sv0t147 = try_assign_index_op_rhs_stub(_sv0t138, _sv0t139, _sv0t140, _sv0t141, _sv0t142, _sv0t143, _sv0t144, _sv0t145, _sv0t146);
  int e24 = (_sv0t147 - 1);
  int _sv0t148 = tok_ident();
  int _sv0t149 = tok_lbracket();
  int _sv0t150 = tok_rhs_atom();
  int _sv0t151 = tok_rbracket();
  int _sv0t152 = tok_assign_eq();
  int _sv0t153 = tok_rhs_atom();
  int _sv0t154 = tok_binop_plus();
  int _sv0t155 = tok_semi();
  int _sv0t156 = try_assign_index_op_rhs_stub(_sv0t148, _sv0t149, _sv0t150, _sv0t151, _sv0t152, _sv0t153, _sv0t154, _sv0t155, 0);
  int e25 = _sv0t156;
  int _sv0t157 = tok_ident();
  int _sv0t158 = tok_lhs_dot();
  int _sv0t159 = tok_rhs_atom();
  int _sv0t160 = tok_rbracket();
  int _sv0t161 = tok_assign_eq();
  int _sv0t162 = tok_rhs_atom();
  int _sv0t163 = tok_binop_plus();
  int _sv0t164 = tok_rhs_atom();
  int _sv0t165 = tok_semi();
  int _sv0t166 = try_assign_index_op_rhs_stub(_sv0t157, _sv0t158, _sv0t159, _sv0t160, _sv0t161, _sv0t162, _sv0t163, _sv0t164, _sv0t165);
  int e26 = _sv0t166;
  int _sv0t167 = tok_assign_eq();
  int _sv0t168 = tok_rhs_atom();
  int _sv0t169 = tok_binop_plus();
  int _sv0t170 = tok_rhs_atom();
  int _sv0t171 = tok_binop_star();
  int _sv0t172 = tok_rhs_atom();
  int _sv0t173 = tok_semi();
  int _sv0t174 = try_assign_id_op_pratt_rhs_stub(_sv0t167, _sv0t168, _sv0t169, _sv0t170, _sv0t171, _sv0t172, _sv0t173);
  int e27 = (_sv0t174 - 1);
  int _sv0t175 = tok_pluseq();
  int _sv0t176 = tok_rhs_atom();
  int _sv0t177 = tok_binop_star();
  int _sv0t178 = tok_rhs_atom();
  int _sv0t179 = tok_binop_plus();
  int _sv0t180 = tok_rhs_atom();
  int _sv0t181 = tok_semi();
  int _sv0t182 = try_assign_id_op_pratt_rhs_stub(_sv0t175, _sv0t176, _sv0t177, _sv0t178, _sv0t179, _sv0t180, _sv0t181);
  int e28 = (_sv0t182 - 1);
  int _sv0t183 = tok_assign_eq();
  int _sv0t184 = tok_rhs_atom();
  int _sv0t185 = tok_binop_plus();
  int _sv0t186 = tok_rhs_atom();
  int _sv0t187 = tok_binop_star();
  int _sv0t188 = tok_rhs_atom();
  int _sv0t189 = try_assign_id_op_pratt_rhs_stub(_sv0t183, _sv0t184, _sv0t185, _sv0t186, _sv0t187, _sv0t188, 0);
  int e29 = _sv0t189;
  int _sv0t190 = (e0 + e1);
  int _sv0t191 = (_sv0t190 + e2);
  int _sv0t192 = (_sv0t191 + e3);
  int _sv0t193 = (_sv0t192 + e4);
  int _sv0t194 = (_sv0t193 + e5);
  int _sv0t195 = (_sv0t194 + e6);
  int _sv0t196 = (_sv0t195 + e7);
  int _sv0t197 = (_sv0t196 + e8);
  int _sv0t198 = (_sv0t197 + e9);
  int _sv0t199 = (_sv0t198 + e10);
  int _sv0t200 = (_sv0t199 + e11);
  int _sv0t201 = (_sv0t200 + e12);
  int _sv0t202 = (_sv0t201 + e13);
  int _sv0t203 = (_sv0t202 + e14);
  int _sv0t204 = (_sv0t203 + e15);
  int _sv0t205 = (_sv0t204 + e16);
  int _sv0t206 = (_sv0t205 + e17);
  int _sv0t207 = (_sv0t206 + e18);
  int _sv0t208 = (_sv0t207 + e19);
  int _sv0t209 = (_sv0t208 + e20);
  int _sv0t210 = (_sv0t209 + e21);
  int _sv0t211 = (_sv0t210 + e22);
  int _sv0t212 = (_sv0t211 + e23);
  int _sv0t213 = (_sv0t212 + e24);
  int _sv0t214 = (_sv0t213 + e25);
  int _sv0t215 = (_sv0t214 + e26);
  int _sv0t216 = (_sv0t215 + e27);
  int _sv0t217 = (_sv0t216 + e28);
  int _sv0t218 = (_sv0t217 + e29);
  return _sv0t218;
}

