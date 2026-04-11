#include "sv0_runtime.h"

static int primary_expr_tag(int token_tag);
static int is_literal_token(int token_tag);
static int is_block_starter(int token_tag);
static int is_control_flow(int token_tag);
static int struct_or_path(int allow_struct, int next_is_lbrace);

static int primary_expr_tag(int token_tag) {
  if ((token_tag == 1)) {
    return 0;
  } else {
  }
  if ((token_tag == 2)) {
    return 0;
  } else {
  }
  if ((token_tag == 3)) {
    return 1;
  } else {
  }
  if ((token_tag == 4)) {
    return 1;
  } else {
  }
  if ((token_tag == 5)) {
    return 1;
  } else {
  }
  if ((token_tag == 6)) {
    return 2;
  } else {
  }
  if ((token_tag == 40)) {
    return 3;
  } else {
  }
  if ((token_tag == 91)) {
    return 4;
  } else {
  }
  if ((token_tag == 123)) {
    return 5;
  } else {
  }
  if ((token_tag == 10)) {
    return 6;
  } else {
  }
  if ((token_tag == 11)) {
    return 7;
  } else {
  }
  if ((token_tag == 12)) {
    return 8;
  } else {
  }
  if ((token_tag == 13)) {
    return 9;
  } else {
  }
  if ((token_tag == 14)) {
    return 10;
  } else {
  }
  if ((token_tag == 15)) {
    return 11;
  } else {
  }
  if ((token_tag == 16)) {
    return 12;
  } else {
  }
  if ((token_tag == 17)) {
    return 13;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int is_literal_token(int token_tag) {
  int _sv0t0 = (token_tag >= 1);
  int _sv0t1 = (token_tag <= 6);
  if ((_sv0t0 && _sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_block_starter(int token_tag) {
  int _sv0t0 = (token_tag == 123);
  return _sv0t0;
}

static int is_control_flow(int token_tag) {
  if ((token_tag == 10)) {
    return 1;
  } else {
  }
  if ((token_tag == 11)) {
    return 1;
  } else {
  }
  if ((token_tag == 12)) {
    return 1;
  } else {
  }
  if ((token_tag == 13)) {
    return 1;
  } else {
  }
  if ((token_tag == 14)) {
    return 1;
  } else {
  }
  if ((token_tag == 15)) {
    return 1;
  } else {
  }
  if ((token_tag == 16)) {
    return 1;
  } else {
  }
  return 0;
}

static int struct_or_path(int allow_struct, int next_is_lbrace) {
  if ((allow_struct && next_is_lbrace)) {
    return 0;
  } else {
  }
  return 1;
}

int main(void) {
  int _sv0t0 = primary_expr_tag(1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = primary_expr_tag(123);
  if ((_sv0t1 != 5)) {
    return 2;
  } else {
  }
  int _sv0t2 = primary_expr_tag(10);
  if ((_sv0t2 != 6)) {
    return 3;
  } else {
  }
  int _sv0t3 = primary_expr_tag(99);
  int _sv0t4 = (-1);
  if ((_sv0t3 != _sv0t4)) {
    return 4;
  } else {
  }
  int _sv0t5 = is_literal_token(1);
  if ((!_sv0t5)) {
    return 5;
  } else {
  }
  int _sv0t6 = is_literal_token(6);
  if ((!_sv0t6)) {
    return 6;
  } else {
  }
  int _sv0t7 = is_literal_token(7);
  if (_sv0t7) {
    return 7;
  } else {
  }
  int _sv0t8 = is_block_starter(123);
  if ((!_sv0t8)) {
    return 8;
  } else {
  }
  int _sv0t9 = is_block_starter(40);
  if (_sv0t9) {
    return 9;
  } else {
  }
  int _sv0t10 = is_control_flow(10);
  if ((!_sv0t10)) {
    return 10;
  } else {
  }
  int _sv0t11 = is_control_flow(16);
  if ((!_sv0t11)) {
    return 11;
  } else {
  }
  int _sv0t12 = is_control_flow(1);
  if (_sv0t12) {
    return 12;
  } else {
  }
  int _sv0t13 = struct_or_path(1, 1);
  if ((_sv0t13 != 0)) {
    return 13;
  } else {
  }
  int _sv0t14 = struct_or_path(0, 1);
  if ((_sv0t14 != 1)) {
    return 14;
  } else {
  }
  int _sv0t15 = struct_or_path(1, 0);
  if ((_sv0t15 != 1)) {
    return 15;
  } else {
  }
  return 0;
}

