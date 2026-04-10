#include "sv0_runtime.h"

static int lower_quant_ir_len(int hi_w, int lo_w, int body_w);

static int lower_quant_ir_len(int hi_w, int lo_w, int body_w) {
  if ((hi_w < 0)) {
    return 255;
  } else {
  }
  if ((lo_w < 0)) {
    return 255;
  } else {
  }
  if ((body_w < 0)) {
    return 255;
  } else {
  }
  int _sv0t0 = (hi_w + lo_w);
  int _sv0t1 = (_sv0t0 + body_w);
  int _sv0t2 = (_sv0t1 + 11);
  return _sv0t2;
}

int main(void) {
  int _sv0t0 = lower_quant_ir_len(0, 0, 0);
  if ((_sv0t0 != 11)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_quant_ir_len(2, 3, 5);
  if ((_sv0t1 != 21)) {
    return 1;
  } else {
  }
  int _sv0t2 = (-1);
  int _sv0t3 = lower_quant_ir_len(_sv0t2, 0, 0);
  if ((_sv0t3 != 255)) {
    return 1;
  } else {
  }
  int _sv0t4 = (-1);
  int _sv0t5 = lower_quant_ir_len(0, _sv0t4, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = (-1);
  int _sv0t7 = lower_quant_ir_len(0, 0, _sv0t6);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

