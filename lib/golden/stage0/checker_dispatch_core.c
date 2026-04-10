#include "sv0_runtime.h"

static int env_extend(int env_size);
static int env_lookup_hit(int env_size, int target_index);
static int with_loop_depth(int depth);
static int after_loop_depth(int depth);
static int break_ok(int loop_depth);
static int canon_ty_import(int has_alias);
static int expr_check_tag(int expr_tag);
static int stmt_env_delta(int stmt_tag);
static int item_registers(int item_tag);

static int env_extend(int env_size) {
  int _sv0t0 = (env_size + 1);
  return _sv0t0;
}

static int env_lookup_hit(int env_size, int target_index) {
  if ((target_index >= 0)) {
    if ((target_index < env_size)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int with_loop_depth(int depth) {
  int _sv0t0 = (depth + 1);
  return _sv0t0;
}

static int after_loop_depth(int depth) {
  return depth;
}

static int break_ok(int loop_depth) {
  if ((loop_depth > 0)) {
    return 1;
  } else {
  }
  return 0;
}

static int canon_ty_import(int has_alias) {
  if ((has_alias == 1)) {
    return 1;
  } else {
  }
  return 0;
}

static int expr_check_tag(int expr_tag) {
  if ((expr_tag >= 0)) {
    if ((expr_tag <= 11)) {
      return expr_tag;
    } else {
    }
  } else {
  }
  return 255;
}

static int stmt_env_delta(int stmt_tag) {
  if ((stmt_tag == 0)) {
    return 1;
  } else {
  }
  return 0;
}

static int item_registers(int item_tag) {
  if ((item_tag >= 0)) {
    if ((item_tag <= 4)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = env_extend(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = env_extend(3);
  if ((_sv0t1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t2 = env_lookup_hit(3, 0);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = env_lookup_hit(3, 2);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = env_lookup_hit(3, 3);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = env_lookup_hit(0, 0);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = with_loop_depth(0);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = after_loop_depth(0);
  if ((_sv0t7 != 0)) {
    return 1;
  } else {
  }
  int _sv0t8 = break_ok(0);
  if ((_sv0t8 != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = break_ok(1);
  if ((_sv0t9 != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = break_ok(3);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = canon_ty_import(1);
  if ((_sv0t11 != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = canon_ty_import(0);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  int _sv0t13 = expr_check_tag(0);
  if ((_sv0t13 != 0)) {
    return 1;
  } else {
  }
  int _sv0t14 = expr_check_tag(11);
  if ((_sv0t14 != 11)) {
    return 1;
  } else {
  }
  int _sv0t15 = expr_check_tag(12);
  if ((_sv0t15 != 255)) {
    return 1;
  } else {
  }
  int _sv0t16 = stmt_env_delta(0);
  if ((_sv0t16 != 1)) {
    return 1;
  } else {
  }
  int _sv0t17 = stmt_env_delta(1);
  if ((_sv0t17 != 0)) {
    return 1;
  } else {
  }
  int _sv0t18 = item_registers(0);
  if ((_sv0t18 != 1)) {
    return 1;
  } else {
  }
  int _sv0t19 = item_registers(4);
  if ((_sv0t19 != 1)) {
    return 1;
  } else {
  }
  int _sv0t20 = item_registers(5);
  if ((_sv0t20 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

