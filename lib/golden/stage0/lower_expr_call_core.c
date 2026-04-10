#include "sv0_runtime.h"

static int expr_call_total_len(int call_kind, int args_pre_total, int eci_len);

static int expr_call_total_len(int call_kind, int args_pre_total, int eci_len) {
  if ((call_kind == 0)) {
    return 1;
  } else {
  }
  if ((call_kind == 1)) {
    return 0;
  } else {
  }
  if ((call_kind == 2)) {
    if ((args_pre_total < 0)) {
      return 255;
    } else {
    }
    int _sv0t0 = (args_pre_total + 1);
    return _sv0t0;
  } else {
  }
  if ((call_kind == 3)) {
    if ((args_pre_total < 0)) {
      return 255;
    } else {
    }
    int _sv0t1 = (args_pre_total + 1);
    return _sv0t1;
  } else {
  }
  if ((call_kind == 4)) {
    if ((eci_len < 0)) {
      return 255;
    } else {
    }
    int _sv0t2 = (1 + eci_len);
    return _sv0t2;
  } else {
  }
  if ((call_kind == 5)) {
    if ((eci_len < 0)) {
      return 255;
    } else {
    }
    int _sv0t3 = (1 + eci_len);
    return _sv0t3;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = expr_call_total_len(0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = expr_call_total_len(1, 0, 0);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = expr_call_total_len(2, 4, 0);
  if ((_sv0t2 != 5)) {
    return 1;
  } else {
  }
  int _sv0t3 = expr_call_total_len(3, 0, 0);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = expr_call_total_len(3, 7, 0);
  if ((_sv0t4 != 8)) {
    return 1;
  } else {
  }
  int _sv0t5 = expr_call_total_len(4, 0, 5);
  if ((_sv0t5 != 6)) {
    return 1;
  } else {
  }
  int _sv0t6 = expr_call_total_len(5, 0, 3);
  if ((_sv0t6 != 4)) {
    return 1;
  } else {
  }
  int _sv0t7 = (-1);
  int _sv0t8 = expr_call_total_len(3, _sv0t7, 0);
  if ((_sv0t8 != 255)) {
    return 1;
  } else {
  }
  int _sv0t9 = expr_call_total_len(9, 0, 0);
  if ((_sv0t9 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

