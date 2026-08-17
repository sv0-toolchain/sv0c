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
static int test_cx_tags(void);
static int test_eval_arith(void);
static int test_eval_compare(void);
static int test_eval_bool(void);
static int test_eval_result_old(void);
static int test_smt_emit(void);

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
  return 0;
}

