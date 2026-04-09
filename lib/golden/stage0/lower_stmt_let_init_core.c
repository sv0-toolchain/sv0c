#include "sv0_runtime.h"

static int lower_stmt_let_init_total_len(int init_kind, int struct_fields_ir_len, int enum_pack_len, int rhs_pre_len, int cty_is_int);

static int lower_stmt_let_init_total_len(int init_kind, int struct_fields_ir_len, int enum_pack_len, int rhs_pre_len, int cty_is_int) {
  if ((init_kind == 0)) {
    if ((struct_fields_ir_len < 0)) {
      return 255;
    } else {
    }
    int _sv0t0 = (1 + struct_fields_ir_len);
    return _sv0t0;
  } else {
  }
  if ((init_kind == 1)) {
    if ((enum_pack_len < 0)) {
      return 255;
    } else {
    }
    return enum_pack_len;
  } else {
  }
  if ((init_kind == 2)) {
    if ((rhs_pre_len < 0)) {
      return 255;
    } else {
    }
    if ((cty_is_int == 1)) {
      int _sv0t1 = (rhs_pre_len + 1);
      return _sv0t1;
    } else {
    }
    if ((cty_is_int == 0)) {
      int _sv0t2 = (rhs_pre_len + 2);
      return _sv0t2;
    } else {
    }
    return 255;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = lower_stmt_let_init_total_len(0, 5, 0, 0, 0);
  if ((_sv0t0 != 6)) {
    return 1;
  } else {
  }
  int _sv0t1 = (-1);
  int _sv0t2 = lower_stmt_let_init_total_len(0, _sv0t1, 0, 0, 0);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = lower_stmt_let_init_total_len(1, 0, 7, 0, 0);
  if ((_sv0t3 != 7)) {
    return 1;
  } else {
  }
  int _sv0t4 = (-1);
  int _sv0t5 = lower_stmt_let_init_total_len(1, 0, _sv0t4, 0, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = lower_stmt_let_init_total_len(2, 0, 0, 3, 1);
  if ((_sv0t6 != 4)) {
    return 1;
  } else {
  }
  int _sv0t7 = lower_stmt_let_init_total_len(2, 0, 0, 3, 0);
  if ((_sv0t7 != 5)) {
    return 1;
  } else {
  }
  int _sv0t8 = (-1);
  int _sv0t9 = lower_stmt_let_init_total_len(2, 0, 0, _sv0t8, 1);
  if ((_sv0t9 != 255)) {
    return 1;
  } else {
  }
  int _sv0t10 = lower_stmt_let_init_total_len(2, 0, 0, 0, 2);
  if ((_sv0t10 != 255)) {
    return 1;
  } else {
  }
  int _sv0t11 = lower_stmt_let_init_total_len(3, 0, 0, 0, 0);
  if ((_sv0t11 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

