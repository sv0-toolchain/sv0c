#include "sv0_runtime.h"

static int c_op_width_single(void);
static int c_op_width_double(void);
static int binop_tag_add(void);
static int binop_tag_sub(void);
static int binop_tag_mul(void);
static int binop_tag_div(void);
static int binop_tag_rem(void);
static int binop_tag_bitand(void);
static int binop_tag_bitor(void);
static int binop_tag_bitxor(void);
static int binop_tag_shl(void);
static int binop_tag_shr(void);
static int binop_tag_and(void);
static int binop_tag_or(void);
static int binop_tag_eq(void);
static int binop_tag_neq(void);
static int binop_tag_lt(void);
static int binop_tag_gt(void);
static int binop_tag_leq(void);
static int binop_tag_geq(void);
static int ast_binop_to_c_width_class(int binop_tag);

static int c_op_width_single(void) {
  return 1;
}

static int c_op_width_double(void) {
  return 2;
}

static int binop_tag_add(void) {
  return 1;
}

static int binop_tag_sub(void) {
  return 2;
}

static int binop_tag_mul(void) {
  return 3;
}

static int binop_tag_div(void) {
  return 4;
}

static int binop_tag_rem(void) {
  return 5;
}

static int binop_tag_bitand(void) {
  return 6;
}

static int binop_tag_bitor(void) {
  return 7;
}

static int binop_tag_bitxor(void) {
  return 8;
}

static int binop_tag_shl(void) {
  return 9;
}

static int binop_tag_shr(void) {
  return 10;
}

static int binop_tag_and(void) {
  return 11;
}

static int binop_tag_or(void) {
  return 12;
}

static int binop_tag_eq(void) {
  return 13;
}

static int binop_tag_neq(void) {
  return 14;
}

static int binop_tag_lt(void) {
  return 15;
}

static int binop_tag_gt(void) {
  return 16;
}

static int binop_tag_leq(void) {
  return 17;
}

static int binop_tag_geq(void) {
  return 18;
}

static int ast_binop_to_c_width_class(int binop_tag) {
  int _sv0t0;
  int _sv0t1;
  if ((binop_tag < 1)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((binop_tag > 18)) {
      return 0;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((binop_tag > 8)) {
        int _sv0t4;
        if ((binop_tag > 10)) {
          int _sv0t5;
          if ((binop_tag > 14)) {
            int _sv0t6;
            if ((binop_tag > 16)) {
              int _sv0t7 = c_op_width_double();
              return _sv0t7;
              _sv0t6 = 0;
            } else {
              int _sv0t8 = c_op_width_single();
              return _sv0t8;
              _sv0t6 = 0;
            }
            _sv0t5 = _sv0t6;
          } else {
            int _sv0t9 = c_op_width_double();
            return _sv0t9;
            _sv0t5 = 0;
          }
          _sv0t4 = _sv0t5;
        } else {
          int _sv0t10 = c_op_width_double();
          return _sv0t10;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        int _sv0t11 = c_op_width_single();
        return _sv0t11;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = binop_tag_add();
  int _sv0t1 = ast_binop_to_c_width_class(_sv0t0);
  int _sv0t2 = c_op_width_single();
  if ((_sv0t1 != _sv0t2)) {
    return 1;
  } else {
  }
  int _sv0t3 = binop_tag_sub();
  int _sv0t4 = ast_binop_to_c_width_class(_sv0t3);
  int _sv0t5 = c_op_width_single();
  if ((_sv0t4 != _sv0t5)) {
    return 1;
  } else {
  }
  int _sv0t6 = binop_tag_mul();
  int _sv0t7 = ast_binop_to_c_width_class(_sv0t6);
  int _sv0t8 = c_op_width_single();
  if ((_sv0t7 != _sv0t8)) {
    return 1;
  } else {
  }
  int _sv0t9 = binop_tag_div();
  int _sv0t10 = ast_binop_to_c_width_class(_sv0t9);
  int _sv0t11 = c_op_width_single();
  if ((_sv0t10 != _sv0t11)) {
    return 1;
  } else {
  }
  int _sv0t12 = binop_tag_rem();
  int _sv0t13 = ast_binop_to_c_width_class(_sv0t12);
  int _sv0t14 = c_op_width_single();
  if ((_sv0t13 != _sv0t14)) {
    return 1;
  } else {
  }
  int _sv0t15 = binop_tag_bitand();
  int _sv0t16 = ast_binop_to_c_width_class(_sv0t15);
  int _sv0t17 = c_op_width_single();
  if ((_sv0t16 != _sv0t17)) {
    return 1;
  } else {
  }
  int _sv0t18 = binop_tag_bitor();
  int _sv0t19 = ast_binop_to_c_width_class(_sv0t18);
  int _sv0t20 = c_op_width_single();
  if ((_sv0t19 != _sv0t20)) {
    return 1;
  } else {
  }
  int _sv0t21 = binop_tag_bitxor();
  int _sv0t22 = ast_binop_to_c_width_class(_sv0t21);
  int _sv0t23 = c_op_width_single();
  if ((_sv0t22 != _sv0t23)) {
    return 1;
  } else {
  }
  int _sv0t24 = binop_tag_shl();
  int _sv0t25 = ast_binop_to_c_width_class(_sv0t24);
  int _sv0t26 = c_op_width_double();
  if ((_sv0t25 != _sv0t26)) {
    return 1;
  } else {
  }
  int _sv0t27 = binop_tag_shr();
  int _sv0t28 = ast_binop_to_c_width_class(_sv0t27);
  int _sv0t29 = c_op_width_double();
  if ((_sv0t28 != _sv0t29)) {
    return 1;
  } else {
  }
  int _sv0t30 = binop_tag_and();
  int _sv0t31 = ast_binop_to_c_width_class(_sv0t30);
  int _sv0t32 = c_op_width_double();
  if ((_sv0t31 != _sv0t32)) {
    return 1;
  } else {
  }
  int _sv0t33 = binop_tag_or();
  int _sv0t34 = ast_binop_to_c_width_class(_sv0t33);
  int _sv0t35 = c_op_width_double();
  if ((_sv0t34 != _sv0t35)) {
    return 1;
  } else {
  }
  int _sv0t36 = binop_tag_eq();
  int _sv0t37 = ast_binop_to_c_width_class(_sv0t36);
  int _sv0t38 = c_op_width_double();
  if ((_sv0t37 != _sv0t38)) {
    return 1;
  } else {
  }
  int _sv0t39 = binop_tag_neq();
  int _sv0t40 = ast_binop_to_c_width_class(_sv0t39);
  int _sv0t41 = c_op_width_double();
  if ((_sv0t40 != _sv0t41)) {
    return 1;
  } else {
  }
  int _sv0t42 = binop_tag_lt();
  int _sv0t43 = ast_binop_to_c_width_class(_sv0t42);
  int _sv0t44 = c_op_width_single();
  if ((_sv0t43 != _sv0t44)) {
    return 1;
  } else {
  }
  int _sv0t45 = binop_tag_gt();
  int _sv0t46 = ast_binop_to_c_width_class(_sv0t45);
  int _sv0t47 = c_op_width_single();
  if ((_sv0t46 != _sv0t47)) {
    return 1;
  } else {
  }
  int _sv0t48 = binop_tag_leq();
  int _sv0t49 = ast_binop_to_c_width_class(_sv0t48);
  int _sv0t50 = c_op_width_double();
  if ((_sv0t49 != _sv0t50)) {
    return 1;
  } else {
  }
  int _sv0t51 = binop_tag_geq();
  int _sv0t52 = ast_binop_to_c_width_class(_sv0t51);
  int _sv0t53 = c_op_width_double();
  if ((_sv0t52 != _sv0t53)) {
    return 1;
  } else {
  }
  int _sv0t54 = ast_binop_to_c_width_class(0);
  if ((_sv0t54 != 0)) {
    return 1;
  } else {
  }
  int _sv0t55 = ast_binop_to_c_width_class(19);
  if ((_sv0t55 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

