#include "sv0_runtime.h"

static int tok_int_lit(void);
static int tok_ident(void);
static int tag_op_minus(void);
static int tag_bang(void);
static int tag_tilde(void);
static int tag_op_star(void);
static int tag_amp(void);
static int tag_kw_mut_stand_in(void);
static int is_atom_tag(int t);
static int rest_all_zero4(int a, int b, int c, int d);
static int rest_all_zero3(int a, int b, int c);
static int rest_all_zero2(int a, int b);
static int rest_all_zero1(int a);
static int parse_unary_stub_ok5(int t0, int t1, int t2, int t3, int t4);

static int tok_int_lit(void) {
  return 40;
}

static int tok_ident(void) {
  return 73;
}

static int tag_op_minus(void) {
  return 21;
}

static int tag_bang(void) {
  return 30;
}

static int tag_tilde(void) {
  return 31;
}

static int tag_op_star(void) {
  return 22;
}

static int tag_amp(void) {
  return 32;
}

static int tag_kw_mut_stand_in(void) {
  return 8;
}

static int is_atom_tag(int t) {
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

static int rest_all_zero4(int a, int b, int c, int d) {
  int _sv0t0;
  int _sv0t1;
  if ((a == 0)) {
    int _sv0t2;
    if ((b == 0)) {
      int _sv0t3;
      if ((c == 0)) {
        int _sv0t4;
        if ((d == 0)) {
          return 1;
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

static int rest_all_zero3(int a, int b, int c) {
  int _sv0t0;
  int _sv0t1;
  if ((a == 0)) {
    int _sv0t2;
    if ((b == 0)) {
      int _sv0t3;
      if ((c == 0)) {
        return 1;
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

static int rest_all_zero2(int a, int b) {
  int _sv0t0;
  int _sv0t1;
  if ((a == 0)) {
    int _sv0t2;
    if ((b == 0)) {
      return 1;
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

static int rest_all_zero1(int a) {
  int _sv0t0;
  int _sv0t1;
  if ((a == 0)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int parse_unary_stub_ok5(int t0, int t1, int t2, int t3, int t4) {
  int _sv0t2 = is_atom_tag(t0);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    int _sv0t3 = rest_all_zero4(t1, t2, t3, t4);
    return _sv0t3;
    _sv0t1 = 0;
  } else {
    int _sv0t4;
    if ((t0 == 21)) {
      int _sv0t6 = is_atom_tag(t1);
      int _sv0t5;
      if ((_sv0t6 == 1)) {
        int _sv0t7 = rest_all_zero3(t2, t3, t4);
        return _sv0t7;
        _sv0t5 = 0;
      } else {
        int _sv0t8;
        if ((t1 == 21)) {
          int _sv0t10 = is_atom_tag(t2);
          int _sv0t9;
          if ((_sv0t10 == 1)) {
            int _sv0t11 = rest_all_zero2(t3, t4);
            return _sv0t11;
            _sv0t9 = 0;
          } else {
            int _sv0t12;
            if ((t2 == 21)) {
              int _sv0t14 = is_atom_tag(t3);
              int _sv0t13;
              if ((_sv0t14 == 1)) {
                int _sv0t15 = rest_all_zero1(t4);
                return _sv0t15;
                _sv0t13 = 0;
              } else {
                int _sv0t16;
                if ((t3 == 21)) {
                  int _sv0t18 = is_atom_tag(t4);
                  int _sv0t17;
                  if ((_sv0t18 == 1)) {
                    return 1;
                    _sv0t17 = 0;
                  } else {
                    return 0;
                    _sv0t17 = 0;
                  }
                  _sv0t16 = _sv0t17;
                } else {
                  return 0;
                  _sv0t16 = 0;
                }
                _sv0t13 = _sv0t16;
              }
              _sv0t12 = _sv0t13;
            } else {
              return 0;
              _sv0t12 = 0;
            }
            _sv0t9 = _sv0t12;
          }
          _sv0t8 = _sv0t9;
        } else {
          return 0;
          _sv0t8 = 0;
        }
        _sv0t5 = _sv0t8;
      }
      _sv0t4 = _sv0t5;
    } else {
      int _sv0t19;
      if ((t0 == 30)) {
        int _sv0t20;
        if ((t1 == 22)) {
          int _sv0t22 = is_atom_tag(t2);
          int _sv0t21;
          if ((_sv0t22 == 1)) {
            int _sv0t23 = rest_all_zero2(t3, t4);
            return _sv0t23;
            _sv0t21 = 0;
          } else {
            return 0;
            _sv0t21 = 0;
          }
          _sv0t20 = _sv0t21;
        } else {
          return 0;
          _sv0t20 = 0;
        }
        _sv0t19 = _sv0t20;
      } else {
        int _sv0t24;
        if ((t0 == 31)) {
          int _sv0t26 = is_atom_tag(t1);
          int _sv0t25;
          if ((_sv0t26 == 1)) {
            int _sv0t27 = rest_all_zero3(t2, t3, t4);
            return _sv0t27;
            _sv0t25 = 0;
          } else {
            return 0;
            _sv0t25 = 0;
          }
          _sv0t24 = _sv0t25;
        } else {
          int _sv0t28;
          if ((t0 == 32)) {
            int _sv0t30 = is_atom_tag(t1);
            int _sv0t29;
            if ((_sv0t30 == 1)) {
              int _sv0t31 = rest_all_zero3(t2, t3, t4);
              return _sv0t31;
              _sv0t29 = 0;
            } else {
              int _sv0t32;
              if ((t1 == 8)) {
                int _sv0t34 = is_atom_tag(t2);
                int _sv0t33;
                if ((_sv0t34 == 1)) {
                  int _sv0t35 = rest_all_zero2(t3, t4);
                  return _sv0t35;
                  _sv0t33 = 0;
                } else {
                  return 0;
                  _sv0t33 = 0;
                }
                _sv0t32 = _sv0t33;
              } else {
                return 0;
                _sv0t32 = 0;
              }
              _sv0t29 = _sv0t32;
            }
            _sv0t28 = _sv0t29;
          } else {
            return 0;
            _sv0t28 = 0;
          }
          _sv0t24 = _sv0t28;
        }
        _sv0t19 = _sv0t24;
      }
      _sv0t4 = _sv0t19;
    }
    _sv0t1 = _sv0t4;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = tok_int_lit();
  int _sv0t1 = parse_unary_stub_ok5(_sv0t0, 0, 0, 0, 0);
  int e0 = (_sv0t1 - 1);
  int _sv0t2 = tok_ident();
  int _sv0t3 = parse_unary_stub_ok5(_sv0t2, 0, 0, 0, 0);
  int e1 = (_sv0t3 - 1);
  int _sv0t4 = tag_op_minus();
  int _sv0t5 = tok_int_lit();
  int _sv0t6 = parse_unary_stub_ok5(_sv0t4, _sv0t5, 0, 0, 0);
  int e2 = (_sv0t6 - 1);
  int _sv0t7 = tag_op_minus();
  int _sv0t8 = tag_op_minus();
  int _sv0t9 = tok_ident();
  int _sv0t10 = parse_unary_stub_ok5(_sv0t7, _sv0t8, _sv0t9, 0, 0);
  int e3 = (_sv0t10 - 1);
  int _sv0t11 = tag_op_minus();
  int _sv0t12 = tag_op_minus();
  int _sv0t13 = tag_op_minus();
  int _sv0t14 = tag_op_minus();
  int _sv0t15 = tok_int_lit();
  int _sv0t16 = parse_unary_stub_ok5(_sv0t11, _sv0t12, _sv0t13, _sv0t14, _sv0t15);
  int e4 = (_sv0t16 - 1);
  int _sv0t17 = tag_bang();
  int _sv0t18 = tag_op_star();
  int _sv0t19 = tok_ident();
  int _sv0t20 = parse_unary_stub_ok5(_sv0t17, _sv0t18, _sv0t19, 0, 0);
  int e5 = (_sv0t20 - 1);
  int _sv0t21 = tag_tilde();
  int _sv0t22 = tok_int_lit();
  int _sv0t23 = parse_unary_stub_ok5(_sv0t21, _sv0t22, 0, 0, 0);
  int e6 = (_sv0t23 - 1);
  int _sv0t24 = tag_amp();
  int _sv0t25 = tok_ident();
  int _sv0t26 = parse_unary_stub_ok5(_sv0t24, _sv0t25, 0, 0, 0);
  int e7 = (_sv0t26 - 1);
  int _sv0t27 = tag_amp();
  int _sv0t28 = tag_kw_mut_stand_in();
  int _sv0t29 = tok_ident();
  int _sv0t30 = parse_unary_stub_ok5(_sv0t27, _sv0t28, _sv0t29, 0, 0);
  int e8 = (_sv0t30 - 1);
  int _sv0t31 = tok_int_lit();
  int _sv0t32 = parse_unary_stub_ok5(_sv0t31, 21, 0, 0, 0);
  int bad_trail = _sv0t32;
  int _sv0t33 = parse_unary_stub_ok5(21, 21, 21, 21, 21);
  int bad_fifth_minus = _sv0t33;
  int _sv0t34 = tag_bang();
  int _sv0t35 = tok_ident();
  int _sv0t36 = parse_unary_stub_ok5(_sv0t34, _sv0t35, 0, 0, 0);
  int bad_bang_no_star = _sv0t36;
  int _sv0t37 = (bad_trail + bad_fifth_minus);
  int e9 = (_sv0t37 + bad_bang_no_star);
  int _sv0t38 = (e0 + e1);
  int _sv0t39 = (_sv0t38 + e2);
  int _sv0t40 = (_sv0t39 + e3);
  int _sv0t41 = (_sv0t40 + e4);
  int _sv0t42 = (_sv0t41 + e5);
  int _sv0t43 = (_sv0t42 + e6);
  int _sv0t44 = (_sv0t43 + e7);
  int _sv0t45 = (_sv0t44 + e8);
  int _sv0t46 = (_sv0t45 + e9);
  return _sv0t46;
}

