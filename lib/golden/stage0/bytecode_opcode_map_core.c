#include "sv0_runtime.h"

static int magic_byte(int idx);
static int format_version(void);
static int header_size(void);
static int opcode_class(int op);
static int arith_op_count(void);
static int cmp_op_count(void);
static int logic_op_count(void);
static int bitwise_op_count(void);
static int string_pool_entry_size(int str_len);
static int func_entry_header_size(void);

static int magic_byte(int idx) {
  if ((idx == 0)) {
    return 83;
  } else {
  }
  if ((idx == 1)) {
    return 86;
  } else {
  }
  if ((idx == 2)) {
    return 48;
  } else {
  }
  if ((idx == 3)) {
    return 66;
  } else {
  }
  return 0;
}

static int format_version(void) {
  return 1;
}

static int header_size(void) {
  return 8;
}

static int opcode_class(int op) {
  if ((op >= 9)) {
    if ((op <= 24)) {
      return 0;
    } else {
    }
  } else {
  }
  if ((op >= 25)) {
    if ((op <= 30)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((op >= 31)) {
    if ((op <= 33)) {
      return 2;
    } else {
    }
  } else {
  }
  if ((op >= 34)) {
    if ((op <= 35)) {
      return 3;
    } else {
    }
  } else {
  }
  if ((op == 36)) {
    return 4;
  } else {
  }
  if ((op == 37)) {
    return 4;
  } else {
  }
  return 255;
}

static int arith_op_count(void) {
  return 16;
}

static int cmp_op_count(void) {
  return 6;
}

static int logic_op_count(void) {
  return 3;
}

static int bitwise_op_count(void) {
  return 2;
}

static int string_pool_entry_size(int str_len) {
  int _sv0t0 = (4 + str_len);
  return _sv0t0;
}

static int func_entry_header_size(void) {
  int _sv0t0 = (4 + 2);
  int _sv0t1 = (_sv0t0 + 2);
  int _sv0t2 = (_sv0t1 + 4);
  return _sv0t2;
}

int main(void) {
  int _sv0t0 = magic_byte(0);
  if ((_sv0t0 != 83)) {
    return 1;
  } else {
  }
  int _sv0t1 = magic_byte(1);
  if ((_sv0t1 != 86)) {
    return 2;
  } else {
  }
  int _sv0t2 = magic_byte(2);
  if ((_sv0t2 != 48)) {
    return 3;
  } else {
  }
  int _sv0t3 = magic_byte(3);
  if ((_sv0t3 != 66)) {
    return 4;
  } else {
  }
  int _sv0t4 = magic_byte(4);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = format_version();
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = header_size();
  if ((_sv0t6 != 8)) {
    return 7;
  } else {
  }
  int _sv0t7 = opcode_class(9);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = opcode_class(15);
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  int _sv0t9 = opcode_class(24);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = opcode_class(25);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = opcode_class(30);
  if ((_sv0t11 != 1)) {
    return 12;
  } else {
  }
  int _sv0t12 = opcode_class(31);
  if ((_sv0t12 != 2)) {
    return 13;
  } else {
  }
  int _sv0t13 = opcode_class(33);
  if ((_sv0t13 != 2)) {
    return 14;
  } else {
  }
  int _sv0t14 = opcode_class(34);
  if ((_sv0t14 != 3)) {
    return 15;
  } else {
  }
  int _sv0t15 = opcode_class(36);
  if ((_sv0t15 != 4)) {
    return 16;
  } else {
  }
  int _sv0t16 = opcode_class(37);
  if ((_sv0t16 != 4)) {
    return 17;
  } else {
  }
  int _sv0t17 = opcode_class(0);
  if ((_sv0t17 != 255)) {
    return 18;
  } else {
  }
  int _sv0t18 = string_pool_entry_size(5);
  if ((_sv0t18 != 9)) {
    return 19;
  } else {
  }
  int _sv0t19 = string_pool_entry_size(0);
  if ((_sv0t19 != 4)) {
    return 20;
  } else {
  }
  int _sv0t20 = func_entry_header_size();
  if ((_sv0t20 != 12)) {
    return 21;
  } else {
  }
  return 0;
}

