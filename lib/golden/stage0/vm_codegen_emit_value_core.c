#include "sv0_runtime.h"

static int emit_value_insn_count(int value_tag, int var_width);
static int emit_expr_insn_count(int expr_tag, int child_count_a, int child_count_b, int load_width);

static int emit_value_insn_count(int value_tag, int var_width) {
  if ((value_tag == 0)) {
    return 1;
  } else {
  }
  if ((value_tag == 1)) {
    return 1;
  } else {
  }
  if ((value_tag == 2)) {
    return 1;
  } else {
  }
  if ((value_tag == 3)) {
    return 1;
  } else {
  }
  if ((value_tag == 4)) {
    return var_width;
  } else {
  }
  if ((value_tag == 5)) {
    return 1;
  } else {
  }
  if ((value_tag == 6)) {
    return 1;
  } else {
  }
  if ((value_tag == 7)) {
    return 1;
  } else {
  }
  return 255;
}

static int emit_expr_insn_count(int expr_tag, int child_count_a, int child_count_b, int load_width) {
  if ((expr_tag == 0)) {
    return child_count_a;
  } else {
  }
  if ((expr_tag == 1)) {
    return load_width;
  } else {
  }
  if ((expr_tag == 2)) {
    int _sv0t0 = (child_count_a + child_count_b);
    int _sv0t1 = (_sv0t0 + 1);
    return _sv0t1;
  } else {
  }
  if ((expr_tag == 3)) {
    int _sv0t2 = (child_count_a + 1);
    return _sv0t2;
  } else {
  }
  if ((expr_tag == 4)) {
    int _sv0t3 = (child_count_a + 1);
    return _sv0t3;
  } else {
  }
  if ((expr_tag == 5)) {
    return child_count_a;
  } else {
  }
  if ((expr_tag == 6)) {
    return 1;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = emit_value_insn_count(0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = emit_value_insn_count(1, 0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = emit_value_insn_count(2, 0);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = emit_value_insn_count(3, 0);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = emit_value_insn_count(4, 3);
  if ((_sv0t4 != 3)) {
    return 1;
  } else {
  }
  int _sv0t5 = emit_value_insn_count(4, 1);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = emit_value_insn_count(5, 0);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = emit_value_insn_count(6, 0);
  if ((_sv0t7 != 1)) {
    return 1;
  } else {
  }
  int _sv0t8 = emit_value_insn_count(7, 0);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = emit_value_insn_count(99, 0);
  if ((_sv0t9 != 255)) {
    return 1;
  } else {
  }
  int _sv0t10 = emit_expr_insn_count(0, 1, 0, 0);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = emit_expr_insn_count(1, 0, 0, 2);
  if ((_sv0t11 != 2)) {
    return 1;
  } else {
  }
  int _sv0t12 = emit_expr_insn_count(2, 1, 1, 0);
  if ((_sv0t12 != 3)) {
    return 1;
  } else {
  }
  int _sv0t13 = emit_expr_insn_count(2, 3, 1, 0);
  if ((_sv0t13 != 5)) {
    return 1;
  } else {
  }
  int _sv0t14 = emit_expr_insn_count(3, 1, 0, 0);
  if ((_sv0t14 != 2)) {
    return 1;
  } else {
  }
  int _sv0t15 = emit_expr_insn_count(4, 1, 0, 0);
  if ((_sv0t15 != 2)) {
    return 1;
  } else {
  }
  int _sv0t16 = emit_expr_insn_count(5, 1, 0, 0);
  if ((_sv0t16 != 1)) {
    return 1;
  } else {
  }
  int _sv0t17 = emit_expr_insn_count(6, 0, 0, 0);
  if ((_sv0t17 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

