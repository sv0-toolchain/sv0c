#include "sv0_runtime.h"

static int build_struct_order_len(int item_count, int struct_count);

static int build_struct_order_len(int item_count, int struct_count) {
  if ((item_count < 0)) {
    return 255;
  } else {
  }
  if ((struct_count < 0)) {
    return 255;
  } else {
  }
  if ((struct_count > item_count)) {
    return 255;
  } else {
  }
  return struct_count;
}

int main(void) {
  int _sv0t0 = build_struct_order_len(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = build_struct_order_len(5, 2);
  if ((_sv0t1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t2 = build_struct_order_len(3, 3);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = build_struct_order_len(1, 0);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = (-1);
  int _sv0t5 = build_struct_order_len(_sv0t4, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = build_struct_order_len(2, 3);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

