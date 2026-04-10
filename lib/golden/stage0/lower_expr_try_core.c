#include "sv0_runtime.h"

static int expr_try_total_len(int wi_pre_len, int pCty_is_int, int scr_is_int_or_void);

static int expr_try_total_len(int wi_pre_len, int pCty_is_int, int scr_is_int_or_void) {
  if ((scr_is_int_or_void == 1)) {
    return 254;
  } else {
  }
  if ((wi_pre_len < 0)) {
    return 255;
  } else {
  }
  if ((pCty_is_int < 0)) {
    return 255;
  } else {
  }
  if ((pCty_is_int > 1)) {
    return 255;
  } else {
  }
  int _sv0t0 = (wi_pre_len + 4);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = expr_try_total_len(0, 1, 0);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = expr_try_total_len(3, 0, 0);
  if ((_sv0t1 != 7)) {
    return 1;
  } else {
  }
  int _sv0t2 = expr_try_total_len(0, 0, 0);
  if ((_sv0t2 != 4)) {
    return 1;
  } else {
  }
  int _sv0t3 = expr_try_total_len(5, 1, 0);
  if ((_sv0t3 != 9)) {
    return 1;
  } else {
  }
  int _sv0t4 = expr_try_total_len(0, 0, 1);
  if ((_sv0t4 != 254)) {
    return 1;
  } else {
  }
  int _sv0t5 = (-1);
  int _sv0t6 = expr_try_total_len(_sv0t5, 0, 0);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  int _sv0t7 = expr_try_total_len(0, 2, 0);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

