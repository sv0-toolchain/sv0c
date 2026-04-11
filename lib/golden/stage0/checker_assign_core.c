#include "sv0_runtime.h"

static int assign_lhs_kind(int expr_tag);
static int assign_is_valid_lhs(int kind);
static int assign_needs_mut_check(int kind);
static int compound_assign_result_ty(int lhs_ty_tag, int op_tag);
static int is_numeric_ty(int ty_tag);
static int is_bitwise_op(int op_tag);
static int is_arith_op(int op_tag);
static int assign_error_code(int kind);

static int assign_lhs_kind(int expr_tag) {
  if ((expr_tag == 0)) {
    return 0;
  } else {
  }
  if ((expr_tag == 3)) {
    return 1;
  } else {
  }
  if ((expr_tag == 24)) {
    return 2;
  } else {
  }
  if ((expr_tag == 7)) {
    return 3;
  } else {
  }
  if ((expr_tag == 8)) {
    return 4;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int assign_is_valid_lhs(int kind) {
  int _sv0t0 = (kind >= 0);
  int _sv0t1 = (kind <= 4);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int assign_needs_mut_check(int kind) {
  int _sv0t0 = (kind == 0);
  return _sv0t0;
}

static int compound_assign_result_ty(int lhs_ty_tag, int op_tag) {
  int _sv0t0 = (lhs_ty_tag >= 0);
  int _sv0t1 = (lhs_ty_tag <= 10);
  if ((_sv0t0 && _sv0t1)) {
    int _sv0t2 = (op_tag >= 0);
    int _sv0t3 = (op_tag <= 4);
    if ((_sv0t2 && _sv0t3)) {
      return lhs_ty_tag;
    } else {
    }
    int _sv0t4 = (op_tag >= 5);
    int _sv0t5 = (op_tag <= 9);
    if ((_sv0t4 && _sv0t5)) {
      return lhs_ty_tag;
    } else {
    }
  } else {
  }
  int _sv0t6 = (-1);
  return _sv0t6;
}

static int is_numeric_ty(int ty_tag) {
  int _sv0t0 = (ty_tag >= 0);
  int _sv0t1 = (ty_tag <= 10);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int is_bitwise_op(int op_tag) {
  int _sv0t0 = (op_tag >= 5);
  int _sv0t1 = (op_tag <= 9);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int is_arith_op(int op_tag) {
  int _sv0t0 = (op_tag >= 0);
  int _sv0t1 = (op_tag <= 4);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int assign_error_code(int kind) {
  int _sv0t0 = (-1);
  if ((kind == _sv0t0)) {
    return 448;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = assign_lhs_kind(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = assign_lhs_kind(3);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = assign_lhs_kind(24);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = assign_lhs_kind(99);
  int _sv0t4 = (-1);
  if ((_sv0t3 != _sv0t4)) {
    return 4;
  } else {
  }
  int _sv0t5 = assign_is_valid_lhs(0);
  if ((!_sv0t5)) {
    return 5;
  } else {
  }
  int _sv0t6 = assign_is_valid_lhs(4);
  if ((!_sv0t6)) {
    return 6;
  } else {
  }
  int _sv0t7 = (-1);
  int _sv0t8 = assign_is_valid_lhs(_sv0t7);
  if (_sv0t8) {
    return 7;
  } else {
  }
  int _sv0t9 = assign_needs_mut_check(0);
  if ((!_sv0t9)) {
    return 8;
  } else {
  }
  int _sv0t10 = assign_needs_mut_check(1);
  if (_sv0t10) {
    return 9;
  } else {
  }
  int _sv0t11 = compound_assign_result_ty(3, 0);
  if ((_sv0t11 != 3)) {
    return 10;
  } else {
  }
  int _sv0t12 = compound_assign_result_ty(3, 7);
  if ((_sv0t12 != 3)) {
    return 11;
  } else {
  }
  int _sv0t13 = compound_assign_result_ty(99, 0);
  int _sv0t14 = (-1);
  if ((_sv0t13 != _sv0t14)) {
    return 12;
  } else {
  }
  int _sv0t15 = is_numeric_ty(3);
  if ((!_sv0t15)) {
    return 13;
  } else {
  }
  int _sv0t16 = is_numeric_ty(99);
  if (_sv0t16) {
    return 14;
  } else {
  }
  int _sv0t17 = is_bitwise_op(5);
  if ((!_sv0t17)) {
    return 15;
  } else {
  }
  int _sv0t18 = is_bitwise_op(0);
  if (_sv0t18) {
    return 16;
  } else {
  }
  int _sv0t19 = is_arith_op(0);
  if ((!_sv0t19)) {
    return 17;
  } else {
  }
  int _sv0t20 = is_arith_op(5);
  if (_sv0t20) {
    return 18;
  } else {
  }
  int _sv0t21 = (-1);
  int _sv0t22 = assign_error_code(_sv0t21);
  if ((_sv0t22 != 448)) {
    return 19;
  } else {
  }
  int _sv0t23 = assign_error_code(0);
  if ((_sv0t23 != 0)) {
    return 20;
  } else {
  }
  return 0;
}

