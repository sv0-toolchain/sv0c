#include "sv0_runtime.h"

static int scan_decl_var(int next_slot);
static int scan_decl_named(int next_slot, int cty_width);
static int scan_assign_ensure(int has_binding, int next_slot);
static int scan_call_result(int next_slot, int result_width);
static int scan_instr_delta(int instr_tag, int has_binding, int width);
static int total_locals_after_scan(int init, int d0, int d1, int d2, int d3);
static int call_nargs(int a0_width, int a1_width, int a2_width);
static int test_scan_decl(void);
static int test_scan_assign(void);
static int test_scan_call(void);
static int test_instr_delta(void);
static int test_total_locals(void);
static int test_nargs(void);

static int scan_decl_var(int next_slot) {
  int _sv0t0 = (next_slot + 1);
  return _sv0t0;
}

static int scan_decl_named(int next_slot, int cty_width) {
  int _sv0t0 = (next_slot + cty_width);
  return _sv0t0;
}

static int scan_assign_ensure(int has_binding, int next_slot) {
  if (has_binding) {
    return next_slot;
  } else {
  }
  int _sv0t0 = (next_slot + 1);
  return _sv0t0;
}

static int scan_call_result(int next_slot, int result_width) {
  if ((result_width < 1)) {
    int _sv0t0 = (next_slot + 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = (next_slot + result_width);
  return _sv0t1;
}

static int scan_instr_delta(int instr_tag, int has_binding, int width) {
  if ((instr_tag == 0)) {
    return 1;
  } else {
  }
  if ((instr_tag == 1)) {
    return width;
  } else {
  }
  if ((instr_tag == 2)) {
    if (has_binding) {
      return 0;
    } else {
    }
    return 1;
  } else {
  }
  if ((instr_tag == 3)) {
    if ((width < 1)) {
      return 1;
    } else {
    }
    return width;
  } else {
  }
  return 0;
}

static int total_locals_after_scan(int init, int d0, int d1, int d2, int d3) {
  int _sv0t0 = (init + d0);
  int _sv0t1 = (_sv0t0 + d1);
  int _sv0t2 = (_sv0t1 + d2);
  int _sv0t3 = (_sv0t2 + d3);
  return _sv0t3;
}

static int call_nargs(int a0_width, int a1_width, int a2_width) {
  int _sv0t0 = (a0_width + a1_width);
  int _sv0t1 = (_sv0t0 + a2_width);
  return _sv0t1;
}

static int test_scan_decl(void) {
  int _sv0t0 = scan_decl_var(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = scan_decl_var(5);
  if ((_sv0t1 != 6)) {
    return 2;
  } else {
  }
  int _sv0t2 = scan_decl_named(0, 3);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = scan_decl_named(5, 4);
  if ((_sv0t3 != 9)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_scan_assign(void) {
  int _sv0t0 = scan_assign_ensure(1, 5);
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  int _sv0t1 = scan_assign_ensure(0, 5);
  if ((_sv0t1 != 6)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_scan_call(void) {
  int _sv0t0 = scan_call_result(0, 1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = scan_call_result(0, 3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = scan_call_result(0, 0);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_instr_delta(void) {
  int _sv0t0 = scan_instr_delta(0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = scan_instr_delta(1, 0, 4);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = scan_instr_delta(2, 1, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = scan_instr_delta(2, 0, 0);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = scan_instr_delta(3, 0, 3);
  if ((_sv0t4 != 3)) {
    return 5;
  } else {
  }
  int _sv0t5 = scan_instr_delta(4, 0, 0);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_total_locals(void) {
  int _sv0t0 = total_locals_after_scan(0, 1, 1, 3, 0);
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  int _sv0t1 = total_locals_after_scan(2, 0, 0, 0, 0);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_nargs(void) {
  int _sv0t0 = call_nargs(1, 1, 1);
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = call_nargs(3, 1, 4);
  if ((_sv0t1 != 8)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_scan_decl();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_scan_assign();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_scan_call();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_instr_delta();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_total_locals();
  int r5 = _sv0t8;
  if ((r5 != 0)) {
    int _sv0t9 = (500 + r5);
    return _sv0t9;
  } else {
  }
  int _sv0t10 = test_nargs();
  int r6 = _sv0t10;
  if ((r6 != 0)) {
    int _sv0t11 = (600 + r6);
    return _sv0t11;
  } else {
  }
  return 0;
}

