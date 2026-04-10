#include "sv0_runtime.h"

static int lower_block_count(int item_count, int fn_count);
static int lower_typedef_count(int struct_count, int enum_count);
static int lower_init_table_count(int struct_count, int enum_count);

static int lower_block_count(int item_count, int fn_count) {
  if ((item_count < 0)) {
    return 255;
  } else {
  }
  if ((fn_count < 0)) {
    return 255;
  } else {
  }
  if ((fn_count > item_count)) {
    return 255;
  } else {
  }
  return fn_count;
}

static int lower_typedef_count(int struct_count, int enum_count) {
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

static int lower_init_table_count(int struct_count, int enum_count) {
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
  int _sv0t0 = lower_block_count(5, 3);
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_block_count(0, 0);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_block_count(2, 3);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = (-1);
  int _sv0t4 = lower_block_count(_sv0t3, 0);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  int _sv0t5 = lower_typedef_count(2, 1);
  if ((_sv0t5 != 3)) {
    return 1;
  } else {
  }
  int _sv0t6 = lower_typedef_count(0, 0);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  int _sv0t7 = (-1);
  int _sv0t8 = lower_typedef_count(_sv0t7, 0);
  if ((_sv0t8 != 255)) {
    return 1;
  } else {
  }
  int _sv0t9 = lower_init_table_count(1, 2);
  if ((_sv0t9 != 3)) {
    return 1;
  } else {
  }
  int _sv0t10 = lower_init_table_count(0, 0);
  if ((_sv0t10 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

