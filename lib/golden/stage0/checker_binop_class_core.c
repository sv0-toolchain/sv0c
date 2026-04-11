#include "sv0_runtime.h"

static int binop_class(int op);
static int binop_result_is_bool(int op_class);
static int classify_assign_lhs(int lhs_tag);

static int binop_class(int op) {
  if ((op >= 0)) {
    if ((op <= 9)) {
      return 0;
    } else {
    }
  } else {
  }
  if ((op == 10)) {
    return 1;
  } else {
  }
  if ((op == 11)) {
    return 1;
  } else {
  }
  if ((op >= 12)) {
    if ((op <= 17)) {
      return 2;
    } else {
    }
  } else {
  }
  return 255;
}

static int binop_result_is_bool(int op_class) {
  if ((op_class == 1)) {
    return 1;
  } else {
  }
  if ((op_class == 2)) {
    return 1;
  } else {
  }
  return 0;
}

static int classify_assign_lhs(int lhs_tag) {
  if ((lhs_tag == 0)) {
    return 0;
  } else {
  }
  if ((lhs_tag == 1)) {
    return 1;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = binop_class(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = binop_class(4);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = binop_class(9);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = binop_class(10);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = binop_class(11);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = binop_class(12);
  if ((_sv0t5 != 2)) {
    return 1;
  } else {
  }
  int _sv0t6 = binop_class(17);
  if ((_sv0t6 != 2)) {
    return 1;
  } else {
  }
  int _sv0t7 = binop_class(18);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  int _sv0t8 = binop_result_is_bool(0);
  if ((_sv0t8 != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = binop_result_is_bool(1);
  if ((_sv0t9 != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = binop_result_is_bool(2);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = classify_assign_lhs(0);
  if ((_sv0t11 != 0)) {
    return 1;
  } else {
  }
  int _sv0t12 = classify_assign_lhs(1);
  if ((_sv0t12 != 1)) {
    return 1;
  } else {
  }
  int _sv0t13 = classify_assign_lhs(2);
  if ((_sv0t13 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

