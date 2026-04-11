#include "sv0_runtime.h"

static int pat_local_count(int pat_tag, int sub_count);
static int binding_is_mutable(int has_mut);
static int env_after_bind(int env_size, int bindings);
static int let_pattern_binds(int pat_tag);
static int match_arm_binds(int arm_pat_tag, int sub_count);
static int total_arm_bindings(int a0, int a1, int a2);

static int pat_local_count(int pat_tag, int sub_count) {
  if ((pat_tag == 0)) {
    return 0;
  } else {
  }
  if ((pat_tag == 1)) {
    return 1;
  } else {
  }
  if ((pat_tag == 2)) {
    return 0;
  } else {
  }
  if ((pat_tag == 3)) {
    return sub_count;
  } else {
  }
  if ((pat_tag == 4)) {
    return sub_count;
  } else {
  }
  if ((pat_tag == 5)) {
    return sub_count;
  } else {
  }
  if ((pat_tag == 6)) {
    return sub_count;
  } else {
  }
  return 0;
}

static int binding_is_mutable(int has_mut) {
  return has_mut;
}

static int env_after_bind(int env_size, int bindings) {
  int _sv0t0 = (env_size + bindings);
  return _sv0t0;
}

static int let_pattern_binds(int pat_tag) {
  if ((pat_tag == 1)) {
    return 1;
  } else {
  }
  if ((pat_tag == 3)) {
    return 0;
  } else {
  }
  return 0;
}

static int match_arm_binds(int arm_pat_tag, int sub_count) {
  int _sv0t0 = pat_local_count(arm_pat_tag, sub_count);
  return _sv0t0;
}

static int total_arm_bindings(int a0, int a1, int a2) {
  int max_val = a0;
  if ((a1 > max_val)) {
    max_val = a1;
  } else {
  }
  if ((a2 > max_val)) {
    max_val = a2;
  } else {
  }
  return max_val;
}

int main(void) {
  int _sv0t0 = pat_local_count(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = pat_local_count(1, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = pat_local_count(3, 2);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = pat_local_count(4, 3);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = binding_is_mutable(1);
  if ((!_sv0t4)) {
    return 5;
  } else {
  }
  int _sv0t5 = binding_is_mutable(0);
  if (_sv0t5) {
    return 6;
  } else {
  }
  int _sv0t6 = env_after_bind(5, 2);
  if ((_sv0t6 != 7)) {
    return 7;
  } else {
  }
  int _sv0t7 = env_after_bind(0, 0);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = let_pattern_binds(1);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = let_pattern_binds(3);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = match_arm_binds(1, 0);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = match_arm_binds(4, 2);
  if ((_sv0t11 != 2)) {
    return 12;
  } else {
  }
  int _sv0t12 = total_arm_bindings(1, 3, 2);
  if ((_sv0t12 != 3)) {
    return 13;
  } else {
  }
  int _sv0t13 = total_arm_bindings(5, 5, 5);
  if ((_sv0t13 != 5)) {
    return 14;
  } else {
  }
  return 0;
}

