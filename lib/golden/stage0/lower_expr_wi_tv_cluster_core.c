#include "sv0_runtime.h"

static int lower_expr_with_instrs_len(int wi_tag, int a, int b);
static int lower_expr_to_value_len(int tv_tag, int a, int b, int cty_is_int);

static int lower_expr_with_instrs_len(int wi_tag, int a, int b) {
  if ((wi_tag == 0)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    int _sv0t0 = (a + b);
    return _sv0t0;
  } else {
  }
  if ((wi_tag == 1)) {
    return 0;
  } else {
  }
  if ((wi_tag == 2)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    return a;
  } else {
  }
  if ((wi_tag == 3)) {
    return 0;
  } else {
  }
  if ((wi_tag == 4)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    int _sv0t1 = (a + b);
    return _sv0t1;
  } else {
  }
  if ((wi_tag == 5)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    return a;
  } else {
  }
  if ((wi_tag == 6)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    return a;
  } else {
  }
  if ((wi_tag == 7)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    return a;
  } else {
  }
  return 255;
}

static int lower_expr_to_value_len(int tv_tag, int a, int b, int cty_is_int) {
  if ((tv_tag == 0)) {
    return 0;
  } else {
  }
  if ((tv_tag == 1)) {
    return 0;
  } else {
  }
  if ((tv_tag == 2)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    return a;
  } else {
  }
  if ((tv_tag == 3)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    int _sv0t0 = (a + b);
    return _sv0t0;
  } else {
  }
  if ((tv_tag == 4)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    int _sv0t1 = (1 + a);
    return _sv0t1;
  } else {
  }
  if ((tv_tag == 5)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    int _sv0t2 = (1 + a);
    return _sv0t2;
  } else {
  }
  if ((tv_tag == 6)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    return a;
  } else {
  }
  if ((tv_tag == 7)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    int _sv0t3 = (a + b);
    int _sv0t4 = (_sv0t3 + 1);
    return _sv0t4;
  } else {
  }
  if ((tv_tag == 8)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    int _sv0t5 = (a + 1);
    return _sv0t5;
  } else {
  }
  if ((tv_tag == 9)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((cty_is_int == 1)) {
      int _sv0t6 = (a + 1);
      return _sv0t6;
    } else {
    }
    if ((cty_is_int == 0)) {
      int _sv0t7 = (a + 2);
      return _sv0t7;
    } else {
    }
    return 255;
  } else {
  }
  if ((tv_tag == 10)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    int _sv0t8 = (a + 1);
    return _sv0t8;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = lower_expr_with_instrs_len(0, 3, 4);
  if ((_sv0t0 != 7)) {
    return 1;
  } else {
  }
  int _sv0t1 = (-1);
  int _sv0t2 = lower_expr_with_instrs_len(0, _sv0t1, 0);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = (-2);
  int _sv0t4 = lower_expr_with_instrs_len(0, 0, _sv0t3);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  int _sv0t5 = lower_expr_with_instrs_len(1, 0, 0);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = lower_expr_with_instrs_len(2, 9, 0);
  if ((_sv0t6 != 9)) {
    return 1;
  } else {
  }
  int _sv0t7 = (-1);
  int _sv0t8 = lower_expr_with_instrs_len(2, _sv0t7, 0);
  if ((_sv0t8 != 255)) {
    return 1;
  } else {
  }
  int _sv0t9 = lower_expr_with_instrs_len(3, 0, 0);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = lower_expr_with_instrs_len(4, 2, 5);
  if ((_sv0t10 != 7)) {
    return 1;
  } else {
  }
  int _sv0t11 = lower_expr_with_instrs_len(5, 4, 0);
  if ((_sv0t11 != 4)) {
    return 1;
  } else {
  }
  int _sv0t12 = lower_expr_with_instrs_len(6, 11, 0);
  if ((_sv0t12 != 11)) {
    return 1;
  } else {
  }
  int _sv0t13 = lower_expr_with_instrs_len(7, 42, 0);
  if ((_sv0t13 != 42)) {
    return 1;
  } else {
  }
  int _sv0t14 = (-1);
  int _sv0t15 = lower_expr_with_instrs_len(7, _sv0t14, 0);
  if ((_sv0t15 != 255)) {
    return 1;
  } else {
  }
  int _sv0t16 = lower_expr_with_instrs_len(8, 0, 0);
  if ((_sv0t16 != 255)) {
    return 1;
  } else {
  }
  int _sv0t17 = lower_expr_to_value_len(0, 0, 0, 0);
  if ((_sv0t17 != 0)) {
    return 1;
  } else {
  }
  int _sv0t18 = lower_expr_to_value_len(2, 6, 0, 0);
  if ((_sv0t18 != 6)) {
    return 1;
  } else {
  }
  int _sv0t19 = lower_expr_to_value_len(3, 1, 2, 0);
  if ((_sv0t19 != 3)) {
    return 1;
  } else {
  }
  int _sv0t20 = lower_expr_to_value_len(4, 8, 0, 0);
  if ((_sv0t20 != 9)) {
    return 1;
  } else {
  }
  int _sv0t21 = lower_expr_to_value_len(5, 3, 0, 0);
  if ((_sv0t21 != 4)) {
    return 1;
  } else {
  }
  int _sv0t22 = lower_expr_to_value_len(6, 2, 0, 0);
  if ((_sv0t22 != 2)) {
    return 1;
  } else {
  }
  int _sv0t23 = lower_expr_to_value_len(7, 1, 1, 0);
  if ((_sv0t23 != 3)) {
    return 1;
  } else {
  }
  int _sv0t24 = lower_expr_to_value_len(8, 0, 0, 0);
  if ((_sv0t24 != 1)) {
    return 1;
  } else {
  }
  int _sv0t25 = lower_expr_to_value_len(9, 4, 0, 1);
  if ((_sv0t25 != 5)) {
    return 1;
  } else {
  }
  int _sv0t26 = lower_expr_to_value_len(9, 4, 0, 0);
  if ((_sv0t26 != 6)) {
    return 1;
  } else {
  }
  int _sv0t27 = lower_expr_to_value_len(9, 4, 0, 2);
  if ((_sv0t27 != 255)) {
    return 1;
  } else {
  }
  int _sv0t28 = lower_expr_to_value_len(10, 5, 0, 0);
  if ((_sv0t28 != 6)) {
    return 1;
  } else {
  }
  int _sv0t29 = (-1);
  int _sv0t30 = lower_expr_to_value_len(10, _sv0t29, 0, 0);
  if ((_sv0t30 != 255)) {
    return 1;
  } else {
  }
  int _sv0t31 = lower_expr_to_value_len(11, 0, 0, 0);
  if ((_sv0t31 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

