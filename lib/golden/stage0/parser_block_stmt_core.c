#include "sv0_runtime.h"

static int stmt_kind_from_token(int token_tag);
static int is_let_stmt(int token_tag);
static int let_has_mut(int next_tag);
static int let_has_type_annotation(int colon_tag);
static int let_has_initializer(int eq_tag);
static int block_trailing_expr(int last_has_semi);
static int is_block_end(int token_tag);
static int stmt_needs_semicolon(int kind);
static int count_stmts_in_block(int s0, int s1, int s2, int s3);

static int stmt_kind_from_token(int token_tag) {
  if ((token_tag == 20)) {
    return 0;
  } else {
  }
  if ((token_tag == 10)) {
    return 1;
  } else {
  }
  if ((token_tag == 12)) {
    return 2;
  } else {
  }
  if ((token_tag == 13)) {
    return 3;
  } else {
  }
  if ((token_tag == 14)) {
    return 4;
  } else {
  }
  if ((token_tag == 16)) {
    return 5;
  } else {
  }
  if ((token_tag == 59)) {
    return 6;
  } else {
  }
  return 7;
}

static int is_let_stmt(int token_tag) {
  int _sv0t0 = (token_tag == 20);
  return _sv0t0;
}

static int let_has_mut(int next_tag) {
  int _sv0t0 = (next_tag == 21);
  return _sv0t0;
}

static int let_has_type_annotation(int colon_tag) {
  int _sv0t0 = (colon_tag == 58);
  return _sv0t0;
}

static int let_has_initializer(int eq_tag) {
  int _sv0t0 = (eq_tag == 61);
  return _sv0t0;
}

static int block_trailing_expr(int last_has_semi) {
  int _sv0t0 = (!last_has_semi);
  return _sv0t0;
}

static int is_block_end(int token_tag) {
  int _sv0t0 = (token_tag == 125);
  return _sv0t0;
}

static int stmt_needs_semicolon(int kind) {
  if ((kind == 0)) {
    return 1;
  } else {
  }
  if ((kind == 6)) {
    return 0;
  } else {
  }
  if ((kind == 7)) {
    return 1;
  } else {
  }
  return 0;
}

static int count_stmts_in_block(int s0, int s1, int s2, int s3) {
  int count = 0;
  if (s0) {
    count = (count + 1);
  } else {
  }
  if (s1) {
    count = (count + 1);
  } else {
  }
  if (s2) {
    count = (count + 1);
  } else {
  }
  if (s3) {
    count = (count + 1);
  } else {
  }
  return count;
}

int main(void) {
  int _sv0t0 = stmt_kind_from_token(20);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = stmt_kind_from_token(10);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = stmt_kind_from_token(59);
  if ((_sv0t2 != 6)) {
    return 3;
  } else {
  }
  int _sv0t3 = stmt_kind_from_token(99);
  if ((_sv0t3 != 7)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_let_stmt(20);
  if ((!_sv0t4)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_let_stmt(10);
  if (_sv0t5) {
    return 6;
  } else {
  }
  int _sv0t6 = let_has_mut(21);
  if ((!_sv0t6)) {
    return 7;
  } else {
  }
  int _sv0t7 = let_has_mut(20);
  if (_sv0t7) {
    return 8;
  } else {
  }
  int _sv0t8 = let_has_type_annotation(58);
  if ((!_sv0t8)) {
    return 9;
  } else {
  }
  int _sv0t9 = let_has_type_annotation(61);
  if (_sv0t9) {
    return 10;
  } else {
  }
  int _sv0t10 = let_has_initializer(61);
  if ((!_sv0t10)) {
    return 11;
  } else {
  }
  int _sv0t11 = let_has_initializer(59);
  if (_sv0t11) {
    return 12;
  } else {
  }
  int _sv0t12 = block_trailing_expr(0);
  if ((!_sv0t12)) {
    return 13;
  } else {
  }
  int _sv0t13 = block_trailing_expr(1);
  if (_sv0t13) {
    return 14;
  } else {
  }
  int _sv0t14 = is_block_end(125);
  if ((!_sv0t14)) {
    return 15;
  } else {
  }
  int _sv0t15 = is_block_end(123);
  if (_sv0t15) {
    return 16;
  } else {
  }
  int _sv0t16 = stmt_needs_semicolon(0);
  if ((!_sv0t16)) {
    return 17;
  } else {
  }
  int _sv0t17 = stmt_needs_semicolon(6);
  if (_sv0t17) {
    return 18;
  } else {
  }
  int _sv0t18 = stmt_needs_semicolon(7);
  if ((!_sv0t18)) {
    return 19;
  } else {
  }
  int _sv0t19 = count_stmts_in_block(1, 1, 0, 0);
  if ((_sv0t19 != 2)) {
    return 20;
  } else {
  }
  int _sv0t20 = count_stmts_in_block(1, 1, 1, 1);
  if ((_sv0t20 != 4)) {
    return 21;
  } else {
  }
  return 0;
}

