#include "sv0_runtime.h"

static int unary_op_from_token(int token_tag, int next_is_mut);
static int is_unary_prefix(int token_tag);
static int unary_tokens_consumed(int op);
static int unary_is_borrow(int op);
static int unary_ast_tag(void);
static int unary_precedence(void);

static int unary_op_from_token(int token_tag, int next_is_mut) {
  if ((token_tag == 70)) {
    return 1;
  } else {
  }
  if ((token_tag == 71)) {
    return 2;
  } else {
  }
  if ((token_tag == 72)) {
    return 3;
  } else {
  }
  if ((token_tag == 73)) {
    return 4;
  } else {
  }
  if ((token_tag == 74)) {
    if (next_is_mut) {
      return 6;
    } else {
    }
    return 5;
  } else {
  }
  return 0;
}

static int is_unary_prefix(int token_tag) {
  if ((token_tag == 70)) {
    return 1;
  } else {
  }
  if ((token_tag == 71)) {
    return 1;
  } else {
  }
  if ((token_tag == 72)) {
    return 1;
  } else {
  }
  if ((token_tag == 73)) {
    return 1;
  } else {
  }
  if ((token_tag == 74)) {
    return 1;
  } else {
  }
  return 0;
}

static int unary_tokens_consumed(int op) {
  if ((op == 6)) {
    return 2;
  } else {
  }
  if ((op == 0)) {
    return 0;
  } else {
  }
  return 1;
}

static int unary_is_borrow(int op) {
  int _sv0t0 = (op == 5);
  int _sv0t1 = (op == 6);
  int _sv0t2 = (_sv0t0 || _sv0t1);
  return _sv0t2;
}

static int unary_ast_tag(void) {
  return 5;
}

static int unary_precedence(void) {
  return 14;
}

int main(void) {
  int _sv0t0 = unary_op_from_token(70, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = unary_op_from_token(71, 0);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = unary_op_from_token(72, 0);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = unary_op_from_token(73, 0);
  if ((_sv0t3 != 4)) {
    return 4;
  } else {
  }
  int _sv0t4 = unary_op_from_token(74, 0);
  if ((_sv0t4 != 5)) {
    return 5;
  } else {
  }
  int _sv0t5 = unary_op_from_token(74, 1);
  if ((_sv0t5 != 6)) {
    return 6;
  } else {
  }
  int _sv0t6 = unary_op_from_token(99, 0);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_unary_prefix(70);
  if ((!_sv0t7)) {
    return 10;
  } else {
  }
  int _sv0t8 = is_unary_prefix(74);
  if ((!_sv0t8)) {
    return 11;
  } else {
  }
  int _sv0t9 = is_unary_prefix(50);
  if (_sv0t9) {
    return 12;
  } else {
  }
  int _sv0t10 = unary_tokens_consumed(1);
  if ((_sv0t10 != 1)) {
    return 20;
  } else {
  }
  int _sv0t11 = unary_tokens_consumed(6);
  if ((_sv0t11 != 2)) {
    return 21;
  } else {
  }
  int _sv0t12 = unary_tokens_consumed(0);
  if ((_sv0t12 != 0)) {
    return 22;
  } else {
  }
  int _sv0t13 = unary_is_borrow(5);
  if ((!_sv0t13)) {
    return 30;
  } else {
  }
  int _sv0t14 = unary_is_borrow(6);
  if ((!_sv0t14)) {
    return 31;
  } else {
  }
  int _sv0t15 = unary_is_borrow(1);
  if (_sv0t15) {
    return 32;
  } else {
  }
  int _sv0t16 = unary_ast_tag();
  if ((_sv0t16 != 5)) {
    return 40;
  } else {
  }
  int _sv0t17 = unary_precedence();
  if ((_sv0t17 != 14)) {
    return 50;
  } else {
  }
  return 0;
}

