#include "sv0_runtime.h"

static int tok_int_lit(void);
static int tok_ident(void);
static int tok_plus(void);
static int tok_semi(void);
static int rhs_stub_ok_with_semi(int t0, int t1, int t2, int t3);
static int rhs_stub_atom_count(int t0, int t1, int t2, int t3);

static int tok_int_lit(void) {
  return 40;
}

static int tok_ident(void) {
  return 73;
}

static int tok_plus(void) {
  return 20;
}

static int tok_semi(void) {
  return 15;
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

static int rhs_stub_atom_count(int t0, int t1, int t2, int t3) {
  int _sv0t2 = rhs_stub_ok_with_semi(t0, t1, t2, t3);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    int _sv0t3;
    if ((t1 == 15)) {
      return 1;
      _sv0t3 = 0;
    } else {
      return 3;
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
  int _sv0t0 = tok_int_lit();
  int _sv0t1 = tok_semi();
  int _sv0t2 = rhs_stub_ok_with_semi(_sv0t0, _sv0t1, 0, 0);
  int e0 = (_sv0t2 - 1);
  int _sv0t3 = tok_ident();
  int _sv0t4 = tok_semi();
  int _sv0t5 = rhs_stub_ok_with_semi(_sv0t3, _sv0t4, 0, 0);
  int e1 = (_sv0t5 - 1);
  int _sv0t6 = tok_int_lit();
  int _sv0t7 = tok_plus();
  int _sv0t8 = tok_int_lit();
  int _sv0t9 = tok_semi();
  int _sv0t10 = rhs_stub_ok_with_semi(_sv0t6, _sv0t7, _sv0t8, _sv0t9);
  int e2 = (_sv0t10 - 1);
  int _sv0t11 = tok_int_lit();
  int _sv0t12 = tok_plus();
  int _sv0t13 = tok_semi();
  int _sv0t14 = rhs_stub_ok_with_semi(_sv0t11, _sv0t12, _sv0t13, 0);
  int e3 = _sv0t14;
  int _sv0t15 = tok_int_lit();
  int _sv0t16 = tok_int_lit();
  int _sv0t17 = tok_semi();
  int _sv0t18 = rhs_stub_ok_with_semi(_sv0t15, 24, _sv0t16, _sv0t17);
  int e4 = _sv0t18;
  int _sv0t19 = tok_int_lit();
  int _sv0t20 = tok_semi();
  int _sv0t21 = rhs_stub_atom_count(_sv0t19, _sv0t20, 0, 0);
  int e5 = (_sv0t21 - 1);
  int _sv0t22 = tok_int_lit();
  int _sv0t23 = tok_plus();
  int _sv0t24 = tok_int_lit();
  int _sv0t25 = tok_semi();
  int _sv0t26 = rhs_stub_atom_count(_sv0t22, _sv0t23, _sv0t24, _sv0t25);
  int e6 = (_sv0t26 - 3);
  int _sv0t27 = (e0 + e1);
  int _sv0t28 = (_sv0t27 + e2);
  int _sv0t29 = (_sv0t28 + e3);
  int _sv0t30 = (_sv0t29 + e4);
  int _sv0t31 = (_sv0t30 + e5);
  int _sv0t32 = (_sv0t31 + e6);
  return _sv0t32;
}

