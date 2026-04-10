#include "sv0_runtime.h"

static int lower_match_one_arm_ir_len(int cpre, int bind_pre, int body_iv, int guard_wi, int has_guard, int rest);

static int lower_match_one_arm_ir_len(int cpre, int bind_pre, int body_iv, int guard_wi, int has_guard, int rest) {
  if ((cpre < 0)) {
    return 255;
  } else {
  }
  if ((bind_pre < 0)) {
    return 255;
  } else {
  }
  if ((body_iv < 0)) {
    return 255;
  } else {
  }
  if ((guard_wi < 0)) {
    return 255;
  } else {
  }
  if ((rest < 0)) {
    return 255;
  } else {
  }
  int body_is = (bind_pre + body_iv);
  if ((has_guard == 0)) {
    int _sv0t0 = (cpre + 1);
    int _sv0t1 = (_sv0t0 + body_is);
    int _sv0t2 = (_sv0t1 + rest);
    return _sv0t2;
  } else {
  }
  if ((has_guard == 1)) {
    int _sv0t3 = (cpre + 1);
    int _sv0t4 = (_sv0t3 + guard_wi);
    int _sv0t5 = (_sv0t4 + 1);
    int _sv0t6 = (_sv0t5 + body_is);
    int _sv0t7 = (_sv0t6 + rest);
    return _sv0t7;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = lower_match_one_arm_ir_len(0, 1, 2, 0, 0, 0);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_match_one_arm_ir_len(2, 0, 3, 5, 1, 0);
  if ((_sv0t1 != 12)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_match_one_arm_ir_len(1, 1, 1, 0, 0, 7);
  if ((_sv0t2 != 11)) {
    return 1;
  } else {
  }
  int _sv0t3 = lower_match_one_arm_ir_len(0, 0, 1, 4, 1, 2);
  if ((_sv0t3 != 9)) {
    return 1;
  } else {
  }
  int _sv0t4 = (-1);
  int _sv0t5 = lower_match_one_arm_ir_len(_sv0t4, 0, 0, 0, 0, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = lower_match_one_arm_ir_len(0, 0, 0, 0, 2, 0);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

