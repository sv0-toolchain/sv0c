#include "sv0_runtime.h"

static int expr_match_total_len(int bind_scrut_len, int match_arms_len);

static int expr_match_total_len(int bind_scrut_len, int match_arms_len) {
  if ((bind_scrut_len < 0)) {
    return 255;
  } else {
  }
  if ((match_arms_len < 0)) {
    return 255;
  } else {
  }
  int _sv0t0 = (bind_scrut_len + 1);
  int _sv0t1 = (_sv0t0 + match_arms_len);
  return _sv0t1;
}

int main(void) {
  int _sv0t0 = expr_match_total_len(0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = expr_match_total_len(1, 5);
  if ((_sv0t1 != 7)) {
    return 1;
  } else {
  }
  int _sv0t2 = expr_match_total_len(3, 10);
  if ((_sv0t2 != 14)) {
    return 1;
  } else {
  }
  int _sv0t3 = expr_match_total_len(0, 1);
  if ((_sv0t3 != 2)) {
    return 1;
  } else {
  }
  int _sv0t4 = (-1);
  int _sv0t5 = expr_match_total_len(_sv0t4, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = (-1);
  int _sv0t7 = expr_match_total_len(0, _sv0t6);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

