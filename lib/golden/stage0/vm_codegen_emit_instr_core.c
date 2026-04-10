#include "sv0_runtime.h"

static int emit_instr_insn_count(int instr_tag, int expr_insns, int store_width, int extra);
static int local_slot_delta(int instr_tag, int cty_width);

static int emit_instr_insn_count(int instr_tag, int expr_insns, int store_width, int extra) {
  if ((instr_tag == 0)) {
    return 0;
  } else {
  }
  if ((instr_tag == 1)) {
    return 0;
  } else {
  }
  if ((instr_tag == 2)) {
    return 0;
  } else {
  }
  if ((instr_tag == 3)) {
    int _sv0t0 = (expr_insns + store_width);
    return _sv0t0;
  } else {
  }
  if ((instr_tag == 4)) {
    int _sv0t1 = (expr_insns + store_width);
    return _sv0t1;
  } else {
  }
  if ((instr_tag == 5)) {
    int _sv0t2 = (expr_insns + 1);
    return _sv0t2;
  } else {
  }
  if ((instr_tag == 6)) {
    int _sv0t3 = (expr_insns + 1);
    int _sv0t4 = (_sv0t3 + store_width);
    int _sv0t5 = (_sv0t4 + 1);
    int _sv0t6 = (_sv0t5 + extra);
    return _sv0t6;
  } else {
  }
  if ((instr_tag == 8)) {
    return expr_insns;
  } else {
  }
  if ((instr_tag == 9)) {
    return 254;
  } else {
  }
  if ((instr_tag == 10)) {
    return 254;
  } else {
  }
  if ((instr_tag == 11)) {
    return 1;
  } else {
  }
  if ((instr_tag == 12)) {
    int _sv0t7 = (expr_insns + 1);
    return _sv0t7;
  } else {
  }
  if ((instr_tag == 13)) {
    return 2;
  } else {
  }
  if ((instr_tag == 14)) {
    int _sv0t8 = (expr_insns + 1);
    int _sv0t9 = (_sv0t8 + store_width);
    return _sv0t9;
  } else {
  }
  if ((instr_tag == 15)) {
    int _sv0t10 = (expr_insns + 1);
    return _sv0t10;
  } else {
  }
  if ((instr_tag == 16)) {
    int _sv0t11 = (expr_insns + 1);
    int _sv0t12 = (_sv0t11 + store_width);
    return _sv0t12;
  } else {
  }
  if ((instr_tag == 17)) {
    int _sv0t13 = (expr_insns + 1);
    return _sv0t13;
  } else {
  }
  if ((instr_tag == 18)) {
    int _sv0t14 = (expr_insns + 1);
    return _sv0t14;
  } else {
  }
  return 255;
}

static int local_slot_delta(int instr_tag, int cty_width) {
  if ((instr_tag == 1)) {
    return 1;
  } else {
  }
  if ((instr_tag == 2)) {
    return cty_width;
  } else {
  }
  if ((instr_tag == 3)) {
    return 1;
  } else {
  }
  if ((instr_tag == 16)) {
    if ((cty_width > 1)) {
      return cty_width;
    } else {
    }
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = emit_instr_insn_count(0, 0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = emit_instr_insn_count(1, 0, 0, 0);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = emit_instr_insn_count(2, 0, 0, 0);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = emit_instr_insn_count(3, 2, 1, 0);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = emit_instr_insn_count(4, 1, 3, 0);
  if ((_sv0t4 != 4)) {
    return 1;
  } else {
  }
  int _sv0t5 = emit_instr_insn_count(5, 2, 0, 0);
  if ((_sv0t5 != 3)) {
    return 1;
  } else {
  }
  int _sv0t6 = emit_instr_insn_count(6, 1, 3, 2);
  if ((_sv0t6 != 8)) {
    return 1;
  } else {
  }
  int _sv0t7 = emit_instr_insn_count(8, 5, 0, 0);
  if ((_sv0t7 != 5)) {
    return 1;
  } else {
  }
  int _sv0t8 = emit_instr_insn_count(9, 0, 0, 0);
  if ((_sv0t8 != 254)) {
    return 1;
  } else {
  }
  int _sv0t9 = emit_instr_insn_count(10, 0, 0, 0);
  if ((_sv0t9 != 254)) {
    return 1;
  } else {
  }
  int _sv0t10 = emit_instr_insn_count(11, 0, 0, 0);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = emit_instr_insn_count(12, 1, 0, 0);
  if ((_sv0t11 != 2)) {
    return 1;
  } else {
  }
  int _sv0t12 = emit_instr_insn_count(13, 0, 0, 0);
  if ((_sv0t12 != 2)) {
    return 1;
  } else {
  }
  int _sv0t13 = emit_instr_insn_count(14, 3, 2, 0);
  if ((_sv0t13 != 6)) {
    return 1;
  } else {
  }
  int _sv0t14 = emit_instr_insn_count(15, 4, 0, 0);
  if ((_sv0t14 != 5)) {
    return 1;
  } else {
  }
  int _sv0t15 = emit_instr_insn_count(16, 2, 3, 0);
  if ((_sv0t15 != 6)) {
    return 1;
  } else {
  }
  int _sv0t16 = emit_instr_insn_count(17, 1, 0, 0);
  if ((_sv0t16 != 2)) {
    return 1;
  } else {
  }
  int _sv0t17 = emit_instr_insn_count(18, 1, 0, 0);
  if ((_sv0t17 != 2)) {
    return 1;
  } else {
  }
  int _sv0t18 = local_slot_delta(1, 0);
  if ((_sv0t18 != 1)) {
    return 1;
  } else {
  }
  int _sv0t19 = local_slot_delta(2, 3);
  if ((_sv0t19 != 3)) {
    return 1;
  } else {
  }
  int _sv0t20 = local_slot_delta(3, 0);
  if ((_sv0t20 != 1)) {
    return 1;
  } else {
  }
  int _sv0t21 = local_slot_delta(16, 1);
  if ((_sv0t21 != 1)) {
    return 1;
  } else {
  }
  int _sv0t22 = local_slot_delta(16, 4);
  if ((_sv0t22 != 4)) {
    return 1;
  } else {
  }
  int _sv0t23 = local_slot_delta(0, 0);
  if ((_sv0t23 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

