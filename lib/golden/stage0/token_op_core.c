#include "sv0_runtime.h"

static int tag_op_plus(void);
static int tag_op_minus(void);
static int tag_op_star(void);
static int tag_op_slash(void);
static int tag_op_eq(void);
static int tag_op_ne(void);
static int tag_op_arrow(void);
static int tag_op_fat_arrow(void);
static int tag_op_coloncolon(void);
static int tag_op_pluseq(void);
static int tag_op_minuseq(void);
static int tag_op_stareq(void);
static int tag_op_slasheq(void);
static int tag_op_percenteq(void);
static int tag_op_ampeq(void);
static int tag_op_pipeeq(void);
static int tag_op_careteq(void);
static int tag_op_ltlteq(void);
static int tag_op_gtgteq(void);
static int is_operator_tag(int t);
static int is_compound_assign_tag(int t);

static int tag_op_plus(void) {
  return 20;
}

static int tag_op_minus(void) {
  return 21;
}

static int tag_op_star(void) {
  return 22;
}

static int tag_op_slash(void) {
  return 23;
}

static int tag_op_eq(void) {
  return 24;
}

static int tag_op_ne(void) {
  return 25;
}

static int tag_op_arrow(void) {
  return 26;
}

static int tag_op_fat_arrow(void) {
  return 27;
}

static int tag_op_coloncolon(void) {
  return 28;
}

static int tag_op_pluseq(void) {
  return 211;
}

static int tag_op_minuseq(void) {
  return 212;
}

static int tag_op_stareq(void) {
  return 213;
}

static int tag_op_slasheq(void) {
  return 214;
}

static int tag_op_percenteq(void) {
  return 215;
}

static int tag_op_ampeq(void) {
  return 216;
}

static int tag_op_pipeeq(void) {
  return 217;
}

static int tag_op_careteq(void) {
  return 218;
}

static int tag_op_ltlteq(void) {
  return 219;
}

static int tag_op_gtgteq(void) {
  return 220;
}

static int is_operator_tag(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 20)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 21)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 22)) {
        return 1;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((t == 23)) {
          return 1;
          _sv0t4 = 0;
        } else {
          int _sv0t5;
          if ((t == 24)) {
            return 1;
            _sv0t5 = 0;
          } else {
            int _sv0t6;
            if ((t == 25)) {
              return 1;
              _sv0t6 = 0;
            } else {
              int _sv0t7;
              if ((t == 26)) {
                return 1;
                _sv0t7 = 0;
              } else {
                int _sv0t8;
                if ((t == 27)) {
                  return 1;
                  _sv0t8 = 0;
                } else {
                  int _sv0t9;
                  if ((t == 28)) {
                    return 1;
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

static int is_compound_assign_tag(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 211)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 212)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 213)) {
        return 1;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((t == 214)) {
          return 1;
          _sv0t4 = 0;
        } else {
          int _sv0t5;
          if ((t == 215)) {
            return 1;
            _sv0t5 = 0;
          } else {
            int _sv0t6;
            if ((t == 216)) {
              return 1;
              _sv0t6 = 0;
            } else {
              int _sv0t7;
              if ((t == 217)) {
                return 1;
                _sv0t7 = 0;
              } else {
                int _sv0t8;
                if ((t == 218)) {
                  return 1;
                  _sv0t8 = 0;
                } else {
                  int _sv0t9;
                  if ((t == 219)) {
                    return 1;
                    _sv0t9 = 0;
                  } else {
                    int _sv0t10;
                    if ((t == 220)) {
                      return 1;
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

int main(void) {
  int _sv0t0 = tag_op_plus();
  int _sv0t1 = is_operator_tag(_sv0t0);
  int a0 = (_sv0t1 - 1);
  int _sv0t2 = tag_op_minus();
  int _sv0t3 = is_operator_tag(_sv0t2);
  int a1 = (_sv0t3 - 1);
  int _sv0t4 = tag_op_star();
  int _sv0t5 = is_operator_tag(_sv0t4);
  int a2 = (_sv0t5 - 1);
  int _sv0t6 = tag_op_slash();
  int _sv0t7 = is_operator_tag(_sv0t6);
  int a3 = (_sv0t7 - 1);
  int _sv0t8 = tag_op_eq();
  int _sv0t9 = is_operator_tag(_sv0t8);
  int a4 = (_sv0t9 - 1);
  int _sv0t10 = tag_op_ne();
  int _sv0t11 = is_operator_tag(_sv0t10);
  int a5 = (_sv0t11 - 1);
  int _sv0t12 = tag_op_arrow();
  int _sv0t13 = is_operator_tag(_sv0t12);
  int a6 = (_sv0t13 - 1);
  int _sv0t14 = tag_op_fat_arrow();
  int _sv0t15 = is_operator_tag(_sv0t14);
  int a7 = (_sv0t15 - 1);
  int _sv0t16 = tag_op_coloncolon();
  int _sv0t17 = is_operator_tag(_sv0t16);
  int a8 = (_sv0t17 - 1);
  int _sv0t18 = is_operator_tag(3);
  int a9 = _sv0t18;
  int _sv0t19 = is_operator_tag(15);
  int a10 = _sv0t19;
  int _sv0t20 = tag_op_pluseq();
  int _sv0t21 = is_compound_assign_tag(_sv0t20);
  int b0 = (_sv0t21 - 1);
  int _sv0t22 = tag_op_minuseq();
  int _sv0t23 = is_compound_assign_tag(_sv0t22);
  int b1 = (_sv0t23 - 1);
  int _sv0t24 = tag_op_stareq();
  int _sv0t25 = is_compound_assign_tag(_sv0t24);
  int b2 = (_sv0t25 - 1);
  int _sv0t26 = tag_op_slasheq();
  int _sv0t27 = is_compound_assign_tag(_sv0t26);
  int b3 = (_sv0t27 - 1);
  int _sv0t28 = tag_op_percenteq();
  int _sv0t29 = is_compound_assign_tag(_sv0t28);
  int b4 = (_sv0t29 - 1);
  int _sv0t30 = tag_op_ampeq();
  int _sv0t31 = is_compound_assign_tag(_sv0t30);
  int b5 = (_sv0t31 - 1);
  int _sv0t32 = tag_op_pipeeq();
  int _sv0t33 = is_compound_assign_tag(_sv0t32);
  int b6 = (_sv0t33 - 1);
  int _sv0t34 = tag_op_careteq();
  int _sv0t35 = is_compound_assign_tag(_sv0t34);
  int b7 = (_sv0t35 - 1);
  int _sv0t36 = tag_op_ltlteq();
  int _sv0t37 = is_compound_assign_tag(_sv0t36);
  int b8 = (_sv0t37 - 1);
  int _sv0t38 = tag_op_gtgteq();
  int _sv0t39 = is_compound_assign_tag(_sv0t38);
  int b9 = (_sv0t39 - 1);
  int _sv0t40 = is_compound_assign_tag(19);
  int b10 = _sv0t40;
  int _sv0t41 = is_compound_assign_tag(24);
  int b11 = _sv0t41;
  int _sv0t42 = is_compound_assign_tag(99);
  int b12 = _sv0t42;
  int _sv0t43 = (a0 + a1);
  int _sv0t44 = (_sv0t43 + a2);
  int _sv0t45 = (_sv0t44 + a3);
  int _sv0t46 = (_sv0t45 + a4);
  int _sv0t47 = (_sv0t46 + a5);
  int _sv0t48 = (_sv0t47 + a6);
  int _sv0t49 = (_sv0t48 + a7);
  int _sv0t50 = (_sv0t49 + a8);
  int _sv0t51 = (_sv0t50 + a9);
  int _sv0t52 = (_sv0t51 + a10);
  int _sv0t53 = (_sv0t52 + b0);
  int _sv0t54 = (_sv0t53 + b1);
  int _sv0t55 = (_sv0t54 + b2);
  int _sv0t56 = (_sv0t55 + b3);
  int _sv0t57 = (_sv0t56 + b4);
  int _sv0t58 = (_sv0t57 + b5);
  int _sv0t59 = (_sv0t58 + b6);
  int _sv0t60 = (_sv0t59 + b7);
  int _sv0t61 = (_sv0t60 + b8);
  int _sv0t62 = (_sv0t61 + b9);
  int _sv0t63 = (_sv0t62 + b10);
  int _sv0t64 = (_sv0t63 + b11);
  int _sv0t65 = (_sv0t64 + b12);
  return _sv0t65;
}

