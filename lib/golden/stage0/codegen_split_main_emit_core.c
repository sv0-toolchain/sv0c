#include "sv0_runtime.h"

static int int_lit_len(int value);
static int split_main_others_count(int block_count, int has_main);
static int split_main_found(int block_count, int has_main);
static int emit_section_count(int block_count, int has_main, int has_typedefs);
static int emit_forward_decl_count(int block_count, int has_main);
static int emit_static_defn_count(int block_count, int has_main);
static int emit_main_section(int has_main);

static int int_lit_len(int value) {
  if ((value == 0)) {
    return 1;
  } else {
  }
  int n = 0;
  int v = value;
  if ((v < 0)) {
    n = 1;
    v = (0 - v);
  } else {
  }
  int _sv0t0 = 10;
  int _sv0t1 = 0;
  while ((_sv0t1 < _sv0t0)) {
    {
      int _i = _sv0t1;
      _sv0t1 = (_sv0t1 + 1);
      if ((v > 0)) {
        n = (n + 1);
        v = (v / 10);
      } else {
      }
    }
  }
  return n;
}

static int split_main_others_count(int block_count, int has_main) {
  if ((has_main == 1)) {
    int _sv0t0 = (block_count - 1);
    return _sv0t0;
  } else {
  }
  return block_count;
}

static int split_main_found(int block_count, int has_main) {
  if ((block_count > 0)) {
    if ((has_main == 1)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int emit_section_count(int block_count, int has_main, int has_typedefs) {
  if ((block_count == 0)) {
    return 1;
  } else {
  }
  int td = 0;
  if ((has_typedefs == 1)) {
    td = 1;
  } else {
  }
  int _sv0t0 = (5 + td);
  return _sv0t0;
}

static int emit_forward_decl_count(int block_count, int has_main) {
  int _sv0t0 = split_main_others_count(block_count, has_main);
  return _sv0t0;
}

static int emit_static_defn_count(int block_count, int has_main) {
  int _sv0t0 = split_main_others_count(block_count, has_main);
  return _sv0t0;
}

static int emit_main_section(int has_main) {
  return 1;
}

int main(void) {
  int _sv0t0 = int_lit_len(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = int_lit_len(5);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = int_lit_len(42);
  if ((_sv0t2 != 2)) {
    return 1;
  } else {
  }
  int _sv0t3 = int_lit_len(100);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = int_lit_len(9999);
  if ((_sv0t4 != 4)) {
    return 1;
  } else {
  }
  int _sv0t5 = (0 - 1);
  int _sv0t6 = int_lit_len(_sv0t5);
  if ((_sv0t6 != 2)) {
    return 1;
  } else {
  }
  int _sv0t7 = (0 - 42);
  int _sv0t8 = int_lit_len(_sv0t7);
  if ((_sv0t8 != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = split_main_others_count(0, 0);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = split_main_found(0, 0);
  if ((_sv0t10 != 0)) {
    return 1;
  } else {
  }
  int _sv0t11 = split_main_others_count(3, 0);
  if ((_sv0t11 != 3)) {
    return 1;
  } else {
  }
  int _sv0t12 = split_main_found(3, 0);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  int _sv0t13 = split_main_others_count(3, 1);
  if ((_sv0t13 != 2)) {
    return 1;
  } else {
  }
  int _sv0t14 = split_main_found(3, 1);
  if ((_sv0t14 != 1)) {
    return 1;
  } else {
  }
  int _sv0t15 = split_main_others_count(1, 1);
  if ((_sv0t15 != 0)) {
    return 1;
  } else {
  }
  int _sv0t16 = split_main_found(1, 1);
  if ((_sv0t16 != 1)) {
    return 1;
  } else {
  }
  int _sv0t17 = emit_section_count(0, 0, 0);
  if ((_sv0t17 != 1)) {
    return 1;
  } else {
  }
  int _sv0t18 = emit_section_count(3, 1, 0);
  if ((_sv0t18 != 5)) {
    return 1;
  } else {
  }
  int _sv0t19 = emit_section_count(3, 1, 1);
  if ((_sv0t19 != 6)) {
    return 1;
  } else {
  }
  int _sv0t20 = emit_section_count(1, 0, 0);
  if ((_sv0t20 != 5)) {
    return 1;
  } else {
  }
  int _sv0t21 = emit_forward_decl_count(3, 1);
  if ((_sv0t21 != 2)) {
    return 1;
  } else {
  }
  int _sv0t22 = emit_forward_decl_count(3, 0);
  if ((_sv0t22 != 3)) {
    return 1;
  } else {
  }
  int _sv0t23 = emit_static_defn_count(4, 1);
  if ((_sv0t23 != 3)) {
    return 1;
  } else {
  }
  int _sv0t24 = emit_static_defn_count(4, 0);
  if ((_sv0t24 != 4)) {
    return 1;
  } else {
  }
  int _sv0t25 = emit_main_section(1);
  if ((_sv0t25 != 1)) {
    return 1;
  } else {
  }
  int _sv0t26 = emit_main_section(0);
  if ((_sv0t26 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

