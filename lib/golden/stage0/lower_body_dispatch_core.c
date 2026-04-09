#include "sv0_runtime.h"

static int return_ir_len(int is_some, int value_lower_len);
static int lower_body_total_len(int is_block, int stmt_instr_len, int has_opt_tail, int tail_is_some, int tail_value_lower_len, int direct_is_some, int direct_value_lower_len);

static int return_ir_len(int is_some, int value_lower_len) {
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

static int lower_body_total_len(int is_block, int stmt_instr_len, int has_opt_tail, int tail_is_some, int tail_value_lower_len, int direct_is_some, int direct_value_lower_len) {
  if ((is_block == 0)) {
    int _sv0t0 = return_ir_len(direct_is_some, direct_value_lower_len);
    return _sv0t0;
  } else {
  }
  if ((is_block != 1)) {
    return 255;
  } else {
  }
  if ((stmt_instr_len < 0)) {
    return 255;
  } else {
  }
  if ((has_opt_tail == 0)) {
    return stmt_instr_len;
  } else {
  }
  if ((has_opt_tail != 1)) {
    return 255;
  } else {
  }
  if ((tail_is_some == 0)) {
    int _sv0t1 = (stmt_instr_len + 1);
    return _sv0t1;
  } else {
  }
  if ((tail_is_some == 1)) {
    if ((tail_value_lower_len < 0)) {
      return 255;
    } else {
    }
    int _sv0t2 = (stmt_instr_len + tail_value_lower_len);
    int _sv0t3 = (_sv0t2 + 1);
    return _sv0t3;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = lower_body_total_len(0, 0, 0, 0, 0, 0, 99);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_body_total_len(0, 0, 0, 0, 0, 1, 5);
  if ((_sv0t1 != 6)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_body_total_len(1, 3, 0, 0, 0, 0, 0);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = lower_body_total_len(1, 2, 1, 0, 0, 0, 0);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = lower_body_total_len(1, 2, 1, 1, 5, 0, 0);
  if ((_sv0t4 != 8)) {
    return 1;
  } else {
  }
  int _sv0t5 = lower_body_total_len(2, 0, 0, 0, 0, 0, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = (-1);
  int _sv0t7 = lower_body_total_len(1, _sv0t6, 0, 0, 0, 0, 0);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  int _sv0t8 = lower_body_total_len(1, 1, 2, 0, 0, 0, 0);
  if ((_sv0t8 != 255)) {
    return 1;
  } else {
  }
  int _sv0t9 = (-1);
  int _sv0t10 = lower_body_total_len(1, 1, 1, 1, _sv0t9, 0, 0);
  if ((_sv0t10 != 255)) {
    return 1;
  } else {
  }
  int _sv0t11 = lower_body_total_len(0, 0, 0, 0, 0, 2, 0);
  if ((_sv0t11 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

