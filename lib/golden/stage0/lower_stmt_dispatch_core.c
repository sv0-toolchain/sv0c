#include "sv0_runtime.h"

static int lower_stmt_total_len(int stmt_kind, int ret_value_lower_len, int effect_instr_len);

static int lower_stmt_total_len(int stmt_kind, int ret_value_lower_len, int effect_instr_len) {
  if ((stmt_kind == 0)) {
    return 1;
  } else {
  }
  if ((stmt_kind == 1)) {
    if ((ret_value_lower_len < 0)) {
      return 255;
    } else {
    }
    int _sv0t0 = (ret_value_lower_len + 1);
    return _sv0t0;
  } else {
  }
  if ((stmt_kind == 2)) {
    if ((effect_instr_len < 0)) {
      return 255;
    } else {
    }
    return effect_instr_len;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = lower_stmt_total_len(0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_stmt_total_len(1, 5, 0);
  if ((_sv0t1 != 6)) {
    return 1;
  } else {
  }
  int _sv0t2 = (-1);
  int _sv0t3 = lower_stmt_total_len(1, _sv0t2, 0);
  if ((_sv0t3 != 255)) {
    return 1;
  } else {
  }
  int _sv0t4 = lower_stmt_total_len(2, 0, 7);
  if ((_sv0t4 != 7)) {
    return 1;
  } else {
  }
  int _sv0t5 = (-1);
  int _sv0t6 = lower_stmt_total_len(2, 0, _sv0t5);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  int _sv0t7 = lower_stmt_total_len(3, 0, 0);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

