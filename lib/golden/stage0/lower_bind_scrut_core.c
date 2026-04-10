#include "sv0_runtime.h"

static int bind_scrut_var_ir_len(int tv_len, int wi_len, int val_kind, int int_cty);

static int bind_scrut_var_ir_len(int tv_len, int wi_len, int val_kind, int int_cty) {
  if ((tv_len < 0)) {
    return 255;
  } else {
  }
  if ((wi_len < 0)) {
    return 255;
  } else {
  }
  if ((val_kind == 0)) {
    return tv_len;
  } else {
  }
  if ((val_kind == 1)) {
    int _sv0t0 = (tv_len + 1);
    return _sv0t0;
  } else {
  }
  if ((val_kind == 2)) {
    if ((int_cty == 1)) {
      int _sv0t1 = (tv_len + wi_len);
      int _sv0t2 = (_sv0t1 + 1);
      return _sv0t2;
    } else {
    }
    int _sv0t3 = (tv_len + wi_len);
    int _sv0t4 = (_sv0t3 + 2);
    return _sv0t4;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = bind_scrut_var_ir_len(3, 9, 0, 0);
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = bind_scrut_var_ir_len(2, 0, 1, 0);
  if ((_sv0t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t2 = bind_scrut_var_ir_len(1, 4, 2, 1);
  if ((_sv0t2 != 6)) {
    return 1;
  } else {
  }
  int _sv0t3 = bind_scrut_var_ir_len(1, 4, 2, 0);
  if ((_sv0t3 != 7)) {
    return 1;
  } else {
  }
  int _sv0t4 = (-1);
  int _sv0t5 = bind_scrut_var_ir_len(_sv0t4, 0, 0, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = (-1);
  int _sv0t7 = bind_scrut_var_ir_len(0, _sv0t6, 2, 1);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  int _sv0t8 = bind_scrut_var_ir_len(0, 0, 3, 0);
  if ((_sv0t8 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

