#include "sv0_runtime.h"

static int loop_continue_sentinel(void);
static int loop_exit_sentinel(void);
static int is_continue_sentinel(int jump_offset);
static int is_exit_sentinel(int jump_offset);
static int patch_continue_jump(int jump_offset, int current_pos, int insn_len, int target_abs);
static int replace_exit_jump(int last_jump_offset, int back_edge_delta);
static int compute_back_edge(int loop_start, int body_end, int jump_size);
static int loop_body_offset(int cond_width, int jifnot_size);
static int test_sentinels(void);
static int test_patch_continue(void);
static int test_replace_exit(void);
static int test_back_edge(void);
static int test_body_offset(void);

static int loop_continue_sentinel(void) {
  return 2147483645;
}

static int loop_exit_sentinel(void) {
  return 2147483646;
}

static int is_continue_sentinel(int jump_offset) {
  int _sv0t0 = (jump_offset == 2147483645);
  return _sv0t0;
}

static int is_exit_sentinel(int jump_offset) {
  int _sv0t0 = (jump_offset == 2147483646);
  return _sv0t0;
}

static int patch_continue_jump(int jump_offset, int current_pos, int insn_len, int target_abs) {
  int _sv0t0 = is_continue_sentinel(jump_offset);
  if (_sv0t0) {
    int _sv0t1 = (current_pos + insn_len);
    int _sv0t2 = (target_abs - _sv0t1);
    return _sv0t2;
  } else {
  }
  return jump_offset;
}

static int replace_exit_jump(int last_jump_offset, int back_edge_delta) {
  int _sv0t0 = is_exit_sentinel(last_jump_offset);
  if (_sv0t0) {
    return back_edge_delta;
  } else {
  }
  int _sv0t1 = (0 - 1);
  return _sv0t1;
}

static int compute_back_edge(int loop_start, int body_end, int jump_size) {
  int _sv0t0 = (body_end + jump_size);
  int _sv0t1 = (loop_start - _sv0t0);
  return _sv0t1;
}

static int loop_body_offset(int cond_width, int jifnot_size) {
  int _sv0t0 = (cond_width + jifnot_size);
  return _sv0t0;
}

static int test_sentinels(void) {
  int _sv0t0 = is_continue_sentinel(2147483645);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_continue_sentinel(0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_exit_sentinel(2147483646);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_exit_sentinel(2147483645);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = loop_continue_sentinel();
  if ((_sv0t4 != 2147483645)) {
    return 5;
  } else {
  }
  int _sv0t5 = loop_exit_sentinel();
  if ((_sv0t5 != 2147483646)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_patch_continue(void) {
  int _sv0t0 = patch_continue_jump(2147483645, 10, 5, 0);
  int r1 = _sv0t0;
  int _sv0t1 = (0 - 15);
  if ((r1 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = patch_continue_jump(2147483645, 0, 5, 20);
  int r2 = _sv0t2;
  if ((r2 != 15)) {
    return 2;
  } else {
  }
  int _sv0t3 = patch_continue_jump(42, 10, 5, 0);
  int r3 = _sv0t3;
  if ((r3 != 42)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_replace_exit(void) {
  int _sv0t0 = (0 - 50);
  int _sv0t1 = replace_exit_jump(2147483646, _sv0t0);
  int r1 = _sv0t1;
  int _sv0t2 = (0 - 50);
  if ((r1 != _sv0t2)) {
    return 1;
  } else {
  }
  int _sv0t3 = (0 - 50);
  int _sv0t4 = replace_exit_jump(99, _sv0t3);
  int r2 = _sv0t4;
  int _sv0t5 = (0 - 1);
  if ((r2 != _sv0t5)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_back_edge(void) {
  int _sv0t0 = compute_back_edge(0, 20, 5);
  int r1 = _sv0t0;
  int _sv0t1 = (0 - 25);
  if ((r1 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = compute_back_edge(10, 30, 5);
  int r2 = _sv0t2;
  int _sv0t3 = (0 - 25);
  if ((r2 != _sv0t3)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_body_offset(void) {
  int _sv0t0 = loop_body_offset(3, 5);
  if ((_sv0t0 != 8)) {
    return 1;
  } else {
  }
  int _sv0t1 = loop_body_offset(10, 5);
  if ((_sv0t1 != 15)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_sentinels();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_patch_continue();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_replace_exit();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_back_edge();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_body_offset();
  int r5 = _sv0t8;
  if ((r5 != 0)) {
    int _sv0t9 = (500 + r5);
    return _sv0t9;
  } else {
  }
  return 0;
}

