#include "sv0_runtime.h"

static int pat_binds_count(int pat_kind, int child_count);

static int pat_binds_count(int pat_kind, int child_count) {
  if ((pat_kind == 0)) {
    return 0;
  } else {
  }
  if ((pat_kind == 1)) {
    return 1;
  } else {
  }
  if ((pat_kind == 2)) {
    return 0;
  } else {
  }
  if ((pat_kind == 3)) {
    return child_count;
  } else {
  }
  if ((pat_kind == 4)) {
    return child_count;
  } else {
  }
  if ((pat_kind == 5)) {
    return child_count;
  } else {
  }
  if ((pat_kind == 6)) {
    return 255;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = pat_binds_count(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = pat_binds_count(1, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = pat_binds_count(2, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = pat_binds_count(3, 3);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = pat_binds_count(4, 2);
  if ((_sv0t4 != 2)) {
    return 5;
  } else {
  }
  int _sv0t5 = pat_binds_count(5, 4);
  if ((_sv0t5 != 4)) {
    return 6;
  } else {
  }
  int _sv0t6 = pat_binds_count(6, 0);
  if ((_sv0t6 != 255)) {
    return 7;
  } else {
  }
  int _sv0t7 = pat_binds_count(3, 0);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = pat_binds_count(5, 1);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = pat_binds_count(99, 5);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  return 0;
}

