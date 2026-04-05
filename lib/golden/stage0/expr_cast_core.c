#include "sv0_runtime.h"

static int tok_int_lit(void);
static int tok_ident(void);
static int tok_as_kw(void);
static int tok_semi(void);
static int tok_plus(void);
static int cast_base_atom(int t);
static int cast_ty_primitive_ident(int t);
static int parse_cast_stub_ok_with_semi6(int t0, int t1, int t2, int t3, int t4, int t5);

static int tok_int_lit(void) {
  return 40;
}

static int tok_ident(void) {
  return 73;
}

static int tok_as_kw(void) {
  return 61;
}

static int tok_semi(void) {
  return 15;
}

static int tok_plus(void) {
  return 20;
}

static int cast_base_atom(int t) {
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

static int cast_ty_primitive_ident(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 73)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int parse_cast_stub_ok_with_semi6(int t0, int t1, int t2, int t3, int t4, int t5) {
  int _sv0t2 = cast_base_atom(t0);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    int _sv0t3;
    if ((t1 == 15)) {
      return 1;
      _sv0t3 = 0;
    } else {
      int _sv0t4;
      if ((t1 == 61)) {
        int _sv0t6 = cast_ty_primitive_ident(t2);
        int _sv0t5;
        if ((_sv0t6 == 1)) {
          int _sv0t7;
          if ((t3 == 15)) {
            return 1;
            _sv0t7 = 0;
          } else {
            int _sv0t8;
            if ((t3 == 61)) {
              int _sv0t10 = cast_ty_primitive_ident(t4);
              int _sv0t9;
              if ((_sv0t10 == 1)) {
                int _sv0t11;
                if ((t5 == 15)) {
                  return 1;
                  _sv0t11 = 0;
                } else {
                  return 0;
                  _sv0t11 = 0;
                }
                _sv0t9 = _sv0t11;
              } else {
                return 0;
                _sv0t9 = 0;
              }
              _sv0t8 = _sv0t9;
            } else {
              return 0;
              _sv0t8 = 0;
            }
            _sv0t7 = _sv0t8;
          }
          _sv0t5 = _sv0t7;
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
  int _sv0t0 = tok_int_lit();
  int _sv0t1 = tok_semi();
  int _sv0t2 = parse_cast_stub_ok_with_semi6(_sv0t0, _sv0t1, 0, 0, 0, 0);
  int e0 = (_sv0t2 - 1);
  int _sv0t3 = tok_ident();
  int _sv0t4 = tok_semi();
  int _sv0t5 = parse_cast_stub_ok_with_semi6(_sv0t3, _sv0t4, 0, 0, 0, 0);
  int e1 = (_sv0t5 - 1);
  int _sv0t6 = tok_int_lit();
  int _sv0t7 = tok_as_kw();
  int _sv0t8 = tok_ident();
  int _sv0t9 = tok_semi();
  int _sv0t10 = parse_cast_stub_ok_with_semi6(_sv0t6, _sv0t7, _sv0t8, _sv0t9, 0, 0);
  int e2 = (_sv0t10 - 1);
  int _sv0t11 = tok_ident();
  int _sv0t12 = tok_as_kw();
  int _sv0t13 = tok_ident();
  int _sv0t14 = tok_semi();
  int _sv0t15 = parse_cast_stub_ok_with_semi6(_sv0t11, _sv0t12, _sv0t13, _sv0t14, 0, 0);
  int e3 = (_sv0t15 - 1);
  int _sv0t16 = tok_int_lit();
  int _sv0t17 = tok_as_kw();
  int _sv0t18 = tok_ident();
  int _sv0t19 = tok_as_kw();
  int _sv0t20 = tok_ident();
  int _sv0t21 = tok_semi();
  int _sv0t22 = parse_cast_stub_ok_with_semi6(_sv0t16, _sv0t17, _sv0t18, _sv0t19, _sv0t20, _sv0t21);
  int e4 = (_sv0t22 - 1);
  int _sv0t23 = tok_int_lit();
  int _sv0t24 = tok_plus();
  int _sv0t25 = tok_int_lit();
  int _sv0t26 = tok_semi();
  int _sv0t27 = parse_cast_stub_ok_with_semi6(_sv0t23, _sv0t24, _sv0t25, _sv0t26, 0, 0);
  int bad_mul = _sv0t27;
  int _sv0t28 = tok_int_lit();
  int _sv0t29 = tok_as_kw();
  int _sv0t30 = tok_int_lit();
  int _sv0t31 = tok_semi();
  int _sv0t32 = parse_cast_stub_ok_with_semi6(_sv0t28, _sv0t29, _sv0t30, _sv0t31, 0, 0);
  int bad_ty_lit = _sv0t32;
  int _sv0t33 = tok_int_lit();
  int _sv0t34 = tok_as_kw();
  int _sv0t35 = tok_ident();
  int _sv0t36 = tok_plus();
  int _sv0t37 = parse_cast_stub_ok_with_semi6(_sv0t33, _sv0t34, _sv0t35, _sv0t36, 0, 0);
  int bad_no_semi = _sv0t37;
  int _sv0t38 = tok_as_kw();
  int _sv0t39 = tok_int_lit();
  int _sv0t40 = tok_ident();
  int _sv0t41 = tok_semi();
  int _sv0t42 = parse_cast_stub_ok_with_semi6(_sv0t38, _sv0t39, _sv0t40, _sv0t41, 0, 0);
  int bad_as_first = _sv0t42;
  int _sv0t43 = tok_int_lit();
  int _sv0t44 = tok_as_kw();
  int _sv0t45 = tok_ident();
  int _sv0t46 = tok_as_kw();
  int _sv0t47 = tok_int_lit();
  int _sv0t48 = tok_semi();
  int _sv0t49 = parse_cast_stub_ok_with_semi6(_sv0t43, _sv0t44, _sv0t45, _sv0t46, _sv0t47, _sv0t48);
  int bad_second_ty_lit = _sv0t49;
  int _sv0t50 = tok_int_lit();
  int _sv0t51 = tok_as_kw();
  int _sv0t52 = tok_ident();
  int _sv0t53 = tok_as_kw();
  int _sv0t54 = tok_ident();
  int _sv0t55 = tok_as_kw();
  int _sv0t56 = parse_cast_stub_ok_with_semi6(_sv0t50, _sv0t51, _sv0t52, _sv0t53, _sv0t54, _sv0t55);
  int bad_third_as = _sv0t56;
  int _sv0t57 = (bad_mul + bad_ty_lit);
  int _sv0t58 = (_sv0t57 + bad_no_semi);
  int _sv0t59 = (_sv0t58 + bad_as_first);
  int _sv0t60 = (_sv0t59 + bad_second_ty_lit);
  int e5 = (_sv0t60 + bad_third_as);
  int _sv0t61 = (e0 + e1);
  int _sv0t62 = (_sv0t61 + e2);
  int _sv0t63 = (_sv0t62 + e3);
  int _sv0t64 = (_sv0t63 + e4);
  int _sv0t65 = (_sv0t64 + e5);
  return _sv0t65;
}

