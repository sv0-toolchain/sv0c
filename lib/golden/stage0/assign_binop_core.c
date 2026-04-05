#include "sv0_runtime.h"

static int tok_pluseq(void);
static int tok_minuseq(void);
static int tok_stareq(void);
static int tok_slasheq(void);
static int tok_percenteq(void);
static int tok_ampeq(void);
static int tok_pipeeq(void);
static int tok_careteq(void);
static int tok_ltlteq(void);
static int tok_gtgteq(void);
static int assign_binop_ast_tag(int t);
static int is_assign_binop_tok(int t);

static int tok_pluseq(void) {
  return 211;
}

static int tok_minuseq(void) {
  return 212;
}

static int tok_stareq(void) {
  return 213;
}

static int tok_slasheq(void) {
  return 214;
}

static int tok_percenteq(void) {
  return 215;
}

static int tok_ampeq(void) {
  return 216;
}

static int tok_pipeeq(void) {
  return 217;
}

static int tok_careteq(void) {
  return 218;
}

static int tok_ltlteq(void) {
  return 219;
}

static int tok_gtgteq(void) {
  return 220;
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

static int is_assign_binop_tok(int t) {
  int _sv0t2 = assign_binop_ast_tag(t);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 > 0)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = tok_pluseq();
  int _sv0t1 = assign_binop_ast_tag(_sv0t0);
  int e0 = (_sv0t1 - 1);
  int _sv0t2 = tok_minuseq();
  int _sv0t3 = assign_binop_ast_tag(_sv0t2);
  int e1 = (_sv0t3 - 2);
  int _sv0t4 = tok_stareq();
  int _sv0t5 = assign_binop_ast_tag(_sv0t4);
  int e2 = (_sv0t5 - 3);
  int _sv0t6 = tok_slasheq();
  int _sv0t7 = assign_binop_ast_tag(_sv0t6);
  int e3 = (_sv0t7 - 4);
  int _sv0t8 = tok_percenteq();
  int _sv0t9 = assign_binop_ast_tag(_sv0t8);
  int e4 = (_sv0t9 - 5);
  int _sv0t10 = tok_ampeq();
  int _sv0t11 = assign_binop_ast_tag(_sv0t10);
  int e5 = (_sv0t11 - 6);
  int _sv0t12 = tok_pipeeq();
  int _sv0t13 = assign_binop_ast_tag(_sv0t12);
  int e6 = (_sv0t13 - 7);
  int _sv0t14 = tok_careteq();
  int _sv0t15 = assign_binop_ast_tag(_sv0t14);
  int e7 = (_sv0t15 - 8);
  int _sv0t16 = tok_ltlteq();
  int _sv0t17 = assign_binop_ast_tag(_sv0t16);
  int e8 = (_sv0t17 - 9);
  int _sv0t18 = tok_gtgteq();
  int _sv0t19 = assign_binop_ast_tag(_sv0t18);
  int e9 = (_sv0t19 - 10);
  int _sv0t20 = assign_binop_ast_tag(19);
  int e10 = _sv0t20;
  int _sv0t21 = assign_binop_ast_tag(24);
  int e11 = _sv0t21;
  int _sv0t22 = assign_binop_ast_tag(221);
  int e12 = _sv0t22;
  int _sv0t23 = tok_pluseq();
  int _sv0t24 = is_assign_binop_tok(_sv0t23);
  int e13 = (_sv0t24 - 1);
  int _sv0t25 = is_assign_binop_tok(99);
  int e14 = _sv0t25;
  int _sv0t26 = (e0 + e1);
  int _sv0t27 = (_sv0t26 + e2);
  int _sv0t28 = (_sv0t27 + e3);
  int _sv0t29 = (_sv0t28 + e4);
  int _sv0t30 = (_sv0t29 + e5);
  int _sv0t31 = (_sv0t30 + e6);
  int _sv0t32 = (_sv0t31 + e7);
  int _sv0t33 = (_sv0t32 + e8);
  int _sv0t34 = (_sv0t33 + e9);
  int _sv0t35 = (_sv0t34 + e10);
  int _sv0t36 = (_sv0t35 + e11);
  int _sv0t37 = (_sv0t36 + e12);
  int _sv0t38 = (_sv0t37 + e13);
  int _sv0t39 = (_sv0t38 + e14);
  return _sv0t39;
}

