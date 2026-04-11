#include "sv0_runtime.h"

static int is_single_char_op(int c);
static int can_have_second(int c);
static int op_width(int c1, int c2, int c3);
static int single_char_token_tag(int c);

static int is_single_char_op(int c) {
  int _sv0t0 = (c == 40);
  int _sv0t1 = (c == 41);
  if ((_sv0t0 || _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = (c == 123);
  int _sv0t3 = (c == 125);
  if ((_sv0t2 || _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = (c == 91);
  int _sv0t5 = (c == 93);
  if ((_sv0t4 || _sv0t5)) {
    return 1;
  } else {
  }
  int _sv0t6 = (c == 44);
  int _sv0t7 = (c == 59);
  if ((_sv0t6 || _sv0t7)) {
    return 1;
  } else {
  }
  int _sv0t8 = (c == 35);
  int _sv0t9 = (c == 126);
  int _sv0t10 = (_sv0t8 || _sv0t9);
  int _sv0t11 = (c == 63);
  if ((_sv0t10 || _sv0t11)) {
    return 1;
  } else {
  }
  return 0;
}

static int can_have_second(int c) {
  int _sv0t0 = (c == 58);
  int _sv0t1 = (c == 46);
  if ((_sv0t0 || _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = (c == 43);
  int _sv0t3 = (c == 45);
  int _sv0t4 = (_sv0t2 || _sv0t3);
  int _sv0t5 = (c == 42);
  int _sv0t6 = (_sv0t4 || _sv0t5);
  int _sv0t7 = (c == 47);
  int _sv0t8 = (_sv0t6 || _sv0t7);
  int _sv0t9 = (c == 37);
  if ((_sv0t8 || _sv0t9)) {
    return 1;
  } else {
  }
  int _sv0t10 = (c == 61);
  int _sv0t11 = (c == 33);
  if ((_sv0t10 || _sv0t11)) {
    return 1;
  } else {
  }
  int _sv0t12 = (c == 60);
  int _sv0t13 = (c == 62);
  if ((_sv0t12 || _sv0t13)) {
    return 1;
  } else {
  }
  int _sv0t14 = (c == 38);
  int _sv0t15 = (c == 124);
  int _sv0t16 = (_sv0t14 || _sv0t15);
  int _sv0t17 = (c == 94);
  if ((_sv0t16 || _sv0t17)) {
    return 1;
  } else {
  }
  return 0;
}

static int op_width(int c1, int c2, int c3) {
  int _sv0t0 = is_single_char_op(c1);
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = (c1 == 46);
  int _sv0t2 = (c2 == 46);
  int _sv0t3 = (_sv0t1 && _sv0t2);
  int _sv0t4 = (c3 == 61);
  if ((_sv0t3 && _sv0t4)) {
    return 3;
  } else {
  }
  int _sv0t5 = (c1 == 46);
  int _sv0t6 = (c2 == 46);
  if ((_sv0t5 && _sv0t6)) {
    return 2;
  } else {
  }
  int _sv0t7 = (c1 == 60);
  int _sv0t8 = (c2 == 60);
  int _sv0t9 = (_sv0t7 && _sv0t8);
  int _sv0t10 = (c3 == 61);
  if ((_sv0t9 && _sv0t10)) {
    return 3;
  } else {
  }
  int _sv0t11 = (c1 == 62);
  int _sv0t12 = (c2 == 62);
  int _sv0t13 = (_sv0t11 && _sv0t12);
  int _sv0t14 = (c3 == 61);
  if ((_sv0t13 && _sv0t14)) {
    return 3;
  } else {
  }
  int _sv0t15 = (c1 == 60);
  int _sv0t16 = (c2 == 60);
  if ((_sv0t15 && _sv0t16)) {
    return 2;
  } else {
  }
  int _sv0t17 = (c1 == 62);
  int _sv0t18 = (c2 == 62);
  if ((_sv0t17 && _sv0t18)) {
    return 2;
  } else {
  }
  int _sv0t19 = (c1 == 58);
  int _sv0t20 = (c2 == 58);
  if ((_sv0t19 && _sv0t20)) {
    return 2;
  } else {
  }
  int _sv0t21 = (c1 == 45);
  int _sv0t22 = (c2 == 62);
  if ((_sv0t21 && _sv0t22)) {
    return 2;
  } else {
  }
  int _sv0t23 = (c1 == 61);
  int _sv0t24 = (c2 == 61);
  if ((_sv0t23 && _sv0t24)) {
    return 2;
  } else {
  }
  int _sv0t25 = (c1 == 61);
  int _sv0t26 = (c2 == 62);
  if ((_sv0t25 && _sv0t26)) {
    return 2;
  } else {
  }
  int _sv0t27 = (c1 == 33);
  int _sv0t28 = (c2 == 61);
  if ((_sv0t27 && _sv0t28)) {
    return 2;
  } else {
  }
  int _sv0t29 = (c1 == 60);
  int _sv0t30 = (c2 == 61);
  if ((_sv0t29 && _sv0t30)) {
    return 2;
  } else {
  }
  int _sv0t31 = (c1 == 62);
  int _sv0t32 = (c2 == 61);
  if ((_sv0t31 && _sv0t32)) {
    return 2;
  } else {
  }
  int _sv0t33 = (c1 == 38);
  int _sv0t34 = (c2 == 38);
  if ((_sv0t33 && _sv0t34)) {
    return 2;
  } else {
  }
  int _sv0t35 = (c1 == 124);
  int _sv0t36 = (c2 == 124);
  if ((_sv0t35 && _sv0t36)) {
    return 2;
  } else {
  }
  int _sv0t37 = (c2 == 61);
  int _sv0t38 = can_have_second(c1);
  if ((_sv0t37 && _sv0t38)) {
    return 2;
  } else {
  }
  return 1;
}

static int single_char_token_tag(int c) {
  if ((c == 40)) {
    return 1;
  } else {
  }
  if ((c == 41)) {
    return 2;
  } else {
  }
  if ((c == 123)) {
    return 3;
  } else {
  }
  if ((c == 125)) {
    return 4;
  } else {
  }
  if ((c == 91)) {
    return 5;
  } else {
  }
  if ((c == 93)) {
    return 6;
  } else {
  }
  if ((c == 44)) {
    return 7;
  } else {
  }
  if ((c == 59)) {
    return 8;
  } else {
  }
  if ((c == 35)) {
    return 9;
  } else {
  }
  if ((c == 126)) {
    return 10;
  } else {
  }
  if ((c == 63)) {
    return 11;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = is_single_char_op(40);
  if ((!_sv0t0)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_single_char_op(93);
  if ((!_sv0t1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_single_char_op(61);
  if (_sv0t2) {
    return 3;
  } else {
  }
  int _sv0t3 = can_have_second(61);
  if ((!_sv0t3)) {
    return 4;
  } else {
  }
  int _sv0t4 = can_have_second(60);
  if ((!_sv0t4)) {
    return 5;
  } else {
  }
  int _sv0t5 = can_have_second(40);
  if (_sv0t5) {
    return 6;
  } else {
  }
  int _sv0t6 = op_width(40, 0, 0);
  if ((_sv0t6 != 1)) {
    return 10;
  } else {
  }
  int _sv0t7 = op_width(58, 58, 0);
  if ((_sv0t7 != 2)) {
    return 11;
  } else {
  }
  int _sv0t8 = op_width(46, 46, 61);
  if ((_sv0t8 != 3)) {
    return 12;
  } else {
  }
  int _sv0t9 = op_width(46, 46, 0);
  if ((_sv0t9 != 2)) {
    return 13;
  } else {
  }
  int _sv0t10 = op_width(45, 62, 0);
  if ((_sv0t10 != 2)) {
    return 14;
  } else {
  }
  int _sv0t11 = op_width(61, 61, 0);
  if ((_sv0t11 != 2)) {
    return 15;
  } else {
  }
  int _sv0t12 = op_width(60, 60, 61);
  if ((_sv0t12 != 3)) {
    return 16;
  } else {
  }
  int _sv0t13 = op_width(60, 60, 0);
  if ((_sv0t13 != 2)) {
    return 17;
  } else {
  }
  int _sv0t14 = op_width(43, 61, 0);
  if ((_sv0t14 != 2)) {
    return 18;
  } else {
  }
  int _sv0t15 = op_width(43, 0, 0);
  if ((_sv0t15 != 1)) {
    return 19;
  } else {
  }
  int _sv0t16 = single_char_token_tag(40);
  if ((_sv0t16 != 1)) {
    return 20;
  } else {
  }
  int _sv0t17 = single_char_token_tag(125);
  if ((_sv0t17 != 4)) {
    return 21;
  } else {
  }
  int _sv0t18 = single_char_token_tag(63);
  if ((_sv0t18 != 11)) {
    return 22;
  } else {
  }
  int _sv0t19 = single_char_token_tag(61);
  if ((_sv0t19 != 0)) {
    return 23;
  } else {
  }
  return 0;
}

