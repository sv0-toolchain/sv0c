#include "sv0_runtime.h"

static int emit_expr_segment_count(int expr_kind, int value_segs);
static int emit_param_list_segment_count(int param_count);
static int emit_fn_proto_segment_count(int is_static, int param_count);

static int emit_expr_segment_count(int expr_kind, int value_segs) {
  if ((expr_kind == 1)) {
    return value_segs;
  } else {
  }
  if ((expr_kind == 2)) {
    return 1;
  } else {
  }
  if ((expr_kind == 3)) {
    return 5;
  } else {
  }
  if ((expr_kind == 4)) {
    return 3;
  } else {
  }
  if ((expr_kind == 5)) {
    return 4;
  } else {
  }
  return 255;
}

static int emit_param_list_segment_count(int param_count) {
  if ((param_count == 0)) {
    return 1;
  } else {
  }
  int _sv0t0 = (param_count * 2);
  int _sv0t1 = (_sv0t0 + param_count);
  int _sv0t2 = (_sv0t1 - 1);
  return _sv0t2;
}

static int emit_fn_proto_segment_count(int is_static, int param_count) {
  int base = 3;
  int _sv0t0 = emit_param_list_segment_count(param_count);
  int param_segs = _sv0t0;
  int _sv0t1 = (is_static + base);
  int _sv0t2 = (_sv0t1 + param_segs);
  return _sv0t2;
}

int main(void) {
  int _sv0t0 = emit_expr_segment_count(1, 1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = emit_expr_segment_count(1, 3);
  if ((_sv0t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t2 = emit_expr_segment_count(2, 0);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = emit_expr_segment_count(3, 0);
  if ((_sv0t3 != 5)) {
    return 1;
  } else {
  }
  int _sv0t4 = emit_expr_segment_count(4, 0);
  if ((_sv0t4 != 3)) {
    return 1;
  } else {
  }
  int _sv0t5 = emit_expr_segment_count(5, 0);
  if ((_sv0t5 != 4)) {
    return 1;
  } else {
  }
  int _sv0t6 = emit_expr_segment_count(99, 0);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  int _sv0t7 = emit_param_list_segment_count(0);
  if ((_sv0t7 != 1)) {
    return 1;
  } else {
  }
  int _sv0t8 = emit_param_list_segment_count(1);
  if ((_sv0t8 != 2)) {
    return 1;
  } else {
  }
  int _sv0t9 = emit_param_list_segment_count(3);
  if ((_sv0t9 != 8)) {
    return 1;
  } else {
  }
  int _sv0t10 = emit_fn_proto_segment_count(1, 0);
  if ((_sv0t10 != 5)) {
    return 1;
  } else {
  }
  int _sv0t11 = emit_fn_proto_segment_count(0, 2);
  if ((_sv0t11 != 8)) {
    return 1;
  } else {
  }
  return 0;
}

