#include "sv0_runtime.h"

typedef struct {
  int name;
  int ctype;
} Param;
typedef struct {
  int label;
  int ret_ctype;
  int instrs;
} IrBlock;
typedef struct {
  int tag;
  int p0;
  int p1;
} Value;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
} Expr;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
  int p3;
} Instr;

static int value_tag(Value v);
static int expr_tag(Expr e);
static int instr_tag(Instr ins);
static int test_values(void);
static int test_exprs(void);
static int test_instrs(void);
static int test_call_variant(void);
static int test_ifelse_variant(void);
static int test_structs(void);

static int value_tag(Value v) {
  int _sv0t0;
  if ((v.tag == 0)) {
    int n = v.p0;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((v.tag == 1)) {
      int h = v.p0;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((v.tag == 2)) {
        int b = v.p0;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((v.tag == 3)) {
          int h = v.p0;
          return 3;
          _sv0t0 = 0;
        } else {
          if ((v.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
            if ((v.tag == 5)) {
              int h = v.p0;
              return 5;
              _sv0t0 = 0;
            } else {
              if ((v.tag == 6)) {
                int bv = v.p0;
                int h = v.p1;
                return 6;
                _sv0t0 = 0;
              } else {
                if ((v.tag == 7)) {
                  int h = v.p0;
                  return 7;
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
  return _sv0t0;
}

static int expr_tag(Expr e) {
  int _sv0t0;
  if ((e.tag == 0)) {
    int bv = e.p0;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((e.tag == 1)) {
      int op = e.p0;
      int l = e.p1;
      int r = e.p2;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((e.tag == 2)) {
        int op = e.p0;
        int v = e.p1;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((e.tag == 3)) {
          int h = e.p0;
          return 3;
          _sv0t0 = 0;
        } else {
          if ((e.tag == 4)) {
            int bv = e.p0;
            int f = e.p1;
            return 4;
            _sv0t0 = 0;
          } else {
            if ((e.tag == 5)) {
              int a = e.p0;
              int i = e.p1;
              return 5;
              _sv0t0 = 0;
            } else {
            }
          }
        }
      }
    }
  }
  return _sv0t0;
}

static int instr_tag(Instr ins) {
  int _sv0t0;
  if ((ins.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((ins.tag == 1)) {
      int h = ins.p0;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((ins.tag == 2)) {
        int ct = ins.p0;
        int h = ins.p1;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((ins.tag == 3)) {
          int h = ins.p0;
          int be = ins.p1;
          return 3;
          _sv0t0 = 0;
        } else {
          if ((ins.tag == 4)) {
            int h = ins.p0;
            int be = ins.p1;
            return 4;
            _sv0t0 = 0;
          } else {
            if ((ins.tag == 5)) {
              int v = ins.p0;
              int f = ins.p1;
              int be = ins.p2;
              return 5;
              _sv0t0 = 0;
            } else {
              if ((ins.tag == 6)) {
                int be = ins.p0;
                int th = ins.p1;
                int el = ins.p2;
                return 6;
                _sv0t0 = 0;
              } else {
                if ((ins.tag == 7)) {
                  int be = ins.p0;
                  int body = ins.p1;
                  return 7;
                  _sv0t0 = 0;
                } else {
                  if ((ins.tag == 8)) {
                    int body = ins.p0;
                    return 8;
                    _sv0t0 = 0;
                  } else {
                    if ((ins.tag == 9)) {
                      return 9;
                      _sv0t0 = 0;
                    } else {
                      if ((ins.tag == 10)) {
                        return 10;
                        _sv0t0 = 0;
                      } else {
                        if ((ins.tag == 11)) {
                          int h = ins.p0;
                          return 11;
                          _sv0t0 = 0;
                        } else {
                          if ((ins.tag == 12)) {
                            int bv = ins.p0;
                            int l1 = ins.p1;
                            int l2 = ins.p2;
                            return 12;
                            _sv0t0 = 0;
                          } else {
                            if ((ins.tag == 13)) {
                              int h = ins.p0;
                              return 13;
                              _sv0t0 = 0;
                            } else {
                              if ((ins.tag == 14)) {
                                int dst = ins.p0;
                                int f = ins.p1;
                                int args = ins.p2;
                                int rt = ins.p3;
                                return 14;
                                _sv0t0 = 0;
                              } else {
                                if ((ins.tag == 15)) {
                                  int be = ins.p0;
                                  int h = ins.p1;
                                  return 15;
                                  _sv0t0 = 0;
                                } else {
                                  if ((ins.tag == 16)) {
                                    int be = ins.p0;
                                    int h = ins.p1;
                                    return 16;
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
  return _sv0t0;
}

static int test_values(void) {
  Value vi;
  vi.tag = 0;
  vi.p0 = 42;
  int _sv0t0 = value_tag(vi);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Value vb;
  vb.tag = 2;
  vb.p0 = 1;
  int _sv0t1 = value_tag(vb);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  Value vu;
  vu.tag = 4;
  int _sv0t2 = value_tag(vu);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  Value vv;
  vv.tag = 5;
  vv.p0 = 10;
  int _sv0t3 = value_tag(vv);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  Value base;
  base.tag = 5;
  base.p0 = 7;
  Value vm;
  int _sv0t4 = sv0_box_alloc(3);
  sv0_box_store(_sv0t4, 0, base.tag);
  sv0_box_store(_sv0t4, 1, base.p0);
  sv0_box_store(_sv0t4, 2, base.p1);
  vm.tag = 6;
  vm.p0 = _sv0t4;
  vm.p1 = 99;
  int _sv0t5 = value_tag(vm);
  if ((_sv0t5 != 6)) {
    return 5;
  } else {
  }
  int _sv0t6;
  if ((vm.tag == 6)) {
    int bv = vm.p0;
    int fld = vm.p1;
    if ((fld != 99)) {
      return 6;
    } else {
    }
    Value _sv0t7;
    int _sv0t8 = sv0_box_load(bv, 0);
    _sv0t7.tag = _sv0t8;
    int _sv0t9 = sv0_box_load(bv, 1);
    _sv0t7.p0 = _sv0t9;
    int _sv0t10 = sv0_box_load(bv, 2);
    _sv0t7.p1 = _sv0t10;
    Value inner;
    inner.tag = (_sv0t7).tag;
    inner.p0 = (_sv0t7).p0;
    inner.p1 = (_sv0t7).p1;
    int _sv0t11 = value_tag(inner);
    if ((_sv0t11 != 5)) {
      return 7;
      _sv0t6 = 0;
    } else {
      _sv0t6 = 0;
    }
  } else {
    if ((vm.tag == 0)) {
      int n = vm.p0;
      return 6;
      _sv0t6 = 0;
    } else {
      if ((vm.tag == 1)) {
        int h = vm.p0;
        return 6;
        _sv0t6 = 0;
      } else {
        if ((vm.tag == 2)) {
          int b = vm.p0;
          return 6;
          _sv0t6 = 0;
        } else {
          if ((vm.tag == 3)) {
            int h = vm.p0;
            return 6;
            _sv0t6 = 0;
          } else {
            if ((vm.tag == 4)) {
              return 6;
              _sv0t6 = 0;
            } else {
              if ((vm.tag == 5)) {
                int h = vm.p0;
                return 6;
                _sv0t6 = 0;
              } else {
                if ((vm.tag == 7)) {
                  int h = vm.p0;
                  return 6;
                  _sv0t6 = 0;
                } else {
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

static int test_exprs(void) {
  Value lit_val;
  lit_val.tag = 0;
  lit_val.p0 = 5;
  Expr el;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, lit_val.tag);
  sv0_box_store(_sv0t0, 1, lit_val.p0);
  sv0_box_store(_sv0t0, 2, lit_val.p1);
  el.tag = 0;
  el.p0 = _sv0t0;
  int _sv0t1 = expr_tag(el);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  Value lv;
  lv.tag = 0;
  lv.p0 = 3;
  Value rv;
  rv.tag = 0;
  rv.p0 = 4;
  Expr eb;
  int _sv0t2 = sv0_box_alloc(3);
  sv0_box_store(_sv0t2, 0, lv.tag);
  sv0_box_store(_sv0t2, 1, lv.p0);
  sv0_box_store(_sv0t2, 2, lv.p1);
  int _sv0t3 = sv0_box_alloc(3);
  sv0_box_store(_sv0t3, 0, rv.tag);
  sv0_box_store(_sv0t3, 1, rv.p0);
  sv0_box_store(_sv0t3, 2, rv.p1);
  eb.tag = 1;
  eb.p0 = 20;
  eb.p1 = _sv0t2;
  eb.p2 = _sv0t3;
  int _sv0t4 = expr_tag(eb);
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  Expr eu;
  eu.tag = 3;
  eu.p0 = 50;
  int _sv0t5 = expr_tag(eu);
  if ((_sv0t5 != 3)) {
    return 3;
  } else {
  }
  Value fv;
  fv.tag = 5;
  fv.p0 = 60;
  Expr ef;
  int _sv0t6 = sv0_box_alloc(3);
  sv0_box_store(_sv0t6, 0, fv.tag);
  sv0_box_store(_sv0t6, 1, fv.p0);
  sv0_box_store(_sv0t6, 2, fv.p1);
  ef.tag = 4;
  ef.p0 = _sv0t6;
  ef.p1 = 61;
  int _sv0t7 = expr_tag(ef);
  if ((_sv0t7 != 4)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_instrs(void) {
  Instr i_nop;
  i_nop.tag = 0;
  int _sv0t0 = instr_tag(i_nop);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Instr i_dv;
  i_dv.tag = 1;
  i_dv.p0 = 50;
  int _sv0t1 = instr_tag(i_dv);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  Instr i_brk;
  i_brk.tag = 9;
  int _sv0t2 = instr_tag(i_brk);
  if ((_sv0t2 != 9)) {
    return 3;
  } else {
  }
  Instr i_cont;
  i_cont.tag = 10;
  int _sv0t3 = instr_tag(i_cont);
  if ((_sv0t3 != 10)) {
    return 4;
  } else {
  }
  Instr i_ret;
  i_ret.tag = 11;
  i_ret.p0 = 0;
  int _sv0t4 = instr_tag(i_ret);
  if ((_sv0t4 != 11)) {
    return 5;
  } else {
  }
  Instr i_jmp;
  i_jmp.tag = 13;
  i_jmp.p0 = 77;
  int _sv0t5 = instr_tag(i_jmp);
  if ((_sv0t5 != 13)) {
    return 6;
  } else {
  }
  Expr asgn_e;
  asgn_e.tag = 3;
  asgn_e.p0 = 70;
  Instr i_asgn;
  int _sv0t6 = sv0_box_alloc(4);
  sv0_box_store(_sv0t6, 0, asgn_e.tag);
  sv0_box_store(_sv0t6, 1, asgn_e.p0);
  sv0_box_store(_sv0t6, 2, asgn_e.p1);
  sv0_box_store(_sv0t6, 3, asgn_e.p2);
  i_asgn.tag = 3;
  i_asgn.p0 = 71;
  i_asgn.p1 = _sv0t6;
  int _sv0t7 = instr_tag(i_asgn);
  if ((_sv0t7 != 3)) {
    return 7;
  } else {
  }
  Expr sf_e;
  sf_e.tag = 3;
  sf_e.p0 = 80;
  Instr i_sf;
  int _sv0t8 = sv0_box_alloc(4);
  sv0_box_store(_sv0t8, 0, sf_e.tag);
  sv0_box_store(_sv0t8, 1, sf_e.p0);
  sv0_box_store(_sv0t8, 2, sf_e.p1);
  sv0_box_store(_sv0t8, 3, sf_e.p2);
  i_sf.tag = 5;
  i_sf.p0 = 81;
  i_sf.p1 = 82;
  i_sf.p2 = _sv0t8;
  int _sv0t9 = instr_tag(i_sf);
  if ((_sv0t9 != 5)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_call_variant(void) {
  int _sv0t0 = sv0_vec_new();
  int call_args = _sv0t0;
  sv0_vec_push(call_args, 100);
  sv0_vec_push(call_args, 101);
  sv0_vec_push(call_args, 102);
  Instr i_call;
  i_call.tag = 14;
  i_call.p0 = 200;
  i_call.p1 = 201;
  i_call.p2 = call_args;
  i_call.p3 = 202;
  int _sv0t1 = instr_tag(i_call);
  if ((_sv0t1 != 14)) {
    return 1;
  } else {
  }
  int _sv0t2;
  if ((i_call.tag == 14)) {
    int dst = i_call.p0;
    int fn_h = i_call.p1;
    int args = i_call.p2;
    int rt = i_call.p3;
    if ((dst != 200)) {
      return 2;
    } else {
    }
    if ((fn_h != 201)) {
      return 3;
    } else {
    }
    int _sv0t3 = sv0_vec_len(args);
    if ((_sv0t3 != 3)) {
      return 4;
    } else {
    }
    if ((rt != 202)) {
      return 5;
      _sv0t2 = 0;
    } else {
      _sv0t2 = 0;
    }
  } else {
    if ((i_call.tag == 0)) {
      return 2;
      _sv0t2 = 0;
    } else {
      if ((i_call.tag == 1)) {
        int h = i_call.p0;
        return 2;
        _sv0t2 = 0;
      } else {
        if ((i_call.tag == 2)) {
          int ct = i_call.p0;
          int h = i_call.p1;
          return 2;
          _sv0t2 = 0;
        } else {
          if ((i_call.tag == 3)) {
            int h = i_call.p0;
            int be = i_call.p1;
            return 2;
            _sv0t2 = 0;
          } else {
            if ((i_call.tag == 4)) {
              int h = i_call.p0;
              int be = i_call.p1;
              return 2;
              _sv0t2 = 0;
            } else {
              if ((i_call.tag == 5)) {
                int v = i_call.p0;
                int f = i_call.p1;
                int be = i_call.p2;
                return 2;
                _sv0t2 = 0;
              } else {
                if ((i_call.tag == 6)) {
                  int be = i_call.p0;
                  int th = i_call.p1;
                  int el = i_call.p2;
                  return 2;
                  _sv0t2 = 0;
                } else {
                  if ((i_call.tag == 7)) {
                    int be = i_call.p0;
                    int body = i_call.p1;
                    return 2;
                    _sv0t2 = 0;
                  } else {
                    if ((i_call.tag == 8)) {
                      int body = i_call.p0;
                      return 2;
                      _sv0t2 = 0;
                    } else {
                      if ((i_call.tag == 9)) {
                        return 2;
                        _sv0t2 = 0;
                      } else {
                        if ((i_call.tag == 10)) {
                          return 2;
                          _sv0t2 = 0;
                        } else {
                          if ((i_call.tag == 11)) {
                            int h = i_call.p0;
                            return 2;
                            _sv0t2 = 0;
                          } else {
                            if ((i_call.tag == 12)) {
                              int bv = i_call.p0;
                              int l1 = i_call.p1;
                              int l2 = i_call.p2;
                              return 2;
                              _sv0t2 = 0;
                            } else {
                              if ((i_call.tag == 13)) {
                                int h = i_call.p0;
                                return 2;
                                _sv0t2 = 0;
                              } else {
                                if ((i_call.tag == 15)) {
                                  int be = i_call.p0;
                                  int h = i_call.p1;
                                  return 2;
                                  _sv0t2 = 0;
                                } else {
                                  if ((i_call.tag == 16)) {
                                    int be = i_call.p0;
                                    int h = i_call.p1;
                                    return 2;
                                    _sv0t2 = 0;
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
  return 0;
}

static int test_ifelse_variant(void) {
  Expr cond_e;
  cond_e.tag = 3;
  cond_e.p0 = 90;
  int _sv0t0 = sv0_vec_new();
  int th_instrs = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int el_instrs = _sv0t1;
  sv0_vec_push(th_instrs, 0);
  sv0_vec_push(el_instrs, 0);
  sv0_vec_push(el_instrs, 1);
  Instr i_ie;
  int _sv0t2 = sv0_box_alloc(4);
  sv0_box_store(_sv0t2, 0, cond_e.tag);
  sv0_box_store(_sv0t2, 1, cond_e.p0);
  sv0_box_store(_sv0t2, 2, cond_e.p1);
  sv0_box_store(_sv0t2, 3, cond_e.p2);
  i_ie.tag = 6;
  i_ie.p0 = _sv0t2;
  i_ie.p1 = th_instrs;
  i_ie.p2 = el_instrs;
  int _sv0t3 = instr_tag(i_ie);
  if ((_sv0t3 != 6)) {
    return 1;
  } else {
  }
  int _sv0t4;
  if ((i_ie.tag == 6)) {
    int be = i_ie.p0;
    int th = i_ie.p1;
    int el = i_ie.p2;
    int _sv0t5 = sv0_vec_len(th);
    if ((_sv0t5 != 1)) {
      return 2;
    } else {
    }
    int _sv0t6 = sv0_vec_len(el);
    if ((_sv0t6 != 2)) {
      return 3;
      _sv0t4 = 0;
    } else {
      _sv0t4 = 0;
    }
  } else {
    if ((i_ie.tag == 0)) {
      return 2;
      _sv0t4 = 0;
    } else {
      if ((i_ie.tag == 1)) {
        int h = i_ie.p0;
        return 2;
        _sv0t4 = 0;
      } else {
        if ((i_ie.tag == 2)) {
          int ct = i_ie.p0;
          int h = i_ie.p1;
          return 2;
          _sv0t4 = 0;
        } else {
          if ((i_ie.tag == 3)) {
            int h = i_ie.p0;
            int be = i_ie.p1;
            return 2;
            _sv0t4 = 0;
          } else {
            if ((i_ie.tag == 4)) {
              int h = i_ie.p0;
              int be = i_ie.p1;
              return 2;
              _sv0t4 = 0;
            } else {
              if ((i_ie.tag == 5)) {
                int v = i_ie.p0;
                int f = i_ie.p1;
                int be = i_ie.p2;
                return 2;
                _sv0t4 = 0;
              } else {
                if ((i_ie.tag == 7)) {
                  int be = i_ie.p0;
                  int body = i_ie.p1;
                  return 2;
                  _sv0t4 = 0;
                } else {
                  if ((i_ie.tag == 8)) {
                    int body = i_ie.p0;
                    return 2;
                    _sv0t4 = 0;
                  } else {
                    if ((i_ie.tag == 9)) {
                      return 2;
                      _sv0t4 = 0;
                    } else {
                      if ((i_ie.tag == 10)) {
                        return 2;
                        _sv0t4 = 0;
                      } else {
                        if ((i_ie.tag == 11)) {
                          int h = i_ie.p0;
                          return 2;
                          _sv0t4 = 0;
                        } else {
                          if ((i_ie.tag == 12)) {
                            int bv = i_ie.p0;
                            int l1 = i_ie.p1;
                            int l2 = i_ie.p2;
                            return 2;
                            _sv0t4 = 0;
                          } else {
                            if ((i_ie.tag == 13)) {
                              int h = i_ie.p0;
                              return 2;
                              _sv0t4 = 0;
                            } else {
                              if ((i_ie.tag == 14)) {
                                int dst = i_ie.p0;
                                int fn_h = i_ie.p1;
                                int args = i_ie.p2;
                                int rt = i_ie.p3;
                                return 2;
                                _sv0t4 = 0;
                              } else {
                                if ((i_ie.tag == 15)) {
                                  int be = i_ie.p0;
                                  int h = i_ie.p1;
                                  return 2;
                                  _sv0t4 = 0;
                                } else {
                                  if ((i_ie.tag == 16)) {
                                    int be = i_ie.p0;
                                    int h = i_ie.p1;
                                    return 2;
                                    _sv0t4 = 0;
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
  return 0;
}

static int test_structs(void) {
  Param p;
  p.name = 10;
  p.ctype = 11;
  if ((p.name != 10)) {
    return 1;
  } else {
  }
  if ((p.ctype != 11)) {
    return 2;
  } else {
  }
  int _sv0t0 = sv0_vec_new();
  int block_instrs = _sv0t0;
  sv0_vec_push(block_instrs, 0);
  IrBlock blk;
  blk.label = 300;
  blk.ret_ctype = 301;
  blk.instrs = block_instrs;
  if ((blk.label != 300)) {
    return 3;
  } else {
  }
  if ((blk.ret_ctype != 301)) {
    return 4;
  } else {
  }
  int _sv0t1 = sv0_vec_len(blk.instrs);
  if ((_sv0t1 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_values();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_exprs();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_instrs();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_call_variant();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_ifelse_variant();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_structs();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  return 0;
}

