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
  int p3;
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
static const char* emit_call_args(int args);
static const char* emit_call_void(const char* indent, int fn_h, int args);
static const char* emit_call_dest(const char* indent, int dst_h, int fn_h, int args, int rt_h);
static const char* emit_instr(const char* indent, const char* ret_ty, Instr ins);
static const char* emit_instrs(int instrs, const char* indent, const char* ret_ty);
static const char* emit_param_pair(int cty_h, int name_h);
static const char* emit_param_list_0(void);
static const char* emit_param_list_1(int cty1, int n1);
static const char* emit_param_list_2(int cty1, int n1, int cty2, int n2);
static const char* emit_fn_proto(int is_static, int ret_cty_h, int label_h, const char* params_str);
static const char* emit_block_defn(const char* proto, const char* body);
static const char* emit_param_list(int params);
static const char* emit_fn_proto_ex(int is_static, int ret_cty_h, int label_h, int params);
static const char* emit_program(const char* typedefs, int blocks);
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
static int test_param_list_general(void);
static int test_emit_program_empty(void);
static int test_emit_program_main_only(void);
static int test_emit_program_with_helper(void);
static int test_emit_program_typedefs(void);
static int test_emit_program_no_main(void);
static int test_fn_proto_ex(void);
static int test_call_args_empty(void);
static int test_call_args_one(void);
static int test_call_args_two(void);
static int test_call_args_three(void);
static int test_instr_call_with_args(void);
static int test_instr_call_void_with_args(void);
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
                          int dh = ins.p0;
                          int fh = ins.p1;
                          int args = ins.p2;
                          int rh = ins.p3;
                          return 11;
                          _sv0t0 = 0;
                        } else {
                          if ((ins.tag == 12)) {
                            int be = ins.p0;
                            int fh = ins.p1;
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
                                return 14;
                                _sv0t0 = 0;
                              } else {
                                if ((ins.tag == 15)) {
                                  int bv = ins.p0;
                                  return 15;
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
  return _sv0t0;
}

static const char* emit_call_args(int args) {
  int _sv0t0 = sv0_vec_len(args);
  int count = _sv0t0;
  if ((count == 0)) {
    return "";
  } else {
  }
  const char* out;
  out = "";
  int idx = 0;
  while ((idx < count)) {
    if ((idx > 0)) {
      const char* _sv0t1 = sv0_string_concat(out, ", ");
      out = _sv0t1;
    } else {
    }
    int _sv0t2 = sv0_vec_get(args, idx);
    int bv = _sv0t2;
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
    const char* _sv0t7 = emit_value(v);
    const char* _sv0t8 = sv0_string_concat(out, _sv0t7);
    out = _sv0t8;
    idx = (idx + 1);
  }
  return out;
}

static const char* emit_call_void(const char* indent, int fn_h, int args) {
  const char* _sv0t0 = emit_call_args(args);
  const char* arg_str;
  arg_str = _sv0t0;
  const char* _sv0t1 = name_of(fn_h);
  const char* _sv0t2 = sv0_string_concat(indent, _sv0t1);
  const char* a;
  a = _sv0t2;
  const char* _sv0t3 = sv0_string_concat(a, "(");
  const char* b;
  b = _sv0t3;
  const char* _sv0t4 = sv0_string_concat(b, arg_str);
  const char* c;
  c = _sv0t4;
  const char* _sv0t5 = sv0_string_concat(c, ");\n");
  return _sv0t5;
}

static const char* emit_call_dest(const char* indent, int dst_h, int fn_h, int args, int rt_h) {
  const char* _sv0t0 = emit_call_args(args);
  const char* arg_str;
  arg_str = _sv0t0;
  const char* _sv0t1 = name_of(fn_h);
  const char* fn_name;
  fn_name = _sv0t1;
  const char* _sv0t2 = name_of(rt_h);
  const char* ret_name;
  ret_name = _sv0t2;
  const char* _sv0t3 = sv0_string_concat(indent, ret_name);
  const char* a;
  a = _sv0t3;
  const char* _sv0t4 = sv0_string_concat(a, " ");
  const char* b;
  b = _sv0t4;
  const char* _sv0t5 = name_of(dst_h);
  const char* _sv0t6 = sv0_string_concat(b, _sv0t5);
  const char* c;
  c = _sv0t6;
  const char* _sv0t7 = sv0_string_concat(c, " = ");
  const char* d;
  d = _sv0t7;
  const char* _sv0t8 = sv0_string_concat(d, fn_name);
  const char* e;
  e = _sv0t8;
  const char* _sv0t9 = sv0_string_concat(e, "(");
  const char* f;
  f = _sv0t9;
  const char* _sv0t10 = sv0_string_concat(f, arg_str);
  const char* g;
  g = _sv0t10;
  const char* _sv0t11 = sv0_string_concat(g, ");\n");
  return _sv0t11;
}

static const char* emit_instr(const char* indent, const char* ret_ty, Instr ins) {
  int _sv0t0;
  if ((ins.tag == 0)) {
    return "";
    _sv0t0 = 0;
  } else {
    if ((ins.tag == 1)) {
      int h = ins.p0;
      const char* _sv0t121 = sv0_string_concat(indent, "int ");
      const char* a;
      a = _sv0t121;
      const char* _sv0t122 = name_of(h);
      const char* _sv0t123 = sv0_string_concat(a, _sv0t122);
      const char* b;
      b = _sv0t123;
      const char* _sv0t124 = sv0_string_concat(b, ";\n");
      return _sv0t124;
      _sv0t0 = 0;
    } else {
      if ((ins.tag == 2)) {
        int ct = ins.p0;
        int h = ins.p1;
        const char* _sv0t115 = name_of(ct);
        const char* _sv0t116 = sv0_string_concat(indent, _sv0t115);
        const char* a;
        a = _sv0t116;
        const char* _sv0t117 = sv0_string_concat(a, " ");
        const char* b;
        b = _sv0t117;
        const char* _sv0t118 = name_of(h);
        const char* _sv0t119 = sv0_string_concat(b, _sv0t118);
        const char* c;
        c = _sv0t119;
        const char* _sv0t120 = sv0_string_concat(c, ";\n");
        return _sv0t120;
        _sv0t0 = 0;
      } else {
        if ((ins.tag == 3)) {
          int h = ins.p0;
          int be = ins.p1;
          Expr _sv0t103;
          int _sv0t104 = sv0_box_load(be, 0);
          _sv0t103.tag = _sv0t104;
          int _sv0t105 = sv0_box_load(be, 1);
          _sv0t103.p0 = _sv0t105;
          int _sv0t106 = sv0_box_load(be, 2);
          _sv0t103.p1 = _sv0t106;
          int _sv0t107 = sv0_box_load(be, 3);
          _sv0t103.p2 = _sv0t107;
          Expr e;
          e.tag = (_sv0t103).tag;
          e.p0 = (_sv0t103).p0;
          e.p1 = (_sv0t103).p1;
          e.p2 = (_sv0t103).p2;
          const char* _sv0t108 = sv0_string_concat(indent, "int ");
          const char* a;
          a = _sv0t108;
          const char* _sv0t109 = name_of(h);
          const char* _sv0t110 = sv0_string_concat(a, _sv0t109);
          const char* b;
          b = _sv0t110;
          const char* _sv0t111 = sv0_string_concat(b, " = ");
          const char* c;
          c = _sv0t111;
          const char* _sv0t112 = emit_expr(e);
          const char* _sv0t113 = sv0_string_concat(c, _sv0t112);
          const char* d;
          d = _sv0t113;
          const char* _sv0t114 = sv0_string_concat(d, ";\n");
          return _sv0t114;
          _sv0t0 = 0;
        } else {
          if ((ins.tag == 4)) {
            int h = ins.p0;
            int be = ins.p1;
            Expr _sv0t92;
            int _sv0t93 = sv0_box_load(be, 0);
            _sv0t92.tag = _sv0t93;
            int _sv0t94 = sv0_box_load(be, 1);
            _sv0t92.p0 = _sv0t94;
            int _sv0t95 = sv0_box_load(be, 2);
            _sv0t92.p1 = _sv0t95;
            int _sv0t96 = sv0_box_load(be, 3);
            _sv0t92.p2 = _sv0t96;
            Expr e;
            e.tag = (_sv0t92).tag;
            e.p0 = (_sv0t92).p0;
            e.p1 = (_sv0t92).p1;
            e.p2 = (_sv0t92).p2;
            const char* _sv0t97 = name_of(h);
            const char* _sv0t98 = sv0_string_concat(indent, _sv0t97);
            const char* a;
            a = _sv0t98;
            const char* _sv0t99 = sv0_string_concat(a, " = ");
            const char* b;
            b = _sv0t99;
            const char* _sv0t100 = emit_expr(e);
            const char* _sv0t101 = sv0_string_concat(b, _sv0t100);
            const char* c;
            c = _sv0t101;
            const char* _sv0t102 = sv0_string_concat(c, ";\n");
            return _sv0t102;
            _sv0t0 = 0;
          } else {
            if ((ins.tag == 5)) {
              int bh = ins.p0;
              int fh = ins.p1;
              int be = ins.p2;
              Expr _sv0t78;
              int _sv0t79 = sv0_box_load(be, 0);
              _sv0t78.tag = _sv0t79;
              int _sv0t80 = sv0_box_load(be, 1);
              _sv0t78.p0 = _sv0t80;
              int _sv0t81 = sv0_box_load(be, 2);
              _sv0t78.p1 = _sv0t81;
              int _sv0t82 = sv0_box_load(be, 3);
              _sv0t78.p2 = _sv0t82;
              Expr e;
              e.tag = (_sv0t78).tag;
              e.p0 = (_sv0t78).p0;
              e.p1 = (_sv0t78).p1;
              e.p2 = (_sv0t78).p2;
              const char* _sv0t83 = name_of(bh);
              const char* _sv0t84 = sv0_string_concat(indent, _sv0t83);
              const char* a;
              a = _sv0t84;
              const char* _sv0t85 = sv0_string_concat(a, ".");
              const char* b;
              b = _sv0t85;
              const char* _sv0t86 = name_of(fh);
              const char* _sv0t87 = sv0_string_concat(b, _sv0t86);
              const char* c;
              c = _sv0t87;
              const char* _sv0t88 = sv0_string_concat(c, " = ");
              const char* d;
              d = _sv0t88;
              const char* _sv0t89 = emit_expr(e);
              const char* _sv0t90 = sv0_string_concat(d, _sv0t89);
              const char* f;
              f = _sv0t90;
              const char* _sv0t91 = sv0_string_concat(f, ";\n");
              return _sv0t91;
              _sv0t0 = 0;
            } else {
              if ((ins.tag == 6)) {
                int be = ins.p0;
                int th = ins.p1;
                int el = ins.p2;
                Expr _sv0t60;
                int _sv0t61 = sv0_box_load(be, 0);
                _sv0t60.tag = _sv0t61;
                int _sv0t62 = sv0_box_load(be, 1);
                _sv0t60.p0 = _sv0t62;
                int _sv0t63 = sv0_box_load(be, 2);
                _sv0t60.p1 = _sv0t63;
                int _sv0t64 = sv0_box_load(be, 3);
                _sv0t60.p2 = _sv0t64;
                Expr e;
                e.tag = (_sv0t60).tag;
                e.p0 = (_sv0t60).p0;
                e.p1 = (_sv0t60).p1;
                e.p2 = (_sv0t60).p2;
                const char* _sv0t65 = sv0_string_concat(indent, "  ");
                const char* deeper;
                deeper = _sv0t65;
                const char* _sv0t66 = sv0_string_concat(indent, "if (");
                const char* a;
                a = _sv0t66;
                const char* _sv0t67 = emit_expr(e);
                const char* _sv0t68 = sv0_string_concat(a, _sv0t67);
                const char* b;
                b = _sv0t68;
                const char* _sv0t69 = sv0_string_concat(b, ") {\n");
                const char* c;
                c = _sv0t69;
                const char* _sv0t70 = emit_instrs(th, deeper, ret_ty);
                const char* _sv0t71 = sv0_string_concat(c, _sv0t70);
                const char* d;
                d = _sv0t71;
                const char* _sv0t72 = sv0_string_concat(d, indent);
                const char* f;
                f = _sv0t72;
                const char* _sv0t73 = sv0_string_concat(f, "} else {\n");
                const char* g;
                g = _sv0t73;
                const char* _sv0t74 = emit_instrs(el, deeper, ret_ty);
                const char* _sv0t75 = sv0_string_concat(g, _sv0t74);
                const char* h;
                h = _sv0t75;
                const char* _sv0t76 = sv0_string_concat(h, indent);
                const char* j;
                j = _sv0t76;
                const char* _sv0t77 = sv0_string_concat(j, "}\n");
                return _sv0t77;
                _sv0t0 = 0;
              } else {
                if ((ins.tag == 7)) {
                  int be = ins.p0;
                  int body = ins.p1;
                  Expr _sv0t46;
                  int _sv0t47 = sv0_box_load(be, 0);
                  _sv0t46.tag = _sv0t47;
                  int _sv0t48 = sv0_box_load(be, 1);
                  _sv0t46.p0 = _sv0t48;
                  int _sv0t49 = sv0_box_load(be, 2);
                  _sv0t46.p1 = _sv0t49;
                  int _sv0t50 = sv0_box_load(be, 3);
                  _sv0t46.p2 = _sv0t50;
                  Expr e;
                  e.tag = (_sv0t46).tag;
                  e.p0 = (_sv0t46).p0;
                  e.p1 = (_sv0t46).p1;
                  e.p2 = (_sv0t46).p2;
                  const char* _sv0t51 = sv0_string_concat(indent, "  ");
                  const char* deeper;
                  deeper = _sv0t51;
                  const char* _sv0t52 = sv0_string_concat(indent, "while (");
                  const char* a;
                  a = _sv0t52;
                  const char* _sv0t53 = emit_expr(e);
                  const char* _sv0t54 = sv0_string_concat(a, _sv0t53);
                  const char* b;
                  b = _sv0t54;
                  const char* _sv0t55 = sv0_string_concat(b, ") {\n");
                  const char* c;
                  c = _sv0t55;
                  const char* _sv0t56 = emit_instrs(body, deeper, ret_ty);
                  const char* _sv0t57 = sv0_string_concat(c, _sv0t56);
                  const char* d;
                  d = _sv0t57;
                  const char* _sv0t58 = sv0_string_concat(d, indent);
                  const char* f;
                  f = _sv0t58;
                  const char* _sv0t59 = sv0_string_concat(f, "}\n");
                  return _sv0t59;
                  _sv0t0 = 0;
                } else {
                  if ((ins.tag == 8)) {
                    int body = ins.p0;
                    const char* _sv0t40 = sv0_string_concat(indent, "  ");
                    const char* deeper;
                    deeper = _sv0t40;
                    const char* _sv0t41 = sv0_string_concat(indent, "{\n");
                    const char* a;
                    a = _sv0t41;
                    const char* _sv0t42 = emit_instrs(body, deeper, ret_ty);
                    const char* _sv0t43 = sv0_string_concat(a, _sv0t42);
                    const char* b;
                    b = _sv0t43;
                    const char* _sv0t44 = sv0_string_concat(b, indent);
                    const char* c;
                    c = _sv0t44;
                    const char* _sv0t45 = sv0_string_concat(c, "}\n");
                    return _sv0t45;
                    _sv0t0 = 0;
                  } else {
                    if ((ins.tag == 9)) {
                      const char* _sv0t39 = sv0_string_concat(indent, "break;\n");
                      return _sv0t39;
                      _sv0t0 = 0;
                    } else {
                      if ((ins.tag == 10)) {
                        const char* _sv0t38 = sv0_string_concat(indent, "continue;\n");
                        return _sv0t38;
                        _sv0t0 = 0;
                      } else {
                        if ((ins.tag == 11)) {
                          int dst_h = ins.p0;
                          int fn_h = ins.p1;
                          int args = ins.p2;
                          int rt_h = ins.p3;
                          if ((dst_h == 0)) {
                            const char* _sv0t36 = emit_call_void(indent, fn_h, args);
                            return _sv0t36;
                          } else {
                          }
                          const char* _sv0t37 = emit_call_dest(indent, dst_h, fn_h, args, rt_h);
                          return _sv0t37;
                          _sv0t0 = 0;
                        } else {
                          if ((ins.tag == 12)) {
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
                            if ((ins.tag == 13)) {
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
                              if ((ins.tag == 14)) {
                                const char* _sv0t11 = sv0_string_concat(indent, "return;\n");
                                return _sv0t11;
                                _sv0t0 = 0;
                              } else {
                                if ((ins.tag == 15)) {
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
    int _sv0t7 = sv0_box_load(bh, 4);
    _sv0t2.p3 = _sv0t7;
    Instr ins;
    ins.tag = (_sv0t2).tag;
    ins.p0 = (_sv0t2).p0;
    ins.p1 = (_sv0t2).p1;
    ins.p2 = (_sv0t2).p2;
    ins.p3 = (_sv0t2).p3;
    const char* _sv0t8 = emit_instr(indent, ret_ty, ins);
    const char* _sv0t9 = sv0_string_concat(out, _sv0t8);
    out = _sv0t9;
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

static const char* emit_param_list(int params) {
  int _sv0t0 = sv0_vec_len(params);
  int total = _sv0t0;
  if ((total == 0)) {
    return "void";
  } else {
  }
  int count = (total / 2);
  const char* out;
  out = "";
  int idx = 0;
  while ((idx < count)) {
    if ((idx > 0)) {
      const char* _sv0t1 = sv0_string_concat(out, ", ");
      out = _sv0t1;
    } else {
    }
    int _sv0t2 = (idx * 2);
    int _sv0t3 = sv0_vec_get(params, _sv0t2);
    int ct_h = _sv0t3;
    int _sv0t4 = (idx * 2);
    int _sv0t5 = (_sv0t4 + 1);
    int _sv0t6 = sv0_vec_get(params, _sv0t5);
    int nm_h = _sv0t6;
    const char* _sv0t7 = name_of(ct_h);
    const char* _sv0t8 = sv0_string_concat(_sv0t7, " ");
    const char* pair;
    pair = _sv0t8;
    const char* _sv0t9 = name_of(nm_h);
    const char* _sv0t10 = sv0_string_concat(pair, _sv0t9);
    const char* _sv0t11 = sv0_string_concat(out, _sv0t10);
    out = _sv0t11;
    idx = (idx + 1);
  }
  return out;
}

static const char* emit_fn_proto_ex(int is_static, int ret_cty_h, int label_h, int params) {
  const char* _sv0t0 = name_of(ret_cty_h);
  const char* ret;
  ret = _sv0t0;
  const char* _sv0t1 = name_of(label_h);
  const char* label;
  label = _sv0t1;
  const char* _sv0t2 = emit_param_list(params);
  const char* pstr;
  pstr = _sv0t2;
  if ((is_static == 1)) {
    const char* _sv0t3 = sv0_string_concat("static ", ret);
    const char* a;
    a = _sv0t3;
    const char* _sv0t4 = sv0_string_concat(a, " ");
    const char* b;
    b = _sv0t4;
    const char* _sv0t5 = sv0_string_concat(b, label);
    const char* c;
    c = _sv0t5;
    const char* _sv0t6 = sv0_string_concat(c, "(");
    const char* d;
    d = _sv0t6;
    const char* _sv0t7 = sv0_string_concat(pstr, ")");
    const char* _sv0t8 = sv0_string_concat(d, _sv0t7);
    return _sv0t8;
  } else {
  }
  const char* _sv0t9 = sv0_string_concat(ret, " ");
  const char* a;
  a = _sv0t9;
  const char* _sv0t10 = sv0_string_concat(a, label);
  const char* b;
  b = _sv0t10;
  const char* _sv0t11 = sv0_string_concat(b, "(");
  const char* c;
  c = _sv0t11;
  const char* _sv0t12 = sv0_string_concat(pstr, ")");
  const char* _sv0t13 = sv0_string_concat(c, _sv0t12);
  return _sv0t13;
}

static const char* emit_program(const char* typedefs, int blocks) {
  int _sv0t0 = sv0_vec_len(blocks);
  int num_blocks = (_sv0t0 / 4);
  if ((num_blocks == 0)) {
    return "#include \"sv0_runtime.h\"\nint main(void) { return 0; }\n";
  } else {
  }
  const char* prelude;
  prelude = "#include \"sv0_runtime.h\"\n\n";
  const char* td_part;
  td_part = "";
  int _sv0t1 = sv0_string_eq(typedefs, "");
  if ((!_sv0t1)) {
    const char* _sv0t2 = sv0_string_concat(typedefs, "\n");
    td_part = _sv0t2;
  } else {
  }
  const char* fwd_decls;
  fwd_decls = "";
  const char* statics;
  statics = "";
  const char* main_part;
  main_part = "";
  int idx = 0;
  while ((idx < num_blocks)) {
    int _sv0t3 = (idx * 4);
    int _sv0t4 = sv0_vec_get(blocks, _sv0t3);
    int label_h = _sv0t4;
    int _sv0t5 = (idx * 4);
    int _sv0t6 = (_sv0t5 + 1);
    int _sv0t7 = sv0_vec_get(blocks, _sv0t6);
    int ret_h = _sv0t7;
    int _sv0t8 = (idx * 4);
    int _sv0t9 = (_sv0t8 + 2);
    int _sv0t10 = sv0_vec_get(blocks, _sv0t9);
    int params = _sv0t10;
    int _sv0t11 = (idx * 4);
    int _sv0t12 = (_sv0t11 + 3);
    int _sv0t13 = sv0_vec_get(blocks, _sv0t12);
    int instrs = _sv0t13;
    const char* _sv0t14 = name_of(ret_h);
    const char* ret_name;
    ret_name = _sv0t14;
    if ((label_h == 11)) {
      const char* _sv0t15 = emit_fn_proto_ex(0, ret_h, label_h, params);
      const char* proto;
      proto = _sv0t15;
      const char* _sv0t16 = emit_instrs(instrs, "  ", ret_name);
      const char* body;
      body = _sv0t16;
      const char* _sv0t17 = emit_block_defn(proto, body);
      main_part = _sv0t17;
    } else {
      const char* _sv0t18 = emit_fn_proto_ex(1, ret_h, label_h, params);
      const char* proto;
      proto = _sv0t18;
      const char* _sv0t19 = sv0_string_concat(proto, ";\n");
      const char* _sv0t20 = sv0_string_concat(fwd_decls, _sv0t19);
      fwd_decls = _sv0t20;
      const char* _sv0t21 = emit_instrs(instrs, "  ", ret_name);
      const char* body;
      body = _sv0t21;
      const char* _sv0t22 = emit_block_defn(proto, body);
      const char* _sv0t23 = sv0_string_concat(statics, _sv0t22);
      statics = _sv0t23;
    }
    idx = (idx + 1);
  }
  int _sv0t24 = sv0_string_eq(main_part, "");
  if (_sv0t24) {
    main_part = "int main(void) { return 0; }\n";
  } else {
  }
  const char* _sv0t25 = sv0_string_concat(prelude, td_part);
  const char* out;
  out = _sv0t25;
  const char* _sv0t26 = sv0_string_concat(out, fwd_decls);
  out = _sv0t26;
  const char* _sv0t27 = sv0_string_concat(out, "\n");
  out = _sv0t27;
  const char* _sv0t28 = sv0_string_concat(out, statics);
  out = _sv0t28;
  const char* _sv0t29 = sv0_string_concat(out, main_part);
  return _sv0t29;
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
  then_i.tag = 15;
  then_i.p0 = _sv0t3;
  Instr else_i;
  Value _sv0t4;
  _sv0t4.tag = 0;
  _sv0t4.p0 = 0;
  int _sv0t5 = sv0_box_alloc(3);
  sv0_box_store(_sv0t5, 0, _sv0t4.tag);
  sv0_box_store(_sv0t5, 1, _sv0t4.p0);
  sv0_box_store(_sv0t5, 2, _sv0t4.p1);
  else_i.tag = 15;
  else_i.p0 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int th = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int el = _sv0t7;
  int _sv0t8 = sv0_box_alloc(5);
  sv0_box_store(_sv0t8, 0, then_i.tag);
  sv0_box_store(_sv0t8, 1, then_i.p0);
  sv0_box_store(_sv0t8, 2, then_i.p1);
  sv0_box_store(_sv0t8, 3, then_i.p2);
  sv0_box_store(_sv0t8, 4, then_i.p3);
  sv0_vec_push(th, _sv0t8);
  int _sv0t9 = sv0_box_alloc(5);
  sv0_box_store(_sv0t9, 0, else_i.tag);
  sv0_box_store(_sv0t9, 1, else_i.p0);
  sv0_box_store(_sv0t9, 2, else_i.p1);
  sv0_box_store(_sv0t9, 3, else_i.p2);
  sv0_box_store(_sv0t9, 4, else_i.p3);
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
  int _sv0t3 = sv0_box_alloc(5);
  sv0_box_store(_sv0t3, 0, body_i.tag);
  sv0_box_store(_sv0t3, 1, body_i.p0);
  sv0_box_store(_sv0t3, 2, body_i.p1);
  sv0_box_store(_sv0t3, 3, body_i.p2);
  sv0_box_store(_sv0t3, 4, body_i.p3);
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
  int _sv0t1 = sv0_box_alloc(5);
  sv0_box_store(_sv0t1, 0, i1.tag);
  sv0_box_store(_sv0t1, 1, i1.p0);
  sv0_box_store(_sv0t1, 2, i1.p1);
  sv0_box_store(_sv0t1, 3, i1.p2);
  sv0_box_store(_sv0t1, 4, i1.p3);
  sv0_vec_push(body, _sv0t1);
  int _sv0t2 = sv0_box_alloc(5);
  sv0_box_store(_sv0t2, 0, i2.tag);
  sv0_box_store(_sv0t2, 1, i2.p0);
  sv0_box_store(_sv0t2, 2, i2.p1);
  sv0_box_store(_sv0t2, 3, i2.p2);
  sv0_box_store(_sv0t2, 4, i2.p3);
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
  int _sv0t0 = sv0_vec_new();
  int args = _sv0t0;
  Instr ins;
  ins.tag = 11;
  ins.p0 = 0;
  ins.p1 = 12;
  ins.p2 = args;
  ins.p3 = 0;
  const char* _sv0t1 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = sv0_string_eq(s, "  foo();\n");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_call_dest(void) {
  int _sv0t0 = sv0_vec_new();
  int args = _sv0t0;
  Instr ins;
  ins.tag = 11;
  ins.p0 = 14;
  ins.p1 = 12;
  ins.p2 = args;
  ins.p3 = 9;
  const char* _sv0t1 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = sv0_string_eq(s, "  int result = foo();\n");
  if ((!_sv0t2)) {
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
  ins.tag = 12;
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
  ins.tag = 13;
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
  ins.tag = 14;
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
  ins.tag = 15;
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
  ins.tag = 15;
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

static int test_param_list_general(void) {
  int _sv0t0 = sv0_vec_new();
  int p0 = _sv0t0;
  const char* _sv0t1 = emit_param_list(p0);
  int _sv0t2 = sv0_string_eq(_sv0t1, "void");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int p1 = _sv0t3;
  sv0_vec_push(p1, 9);
  sv0_vec_push(p1, 1);
  const char* _sv0t4 = emit_param_list(p1);
  int _sv0t5 = sv0_string_eq(_sv0t4, "int x");
  if ((!_sv0t5)) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int p2 = _sv0t6;
  sv0_vec_push(p2, 9);
  sv0_vec_push(p2, 1);
  sv0_vec_push(p2, 9);
  sv0_vec_push(p2, 2);
  const char* _sv0t7 = emit_param_list(p2);
  int _sv0t8 = sv0_string_eq(_sv0t7, "int x, int y");
  if ((!_sv0t8)) {
    return 3;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int p3 = _sv0t9;
  sv0_vec_push(p3, 9);
  sv0_vec_push(p3, 1);
  sv0_vec_push(p3, 10);
  sv0_vec_push(p3, 2);
  sv0_vec_push(p3, 9);
  sv0_vec_push(p3, 3);
  const char* _sv0t10 = emit_param_list(p3);
  int _sv0t11 = sv0_string_eq(_sv0t10, "int x, void y, int n");
  if ((!_sv0t11)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_emit_program_empty(void) {
  int _sv0t0 = sv0_vec_new();
  int blocks = _sv0t0;
  const char* _sv0t1 = emit_program("", blocks);
  const char* s;
  s = _sv0t1;
  const char* expected;
  expected = "#include \"sv0_runtime.h\"\nint main(void) { return 0; }\n";
  int _sv0t2 = sv0_string_eq(s, expected);
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_program_main_only(void) {
  int _sv0t0 = sv0_vec_new();
  int blocks = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int params = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int instrs = _sv0t2;
  Instr _sv0t3;
  Value _sv0t4;
  _sv0t4.tag = 0;
  _sv0t4.p0 = 0;
  int _sv0t5 = sv0_box_alloc(3);
  sv0_box_store(_sv0t5, 0, _sv0t4.tag);
  sv0_box_store(_sv0t5, 1, _sv0t4.p0);
  sv0_box_store(_sv0t5, 2, _sv0t4.p1);
  _sv0t3.tag = 15;
  _sv0t3.p0 = _sv0t5;
  int _sv0t6 = sv0_box_alloc(5);
  sv0_box_store(_sv0t6, 0, _sv0t3.tag);
  sv0_box_store(_sv0t6, 1, _sv0t3.p0);
  sv0_box_store(_sv0t6, 2, _sv0t3.p1);
  sv0_box_store(_sv0t6, 3, _sv0t3.p2);
  sv0_box_store(_sv0t6, 4, _sv0t3.p3);
  sv0_vec_push(instrs, _sv0t6);
  sv0_vec_push(blocks, 11);
  sv0_vec_push(blocks, 9);
  sv0_vec_push(blocks, params);
  sv0_vec_push(blocks, instrs);
  const char* _sv0t7 = emit_program("", blocks);
  const char* s;
  s = _sv0t7;
  const char* expected;
  expected = "#include \"sv0_runtime.h\"\n\n\nint main(void) {\n  return 0;\n}\n\n";
  int _sv0t8 = sv0_string_eq(s, expected);
  if ((!_sv0t8)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_program_with_helper(void) {
  int _sv0t0 = sv0_vec_new();
  int blocks = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int foo_params = _sv0t1;
  sv0_vec_push(foo_params, 9);
  sv0_vec_push(foo_params, 3);
  int _sv0t2 = sv0_vec_new();
  int foo_instrs = _sv0t2;
  Instr _sv0t3;
  Value _sv0t4;
  _sv0t4.tag = 3;
  _sv0t4.p0 = 3;
  int _sv0t5 = sv0_box_alloc(3);
  sv0_box_store(_sv0t5, 0, _sv0t4.tag);
  sv0_box_store(_sv0t5, 1, _sv0t4.p0);
  sv0_box_store(_sv0t5, 2, _sv0t4.p1);
  _sv0t3.tag = 15;
  _sv0t3.p0 = _sv0t5;
  int _sv0t6 = sv0_box_alloc(5);
  sv0_box_store(_sv0t6, 0, _sv0t3.tag);
  sv0_box_store(_sv0t6, 1, _sv0t3.p0);
  sv0_box_store(_sv0t6, 2, _sv0t3.p1);
  sv0_box_store(_sv0t6, 3, _sv0t3.p2);
  sv0_box_store(_sv0t6, 4, _sv0t3.p3);
  sv0_vec_push(foo_instrs, _sv0t6);
  sv0_vec_push(blocks, 12);
  sv0_vec_push(blocks, 9);
  sv0_vec_push(blocks, foo_params);
  sv0_vec_push(blocks, foo_instrs);
  int _sv0t7 = sv0_vec_new();
  int main_params = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int main_instrs = _sv0t8;
  Instr _sv0t9;
  Value _sv0t10;
  _sv0t10.tag = 0;
  _sv0t10.p0 = 0;
  int _sv0t11 = sv0_box_alloc(3);
  sv0_box_store(_sv0t11, 0, _sv0t10.tag);
  sv0_box_store(_sv0t11, 1, _sv0t10.p0);
  sv0_box_store(_sv0t11, 2, _sv0t10.p1);
  _sv0t9.tag = 15;
  _sv0t9.p0 = _sv0t11;
  int _sv0t12 = sv0_box_alloc(5);
  sv0_box_store(_sv0t12, 0, _sv0t9.tag);
  sv0_box_store(_sv0t12, 1, _sv0t9.p0);
  sv0_box_store(_sv0t12, 2, _sv0t9.p1);
  sv0_box_store(_sv0t12, 3, _sv0t9.p2);
  sv0_box_store(_sv0t12, 4, _sv0t9.p3);
  sv0_vec_push(main_instrs, _sv0t12);
  sv0_vec_push(blocks, 11);
  sv0_vec_push(blocks, 9);
  sv0_vec_push(blocks, main_params);
  sv0_vec_push(blocks, main_instrs);
  const char* _sv0t13 = emit_program("", blocks);
  const char* s;
  s = _sv0t13;
  const char* exp;
  exp = "#include \"sv0_runtime.h\"\n\n";
  const char* _sv0t14 = sv0_string_concat(exp, "static int foo(int n);\n");
  exp = _sv0t14;
  const char* _sv0t15 = sv0_string_concat(exp, "\n");
  exp = _sv0t15;
  const char* _sv0t16 = sv0_string_concat(exp, "static int foo(int n) {\n  return n;\n}\n\n");
  exp = _sv0t16;
  const char* _sv0t17 = sv0_string_concat(exp, "int main(void) {\n  return 0;\n}\n\n");
  exp = _sv0t17;
  int _sv0t18 = sv0_string_eq(s, exp);
  if ((!_sv0t18)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_program_typedefs(void) {
  int _sv0t0 = sv0_vec_new();
  int blocks = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int params = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int instrs = _sv0t2;
  Instr _sv0t3;
  Value _sv0t4;
  _sv0t4.tag = 0;
  _sv0t4.p0 = 0;
  int _sv0t5 = sv0_box_alloc(3);
  sv0_box_store(_sv0t5, 0, _sv0t4.tag);
  sv0_box_store(_sv0t5, 1, _sv0t4.p0);
  sv0_box_store(_sv0t5, 2, _sv0t4.p1);
  _sv0t3.tag = 15;
  _sv0t3.p0 = _sv0t5;
  int _sv0t6 = sv0_box_alloc(5);
  sv0_box_store(_sv0t6, 0, _sv0t3.tag);
  sv0_box_store(_sv0t6, 1, _sv0t3.p0);
  sv0_box_store(_sv0t6, 2, _sv0t3.p1);
  sv0_box_store(_sv0t6, 3, _sv0t3.p2);
  sv0_box_store(_sv0t6, 4, _sv0t3.p3);
  sv0_vec_push(instrs, _sv0t6);
  sv0_vec_push(blocks, 11);
  sv0_vec_push(blocks, 9);
  sv0_vec_push(blocks, params);
  sv0_vec_push(blocks, instrs);
  const char* _sv0t7 = emit_program("typedef int sv0_int;", blocks);
  const char* s;
  s = _sv0t7;
  const char* exp;
  exp = "#include \"sv0_runtime.h\"\n\n";
  const char* _sv0t8 = sv0_string_concat(exp, "typedef int sv0_int;\n");
  exp = _sv0t8;
  const char* _sv0t9 = sv0_string_concat(exp, "\n");
  exp = _sv0t9;
  const char* _sv0t10 = sv0_string_concat(exp, "int main(void) {\n  return 0;\n}\n\n");
  exp = _sv0t10;
  int _sv0t11 = sv0_string_eq(s, exp);
  if ((!_sv0t11)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_emit_program_no_main(void) {
  int _sv0t0 = sv0_vec_new();
  int blocks = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int foo_params = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int foo_instrs = _sv0t2;
  Instr _sv0t3;
  Value _sv0t4;
  _sv0t4.tag = 0;
  _sv0t4.p0 = 1;
  int _sv0t5 = sv0_box_alloc(3);
  sv0_box_store(_sv0t5, 0, _sv0t4.tag);
  sv0_box_store(_sv0t5, 1, _sv0t4.p0);
  sv0_box_store(_sv0t5, 2, _sv0t4.p1);
  _sv0t3.tag = 15;
  _sv0t3.p0 = _sv0t5;
  int _sv0t6 = sv0_box_alloc(5);
  sv0_box_store(_sv0t6, 0, _sv0t3.tag);
  sv0_box_store(_sv0t6, 1, _sv0t3.p0);
  sv0_box_store(_sv0t6, 2, _sv0t3.p1);
  sv0_box_store(_sv0t6, 3, _sv0t3.p2);
  sv0_box_store(_sv0t6, 4, _sv0t3.p3);
  sv0_vec_push(foo_instrs, _sv0t6);
  sv0_vec_push(blocks, 12);
  sv0_vec_push(blocks, 9);
  sv0_vec_push(blocks, foo_params);
  sv0_vec_push(blocks, foo_instrs);
  const char* _sv0t7 = emit_program("", blocks);
  const char* s;
  s = _sv0t7;
  const char* exp;
  exp = "#include \"sv0_runtime.h\"\n\n";
  const char* _sv0t8 = sv0_string_concat(exp, "static int foo(void);\n");
  exp = _sv0t8;
  const char* _sv0t9 = sv0_string_concat(exp, "\n");
  exp = _sv0t9;
  const char* _sv0t10 = sv0_string_concat(exp, "static int foo(void) {\n  return 1;\n}\n\n");
  exp = _sv0t10;
  const char* _sv0t11 = sv0_string_concat(exp, "int main(void) { return 0; }\n");
  exp = _sv0t11;
  int _sv0t12 = sv0_string_eq(s, exp);
  if ((!_sv0t12)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_fn_proto_ex(void) {
  int _sv0t0 = sv0_vec_new();
  int p0 = _sv0t0;
  const char* _sv0t1 = emit_fn_proto_ex(1, 9, 12, p0);
  const char* s1;
  s1 = _sv0t1;
  int _sv0t2 = sv0_string_eq(s1, "static int foo(void)");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int p2 = _sv0t3;
  sv0_vec_push(p2, 9);
  sv0_vec_push(p2, 1);
  sv0_vec_push(p2, 9);
  sv0_vec_push(p2, 2);
  const char* _sv0t4 = emit_fn_proto_ex(0, 9, 11, p2);
  const char* s2;
  s2 = _sv0t4;
  int _sv0t5 = sv0_string_eq(s2, "int main(int x, int y)");
  if ((!_sv0t5)) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int p3 = _sv0t6;
  sv0_vec_push(p3, 9);
  sv0_vec_push(p3, 3);
  const char* _sv0t7 = emit_fn_proto_ex(1, 10, 13, p3);
  const char* s3;
  s3 = _sv0t7;
  int _sv0t8 = sv0_string_eq(s3, "static void bar(int n)");
  if ((!_sv0t8)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_call_args_empty(void) {
  int _sv0t0 = sv0_vec_new();
  int args = _sv0t0;
  const char* _sv0t1 = emit_call_args(args);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = sv0_string_eq(s, "");
  if ((!_sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_call_args_one(void) {
  int _sv0t0 = sv0_vec_new();
  int args = _sv0t0;
  Value _sv0t1;
  _sv0t1.tag = 0;
  _sv0t1.p0 = 42;
  int _sv0t2 = sv0_box_alloc(3);
  sv0_box_store(_sv0t2, 0, _sv0t1.tag);
  sv0_box_store(_sv0t2, 1, _sv0t1.p0);
  sv0_box_store(_sv0t2, 2, _sv0t1.p1);
  sv0_vec_push(args, _sv0t2);
  const char* _sv0t3 = emit_call_args(args);
  const char* s;
  s = _sv0t3;
  int _sv0t4 = sv0_string_eq(s, "42");
  if ((!_sv0t4)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_call_args_two(void) {
  int _sv0t0 = sv0_vec_new();
  int args = _sv0t0;
  Value _sv0t1;
  _sv0t1.tag = 3;
  _sv0t1.p0 = 1;
  int _sv0t2 = sv0_box_alloc(3);
  sv0_box_store(_sv0t2, 0, _sv0t1.tag);
  sv0_box_store(_sv0t2, 1, _sv0t1.p0);
  sv0_box_store(_sv0t2, 2, _sv0t1.p1);
  sv0_vec_push(args, _sv0t2);
  Value _sv0t3;
  _sv0t3.tag = 0;
  _sv0t3.p0 = 10;
  int _sv0t4 = sv0_box_alloc(3);
  sv0_box_store(_sv0t4, 0, _sv0t3.tag);
  sv0_box_store(_sv0t4, 1, _sv0t3.p0);
  sv0_box_store(_sv0t4, 2, _sv0t3.p1);
  sv0_vec_push(args, _sv0t4);
  const char* _sv0t5 = emit_call_args(args);
  const char* s;
  s = _sv0t5;
  int _sv0t6 = sv0_string_eq(s, "x, 10");
  if ((!_sv0t6)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_call_args_three(void) {
  int _sv0t0 = sv0_vec_new();
  int args = _sv0t0;
  Value _sv0t1;
  _sv0t1.tag = 3;
  _sv0t1.p0 = 1;
  int _sv0t2 = sv0_box_alloc(3);
  sv0_box_store(_sv0t2, 0, _sv0t1.tag);
  sv0_box_store(_sv0t2, 1, _sv0t1.p0);
  sv0_box_store(_sv0t2, 2, _sv0t1.p1);
  sv0_vec_push(args, _sv0t2);
  Value _sv0t3;
  _sv0t3.tag = 3;
  _sv0t3.p0 = 2;
  int _sv0t4 = sv0_box_alloc(3);
  sv0_box_store(_sv0t4, 0, _sv0t3.tag);
  sv0_box_store(_sv0t4, 1, _sv0t3.p0);
  sv0_box_store(_sv0t4, 2, _sv0t3.p1);
  sv0_vec_push(args, _sv0t4);
  Value _sv0t5;
  _sv0t5.tag = 0;
  _sv0t5.p0 = 0;
  int _sv0t6 = sv0_box_alloc(3);
  sv0_box_store(_sv0t6, 0, _sv0t5.tag);
  sv0_box_store(_sv0t6, 1, _sv0t5.p0);
  sv0_box_store(_sv0t6, 2, _sv0t5.p1);
  sv0_vec_push(args, _sv0t6);
  const char* _sv0t7 = emit_call_args(args);
  const char* s;
  s = _sv0t7;
  int _sv0t8 = sv0_string_eq(s, "x, y, 0");
  if ((!_sv0t8)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_call_with_args(void) {
  int _sv0t0 = sv0_vec_new();
  int args = _sv0t0;
  Value _sv0t1;
  _sv0t1.tag = 3;
  _sv0t1.p0 = 1;
  int _sv0t2 = sv0_box_alloc(3);
  sv0_box_store(_sv0t2, 0, _sv0t1.tag);
  sv0_box_store(_sv0t2, 1, _sv0t1.p0);
  sv0_box_store(_sv0t2, 2, _sv0t1.p1);
  sv0_vec_push(args, _sv0t2);
  Value _sv0t3;
  _sv0t3.tag = 0;
  _sv0t3.p0 = 5;
  int _sv0t4 = sv0_box_alloc(3);
  sv0_box_store(_sv0t4, 0, _sv0t3.tag);
  sv0_box_store(_sv0t4, 1, _sv0t3.p0);
  sv0_box_store(_sv0t4, 2, _sv0t3.p1);
  sv0_vec_push(args, _sv0t4);
  Instr ins;
  ins.tag = 11;
  ins.p0 = 14;
  ins.p1 = 12;
  ins.p2 = args;
  ins.p3 = 9;
  const char* _sv0t5 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t5;
  int _sv0t6 = sv0_string_eq(s, "  int result = foo(x, 5);\n");
  if ((!_sv0t6)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_instr_call_void_with_args(void) {
  int _sv0t0 = sv0_vec_new();
  int args = _sv0t0;
  Value _sv0t1;
  _sv0t1.tag = 3;
  _sv0t1.p0 = 3;
  int _sv0t2 = sv0_box_alloc(3);
  sv0_box_store(_sv0t2, 0, _sv0t1.tag);
  sv0_box_store(_sv0t2, 1, _sv0t1.p0);
  sv0_box_store(_sv0t2, 2, _sv0t1.p1);
  sv0_vec_push(args, _sv0t2);
  Instr ins;
  ins.tag = 11;
  ins.p0 = 0;
  ins.p1 = 12;
  ins.p2 = args;
  ins.p3 = 0;
  const char* _sv0t3 = emit_instr("  ", "int", ins);
  const char* s;
  s = _sv0t3;
  int _sv0t4 = sv0_string_eq(s, "  foo(n);\n");
  if ((!_sv0t4)) {
    return 1;
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
  int _sv0t61 = test_call_args_empty();
  int r29 = _sv0t61;
  if ((r29 != 0)) {
    int _sv0t62 = (280 + r29);
    return _sv0t62;
  } else {
  }
  sv0_println("PASS test_call_args_empty");
  int _sv0t63 = test_call_args_one();
  int r30 = _sv0t63;
  if ((r30 != 0)) {
    int _sv0t64 = (290 + r30);
    return _sv0t64;
  } else {
  }
  sv0_println("PASS test_call_args_one");
  int _sv0t65 = test_call_args_two();
  int r31 = _sv0t65;
  if ((r31 != 0)) {
    int _sv0t66 = (300 + r31);
    return _sv0t66;
  } else {
  }
  sv0_println("PASS test_call_args_two");
  int _sv0t67 = test_call_args_three();
  int r32 = _sv0t67;
  if ((r32 != 0)) {
    int _sv0t68 = (310 + r32);
    return _sv0t68;
  } else {
  }
  sv0_println("PASS test_call_args_three");
  int _sv0t69 = test_instr_call_with_args();
  int r33 = _sv0t69;
  if ((r33 != 0)) {
    int _sv0t70 = (320 + r33);
    return _sv0t70;
  } else {
  }
  sv0_println("PASS test_instr_call_with_args");
  int _sv0t71 = test_instr_call_void_with_args();
  int r34 = _sv0t71;
  if ((r34 != 0)) {
    int _sv0t72 = (330 + r34);
    return _sv0t72;
  } else {
  }
  sv0_println("PASS test_instr_call_void_with_args");
  int _sv0t73 = test_param_list_general();
  int r35 = _sv0t73;
  if ((r35 != 0)) {
    int _sv0t74 = (340 + r35);
    return _sv0t74;
  } else {
  }
  sv0_println("PASS test_param_list_general");
  int _sv0t75 = test_fn_proto_ex();
  int r36 = _sv0t75;
  if ((r36 != 0)) {
    int _sv0t76 = (350 + r36);
    return _sv0t76;
  } else {
  }
  sv0_println("PASS test_fn_proto_ex");
  int _sv0t77 = test_emit_program_empty();
  int r37 = _sv0t77;
  if ((r37 != 0)) {
    int _sv0t78 = (360 + r37);
    return _sv0t78;
  } else {
  }
  sv0_println("PASS test_emit_program_empty");
  int _sv0t79 = test_emit_program_main_only();
  int r38 = _sv0t79;
  if ((r38 != 0)) {
    int _sv0t80 = (370 + r38);
    return _sv0t80;
  } else {
  }
  sv0_println("PASS test_emit_program_main_only");
  int _sv0t81 = test_emit_program_with_helper();
  int r39 = _sv0t81;
  if ((r39 != 0)) {
    int _sv0t82 = (380 + r39);
    return _sv0t82;
  } else {
  }
  sv0_println("PASS test_emit_program_with_helper");
  int _sv0t83 = test_emit_program_typedefs();
  int r40 = _sv0t83;
  if ((r40 != 0)) {
    int _sv0t84 = (390 + r40);
    return _sv0t84;
  } else {
  }
  sv0_println("PASS test_emit_program_typedefs");
  int _sv0t85 = test_emit_program_no_main();
  int r41 = _sv0t85;
  if ((r41 != 0)) {
    int _sv0t86 = (400 + r41);
    return _sv0t86;
  } else {
  }
  sv0_println("PASS test_emit_program_no_main");
  return 0;
}

