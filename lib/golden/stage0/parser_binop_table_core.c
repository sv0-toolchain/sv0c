#include "sv0_runtime.h"

static int binop_of(int token_tag);
static int has_binop(int token_tag);
static int assign_binop(int token_tag);
static int is_compound_assign(int token_tag);
static int lit_from_tok(int token_tag);
static int has_literal(int token_tag);
static int test_binop_of(void);
static int test_assign_binop(void);
static int test_lit_from_tok(void);

static int binop_of(int token_tag) {
  if ((token_tag == 17)) {
    return 0;
  } else {
  }
  if ((token_tag == 18)) {
    return 1;
  } else {
  }
  if ((token_tag == 19)) {
    return 2;
  } else {
  }
  if ((token_tag == 20)) {
    return 3;
  } else {
  }
  if ((token_tag == 21)) {
    return 4;
  } else {
  }
  if ((token_tag == 22)) {
    return 5;
  } else {
  }
  if ((token_tag == 23)) {
    return 6;
  } else {
  }
  if ((token_tag == 24)) {
    return 7;
  } else {
  }
  if ((token_tag == 25)) {
    return 8;
  } else {
  }
  if ((token_tag == 26)) {
    return 9;
  } else {
  }
  if ((token_tag == 27)) {
    return 10;
  } else {
  }
  if ((token_tag == 28)) {
    return 11;
  } else {
  }
  if ((token_tag == 29)) {
    return 12;
  } else {
  }
  if ((token_tag == 30)) {
    return 13;
  } else {
  }
  if ((token_tag == 31)) {
    return 14;
  } else {
  }
  if ((token_tag == 32)) {
    return 15;
  } else {
  }
  if ((token_tag == 33)) {
    return 16;
  } else {
  }
  if ((token_tag == 34)) {
    return 17;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int has_binop(int token_tag) {
  int _sv0t0 = binop_of(token_tag);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int assign_binop(int token_tag) {
  if ((token_tag == 35)) {
    return 0;
  } else {
  }
  if ((token_tag == 36)) {
    return 1;
  } else {
  }
  if ((token_tag == 37)) {
    return 2;
  } else {
  }
  if ((token_tag == 38)) {
    return 3;
  } else {
  }
  if ((token_tag == 39)) {
    return 4;
  } else {
  }
  if ((token_tag == 40)) {
    return 5;
  } else {
  }
  if ((token_tag == 41)) {
    return 6;
  } else {
  }
  if ((token_tag == 42)) {
    return 7;
  } else {
  }
  if ((token_tag == 43)) {
    return 8;
  } else {
  }
  if ((token_tag == 44)) {
    return 9;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int is_compound_assign(int token_tag) {
  int _sv0t0 = assign_binop(token_tag);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int lit_from_tok(int token_tag) {
  if ((token_tag == 0)) {
    return 0;
  } else {
  }
  if ((token_tag == 1)) {
    return 1;
  } else {
  }
  if ((token_tag == 2)) {
    return 2;
  } else {
  }
  if ((token_tag == 3)) {
    return 3;
  } else {
  }
  if ((token_tag == 4)) {
    return 4;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int has_literal(int token_tag) {
  int _sv0t0 = lit_from_tok(token_tag);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int test_binop_of(void) {
  int _sv0t0 = binop_of(17);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = binop_of(18);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = binop_of(19);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = binop_of(27);
  if ((_sv0t3 != 10)) {
    return 4;
  } else {
  }
  int _sv0t4 = binop_of(34);
  if ((_sv0t4 != 17)) {
    return 5;
  } else {
  }
  int _sv0t5 = binop_of(0);
  int _sv0t6 = (0 - 1);
  if ((_sv0t5 != _sv0t6)) {
    return 6;
  } else {
  }
  int _sv0t7 = binop_of(99);
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 7;
  } else {
  }
  int _sv0t9 = has_binop(17);
  if ((_sv0t9 != 1)) {
    return 8;
  } else {
  }
  int _sv0t10 = has_binop(0);
  if ((_sv0t10 != 0)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_assign_binop(void) {
  int _sv0t0 = assign_binop(35);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = assign_binop(36);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = assign_binop(44);
  if ((_sv0t2 != 9)) {
    return 3;
  } else {
  }
  int _sv0t3 = assign_binop(17);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 4;
  } else {
  }
  int _sv0t5 = is_compound_assign(35);
  if ((_sv0t5 != 1)) {
    return 5;
  } else {
  }
  int _sv0t6 = is_compound_assign(17);
  if ((_sv0t6 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_lit_from_tok(void) {
  int _sv0t0 = lit_from_tok(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = lit_from_tok(4);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = lit_from_tok(17);
  int _sv0t3 = (0 - 1);
  if ((_sv0t2 != _sv0t3)) {
    return 3;
  } else {
  }
  int _sv0t4 = has_literal(0);
  if ((_sv0t4 != 1)) {
    return 4;
  } else {
  }
  int _sv0t5 = has_literal(17);
  if ((_sv0t5 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_binop_of();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_assign_binop();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_lit_from_tok();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  return 0;
}

