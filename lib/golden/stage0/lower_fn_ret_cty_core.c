#include "sv0_runtime.h"

static int fn_ret_cty_class(int has_ret, int ret_ty_class);

static int fn_ret_cty_class(int has_ret, int ret_ty_class) {
  if ((has_ret == 0)) {
    return 4;
  } else {
  }
  if ((has_ret == 1)) {
    if ((ret_ty_class < 1)) {
      return 255;
    } else {
    }
    if ((ret_ty_class > 4)) {
      return 255;
    } else {
    }
    return ret_ty_class;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = fn_ret_cty_class(0, 0);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = fn_ret_cty_class(1, 1);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = fn_ret_cty_class(1, 2);
  if ((_sv0t2 != 2)) {
    return 1;
  } else {
  }
  int _sv0t3 = fn_ret_cty_class(1, 3);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = fn_ret_cty_class(1, 4);
  if ((_sv0t4 != 4)) {
    return 1;
  } else {
  }
  int _sv0t5 = fn_ret_cty_class(2, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = fn_ret_cty_class(1, 0);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  int _sv0t7 = fn_ret_cty_class(1, 5);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

