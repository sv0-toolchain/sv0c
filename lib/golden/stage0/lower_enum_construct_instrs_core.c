#include "sv0_runtime.h"

static int enum_construct_instrs_total(int args_iv_total, int arg_count);

static int enum_construct_instrs_total(int args_iv_total, int arg_count) {
  if ((args_iv_total < 0)) {
    return 255;
  } else {
  }
  if ((arg_count < 0)) {
    return 255;
  } else {
  }
  int _sv0t0 = (args_iv_total + 1);
  int _sv0t1 = (_sv0t0 + arg_count);
  return _sv0t1;
}

int main(void) {
  int _sv0t0 = enum_construct_instrs_total(0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = enum_construct_instrs_total(5, 2);
  if ((_sv0t1 != 8)) {
    return 1;
  } else {
  }
  int _sv0t2 = enum_construct_instrs_total(0, 5);
  if ((_sv0t2 != 6)) {
    return 1;
  } else {
  }
  int _sv0t3 = (-1);
  int _sv0t4 = enum_construct_instrs_total(_sv0t3, 0);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  int _sv0t5 = (-3);
  int _sv0t6 = enum_construct_instrs_total(0, _sv0t5);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

