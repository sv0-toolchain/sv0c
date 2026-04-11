#include "sv0_runtime.h"

static int insn_byte_size(int opcode);
static int is_jump_insn(int opcode);
static int is_push_insn(int opcode);

static int insn_byte_size(int opcode) {
  if ((opcode == 0)) {
    return 1;
  } else {
  }
  if ((opcode == 1)) {
    return 1;
  } else {
  }
  if ((opcode == 2)) {
    return 1;
  } else {
  }
  if ((opcode == 3)) {
    return 1;
  } else {
  }
  if ((opcode == 4)) {
    return 5;
  } else {
  }
  if ((opcode == 5)) {
    return 9;
  } else {
  }
  if ((opcode == 6)) {
    return 9;
  } else {
  }
  if ((opcode == 7)) {
    return 2;
  } else {
  }
  if ((opcode == 8)) {
    return 5;
  } else {
  }
  if ((opcode >= 9)) {
    if ((opcode <= 35)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((opcode == 36)) {
    return 2;
  } else {
  }
  if ((opcode == 37)) {
    return 2;
  } else {
  }
  if ((opcode == 38)) {
    return 5;
  } else {
  }
  if ((opcode == 39)) {
    return 5;
  } else {
  }
  if ((opcode == 40)) {
    return 5;
  } else {
  }
  if ((opcode == 41)) {
    return 5;
  } else {
  }
  if ((opcode == 42)) {
    return 1;
  } else {
  }
  if ((opcode == 43)) {
    return 2;
  } else {
  }
  if ((opcode == 44)) {
    return 2;
  } else {
  }
  if ((opcode == 45)) {
    return 2;
  } else {
  }
  if ((opcode == 46)) {
    return 2;
  } else {
  }
  if ((opcode == 47)) {
    return 2;
  } else {
  }
  if ((opcode == 48)) {
    return 2;
  } else {
  }
  if ((opcode == 49)) {
    return 1;
  } else {
  }
  if ((opcode == 50)) {
    return 1;
  } else {
  }
  if ((opcode == 51)) {
    return 4;
  } else {
  }
  if ((opcode == 52)) {
    return 1;
  } else {
  }
  if ((opcode == 53)) {
    return 2;
  } else {
  }
  if ((opcode == 54)) {
    return 2;
  } else {
  }
  if ((opcode == 55)) {
    return 3;
  } else {
  }
  return 0;
}

static int is_jump_insn(int opcode) {
  if ((opcode == 38)) {
    return 1;
  } else {
  }
  if ((opcode == 39)) {
    return 1;
  } else {
  }
  if ((opcode == 40)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_push_insn(int opcode) {
  if ((opcode == 3)) {
    return 1;
  } else {
  }
  if ((opcode == 4)) {
    return 1;
  } else {
  }
  if ((opcode == 5)) {
    return 1;
  } else {
  }
  if ((opcode == 6)) {
    return 1;
  } else {
  }
  if ((opcode == 7)) {
    return 1;
  } else {
  }
  if ((opcode == 8)) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = insn_byte_size(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = insn_byte_size(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = insn_byte_size(2);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = insn_byte_size(3);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = insn_byte_size(4);
  if ((_sv0t4 != 5)) {
    return 5;
  } else {
  }
  int _sv0t5 = insn_byte_size(5);
  if ((_sv0t5 != 9)) {
    return 6;
  } else {
  }
  int _sv0t6 = insn_byte_size(6);
  if ((_sv0t6 != 9)) {
    return 7;
  } else {
  }
  int _sv0t7 = insn_byte_size(7);
  if ((_sv0t7 != 2)) {
    return 8;
  } else {
  }
  int _sv0t8 = insn_byte_size(8);
  if ((_sv0t8 != 5)) {
    return 9;
  } else {
  }
  int _sv0t9 = insn_byte_size(9);
  if ((_sv0t9 != 1)) {
    return 10;
  } else {
  }
  int _sv0t10 = insn_byte_size(20);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = insn_byte_size(35);
  if ((_sv0t11 != 1)) {
    return 12;
  } else {
  }
  int _sv0t12 = insn_byte_size(36);
  if ((_sv0t12 != 2)) {
    return 13;
  } else {
  }
  int _sv0t13 = insn_byte_size(37);
  if ((_sv0t13 != 2)) {
    return 14;
  } else {
  }
  int _sv0t14 = insn_byte_size(38);
  if ((_sv0t14 != 5)) {
    return 15;
  } else {
  }
  int _sv0t15 = insn_byte_size(41);
  if ((_sv0t15 != 5)) {
    return 16;
  } else {
  }
  int _sv0t16 = insn_byte_size(42);
  if ((_sv0t16 != 1)) {
    return 17;
  } else {
  }
  int _sv0t17 = insn_byte_size(43);
  if ((_sv0t17 != 2)) {
    return 18;
  } else {
  }
  int _sv0t18 = insn_byte_size(44);
  if ((_sv0t18 != 2)) {
    return 19;
  } else {
  }
  int _sv0t19 = is_jump_insn(38);
  if ((_sv0t19 != 1)) {
    return 20;
  } else {
  }
  int _sv0t20 = is_jump_insn(39);
  if ((_sv0t20 != 1)) {
    return 21;
  } else {
  }
  int _sv0t21 = is_jump_insn(40);
  if ((_sv0t21 != 1)) {
    return 22;
  } else {
  }
  int _sv0t22 = is_jump_insn(0);
  if ((_sv0t22 != 0)) {
    return 23;
  } else {
  }
  int _sv0t23 = is_jump_insn(42);
  if ((_sv0t23 != 0)) {
    return 24;
  } else {
  }
  int _sv0t24 = is_push_insn(3);
  if ((_sv0t24 != 1)) {
    return 25;
  } else {
  }
  int _sv0t25 = is_push_insn(4);
  if ((_sv0t25 != 1)) {
    return 26;
  } else {
  }
  int _sv0t26 = is_push_insn(8);
  if ((_sv0t26 != 1)) {
    return 27;
  } else {
  }
  int _sv0t27 = is_push_insn(0);
  if ((_sv0t27 != 0)) {
    return 28;
  } else {
  }
  int _sv0t28 = is_push_insn(36);
  if ((_sv0t28 != 0)) {
    return 29;
  } else {
  }
  return 0;
}

