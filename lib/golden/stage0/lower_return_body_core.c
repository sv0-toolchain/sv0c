#include "sv0_runtime.h"

static int lower_return_ir_len(int is_some, int value_lower_len);
static int lower_body_block_tail_len(int stmt_prefix_len, int tail_kind, int tail_expr_len);

static int lower_return_ir_len(int is_some, int value_lower_len) {
  if ((is_some == 0)) {
    return 1;
  } else {
  }
  if ((is_some == 1)) {
    if ((value_lower_len < 0)) {
      return 255;
    } else {
    }
    int _sv0t0 = (value_lower_len + 1);
    return _sv0t0;
  } else {
  }
  return 255;
}

static int lower_body_block_tail_len(int stmt_prefix_len, int tail_kind, int tail_expr_len) {
  if ((stmt_prefix_len < 0)) {
    return 255;
  } else {
  }
  if ((tail_kind == 0)) {
    return stmt_prefix_len;
  } else {
  }
  if ((tail_kind == 1)) {
    int _sv0t0 = (stmt_prefix_len + 1);
    return _sv0t0;
  } else {
  }
  if ((tail_kind == 2)) {
    if ((tail_expr_len < 0)) {
      return 255;
    } else {
    }
    int _sv0t1 = (stmt_prefix_len + tail_expr_len);
    int _sv0t2 = (_sv0t1 + 1);
    return _sv0t2;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = lower_return_ir_len(0, 99);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_return_ir_len(1, 0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_return_ir_len(1, 5);
  if ((_sv0t2 != 6)) {
    return 1;
  } else {
  }
  int _sv0t3 = (-1);
  int _sv0t4 = lower_return_ir_len(1, _sv0t3);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  int _sv0t5 = lower_return_ir_len(2, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = lower_body_block_tail_len(3, 0, 0);
  if ((_sv0t6 != 3)) {
    return 1;
  } else {
  }
  int _sv0t7 = lower_body_block_tail_len(3, 1, 0);
  if ((_sv0t7 != 4)) {
    return 1;
  } else {
  }
  int _sv0t8 = lower_body_block_tail_len(3, 2, 2);
  if ((_sv0t8 != 6)) {
    return 1;
  } else {
  }
  int _sv0t9 = (-1);
  int _sv0t10 = lower_body_block_tail_len(3, 2, _sv0t9);
  if ((_sv0t10 != 255)) {
    return 1;
  } else {
  }
  int _sv0t11 = (-1);
  int _sv0t12 = lower_body_block_tail_len(_sv0t11, 0, 0);
  if ((_sv0t12 != 255)) {
    return 1;
  } else {
  }
  int _sv0t13 = lower_body_block_tail_len(0, 3, 0);
  if ((_sv0t13 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

