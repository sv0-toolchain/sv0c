#include "sv0_runtime.h"

static int update_depth(int cur_char, int next_char, int depth);
static int chars_consumed(int cur_char, int next_char);
static int is_comment_done(int depth);
static int simulate_comment(int c0, int n0, int c1, int n1, int c2, int n2, int c3, int n3);

static int update_depth(int cur_char, int next_char, int depth) {
  int _sv0t0 = (cur_char == 47);
  int _sv0t1 = (next_char == 42);
  if ((_sv0t0 && _sv0t1)) {
    int _sv0t2 = (depth + 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = (cur_char == 42);
  int _sv0t4 = (next_char == 47);
  if ((_sv0t3 && _sv0t4)) {
    int _sv0t5 = (depth - 1);
    return _sv0t5;
  } else {
  }
  return depth;
}

static int chars_consumed(int cur_char, int next_char) {
  int _sv0t0 = (cur_char == 47);
  int _sv0t1 = (next_char == 42);
  if ((_sv0t0 && _sv0t1)) {
    return 2;
  } else {
  }
  int _sv0t2 = (cur_char == 42);
  int _sv0t3 = (next_char == 47);
  if ((_sv0t2 && _sv0t3)) {
    return 2;
  } else {
  }
  return 1;
}

static int is_comment_done(int depth) {
  int _sv0t0 = (depth == 0);
  return _sv0t0;
}

static int simulate_comment(int c0, int n0, int c1, int n1, int c2, int n2, int c3, int n3) {
  int depth = 1;
  int _sv0t0 = update_depth(c0, n0, depth);
  depth = _sv0t0;
  int _sv0t1 = is_comment_done(depth);
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = update_depth(c1, n1, depth);
  depth = _sv0t2;
  int _sv0t3 = is_comment_done(depth);
  if (_sv0t3) {
    return 2;
  } else {
  }
  int _sv0t4 = update_depth(c2, n2, depth);
  depth = _sv0t4;
  int _sv0t5 = is_comment_done(depth);
  if (_sv0t5) {
    return 3;
  } else {
  }
  int _sv0t6 = update_depth(c3, n3, depth);
  depth = _sv0t6;
  int _sv0t7 = is_comment_done(depth);
  if (_sv0t7) {
    return 4;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = update_depth(47, 42, 1);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = update_depth(42, 47, 2);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = update_depth(42, 47, 1);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = update_depth(65, 66, 1);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = chars_consumed(47, 42);
  if ((_sv0t4 != 2)) {
    return 5;
  } else {
  }
  int _sv0t5 = chars_consumed(42, 47);
  if ((_sv0t5 != 2)) {
    return 6;
  } else {
  }
  int _sv0t6 = chars_consumed(65, 66);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_comment_done(0);
  if ((!_sv0t7)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_comment_done(1);
  if (_sv0t8) {
    return 9;
  } else {
  }
  int _sv0t9 = simulate_comment(65, 66, 42, 47, 0, 0, 0, 0);
  int r1 = _sv0t9;
  if ((r1 != 2)) {
    return 10;
  } else {
  }
  int _sv0t10 = simulate_comment(47, 42, 65, 66, 42, 47, 42, 47);
  int r2 = _sv0t10;
  if ((r2 != 4)) {
    return 11;
  } else {
  }
  return 0;
}

