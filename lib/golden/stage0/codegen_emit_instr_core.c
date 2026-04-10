#include "sv0_runtime.h"

static int emit_instr_line_count(int instr_kind, int child_lines_a, int child_lines_b);
static int emit_block_body_total_lines(int instr_count, int total_instr_lines);
static int emit_block_defn_lines(int body_lines);
static int return_line_count(int ret_has_value, int ret_ty_is_void);

static int emit_instr_line_count(int instr_kind, int child_lines_a, int child_lines_b) {
  if ((instr_kind == 0)) {
    return 0;
  } else {
  }
  if ((instr_kind == 1)) {
    return 1;
  } else {
  }
  if ((instr_kind == 2)) {
    return 1;
  } else {
  }
  if ((instr_kind == 3)) {
    return 1;
  } else {
  }
  if ((instr_kind == 4)) {
    return 1;
  } else {
  }
  if ((instr_kind == 5)) {
    return 1;
  } else {
  }
  if ((instr_kind == 6)) {
    int _sv0t0 = (3 + child_lines_a);
    int _sv0t1 = (_sv0t0 + child_lines_b);
    return _sv0t1;
  } else {
  }
  if ((instr_kind == 7)) {
    int _sv0t2 = (2 + child_lines_a);
    return _sv0t2;
  } else {
  }
  if ((instr_kind == 8)) {
    int _sv0t3 = (2 + child_lines_a);
    return _sv0t3;
  } else {
  }
  if ((instr_kind == 9)) {
    return 1;
  } else {
  }
  if ((instr_kind == 10)) {
    return 1;
  } else {
  }
  if ((instr_kind == 11)) {
    return 1;
  } else {
  }
  if ((instr_kind == 12)) {
    return 1;
  } else {
  }
  if ((instr_kind == 13)) {
    return 1;
  } else {
  }
  if ((instr_kind == 14)) {
    return 1;
  } else {
  }
  if ((instr_kind == 15)) {
    return 1;
  } else {
  }
  if ((instr_kind == 16)) {
    return 1;
  } else {
  }
  return 255;
}

static int emit_block_body_total_lines(int instr_count, int total_instr_lines) {
  if ((instr_count == 0)) {
    return 0;
  } else {
  }
  return total_instr_lines;
}

static int emit_block_defn_lines(int body_lines) {
  int _sv0t0 = (1 + body_lines);
  int _sv0t1 = (_sv0t0 + 1);
  return _sv0t1;
}

static int return_line_count(int ret_has_value, int ret_ty_is_void) {
  if ((ret_has_value == 0)) {
    if ((ret_ty_is_void == 1)) {
      return 1;
    } else {
    }
    return 254;
  } else {
  }
  return 1;
}

int main(void) {
  int _sv0t0 = emit_instr_line_count(0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = emit_instr_line_count(1, 0, 0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = emit_instr_line_count(2, 0, 0);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = emit_instr_line_count(3, 0, 0);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = emit_instr_line_count(4, 0, 0);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = emit_instr_line_count(5, 0, 0);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = emit_instr_line_count(6, 2, 3);
  if ((_sv0t6 != 8)) {
    return 1;
  } else {
  }
  int _sv0t7 = emit_instr_line_count(6, 0, 0);
  if ((_sv0t7 != 3)) {
    return 1;
  } else {
  }
  int _sv0t8 = emit_instr_line_count(7, 4, 0);
  if ((_sv0t8 != 6)) {
    return 1;
  } else {
  }
  int _sv0t9 = emit_instr_line_count(8, 5, 0);
  if ((_sv0t9 != 7)) {
    return 1;
  } else {
  }
  int _sv0t10 = emit_instr_line_count(9, 0, 0);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = emit_instr_line_count(10, 0, 0);
  if ((_sv0t11 != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = emit_instr_line_count(11, 0, 0);
  if ((_sv0t12 != 1)) {
    return 1;
  } else {
  }
  int _sv0t13 = emit_instr_line_count(12, 0, 0);
  if ((_sv0t13 != 1)) {
    return 1;
  } else {
  }
  int _sv0t14 = emit_instr_line_count(13, 0, 0);
  if ((_sv0t14 != 1)) {
    return 1;
  } else {
  }
  int _sv0t15 = emit_instr_line_count(14, 0, 0);
  if ((_sv0t15 != 1)) {
    return 1;
  } else {
  }
  int _sv0t16 = emit_instr_line_count(15, 0, 0);
  if ((_sv0t16 != 1)) {
    return 1;
  } else {
  }
  int _sv0t17 = emit_instr_line_count(16, 0, 0);
  if ((_sv0t17 != 1)) {
    return 1;
  } else {
  }
  int _sv0t18 = emit_instr_line_count(99, 0, 0);
  if ((_sv0t18 != 255)) {
    return 1;
  } else {
  }
  int _sv0t19 = emit_block_body_total_lines(0, 0);
  if ((_sv0t19 != 0)) {
    return 1;
  } else {
  }
  int _sv0t20 = emit_block_body_total_lines(5, 8);
  if ((_sv0t20 != 8)) {
    return 1;
  } else {
  }
  int _sv0t21 = emit_block_defn_lines(3);
  if ((_sv0t21 != 5)) {
    return 1;
  } else {
  }
  int _sv0t22 = return_line_count(0, 1);
  if ((_sv0t22 != 1)) {
    return 1;
  } else {
  }
  int _sv0t23 = return_line_count(0, 0);
  if ((_sv0t23 != 254)) {
    return 1;
  } else {
  }
  int _sv0t24 = return_line_count(1, 1);
  if ((_sv0t24 != 1)) {
    return 1;
  } else {
  }
  int _sv0t25 = return_line_count(1, 0);
  if ((_sv0t25 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

