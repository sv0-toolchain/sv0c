#include "sv0_runtime.h"

static int ast_unop_neg(void);
static int ast_unop_not(void);
static int ast_unop_bitnot(void);
static int ast_unop_deref(void);
static int ast_unop_borrow(void);
static int ast_unop_borrow_mut(void);
static int lowers_to_generic_ir_unop(int u);
static int is_addr_borrow_unop(int u);
static int unop_to_c_raises_default(int u);

static int ast_unop_neg(void) {
  return 1;
}

static int ast_unop_not(void) {
  return 2;
}

static int ast_unop_bitnot(void) {
  return 3;
}

static int ast_unop_deref(void) {
  return 4;
}

static int ast_unop_borrow(void) {
  return 5;
}

static int ast_unop_borrow_mut(void) {
  return 6;
}

static int lowers_to_generic_ir_unop(int u) {
  int _sv0t0;
  int _sv0t1;
  if ((u == 1)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((u == 2)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((u == 3)) {
        return 1;
        _sv0t3 = 0;
      } else {
        return 0;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int is_addr_borrow_unop(int u) {
  int _sv0t0;
  int _sv0t1;
  if ((u == 5)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((u == 6)) {
      return 1;
      _sv0t2 = 0;
    } else {
      return 0;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int unop_to_c_raises_default(int u) {
  int _sv0t0;
  int _sv0t1;
  if ((u == 4)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = ast_unop_neg();
  int _sv0t1 = lowers_to_generic_ir_unop(_sv0t0);
  int a0 = (_sv0t1 - 1);
  int _sv0t2 = ast_unop_not();
  int _sv0t3 = lowers_to_generic_ir_unop(_sv0t2);
  int a1 = (_sv0t3 - 1);
  int _sv0t4 = ast_unop_bitnot();
  int _sv0t5 = lowers_to_generic_ir_unop(_sv0t4);
  int a2 = (_sv0t5 - 1);
  int _sv0t6 = ast_unop_deref();
  int _sv0t7 = lowers_to_generic_ir_unop(_sv0t6);
  int a3 = _sv0t7;
  int _sv0t8 = ast_unop_borrow();
  int _sv0t9 = lowers_to_generic_ir_unop(_sv0t8);
  int a4 = _sv0t9;
  int _sv0t10 = ast_unop_borrow_mut();
  int _sv0t11 = lowers_to_generic_ir_unop(_sv0t10);
  int a5 = _sv0t11;
  int _sv0t12 = ast_unop_borrow();
  int _sv0t13 = is_addr_borrow_unop(_sv0t12);
  int b0 = (_sv0t13 - 1);
  int _sv0t14 = ast_unop_borrow_mut();
  int _sv0t15 = is_addr_borrow_unop(_sv0t14);
  int b1 = (_sv0t15 - 1);
  int _sv0t16 = ast_unop_neg();
  int _sv0t17 = is_addr_borrow_unop(_sv0t16);
  int b2 = _sv0t17;
  int _sv0t18 = ast_unop_deref();
  int _sv0t19 = unop_to_c_raises_default(_sv0t18);
  int c0 = (_sv0t19 - 1);
  int _sv0t20 = ast_unop_neg();
  int _sv0t21 = unop_to_c_raises_default(_sv0t20);
  int c1 = _sv0t21;
  int _sv0t22 = (a0 + a1);
  int _sv0t23 = (_sv0t22 + a2);
  int _sv0t24 = (_sv0t23 + a3);
  int _sv0t25 = (_sv0t24 + a4);
  int _sv0t26 = (_sv0t25 + a5);
  int _sv0t27 = (_sv0t26 + b0);
  int _sv0t28 = (_sv0t27 + b1);
  int _sv0t29 = (_sv0t28 + b2);
  int _sv0t30 = (_sv0t29 + c0);
  int _sv0t31 = (_sv0t30 + c1);
  return _sv0t31;
}

