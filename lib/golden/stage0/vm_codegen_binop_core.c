#include "sv0_runtime.h"

static int binop_to_insn_tag(int op_tag);
static int unop_to_insn_tag(int unop_tag);

static int binop_to_insn_tag(int op_tag) {
  if ((op_tag >= 0)) {
    if ((op_tag <= 17)) {
      return op_tag;
    } else {
    }
  } else {
  }
  return 255;
}

static int unop_to_insn_tag(int unop_tag) {
  if ((unop_tag == 0)) {
    return 0;
  } else {
  }
  if ((unop_tag == 1)) {
    return 1;
  } else {
  }
  return 2;
}

int main(void) {
  int _sv0t0 = binop_to_insn_tag(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = binop_to_insn_tag(4);
  if ((_sv0t1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t2 = binop_to_insn_tag(10);
  if ((_sv0t2 != 10)) {
    return 1;
  } else {
  }
  int _sv0t3 = binop_to_insn_tag(17);
  if ((_sv0t3 != 17)) {
    return 1;
  } else {
  }
  int _sv0t4 = binop_to_insn_tag(18);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  int _sv0t5 = (0 - 1);
  int _sv0t6 = binop_to_insn_tag(_sv0t5);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  int _sv0t7 = unop_to_insn_tag(0);
  if ((_sv0t7 != 0)) {
    return 1;
  } else {
  }
  int _sv0t8 = unop_to_insn_tag(1);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = unop_to_insn_tag(2);
  if ((_sv0t9 != 2)) {
    return 1;
  } else {
  }
  int _sv0t10 = unop_to_insn_tag(99);
  if ((_sv0t10 != 2)) {
    return 1;
  } else {
  }
  return 0;
}

