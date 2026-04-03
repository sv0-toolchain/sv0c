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
static int is_operator_tag(int t);

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
  int _sv0t20 = (a0 + a1);
  int _sv0t21 = (_sv0t20 + a2);
  int _sv0t22 = (_sv0t21 + a3);
  int _sv0t23 = (_sv0t22 + a4);
  int _sv0t24 = (_sv0t23 + a5);
  int _sv0t25 = (_sv0t24 + a6);
  int _sv0t26 = (_sv0t25 + a7);
  int _sv0t27 = (_sv0t26 + a8);
  int _sv0t28 = (_sv0t27 + a9);
  int _sv0t29 = (_sv0t28 + a10);
  return _sv0t29;
}

