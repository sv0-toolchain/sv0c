#include "sv0_runtime.h"

static int ir_expr_load(void);
static int ir_expr_literal(void);
static int v_kind_var(void);
static int value_to_expr_class(int v_kind);
static int enum_payload_field_suffix(int i);
static int enum_store_payload_count(int len);

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

static int enum_payload_field_suffix(int i) {
  return i;
}

static int enum_store_payload_count(int len) {
  return len;
}

int main(void) {
  int _sv0t0 = enum_store_payload_count(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = enum_store_payload_count(4);
  if ((_sv0t1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t2 = enum_payload_field_suffix(0);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = enum_payload_field_suffix(3);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = v_kind_var();
  int _sv0t5 = value_to_expr_class(_sv0t4);
  int _sv0t6 = ir_expr_load();
  if ((_sv0t5 != _sv0t6)) {
    return 1;
  } else {
  }
  int _sv0t7 = value_to_expr_class(0);
  int _sv0t8 = ir_expr_literal();
  if ((_sv0t7 != _sv0t8)) {
    return 1;
  } else {
  }
  int _sv0t9 = enum_payload_field_suffix(0);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = v_kind_var();
  int _sv0t11 = value_to_expr_class(_sv0t10);
  int _sv0t12 = ir_expr_load();
  if ((_sv0t11 != _sv0t12)) {
    return 1;
  } else {
  }
  int _sv0t13 = enum_payload_field_suffix(1);
  if ((_sv0t13 != 1)) {
    return 1;
  } else {
  }
  int _sv0t14 = value_to_expr_class(99);
  int _sv0t15 = ir_expr_literal();
  if ((_sv0t14 != _sv0t15)) {
    return 1;
  } else {
  }
  return 0;
}

