#include "sv0_runtime.h"

typedef struct {
  int pat;
  int guard;
  int body;
} Arm;
typedef struct {
  int name;
  int type_params;
  int params;
  int ret_ty;
  int contracts;
  int body;
  int span;
} FnDef;
typedef struct {
  int name;
  int type_params;
  int fields;
  int span;
} StructDef;
typedef struct {
  int name;
  int type_params;
  int variants;
  int span;
} EnumDef;
typedef struct {
  int name;
  int methods;
  int span;
} TraitDef;
typedef struct {
  int trait_path;
  int self_ty;
  int items;
  int span;
} ImplDef;
typedef struct {
  int tag;
  int p0;
} Literal;
typedef struct {
  int tag;
} Unop;
typedef struct {
  int tag;
} Binop;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
} Ty;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
} Pat;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
  int p3;
} Expr;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
  int p3;
} Stmt;
typedef struct {
  int tag;
  int p0;
  int p1;
} Contract;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
} AstVariant;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
} Item;

static int literal_tag(Literal l);
static int unop_tag(Unop u);
static int binop_tag(Binop b);
static int ty_tag(Ty t);
static int pat_tag(Pat p);
static int expr_tag(Expr e);
static int stmt_tag(Stmt st);
static int contract_tag(Contract c);
static int variant_tag(AstVariant v);
static int item_tag(Item it);
static int test_literals(void);
static int test_ops(void);
static int test_ty(void);
static int test_pat(void);
static int test_expr(void);
static int test_stmt(void);
static int test_items(void);

static int literal_tag(Literal l) {
  int _sv0t0;
  if ((l.tag == 0)) {
    int n = l.p0;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((l.tag == 1)) {
      int h = l.p0;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((l.tag == 2)) {
        int h = l.p0;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((l.tag == 3)) {
          int c = l.p0;
          return 3;
          _sv0t0 = 0;
        } else {
          if ((l.tag == 4)) {
            int b = l.p0;
            return 4;
            _sv0t0 = 0;
          } else {
            if ((l.tag == 5)) {
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

static int unop_tag(Unop u) {
  int _sv0t0;
  if ((u.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((u.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((u.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
        if ((u.tag == 3)) {
          return 3;
          _sv0t0 = 0;
        } else {
          if ((u.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
            if ((u.tag == 5)) {
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

static int binop_tag(Binop b) {
  int _sv0t0;
  if ((b.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((b.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((b.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
        if ((b.tag == 3)) {
          return 3;
          _sv0t0 = 0;
        } else {
          if ((b.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
            if ((b.tag == 5)) {
              return 5;
              _sv0t0 = 0;
            } else {
              if ((b.tag == 6)) {
                return 6;
                _sv0t0 = 0;
              } else {
                if ((b.tag == 7)) {
                  return 7;
                  _sv0t0 = 0;
                } else {
                  if ((b.tag == 8)) {
                    return 8;
                    _sv0t0 = 0;
                  } else {
                    if ((b.tag == 9)) {
                      return 9;
                      _sv0t0 = 0;
                    } else {
                      if ((b.tag == 10)) {
                        return 10;
                        _sv0t0 = 0;
                      } else {
                        if ((b.tag == 11)) {
                          return 11;
                          _sv0t0 = 0;
                        } else {
                          if ((b.tag == 12)) {
                            return 12;
                            _sv0t0 = 0;
                          } else {
                            if ((b.tag == 13)) {
                              return 13;
                              _sv0t0 = 0;
                            } else {
                              if ((b.tag == 14)) {
                                return 14;
                                _sv0t0 = 0;
                              } else {
                                if ((b.tag == 15)) {
                                  return 15;
                                  _sv0t0 = 0;
                                } else {
                                  if ((b.tag == 16)) {
                                    return 16;
                                    _sv0t0 = 0;
                                  } else {
                                    if ((b.tag == 17)) {
                                      return 17;
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
  return _sv0t0;
}

static int ty_tag(Ty t) {
  int _sv0t0;
  if ((t.tag == 0)) {
    int p = t.p0;
    int a = t.p1;
    int s = t.p2;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((t.tag == 1)) {
      int inner = t.p0;
      int s = t.p1;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((t.tag == 2)) {
        int inner = t.p0;
        int s = t.p1;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((t.tag == 3)) {
          int elem = t.p0;
          int e = t.p1;
          int s = t.p2;
          return 3;
          _sv0t0 = 0;
        } else {
          if ((t.tag == 4)) {
            int elem = t.p0;
            int s = t.p1;
            return 4;
            _sv0t0 = 0;
          } else {
            if ((t.tag == 5)) {
              int ts = t.p0;
              int s = t.p1;
              return 5;
              _sv0t0 = 0;
            } else {
              if ((t.tag == 6)) {
                int s = t.p0;
                return 6;
                _sv0t0 = 0;
              } else {
                if ((t.tag == 7)) {
                  int s = t.p0;
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

static int pat_tag(Pat p) {
  int _sv0t0;
  if ((p.tag == 0)) {
    int s = p.p0;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((p.tag == 1)) {
      int n = p.p0;
      int s = p.p1;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((p.tag == 2)) {
        int l = p.p0;
        int s = p.p1;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((p.tag == 3)) {
          int ps = p.p0;
          int s = p.p1;
          return 3;
          _sv0t0 = 0;
        } else {
          if ((p.tag == 4)) {
            int path = p.p0;
            int fs = p.p1;
            int s = p.p2;
            return 4;
            _sv0t0 = 0;
          } else {
            if ((p.tag == 5)) {
              int path = p.p0;
              int ps = p.p1;
              int s = p.p2;
              return 5;
              _sv0t0 = 0;
            } else {
              if ((p.tag == 6)) {
                int ps = p.p0;
                int s = p.p1;
                return 6;
                _sv0t0 = 0;
              } else {
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
    int l = e.p0;
    int s = e.p1;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((e.tag == 1)) {
      int p = e.p0;
      int s = e.p1;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((e.tag == 2)) {
        int op = e.p0;
        int x = e.p1;
        int s = e.p2;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((e.tag == 3)) {
          int op = e.p0;
          int l = e.p1;
          int r = e.p2;
          int s = e.p3;
          return 3;
          _sv0t0 = 0;
        } else {
          if ((e.tag == 4)) {
            int f = e.p0;
            int a = e.p1;
            int s = e.p2;
            return 4;
            _sv0t0 = 0;
          } else {
            if ((e.tag == 5)) {
              int o = e.p0;
              int m = e.p1;
              int a = e.p2;
              int s = e.p3;
              return 5;
              _sv0t0 = 0;
            } else {
              if ((e.tag == 6)) {
                int o = e.p0;
                int f = e.p1;
                int s = e.p2;
                return 6;
                _sv0t0 = 0;
              } else {
                if ((e.tag == 7)) {
                  int o = e.p0;
                  int i = e.p1;
                  int s = e.p2;
                  return 7;
                  _sv0t0 = 0;
                } else {
                  if ((e.tag == 8)) {
                    int o = e.p0;
                    int i = e.p1;
                    int s = e.p2;
                    return 8;
                    _sv0t0 = 0;
                  } else {
                    if ((e.tag == 9)) {
                      int st = e.p0;
                      int tl = e.p1;
                      int s = e.p2;
                      return 9;
                      _sv0t0 = 0;
                    } else {
                      if ((e.tag == 10)) {
                        int c = e.p0;
                        int th = e.p1;
                        int el = e.p2;
                        int s = e.p3;
                        return 10;
                        _sv0t0 = 0;
                      } else {
                        if ((e.tag == 11)) {
                          int sc = e.p0;
                          int ar = e.p1;
                          int s = e.p2;
                          return 11;
                          _sv0t0 = 0;
                        } else {
                          if ((e.tag == 12)) {
                            int c = e.p0;
                            int inv = e.p1;
                            int b = e.p2;
                            int s = e.p3;
                            return 12;
                            _sv0t0 = 0;
                          } else {
                            if ((e.tag == 13)) {
                              int p = e.p0;
                              int it = e.p1;
                              int b = e.p2;
                              int s = e.p3;
                              return 13;
                              _sv0t0 = 0;
                            } else {
                              if ((e.tag == 14)) {
                                int b = e.p0;
                                int s = e.p1;
                                return 14;
                                _sv0t0 = 0;
                              } else {
                                if ((e.tag == 15)) {
                                  int x = e.p0;
                                  int s = e.p1;
                                  return 15;
                                  _sv0t0 = 0;
                                } else {
                                  if ((e.tag == 16)) {
                                    int x = e.p0;
                                    int s = e.p1;
                                    return 16;
                                    _sv0t0 = 0;
                                  } else {
                                    if ((e.tag == 17)) {
                                      int s = e.p0;
                                      return 17;
                                      _sv0t0 = 0;
                                    } else {
                                      if ((e.tag == 18)) {
                                        int l = e.p0;
                                        int r = e.p1;
                                        int s = e.p2;
                                        return 18;
                                        _sv0t0 = 0;
                                      } else {
                                        if ((e.tag == 19)) {
                                          int op = e.p0;
                                          int l = e.p1;
                                          int r = e.p2;
                                          int s = e.p3;
                                          return 19;
                                          _sv0t0 = 0;
                                        } else {
                                          if ((e.tag == 20)) {
                                            int x = e.p0;
                                            int t = e.p1;
                                            int s = e.p2;
                                            return 20;
                                            _sv0t0 = 0;
                                          } else {
                                            if ((e.tag == 21)) {
                                              int lo = e.p0;
                                              int hi = e.p1;
                                              int s = e.p2;
                                              return 21;
                                              _sv0t0 = 0;
                                            } else {
                                              if ((e.tag == 22)) {
                                                int x = e.p0;
                                                int s = e.p1;
                                                return 22;
                                                _sv0t0 = 0;
                                              } else {
                                                if ((e.tag == 23)) {
                                                  int x = e.p0;
                                                  int s = e.p1;
                                                  return 23;
                                                  _sv0t0 = 0;
                                                } else {
                                                  if ((e.tag == 24)) {
                                                    int p = e.p0;
                                                    int fs = e.p1;
                                                    int s = e.p2;
                                                    return 24;
                                                    _sv0t0 = 0;
                                                  } else {
                                                    if ((e.tag == 25)) {
                                                      int xs = e.p0;
                                                      int s = e.p1;
                                                      return 25;
                                                      _sv0t0 = 0;
                                                    } else {
                                                      if ((e.tag == 26)) {
                                                        int xs = e.p0;
                                                        int s = e.p1;
                                                        return 26;
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
              }
            }
          }
        }
      }
    }
  }
  return _sv0t0;
}

static int stmt_tag(Stmt st) {
  int _sv0t0;
  if ((st.tag == 0)) {
    int p = st.p0;
    int m = st.p1;
    int t = st.p2;
    int s = st.p3;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((st.tag == 1)) {
      int e = st.p0;
      int s = st.p1;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((st.tag == 2)) {
        int e = st.p0;
        int s = st.p1;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((st.tag == 3)) {
          int it = st.p0;
          int s = st.p1;
          return 3;
          _sv0t0 = 0;
        } else {
        }
      }
    }
  }
  return _sv0t0;
}

static int contract_tag(Contract c) {
  int _sv0t0;
  if ((c.tag == 0)) {
    int e = c.p0;
    int s = c.p1;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((c.tag == 1)) {
      int e = c.p0;
      int s = c.p1;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((c.tag == 2)) {
        int e = c.p0;
        int s = c.p1;
        return 2;
        _sv0t0 = 0;
      } else {
      }
    }
  }
  return _sv0t0;
}

static int variant_tag(AstVariant v) {
  int _sv0t0;
  if ((v.tag == 0)) {
    int n = v.p0;
    int s = v.p1;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((v.tag == 1)) {
      int n = v.p0;
      int ts = v.p1;
      int s = v.p2;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((v.tag == 2)) {
        int n = v.p0;
        int fs = v.p1;
        int s = v.p2;
        return 2;
        _sv0t0 = 0;
      } else {
      }
    }
  }
  return _sv0t0;
}

static int item_tag(Item it) {
  int _sv0t0;
  if ((it.tag == 0)) {
    int h = it.p0;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((it.tag == 1)) {
      int h = it.p0;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((it.tag == 2)) {
        int h = it.p0;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((it.tag == 3)) {
          int h = it.p0;
          return 3;
          _sv0t0 = 0;
        } else {
          if ((it.tag == 4)) {
            int h = it.p0;
            return 4;
            _sv0t0 = 0;
          } else {
            if ((it.tag == 5)) {
              int p = it.p0;
              int s = it.p1;
              return 5;
              _sv0t0 = 0;
            } else {
              if ((it.tag == 6)) {
                int p = it.p0;
                int s = it.p1;
                return 6;
                _sv0t0 = 0;
              } else {
                if ((it.tag == 7)) {
                  int n = it.p0;
                  int t = it.p1;
                  int s = it.p2;
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

static int test_literals(void) {
  Literal li;
  li.tag = 0;
  li.p0 = 42;
  int _sv0t0 = literal_tag(li);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Literal lb;
  lb.tag = 4;
  lb.p0 = 1;
  int _sv0t1 = literal_tag(lb);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  Literal lu;
  lu.tag = 5;
  int _sv0t2 = literal_tag(lu);
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_ops(void) {
  Unop n;
  n.tag = 0;
  int _sv0t0 = unop_tag(n);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Unop bm;
  bm.tag = 5;
  int _sv0t1 = unop_tag(bm);
  if ((_sv0t1 != 5)) {
    return 2;
  } else {
  }
  Binop a;
  a.tag = 0;
  int _sv0t2 = binop_tag(a);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  Binop g;
  g.tag = 17;
  int _sv0t3 = binop_tag(g);
  if ((_sv0t3 != 17)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_ty(void) {
  Ty tu;
  tu.tag = 6;
  tu.p0 = 0;
  int _sv0t0 = ty_tag(tu);
  if ((_sv0t0 != 6)) {
    return 1;
  } else {
  }
  Ty tn;
  tn.tag = 0;
  tn.p0 = 10;
  tn.p1 = 11;
  tn.p2 = 0;
  int _sv0t1 = ty_tag(tn);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  Ty inner;
  inner.tag = 6;
  inner.p0 = 0;
  Ty tr;
  int _sv0t2 = sv0_box_alloc(4);
  sv0_box_store(_sv0t2, 0, inner.tag);
  sv0_box_store(_sv0t2, 1, inner.p0);
  sv0_box_store(_sv0t2, 2, inner.p1);
  sv0_box_store(_sv0t2, 3, inner.p2);
  tr.tag = 1;
  tr.p0 = _sv0t2;
  tr.p1 = 0;
  int _sv0t3 = ty_tag(tr);
  if ((_sv0t3 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_pat(void) {
  Pat pw;
  pw.tag = 0;
  pw.p0 = 0;
  int _sv0t0 = pat_tag(pw);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Pat pb;
  pb.tag = 1;
  pb.p0 = 50;
  pb.p1 = 0;
  int _sv0t1 = pat_tag(pb);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  Pat pe;
  pe.tag = 5;
  pe.p0 = 60;
  pe.p1 = 61;
  pe.p2 = 0;
  int _sv0t2 = pat_tag(pe);
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_expr(void) {
  Expr el;
  el.tag = 0;
  el.p0 = 42;
  el.p1 = 0;
  int _sv0t0 = expr_tag(el);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Expr ep;
  ep.tag = 1;
  ep.p0 = 10;
  ep.p1 = 0;
  int _sv0t1 = expr_tag(ep);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  Expr ec;
  ec.tag = 17;
  ec.p0 = 0;
  int _sv0t2 = expr_tag(ec);
  if ((_sv0t2 != 17)) {
    return 3;
  } else {
  }
  Expr lhs;
  lhs.tag = 0;
  lhs.p0 = 1;
  lhs.p1 = 0;
  Expr rhs;
  rhs.tag = 0;
  rhs.p0 = 2;
  rhs.p1 = 0;
  Expr eb;
  int _sv0t3 = sv0_box_alloc(5);
  sv0_box_store(_sv0t3, 0, lhs.tag);
  sv0_box_store(_sv0t3, 1, lhs.p0);
  sv0_box_store(_sv0t3, 2, lhs.p1);
  sv0_box_store(_sv0t3, 3, lhs.p2);
  sv0_box_store(_sv0t3, 4, lhs.p3);
  int _sv0t4 = sv0_box_alloc(5);
  sv0_box_store(_sv0t4, 0, rhs.tag);
  sv0_box_store(_sv0t4, 1, rhs.p0);
  sv0_box_store(_sv0t4, 2, rhs.p1);
  sv0_box_store(_sv0t4, 3, rhs.p2);
  sv0_box_store(_sv0t4, 4, rhs.p3);
  eb.tag = 3;
  eb.p0 = 0;
  eb.p1 = _sv0t3;
  eb.p2 = _sv0t4;
  eb.p3 = 0;
  int _sv0t5 = expr_tag(eb);
  if ((_sv0t5 != 3)) {
    return 4;
  } else {
  }
  Expr ea;
  ea.tag = 26;
  ea.p0 = 70;
  ea.p1 = 0;
  int _sv0t6 = expr_tag(ea);
  if ((_sv0t6 != 26)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_stmt(void) {
  Stmt sl;
  sl.tag = 0;
  sl.p0 = 10;
  sl.p1 = 0;
  sl.p2 = 0;
  sl.p3 = 0;
  int _sv0t0 = stmt_tag(sl);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Stmt se;
  se.tag = 1;
  se.p0 = 20;
  se.p1 = 0;
  int _sv0t1 = stmt_tag(se);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_items(void) {
  Contract c;
  c.tag = 0;
  c.p0 = 30;
  c.p1 = 0;
  int _sv0t0 = contract_tag(c);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  AstVariant vu;
  vu.tag = 0;
  vu.p0 = 40;
  vu.p1 = 0;
  int _sv0t1 = variant_tag(vu);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  AstVariant vt;
  vt.tag = 1;
  vt.p0 = 41;
  vt.p1 = 42;
  vt.p2 = 0;
  int _sv0t2 = variant_tag(vt);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  Arm arm;
  arm.pat = 50;
  arm.guard = 0;
  arm.body = 51;
  if ((arm.pat != 50)) {
    return 4;
  } else {
  }
  FnDef fd;
  fd.name = 1;
  fd.type_params = 0;
  fd.params = 0;
  fd.ret_ty = 0;
  fd.contracts = 0;
  fd.body = 0;
  fd.span = 0;
  if ((fd.name != 1)) {
    return 5;
  } else {
  }
  Item iu;
  iu.tag = 5;
  iu.p0 = 60;
  iu.p1 = 0;
  int _sv0t3 = item_tag(iu);
  if ((_sv0t3 != 5)) {
    return 6;
  } else {
  }
  Item ia;
  ia.tag = 7;
  ia.p0 = 70;
  ia.p1 = 71;
  ia.p2 = 0;
  int _sv0t4 = item_tag(ia);
  if ((_sv0t4 != 7)) {
    return 7;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_literals();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_ops();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_ty();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_pat();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_expr();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_stmt();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_items();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (60 + r7);
    return _sv0t12;
  } else {
  }
  return 0;
}

