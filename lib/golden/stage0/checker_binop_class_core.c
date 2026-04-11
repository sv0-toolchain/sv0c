#include "sv0_runtime.h"

static int binop_class(int op_tag);
static int binop_returns_bool(int op_tag);
static int binop_is_arith(int op_tag);

static int binop_class(int op_tag) {
  if ((op_tag == 0)) {
    return 0;
  } else {
  }
  if ((op_tag == 1)) {
    return 0;
  } else {
  }
  if ((op_tag == 2)) {
    return 0;
  } else {
  }
  if ((op_tag == 3)) {
    return 0;
  } else {
  }
  if ((op_tag == 4)) {
    return 0;
  } else {
  }
  if ((op_tag == 5)) {
    return 0;
  } else {
  }
  if ((op_tag == 6)) {
    return 0;
  } else {
  }
  if ((op_tag == 7)) {
    return 0;
  } else {
  }
  if ((op_tag == 8)) {
    return 0;
  } else {
  }
  if ((op_tag == 9)) {
    return 0;
  } else {
  }
  if ((op_tag == 10)) {
    return 1;
  } else {
  }
  if ((op_tag == 11)) {
    return 1;
  } else {
  }
  if ((op_tag == 12)) {
    return 2;
  } else {
  }
  if ((op_tag == 13)) {
    return 2;
  } else {
  }
  if ((op_tag == 14)) {
    return 2;
  } else {
  }
  if ((op_tag == 15)) {
    return 2;
  } else {
  }
  if ((op_tag == 16)) {
    return 2;
  } else {
  }
  if ((op_tag == 17)) {
    return 2;
  } else {
  }
  return 255;
}

static int binop_returns_bool(int op_tag) {
  int _sv0t0 = binop_class(op_tag);
  int kind = _sv0t0;
  if ((kind == 1)) {
    return 1;
  } else {
  }
  if ((kind == 2)) {
    return 1;
  } else {
  }
  return 0;
}

static int binop_is_arith(int op_tag) {
  int _sv0t0 = binop_class(op_tag);
  int _sv0t1 = (_sv0t0 == 0);
  return _sv0t1;
}

int main(void) {
  int _sv0t0 = binop_class(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = binop_class(1);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = binop_class(2);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = binop_class(3);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = binop_class(4);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = binop_class(5);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = binop_class(6);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = binop_class(7);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = binop_class(8);
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  int _sv0t9 = binop_class(9);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = binop_class(10);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = binop_class(11);
  if ((_sv0t11 != 1)) {
    return 12;
  } else {
  }
  int _sv0t12 = binop_class(12);
  if ((_sv0t12 != 2)) {
    return 13;
  } else {
  }
  int _sv0t13 = binop_class(13);
  if ((_sv0t13 != 2)) {
    return 14;
  } else {
  }
  int _sv0t14 = binop_class(14);
  if ((_sv0t14 != 2)) {
    return 15;
  } else {
  }
  int _sv0t15 = binop_class(15);
  if ((_sv0t15 != 2)) {
    return 16;
  } else {
  }
  int _sv0t16 = binop_class(16);
  if ((_sv0t16 != 2)) {
    return 17;
  } else {
  }
  int _sv0t17 = binop_class(17);
  if ((_sv0t17 != 2)) {
    return 18;
  } else {
  }
  int _sv0t18 = binop_class(99);
  if ((_sv0t18 != 255)) {
    return 19;
  } else {
  }
  int _sv0t19 = binop_returns_bool(0);
  if ((_sv0t19 != 0)) {
    return 20;
  } else {
  }
  int _sv0t20 = binop_returns_bool(10);
  if ((_sv0t20 != 1)) {
    return 21;
  } else {
  }
  int _sv0t21 = binop_returns_bool(12);
  if ((_sv0t21 != 1)) {
    return 22;
  } else {
  }
  int _sv0t22 = binop_is_arith(0);
  if ((_sv0t22 != 1)) {
    return 23;
  } else {
  }
  int _sv0t23 = binop_is_arith(10);
  if ((_sv0t23 != 0)) {
    return 24;
  } else {
  }
  int _sv0t24 = binop_is_arith(12);
  if ((_sv0t24 != 0)) {
    return 25;
  } else {
  }
  return 0;
}

