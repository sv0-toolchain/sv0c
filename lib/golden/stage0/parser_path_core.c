#include "sv0_runtime.h"

static int path_segment_count(int has_seg0, int has_coloncolon_1, int has_seg1, int has_coloncolon_2, int has_seg2);
static int path_is_qualified(int seg_count);
static int path_is_simple(int seg_count);
static int generic_param_count(int has_lt, int p0, int p1, int p2, int has_gt);
static int skip_balanced_angles(int depth, int is_lt, int is_gt);
static int generic_depth_after(int initial, int lt_count, int gt_count);
static int test_path_segments(void);
static int test_path_qualified(void);
static int test_generic_count(void);
static int test_balanced_angles(void);
static int test_generic_depth(void);

static int path_segment_count(int has_seg0, int has_coloncolon_1, int has_seg1, int has_coloncolon_2, int has_seg2) {
  if ((has_seg0 == 0)) {
    return 0;
  } else {
  }
  if ((has_coloncolon_1 == 0)) {
    return 1;
  } else {
  }
  if ((has_seg1 == 0)) {
    return 1;
  } else {
  }
  if ((has_coloncolon_2 == 0)) {
    return 2;
  } else {
  }
  if ((has_seg2 == 0)) {
    return 2;
  } else {
  }
  return 3;
}

static int path_is_qualified(int seg_count) {
  int _sv0t0 = (seg_count >= 2);
  return _sv0t0;
}

static int path_is_simple(int seg_count) {
  int _sv0t0 = (seg_count == 1);
  return _sv0t0;
}

static int generic_param_count(int has_lt, int p0, int p1, int p2, int has_gt) {
  if ((has_lt == 0)) {
    return 0;
  } else {
  }
  if ((has_gt == 0)) {
    return 0;
  } else {
  }
  int count = 0;
  if (p0) {
    count = (count + 1);
  } else {
  }
  if (p1) {
    count = (count + 1);
  } else {
  }
  if (p2) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int skip_balanced_angles(int depth, int is_lt, int is_gt) {
  if (is_lt) {
    int _sv0t0 = (depth + 1);
    return _sv0t0;
  } else {
  }
  if (is_gt) {
    if ((depth > 0)) {
      int _sv0t1 = (depth - 1);
      return _sv0t1;
    } else {
    }
    return 0;
  } else {
  }
  return depth;
}

static int generic_depth_after(int initial, int lt_count, int gt_count) {
  int _sv0t0 = (initial + lt_count);
  int _sv0t1 = (_sv0t0 - gt_count);
  return _sv0t1;
}

static int test_path_segments(void) {
  int _sv0t0 = path_segment_count(0, 0, 0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = path_segment_count(1, 0, 0, 0, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = path_segment_count(1, 1, 1, 0, 0);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = path_segment_count(1, 1, 1, 1, 1);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_path_qualified(void) {
  int _sv0t0 = path_is_qualified(1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = path_is_qualified(2);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = path_is_simple(1);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = path_is_simple(2);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_generic_count(void) {
  int _sv0t0 = generic_param_count(0, 0, 0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = generic_param_count(1, 1, 0, 0, 1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = generic_param_count(1, 1, 1, 0, 1);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = generic_param_count(1, 1, 1, 1, 1);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_balanced_angles(void) {
  int _sv0t0 = skip_balanced_angles(0, 1, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = skip_balanced_angles(1, 0, 1);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = skip_balanced_angles(2, 0, 1);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = skip_balanced_angles(0, 0, 0);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_generic_depth(void) {
  int _sv0t0 = generic_depth_after(0, 1, 1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = generic_depth_after(0, 2, 1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = generic_depth_after(1, 0, 1);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_path_segments();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_path_qualified();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_generic_count();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_balanced_angles();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_generic_depth();
  int r5 = _sv0t8;
  if ((r5 != 0)) {
    int _sv0t9 = (500 + r5);
    return _sv0t9;
  } else {
  }
  return 0;
}

