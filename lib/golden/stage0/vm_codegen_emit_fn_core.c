#include "sv0_runtime.h"

static int struct_layout_count(int item_count, int struct_count);
static int enum_layout_count(int item_count, int enum_count);
static int pool_add_index(int pool_size, int is_new);
static int emit_fn_arity(int param_count, int total_param_width);
static int func_order_count(int block_count, int has_main);
static int main_func_index(int block_count, int has_main);
static int encode_file_section_count(int string_count, int func_count);

static int struct_layout_count(int item_count, int struct_count) {
  return struct_count;
}

static int enum_layout_count(int item_count, int enum_count) {
  return enum_count;
}

static int pool_add_index(int pool_size, int is_new) {
  if ((is_new == 1)) {
    return pool_size;
  } else {
  }
  int _sv0t0 = (pool_size - 1);
  return _sv0t0;
}

static int emit_fn_arity(int param_count, int total_param_width) {
  return total_param_width;
}

static int func_order_count(int block_count, int has_main) {
  return block_count;
}

static int main_func_index(int block_count, int has_main) {
  if ((has_main == 1)) {
    int _sv0t0 = (block_count - 1);
    return _sv0t0;
  } else {
  }
  return 255;
}

static int encode_file_section_count(int string_count, int func_count) {
  int _sv0t0 = (1 + string_count);
  int _sv0t1 = (_sv0t0 + func_count);
  return _sv0t1;
}

int main(void) {
  int _sv0t0 = struct_layout_count(5, 2);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = struct_layout_count(3, 0);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = enum_layout_count(5, 1);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = pool_add_index(0, 1);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = pool_add_index(3, 1);
  if ((_sv0t4 != 3)) {
    return 1;
  } else {
  }
  int _sv0t5 = pool_add_index(3, 0);
  if ((_sv0t5 != 2)) {
    return 1;
  } else {
  }
  int _sv0t6 = emit_fn_arity(2, 4);
  if ((_sv0t6 != 4)) {
    return 1;
  } else {
  }
  int _sv0t7 = emit_fn_arity(0, 0);
  if ((_sv0t7 != 0)) {
    return 1;
  } else {
  }
  int _sv0t8 = func_order_count(5, 1);
  if ((_sv0t8 != 5)) {
    return 1;
  } else {
  }
  int _sv0t9 = func_order_count(3, 0);
  if ((_sv0t9 != 3)) {
    return 1;
  } else {
  }
  int _sv0t10 = main_func_index(5, 1);
  if ((_sv0t10 != 4)) {
    return 1;
  } else {
  }
  int _sv0t11 = main_func_index(3, 0);
  if ((_sv0t11 != 255)) {
    return 1;
  } else {
  }
  int _sv0t12 = encode_file_section_count(2, 3);
  if ((_sv0t12 != 6)) {
    return 1;
  } else {
  }
  int _sv0t13 = encode_file_section_count(0, 1);
  if ((_sv0t13 != 2)) {
    return 1;
  } else {
  }
  return 0;
}

