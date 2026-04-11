#include "sv0_runtime.h"

static int is_link_directive(int item_kind);
static int strip_count(int k0, int k1, int k2, int k3);
static int is_top_level_item(int item_kind);
static int collect_tops_count(int k0, int k1, int k2, int k3);
static int path_seg_needs_rewrite(int seg, int t0, int t1, int t2);
static int rewrite_seg(int seg, int mod_id, int t0, int t1, int t2);

static int is_link_directive(int item_kind) {
  int _sv0t0 = (item_kind == 7);
  return _sv0t0;
}

static int strip_count(int k0, int k1, int k2, int k3) {
  int count = 0;
  int _sv0t0 = is_link_directive(k0);
  if ((!_sv0t0)) {
    count = (count + 1);
  } else {
  }
  int _sv0t1 = is_link_directive(k1);
  if ((!_sv0t1)) {
    count = (count + 1);
  } else {
  }
  int _sv0t2 = is_link_directive(k2);
  if ((!_sv0t2)) {
    count = (count + 1);
  } else {
  }
  int _sv0t3 = is_link_directive(k3);
  if ((!_sv0t3)) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int is_top_level_item(int item_kind) {
  if ((item_kind == 0)) {
    return 1;
  } else {
  }
  if ((item_kind == 3)) {
    return 1;
  } else {
  }
  if ((item_kind == 4)) {
    return 1;
  } else {
  }
  if ((item_kind == 5)) {
    return 1;
  } else {
  }
  return 0;
}

static int collect_tops_count(int k0, int k1, int k2, int k3) {
  int count = 0;
  int _sv0t0 = is_top_level_item(k0);
  if (_sv0t0) {
    count = (count + 1);
  } else {
  }
  int _sv0t1 = is_top_level_item(k1);
  if (_sv0t1) {
    count = (count + 1);
  } else {
  }
  int _sv0t2 = is_top_level_item(k2);
  if (_sv0t2) {
    count = (count + 1);
  } else {
  }
  int _sv0t3 = is_top_level_item(k3);
  if (_sv0t3) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int path_seg_needs_rewrite(int seg, int t0, int t1, int t2) {
  if ((seg == t0)) {
    return 1;
  } else {
  }
  if ((seg == t1)) {
    return 1;
  } else {
  }
  if ((seg == t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int rewrite_seg(int seg, int mod_id, int t0, int t1, int t2) {
  int _sv0t0 = path_seg_needs_rewrite(seg, t0, t1, t2);
  if (_sv0t0) {
    int _sv0t1 = (mod_id * 1000);
    int _sv0t2 = (_sv0t1 + seg);
    return _sv0t2;
  } else {
  }
  return seg;
}

int main(void) {
  int _sv0t0 = is_link_directive(7);
  if ((!_sv0t0)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_link_directive(0);
  if (_sv0t1) {
    return 2;
  } else {
  }
  int _sv0t2 = is_link_directive(3);
  if (_sv0t2) {
    return 3;
  } else {
  }
  int _sv0t3 = strip_count(0, 7, 3, 7);
  if ((_sv0t3 != 2)) {
    return 4;
  } else {
  }
  int _sv0t4 = strip_count(0, 0, 0, 0);
  if ((_sv0t4 != 4)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_top_level_item(0);
  if ((!_sv0t5)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_top_level_item(3);
  if ((!_sv0t6)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_top_level_item(7);
  if (_sv0t7) {
    return 8;
  } else {
  }
  int _sv0t8 = collect_tops_count(0, 3, 7, 4);
  if ((_sv0t8 != 3)) {
    return 9;
  } else {
  }
  int _sv0t9 = path_seg_needs_rewrite(10, 10, 20, 30);
  if ((!_sv0t9)) {
    return 10;
  } else {
  }
  int _sv0t10 = path_seg_needs_rewrite(15, 10, 20, 30);
  if (_sv0t10) {
    return 11;
  } else {
  }
  int _sv0t11 = rewrite_seg(10, 2, 10, 20, 30);
  if ((_sv0t11 != 2010)) {
    return 12;
  } else {
  }
  int _sv0t12 = rewrite_seg(15, 2, 10, 20, 30);
  if ((_sv0t12 != 15)) {
    return 13;
  } else {
  }
  return 0;
}

