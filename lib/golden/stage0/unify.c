#include "sv0_runtime.h"

typedef struct {
  int tag;
  int p0;
  int p1;
} Ty;

static int ty_tag(Ty t);
static int unify_ref_inner(Ty a, Ty b);
static int unify_same_tag(Ty a, Ty b);
static int unify(Ty a, Ty b);
static int test_unit_variants(void);
static int test_payload_variants(void);
static int test_recursive(void);
static int test_ref_cross_unify(void);
static int test_tyvar_wildcard(void);
static int test_list_len(void);

static int ty_tag(Ty t) {
  int _sv0t0;
  if ((t.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((t.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((t.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
        if ((t.tag == 3)) {
          return 3;
          _sv0t0 = 0;
        } else {
          if ((t.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
            if ((t.tag == 5)) {
              return 5;
              _sv0t0 = 0;
            } else {
              if ((t.tag == 6)) {
                return 6;
                _sv0t0 = 0;
              } else {
                if ((t.tag == 7)) {
                  int w = t.p0;
                  return 7;
                  _sv0t0 = 0;
                } else {
                  if ((t.tag == 8)) {
                    int w = t.p0;
                    return 8;
                    _sv0t0 = 0;
                  } else {
                    if ((t.tag == 9)) {
                      int w = t.p0;
                      return 9;
                      _sv0t0 = 0;
                    } else {
                      if ((t.tag == 10)) {
                        int n = t.p0;
                        return 10;
                        _sv0t0 = 0;
                      } else {
                        if ((t.tag == 11)) {
                          int id = t.p0;
                          return 11;
                          _sv0t0 = 0;
                        } else {
                          if ((t.tag == 12)) {
                            int id = t.p0;
                            return 12;
                            _sv0t0 = 0;
                          } else {
                            if ((t.tag == 13)) {
                              int b = t.p0;
                              return 13;
                              _sv0t0 = 0;
                            } else {
                              if ((t.tag == 14)) {
                                int b = t.p0;
                                return 14;
                                _sv0t0 = 0;
                              } else {
                                if ((t.tag == 15)) {
                                  int b = t.p0;
                                  return 15;
                                  _sv0t0 = 0;
                                } else {
                                  if ((t.tag == 16)) {
                                    int b = t.p0;
                                    int n = t.p1;
                                    return 16;
                                    _sv0t0 = 0;
                                  } else {
                                    if ((t.tag == 17)) {
                                      int v = t.p0;
                                      return 17;
                                      _sv0t0 = 0;
                                    } else {
                                      if ((t.tag == 18)) {
                                        int ps = t.p0;
                                        int rb = t.p1;
                                        return 18;
                                        _sv0t0 = 0;
                                      } else {
                                        if ((t.tag == 19)) {
                                          int id = t.p0;
                                          int args = t.p1;
                                          return 19;
                                          _sv0t0 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return _sv0t0;
}

static int unify_ref_inner(Ty a, Ty b) {
  int _sv0t0;
  if ((a.tag == 13)) {
    int b1 = a.p0;
    int _sv0t20;
    if ((b.tag == 13)) {
      int b2 = b.p0;
      Ty _sv0t30;
      int _sv0t31 = sv0_box_load(b1, 0);
      _sv0t30.tag = _sv0t31;
      int _sv0t32 = sv0_box_load(b1, 1);
      _sv0t30.p0 = _sv0t32;
      int _sv0t33 = sv0_box_load(b1, 2);
      _sv0t30.p1 = _sv0t33;
      Ty d1;
      d1.tag = (_sv0t30).tag;
      d1.p0 = (_sv0t30).p0;
      d1.p1 = (_sv0t30).p1;
      Ty _sv0t34;
      int _sv0t35 = sv0_box_load(b2, 0);
      _sv0t34.tag = _sv0t35;
      int _sv0t36 = sv0_box_load(b2, 1);
      _sv0t34.p0 = _sv0t36;
      int _sv0t37 = sv0_box_load(b2, 2);
      _sv0t34.p1 = _sv0t37;
      Ty d2;
      d2.tag = (_sv0t34).tag;
      d2.p0 = (_sv0t34).p0;
      d2.p1 = (_sv0t34).p1;
      int _sv0t38 = unify(d1, d2);
      return _sv0t38;
      _sv0t20 = 0;
    } else {
      if ((b.tag == 14)) {
        int b2 = b.p0;
        Ty _sv0t21;
        int _sv0t22 = sv0_box_load(b1, 0);
        _sv0t21.tag = _sv0t22;
        int _sv0t23 = sv0_box_load(b1, 1);
        _sv0t21.p0 = _sv0t23;
        int _sv0t24 = sv0_box_load(b1, 2);
        _sv0t21.p1 = _sv0t24;
        Ty d1;
        d1.tag = (_sv0t21).tag;
        d1.p0 = (_sv0t21).p0;
        d1.p1 = (_sv0t21).p1;
        Ty _sv0t25;
        int _sv0t26 = sv0_box_load(b2, 0);
        _sv0t25.tag = _sv0t26;
        int _sv0t27 = sv0_box_load(b2, 1);
        _sv0t25.p0 = _sv0t27;
        int _sv0t28 = sv0_box_load(b2, 2);
        _sv0t25.p1 = _sv0t28;
        Ty d2;
        d2.tag = (_sv0t25).tag;
        d2.p0 = (_sv0t25).p0;
        d2.p1 = (_sv0t25).p1;
        int _sv0t29 = unify(d1, d2);
        return _sv0t29;
        _sv0t20 = 0;
      } else {
        if (1) {
          return 0;
          _sv0t20 = 0;
        } else {
        }
      }
    }
    _sv0t0 = _sv0t20;
  } else {
    if ((a.tag == 14)) {
      int b1 = a.p0;
      int _sv0t1;
      if ((b.tag == 13)) {
        int b2 = b.p0;
        Ty _sv0t11;
        int _sv0t12 = sv0_box_load(b1, 0);
        _sv0t11.tag = _sv0t12;
        int _sv0t13 = sv0_box_load(b1, 1);
        _sv0t11.p0 = _sv0t13;
        int _sv0t14 = sv0_box_load(b1, 2);
        _sv0t11.p1 = _sv0t14;
        Ty d1;
        d1.tag = (_sv0t11).tag;
        d1.p0 = (_sv0t11).p0;
        d1.p1 = (_sv0t11).p1;
        Ty _sv0t15;
        int _sv0t16 = sv0_box_load(b2, 0);
        _sv0t15.tag = _sv0t16;
        int _sv0t17 = sv0_box_load(b2, 1);
        _sv0t15.p0 = _sv0t17;
        int _sv0t18 = sv0_box_load(b2, 2);
        _sv0t15.p1 = _sv0t18;
        Ty d2;
        d2.tag = (_sv0t15).tag;
        d2.p0 = (_sv0t15).p0;
        d2.p1 = (_sv0t15).p1;
        int _sv0t19 = unify(d1, d2);
        return _sv0t19;
        _sv0t1 = 0;
      } else {
        if ((b.tag == 14)) {
          int b2 = b.p0;
          Ty _sv0t2;
          int _sv0t3 = sv0_box_load(b1, 0);
          _sv0t2.tag = _sv0t3;
          int _sv0t4 = sv0_box_load(b1, 1);
          _sv0t2.p0 = _sv0t4;
          int _sv0t5 = sv0_box_load(b1, 2);
          _sv0t2.p1 = _sv0t5;
          Ty d1;
          d1.tag = (_sv0t2).tag;
          d1.p0 = (_sv0t2).p0;
          d1.p1 = (_sv0t2).p1;
          Ty _sv0t6;
          int _sv0t7 = sv0_box_load(b2, 0);
          _sv0t6.tag = _sv0t7;
          int _sv0t8 = sv0_box_load(b2, 1);
          _sv0t6.p0 = _sv0t8;
          int _sv0t9 = sv0_box_load(b2, 2);
          _sv0t6.p1 = _sv0t9;
          Ty d2;
          d2.tag = (_sv0t6).tag;
          d2.p0 = (_sv0t6).p0;
          d2.p1 = (_sv0t6).p1;
          int _sv0t10 = unify(d1, d2);
          return _sv0t10;
          _sv0t1 = 0;
        } else {
          if (1) {
            return 0;
            _sv0t1 = 0;
          } else {
          }
        }
      }
      _sv0t0 = _sv0t1;
    } else {
      if (1) {
        return 0;
        _sv0t0 = 0;
      } else {
      }
    }
  }
  return _sv0t0;
}

static int unify_same_tag(Ty a, Ty b) {
  int _sv0t0;
  if ((a.tag == 7)) {
    int w1 = a.p0;
    int _sv0t49;
    if ((b.tag == 7)) {
      int w2 = b.p0;
      int _sv0t50 = (w1 == w2);
      return _sv0t50;
      _sv0t49 = 0;
    } else {
      if (1) {
        return 0;
        _sv0t49 = 0;
      } else {
      }
    }
    _sv0t0 = _sv0t49;
  } else {
    if ((a.tag == 8)) {
      int w1 = a.p0;
      int _sv0t47;
      if ((b.tag == 8)) {
        int w2 = b.p0;
        int _sv0t48 = (w1 == w2);
        return _sv0t48;
        _sv0t47 = 0;
      } else {
        if (1) {
          return 0;
          _sv0t47 = 0;
        } else {
        }
      }
      _sv0t0 = _sv0t47;
    } else {
      if ((a.tag == 9)) {
        int w1 = a.p0;
        int _sv0t45;
        if ((b.tag == 9)) {
          int w2 = b.p0;
          int _sv0t46 = (w1 == w2);
          return _sv0t46;
          _sv0t45 = 0;
        } else {
          if (1) {
            return 0;
            _sv0t45 = 0;
          } else {
          }
        }
        _sv0t0 = _sv0t45;
      } else {
        if ((a.tag == 11)) {
          int id1 = a.p0;
          int _sv0t43;
          if ((b.tag == 11)) {
            int id2 = b.p0;
            int _sv0t44 = (id1 == id2);
            return _sv0t44;
            _sv0t43 = 0;
          } else {
            if (1) {
              return 0;
              _sv0t43 = 0;
            } else {
            }
          }
          _sv0t0 = _sv0t43;
        } else {
          if ((a.tag == 12)) {
            int id1 = a.p0;
            int _sv0t41;
            if ((b.tag == 12)) {
              int id2 = b.p0;
              int _sv0t42 = (id1 == id2);
              return _sv0t42;
              _sv0t41 = 0;
            } else {
              if (1) {
                return 0;
                _sv0t41 = 0;
              } else {
              }
            }
            _sv0t0 = _sv0t41;
          } else {
            if ((a.tag == 15)) {
              int b1 = a.p0;
              int _sv0t31;
              if ((b.tag == 15)) {
                int b2 = b.p0;
                Ty _sv0t32;
                int _sv0t33 = sv0_box_load(b1, 0);
                _sv0t32.tag = _sv0t33;
                int _sv0t34 = sv0_box_load(b1, 1);
                _sv0t32.p0 = _sv0t34;
                int _sv0t35 = sv0_box_load(b1, 2);
                _sv0t32.p1 = _sv0t35;
                Ty d1;
                d1.tag = (_sv0t32).tag;
                d1.p0 = (_sv0t32).p0;
                d1.p1 = (_sv0t32).p1;
                Ty _sv0t36;
                int _sv0t37 = sv0_box_load(b2, 0);
                _sv0t36.tag = _sv0t37;
                int _sv0t38 = sv0_box_load(b2, 1);
                _sv0t36.p0 = _sv0t38;
                int _sv0t39 = sv0_box_load(b2, 2);
                _sv0t36.p1 = _sv0t39;
                Ty d2;
                d2.tag = (_sv0t36).tag;
                d2.p0 = (_sv0t36).p0;
                d2.p1 = (_sv0t36).p1;
                int _sv0t40 = unify(d1, d2);
                return _sv0t40;
                _sv0t31 = 0;
              } else {
                if (1) {
                  return 0;
                  _sv0t31 = 0;
                } else {
                }
              }
              _sv0t0 = _sv0t31;
            } else {
              if ((a.tag == 16)) {
                int b1 = a.p0;
                int n1 = a.p1;
                int _sv0t21;
                if ((b.tag == 16)) {
                  int b2 = b.p0;
                  int n2 = b.p1;
                  if ((n1 != n2)) {
                    return 0;
                  } else {
                  }
                  Ty _sv0t22;
                  int _sv0t23 = sv0_box_load(b1, 0);
                  _sv0t22.tag = _sv0t23;
                  int _sv0t24 = sv0_box_load(b1, 1);
                  _sv0t22.p0 = _sv0t24;
                  int _sv0t25 = sv0_box_load(b1, 2);
                  _sv0t22.p1 = _sv0t25;
                  Ty d1;
                  d1.tag = (_sv0t22).tag;
                  d1.p0 = (_sv0t22).p0;
                  d1.p1 = (_sv0t22).p1;
                  Ty _sv0t26;
                  int _sv0t27 = sv0_box_load(b2, 0);
                  _sv0t26.tag = _sv0t27;
                  int _sv0t28 = sv0_box_load(b2, 1);
                  _sv0t26.p0 = _sv0t28;
                  int _sv0t29 = sv0_box_load(b2, 2);
                  _sv0t26.p1 = _sv0t29;
                  Ty d2;
                  d2.tag = (_sv0t26).tag;
                  d2.p0 = (_sv0t26).p0;
                  d2.p1 = (_sv0t26).p1;
                  int _sv0t30 = unify(d1, d2);
                  return _sv0t30;
                  _sv0t21 = 0;
                } else {
                  if (1) {
                    return 0;
                    _sv0t21 = 0;
                  } else {
                  }
                }
                _sv0t0 = _sv0t21;
              } else {
                if ((a.tag == 17)) {
                  int v1 = a.p0;
                  int _sv0t17;
                  if ((b.tag == 17)) {
                    int v2 = b.p0;
                    int _sv0t18 = sv0_vec_len(v1);
                    int _sv0t19 = sv0_vec_len(v2);
                    int _sv0t20 = (_sv0t18 == _sv0t19);
                    return _sv0t20;
                    _sv0t17 = 0;
                  } else {
                    if (1) {
                      return 0;
                      _sv0t17 = 0;
                    } else {
                    }
                  }
                  _sv0t0 = _sv0t17;
                } else {
                  if ((a.tag == 18)) {
                    int ps1 = a.p0;
                    int r1 = a.p1;
                    int _sv0t5;
                    if ((b.tag == 18)) {
                      int ps2 = b.p0;
                      int r2 = b.p1;
                      int _sv0t6 = sv0_vec_len(ps1);
                      int _sv0t7 = sv0_vec_len(ps2);
                      if ((_sv0t6 != _sv0t7)) {
                        return 0;
                      } else {
                      }
                      Ty _sv0t8;
                      int _sv0t9 = sv0_box_load(r1, 0);
                      _sv0t8.tag = _sv0t9;
                      int _sv0t10 = sv0_box_load(r1, 1);
                      _sv0t8.p0 = _sv0t10;
                      int _sv0t11 = sv0_box_load(r1, 2);
                      _sv0t8.p1 = _sv0t11;
                      Ty d1;
                      d1.tag = (_sv0t8).tag;
                      d1.p0 = (_sv0t8).p0;
                      d1.p1 = (_sv0t8).p1;
                      Ty _sv0t12;
                      int _sv0t13 = sv0_box_load(r2, 0);
                      _sv0t12.tag = _sv0t13;
                      int _sv0t14 = sv0_box_load(r2, 1);
                      _sv0t12.p0 = _sv0t14;
                      int _sv0t15 = sv0_box_load(r2, 2);
                      _sv0t12.p1 = _sv0t15;
                      Ty d2;
                      d2.tag = (_sv0t12).tag;
                      d2.p0 = (_sv0t12).p0;
                      d2.p1 = (_sv0t12).p1;
                      int _sv0t16 = unify(d1, d2);
                      return _sv0t16;
                      _sv0t5 = 0;
                    } else {
                      if (1) {
                        return 0;
                        _sv0t5 = 0;
                      } else {
                      }
                    }
                    _sv0t0 = _sv0t5;
                  } else {
                    if ((a.tag == 19)) {
                      int n1 = a.p0;
                      int g1 = a.p1;
                      int _sv0t1;
                      if ((b.tag == 19)) {
                        int n2 = b.p0;
                        int g2 = b.p1;
                        if ((n1 != n2)) {
                          return 0;
                        } else {
                        }
                        int _sv0t2 = sv0_vec_len(g1);
                        int _sv0t3 = sv0_vec_len(g2);
                        int _sv0t4 = (_sv0t2 == _sv0t3);
                        return _sv0t4;
                        _sv0t1 = 0;
                      } else {
                        if (1) {
                          return 0;
                          _sv0t1 = 0;
                        } else {
                        }
                      }
                      _sv0t0 = _sv0t1;
                    } else {
                      if (1) {
                        return 1;
                        _sv0t0 = 0;
                      } else {
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return _sv0t0;
}

static int unify(Ty a, Ty b) {
  int _sv0t0 = ty_tag(a);
  int ta = _sv0t0;
  int _sv0t1 = ty_tag(b);
  int tb = _sv0t1;
  if ((ta == 10)) {
    return 1;
  } else {
  }
  if ((tb == 10)) {
    return 1;
  } else {
  }
  if ((ta == 13)) {
    if ((tb == 13)) {
      int _sv0t2 = unify_ref_inner(a, b);
      return _sv0t2;
    } else {
    }
  } else {
  }
  if ((ta == 13)) {
    if ((tb == 14)) {
      int _sv0t3 = unify_ref_inner(a, b);
      return _sv0t3;
    } else {
    }
  } else {
  }
  if ((ta == 14)) {
    if ((tb == 13)) {
      int _sv0t4 = unify_ref_inner(a, b);
      return _sv0t4;
    } else {
    }
  } else {
  }
  if ((ta == 14)) {
    if ((tb == 14)) {
      int _sv0t5 = unify_ref_inner(a, b);
      return _sv0t5;
    } else {
    }
  } else {
  }
  if ((ta != tb)) {
    return 0;
  } else {
  }
  if ((ta <= 6)) {
    return 1;
  } else {
  }
  int _sv0t6 = unify_same_tag(a, b);
  return _sv0t6;
}

static int test_unit_variants(void) {
  Ty _sv0t0;
  _sv0t0.tag = 0;
  Ty _sv0t1;
  _sv0t1.tag = 0;
  int _sv0t2 = unify(_sv0t0, _sv0t1);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  Ty _sv0t3;
  _sv0t3.tag = 1;
  Ty _sv0t4;
  _sv0t4.tag = 1;
  int _sv0t5 = unify(_sv0t3, _sv0t4);
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  Ty _sv0t6;
  _sv0t6.tag = 2;
  Ty _sv0t7;
  _sv0t7.tag = 2;
  int _sv0t8 = unify(_sv0t6, _sv0t7);
  if ((_sv0t8 != 1)) {
    return 3;
  } else {
  }
  Ty _sv0t9;
  _sv0t9.tag = 3;
  Ty _sv0t10;
  _sv0t10.tag = 3;
  int _sv0t11 = unify(_sv0t9, _sv0t10);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  Ty _sv0t12;
  _sv0t12.tag = 4;
  Ty _sv0t13;
  _sv0t13.tag = 4;
  int _sv0t14 = unify(_sv0t12, _sv0t13);
  if ((_sv0t14 != 1)) {
    return 5;
  } else {
  }
  Ty _sv0t15;
  _sv0t15.tag = 5;
  Ty _sv0t16;
  _sv0t16.tag = 5;
  int _sv0t17 = unify(_sv0t15, _sv0t16);
  if ((_sv0t17 != 1)) {
    return 6;
  } else {
  }
  Ty _sv0t18;
  _sv0t18.tag = 6;
  Ty _sv0t19;
  _sv0t19.tag = 6;
  int _sv0t20 = unify(_sv0t18, _sv0t19);
  if ((_sv0t20 != 1)) {
    return 7;
  } else {
  }
  Ty _sv0t21;
  _sv0t21.tag = 0;
  Ty _sv0t22;
  _sv0t22.tag = 1;
  int _sv0t23 = unify(_sv0t21, _sv0t22);
  if ((_sv0t23 != 0)) {
    return 8;
  } else {
  }
  Ty _sv0t24;
  _sv0t24.tag = 1;
  Ty _sv0t25;
  _sv0t25.tag = 2;
  int _sv0t26 = unify(_sv0t24, _sv0t25);
  if ((_sv0t26 != 0)) {
    return 9;
  } else {
  }
  Ty _sv0t27;
  _sv0t27.tag = 3;
  Ty _sv0t28;
  _sv0t28.tag = 4;
  int _sv0t29 = unify(_sv0t27, _sv0t28);
  if ((_sv0t29 != 0)) {
    return 10;
  } else {
  }
  return 0;
}

static int test_payload_variants(void) {
  Ty _sv0t0;
  _sv0t0.tag = 7;
  _sv0t0.p0 = 32;
  Ty _sv0t1;
  _sv0t1.tag = 7;
  _sv0t1.p0 = 32;
  int _sv0t2 = unify(_sv0t0, _sv0t1);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  Ty _sv0t3;
  _sv0t3.tag = 7;
  _sv0t3.p0 = 32;
  Ty _sv0t4;
  _sv0t4.tag = 7;
  _sv0t4.p0 = 64;
  int _sv0t5 = unify(_sv0t3, _sv0t4);
  if ((_sv0t5 != 0)) {
    return 2;
  } else {
  }
  Ty _sv0t6;
  _sv0t6.tag = 8;
  _sv0t6.p0 = 8;
  Ty _sv0t7;
  _sv0t7.tag = 8;
  _sv0t7.p0 = 8;
  int _sv0t8 = unify(_sv0t6, _sv0t7);
  if ((_sv0t8 != 1)) {
    return 3;
  } else {
  }
  Ty _sv0t9;
  _sv0t9.tag = 8;
  _sv0t9.p0 = 8;
  Ty _sv0t10;
  _sv0t10.tag = 8;
  _sv0t10.p0 = 16;
  int _sv0t11 = unify(_sv0t9, _sv0t10);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  Ty _sv0t12;
  _sv0t12.tag = 9;
  _sv0t12.p0 = 32;
  Ty _sv0t13;
  _sv0t13.tag = 9;
  _sv0t13.p0 = 32;
  int _sv0t14 = unify(_sv0t12, _sv0t13);
  if ((_sv0t14 != 1)) {
    return 5;
  } else {
  }
  Ty _sv0t15;
  _sv0t15.tag = 9;
  _sv0t15.p0 = 32;
  Ty _sv0t16;
  _sv0t16.tag = 9;
  _sv0t16.p0 = 64;
  int _sv0t17 = unify(_sv0t15, _sv0t16);
  if ((_sv0t17 != 0)) {
    return 6;
  } else {
  }
  Ty _sv0t18;
  _sv0t18.tag = 11;
  _sv0t18.p0 = 1;
  Ty _sv0t19;
  _sv0t19.tag = 11;
  _sv0t19.p0 = 1;
  int _sv0t20 = unify(_sv0t18, _sv0t19);
  if ((_sv0t20 != 1)) {
    return 7;
  } else {
  }
  Ty _sv0t21;
  _sv0t21.tag = 11;
  _sv0t21.p0 = 1;
  Ty _sv0t22;
  _sv0t22.tag = 11;
  _sv0t22.p0 = 2;
  int _sv0t23 = unify(_sv0t21, _sv0t22);
  if ((_sv0t23 != 0)) {
    return 8;
  } else {
  }
  Ty _sv0t24;
  _sv0t24.tag = 12;
  _sv0t24.p0 = 5;
  Ty _sv0t25;
  _sv0t25.tag = 12;
  _sv0t25.p0 = 5;
  int _sv0t26 = unify(_sv0t24, _sv0t25);
  if ((_sv0t26 != 1)) {
    return 9;
  } else {
  }
  Ty _sv0t27;
  _sv0t27.tag = 12;
  _sv0t27.p0 = 5;
  Ty _sv0t28;
  _sv0t28.tag = 12;
  _sv0t28.p0 = 6;
  int _sv0t29 = unify(_sv0t27, _sv0t28);
  if ((_sv0t29 != 0)) {
    return 10;
  } else {
  }
  Ty _sv0t30;
  _sv0t30.tag = 7;
  _sv0t30.p0 = 32;
  Ty _sv0t31;
  _sv0t31.tag = 8;
  _sv0t31.p0 = 32;
  int _sv0t32 = unify(_sv0t30, _sv0t31);
  if ((_sv0t32 != 0)) {
    return 11;
  } else {
  }
  Ty _sv0t33;
  _sv0t33.tag = 11;
  _sv0t33.p0 = 1;
  Ty _sv0t34;
  _sv0t34.tag = 12;
  _sv0t34.p0 = 1;
  int _sv0t35 = unify(_sv0t33, _sv0t34);
  if ((_sv0t35 != 0)) {
    return 12;
  } else {
  }
  return 0;
}

static int test_recursive(void) {
  Ty r1;
  Ty _sv0t0;
  _sv0t0.tag = 1;
  int _sv0t1 = sv0_box_alloc(3);
  sv0_box_store(_sv0t1, 0, _sv0t0.tag);
  sv0_box_store(_sv0t1, 1, _sv0t0.p0);
  sv0_box_store(_sv0t1, 2, _sv0t0.p1);
  r1.tag = 13;
  r1.p0 = _sv0t1;
  Ty r2;
  Ty _sv0t2;
  _sv0t2.tag = 1;
  int _sv0t3 = sv0_box_alloc(3);
  sv0_box_store(_sv0t3, 0, _sv0t2.tag);
  sv0_box_store(_sv0t3, 1, _sv0t2.p0);
  sv0_box_store(_sv0t3, 2, _sv0t2.p1);
  r2.tag = 13;
  r2.p0 = _sv0t3;
  int _sv0t4 = unify(r1, r2);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  Ty r3;
  Ty _sv0t5;
  _sv0t5.tag = 2;
  int _sv0t6 = sv0_box_alloc(3);
  sv0_box_store(_sv0t6, 0, _sv0t5.tag);
  sv0_box_store(_sv0t6, 1, _sv0t5.p0);
  sv0_box_store(_sv0t6, 2, _sv0t5.p1);
  r3.tag = 13;
  r3.p0 = _sv0t6;
  int _sv0t7 = unify(r1, r3);
  if ((_sv0t7 != 0)) {
    return 2;
  } else {
  }
  Ty s1;
  Ty _sv0t8;
  _sv0t8.tag = 7;
  _sv0t8.p0 = 32;
  int _sv0t9 = sv0_box_alloc(3);
  sv0_box_store(_sv0t9, 0, _sv0t8.tag);
  sv0_box_store(_sv0t9, 1, _sv0t8.p0);
  sv0_box_store(_sv0t9, 2, _sv0t8.p1);
  s1.tag = 15;
  s1.p0 = _sv0t9;
  Ty s2;
  Ty _sv0t10;
  _sv0t10.tag = 7;
  _sv0t10.p0 = 32;
  int _sv0t11 = sv0_box_alloc(3);
  sv0_box_store(_sv0t11, 0, _sv0t10.tag);
  sv0_box_store(_sv0t11, 1, _sv0t10.p0);
  sv0_box_store(_sv0t11, 2, _sv0t10.p1);
  s2.tag = 15;
  s2.p0 = _sv0t11;
  int _sv0t12 = unify(s1, s2);
  if ((_sv0t12 != 1)) {
    return 3;
  } else {
  }
  Ty s3;
  Ty _sv0t13;
  _sv0t13.tag = 7;
  _sv0t13.p0 = 64;
  int _sv0t14 = sv0_box_alloc(3);
  sv0_box_store(_sv0t14, 0, _sv0t13.tag);
  sv0_box_store(_sv0t14, 1, _sv0t13.p0);
  sv0_box_store(_sv0t14, 2, _sv0t13.p1);
  s3.tag = 15;
  s3.p0 = _sv0t14;
  int _sv0t15 = unify(s1, s3);
  if ((_sv0t15 != 0)) {
    return 4;
  } else {
  }
  Ty a1;
  Ty _sv0t16;
  _sv0t16.tag = 1;
  int _sv0t17 = sv0_box_alloc(3);
  sv0_box_store(_sv0t17, 0, _sv0t16.tag);
  sv0_box_store(_sv0t17, 1, _sv0t16.p0);
  sv0_box_store(_sv0t17, 2, _sv0t16.p1);
  a1.tag = 16;
  a1.p0 = _sv0t17;
  a1.p1 = 10;
  Ty a2;
  Ty _sv0t18;
  _sv0t18.tag = 1;
  int _sv0t19 = sv0_box_alloc(3);
  sv0_box_store(_sv0t19, 0, _sv0t18.tag);
  sv0_box_store(_sv0t19, 1, _sv0t18.p0);
  sv0_box_store(_sv0t19, 2, _sv0t18.p1);
  a2.tag = 16;
  a2.p0 = _sv0t19;
  a2.p1 = 10;
  int _sv0t20 = unify(a1, a2);
  if ((_sv0t20 != 1)) {
    return 5;
  } else {
  }
  Ty a3;
  Ty _sv0t21;
  _sv0t21.tag = 1;
  int _sv0t22 = sv0_box_alloc(3);
  sv0_box_store(_sv0t22, 0, _sv0t21.tag);
  sv0_box_store(_sv0t22, 1, _sv0t21.p0);
  sv0_box_store(_sv0t22, 2, _sv0t21.p1);
  a3.tag = 16;
  a3.p0 = _sv0t22;
  a3.p1 = 20;
  int _sv0t23 = unify(a1, a3);
  if ((_sv0t23 != 0)) {
    return 6;
  } else {
  }
  Ty a4;
  Ty _sv0t24;
  _sv0t24.tag = 2;
  int _sv0t25 = sv0_box_alloc(3);
  sv0_box_store(_sv0t25, 0, _sv0t24.tag);
  sv0_box_store(_sv0t25, 1, _sv0t24.p0);
  sv0_box_store(_sv0t25, 2, _sv0t24.p1);
  a4.tag = 16;
  a4.p0 = _sv0t25;
  a4.p1 = 10;
  int _sv0t26 = unify(a1, a4);
  if ((_sv0t26 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_ref_cross_unify(void) {
  Ty ref1;
  Ty _sv0t0;
  _sv0t0.tag = 1;
  int _sv0t1 = sv0_box_alloc(3);
  sv0_box_store(_sv0t1, 0, _sv0t0.tag);
  sv0_box_store(_sv0t1, 1, _sv0t0.p0);
  sv0_box_store(_sv0t1, 2, _sv0t0.p1);
  ref1.tag = 13;
  ref1.p0 = _sv0t1;
  Ty mut1;
  Ty _sv0t2;
  _sv0t2.tag = 1;
  int _sv0t3 = sv0_box_alloc(3);
  sv0_box_store(_sv0t3, 0, _sv0t2.tag);
  sv0_box_store(_sv0t3, 1, _sv0t2.p0);
  sv0_box_store(_sv0t3, 2, _sv0t2.p1);
  mut1.tag = 14;
  mut1.p0 = _sv0t3;
  int _sv0t4 = unify(ref1, mut1);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = unify(mut1, ref1);
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  Ty mut2;
  Ty _sv0t6;
  _sv0t6.tag = 1;
  int _sv0t7 = sv0_box_alloc(3);
  sv0_box_store(_sv0t7, 0, _sv0t6.tag);
  sv0_box_store(_sv0t7, 1, _sv0t6.p0);
  sv0_box_store(_sv0t7, 2, _sv0t6.p1);
  mut2.tag = 14;
  mut2.p0 = _sv0t7;
  int _sv0t8 = unify(mut1, mut2);
  if ((_sv0t8 != 1)) {
    return 3;
  } else {
  }
  Ty ref2;
  Ty _sv0t9;
  _sv0t9.tag = 2;
  int _sv0t10 = sv0_box_alloc(3);
  sv0_box_store(_sv0t10, 0, _sv0t9.tag);
  sv0_box_store(_sv0t10, 1, _sv0t9.p0);
  sv0_box_store(_sv0t10, 2, _sv0t9.p1);
  ref2.tag = 13;
  ref2.p0 = _sv0t10;
  int _sv0t11 = unify(ref1, ref2);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  Ty mut3;
  Ty _sv0t12;
  _sv0t12.tag = 2;
  int _sv0t13 = sv0_box_alloc(3);
  sv0_box_store(_sv0t13, 0, _sv0t12.tag);
  sv0_box_store(_sv0t13, 1, _sv0t12.p0);
  sv0_box_store(_sv0t13, 2, _sv0t12.p1);
  mut3.tag = 14;
  mut3.p0 = _sv0t13;
  int _sv0t14 = unify(ref1, mut3);
  if ((_sv0t14 != 0)) {
    return 5;
  } else {
  }
  int _sv0t15 = unify(mut1, ref2);
  if ((_sv0t15 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_tyvar_wildcard(void) {
  Ty tv;
  tv.tag = 10;
  tv.p0 = 0;
  Ty _sv0t0;
  _sv0t0.tag = 7;
  _sv0t0.p0 = 32;
  int _sv0t1 = unify(tv, _sv0t0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  Ty _sv0t2;
  _sv0t2.tag = 1;
  int _sv0t3 = unify(tv, _sv0t2);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  Ty _sv0t4;
  _sv0t4.tag = 0;
  int _sv0t5 = unify(tv, _sv0t4);
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  Ty _sv0t6;
  _sv0t6.tag = 9;
  _sv0t6.p0 = 64;
  int _sv0t7 = unify(_sv0t6, tv);
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  Ty _sv0t8;
  _sv0t8.tag = 3;
  int _sv0t9 = unify(_sv0t8, tv);
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  Ty tv2;
  tv2.tag = 10;
  tv2.p0 = 1;
  int _sv0t10 = unify(tv, tv2);
  if ((_sv0t10 != 1)) {
    return 6;
  } else {
  }
  Ty deep;
  Ty _sv0t11;
  Ty _sv0t12;
  _sv0t12.tag = 2;
  int _sv0t13 = sv0_box_alloc(3);
  sv0_box_store(_sv0t13, 0, _sv0t12.tag);
  sv0_box_store(_sv0t13, 1, _sv0t12.p0);
  sv0_box_store(_sv0t13, 2, _sv0t12.p1);
  _sv0t11.tag = 15;
  _sv0t11.p0 = _sv0t13;
  int _sv0t14 = sv0_box_alloc(3);
  sv0_box_store(_sv0t14, 0, _sv0t11.tag);
  sv0_box_store(_sv0t14, 1, _sv0t11.p0);
  sv0_box_store(_sv0t14, 2, _sv0t11.p1);
  deep.tag = 13;
  deep.p0 = _sv0t14;
  int _sv0t15 = unify(tv, deep);
  if ((_sv0t15 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_list_len(void) {
  int _sv0t0 = sv0_vec_new();
  int v1 = _sv0t0;
  sv0_vec_push(v1, 0);
  sv0_vec_push(v1, 1);
  int _sv0t1 = sv0_vec_new();
  int v2 = _sv0t1;
  sv0_vec_push(v2, 0);
  sv0_vec_push(v2, 1);
  Ty t1;
  t1.tag = 17;
  t1.p0 = v1;
  Ty t2;
  t2.tag = 17;
  t2.p0 = v2;
  int _sv0t2 = unify(t1, t2);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int v3 = _sv0t3;
  sv0_vec_push(v3, 0);
  Ty t3;
  t3.tag = 17;
  t3.p0 = v3;
  int _sv0t4 = unify(t1, t3);
  if ((_sv0t4 != 0)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int p1 = _sv0t5;
  sv0_vec_push(p1, 0);
  Ty r1;
  Ty _sv0t6;
  _sv0t6.tag = 1;
  int _sv0t7 = sv0_box_alloc(3);
  sv0_box_store(_sv0t7, 0, _sv0t6.tag);
  sv0_box_store(_sv0t7, 1, _sv0t6.p0);
  sv0_box_store(_sv0t7, 2, _sv0t6.p1);
  r1.tag = 18;
  r1.p0 = p1;
  r1.p1 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int p2 = _sv0t8;
  sv0_vec_push(p2, 0);
  Ty r2;
  Ty _sv0t9;
  _sv0t9.tag = 1;
  int _sv0t10 = sv0_box_alloc(3);
  sv0_box_store(_sv0t10, 0, _sv0t9.tag);
  sv0_box_store(_sv0t10, 1, _sv0t9.p0);
  sv0_box_store(_sv0t10, 2, _sv0t9.p1);
  r2.tag = 18;
  r2.p0 = p2;
  r2.p1 = _sv0t10;
  int _sv0t11 = unify(r1, r2);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int p3 = _sv0t12;
  sv0_vec_push(p3, 0);
  Ty r3;
  Ty _sv0t13;
  _sv0t13.tag = 2;
  int _sv0t14 = sv0_box_alloc(3);
  sv0_box_store(_sv0t14, 0, _sv0t13.tag);
  sv0_box_store(_sv0t14, 1, _sv0t13.p0);
  sv0_box_store(_sv0t14, 2, _sv0t13.p1);
  r3.tag = 18;
  r3.p0 = p3;
  r3.p1 = _sv0t14;
  int _sv0t15 = unify(r1, r3);
  if ((_sv0t15 != 0)) {
    return 4;
  } else {
  }
  int _sv0t16 = sv0_vec_new();
  int g1 = _sv0t16;
  sv0_vec_push(g1, 0);
  Ty n1;
  n1.tag = 19;
  n1.p0 = 42;
  n1.p1 = g1;
  int _sv0t17 = sv0_vec_new();
  int g2 = _sv0t17;
  sv0_vec_push(g2, 0);
  Ty n2;
  n2.tag = 19;
  n2.p0 = 42;
  n2.p1 = g2;
  int _sv0t18 = unify(n1, n2);
  if ((_sv0t18 != 1)) {
    return 5;
  } else {
  }
  int _sv0t19 = sv0_vec_new();
  int g3 = _sv0t19;
  sv0_vec_push(g3, 0);
  Ty n3;
  n3.tag = 19;
  n3.p0 = 99;
  n3.p1 = g3;
  int _sv0t20 = unify(n1, n3);
  if ((_sv0t20 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_unit_variants();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_payload_variants();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (20 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_recursive();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (40 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_ref_cross_unify();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (50 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_tyvar_wildcard();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (60 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_list_len();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (70 + r6);
    return _sv0t10;
  } else {
  }
  return 0;
}

