#include "sv0_runtime.h"

static int lower_expr_for_effect_len(int eff_tag, int a, int b, int c, int d);
static int lower_into_var_instrs_len(int iv_tag, int a, int b, int c, int d);

static int lower_expr_for_effect_len(int eff_tag, int a, int b, int c, int d) {
  if ((eff_tag == 0)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    return a;
  } else {
  }
  if ((eff_tag == 1)) {
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
  if ((eff_tag == 2)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    int _sv0t1 = (a + 1);
    int _sv0t2 = (_sv0t1 + b);
    return _sv0t2;
  } else {
  }
  if ((eff_tag == 3)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    if ((c < 0)) {
      return 255;
    } else {
    }
    int _sv0t3 = (a + 1);
    int _sv0t4 = (_sv0t3 + b);
    int _sv0t5 = (_sv0t4 + c);
    return _sv0t5;
  } else {
  }
  if ((eff_tag == 4)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    if ((c < 0)) {
      return 255;
    } else {
    }
    if ((d < 0)) {
      return 255;
    } else {
    }
    int _sv0t6 = (a + 1);
    int _sv0t7 = (c + 1);
    int _sv0t8 = (b * _sv0t7);
    int _sv0t9 = (_sv0t6 + _sv0t8);
    int _sv0t10 = (_sv0t9 + d);
    return _sv0t10;
  } else {
  }
  if ((eff_tag == 5)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    int _sv0t11 = (a + b);
    return _sv0t11;
  } else {
  }
  if ((eff_tag == 6)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    int _sv0t12 = (1 + a);
    return _sv0t12;
  } else {
  }
  if ((eff_tag == 7)) {
    return 1;
  } else {
  }
  if ((eff_tag == 8)) {
    return 1;
  } else {
  }
  if ((eff_tag == 9)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    int _sv0t13 = (a + 1);
    return _sv0t13;
  } else {
  }
  if ((eff_tag == 10)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    int _sv0t14 = (a + 1);
    return _sv0t14;
  } else {
  }
  if ((eff_tag == 11)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    int _sv0t15 = (a + 1);
    return _sv0t15;
  } else {
  }
  if ((eff_tag == 12)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    return a;
  } else {
  }
  return 255;
}

static int lower_into_var_instrs_len(int iv_tag, int a, int b, int c, int d) {
  if ((iv_tag == 0)) {
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
  if ((iv_tag == 1)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    int _sv0t1 = (a + 1);
    return _sv0t1;
  } else {
  }
  if ((iv_tag == 2)) {
    return 1;
  } else {
  }
  if ((iv_tag == 3)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    return a;
  } else {
  }
  if ((iv_tag == 4)) {
    return 1;
  } else {
  }
  if ((iv_tag == 5)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    if ((c < 0)) {
      return 255;
    } else {
    }
    int _sv0t2 = (a + 2);
    int _sv0t3 = (_sv0t2 + b);
    int _sv0t4 = (_sv0t3 + c);
    return _sv0t4;
  } else {
  }
  if ((iv_tag == 6)) {
    if ((a < 0)) {
      return 255;
    } else {
    }
    if ((b < 0)) {
      return 255;
    } else {
    }
    int _sv0t5 = (a + 1);
    int _sv0t6 = (_sv0t5 + b);
    int _sv0t7 = (_sv0t6 + 1);
    return _sv0t7;
  } else {
  }
  if ((iv_tag == 7)) {
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
  int _sv0t0 = lower_expr_for_effect_len(0, 4, 0, 0, 0);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_expr_for_effect_len(1, 2, 3, 0, 0);
  if ((_sv0t1 != 5)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_expr_for_effect_len(2, 1, 2, 0, 0);
  if ((_sv0t2 != 4)) {
    return 1;
  } else {
  }
  int _sv0t3 = lower_expr_for_effect_len(3, 1, 2, 3, 0);
  if ((_sv0t3 != 7)) {
    return 1;
  } else {
  }
  int _sv0t4 = lower_expr_for_effect_len(4, 1, 2, 0, 5);
  if ((_sv0t4 != 9)) {
    return 1;
  } else {
  }
  int _sv0t5 = lower_expr_for_effect_len(4, 1, 2, 1, 5);
  if ((_sv0t5 != 11)) {
    return 1;
  } else {
  }
  int _sv0t6 = lower_expr_for_effect_len(5, 3, 10, 0, 0);
  if ((_sv0t6 != 13)) {
    return 1;
  } else {
  }
  int _sv0t7 = lower_expr_for_effect_len(6, 9, 0, 0, 0);
  if ((_sv0t7 != 10)) {
    return 1;
  } else {
  }
  int _sv0t8 = lower_expr_for_effect_len(7, 0, 0, 0, 0);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = lower_expr_for_effect_len(9, 4, 0, 0, 0);
  if ((_sv0t9 != 5)) {
    return 1;
  } else {
  }
  int _sv0t10 = lower_expr_for_effect_len(11, 6, 0, 0, 0);
  if ((_sv0t10 != 7)) {
    return 1;
  } else {
  }
  int _sv0t11 = lower_expr_for_effect_len(12, 8, 0, 0, 0);
  if ((_sv0t11 != 8)) {
    return 1;
  } else {
  }
  int _sv0t12 = (-1);
  int _sv0t13 = lower_expr_for_effect_len(2, _sv0t12, 0, 0, 0);
  if ((_sv0t13 != 255)) {
    return 1;
  } else {
  }
  int _sv0t14 = lower_expr_for_effect_len(13, 0, 0, 0, 0);
  if ((_sv0t14 != 255)) {
    return 1;
  } else {
  }
  int _sv0t15 = lower_into_var_instrs_len(0, 2, 5, 0, 0);
  if ((_sv0t15 != 7)) {
    return 1;
  } else {
  }
  int _sv0t16 = lower_into_var_instrs_len(1, 3, 0, 0, 0);
  if ((_sv0t16 != 4)) {
    return 1;
  } else {
  }
  int _sv0t17 = lower_into_var_instrs_len(2, 0, 0, 0, 0);
  if ((_sv0t17 != 1)) {
    return 1;
  } else {
  }
  int _sv0t18 = lower_into_var_instrs_len(3, 6, 0, 0, 0);
  if ((_sv0t18 != 6)) {
    return 1;
  } else {
  }
  int _sv0t19 = lower_into_var_instrs_len(4, 0, 0, 0, 0);
  if ((_sv0t19 != 1)) {
    return 1;
  } else {
  }
  int _sv0t20 = lower_into_var_instrs_len(5, 1, 2, 3, 0);
  if ((_sv0t20 != 8)) {
    return 1;
  } else {
  }
  int _sv0t21 = lower_into_var_instrs_len(6, 1, 2, 0, 0);
  if ((_sv0t21 != 5)) {
    return 1;
  } else {
  }
  int _sv0t22 = lower_into_var_instrs_len(7, 4, 0, 0, 0);
  if ((_sv0t22 != 5)) {
    return 1;
  } else {
  }
  int _sv0t23 = (-1);
  int _sv0t24 = lower_into_var_instrs_len(5, _sv0t23, 0, 0, 0);
  if ((_sv0t24 != 255)) {
    return 1;
  } else {
  }
  int _sv0t25 = lower_into_var_instrs_len(8, 0, 0, 0, 0);
  if ((_sv0t25 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

