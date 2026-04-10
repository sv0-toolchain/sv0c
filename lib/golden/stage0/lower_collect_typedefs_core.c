#include "sv0_runtime.h"

static int collect_typedefs_count(int struct_count, int enum_count);

static int collect_typedefs_count(int struct_count, int enum_count) {
  if ((struct_count < 0)) {
    return 255;
  } else {
  }
  if ((enum_count < 0)) {
    return 255;
  } else {
  }
  int _sv0t0 = (struct_count + enum_count);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = collect_typedefs_count(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = collect_typedefs_count(2, 3);
  if ((_sv0t1 != 5)) {
    return 1;
  } else {
  }
  int _sv0t2 = collect_typedefs_count(1, 0);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = collect_typedefs_count(0, 4);
  if ((_sv0t3 != 4)) {
    return 1;
  } else {
  }
  int _sv0t4 = (-1);
  int _sv0t5 = collect_typedefs_count(_sv0t4, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = (-2);
  int _sv0t7 = collect_typedefs_count(0, _sv0t6);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

