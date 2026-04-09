#include "sv0_runtime.h"

static int lower_fn_prefix_len(int use_ret_slot, int old_instrs_total, int req_instrs_total);

static int lower_fn_prefix_len(int use_ret_slot, int old_instrs_total, int req_instrs_total) {
  if ((use_ret_slot != 0)) {
    if ((use_ret_slot != 1)) {
      return 255;
    } else {
    }
  } else {
  }
  if ((old_instrs_total < 0)) {
    return 255;
  } else {
  }
  if ((req_instrs_total < 0)) {
    return 255;
  } else {
  }
  if ((use_ret_slot == 1)) {
    int _sv0t0 = (1 + old_instrs_total);
    int _sv0t1 = (_sv0t0 + req_instrs_total);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = (old_instrs_total + req_instrs_total);
  return _sv0t2;
}

int main(void) {
  int _sv0t0 = lower_fn_prefix_len(0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_fn_prefix_len(1, 0, 0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_fn_prefix_len(0, 4, 10);
  if ((_sv0t2 != 14)) {
    return 1;
  } else {
  }
  int _sv0t3 = lower_fn_prefix_len(1, 4, 10);
  if ((_sv0t3 != 15)) {
    return 1;
  } else {
  }
  int _sv0t4 = lower_fn_prefix_len(2, 0, 0);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  int _sv0t5 = (-1);
  int _sv0t6 = lower_fn_prefix_len(1, _sv0t5, 0);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  int _sv0t7 = (-3);
  int _sv0t8 = lower_fn_prefix_len(1, 0, _sv0t7);
  if ((_sv0t8 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

