#include "sv0_runtime.h"

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
} Instr;

static int value_tag(Value v);
static int expr_tag(Expr e);
static const char* name_of(int h);
static const char* op_of(int h);
static const char* digit_char(int d);
static const char* int_to_str_pos(int n);
static const char* int_to_string(int n);
static const char* c_escape_one(int c, const char* src, int pos);
static const char* c_escape_from(const char* src, int pos, const char* acc);
static const char* c_escape_string(const char* s);
static const char* emit_value(Value v);
static const char* emit_expr(Expr e);
static int instr_tag(Instr ins);
static const char* emit_instr(const char* indent, const char* ret_ty, Instr ins);
static const char* emit_instrs(int instrs, const char* indent, const char* ret_ty);
static const char* emit_param_pair(int cty_h, int name_h);
static const char* emit_param_list_0(void);
static const char* emit_param_list_1(int cty1, int n1);
static const char* emit_param_list_2(int cty1, int n1, int cty2, int n2);
static const char* emit_fn_proto(int is_static, int ret_cty_h, int label_h, const char* params_str);
static const char* emit_block_defn(const char* proto, const char* body);
static int test_int_format(void);
static int test_escape(void);
static int test_emit_value_basic(void);
static int test_emit_value_string(void);
static int test_emit_value_addr(void);
static int test_emit_member(void);
static int test_emit_expr_lit(void);
static int test_emit_expr_load(void);
static int test_emit_expr_binop(void);
static int test_emit_expr_unop(void);
static int test_emit_expr_field(void);
static int test_instr_nop(void);
static int test_instr_declvar(void);
static int test_instr_declnamed(void);
static int test_instr_assign(void);
static int test_instr_store(void);
static int test_instr_storefield(void);
static int test_instr_ifelse(void);
static int test_instr_while(void);
static int test_instr_block(void);
static int test_instr_break_continue(void);
static int test_instr_call_void(void);
static int test_instr_call_dest(void);
static int test_instr_requires(void);
static int test_instr_ensures(void);
static int test_instr_return_void(void);
static int test_instr_return_val(void);
static int test_instr_return_val_void(void);
static int test_param_list(void);
static int test_fn_proto(void);
static int test_block_defn(void);

static int value_tag(Value v) {
  int _sv0t0;
  if ((v.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((v.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((v.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
        if ((v.tag == 3)) {
          return 3;
          _sv0t0 = 0;
        } else {
          if ((v.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
            if ((v.tag == 5)) {
              return 5;
              _sv0t0 = 0;
            } else {
              if ((v.tag == 6)) {
                return 6;
                _sv0t0 = 0;
              } else {
                if ((v.tag == 7)) {
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
    return 0;
    _sv0t0 = 0;
  } else {
    if ((e.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((e.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
        if ((e.tag == 3)) {
          return 3;
          _sv0t0 = 0;
        } else {
          if ((e.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
          }
        }
      }
    }
  }
  return _sv0t0;
}

static const char* name_of(int h) {
  if ((h == 1)) {
    return "x";
  } else {
  }
  if ((h == 2)) {
    return "y";
  } else {
  }
  if ((h == 3)) {
    return "n";
  } else {
  }
  if ((h == 4)) {
    return "tmp";
  } else {
  }
  if ((h == 5)) {
    return "count";
  } else {
  }
  if ((h == 6)) {
    return "val";
  } else {
  }
  if ((h == 7)) {
    return "hello";
  } else {
  }
  if ((h == 8)) {
    return "data";
  } else {
  }
  if ((h == 9)) {
    return "int";
  } else {
  }
  if ((h == 10)) {
    return "void";
  } else {
  }
  if ((h == 11)) {
    return "main";
  } else {
  }
  if ((h == 12)) {
    return "foo";
  } else {
  }
  if ((h == 13)) {
    return "bar";
  } else {
  }
  if ((h == 14)) {
    return "result";
  } else {
  }
  if ((h == 15)) {
    return "i";
  } else {
  }
  return "?";
}

static const char* op_of(int h) {
  if ((h == 1)) {
    return "+";
  } else {
  }
  if ((h == 2)) {
    return "-";
  } else {
  }
  if ((h == 3)) {
    return "*";
  } else {
  }
  if ((h == 4)) {
    return "==";
  } else {
  }
  if ((h == 5)) {
    return "!=";
  } else {
  }
  if ((h == 6)) {
    return "!";
  } else {
  }
  if ((h == 7)) {
    return "<";
  } else {
  }
  if ((h == 8)) {
    return "&&";
  } else {
  }
  return "?";
}

static const char* digit_char(int d) {
  if ((d == 0)) {
    return "0";
  } else {
  }
  if ((d == 1)) {
    return "1";
  } else {
  }
  if ((d == 2)) {
    return "2";
  } else {
  }
  if ((d == 3)) {
    return "3";
  } else {
  }
  if ((d == 4)) {
    return "4";
  } else {
  }
  if ((d == 5)) {
    return "5";
  } else {
  }
  if ((d == 6)) {
    return "6";
  } else {
  }
  if ((d == 7)) {
    return "7";
  } else {
  }
  if ((d == 8)) {
    return "8";
  } else {
  }
  if ((d == 9)) {
    return "9";
  } else {
  }
  return "?";
}

static const char* int_to_str_pos(int n) {
  if ((n < 10)) {
    const char* _sv0t0 = digit_char(n);
    return _sv0t0;
  } else {
  }
  int rest = (n / 10);
  int d = (n % 10);
  const char* _sv0t1 = int_to_str_pos(rest);
  const char* _sv0t2 = digit_char(d);
  const char* _sv0t3 = sv0_string_concat(_sv0t1, _sv0t2);
  return _sv0t3;
}

static const char* int_to_string(int n) {
  if ((n == 0)) {
    return "0";
  } else {
  }
  if ((n < 0)) {
    int _sv0t0 = (0 - n);
    const char* _sv0t1 = int_to_str_pos(_sv0t0);
    const char* _sv0t2 = sv0_string_concat("-", _sv0t1);
    return _sv0t2;
  } else {
  }
  const char* _sv0t3 = int_to_str_pos(n);
  return _sv0t3;
}

static const char* c_escape_one(int c, const char* src, int pos) {
  if ((c == 34)) {
    return "\\\"";
  } else {
  }
  if ((c == 92)) {
    return "\\\\";
  } else {
  }
  if ((c == 10)) {
    return "\\n";
  } else {
  }
  if ((c == 9)) {
    return "\\t";
  } else {
  }
  if ((c == 13)) {
    return "\\r";
  } else {
  }
  const char* _sv0t0 = sv0_string_substr(src, pos, 1);
  return _sv0t0;
}

static const char* c_escape_from(const char* src, int pos, const char* acc) {
  int _sv0t0 = sv0_string_len(src);
  if ((pos >= _sv0t0)) {
    return acc;
  } else {
  }
  int _sv0t1 = sv0_string_char_at(src, pos);
  int c = _sv0t1;
  const char* _sv0t2 = c_escape_one(c, src, pos);
  const char* esc;
  esc = _sv0t2;
  int _sv0t3 = (pos + 1);
  const char* _sv0t4 = sv0_string_concat(acc, esc);
  const char* _sv0t5 = c_escape_from(src, _sv0t3, _sv0t4);
  return _sv0t5;
}

static const char* c_escape_string(const char* s) {
  const char* _sv0t0 = c_escape_from(s, 0, "");
  return _sv0t0;
}

static const char* emit_value(Value v) {
  int _sv0t0;
  if ((v.tag == 0)) {
    int i = v.p0;
    const char* _sv0t22 = int_to_string(i);
    return _sv0t22;
    _sv0t0 = 0;
  } else {
    if ((v.tag == 1)) {
      return "1";
      _sv0t0 = 0;
    } else {
      if ((v.tag == 2)) {
        return "0";
        _sv0t0 = 0;
      } else {
        if ((v.tag == 3)) {
          int h = v.p0;
          const char* _sv0t21 = name_of(h);
          return _sv0t21;
          _sv0t0 = 0;
        } else {
          if ((v.tag == 4)) {
            return "0";
            _sv0t0 = 0;
          } else {
            if ((v.tag == 5)) {
              int h = v.p0;
              const char* _sv0t17 = name_of(h);
              const char* raw;
              raw = _sv0t17;
              const char* _sv0t18 = c_escape_string(raw);
              const char* escaped;
              escaped = _sv0t18;
              const char* _sv0t19 = sv0_string_concat("\"", escaped);
              const char* a;
              a = _sv0t19;
              const char* _sv0t20 = sv0_string_concat(a, "\"");
              return _sv0t20;
              _sv0t0 = 0;
            } else {
              if ((v.tag == 6)) {
                int bv = v.p0;
                int fh = v.p1;
                Value _sv0t4;
                int _sv0t5 = sv0_box_load(bv, 0);
                _sv0t4.tag = _sv0t5;
                int _sv0t6 = sv0_box_load(bv, 1);
                _sv0t4.p0 = _sv0t6;
                int _sv0t7 = sv0_box_load(bv, 2);
                _sv0t4.p1 = _sv0t7;
                Value inner;
                inner.tag = (_sv0t4).tag;
                inner.p0 = (_sv0t4).p0;
                inner.p1 = (_sv0t4).p1;
                const char* _sv0t8 = name_of(fh);
                const char* field;
                field = _sv0t8;
                int _sv0t9 = value_tag(inner);
                int tag = _sv0t9;
                if ((tag == 3)) {
                  const char* _sv0t10 = emit_value(inner);
                  const char* prefix;
                  prefix = _sv0t10;
                  const char* _sv0t11 = sv0_string_concat(prefix, ".");
                  const char* _sv0t12 = sv0_string_concat(_sv0t11, field);
                  return _sv0t12;
                } else {
                }
                const char* _sv0t13 = emit_value(inner);
                const char* _sv0t14 = sv0_string_concat("(", _sv0t13);
                const char* prefix;
                prefix = _sv0t14;
                const char* _sv0t15 = sv0_string_concat(prefix, ").");
                const char* _sv0t16 = sv0_string_concat(_sv0t15, field);
                return _sv0t16;
                _sv0t0 = 0;
              } else {
                if ((v.tag == 7)) {
                  int h = v.p0;
                  const char* _sv0t1 = name_of(h);
                  const char* _sv0t2 = sv0_string_concat("(&", _sv0t1);
                  const char* _sv0t3 = sv0_string_concat(_sv0t2, ")");
                  return _sv0t3;
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
  return "";
}

static const char* emit_expr(Expr e) {
  int _sv0t0;
  if ((e.tag == 0)) {
    int bv = e.p0;
    Value _sv0t37;
    int _sv0t38 = sv0_box_load(bv, 0);
    _sv0t37.tag = _sv0t38;
    int _sv0t39 = sv0_box_load(bv, 1);
    _sv0t37.p0 = _sv0t39;
    int _sv0t40 = sv0_box_load(bv, 2);
    _sv0t37.p1 = _sv0t40;
    Value v;
    v.tag = (_sv0t37).tag;
    v.p0 = (_sv0t37).p0;
    v.p1 = (_sv0t37).p1;
    const char* _sv0t41 = emit_value(v);
    return _sv0t41;
    _sv0t0 = 0;
  } else {
    if ((e.tag == 1)) {
      int h = e.p0;
      const char* _sv0t36 = name_of(h);
      return _sv0t36;
      _sv0t0 = 0;
    } else {
      if ((e.tag == 2)) {
        int op_h = e.p0;
        int bv1 = e.p1;
        int bv2 = e.p2;
        Value _sv0t19;
        int _sv0t20 = sv0_box_load(bv1, 0);
        _sv0t19.tag = _sv0t20;
        int _sv0t21 = sv0_box_load(bv1, 1);
        _sv0t19.p0 = _sv0t21;
        int _sv0t22 = sv0_box_load(bv1, 2);
        _sv0t19.p1 = _sv0t22;
        Value v1;
        v1.tag = (_sv0t19).tag;
        v1.p0 = (_sv0t19).p0;
        v1.p1 = (_sv0t19).p1;
        Value _sv0t23;
        int _sv0t24 = sv0_box_load(bv2, 0);
        _sv0t23.tag = _sv0t24;
        int _sv0t25 = sv0_box_load(bv2, 1);
        _sv0t23.p0 = _sv0t25;
        int _sv0t26 = sv0_box_load(bv2, 2);
        _sv0t23.p1 = _sv0t26;
        Value v2;
        v2.tag = (_sv0t23).tag;
        v2.p0 = (_sv0t23).p0;
        v2.p1 = (_sv0t23).p1;
        const char* _sv0t27 = emit_value(v1);
        const char* s1;
        s1 = _sv0t27;
        const char* _sv0t28 = emit_value(v2);
        const char* s2;
        s2 = _sv0t28;
        const char* _sv0t29 = op_of(op_h);
        const char* op;
        op = _sv0t29;
        const char* _sv0t30 = sv0_string_concat("(", s1);
        const char* a;
        a = _sv0t30;
        const char* _sv0t31 = sv0_string_concat(a, " ");
        const char* b;
        b = _sv0t31;
        const char* _sv0t32 = sv0_string_concat(b, op);
        const char* c;
        c = _sv0t32;
        const char* _sv0t33 = sv0_string_concat(c, " ");
        const char* d;
        d = _sv0t33;
        const char* _sv0t34 = sv0_string_concat(d, s2);
        const char* f;
        f = _sv0t34;
        const char* _sv0t35 = sv0_string_concat(f, ")");
        return _sv0t35;
        _sv0t0 = 0;
      } else {
        if ((e.tag == 3)) {
          int op_h = e.p0;
          int bv = e.p1;
          Value _sv0t10;
          int _sv0t11 = sv0_box_load(bv, 0);
          _sv0t10.tag = _sv0t11;
          int _sv0t12 = sv0_box_load(bv, 1);
          _sv0t10.p0 = _sv0t12;
          int _sv0t13 = sv0_box_load(bv, 2);
          _sv0t10.p1 = _sv0t13;
          Value v;
          v.tag = (_sv0t10).tag;
          v.p0 = (_sv0t10).p0;
          v.p1 = (_sv0t10).p1;
          const char* _sv0t14 = op_of(op_h);
          const char* op;
          op = _sv0t14;
          const char* _sv0t15 = emit_value(v);
          const char* inner;
          inner = _sv0t15;
          const char* _sv0t16 = sv0_string_concat("(", op);
          const char* a;
          a = _sv0t16;
          const char* _sv0t17 = sv0_string_concat(inner, ")");
          const char* _sv0t18 = sv0_string_concat(a, _sv0t17);
          return _sv0t18;
          _sv0t0 = 0;
        } else {
          if ((e.tag == 4)) {
            int bv = e.p0;
            int fh = e.p1;
            Value _sv0t1;
            int _sv0t2 = sv0_box_load(bv, 0);
            _sv0t1.tag = _sv0t2;
            int _sv0t3 = sv0_box_load(bv, 1);
            _sv0t1.p0 = _sv0t3;
            int _sv0t4 = sv0_box_load(bv, 2);
            _sv0t1.p1 = _sv0t4;
            Value v;
            v.tag = (_sv0t1).tag;
            v.p0 = (_sv0t1).p0;
            v.p1 = (_sv0t1).p1;
            const char* _sv0t5 = emit_value(v);
            const char* _sv0t6 = sv0_string_concat("(", _sv0t5);
            const char* a;
            a = _sv0t6;
            const char* _sv0t7 = sv0_string_concat(a, ").");
            const char* _sv0t8 = name_of(fh);
            const char* _sv0t9 = sv0_string_concat(_sv0t7, _sv0t8);
            return _sv0t9;
            _sv0t0 = 0;
          } else {
          }
        }
      }
    }
  }
  return "";
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
              int b = ins.p0;
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
                          int fh = ins.p0;
                          return 11;
                          _sv0t0 = 0;
                        } else {
                          if ((ins.tag == 12)) {
                            int dh = ins.p0;
                            int fh = ins.p1;
                            int rh = ins.p2;
                            return 12;
                            _sv0t0 = 0;
                          } else {
                            if ((ins.tag == 13)) {
                              int be = ins.p0;
                              int fh = ins.p1;
                              return 13;
                              _sv0t0 = 0;
                            } else {
                              if ((ins.tag == 14)) {
                                int be = ins.p0;
                                int fh = ins.p1;
                                return 14;
                                _sv0t0 = 0;
                              } else {
                                if ((ins.tag == 15)) {
                                  return 15;
                                  _sv0t0 = 0;
                                } else {
                                  if ((ins.tag == 16)) {
                                    int bv = ins.p0;
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

static const char* emit_instr(const char* indent, const char* ret_ty, Instr ins) {
  int _sv0t0;
  if ((ins.tag == 0)) {
    return "";
    _sv0t0 = 0;
  } else {
    if ((ins.tag == 1)) {
      int h = ins.p0;
      const char* _sv0t131 = sv0_string_concat(indent, "int ");
      const char* a;
      a = _sv0t131;
      const char* _sv0t132 = name_of(h);
      const char* _sv0t133 = sv0_string_concat(a, _sv0t132);
      const char* b;
      b = _sv0t133;
      const char* _sv0t134 = sv0_string_concat(b, ";\n");
      return _sv0t134;
      _sv0t0 = 0;
    } else {
      if ((ins.tag == 2)) {
        int ct = ins.p0;
        int h = ins.p1;
        const char* _sv0t125 = name_of(ct);
        const char* _sv0t126 = sv0_string_concat(indent, _sv0t125);
        const char* a;
        a = _sv0t126;
        const char* _sv0t127 = sv0_string_concat(a, " ");
        const char* b;
        b = _sv0t127;
        const char* _sv0t128 = name_of(h);
        const char* _sv0t129 = sv0_string_concat(b, _sv0t128);
        const char* c;
        c = _sv0t129;
        const char* _sv0t130 = sv0_string_concat(c, ";\n");
        return _sv0t130;
        _sv0t0 = 0;
      } else {
        if ((ins.tag == 3)) {
          int h = ins.p0;
          int be = ins.p1;
          Expr _sv0t113;
          int _sv0t114 = sv0_box_load(be, 0);
          _sv0t113.tag = _sv0t114;
          int _sv0t115 = sv0_box_load(be, 1);
          _sv0t113.p0 = _sv0t115;
          int _sv0t116 = sv0_box_load(be, 2);
          _sv0t113.p1 = _sv0t116;
          int _sv0t117 = sv0_box_load(be, 3);
          _sv0t113.p2 = _sv0t117;
          Expr e;
          e.tag = (_sv0t113).tag;
          e.p0 = (_sv0t113).p0;
          e.p1 = (_sv0t113).p1;
          e.p2 = (_sv0t113).p2;
          const char* _sv0t118 = sv0_string_concat(indent, "int ");
          const char* a;
          a = _sv0t118;
          const char* _sv0t119 = name_of(h);
          const char* _sv0t120 = sv0_string_concat(a, _sv0t119);
          const char* b;
          b = _sv0t120;
          const char* _sv0t121 = sv0_string_concat(b, " = ");
          const char* c;
          c = _sv0t121;
          const char* _sv0t122 = emit_expr(e);
          const char* _sv0t123 = sv0_string_concat(c, _sv0t122);
          const char* d;
          d = _sv0t123;
          const char* _sv0t124 = sv0_string_concat(d, ";\n");
          return _sv0t124;
          _sv0t0 = 0;
        } else {
          if ((ins.tag == 4)) {
            int h = ins.p0;
            int be = ins.p1;
            Expr _sv0t102;
            int _sv0t103 = sv0_box_load(be, 0);
            _sv0t102.tag = _sv0t103;
            int _sv0t104 = sv0_box_load(be, 1);
            _sv0t102.p0 = _sv0t104;
            int _sv0t105 = sv0_box_load(be, 2);
            _sv0t102.p1 = _sv0t105;
            int _sv0t106 = sv0_box_load(be, 3);
            _sv0t102.p2 = _sv0t106;
            Expr e;
            e.tag = (_sv0t102).tag;
            e.p0 = (_sv0t102).p0;
            e.p1 = (_sv0t102).p1;
            e.p2 = (_sv0t102).p2;
            const char* _sv0t107 = name_of(h);
            const char* _sv0t108 = sv0_string_concat(indent, _sv0t107);
            const char* a;
            a = _sv0t108;
            const char* _sv0t109 = sv0_string_concat(a, " = ");
            const char* b;
            b = _sv0t109;
            const char* _sv0t110 = emit_expr(e);
            const char* _sv0t111 = sv0_string_concat(b, _sv0t110);
            const char* c;
            c = _sv0t111;
            const char* _sv0t112 = sv0_string_concat(c, ";\n");
            return _sv0t112;
            _sv0t0 = 0;
          } else {
            if ((ins.tag == 5)) {
              int bh = ins.p0;
              int fh = ins.p1;
              int be = ins.p2;
              Expr _sv0t88;
              int _sv0t89 = sv0_box_load(be, 0);
              _sv0t88.tag = _sv0t89;
              int _sv0t90 = sv0_box_load(be, 1);
              _sv0t88.p0 = _sv0t90;
              int _sv0t91 = sv0_box_load(be, 2);
              _sv0t88.p1 = _sv0t91;
              int _sv0t92 = sv0_box_load(be, 3);
              _sv0t88.p2 = _sv0t92;
              Expr e;
              e.tag = (_sv0t88).tag;
              e.p0 = (_sv0t88).p0;
              e.p1 = (_sv0t88).p1;
              e.p2 = (_sv0t88).p2;
              const char* _sv0t93 = name_of(bh);
              const char* _sv0t94 = sv0_string_concat(indent, _sv0t93);
              const char* a;
              a = _sv0t94;
              const char* _sv0t95 = sv0_string_concat(a, ".");
              const char* b;
              b = _sv0t95;
              const char* _sv0t96 = name_of(fh);
              const char* _sv0t97 = sv0_string_concat(b, _sv0t96);
              const char* c;
              c = _sv0t97;
              const char* _sv0t98 = sv0_string_concat(c, " = ");
              const char* d;
              d = _sv0t98;
              const char* _sv0t99 = emit_expr(e);
              const char* _sv0t100 = sv0_string_concat(d, _sv0t99);
              const char* f;
              f = _sv0t100;
              const char* _sv0t101 = sv0_string_concat(f, ";\n");
              return _sv0t101;
              _sv0t0 = 0;
            } else {
              if ((ins.tag == 6)) {
                int be = ins.p0;
                int th = ins.p1;
                int el = ins.p2;
                Expr _sv0t70;
                int _sv0t71 = sv0_box_load(be, 0);
                _sv0t70.tag = _sv0t71;
                int _sv0t72 = sv0_box_load(be, 1);
                _sv0t70.p0 = _sv0t72;
                int _sv0t73 = sv0_box_load(be, 2);
                _sv0t70.p1 = _sv0t73;
                int _sv0t74 = sv0_box_load(be, 3);
                _sv0t70.p2 = _sv0t74;
                Expr e;
                e.tag = (_sv0t70).tag;
                e.p0 = (_sv0t70).p0;
                e.p1 = (_sv0t70).p1;
                e.p2 = (_sv0t70).p2;
                const char* _sv0t75 = sv0_string_concat(indent, "  ");
                const char* deeper;
                deeper = _sv0t75;
                const char* _sv0t76 = sv0_string_concat(indent, "if (");
                const char* a;
                a = _sv0t76;
                const char* _sv0t77 = emit_expr(e);
                const char* _sv0t78 = sv0_string_concat(a, _sv0t77);
                const char* b;
                b = _sv0t78;
                const char* _sv0t79 = sv0_string_concat(b, ") {\n");
                const char* c;
                c = _sv0t79;
                const char* _sv0t80 = emit_instrs(th, deeper, ret_ty);
                const char* _sv0t81 = sv0_string_concat(c, _sv0t80);
                const char* d;
                d = _sv0t81;
                const char* _sv0t82 = sv0_string_concat(d, indent);
                const char* f;
                f = _sv0t82;
                const char* _sv0t83 = sv0_string_concat(f, "} else {\n");
                const char* g;
                g = _sv0t83;
                const char* _sv0t84 = emit_instrs(el, deeper, ret_ty);
                const char* _sv0t85 = sv0_string_concat(g, _sv0t84);
                const char* h;
                h = _sv0t85;
                const char* _sv0t86 = sv0_string_concat(h, indent);
                const char* j;
                j = _sv0t86;
                const char* _sv0t87 = sv0_string_concat(j, "}\n");
                return _sv0t87;
                _sv0t0 = 0;
              } else {
                if ((ins.tag == 7)) {
                  int be = ins.p0;
                  int body = ins.p1;
                  Expr _sv0t56;
                  int _sv0t57 = sv0_box_load(be, 0);
                  _sv0t56.tag = _sv0t57;
                  int _sv0t58 = sv0_box_load(be, 1);
                  _sv0t56.p0 = _sv0t58;
                  int _sv0t59 = sv0_box_load(be, 2);
                  _sv0t56.p1 = _sv0t59;
                  int _sv0t60 = sv0_box_load(be, 3);
                  _sv0t56.p2 = _sv0t60;
                  Expr e;
                  e.tag = (_sv0t56).tag;
                  e.p0 = (_sv0t56).p0;
                  e.p1 = (_sv0t56).p1;
                  e.p2 = (_sv0t56).p2;
                  const char* _sv0t61 = sv0_string_concat(indent, "  ");
                  const char* deeper;
                  deeper = _sv0t61;
                  const char* _sv0t62 = sv0_string_concat(indent, "while (");
                  const char* a;
                  a = _sv0t62;
                  const char* _sv0t63 = emit_expr(e);
                  const char* _sv0t64 = sv0_string_concat(a, _sv0t63);
                  const char* b;
                  b = _sv0t64;
                  const char* _sv0t65 = sv0_string_concat(b, ") {\n");
                  const char* c;
                  c = _sv0t65;
                  const char* _sv0t66 = emit_instrs(body, deeper, ret_ty);
                  const char* _sv0t67 = sv0_string_concat(c, _sv0t66);
                  const char* d;
                  d = _sv0t67;
                  const char* _sv0t68 = sv0_string_concat(d, indent);
                  const char* f;
                  f = _sv0t68;
                  const char* _sv0t69 = sv0_string_concat(f, "}\n");
                  return _sv0t69;
                  _sv0t0 = 0;
                } else {
                  if ((ins.tag == 8)) {
                    int body = ins.p0;
                    const char* _sv0t50 = sv0_string_concat(indent, "  ");
                    const char* deeper;
                    deeper = _sv0t50;
                    const char* _sv0t51 = sv0_string_concat(indent, "{\n");
                    const char* a;
                    a = _sv0t51;
                    const char* _sv0t52 = emit_instrs(body, deeper, ret_ty);
                    const char* _sv0t53 = sv0_string_concat(a, _sv0t52);
                    const char* b;
                    b = _sv0t53;
                    const char* _sv0t54 = sv0_string_concat(b, indent);
                    const char* c;
                    c = _sv0t54;
                    const char* _sv0t55 = sv0_string_concat(c, "}\n");
                    return _sv0t55;
                    _sv0t0 = 0;
                  } else {
                    if ((ins.tag == 9)) {
                      const char* _sv0t49 = sv0_string_concat(indent, "break;\n");
                      return _sv0t49;
                      _sv0t0 = 0;
                    } else {
                      if ((ins.tag == 10)) {
                        const char* _sv0t48 = sv0_string_concat(indent, "continue;\n");
                        return _sv0t48;
                        _sv0t0 = 0;
                      } else {
                        if ((ins.tag == 11)) {
                          int fh = ins.p0;
                          const char* _sv0t45 = name_of(fh);
                          const char* _sv0t46 = sv0_string_concat(indent, _sv0t45);
                          const char* a;
                          a = _sv0t46;
                          const char* _sv0t47 = sv0_string_concat(a, "();\n");
                          return _sv0t47;
                          _sv0t0 = 0;
                        } else {
                          if ((ins.tag == 12)) {
                            int dh = ins.p0;
                            int fh = ins.p1;
                            int rh = ins.p2;
                            const char* _sv0t36 = name_of(rh);
                            const char* _sv0t37 = sv0_string_concat(indent, _sv0t36);
                            const char* a;
                            a = _sv0t37;
                            const char* _sv0t38 = sv0_string_concat(a, " ");
                            const char* b;
                            b = _sv0t38;
                            const char* _sv0t39 = name_of(dh);
                            const char* _sv0t40 = sv0_string_concat(b, _sv0t39);
                            const char* c;
                            c = _sv0t40;
                            const char* _sv0t41 = sv0_string_concat(c, " = ");
                            const char* d;
                            d = _sv0t41;
                            const char* _sv0t42 = name_of(fh);
                            const char* _sv0t43 = sv0_string_concat(d, _sv0t42);
                            const char* f;
                            f = _sv0t43;
                            const char* _sv0t44 = sv0_string_concat(f, "();\n");
                            return _sv0t44;
                            _sv0t0 = 0;
                          } else {
                            if ((ins.tag == 13)) {
                              int be = ins.p0;
                              int fh = ins.p1;
                              Expr _sv0t24;
                              int _sv0t25 = sv0_box_load(be, 0);
                              _sv0t24.tag = _sv0t25;
                              int _sv0t26 = sv0_box_load(be, 1);
                              _sv0t24.p0 = _sv0t26;
                              int _sv0t27 = sv0_box_load(be, 2);
                              _sv0t24.p1 = _sv0t27;
                              int _sv0t28 = sv0_box_load(be, 3);
                              _sv0t24.p2 = _sv0t28;
                              Expr e;
                              e.tag = (_sv0t24).tag;
                              e.p0 = (_sv0t24).p0;
                              e.p1 = (_sv0t24).p1;
                              e.p2 = (_sv0t24).p2;
                              const char* _sv0t29 = sv0_string_concat(indent, "sv0_requires(");
                              const char* a;
                              a = _sv0t29;
                              const char* _sv0t30 = emit_expr(e);
                              const char* _sv0t31 = sv0_string_concat(a, _sv0t30);
                              const char* b;
                              b = _sv0t31;
                              const char* _sv0t32 = sv0_string_concat(b, ", \"");
                              const char* c;
                              c = _sv0t32;
                              const char* _sv0t33 = name_of(fh);
                              const char* _sv0t34 = sv0_string_concat(c, _sv0t33);
                              const char* d;
                              d = _sv0t34;
                              const char* _sv0t35 = sv0_string_concat(d, "\");\n");
                              return _sv0t35;
                              _sv0t0 = 0;
                            } else {
                              if ((ins.tag == 14)) {
                                int be = ins.p0;
                                int fh = ins.p1;
                                Expr _sv0t12;
                                int _sv0t13 = sv0_box_load(be, 0);
                                _sv0t12.tag = _sv0t13;
                                int _sv0t14 = sv0_box_load(be, 1);
                                _sv0t12.p0 = _sv0t14;
                                int _sv0t15 = sv0_box_load(be, 2);
                                _sv0t12.p1 = _sv0t15;
                                int _sv0t16 = sv0_box_load(be, 3);
                                _sv0t12.p2 = _sv0t16;
                                Expr e;
                                e.tag = (_sv0t12).tag;
                                e.p0 = (_sv0t12).p0;
                                e.p1 = (_sv0t12).p1;
                                e.p2 = (_sv0t12).p2;
                                const char* _sv0t17 = sv0_string_concat(indent, "sv0_ensures(");
                                const char* a;
                                a = _sv0t17;
                                const char* _sv0t18 = emit_expr(e);
                                const char* _sv0t19 = sv0_string_concat(a, _sv0t18);
                                const char* b;
                                b = _sv0t19;
                                const char* _sv0t20 = sv0_string_concat(b, ", \"");
                                const char* c;
                                c = _sv0t20;
                                const char* _sv0t21 = name_of(fh);
                                const char* _sv0t22 = sv0_string_concat(c, _sv0t21);
                                const char* d;
                                d = _sv0t22;
                                const char* _sv0t23 = sv0_string_concat(d, "\");\n");
                                return _sv0t23;
                                _sv0t0 = 0;
                              } else {
                                if ((ins.tag == 15)) {
                                  const char* _sv0t11 = sv0_string_concat(indent, "return;\n");
                                  return _sv0t11;
                                  _sv0t0 = 0;
                                } else {
                                  if ((ins.tag == 16)) {
                                    int bv = ins.p0;
                                    int _sv0t1 = sv0_string_eq(ret_ty, "void");
                                    if (_sv0t1) {
                                      const char* _sv0t2 = sv0_string_concat(indent, "return;\n");
                                      return _sv0t2;
                                    } else {
                                    }
                                    Value _sv0t3;
                                    int _sv0t4 = sv0_box_load(bv, 0);
                                    _sv0t3.tag = _sv0t4;
                                    int _sv0t5 = sv0_box_load(bv, 1);
                                    _sv0t3.p0 = _sv0t5;
                                    int _sv0t6 = sv0_box_load(bv, 2);
                                    _sv0t3.p1 = _sv0t6;
                                    Value v;
                                    v.tag = (_sv0t3).tag;
                                    v.p0 = (_sv0t3).p0;
                                    v.p1 = (_sv0t3).p1;
                                    const char* _sv0t7 = sv0_string_concat(indent, "return ");
                                    const char* a;
                                    a = _sv0t7;
                                    const char* _sv0t8 = emit_value(v);
                                    const char* _sv0t9 = sv0_string_concat(a, _sv0t8);
                                    const char* b;
                                    b = _sv0t9;
                                    const char* _sv0t10 = sv0_string_concat(b, ";\n");
                                    return _sv0t10;
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
  return "";
}

static const char* emit_instrs(int instrs, const char* indent, const char* ret_ty) {
  int _sv0t0 = sv0_vec_len(instrs);
  int count = _sv0t0;
  if ((count == 0)) {
    return "";
  } else {
  }
  const char* out;
  out = "";
  int idx = 0;
  while ((idx < count)) {
    int _sv0t1 = sv0_vec_get(instrs, idx);
    int bh = _sv0t1;
    Instr _sv0t2;
    int _sv0t3 = sv0_box_load(bh, 0);
    _sv0t2.tag = _sv0t3;
    int _sv0t4 = sv0_box_load(bh, 1);
    _sv0t2.p0 = _sv0t4;
    int _sv0t5 = sv0_box_load(bh, 2);
    _sv0t2.p1 = _sv0t5;
    int _sv0t6 = sv0_box_load(bh, 3);
    _sv0t2.p2 = _sv0t6;
    Instr ins;
    ins.tag = (_sv0t2).tag;
    ins.p0 = (_sv0t2).p0;
    ins.p1 = (_sv0t2).p1;
    ins.p2 = (_sv0t2).p2;
    const char* _sv0t7 = emit_instr(indent, ret_ty, ins);
    const char* _sv0t8 = sv0_string_concat(out, _sv0t7);
    out = _sv0t8;
    idx = (idx + 1);
  }
  return out;
}

static const char* emit_param_pair(int cty_h, int name_h) {
  const char* _sv0t0 = name_of(cty_h);
  const char* _sv0t1 = sv0_string_concat(_sv0t0, " ");
  const char* a;
  a = _sv0t1;
  const char* _sv0t2 = name_of(name_h);
  const char* _sv0t3 = sv0_string_concat(a, _sv0t2);
  return _sv0t3;
}

static const char* emit_param_list_0(void) {
  return "void";
}

static const char* emit_param_list_1(int cty1, int n1) {
  const char* _sv0t0 = emit_param_pair(cty1, n1);
  return _sv0t0;
}

static const char* emit_param_list_2(int cty1, int n1, int cty2, int n2) {
  const char* _sv0t0 = emit_param_pair(cty1, n1);
  const char* a;
  a = _sv0t0;
  const char* _sv0t1 = sv0_string_concat(a, ", ");
  const char* b;
  b = _sv0t1;
  const char* _sv0t2 = emit_param_pair(cty2, n2);
  const char* _sv0t3 = sv0_string_concat(b, _sv0t2);
  return _sv0t3;
}

static const char* emit_fn_proto(int is_static, int ret_cty_h, int label_h, const char* params_str) {
  const char* _sv0t0 = name_of(ret_cty_h);
  const char* ret;
  ret = _sv0t0;
  const char* _sv0t1 = name_of(label_h);
  const char* label;
  label = _sv0t1;
  if ((is_static == 1)) {
    const char* _sv0t2 = sv0_string_concat("static ", ret);
    const char* a;
    a = _sv0t2;
    const char* _sv0t3 = sv0_string_concat(a, " ");
    const char* b;
    b = _sv0t3;
    const char* _sv0t4 = sv0_string_concat(b, label);
    const char* c;
    c = _sv0t4;
    const char* _sv0t5 = sv0_string_concat(c, "(");
    const char* d;
    d = _sv0t5;
    const char* _sv0t6 = sv0_string_concat(params_str, ")");
    const char* _sv0t7 = sv0_string_concat(d, _sv0t6);
    return _sv0t7;
  } else {
  }
  const char* _sv0t8 = sv0_string_concat(ret, " ");
  const char* a;
  a = _sv0t8;
  const char* _sv0t9 = sv0_string_concat(a, label);
  const char* b;
  b = _sv0t9;
  const char* _sv0t10 = sv0_string_concat(b, "(");
  const char* c;
  c = _sv0t10;
  const char* _sv0t11 = sv0_string_concat(params_str, ")");
  const char* _sv0t12 = sv0_string_concat(c, _sv0t11);
  return _sv0t12;
}

static const char* emit_block_defn(const char* proto, const char* body) {
  const char* _sv0t0 = sv0_string_concat(proto, " {\n");
  const char* a;
  a = _sv0t0;
  const char* _sv0t1 = sv0_string_concat(a, body);
  const char* b;
  b = _sv0t1;
  const char* _sv0t2 = sv0_string_concat(b, "}\n\n");
  return _sv0t2;
}

static int test_int_format(void) {
  const char* _sv0t0 = int_to_string(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "0");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = int_to_string(1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "1");
  if ((!_sv0t3)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = int_to_string(42);
  int _sv0t5 = sv0_string_eq(_sv0t4, "42");
  if ((!_sv0t5)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = int_to_string(100);
  int _sv0t7 = sv0_string_eq(_sv0t6, "100");
  if ((!_sv0t7)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = int_to_string(9999);
  int _sv0t9 = sv0_string_eq(_sv0t8, "9999");
  if ((!_sv0t9)) {
    return 5;
  } else {
  }
  int _sv0t10 = (-1);
  const char* _sv0t11 = int_to_string(_sv0t10);
  int _sv0t12 = sv0_string_eq(_sv0t11, "-1");
  if ((!_sv0t12)) {
    return 6;
  } else {
  }
  int _sv0t13 = (-42);
  const char* _sv0t14 = int_to_string(_sv0t13);
  int _sv0t15 = sv0_string_eq(_sv0t14, "-42");
  if ((!_sv0t15)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_escape(void) {
  const char* _sv0t0 = c_escape_string("hello");
  int _sv0t1 = sv0_string_eq(_sv0t0, "hello");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = c_escape_string("");
  int _sv0t3 = sv0_string_eq(_sv0t2, "");
  if ((!_sv0t3)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = c_escape_string("ab");
  int _sv0t5 = sv0_string_eq(_sv0t4, "ab");
  if ((!_sv0t5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_emit_value_basic(void) {
  Value v1;
  v1.tag = 0;
  v1.p0 = 42;
  const char* _sv0t0 = emit_value(v1);
  int _sv0t1 = sv0_string_eq(_sv0t0, "42");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  Value v2;
  v2.tag = 1;
  const char* _sv0t2 = emit_value(v2);
  int _sv0t3 = sv0_string_eq(_sv0t2, "1");
  if ((!_sv0t3)) {
    return 2;
  } else {
  }
  Value v3;
  v3.tag = 2;
  const char* _sv0t4 = emit_value(v3);
  int _sv0t5 = sv0_string_eq(_sv0t4, "0");
  if ((!_sv0t5)) {
    return 3;
  } else {
  }
  Value v4;
  v4.tag = 3;
  v4.p0 = 1;
  const char* _sv0t6 = emit_value(v4);
  int _sv0t7 = sv0_string_eq(_sv0t6, "x");
  if ((!_sv0t7)) {
    return 4;
  } else {
  }
  Value v5;
  v5.tag = 4;
  const char* _sv0t8 = emit_value(v5);
  int _sv0t9 = sv0_string_eq(_sv0t8, "0");
  if ((!_sv0t9)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_emit_value_string(void) {
  Value v;
  v.tag = 5;
  v.p0 = 7;
  const char* _sv0t0 = emit_value(v);
  const char* s;
  s = _sv0t0;
  int _sv0t1 = sv0_string_eq(s, "\"hello\"");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_value_addr(void) {
  Value v;
  v.tag = 7;
  v.p0 = 1;
  const char* _sv0t0 = emit_value(v);
  int _sv0t1 = sv0_string_eq(_sv0t0, "(&x)");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_member(void) {
  Value inner;
  inner.tag = 3;
  inner.p0 = 1;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, inner.tag);
  sv0_box_store(_sv0t0, 1, inner.p0);
  sv0_box_store(_sv0t0, 2, inner.p1);
  int bv = _sv0t0;
  Value v;
  v.tag = 6;
  v.p0 = bv;
  v.p1 = 2;
  const char* _sv0t1 = emit_value(v);
  int _sv0t2 = sv0_string_eq(_sv0t1, "x.y");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_expr_lit(void) {
  Value v;
  v.tag = 0;
  v.p0 = 7;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Expr e;
  e.tag = 0;
  e.p0 = bv;
  const char* _sv0t1 = emit_expr(e);
  int _sv0t2 = sv0_string_eq(_sv0t1, "7");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_expr_load(void) {
  Expr e;
  e.tag = 1;
  e.p0 = 1;
  const char* _sv0t0 = emit_expr(e);
  int _sv0t1 = sv0_string_eq(_sv0t0, "x");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_expr_binop(void) {
  Value v1;
  v1.tag = 3;
  v1.p0 = 1;
  Value v2;
  v2.tag = 3;
  v2.p0 = 2;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v1.tag);
  sv0_box_store(_sv0t0, 1, v1.p0);
  sv0_box_store(_sv0t0, 2, v1.p1);
  int bv1 = _sv0t0;
  int _sv0t1 = sv0_box_alloc(3);
  sv0_box_store(_sv0t1, 0, v2.tag);
  sv0_box_store(_sv0t1, 1, v2.p0);
  sv0_box_store(_sv0t1, 2, v2.p1);
  int bv2 = _sv0t1;
  Expr e;
  e.tag = 2;
  e.p0 = 1;
  e.p1 = bv1;
  e.p2 = bv2;
  const char* _sv0t2 = emit_expr(e);
  int _sv0t3 = sv0_string_eq(_sv0t2, "(x + y)");
  if ((!_sv0t3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_expr_unop(void) {
  Value v;
  v.tag = 3;
  v.p0 = 1;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Expr e;
  e.tag = 3;
  e.p0 = 6;
  e.p1 = bv;
  const char* _sv0t1 = emit_expr(e);
  int _sv0t2 = sv0_string_eq(_sv0t1, "(!x)");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_expr_field(void) {
  Value v;
  v.tag = 3;
  v.p0 = 1;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Expr e;
  e.tag = 4;
  e.p0 = bv;
  e.p1 = 2;
  const char* _sv0t1 = emit_expr(e);
  int _sv0t2 = sv0_string_eq(_sv0t1, "(x).y");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_nop(void) {
  Instr ins;
  ins.tag = 0;
  int _sv0t0 = instr_tag(ins);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  const char* _sv0t1 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = sv0_string_eq(s, "");
  if ((!_sv0t2)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_instr_declvar(void) {
  Instr ins;
  ins.tag = 1;
  ins.p0 = 1;
  int _sv0t0 = instr_tag(ins);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t1 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = sv0_string_eq(s, "  int x;\n");
  if ((!_sv0t2)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_instr_declnamed(void) {
  Instr ins;
  ins.tag = 2;
  ins.p0 = 9;
  ins.p1 = 4;
  const char* _sv0t0 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t0;
  int _sv0t1 = sv0_string_eq(s, "  int tmp;\n");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_assign(void) {
  Value v;
  v.tag = 0;
  v.p0 = 42;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Expr e;
  e.tag = 0;
  e.p0 = bv;
  int _sv0t1 = sv0_box_alloc(4);
  sv0_box_store(_sv0t1, 0, e.tag);
  sv0_box_store(_sv0t1, 1, e.p0);
  sv0_box_store(_sv0t1, 2, e.p1);
  sv0_box_store(_sv0t1, 3, e.p2);
  int be = _sv0t1;
  Instr ins;
  ins.tag = 3;
  ins.p0 = 1;
  ins.p1 = be;
  const char* _sv0t2 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t2;
  int _sv0t3 = sv0_string_eq(s, "  int x = 42;\n");
  if ((!_sv0t3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_store(void) {
  Value v;
  v.tag = 3;
  v.p0 = 2;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Expr e;
  e.tag = 0;
  e.p0 = bv;
  int _sv0t1 = sv0_box_alloc(4);
  sv0_box_store(_sv0t1, 0, e.tag);
  sv0_box_store(_sv0t1, 1, e.p0);
  sv0_box_store(_sv0t1, 2, e.p1);
  sv0_box_store(_sv0t1, 3, e.p2);
  int be = _sv0t1;
  Instr ins;
  ins.tag = 4;
  ins.p0 = 1;
  ins.p1 = be;
  const char* _sv0t2 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t2;
  int _sv0t3 = sv0_string_eq(s, "  x = y;\n");
  if ((!_sv0t3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_storefield(void) {
  Value v;
  v.tag = 0;
  v.p0 = 10;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Expr e;
  e.tag = 0;
  e.p0 = bv;
  int _sv0t1 = sv0_box_alloc(4);
  sv0_box_store(_sv0t1, 0, e.tag);
  sv0_box_store(_sv0t1, 1, e.p0);
  sv0_box_store(_sv0t1, 2, e.p1);
  sv0_box_store(_sv0t1, 3, e.p2);
  int be = _sv0t1;
  Instr ins;
  ins.tag = 5;
  ins.p0 = 1;
  ins.p1 = 2;
  ins.p2 = be;
  const char* _sv0t2 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t2;
  int _sv0t3 = sv0_string_eq(s, "  x.y = 10;\n");
  if ((!_sv0t3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_ifelse(void) {
  Value cv;
  cv.tag = 3;
  cv.p0 = 1;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, cv.tag);
  sv0_box_store(_sv0t0, 1, cv.p0);
  sv0_box_store(_sv0t0, 2, cv.p1);
  int cbv = _sv0t0;
  Expr ce;
  ce.tag = 0;
  ce.p0 = cbv;
  int _sv0t1 = sv0_box_alloc(4);
  sv0_box_store(_sv0t1, 0, ce.tag);
  sv0_box_store(_sv0t1, 1, ce.p0);
  sv0_box_store(_sv0t1, 2, ce.p1);
  sv0_box_store(_sv0t1, 3, ce.p2);
  int cbe = _sv0t1;
  Instr then_i;
  Value _sv0t2;
  _sv0t2.tag = 0;
  _sv0t2.p0 = 1;
  int _sv0t3 = sv0_box_alloc(3);
  sv0_box_store(_sv0t3, 0, _sv0t2.tag);
  sv0_box_store(_sv0t3, 1, _sv0t2.p0);
  sv0_box_store(_sv0t3, 2, _sv0t2.p1);
  then_i.tag = 16;
  then_i.p0 = _sv0t3;
  Instr else_i;
  Value _sv0t4;
  _sv0t4.tag = 0;
  _sv0t4.p0 = 0;
  int _sv0t5 = sv0_box_alloc(3);
  sv0_box_store(_sv0t5, 0, _sv0t4.tag);
  sv0_box_store(_sv0t5, 1, _sv0t4.p0);
  sv0_box_store(_sv0t5, 2, _sv0t4.p1);
  else_i.tag = 16;
  else_i.p0 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int th = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int el = _sv0t7;
  int _sv0t8 = sv0_box_alloc(4);
  sv0_box_store(_sv0t8, 0, then_i.tag);
  sv0_box_store(_sv0t8, 1, then_i.p0);
  sv0_box_store(_sv0t8, 2, then_i.p1);
  sv0_box_store(_sv0t8, 3, then_i.p2);
  sv0_vec_push(th, _sv0t8);
  int _sv0t9 = sv0_box_alloc(4);
  sv0_box_store(_sv0t9, 0, else_i.tag);
  sv0_box_store(_sv0t9, 1, else_i.p0);
  sv0_box_store(_sv0t9, 2, else_i.p1);
  sv0_box_store(_sv0t9, 3, else_i.p2);
  sv0_vec_push(el, _sv0t9);
  Instr ins;
  ins.tag = 6;
  ins.p0 = cbe;
  ins.p1 = th;
  ins.p2 = el;
  const char* _sv0t10 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t10;
  const char* expected;
  expected = "  if (x) {\n    return 1;\n  } else {\n    return 0;\n  }\n";
  int _sv0t11 = sv0_string_eq(s, expected);
  if ((!_sv0t11)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_while(void) {
  Value cv;
  cv.tag = 3;
  cv.p0 = 1;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, cv.tag);
  sv0_box_store(_sv0t0, 1, cv.p0);
  sv0_box_store(_sv0t0, 2, cv.p1);
  int cbv = _sv0t0;
  Expr ce;
  ce.tag = 0;
  ce.p0 = cbv;
  int _sv0t1 = sv0_box_alloc(4);
  sv0_box_store(_sv0t1, 0, ce.tag);
  sv0_box_store(_sv0t1, 1, ce.p0);
  sv0_box_store(_sv0t1, 2, ce.p1);
  sv0_box_store(_sv0t1, 3, ce.p2);
  int cbe = _sv0t1;
  Instr body_i;
  body_i.tag = 9;
  int _sv0t2 = sv0_vec_new();
  int body = _sv0t2;
  int _sv0t3 = sv0_box_alloc(4);
  sv0_box_store(_sv0t3, 0, body_i.tag);
  sv0_box_store(_sv0t3, 1, body_i.p0);
  sv0_box_store(_sv0t3, 2, body_i.p1);
  sv0_box_store(_sv0t3, 3, body_i.p2);
  sv0_vec_push(body, _sv0t3);
  Instr ins;
  ins.tag = 7;
  ins.p0 = cbe;
  ins.p1 = body;
  const char* _sv0t4 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t4;
  const char* expected;
  expected = "  while (x) {\n    break;\n  }\n";
  int _sv0t5 = sv0_string_eq(s, expected);
  if ((!_sv0t5)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_block(void) {
  Instr i1;
  i1.tag = 1;
  i1.p0 = 1;
  Instr i2;
  i2.tag = 1;
  i2.p0 = 2;
  int _sv0t0 = sv0_vec_new();
  int body = _sv0t0;
  int _sv0t1 = sv0_box_alloc(4);
  sv0_box_store(_sv0t1, 0, i1.tag);
  sv0_box_store(_sv0t1, 1, i1.p0);
  sv0_box_store(_sv0t1, 2, i1.p1);
  sv0_box_store(_sv0t1, 3, i1.p2);
  sv0_vec_push(body, _sv0t1);
  int _sv0t2 = sv0_box_alloc(4);
  sv0_box_store(_sv0t2, 0, i2.tag);
  sv0_box_store(_sv0t2, 1, i2.p0);
  sv0_box_store(_sv0t2, 2, i2.p1);
  sv0_box_store(_sv0t2, 3, i2.p2);
  sv0_vec_push(body, _sv0t2);
  Instr ins;
  ins.tag = 8;
  ins.p0 = body;
  const char* _sv0t3 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t3;
  const char* expected;
  expected = "  {\n    int x;\n    int y;\n  }\n";
  int _sv0t4 = sv0_string_eq(s, expected);
  if ((!_sv0t4)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_break_continue(void) {
  Instr b;
  b.tag = 9;
  const char* _sv0t0 = emit_instr("  ", "int", b);
  int _sv0t1 = sv0_string_eq(_sv0t0, "  break;\n");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  Instr c;
  c.tag = 10;
  const char* _sv0t2 = emit_instr("  ", "int", c);
  int _sv0t3 = sv0_string_eq(_sv0t2, "  continue;\n");
  if ((!_sv0t3)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_instr_call_void(void) {
  Instr ins;
  ins.tag = 11;
  ins.p0 = 12;
  const char* _sv0t0 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t0;
  int _sv0t1 = sv0_string_eq(s, "  foo();\n");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_call_dest(void) {
  Instr ins;
  ins.tag = 12;
  ins.p0 = 14;
  ins.p1 = 12;
  ins.p2 = 9;
  const char* _sv0t0 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t0;
  int _sv0t1 = sv0_string_eq(s, "  int result = foo();\n");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_requires(void) {
  Value v;
  v.tag = 3;
  v.p0 = 1;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Expr e;
  e.tag = 0;
  e.p0 = bv;
  int _sv0t1 = sv0_box_alloc(4);
  sv0_box_store(_sv0t1, 0, e.tag);
  sv0_box_store(_sv0t1, 1, e.p0);
  sv0_box_store(_sv0t1, 2, e.p1);
  sv0_box_store(_sv0t1, 3, e.p2);
  int be = _sv0t1;
  Instr ins;
  ins.tag = 13;
  ins.p0 = be;
  ins.p1 = 12;
  const char* _sv0t2 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t2;
  int _sv0t3 = sv0_string_eq(s, "  sv0_requires(x, \"foo\");\n");
  if ((!_sv0t3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_ensures(void) {
  Value v;
  v.tag = 3;
  v.p0 = 2;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Expr e;
  e.tag = 0;
  e.p0 = bv;
  int _sv0t1 = sv0_box_alloc(4);
  sv0_box_store(_sv0t1, 0, e.tag);
  sv0_box_store(_sv0t1, 1, e.p0);
  sv0_box_store(_sv0t1, 2, e.p1);
  sv0_box_store(_sv0t1, 3, e.p2);
  int be = _sv0t1;
  Instr ins;
  ins.tag = 14;
  ins.p0 = be;
  ins.p1 = 13;
  const char* _sv0t2 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t2;
  int _sv0t3 = sv0_string_eq(s, "  sv0_ensures(y, \"bar\");\n");
  if ((!_sv0t3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_return_void(void) {
  Instr ins;
  ins.tag = 15;
  const char* _sv0t0 = emit_instr("  ", "void", ins);
  const char* s;
  s = _sv0t0;
  int _sv0t1 = sv0_string_eq(s, "  return;\n");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_return_val(void) {
  Value v;
  v.tag = 0;
  v.p0 = 0;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Instr ins;
  ins.tag = 16;
  ins.p0 = bv;
  const char* _sv0t1 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = sv0_string_eq(s, "  return 0;\n");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_return_val_void(void) {
  Value v;
  v.tag = 0;
  v.p0 = 0;
  int _sv0t0 = sv0_box_alloc(3);
  sv0_box_store(_sv0t0, 0, v.tag);
  sv0_box_store(_sv0t0, 1, v.p0);
  sv0_box_store(_sv0t0, 2, v.p1);
  int bv = _sv0t0;
  Instr ins;
  ins.tag = 16;
  ins.p0 = bv;
  const char* _sv0t1 = emit_instr("  ", "void", ins);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = sv0_string_eq(s, "  return;\n");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_param_list(void) {
  const char* _sv0t0 = emit_param_list_0();
  int _sv0t1 = sv0_string_eq(_sv0t0, "void");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = emit_param_list_1(9, 1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "int x");
  if ((!_sv0t3)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = emit_param_list_2(9, 1, 9, 2);
  int _sv0t5 = sv0_string_eq(_sv0t4, "int x, int y");
  if ((!_sv0t5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_fn_proto(void) {
  const char* _sv0t0 = emit_fn_proto(1, 9, 12, "void");
  const char* s1;
  s1 = _sv0t0;
  int _sv0t1 = sv0_string_eq(s1, "static int foo(void)");
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = emit_fn_proto(0, 9, 11, "int x, int y");
  const char* s2;
  s2 = _sv0t2;
  int _sv0t3 = sv0_string_eq(s2, "int main(int x, int y)");
  if ((!_sv0t3)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = emit_fn_proto(1, 10, 13, "int n");
  const char* s3;
  s3 = _sv0t4;
  int _sv0t5 = sv0_string_eq(s3, "static void bar(int n)");
  if ((!_sv0t5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_block_defn(void) {
  const char* _sv0t0 = emit_fn_proto(0, 9, 11, "void");
  const char* proto;
  proto = _sv0t0;
  const char* body;
  body = "  return 0;\n";
  const char* _sv0t1 = emit_block_defn(proto, body);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = sv0_string_eq(s, "int main(void) {\n  return 0;\n}\n\n");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_int_format();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  sv0_println("PASS test_int_format");
  int _sv0t1 = test_escape();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  sv0_println("PASS test_escape");
  int _sv0t3 = test_emit_value_basic();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  sv0_println("PASS test_emit_value_basic");
  int _sv0t5 = test_emit_value_string();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  sv0_println("PASS test_emit_value_string");
  int _sv0t7 = test_emit_value_addr();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  sv0_println("PASS test_emit_value_addr");
  int _sv0t9 = test_emit_member();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  sv0_println("PASS test_emit_member");
  int _sv0t11 = test_emit_expr_lit();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (60 + r7);
    return _sv0t12;
  } else {
  }
  sv0_println("PASS test_emit_expr_lit");
  int _sv0t13 = test_emit_expr_load();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (70 + r8);
    return _sv0t14;
  } else {
  }
  sv0_println("PASS test_emit_expr_load");
  int _sv0t15 = test_emit_expr_binop();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (80 + r9);
    return _sv0t16;
  } else {
  }
  sv0_println("PASS test_emit_expr_binop");
  int _sv0t17 = test_emit_expr_unop();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (90 + r10);
    return _sv0t18;
  } else {
  }
  sv0_println("PASS test_emit_expr_unop");
  int _sv0t19 = test_emit_expr_field();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (100 + r11);
    return _sv0t20;
  } else {
  }
  sv0_println("PASS test_emit_expr_field");
  int _sv0t21 = test_instr_nop();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (110 + r12);
    return _sv0t22;
  } else {
  }
  sv0_println("PASS test_instr_nop");
  int _sv0t23 = test_instr_declvar();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (120 + r13);
    return _sv0t24;
  } else {
  }
  sv0_println("PASS test_instr_declvar");
  int _sv0t25 = test_instr_declnamed();
  int r14 = _sv0t25;
  if ((r14 != 0)) {
    int _sv0t26 = (130 + r14);
    return _sv0t26;
  } else {
  }
  sv0_println("PASS test_instr_declnamed");
  int _sv0t27 = test_instr_assign();
  int r15 = _sv0t27;
  if ((r15 != 0)) {
    int _sv0t28 = (140 + r15);
    return _sv0t28;
  } else {
  }
  sv0_println("PASS test_instr_assign");
  int _sv0t29 = test_instr_store();
  int r16 = _sv0t29;
  if ((r16 != 0)) {
    int _sv0t30 = (150 + r16);
    return _sv0t30;
  } else {
  }
  sv0_println("PASS test_instr_store");
  int _sv0t31 = test_instr_storefield();
  int r17 = _sv0t31;
  if ((r17 != 0)) {
    int _sv0t32 = (160 + r17);
    return _sv0t32;
  } else {
  }
  sv0_println("PASS test_instr_storefield");
  int _sv0t33 = test_instr_ifelse();
  int r18a = _sv0t33;
  if ((r18a != 0)) {
    int _sv0t34 = (170 + r18a);
    return _sv0t34;
  } else {
  }
  sv0_println("PASS test_instr_ifelse");
  int _sv0t35 = test_instr_while();
  int r18b = _sv0t35;
  if ((r18b != 0)) {
    int _sv0t36 = (175 + r18b);
    return _sv0t36;
  } else {
  }
  sv0_println("PASS test_instr_while");
  int _sv0t37 = test_instr_block();
  int r18c = _sv0t37;
  if ((r18c != 0)) {
    int _sv0t38 = (178 + r18c);
    return _sv0t38;
  } else {
  }
  sv0_println("PASS test_instr_block");
  int _sv0t39 = test_instr_break_continue();
  int r18 = _sv0t39;
  if ((r18 != 0)) {
    int _sv0t40 = (180 + r18);
    return _sv0t40;
  } else {
  }
  sv0_println("PASS test_instr_break_continue");
  int _sv0t41 = test_instr_call_void();
  int r19 = _sv0t41;
  if ((r19 != 0)) {
    int _sv0t42 = (180 + r19);
    return _sv0t42;
  } else {
  }
  sv0_println("PASS test_instr_call_void");
  int _sv0t43 = test_instr_call_dest();
  int r20 = _sv0t43;
  if ((r20 != 0)) {
    int _sv0t44 = (190 + r20);
    return _sv0t44;
  } else {
  }
  sv0_println("PASS test_instr_call_dest");
  int _sv0t45 = test_instr_requires();
  int r21 = _sv0t45;
  if ((r21 != 0)) {
    int _sv0t46 = (200 + r21);
    return _sv0t46;
  } else {
  }
  sv0_println("PASS test_instr_requires");
  int _sv0t47 = test_instr_ensures();
  int r22 = _sv0t47;
  if ((r22 != 0)) {
    int _sv0t48 = (210 + r22);
    return _sv0t48;
  } else {
  }
  sv0_println("PASS test_instr_ensures");
  int _sv0t49 = test_instr_return_void();
  int r23 = _sv0t49;
  if ((r23 != 0)) {
    int _sv0t50 = (220 + r23);
    return _sv0t50;
  } else {
  }
  sv0_println("PASS test_instr_return_void");
  int _sv0t51 = test_instr_return_val();
  int r24 = _sv0t51;
  if ((r24 != 0)) {
    int _sv0t52 = (230 + r24);
    return _sv0t52;
  } else {
  }
  sv0_println("PASS test_instr_return_val");
  int _sv0t53 = test_instr_return_val_void();
  int r25 = _sv0t53;
  if ((r25 != 0)) {
    int _sv0t54 = (240 + r25);
    return _sv0t54;
  } else {
  }
  sv0_println("PASS test_instr_return_val_void");
  int _sv0t55 = test_param_list();
  int r26 = _sv0t55;
  if ((r26 != 0)) {
    int _sv0t56 = (250 + r26);
    return _sv0t56;
  } else {
  }
  sv0_println("PASS test_param_list");
  int _sv0t57 = test_fn_proto();
  int r27 = _sv0t57;
  if ((r27 != 0)) {
    int _sv0t58 = (260 + r27);
    return _sv0t58;
  } else {
  }
  sv0_println("PASS test_fn_proto");
  int _sv0t59 = test_block_defn();
  int r28 = _sv0t59;
  if ((r28 != 0)) {
    int _sv0t60 = (270 + r28);
    return _sv0t60;
  } else {
  }
  sv0_println("PASS test_block_defn");
  return 0;
}

