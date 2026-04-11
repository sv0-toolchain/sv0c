#include "sv0_runtime.h"

static int arg_width(int is_var, int var_slot_width);
static int sum_arg_widths(int a0_var, int a0_w, int a1_var, int a1_w, int a2_var, int a2_w, int a3_var, int a3_w);
static int func_index_found(int idx);
static int call_insn_payload_size(int nargs);
static int call_builtin_payload_size(void);
static int is_builtin_call(int func_name_starts_with);
static int test_arg_width(void);
static int test_sum_widths(void);
static int test_func_index(void);
static int test_payload_sizes(void);

static int arg_width(int is_var, int var_slot_width) {
  if (is_var) {
    return var_slot_width;
  } else {
  }
  return 1;
}

static int sum_arg_widths(int a0_var, int a0_w, int a1_var, int a1_w, int a2_var, int a2_w, int a3_var, int a3_w) {
  int total = 0;
  int _sv0t0 = arg_width(a0_var, a0_w);
  total = (total + _sv0t0);
  int _sv0t1 = arg_width(a1_var, a1_w);
  total = (total + _sv0t1);
  int _sv0t2 = arg_width(a2_var, a2_w);
  total = (total + _sv0t2);
  int _sv0t3 = arg_width(a3_var, a3_w);
  total = (total + _sv0t3);
  return total;
}

static int func_index_found(int idx) {
  int _sv0t0 = (idx >= 0);
  return _sv0t0;
}

static int call_insn_payload_size(int nargs) {
  int _sv0t0 = (4 + 4);
  int _sv0t1 = (_sv0t0 + 2);
  return _sv0t1;
}

static int call_builtin_payload_size(void) {
  return 2;
}

static int is_builtin_call(int func_name_starts_with) {
  int _sv0t0 = (func_name_starts_with == 1);
  return _sv0t0;
}

static int test_arg_width(void) {
  int _sv0t0 = arg_width(0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = arg_width(1, 3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = arg_width(1, 1);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_sum_widths(void) {
  int _sv0t0 = sum_arg_widths(0, 0, 0, 0, 0, 0, 0, 0);
  int s1 = _sv0t0;
  if ((s1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = sum_arg_widths(1, 3, 0, 0, 1, 2, 0, 0);
  int s2 = _sv0t1;
  if ((s2 != 7)) {
    return 2;
  } else {
  }
  int _sv0t2 = sum_arg_widths(1, 1, 1, 1, 1, 1, 1, 1);
  int s3 = _sv0t2;
  if ((s3 != 4)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_func_index(void) {
  int _sv0t0 = func_index_found(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = func_index_found(5);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = (0 - 1);
  int _sv0t3 = func_index_found(_sv0t2);
  if ((_sv0t3 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_payload_sizes(void) {
  int _sv0t0 = call_insn_payload_size(3);
  if ((_sv0t0 != 10)) {
    return 1;
  } else {
  }
  int _sv0t1 = call_builtin_payload_size();
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_arg_width();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_sum_widths();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_func_index();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_payload_sizes();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

