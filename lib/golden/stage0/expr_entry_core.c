#include "sv0_runtime.h"

static int arm_lit(void);
static int arm_self(void);
static int arm_self_ty(void);
static int arm_result(void);
static int arm_ident(void);
static int arm_lparen(void);
static int arm_lbracket(void);
static int arm_lbrace(void);
static int arm_if(void);
static int arm_match(void);
static int arm_while(void);
static int arm_for(void);
static int arm_loop(void);
static int arm_return(void);
static int arm_unsafe(void);
static int arm_assert(void);
static int arm_old(void);
static int arm_forall(void);
static int arm_exists(void);
static int arm_no_alias(void);
static int tok_int_lit(void);
static int tok_true(void);
static int tok_false(void);
static int tok_string_lit(void);
static int tok_float_lit(void);
static int tok_char_lit(void);
static int tok_self(void);
static int tok_self_ty(void);
static int tok_result(void);
static int tok_ident(void);
static int tok_lbracket_delim(void);
static int tok_match_kw(void);
static int tok_while_kw(void);
static int tok_for_kw(void);
static int tok_loop_kw(void);
static int tok_return_kw(void);
static int tok_unsafe_kw(void);
static int tok_assert_kw(void);
static int tok_old_kw(void);
static int tok_forall_kw(void);
static int tok_exists_kw(void);
static int tok_no_alias_kw(void);
static int parse_primary_dispatch(int t);
static int lexer_keyword_tag_to_dispatch(int kw);
static int lexer_delim_tag_to_dispatch(int d);
static int primary_arm_is_compound(int arm);

static int arm_lit(void) {
  return 1;
}

static int arm_self(void) {
  return 2;
}

static int arm_self_ty(void) {
  return 3;
}

static int arm_result(void) {
  return 4;
}

static int arm_ident(void) {
  return 5;
}

static int arm_lparen(void) {
  return 6;
}

static int arm_lbracket(void) {
  return 7;
}

static int arm_lbrace(void) {
  return 8;
}

static int arm_if(void) {
  return 9;
}

static int arm_match(void) {
  return 10;
}

static int arm_while(void) {
  return 11;
}

static int arm_for(void) {
  return 12;
}

static int arm_loop(void) {
  return 13;
}

static int arm_return(void) {
  return 14;
}

static int arm_unsafe(void) {
  return 15;
}

static int arm_assert(void) {
  return 16;
}

static int arm_old(void) {
  return 17;
}

static int arm_forall(void) {
  return 18;
}

static int arm_exists(void) {
  return 19;
}

static int arm_no_alias(void) {
  return 20;
}

static int tok_int_lit(void) {
  return 40;
}

static int tok_true(void) {
  return 41;
}

static int tok_false(void) {
  return 42;
}

static int tok_string_lit(void) {
  return 43;
}

static int tok_float_lit(void) {
  return 44;
}

static int tok_char_lit(void) {
  return 45;
}

static int tok_self(void) {
  return 70;
}

static int tok_self_ty(void) {
  return 71;
}

static int tok_result(void) {
  return 72;
}

static int tok_ident(void) {
  return 73;
}

static int tok_lbracket_delim(void) {
  return 16;
}

static int tok_match_kw(void) {
  return 50;
}

static int tok_while_kw(void) {
  return 51;
}

static int tok_for_kw(void) {
  return 52;
}

static int tok_loop_kw(void) {
  return 53;
}

static int tok_return_kw(void) {
  return 54;
}

static int tok_unsafe_kw(void) {
  return 55;
}

static int tok_assert_kw(void) {
  return 56;
}

static int tok_old_kw(void) {
  return 57;
}

static int tok_forall_kw(void) {
  return 58;
}

static int tok_exists_kw(void) {
  return 59;
}

static int tok_no_alias_kw(void) {
  return 60;
}

static int parse_primary_dispatch(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 40)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 41)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 42)) {
        return 1;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((t == 43)) {
          return 1;
          _sv0t4 = 0;
        } else {
          int _sv0t5;
          if ((t == 44)) {
            return 1;
            _sv0t5 = 0;
          } else {
            int _sv0t6;
            if ((t == 45)) {
              return 1;
              _sv0t6 = 0;
            } else {
              int _sv0t7;
              if ((t == 70)) {
                return 2;
                _sv0t7 = 0;
              } else {
                int _sv0t8;
                if ((t == 71)) {
                  return 3;
                  _sv0t8 = 0;
                } else {
                  int _sv0t9;
                  if ((t == 72)) {
                    return 4;
                    _sv0t9 = 0;
                  } else {
                    int _sv0t10;
                    if ((t == 73)) {
                      return 5;
                      _sv0t10 = 0;
                    } else {
                      int _sv0t11;
                      if ((t == 10)) {
                        return 6;
                        _sv0t11 = 0;
                      } else {
                        int _sv0t12;
                        if ((t == 16)) {
                          return 7;
                          _sv0t12 = 0;
                        } else {
                          int _sv0t13;
                          if ((t == 12)) {
                            return 8;
                            _sv0t13 = 0;
                          } else {
                            int _sv0t14;
                            if ((t == 3)) {
                              return 9;
                              _sv0t14 = 0;
                            } else {
                              int _sv0t15;
                              if ((t == 50)) {
                                return 10;
                                _sv0t15 = 0;
                              } else {
                                int _sv0t16;
                                if ((t == 51)) {
                                  return 11;
                                  _sv0t16 = 0;
                                } else {
                                  int _sv0t17;
                                  if ((t == 52)) {
                                    return 12;
                                    _sv0t17 = 0;
                                  } else {
                                    int _sv0t18;
                                    if ((t == 53)) {
                                      return 13;
                                      _sv0t18 = 0;
                                    } else {
                                      int _sv0t19;
                                      if ((t == 54)) {
                                        return 14;
                                        _sv0t19 = 0;
                                      } else {
                                        int _sv0t20;
                                        if ((t == 55)) {
                                          return 15;
                                          _sv0t20 = 0;
                                        } else {
                                          int _sv0t21;
                                          if ((t == 56)) {
                                            return 16;
                                            _sv0t21 = 0;
                                          } else {
                                            int _sv0t22;
                                            if ((t == 57)) {
                                              return 17;
                                              _sv0t22 = 0;
                                            } else {
                                              int _sv0t23;
                                              if ((t == 58)) {
                                                return 18;
                                                _sv0t23 = 0;
                                              } else {
                                                int _sv0t24;
                                                if ((t == 59)) {
                                                  return 19;
                                                  _sv0t24 = 0;
                                                } else {
                                                  int _sv0t25;
                                                  if ((t == 60)) {
                                                    return 20;
                                                    _sv0t25 = 0;
                                                  } else {
                                                    return 0;
                                                    _sv0t25 = 0;
                                                  }
                                                  _sv0t24 = _sv0t25;
                                                }
                                                _sv0t23 = _sv0t24;
                                              }
                                              _sv0t22 = _sv0t23;
                                            }
                                            _sv0t21 = _sv0t22;
                                          }
                                          _sv0t20 = _sv0t21;
                                        }
                                        _sv0t19 = _sv0t20;
                                      }
                                      _sv0t18 = _sv0t19;
                                    }
                                    _sv0t17 = _sv0t18;
                                  }
                                  _sv0t16 = _sv0t17;
                                }
                                _sv0t15 = _sv0t16;
                              }
                              _sv0t14 = _sv0t15;
                            }
                            _sv0t13 = _sv0t14;
                          }
                          _sv0t12 = _sv0t13;
                        }
                        _sv0t11 = _sv0t12;
                      }
                      _sv0t10 = _sv0t11;
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

static int lexer_keyword_tag_to_dispatch(int kw) {
  int _sv0t0 = parse_primary_dispatch(kw);
  return _sv0t0;
}

static int lexer_delim_tag_to_dispatch(int d) {
  int _sv0t0 = parse_primary_dispatch(d);
  return _sv0t0;
}

static int primary_arm_is_compound(int arm) {
  int _sv0t0;
  int _sv0t1;
  if ((arm >= 6)) {
    int _sv0t2;
    if ((arm <= 20)) {
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

int main(void) {
  int _sv0t0 = tok_int_lit();
  int _sv0t1 = parse_primary_dispatch(_sv0t0);
  int _sv0t2 = arm_lit();
  int f0 = (_sv0t1 - _sv0t2);
  int _sv0t3 = tok_true();
  int _sv0t4 = parse_primary_dispatch(_sv0t3);
  int _sv0t5 = arm_lit();
  int f1 = (_sv0t4 - _sv0t5);
  int _sv0t6 = parse_primary_dispatch(3);
  int _sv0t7 = arm_if();
  int f2 = (_sv0t6 - _sv0t7);
  int _sv0t8 = parse_primary_dispatch(10);
  int _sv0t9 = arm_lparen();
  int f3 = (_sv0t8 - _sv0t9);
  int _sv0t10 = parse_primary_dispatch(12);
  int _sv0t11 = arm_lbrace();
  int f4 = (_sv0t10 - _sv0t11);
  int _sv0t12 = tok_lbracket_delim();
  int _sv0t13 = parse_primary_dispatch(_sv0t12);
  int _sv0t14 = arm_lbracket();
  int f5 = (_sv0t13 - _sv0t14);
  int _sv0t15 = tok_ident();
  int _sv0t16 = parse_primary_dispatch(_sv0t15);
  int _sv0t17 = arm_ident();
  int f6 = (_sv0t16 - _sv0t17);
  int _sv0t18 = tok_match_kw();
  int _sv0t19 = parse_primary_dispatch(_sv0t18);
  int _sv0t20 = arm_match();
  int f7 = (_sv0t19 - _sv0t20);
  int _sv0t21 = parse_primary_dispatch(99);
  int f8 = _sv0t21;
  int _sv0t22 = lexer_keyword_tag_to_dispatch(3);
  int _sv0t23 = arm_if();
  int f9 = (_sv0t22 - _sv0t23);
  int _sv0t24 = lexer_delim_tag_to_dispatch(10);
  int _sv0t25 = arm_lparen();
  int f10 = (_sv0t24 - _sv0t25);
  int _sv0t26 = lexer_delim_tag_to_dispatch(11);
  int f11 = _sv0t26;
  int _sv0t27 = lexer_delim_tag_to_dispatch(16);
  int _sv0t28 = arm_lbracket();
  int f12 = (_sv0t27 - _sv0t28);
  int _sv0t29 = lexer_delim_tag_to_dispatch(17);
  int f13 = _sv0t29;
  int _sv0t30 = arm_lit();
  int _sv0t31 = primary_arm_is_compound(_sv0t30);
  int f14 = _sv0t31;
  int _sv0t32 = arm_if();
  int _sv0t33 = primary_arm_is_compound(_sv0t32);
  int f15 = (1 - _sv0t33);
  int _sv0t34 = (f0 + f1);
  int _sv0t35 = (_sv0t34 + f2);
  int _sv0t36 = (_sv0t35 + f3);
  int _sv0t37 = (_sv0t36 + f4);
  int _sv0t38 = (_sv0t37 + f5);
  int _sv0t39 = (_sv0t38 + f6);
  int _sv0t40 = (_sv0t39 + f7);
  int _sv0t41 = (_sv0t40 + f8);
  int _sv0t42 = (_sv0t41 + f9);
  int _sv0t43 = (_sv0t42 + f10);
  int _sv0t44 = (_sv0t43 + f11);
  int _sv0t45 = (_sv0t44 + f12);
  int _sv0t46 = (_sv0t45 + f13);
  int _sv0t47 = (_sv0t46 + f14);
  int _sv0t48 = (_sv0t47 + f15);
  return _sv0t48;
}

