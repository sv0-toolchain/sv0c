#include "sv0_runtime.h"

static int cx_push(int ct, int cd1, int cd2, int cd3, int tag, int d1, int d2, int d3);
static int cx_int(int ct, int cd1, int cd2, int cd3, int n);
static int cx_bool(int ct, int cd1, int cd2, int cd3, int b);
static int cx_var(int ct, int cd1, int cd2, int cd3, int h);
static int cx_result(int ct, int cd1, int cd2, int cd3);
static int cx_old(int ct, int cd1, int cd2, int cd3, int child);
static int cx_unop(int ct, int cd1, int cd2, int cd3, int op, int child);
static int cx_binop(int ct, int cd1, int cd2, int cd3, int op, int l, int r);
static const char* vc_digit_char(int d);
static const char* vc_int_to_str(int n);
static int vc_lookup(int names, int vals, int h);
static int vc_binop_eval(int op, int a, int b);
static int cexpr_eval(int ct, int cd1, int cd2, int cd3, int idx, int names, int vals, int result_val);
static const char* vc_binop_smt(int op);
static const char* vc_unop_smt(int op);
static const char* vc_smt_int(int n);
static const char* cexpr_to_smt(int ct, int cd1, int cd2, int cd3, int idx);
static int vc_parse_dec(const char* source, int starts, int ends, int tok);
static const char* vc_tok_text(const char* source, int starts, int ends, int tok);
static int vc_tok_name_eq(const char* source, int starts, int ends, int tok, const char* name);
static int vc_intern_var(int var_names, const char* source, int starts, int ends, int tok);
static int vc_block_idx(int pp, int first, int sidecar, int i);
static int ast_binop_to_cexpr(int tag);
static int extract_cexpr(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int idx, const char* source, int starts, int ends, int tok_tags, int ct, int cd1, int cd2, int cd3, int var_names);
static const char* vc_query_logic(int ct, int cd1);
static int vc_uses_result(int ct);
static const char* vc_build_query(int ct, int cd1, int cd2, int cd3, int hyps, int goal, int var_names);
static const char* vc_gen_ensures_query(int ct, int cd1, int cd2, int cd3, int requires_list, int ensures_root, int return_expr, int var_names);
static int vc_leftmost_leaf_tok(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int idx);
static int vc_contract_kind_tok(int tok_tags, int leaf);
static int vc_contract_kind(int tok_tags, int leaf);
static const char* vc_kind_name(int tag);
static int vc_match_rparen(int tok_tags, int lparen);
static const char* vc_clause_text(const char* source, int starts, int ends, int kw_tok, int rparen_tok);
static int vc_line_of_pos(const char* source, int pos);
static int vc_find_return_value(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int body_root);
static const char* verify_all_fns(int it, int id1, int id2, int id3, int id4, int fcb, int fcr, int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int tok_tags, const char* source, int starts, int ends);
static const char* verify_ensures_payload(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int req_roots, int ens_root, int body_root, int tok_tags, const char* source, int starts, int ends);
static int test_cx_tags(void);
static int test_eval_arith(void);
static int test_eval_compare(void);
static int test_eval_bool(void);
static int test_eval_result_old(void);
static int test_smt_emit(void);
static int be_push(int bet, int bed1, int bed2, int bed3, int bed4, int tag, int d1, int d2, int d3, int d4);
static const char* vc_test_vocab(int starts, int ends, int tok_tags);
static int test_extract_compare(void);
static int test_extract_result_arith(void);
static int test_extract_old(void);
static int test_extract_unsupported(void);
static int test_vc_query(void);
static int test_vc_gen_ensures(void);
static int test_verify_all_fns(void);

static int cx_push(int ct, int cd1, int cd2, int cd3, int tag, int d1, int d2, int d3) {
  sv0_vec_push(ct, tag);
  sv0_vec_push(cd1, d1);
  sv0_vec_push(cd2, d2);
  sv0_vec_push(cd3, d3);
  int _sv0t0 = sv0_vec_len(ct);
  int _sv0t1 = (_sv0t0 - 1);
  return _sv0t1;
}

static int cx_int(int ct, int cd1, int cd2, int cd3, int n) {
  int _sv0t0 = cx_push(ct, cd1, cd2, cd3, 0, n, 0, 0);
  return _sv0t0;
}

static int cx_bool(int ct, int cd1, int cd2, int cd3, int b) {
  int _sv0t0 = cx_push(ct, cd1, cd2, cd3, 1, b, 0, 0);
  return _sv0t0;
}

static int cx_var(int ct, int cd1, int cd2, int cd3, int h) {
  int _sv0t0 = cx_push(ct, cd1, cd2, cd3, 2, h, 0, 0);
  return _sv0t0;
}

static int cx_result(int ct, int cd1, int cd2, int cd3) {
  int _sv0t0 = cx_push(ct, cd1, cd2, cd3, 3, 0, 0, 0);
  return _sv0t0;
}

static int cx_old(int ct, int cd1, int cd2, int cd3, int child) {
  int _sv0t0 = cx_push(ct, cd1, cd2, cd3, 4, 0, child, 0);
  return _sv0t0;
}

static int cx_unop(int ct, int cd1, int cd2, int cd3, int op, int child) {
  int _sv0t0 = cx_push(ct, cd1, cd2, cd3, 5, op, child, 0);
  return _sv0t0;
}

static int cx_binop(int ct, int cd1, int cd2, int cd3, int op, int l, int r) {
  int _sv0t0 = cx_push(ct, cd1, cd2, cd3, 6, op, l, r);
  return _sv0t0;
}

static const char* vc_digit_char(int d) {
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
  return "9";
}

static const char* vc_int_to_str(int n) {
  if ((n < 0)) {
    int _sv0t0 = (0 - n);
    const char* _sv0t1 = vc_int_to_str(_sv0t0);
    const char* _sv0t2 = sv0_string_concat("-", _sv0t1);
    return _sv0t2;
  } else {
  }
  if ((n < 10)) {
    const char* _sv0t3 = vc_digit_char(n);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = (n / 10);
  const char* _sv0t5 = vc_int_to_str(_sv0t4);
  const char* rest;
  rest = _sv0t5;
  int _sv0t6 = (n / 10);
  int _sv0t7 = (_sv0t6 * 10);
  int _sv0t8 = (n - _sv0t7);
  const char* _sv0t9 = vc_digit_char(_sv0t8);
  const char* last;
  last = _sv0t9;
  const char* _sv0t10 = sv0_string_concat(rest, last);
  return _sv0t10;
}

static int vc_lookup(int names, int vals, int h) {
  int _sv0t0 = sv0_vec_len(names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(names, i);
    if ((_sv0t1 == h)) {
      int _sv0t2 = sv0_vec_get(vals, i);
      return _sv0t2;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int vc_binop_eval(int op, int a, int b) {
  if ((op == 0)) {
    int _sv0t0 = (a + b);
    return _sv0t0;
  } else {
  }
  if ((op == 1)) {
    int _sv0t1 = (a - b);
    return _sv0t1;
  } else {
  }
  if ((op == 2)) {
    int _sv0t2 = (a * b);
    return _sv0t2;
  } else {
  }
  if ((op == 3)) {
    int _sv0t3 = (a / b);
    return _sv0t3;
  } else {
  }
  if ((op == 4)) {
    int _sv0t4 = (a / b);
    int _sv0t5 = (_sv0t4 * b);
    int _sv0t6 = (a - _sv0t5);
    return _sv0t6;
  } else {
  }
  if ((op == 5)) {
    if ((a == b)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((op == 6)) {
    if ((a != b)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((op == 7)) {
    if ((a < b)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((op == 8)) {
    if ((a <= b)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((op == 9)) {
    if ((a > b)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((op == 10)) {
    if ((a >= b)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((op == 11)) {
    if ((a != 0)) {
      if ((b != 0)) {
        return 1;
      } else {
      }
    } else {
    }
    return 0;
  } else {
  }
  if ((op == 12)) {
    if ((a != 0)) {
      return 1;
    } else {
    }
    if ((b != 0)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  return 0;
}

static int cexpr_eval(int ct, int cd1, int cd2, int cd3, int idx, int names, int vals, int result_val) {
  int _sv0t0 = sv0_vec_get(ct, idx);
  int tag = _sv0t0;
  if ((tag == 0)) {
    int _sv0t1 = sv0_vec_get(cd1, idx);
    return _sv0t1;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t2 = sv0_vec_get(cd1, idx);
    return _sv0t2;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t3 = sv0_vec_get(cd1, idx);
    int _sv0t4 = vc_lookup(names, vals, _sv0t3);
    return _sv0t4;
  } else {
  }
  if ((tag == 3)) {
    return result_val;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t5 = sv0_vec_get(cd2, idx);
    int _sv0t6 = cexpr_eval(ct, cd1, cd2, cd3, _sv0t5, names, vals, result_val);
    return _sv0t6;
  } else {
  }
  if ((tag == 5)) {
    int _sv0t7 = sv0_vec_get(cd2, idx);
    int _sv0t8 = cexpr_eval(ct, cd1, cd2, cd3, _sv0t7, names, vals, result_val);
    int v = _sv0t8;
    int _sv0t9 = sv0_vec_get(cd1, idx);
    int op = _sv0t9;
    if ((op == 0)) {
      int _sv0t10 = (0 - v);
      return _sv0t10;
    } else {
    }
    if ((v != 0)) {
      return 0;
    } else {
    }
    return 1;
  } else {
  }
  int _sv0t11 = sv0_vec_get(cd2, idx);
  int _sv0t12 = cexpr_eval(ct, cd1, cd2, cd3, _sv0t11, names, vals, result_val);
  int a = _sv0t12;
  int _sv0t13 = sv0_vec_get(cd3, idx);
  int _sv0t14 = cexpr_eval(ct, cd1, cd2, cd3, _sv0t13, names, vals, result_val);
  int b = _sv0t14;
  int _sv0t15 = sv0_vec_get(cd1, idx);
  int _sv0t16 = vc_binop_eval(_sv0t15, a, b);
  return _sv0t16;
}

static const char* vc_binop_smt(int op) {
  if ((op == 0)) {
    return "+";
  } else {
  }
  if ((op == 1)) {
    return "-";
  } else {
  }
  if ((op == 2)) {
    return "*";
  } else {
  }
  if ((op == 3)) {
    return "div";
  } else {
  }
  if ((op == 4)) {
    return "mod";
  } else {
  }
  if ((op == 5)) {
    return "=";
  } else {
  }
  if ((op == 6)) {
    return "distinct";
  } else {
  }
  if ((op == 7)) {
    return "<";
  } else {
  }
  if ((op == 8)) {
    return "<=";
  } else {
  }
  if ((op == 9)) {
    return ">";
  } else {
  }
  if ((op == 10)) {
    return ">=";
  } else {
  }
  if ((op == 11)) {
    return "and";
  } else {
  }
  return "or";
}

static const char* vc_unop_smt(int op) {
  if ((op == 0)) {
    return "-";
  } else {
  }
  return "not";
}

static const char* vc_smt_int(int n) {
  if ((n < 0)) {
    int _sv0t0 = (0 - n);
    const char* _sv0t1 = vc_int_to_str(_sv0t0);
    const char* _sv0t2 = sv0_string_concat("(- ", _sv0t1);
    const char* _sv0t3 = sv0_string_concat(_sv0t2, ")");
    return _sv0t3;
  } else {
  }
  const char* _sv0t4 = vc_int_to_str(n);
  return _sv0t4;
}

static const char* cexpr_to_smt(int ct, int cd1, int cd2, int cd3, int idx) {
  int _sv0t0 = sv0_vec_get(ct, idx);
  int tag = _sv0t0;
  if ((tag == 0)) {
    int _sv0t1 = sv0_vec_get(cd1, idx);
    const char* _sv0t2 = vc_smt_int(_sv0t1);
    return _sv0t2;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t3 = sv0_vec_get(cd1, idx);
    if ((_sv0t3 != 0)) {
      return "true";
    } else {
    }
    return "false";
  } else {
  }
  if ((tag == 2)) {
    int _sv0t4 = sv0_vec_get(cd1, idx);
    const char* _sv0t5 = vc_int_to_str(_sv0t4);
    const char* _sv0t6 = sv0_string_concat("v", _sv0t5);
    return _sv0t6;
  } else {
  }
  if ((tag == 3)) {
    return "result";
  } else {
  }
  if ((tag == 4)) {
    int _sv0t7 = sv0_vec_get(cd2, idx);
    const char* _sv0t8 = cexpr_to_smt(ct, cd1, cd2, cd3, _sv0t7);
    return _sv0t8;
  } else {
  }
  if ((tag == 5)) {
    int _sv0t9 = sv0_vec_get(cd2, idx);
    const char* _sv0t10 = cexpr_to_smt(ct, cd1, cd2, cd3, _sv0t9);
    const char* inner;
    inner = _sv0t10;
    int _sv0t11 = sv0_vec_get(cd1, idx);
    const char* _sv0t12 = vc_unop_smt(_sv0t11);
    const char* _sv0t13 = sv0_string_concat("(", _sv0t12);
    const char* h1;
    h1 = _sv0t13;
    const char* _sv0t14 = sv0_string_concat(h1, " ");
    const char* h2;
    h2 = _sv0t14;
    const char* _sv0t15 = sv0_string_concat(h2, inner);
    const char* h3;
    h3 = _sv0t15;
    const char* _sv0t16 = sv0_string_concat(h3, ")");
    return _sv0t16;
  } else {
  }
  int _sv0t17 = sv0_vec_get(cd2, idx);
  const char* _sv0t18 = cexpr_to_smt(ct, cd1, cd2, cd3, _sv0t17);
  const char* ls;
  ls = _sv0t18;
  int _sv0t19 = sv0_vec_get(cd3, idx);
  const char* _sv0t20 = cexpr_to_smt(ct, cd1, cd2, cd3, _sv0t19);
  const char* rs;
  rs = _sv0t20;
  int _sv0t21 = sv0_vec_get(cd1, idx);
  const char* _sv0t22 = vc_binop_smt(_sv0t21);
  const char* _sv0t23 = sv0_string_concat("(", _sv0t22);
  const char* h1;
  h1 = _sv0t23;
  const char* _sv0t24 = sv0_string_concat(h1, " ");
  const char* h2;
  h2 = _sv0t24;
  const char* _sv0t25 = sv0_string_concat(h2, ls);
  const char* h3;
  h3 = _sv0t25;
  const char* _sv0t26 = sv0_string_concat(h3, " ");
  const char* h4;
  h4 = _sv0t26;
  const char* _sv0t27 = sv0_string_concat(h4, rs);
  const char* h5;
  h5 = _sv0t27;
  const char* _sv0t28 = sv0_string_concat(h5, ")");
  return _sv0t28;
}

static int vc_parse_dec(const char* source, int starts, int ends, int tok) {
  if ((tok < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(starts);
  if ((tok >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(starts, tok);
  int s = _sv0t1;
  int _sv0t2 = sv0_vec_get(ends, tok);
  int e = _sv0t2;
  if ((e <= s)) {
    return 0;
  } else {
  }
  int acc = 0;
  int i = s;
  while ((i < e)) {
    int _sv0t3 = sv0_string_char_at(source, i);
    int c = _sv0t3;
    if ((c < 48)) {
      return 0;
    } else {
    }
    if ((c > 57)) {
      return 0;
    } else {
    }
    int _sv0t4 = (acc * 10);
    int _sv0t5 = (c - 48);
    acc = (_sv0t4 + _sv0t5);
    i = (i + 1);
  }
  return acc;
}

static const char* vc_tok_text(const char* source, int starts, int ends, int tok) {
  if ((tok < 0)) {
    return "";
  } else {
  }
  int _sv0t0 = sv0_vec_len(starts);
  if ((tok >= _sv0t0)) {
    return "";
  } else {
  }
  int _sv0t1 = sv0_vec_get(starts, tok);
  int s = _sv0t1;
  int _sv0t2 = sv0_vec_get(ends, tok);
  int e = _sv0t2;
  if ((e <= s)) {
    return "";
  } else {
  }
  int _sv0t3 = (e - s);
  const char* _sv0t4 = sv0_string_substr(source, s, _sv0t3);
  return _sv0t4;
}

static int vc_tok_name_eq(const char* source, int starts, int ends, int tok, const char* name) {
  const char* _sv0t0 = vc_tok_text(source, starts, ends, tok);
  int _sv0t1 = sv0_string_eq(_sv0t0, name);
  return _sv0t1;
}

static int vc_intern_var(int var_names, const char* source, int starts, int ends, int tok) {
  const char* _sv0t0 = vc_tok_text(source, starts, ends, tok);
  const char* nm;
  nm = _sv0t0;
  int _sv0t1 = sv0_vec_len(var_names);
  int n = _sv0t1;
  int i = 0;
  while ((i < n)) {
    int _sv0t2 = sv0_vec_get(var_names, i);
    const char* _sv0t3 = vc_tok_text(source, starts, ends, _sv0t2);
    int _sv0t4 = sv0_string_eq(_sv0t3, nm);
    if (_sv0t4) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  sv0_vec_push(var_names, tok);
  return n;
}

static int vc_block_idx(int pp, int first, int sidecar, int i) {
  if ((sidecar > 0)) {
    int _sv0t0 = (sidecar - 1);
    int _sv0t1 = (_sv0t0 + i);
    int _sv0t2 = sv0_vec_get(pp, _sv0t1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = (first + i);
  return _sv0t3;
}

static int ast_binop_to_cexpr(int tag) {
  if ((tag < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  if ((tag <= 7)) {
    return tag;
  } else {
  }
  if ((tag == 8)) {
    return 9;
  } else {
  }
  if ((tag == 9)) {
    return 8;
  } else {
  }
  if ((tag == 10)) {
    return 10;
  } else {
  }
  if ((tag == 11)) {
    return 11;
  } else {
  }
  if ((tag == 12)) {
    return 12;
  } else {
  }
  int _sv0t1 = (0 - 1);
  return _sv0t1;
}

static int extract_cexpr(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int idx, const char* source, int starts, int ends, int tok_tags, int ct, int cd1, int cd2, int cd3, int var_names) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_len(bet);
  if ((idx >= _sv0t1)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(bet, idx);
  int tag = _sv0t3;
  if ((tag == 0)) {
    int _sv0t4 = sv0_vec_get(bed1, idx);
    int lit_tag = _sv0t4;
    int _sv0t5 = sv0_vec_get(bed2, idx);
    int tok = _sv0t5;
    if ((lit_tag == 0)) {
      int _sv0t6 = vc_parse_dec(source, starts, ends, tok);
      int _sv0t7 = cx_int(ct, cd1, cd2, cd3, _sv0t6);
      return _sv0t7;
    } else {
    }
    if ((lit_tag == 5)) {
      if ((tok >= 0)) {
        int _sv0t8 = sv0_vec_len(tok_tags);
        if ((tok < _sv0t8)) {
          int _sv0t9 = sv0_vec_get(tok_tags, tok);
          if ((_sv0t9 == 91)) {
            int _sv0t10 = cx_bool(ct, cd1, cd2, cd3, 1);
            return _sv0t10;
          } else {
          }
        } else {
        }
      } else {
      }
      int _sv0t11 = cx_bool(ct, cd1, cd2, cd3, 0);
      return _sv0t11;
    } else {
    }
    int _sv0t12 = (0 - 1);
    return _sv0t12;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t13 = sv0_vec_get(bed2, idx);
    if ((_sv0t13 != 1)) {
      int _sv0t14 = (0 - 1);
      return _sv0t14;
    } else {
    }
    int _sv0t15 = sv0_vec_get(bed1, idx);
    int _sv0t16 = sv0_vec_get(bpp, _sv0t15);
    int nametok = _sv0t16;
    int _sv0t17 = vc_tok_name_eq(source, starts, ends, nametok, "result");
    if (_sv0t17) {
      int _sv0t18 = cx_result(ct, cd1, cd2, cd3);
      return _sv0t18;
    } else {
    }
    int _sv0t19 = vc_intern_var(var_names, source, starts, ends, nametok);
    int _sv0t20 = cx_var(ct, cd1, cd2, cd3, _sv0t19);
    return _sv0t20;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t21 = sv0_vec_get(bed1, idx);
    int uop = _sv0t21;
    if ((uop == 0)) {
      int _sv0t22 = sv0_vec_get(bed2, idx);
      int _sv0t23 = extract_cexpr(bet, bed1, bed2, bed3, bed4, bpp, _sv0t22, source, starts, ends, tok_tags, ct, cd1, cd2, cd3, var_names);
      int ch = _sv0t23;
      if ((ch < 0)) {
        int _sv0t24 = (0 - 1);
        return _sv0t24;
      } else {
      }
      int _sv0t25 = cx_unop(ct, cd1, cd2, cd3, 0, ch);
      return _sv0t25;
    } else {
    }
    if ((uop == 1)) {
      int _sv0t26 = sv0_vec_get(bed2, idx);
      int _sv0t27 = extract_cexpr(bet, bed1, bed2, bed3, bed4, bpp, _sv0t26, source, starts, ends, tok_tags, ct, cd1, cd2, cd3, var_names);
      int ch = _sv0t27;
      if ((ch < 0)) {
        int _sv0t28 = (0 - 1);
        return _sv0t28;
      } else {
      }
      int _sv0t29 = cx_unop(ct, cd1, cd2, cd3, 1, ch);
      return _sv0t29;
    } else {
    }
    int _sv0t30 = (0 - 1);
    return _sv0t30;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t31 = sv0_vec_get(bed1, idx);
    int _sv0t32 = ast_binop_to_cexpr(_sv0t31);
    int cop = _sv0t32;
    if ((cop < 0)) {
      int _sv0t33 = (0 - 1);
      return _sv0t33;
    } else {
    }
    int _sv0t34 = sv0_vec_get(bed2, idx);
    int _sv0t35 = extract_cexpr(bet, bed1, bed2, bed3, bed4, bpp, _sv0t34, source, starts, ends, tok_tags, ct, cd1, cd2, cd3, var_names);
    int l = _sv0t35;
    if ((l < 0)) {
      int _sv0t36 = (0 - 1);
      return _sv0t36;
    } else {
    }
    int _sv0t37 = sv0_vec_get(bed3, idx);
    int _sv0t38 = extract_cexpr(bet, bed1, bed2, bed3, bed4, bpp, _sv0t37, source, starts, ends, tok_tags, ct, cd1, cd2, cd3, var_names);
    int r = _sv0t38;
    if ((r < 0)) {
      int _sv0t39 = (0 - 1);
      return _sv0t39;
    } else {
    }
    int _sv0t40 = cx_binop(ct, cd1, cd2, cd3, cop, l, r);
    return _sv0t40;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t41 = sv0_vec_get(bed1, idx);
    int callee = _sv0t41;
    if ((callee >= 0)) {
      int _sv0t42 = sv0_vec_len(bet);
      if ((callee < _sv0t42)) {
        int _sv0t43 = sv0_vec_get(bet, callee);
        if ((_sv0t43 == 1)) {
          int _sv0t44 = sv0_vec_get(bed2, callee);
          if ((_sv0t44 == 1)) {
            int _sv0t45 = sv0_vec_get(bed1, callee);
            int _sv0t46 = sv0_vec_get(bpp, _sv0t45);
            int cnametok = _sv0t46;
            int _sv0t47 = vc_tok_name_eq(source, starts, ends, cnametok, "old");
            if (_sv0t47) {
              int _sv0t48 = sv0_vec_get(bed3, idx);
              if ((_sv0t48 == 1)) {
                int _sv0t49 = sv0_vec_get(bed2, idx);
                int _sv0t50 = sv0_vec_get(bed4, idx);
                int _sv0t51 = vc_block_idx(bpp, _sv0t49, _sv0t50, 0);
                int arg0 = _sv0t51;
                int _sv0t52 = extract_cexpr(bet, bed1, bed2, bed3, bed4, bpp, arg0, source, starts, ends, tok_tags, ct, cd1, cd2, cd3, var_names);
                int inner = _sv0t52;
                if ((inner < 0)) {
                  int _sv0t53 = (0 - 1);
                  return _sv0t53;
                } else {
                }
                int _sv0t54 = cx_old(ct, cd1, cd2, cd3, inner);
                return _sv0t54;
              } else {
              }
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    int _sv0t55 = (0 - 1);
    return _sv0t55;
  } else {
  }
  int _sv0t56 = (0 - 1);
  return _sv0t56;
}

static const char* vc_query_logic(int ct, int cd1) {
  int _sv0t0 = sv0_vec_len(ct);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(ct, i);
    if ((_sv0t1 == 6)) {
      int _sv0t2 = sv0_vec_get(cd1, i);
      int op = _sv0t2;
      if ((op == 2)) {
        return "QF_NIA";
      } else {
      }
      if ((op == 3)) {
        return "QF_NIA";
      } else {
      }
      if ((op == 4)) {
        return "QF_NIA";
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return "QF_LIA";
}

static int vc_uses_result(int ct) {
  int _sv0t0 = sv0_vec_len(ct);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(ct, i);
    if ((_sv0t1 == 3)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static const char* vc_build_query(int ct, int cd1, int cd2, int cd3, int hyps, int goal, int var_names) {
  const char* _sv0t0 = vc_query_logic(ct, cd1);
  const char* _sv0t1 = sv0_string_concat("(set-logic ", _sv0t0);
  const char* _sv0t2 = sv0_string_concat(_sv0t1, ")");
  const char* q;
  q = _sv0t2;
  int _sv0t3 = sv0_vec_len(var_names);
  int nv = _sv0t3;
  int i = 0;
  while ((i < nv)) {
    const char* _sv0t4 = vc_int_to_str(i);
    const char* _sv0t5 = sv0_string_concat(" (declare-const v", _sv0t4);
    const char* _sv0t6 = sv0_string_concat(_sv0t5, " Int)");
    const char* _sv0t7 = sv0_string_concat(q, _sv0t6);
    q = _sv0t7;
    i = (i + 1);
  }
  int _sv0t8 = vc_uses_result(ct);
  if (_sv0t8) {
    const char* _sv0t9 = sv0_string_concat(q, " (declare-const result Int)");
    q = _sv0t9;
  } else {
  }
  int _sv0t10 = sv0_vec_len(hyps);
  int nh = _sv0t10;
  int j = 0;
  while ((j < nh)) {
    int _sv0t11 = sv0_vec_get(hyps, j);
    const char* _sv0t12 = cexpr_to_smt(ct, cd1, cd2, cd3, _sv0t11);
    const char* hs;
    hs = _sv0t12;
    const char* _sv0t13 = sv0_string_concat(" (assert ", hs);
    const char* _sv0t14 = sv0_string_concat(_sv0t13, ")");
    const char* _sv0t15 = sv0_string_concat(q, _sv0t14);
    q = _sv0t15;
    j = (j + 1);
  }
  const char* _sv0t16 = cexpr_to_smt(ct, cd1, cd2, cd3, goal);
  const char* gs;
  gs = _sv0t16;
  const char* _sv0t17 = sv0_string_concat(" (assert (not ", gs);
  const char* _sv0t18 = sv0_string_concat(_sv0t17, "))");
  const char* _sv0t19 = sv0_string_concat(q, _sv0t18);
  q = _sv0t19;
  const char* _sv0t20 = sv0_string_concat(q, " (check-sat)");
  q = _sv0t20;
  return q;
}

static const char* vc_gen_ensures_query(int ct, int cd1, int cd2, int cd3, int requires_list, int ensures_root, int return_expr, int var_names) {
  int _sv0t0 = sv0_vec_new();
  int hyps = _sv0t0;
  int i = 0;
  while (1) {
    int _sv0t1 = sv0_vec_len(requires_list);
    int _sv0t3 = (i < _sv0t1);
    if ((!_sv0t3)) {
      break;
    } else {
    }
    int _sv0t2 = sv0_vec_get(requires_list, i);
    sv0_vec_push(hyps, _sv0t2);
    i = (i + 1);
  }
  if ((return_expr >= 0)) {
    int _sv0t4 = cx_result(ct, cd1, cd2, cd3);
    int rn = _sv0t4;
    int _sv0t5 = cx_binop(ct, cd1, cd2, cd3, 5, rn, return_expr);
    int eqn = _sv0t5;
    sv0_vec_push(hyps, eqn);
  } else {
  }
  const char* _sv0t6 = vc_build_query(ct, cd1, cd2, cd3, hyps, ensures_root, var_names);
  return _sv0t6;
}

static int vc_leftmost_leaf_tok(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int idx) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_len(bet);
  if ((idx >= _sv0t1)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(bet, idx);
  int tag = _sv0t3;
  if ((tag == 0)) {
    int _sv0t4 = sv0_vec_get(bed2, idx);
    return _sv0t4;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t5 = sv0_vec_get(bed1, idx);
    int _sv0t6 = sv0_vec_get(bpp, _sv0t5);
    return _sv0t6;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t7 = sv0_vec_get(bed2, idx);
    int _sv0t8 = vc_leftmost_leaf_tok(bet, bed1, bed2, bed3, bed4, bpp, _sv0t7);
    return _sv0t8;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t9 = sv0_vec_get(bed2, idx);
    int _sv0t10 = vc_leftmost_leaf_tok(bet, bed1, bed2, bed3, bed4, bpp, _sv0t9);
    return _sv0t10;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t11 = sv0_vec_get(bed1, idx);
    int _sv0t12 = vc_leftmost_leaf_tok(bet, bed1, bed2, bed3, bed4, bpp, _sv0t11);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = (0 - 1);
  return _sv0t13;
}

static int vc_contract_kind_tok(int tok_tags, int leaf) {
  if ((leaf < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int j = (leaf - 1);
  while ((j >= 0)) {
    int _sv0t1 = sv0_vec_get(tok_tags, j);
    int t = _sv0t1;
    if ((t == 83)) {
      return j;
    } else {
    }
    if ((t == 61)) {
      return j;
    } else {
    }
    if ((t == 73)) {
      return j;
    } else {
    }
    j = (j - 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int vc_contract_kind(int tok_tags, int leaf) {
  int _sv0t0 = vc_contract_kind_tok(tok_tags, leaf);
  int k = _sv0t0;
  if ((k < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(tok_tags, k);
  return _sv0t2;
}

static const char* vc_kind_name(int tag) {
  if ((tag == 83)) {
    return "requires";
  } else {
  }
  if ((tag == 61)) {
    return "ensures";
  } else {
  }
  if ((tag == 73)) {
    return "loop_invariant";
  } else {
  }
  return "contract";
}

static int vc_match_rparen(int tok_tags, int lparen) {
  int _sv0t0 = sv0_vec_len(tok_tags);
  int n = _sv0t0;
  if ((lparen < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int depth = 0;
  int j = lparen;
  while ((j < n)) {
    int _sv0t2 = sv0_vec_get(tok_tags, j);
    int t = _sv0t2;
    if ((t == 6)) {
      depth = (depth + 1);
    } else {
    }
    if ((t == 7)) {
      depth = (depth - 1);
      if ((depth == 0)) {
        return j;
      } else {
      }
    } else {
    }
    j = (j + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static const char* vc_clause_text(const char* source, int starts, int ends, int kw_tok, int rparen_tok) {
  if ((kw_tok < 0)) {
    return "";
  } else {
  }
  if ((rparen_tok < 0)) {
    return "";
  } else {
  }
  int _sv0t0 = sv0_vec_get(starts, kw_tok);
  int s = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, rparen_tok);
  int e = _sv0t1;
  if ((e <= s)) {
    return "";
  } else {
  }
  const char* out;
  out = "";
  int i = s;
  int prev_ws = 0;
  while ((i < e)) {
    int _sv0t2 = sv0_string_char_at(source, i);
    int c = _sv0t2;
    int is_ws = 0;
    if ((c == 32)) {
      is_ws = 1;
    } else {
    }
    if ((c == 9)) {
      is_ws = 1;
    } else {
    }
    if ((c == 10)) {
      is_ws = 1;
    } else {
    }
    if ((c == 13)) {
      is_ws = 1;
    } else {
    }
    if (is_ws) {
      if ((prev_ws != 1)) {
        const char* _sv0t3 = sv0_string_concat(out, " ");
        out = _sv0t3;
      } else {
      }
      prev_ws = 1;
    } else {
      const char* _sv0t4 = sv0_string_substr(source, i, 1);
      const char* _sv0t5 = sv0_string_concat(out, _sv0t4);
      out = _sv0t5;
      prev_ws = 0;
    }
    i = (i + 1);
  }
  return out;
}

static int vc_line_of_pos(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int n = _sv0t0;
  int line = 1;
  int i = 0;
  while ((i < pos)) {
    if ((i >= n)) {
      return line;
    } else {
    }
    int _sv0t1 = sv0_string_char_at(source, i);
    if ((_sv0t1 == 10)) {
      line = (line + 1);
    } else {
    }
    i = (i + 1);
  }
  return line;
}

static int vc_find_return_value(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int body_root) {
  if ((body_root < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_len(bet);
  if ((body_root >= _sv0t1)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(bet, body_root);
  int tag = _sv0t3;
  if ((tag != 9)) {
    return body_root;
  } else {
  }
  int _sv0t4 = sv0_vec_get(bed3, body_root);
  int tail = _sv0t4;
  if ((tail >= 0)) {
    return tail;
  } else {
  }
  int _sv0t5 = sv0_vec_get(bed1, body_root);
  int first = _sv0t5;
  int _sv0t6 = sv0_vec_get(bed2, body_root);
  int count = _sv0t6;
  int _sv0t7 = sv0_vec_get(bed4, body_root);
  int sidecar = _sv0t7;
  int si = 0;
  while ((si < count)) {
    int _sv0t8 = vc_block_idx(bpp, first, sidecar, si);
    int s = _sv0t8;
    if ((s >= 0)) {
      int _sv0t9 = sv0_vec_len(bet);
      if ((s < _sv0t9)) {
        int e = s;
        int _sv0t10 = sv0_vec_get(bet, s);
        if ((_sv0t10 == 28)) {
          int _sv0t11 = sv0_vec_get(bed1, s);
          e = _sv0t11;
        } else {
        }
        if ((e >= 0)) {
          int _sv0t12 = sv0_vec_len(bet);
          if ((e < _sv0t12)) {
            int _sv0t13 = sv0_vec_get(bet, e);
            if ((_sv0t13 == 15)) {
              int _sv0t14 = sv0_vec_get(bed1, e);
              return _sv0t14;
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    si = (si + 1);
  }
  int _sv0t15 = (0 - 1);
  return _sv0t15;
}

static const char* verify_all_fns(int it, int id1, int id2, int id3, int id4, int fcb, int fcr, int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int tok_tags, const char* source, int starts, int ends) {
  const char* out;
  out = "";
  int _sv0t0 = sv0_vec_len(it);
  int ni = _sv0t0;
  int ii = 0;
  while ((ii < ni)) {
    int _sv0t1 = sv0_vec_get(it, ii);
    if ((_sv0t1 == 0)) {
      int _sv0t2 = sv0_vec_get(fcb, ii);
      int base = _sv0t2;
      int _sv0t3 = sv0_vec_get(id2, ii);
      int count = (_sv0t3 / 2);
      int _sv0t4 = sv0_vec_get(id4, ii);
      int body_root = _sv0t4;
      int _sv0t5 = sv0_vec_new();
      int req_roots = _sv0t5;
      int ci = 0;
      while ((ci < count)) {
        int _sv0t6 = (base + ci);
        int _sv0t7 = sv0_vec_get(fcr, _sv0t6);
        int croot = _sv0t7;
        int _sv0t8 = vc_leftmost_leaf_tok(bet, bed1, bed2, bed3, bed4, bpp, croot);
        int leaf = _sv0t8;
        int _sv0t9 = vc_contract_kind(tok_tags, leaf);
        if ((_sv0t9 == 83)) {
          sv0_vec_push(req_roots, croot);
        } else {
        }
        ci = (ci + 1);
      }
      int cj = 0;
      while ((cj < count)) {
        int _sv0t10 = (base + cj);
        int _sv0t11 = sv0_vec_get(fcr, _sv0t10);
        int croot = _sv0t11;
        int _sv0t12 = vc_leftmost_leaf_tok(bet, bed1, bed2, bed3, bed4, bpp, croot);
        int leaf = _sv0t12;
        int _sv0t13 = vc_contract_kind_tok(tok_tags, leaf);
        int kwtok = _sv0t13;
        int kindtag = (0 - 1);
        if ((kwtok >= 0)) {
          int _sv0t14 = sv0_vec_get(tok_tags, kwtok);
          kindtag = _sv0t14;
        } else {
        }
        int _sv0t15 = (kwtok + 1);
        int _sv0t16 = vc_match_rparen(tok_tags, _sv0t15);
        int rparen = _sv0t16;
        int line = 1;
        if ((kwtok >= 0)) {
          int _sv0t17 = sv0_vec_get(starts, kwtok);
          int _sv0t18 = vc_line_of_pos(source, _sv0t17);
          line = _sv0t18;
        } else {
        }
        const char* _sv0t19 = sv0_string_concat(out, "VC\t");
        out = _sv0t19;
        const char* _sv0t20 = vc_int_to_str(line);
        const char* _sv0t21 = sv0_string_concat(out, _sv0t20);
        out = _sv0t21;
        const char* _sv0t22 = sv0_string_concat(out, "\t");
        out = _sv0t22;
        const char* _sv0t23 = vc_kind_name(kindtag);
        const char* _sv0t24 = sv0_string_concat(out, _sv0t23);
        out = _sv0t24;
        const char* _sv0t25 = sv0_string_concat(out, "\t");
        out = _sv0t25;
        const char* _sv0t26 = vc_clause_text(source, starts, ends, kwtok, rparen);
        const char* _sv0t27 = sv0_string_concat(out, _sv0t26);
        out = _sv0t27;
        const char* _sv0t28 = sv0_string_concat(out, "\t");
        out = _sv0t28;
        if ((kindtag == 83)) {
          const char* _sv0t29 = sv0_string_concat(out, "PRECOND");
          out = _sv0t29;
        } else {
          if ((kindtag == 61)) {
            const char* _sv0t30 = verify_ensures_payload(bet, bed1, bed2, bed3, bed4, bpp, req_roots, croot, body_root, tok_tags, source, starts, ends);
            const char* _sv0t31 = sv0_string_concat(out, _sv0t30);
            out = _sv0t31;
          } else {
            const char* _sv0t32 = sv0_string_concat(out, "RESIDUAL");
            out = _sv0t32;
          }
        }
        const char* _sv0t33 = sv0_string_concat(out, "\n");
        out = _sv0t33;
        cj = (cj + 1);
      }
    } else {
    }
    ii = (ii + 1);
  }
  return out;
}

static const char* verify_ensures_payload(int bet, int bed1, int bed2, int bed3, int bed4, int bpp, int req_roots, int ens_root, int body_root, int tok_tags, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_new();
  int ct = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int c1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int c2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int c3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int vn = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int reqs = _sv0t5;
  int ri = 0;
  while (1) {
    int _sv0t6 = sv0_vec_len(req_roots);
    int _sv0t9 = (ri < _sv0t6);
    if ((!_sv0t9)) {
      break;
    } else {
    }
    int _sv0t7 = sv0_vec_get(req_roots, ri);
    int _sv0t8 = extract_cexpr(bet, bed1, bed2, bed3, bed4, bpp, _sv0t7, source, starts, ends, tok_tags, ct, c1, c2, c3, vn);
    int rc = _sv0t8;
    if ((rc >= 0)) {
      sv0_vec_push(reqs, rc);
    } else {
    }
    ri = (ri + 1);
  }
  int _sv0t10 = extract_cexpr(bet, bed1, bed2, bed3, bed4, bpp, ens_root, source, starts, ends, tok_tags, ct, c1, c2, c3, vn);
  int ec = _sv0t10;
  if ((ec < 0)) {
    return "RESIDUAL";
  } else {
  }
  int ret_ce = (0 - 1);
  int _sv0t11 = vc_find_return_value(bet, bed1, bed2, bed3, bed4, bpp, body_root);
  int retv = _sv0t11;
  if ((retv >= 0)) {
    int _sv0t12 = extract_cexpr(bet, bed1, bed2, bed3, bed4, bpp, retv, source, starts, ends, tok_tags, ct, c1, c2, c3, vn);
    ret_ce = _sv0t12;
  } else {
  }
  const char* _sv0t13 = vc_gen_ensures_query(ct, c1, c2, c3, reqs, ec, ret_ce, vn);
  return _sv0t13;
}

static int test_cx_tags(void) {
  int _sv0t0 = sv0_vec_new();
  int ct = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int cd1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int cd2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int cd3 = _sv0t3;
  int _sv0t4 = cx_int(ct, cd1, cd2, cd3, 5);
  int a = _sv0t4;
  int _sv0t5 = sv0_vec_get(ct, a);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_vec_get(cd1, a);
  if ((_sv0t6 != 5)) {
    return 2;
  } else {
  }
  int _sv0t7 = cx_bool(ct, cd1, cd2, cd3, 1);
  int b = _sv0t7;
  int _sv0t8 = sv0_vec_get(ct, b);
  if ((_sv0t8 != 1)) {
    return 3;
  } else {
  }
  int _sv0t9 = cx_var(ct, cd1, cd2, cd3, 3);
  int v = _sv0t9;
  int _sv0t10 = sv0_vec_get(ct, v);
  if ((_sv0t10 != 2)) {
    return 4;
  } else {
  }
  int _sv0t11 = cx_result(ct, cd1, cd2, cd3);
  int r = _sv0t11;
  int _sv0t12 = sv0_vec_get(ct, r);
  if ((_sv0t12 != 3)) {
    return 5;
  } else {
  }
  int _sv0t13 = cx_old(ct, cd1, cd2, cd3, v);
  int o = _sv0t13;
  int _sv0t14 = sv0_vec_get(ct, o);
  if ((_sv0t14 != 4)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(cd2, o);
  if ((_sv0t15 != v)) {
    return 7;
  } else {
  }
  int _sv0t16 = cx_unop(ct, cd1, cd2, cd3, 1, b);
  int u = _sv0t16;
  int _sv0t17 = sv0_vec_get(ct, u);
  if ((_sv0t17 != 5)) {
    return 8;
  } else {
  }
  int _sv0t18 = cx_binop(ct, cd1, cd2, cd3, 0, a, v);
  int bin = _sv0t18;
  int _sv0t19 = sv0_vec_get(ct, bin);
  if ((_sv0t19 != 6)) {
    return 9;
  } else {
  }
  int _sv0t20 = sv0_vec_get(cd2, bin);
  if ((_sv0t20 != a)) {
    return 10;
  } else {
  }
  int _sv0t21 = sv0_vec_get(cd3, bin);
  if ((_sv0t21 != v)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_eval_arith(void) {
  int _sv0t0 = sv0_vec_new();
  int ct = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int cd1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int cd2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int cd3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int names = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int vals = _sv0t5;
  int _sv0t6 = cx_int(ct, cd1, cd2, cd3, 2);
  int i2 = _sv0t6;
  int _sv0t7 = cx_int(ct, cd1, cd2, cd3, 3);
  int i3 = _sv0t7;
  int _sv0t8 = cx_int(ct, cd1, cd2, cd3, 4);
  int i4 = _sv0t8;
  int _sv0t9 = cx_binop(ct, cd1, cd2, cd3, 2, i3, i4);
  int mul = _sv0t9;
  int _sv0t10 = cx_binop(ct, cd1, cd2, cd3, 0, i2, mul);
  int add = _sv0t10;
  int _sv0t11 = cexpr_eval(ct, cd1, cd2, cd3, add, names, vals, 0);
  if ((_sv0t11 != 14)) {
    return 1;
  } else {
  }
  int _sv0t12 = cx_int(ct, cd1, cd2, cd3, 20);
  int i20 = _sv0t12;
  int _sv0t13 = cx_int(ct, cd1, cd2, cd3, 4);
  int i4b = _sv0t13;
  int _sv0t14 = cx_binop(ct, cd1, cd2, cd3, 3, i20, i4b);
  int dv = _sv0t14;
  int _sv0t15 = cexpr_eval(ct, cd1, cd2, cd3, dv, names, vals, 0);
  if ((_sv0t15 != 5)) {
    return 2;
  } else {
  }
  int _sv0t16 = cx_int(ct, cd1, cd2, cd3, 17);
  int i17 = _sv0t16;
  int _sv0t17 = cx_int(ct, cd1, cd2, cd3, 5);
  int i5 = _sv0t17;
  int _sv0t18 = cx_binop(ct, cd1, cd2, cd3, 4, i17, i5);
  int md = _sv0t18;
  int _sv0t19 = cexpr_eval(ct, cd1, cd2, cd3, md, names, vals, 0);
  if ((_sv0t19 != 2)) {
    return 3;
  } else {
  }
  int _sv0t20 = cx_int(ct, cd1, cd2, cd3, 7);
  int i7 = _sv0t20;
  int _sv0t21 = cx_unop(ct, cd1, cd2, cd3, 0, i7);
  int ng = _sv0t21;
  int _sv0t22 = cexpr_eval(ct, cd1, cd2, cd3, ng, names, vals, 0);
  int _sv0t23 = (0 - 7);
  if ((_sv0t22 != _sv0t23)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_eval_compare(void) {
  int _sv0t0 = sv0_vec_new();
  int ct = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int cd1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int cd2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int cd3 = _sv0t3;
  int _sv0t4 = cx_var(ct, cd1, cd2, cd3, 1);
  int vx = _sv0t4;
  int _sv0t5 = cx_int(ct, cd1, cd2, cd3, 0);
  int z = _sv0t5;
  int _sv0t6 = cx_binop(ct, cd1, cd2, cd3, 9, vx, z);
  int gt = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int names = _sv0t7;
  sv0_vec_push(names, 1);
  int _sv0t8 = sv0_vec_new();
  int vpos = _sv0t8;
  sv0_vec_push(vpos, 5);
  int _sv0t9 = cexpr_eval(ct, cd1, cd2, cd3, gt, names, vpos, 0);
  if ((_sv0t9 != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int vneg = _sv0t10;
  int _sv0t11 = (0 - 2);
  sv0_vec_push(vneg, _sv0t11);
  int _sv0t12 = cexpr_eval(ct, cd1, cd2, cd3, gt, names, vneg, 0);
  if ((_sv0t12 != 0)) {
    return 2;
  } else {
  }
  int _sv0t13 = cx_int(ct, cd1, cd2, cd3, 5);
  int a5 = _sv0t13;
  int _sv0t14 = cx_int(ct, cd1, cd2, cd3, 5);
  int b5 = _sv0t14;
  int _sv0t15 = cx_binop(ct, cd1, cd2, cd3, 5, a5, b5);
  int eq = _sv0t15;
  int _sv0t16 = cexpr_eval(ct, cd1, cd2, cd3, eq, names, vpos, 0);
  if ((_sv0t16 != 1)) {
    return 3;
  } else {
  }
  int _sv0t17 = cx_int(ct, cd1, cd2, cd3, 6);
  int c6 = _sv0t17;
  int _sv0t18 = cx_binop(ct, cd1, cd2, cd3, 6, a5, c6);
  int ne = _sv0t18;
  int _sv0t19 = cexpr_eval(ct, cd1, cd2, cd3, ne, names, vpos, 0);
  if ((_sv0t19 != 1)) {
    return 4;
  } else {
  }
  int _sv0t20 = cx_binop(ct, cd1, cd2, cd3, 8, a5, b5);
  int le = _sv0t20;
  int _sv0t21 = cexpr_eval(ct, cd1, cd2, cd3, le, names, vpos, 0);
  if ((_sv0t21 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_eval_bool(void) {
  int _sv0t0 = sv0_vec_new();
  int ct = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int cd1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int cd2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int cd3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int names = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int vals = _sv0t5;
  int _sv0t6 = cx_bool(ct, cd1, cd2, cd3, 1);
  int bt = _sv0t6;
  int _sv0t7 = cx_bool(ct, cd1, cd2, cd3, 0);
  int bf = _sv0t7;
  int _sv0t8 = cx_binop(ct, cd1, cd2, cd3, 11, bt, bf);
  int an = _sv0t8;
  int _sv0t9 = cexpr_eval(ct, cd1, cd2, cd3, an, names, vals, 0);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = cx_binop(ct, cd1, cd2, cd3, 12, bt, bf);
  int orr = _sv0t10;
  int _sv0t11 = cexpr_eval(ct, cd1, cd2, cd3, orr, names, vals, 0);
  if ((_sv0t11 != 1)) {
    return 2;
  } else {
  }
  int _sv0t12 = cx_unop(ct, cd1, cd2, cd3, 1, bf);
  int nt = _sv0t12;
  int _sv0t13 = cexpr_eval(ct, cd1, cd2, cd3, nt, names, vals, 0);
  if ((_sv0t13 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_eval_result_old(void) {
  int _sv0t0 = sv0_vec_new();
  int ct = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int cd1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int cd2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int cd3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int names = _sv0t4;
  sv0_vec_push(names, 1);
  int _sv0t5 = sv0_vec_new();
  int vals = _sv0t5;
  sv0_vec_push(vals, 7);
  int _sv0t6 = cx_result(ct, cd1, cd2, cd3);
  int r = _sv0t6;
  int _sv0t7 = cx_var(ct, cd1, cd2, cd3, 1);
  int vx = _sv0t7;
  int _sv0t8 = cx_binop(ct, cd1, cd2, cd3, 5, r, vx);
  int eq = _sv0t8;
  int _sv0t9 = cexpr_eval(ct, cd1, cd2, cd3, eq, names, vals, 7);
  if ((_sv0t9 != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = cx_result(ct, cd1, cd2, cd3);
  int r2 = _sv0t10;
  int _sv0t11 = cx_var(ct, cd1, cd2, cd3, 1);
  int vx2 = _sv0t11;
  int _sv0t12 = cx_old(ct, cd1, cd2, cd3, vx2);
  int ol = _sv0t12;
  int _sv0t13 = cx_binop(ct, cd1, cd2, cd3, 9, r2, ol);
  int gt = _sv0t13;
  int _sv0t14 = cexpr_eval(ct, cd1, cd2, cd3, gt, names, vals, 9);
  if ((_sv0t14 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_smt_emit(void) {
  int _sv0t0 = sv0_vec_new();
  int ct = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int cd1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int cd2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int cd3 = _sv0t3;
  int _sv0t4 = cx_var(ct, cd1, cd2, cd3, 1);
  int vx = _sv0t4;
  int _sv0t5 = cx_int(ct, cd1, cd2, cd3, 0);
  int z = _sv0t5;
  int _sv0t6 = cx_binop(ct, cd1, cd2, cd3, 9, vx, z);
  int gt = _sv0t6;
  const char* _sv0t7 = cexpr_to_smt(ct, cd1, cd2, cd3, gt);
  int _sv0t8 = sv0_string_eq(_sv0t7, "(> v1 0)");
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = cx_var(ct, cd1, cd2, cd3, 2);
  int v2 = _sv0t9;
  int _sv0t10 = cx_var(ct, cd1, cd2, cd3, 3);
  int v3 = _sv0t10;
  int _sv0t11 = cx_binop(ct, cd1, cd2, cd3, 6, v2, v3);
  int ne = _sv0t11;
  const char* _sv0t12 = cexpr_to_smt(ct, cd1, cd2, cd3, ne);
  int _sv0t13 = sv0_string_eq(_sv0t12, "(distinct v2 v3)");
  if ((_sv0t13 != 1)) {
    return 2;
  } else {
  }
  int _sv0t14 = cx_result(ct, cd1, cd2, cd3);
  int r = _sv0t14;
  int _sv0t15 = cx_int(ct, cd1, cd2, cd3, 5);
  int i5 = _sv0t15;
  int _sv0t16 = cx_binop(ct, cd1, cd2, cd3, 5, r, i5);
  int eq = _sv0t16;
  const char* _sv0t17 = cexpr_to_smt(ct, cd1, cd2, cd3, eq);
  int _sv0t18 = sv0_string_eq(_sv0t17, "(= result 5)");
  if ((_sv0t18 != 1)) {
    return 3;
  } else {
  }
  int _sv0t19 = cx_var(ct, cd1, cd2, cd3, 1);
  int vx2 = _sv0t19;
  int _sv0t20 = cx_int(ct, cd1, cd2, cd3, 0);
  int z2 = _sv0t20;
  int _sv0t21 = cx_binop(ct, cd1, cd2, cd3, 9, vx2, z2);
  int lo = _sv0t21;
  int _sv0t22 = cx_var(ct, cd1, cd2, cd3, 1);
  int vx3 = _sv0t22;
  int _sv0t23 = cx_int(ct, cd1, cd2, cd3, 10);
  int t10 = _sv0t23;
  int _sv0t24 = cx_binop(ct, cd1, cd2, cd3, 7, vx3, t10);
  int hi = _sv0t24;
  int _sv0t25 = cx_binop(ct, cd1, cd2, cd3, 11, lo, hi);
  int both = _sv0t25;
  const char* _sv0t26 = cexpr_to_smt(ct, cd1, cd2, cd3, both);
  int _sv0t27 = sv0_string_eq(_sv0t26, "(and (> v1 0) (< v1 10))");
  if ((_sv0t27 != 1)) {
    return 4;
  } else {
  }
  int _sv0t28 = (0 - 3);
  int _sv0t29 = cx_int(ct, cd1, cd2, cd3, _sv0t28);
  int nn = _sv0t29;
  const char* _sv0t30 = cexpr_to_smt(ct, cd1, cd2, cd3, nn);
  int _sv0t31 = sv0_string_eq(_sv0t30, "(- 3)");
  if ((_sv0t31 != 1)) {
    return 5;
  } else {
  }
  int _sv0t32 = cx_bool(ct, cd1, cd2, cd3, 1);
  int bt = _sv0t32;
  int _sv0t33 = cx_unop(ct, cd1, cd2, cd3, 1, bt);
  int nb = _sv0t33;
  const char* _sv0t34 = cexpr_to_smt(ct, cd1, cd2, cd3, nb);
  int _sv0t35 = sv0_string_eq(_sv0t34, "(not true)");
  if ((_sv0t35 != 1)) {
    return 6;
  } else {
  }
  return 0;
}

static int be_push(int bet, int bed1, int bed2, int bed3, int bed4, int tag, int d1, int d2, int d3, int d4) {
  sv0_vec_push(bet, tag);
  sv0_vec_push(bed1, d1);
  sv0_vec_push(bed2, d2);
  sv0_vec_push(bed3, d3);
  sv0_vec_push(bed4, d4);
  int _sv0t0 = sv0_vec_len(bet);
  int _sv0t1 = (_sv0t0 - 1);
  return _sv0t1;
}

static const char* vc_test_vocab(int starts, int ends, int tok_tags) {
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tok_tags, 5);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tok_tags, 5);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tok_tags, 84);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tok_tags, 5);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(tok_tags, 40);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tok_tags, 40);
  return "xyresultold01";
}

static int test_extract_compare(void) {
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  const char* _sv0t3 = vc_test_vocab(starts, ends, tt);
  const char* src;
  src = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int bet = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int b1 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int b2 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int b3 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int b4 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bpp = _sv0t9;
  sv0_vec_push(bpp, 0);
  int _sv0t10 = be_push(bet, b1, b2, b3, b4, 1, 0, 1, 0, 0);
  int nx = _sv0t10;
  int _sv0t11 = be_push(bet, b1, b2, b3, b4, 0, 0, 4, 0, 0);
  int nz = _sv0t11;
  int _sv0t12 = be_push(bet, b1, b2, b3, b4, 3, 8, nx, nz, 0);
  int root = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int ct = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int c1 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int c2 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int c3 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int vn = _sv0t17;
  int _sv0t18 = extract_cexpr(bet, b1, b2, b3, b4, bpp, root, src, starts, ends, tt, ct, c1, c2, c3, vn);
  int ce = _sv0t18;
  if ((ce < 0)) {
    return 1;
  } else {
  }
  const char* _sv0t19 = cexpr_to_smt(ct, c1, c2, c3, ce);
  int _sv0t20 = sv0_string_eq(_sv0t19, "(> v0 0)");
  if ((_sv0t20 != 1)) {
    return 2;
  } else {
  }
  int _sv0t21 = sv0_vec_len(vn);
  if ((_sv0t21 != 1)) {
    return 3;
  } else {
  }
  int _sv0t22 = sv0_vec_new();
  int nm = _sv0t22;
  sv0_vec_push(nm, 0);
  int _sv0t23 = sv0_vec_new();
  int vp = _sv0t23;
  sv0_vec_push(vp, 5);
  int _sv0t24 = cexpr_eval(ct, c1, c2, c3, ce, nm, vp, 0);
  if ((_sv0t24 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_extract_result_arith(void) {
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  const char* _sv0t3 = vc_test_vocab(starts, ends, tt);
  const char* src;
  src = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int bet = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int b1 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int b2 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int b3 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int b4 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bpp = _sv0t9;
  sv0_vec_push(bpp, 2);
  sv0_vec_push(bpp, 0);
  sv0_vec_push(bpp, 1);
  int _sv0t10 = be_push(bet, b1, b2, b3, b4, 1, 0, 1, 0, 0);
  int nr = _sv0t10;
  int _sv0t11 = be_push(bet, b1, b2, b3, b4, 1, 1, 1, 0, 0);
  int nx = _sv0t11;
  int _sv0t12 = be_push(bet, b1, b2, b3, b4, 1, 2, 1, 0, 0);
  int ny = _sv0t12;
  int _sv0t13 = be_push(bet, b1, b2, b3, b4, 3, 0, nx, ny, 0);
  int sum = _sv0t13;
  int _sv0t14 = be_push(bet, b1, b2, b3, b4, 3, 5, nr, sum, 0);
  int root = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ct = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int c1 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int c2 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int c3 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int vn = _sv0t19;
  int _sv0t20 = extract_cexpr(bet, b1, b2, b3, b4, bpp, root, src, starts, ends, tt, ct, c1, c2, c3, vn);
  int ce = _sv0t20;
  if ((ce < 0)) {
    return 1;
  } else {
  }
  const char* _sv0t21 = cexpr_to_smt(ct, c1, c2, c3, ce);
  int _sv0t22 = sv0_string_eq(_sv0t21, "(= result (+ v0 v1))");
  if ((_sv0t22 != 1)) {
    return 2;
  } else {
  }
  int _sv0t23 = sv0_vec_len(vn);
  if ((_sv0t23 != 2)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_extract_old(void) {
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  const char* _sv0t3 = vc_test_vocab(starts, ends, tt);
  const char* src;
  src = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int bet = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int b1 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int b2 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int b3 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int b4 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bpp = _sv0t9;
  sv0_vec_push(bpp, 2);
  sv0_vec_push(bpp, 3);
  sv0_vec_push(bpp, 0);
  int _sv0t10 = be_push(bet, b1, b2, b3, b4, 1, 0, 1, 0, 0);
  int nr = _sv0t10;
  int _sv0t11 = be_push(bet, b1, b2, b3, b4, 1, 1, 1, 0, 0);
  int nold = _sv0t11;
  int _sv0t12 = be_push(bet, b1, b2, b3, b4, 1, 2, 1, 0, 0);
  int nx = _sv0t12;
  sv0_vec_push(bpp, nx);
  int _sv0t13 = be_push(bet, b1, b2, b3, b4, 4, nold, 0, 1, 4);
  int ncall = _sv0t13;
  int _sv0t14 = be_push(bet, b1, b2, b3, b4, 3, 8, nr, ncall, 0);
  int root = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ct = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int c1 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int c2 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int c3 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int vn = _sv0t19;
  int _sv0t20 = extract_cexpr(bet, b1, b2, b3, b4, bpp, root, src, starts, ends, tt, ct, c1, c2, c3, vn);
  int ce = _sv0t20;
  if ((ce < 0)) {
    return 1;
  } else {
  }
  const char* _sv0t21 = cexpr_to_smt(ct, c1, c2, c3, ce);
  int _sv0t22 = sv0_string_eq(_sv0t21, "(> result v0)");
  if ((_sv0t22 != 1)) {
    return 2;
  } else {
  }
  int _sv0t23 = sv0_vec_len(vn);
  if ((_sv0t23 != 1)) {
    return 3;
  } else {
  }
  int _sv0t24 = sv0_vec_get(c3, ce);
  int _sv0t25 = sv0_vec_get(ct, _sv0t24);
  if ((_sv0t25 != 4)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_extract_unsupported(void) {
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  const char* _sv0t3 = vc_test_vocab(starts, ends, tt);
  const char* src;
  src = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int bet = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int b1 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int b2 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int b3 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int b4 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bpp = _sv0t9;
  sv0_vec_push(bpp, 0);
  int _sv0t10 = be_push(bet, b1, b2, b3, b4, 1, 0, 1, 0, 0);
  int nx = _sv0t10;
  int _sv0t11 = be_push(bet, b1, b2, b3, b4, 0, 0, 5, 0, 0);
  int n1 = _sv0t11;
  int _sv0t12 = be_push(bet, b1, b2, b3, b4, 3, 13, nx, n1, 0);
  int root = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int ct = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int c1 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int c2 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int c3 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int vn = _sv0t17;
  int _sv0t18 = extract_cexpr(bet, b1, b2, b3, b4, bpp, root, src, starts, ends, tt, ct, c1, c2, c3, vn);
  int _sv0t19 = (0 - 1);
  if ((_sv0t18 != _sv0t19)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_vc_query(void) {
  int _sv0t0 = sv0_vec_new();
  int ct = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int c1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int c2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int c3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int vn = _sv0t4;
  sv0_vec_push(vn, 0);
  int _sv0t5 = cx_var(ct, c1, c2, c3, 0);
  int vx = _sv0t5;
  int _sv0t6 = cx_int(ct, c1, c2, c3, 0);
  int z = _sv0t6;
  int _sv0t7 = cx_binop(ct, c1, c2, c3, 9, vx, z);
  int gt = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int hyps = _sv0t8;
  const char* _sv0t9 = vc_build_query(ct, c1, c2, c3, hyps, gt, vn);
  const char* q;
  q = _sv0t9;
  int _sv0t10 = sv0_string_eq(q, "(set-logic QF_LIA) (declare-const v0 Int) (assert (not (> v0 0))) (check-sat)");
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int ct2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int d1 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int d2 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int d3 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int vn2 = _sv0t15;
  sv0_vec_push(vn2, 0);
  sv0_vec_push(vn2, 1);
  int _sv0t16 = cx_var(ct2, d1, d2, d3, 0);
  int ax = _sv0t16;
  int _sv0t17 = cx_var(ct2, d1, d2, d3, 1);
  int ay = _sv0t17;
  int _sv0t18 = cx_binop(ct2, d1, d2, d3, 2, ax, ay);
  int mul = _sv0t18;
  int _sv0t19 = cx_int(ct2, d1, d2, d3, 0);
  int zz = _sv0t19;
  int _sv0t20 = cx_binop(ct2, d1, d2, d3, 9, mul, zz);
  int g2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int h2 = _sv0t21;
  const char* _sv0t22 = vc_build_query(ct2, d1, d2, d3, h2, g2, vn2);
  const char* q2;
  q2 = _sv0t22;
  int _sv0t23 = sv0_string_eq(q2, "(set-logic QF_NIA) (declare-const v0 Int) (declare-const v1 Int) (assert (not (> (* v0 v1) 0))) (check-sat)");
  if ((_sv0t23 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_vc_gen_ensures(void) {
  int _sv0t0 = sv0_vec_new();
  int ct = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int c1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int c2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int c3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int vn = _sv0t4;
  sv0_vec_push(vn, 0);
  int _sv0t5 = cx_var(ct, c1, c2, c3, 0);
  int vx = _sv0t5;
  int _sv0t6 = cx_int(ct, c1, c2, c3, 0);
  int z = _sv0t6;
  int _sv0t7 = cx_binop(ct, c1, c2, c3, 9, vx, z);
  int req = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int reqs = _sv0t8;
  sv0_vec_push(reqs, req);
  int _sv0t9 = cx_result(ct, c1, c2, c3);
  int r = _sv0t9;
  int _sv0t10 = cx_int(ct, c1, c2, c3, 1);
  int one = _sv0t10;
  int _sv0t11 = cx_binop(ct, c1, c2, c3, 10, r, one);
  int ens = _sv0t11;
  int _sv0t12 = cx_var(ct, c1, c2, c3, 0);
  int retx = _sv0t12;
  const char* _sv0t13 = vc_gen_ensures_query(ct, c1, c2, c3, reqs, ens, retx, vn);
  const char* q;
  q = _sv0t13;
  int _sv0t14 = sv0_string_eq(q, "(set-logic QF_LIA) (declare-const v0 Int) (declare-const result Int) (assert (> v0 0)) (assert (= result v0)) (assert (not (>= result 1))) (check-sat)");
  if ((_sv0t14 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_verify_all_fns(void) {
  const char* src;
  src = "requires(x>0)ensures(result>=1)";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int tt = _sv0t2;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tt, 83);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tt, 6);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tt, 5);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tt, 0);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(tt, 40);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tt, 7);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 20);
  sv0_vec_push(tt, 61);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(tt, 6);
  sv0_vec_push(starts, 21);
  sv0_vec_push(ends, 27);
  sv0_vec_push(tt, 84);
  sv0_vec_push(starts, 27);
  sv0_vec_push(ends, 29);
  sv0_vec_push(tt, 0);
  sv0_vec_push(starts, 29);
  sv0_vec_push(ends, 30);
  sv0_vec_push(tt, 40);
  sv0_vec_push(starts, 30);
  sv0_vec_push(ends, 31);
  sv0_vec_push(tt, 7);
  int _sv0t3 = sv0_vec_new();
  int bet = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int b1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int b2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int b3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int b4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int bpp = _sv0t8;
  sv0_vec_push(bpp, 2);
  int _sv0t9 = be_push(bet, b1, b2, b3, b4, 1, 0, 1, 0, 0);
  int nx = _sv0t9;
  int _sv0t10 = be_push(bet, b1, b2, b3, b4, 0, 0, 4, 0, 0);
  int nz = _sv0t10;
  int _sv0t11 = be_push(bet, b1, b2, b3, b4, 3, 8, nx, nz, 0);
  int req = _sv0t11;
  sv0_vec_push(bpp, 8);
  int _sv0t12 = be_push(bet, b1, b2, b3, b4, 1, 1, 1, 0, 0);
  int nr = _sv0t12;
  int _sv0t13 = be_push(bet, b1, b2, b3, b4, 0, 0, 10, 0, 0);
  int n1 = _sv0t13;
  int _sv0t14 = be_push(bet, b1, b2, b3, b4, 3, 10, nr, n1, 0);
  int ens = _sv0t14;
  sv0_vec_push(bpp, 2);
  int _sv0t15 = be_push(bet, b1, b2, b3, b4, 1, 2, 1, 0, 0);
  int nrx = _sv0t15;
  int _sv0t16 = be_push(bet, b1, b2, b3, b4, 15, nrx, 0, 0, 0);
  int nret = _sv0t16;
  int _sv0t17 = be_push(bet, b1, b2, b3, b4, 28, nret, 0, 0, 0);
  int nstmt = _sv0t17;
  sv0_vec_push(bpp, nstmt);
  int _sv0t18 = (0 - 1);
  int _sv0t19 = be_push(bet, b1, b2, b3, b4, 9, 0, 1, _sv0t18, 4);
  int nblk = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int it = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int id1 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int id2 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int id3 = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int id4 = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int fcb = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int fcr = _sv0t26;
  sv0_vec_push(it, 0);
  sv0_vec_push(id1, 2);
  sv0_vec_push(id2, 4);
  sv0_vec_push(id3, 1);
  sv0_vec_push(id4, nblk);
  sv0_vec_push(fcb, 0);
  sv0_vec_push(fcr, req);
  sv0_vec_push(fcr, ens);
  const char* _sv0t27 = verify_all_fns(it, id1, id2, id3, id4, fcb, fcr, bet, b1, b2, b3, b4, bpp, tt, src, starts, ends);
  const char* got;
  got = _sv0t27;
  const char* _sv0t28 = sv0_string_concat("VC\t1\trequires\trequires(x>0)\tPRECOND\n", "VC\t1\tensures\tensures(result>=1)\t(set-logic QF_LIA) (declare-const v0 Int) (declare-const result Int) (assert (> v0 0)) (assert (= result v0)) (assert (not (>= result 1))) (check-sat)\n");
  const char* want;
  want = _sv0t28;
  int _sv0t29 = sv0_string_eq(got, want);
  if ((_sv0t29 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_cx_tags();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_eval_arith();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_eval_compare();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_eval_bool();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_eval_result_old();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_smt_emit();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_extract_compare();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (60 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_extract_result_arith();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (70 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_extract_old();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (80 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_extract_unsupported();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (90 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_vc_query();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (100 + r11);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_vc_gen_ensures();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (110 + r12);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_verify_all_fns();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (120 + r13);
    return _sv0t24;
  } else {
  }
  return 0;
}

