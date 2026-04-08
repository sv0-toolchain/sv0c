#include "sv0_runtime.h"

static int binop_tag_add(void);
static int binop_tag_sub(void);
static int binop_tag_mul(void);
static int binop_tag_div(void);
static int binop_tag_rem(void);
static int binop_tag_bitand(void);
static int binop_tag_bitor(void);
static int binop_tag_bitxor(void);
static int binop_tag_shl(void);
static int binop_tag_shr(void);
static int binop_tag_and(void);
static int lower_binop_is_ir_binop(int tag);

static int binop_tag_add(void) {
  return 1;
}

static int binop_tag_sub(void) {
  return 2;
}

static int binop_tag_mul(void) {
  return 3;
}

static int binop_tag_div(void) {
  return 4;
}

static int binop_tag_rem(void) {
  return 5;
}

static int binop_tag_bitand(void) {
  return 6;
}

static int binop_tag_bitor(void) {
  return 7;
}

static int binop_tag_bitxor(void) {
  return 8;
}

static int binop_tag_shl(void) {
  return 9;
}

static int binop_tag_shr(void) {
  return 10;
}

static int binop_tag_and(void) {
  return 11;
}

static int lower_binop_is_ir_binop(int tag) {
  int _sv0t0;
  int _sv0t1;
  if ((tag < 1)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((tag > 10)) {
      return 0;
      _sv0t2 = 0;
    } else {
      return 1;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = binop_tag_add();
  int _sv0t1 = lower_binop_is_ir_binop(_sv0t0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = binop_tag_sub();
  int _sv0t3 = lower_binop_is_ir_binop(_sv0t2);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = binop_tag_mul();
  int _sv0t5 = lower_binop_is_ir_binop(_sv0t4);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = binop_tag_div();
  int _sv0t7 = lower_binop_is_ir_binop(_sv0t6);
  if ((_sv0t7 != 1)) {
    return 1;
  } else {
  }
  int _sv0t8 = binop_tag_rem();
  int _sv0t9 = lower_binop_is_ir_binop(_sv0t8);
  if ((_sv0t9 != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = binop_tag_bitand();
  int _sv0t11 = lower_binop_is_ir_binop(_sv0t10);
  if ((_sv0t11 != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = binop_tag_bitor();
  int _sv0t13 = lower_binop_is_ir_binop(_sv0t12);
  if ((_sv0t13 != 1)) {
    return 1;
  } else {
  }
  int _sv0t14 = binop_tag_bitxor();
  int _sv0t15 = lower_binop_is_ir_binop(_sv0t14);
  if ((_sv0t15 != 1)) {
    return 1;
  } else {
  }
  int _sv0t16 = binop_tag_shl();
  int _sv0t17 = lower_binop_is_ir_binop(_sv0t16);
  if ((_sv0t17 != 1)) {
    return 1;
  } else {
  }
  int _sv0t18 = binop_tag_shr();
  int _sv0t19 = lower_binop_is_ir_binop(_sv0t18);
  if ((_sv0t19 != 1)) {
    return 1;
  } else {
  }
  int _sv0t20 = binop_tag_and();
  int _sv0t21 = lower_binop_is_ir_binop(_sv0t20);
  if ((_sv0t21 != 0)) {
    return 1;
  } else {
  }
  int _sv0t22 = lower_binop_is_ir_binop(0);
  if ((_sv0t22 != 0)) {
    return 1;
  } else {
  }
  int _sv0t23 = lower_binop_is_ir_binop(18);
  if ((_sv0t23 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

