#include "sv0_runtime.h"

static int ir_expr_load(void);
static int ir_expr_literal(void);
static int v_kind_var(void);
static int value_to_expr_class(int v_kind);

static int ir_expr_load(void) {
  return 1;
}

static int ir_expr_literal(void) {
  return 2;
}

static int v_kind_var(void) {
  return 1;
}

static int value_to_expr_class(int v_kind) {
  int _sv0t2 = v_kind_var();
  int _sv0t0;
  int _sv0t1;
  if ((v_kind == _sv0t2)) {
    int _sv0t3 = ir_expr_load();
    return _sv0t3;
    _sv0t1 = 0;
  } else {
    int _sv0t4 = ir_expr_literal();
    return _sv0t4;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = v_kind_var();
  int _sv0t1 = value_to_expr_class(_sv0t0);
  int _sv0t2 = ir_expr_load();
  if ((_sv0t1 != _sv0t2)) {
    return 1;
  } else {
  }
  int _sv0t3 = value_to_expr_class(0);
  int _sv0t4 = ir_expr_literal();
  if ((_sv0t3 != _sv0t4)) {
    return 1;
  } else {
  }
  int _sv0t5 = value_to_expr_class(2);
  int _sv0t6 = ir_expr_literal();
  if ((_sv0t5 != _sv0t6)) {
    return 1;
  } else {
  }
  int _sv0t7 = value_to_expr_class(99);
  int _sv0t8 = ir_expr_literal();
  if ((_sv0t7 != _sv0t8)) {
    return 1;
  } else {
  }
  return 0;
}

