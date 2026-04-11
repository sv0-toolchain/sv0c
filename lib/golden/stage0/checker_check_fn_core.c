#include "sv0_runtime.h"

static int check_fn_env_size(int param_count, int has_ret_type);
static int contract_param_count(int fn_param_count);
static int block_tail_rule(int has_tail, int last_returns, int ret_is_unit);
static int stmt_returns(int stmt_tag, int inner_is_return);
static int contract_extends_env(int kind, int ret_is_unit);
static int variant_shape_lookup(int enum_found, int variant_found);

static int check_fn_env_size(int param_count, int has_ret_type) {
  if ((has_ret_type == 0)) {
    return 255;
  } else {
  }
  return param_count;
}

static int contract_param_count(int fn_param_count) {
  return fn_param_count;
}

static int block_tail_rule(int has_tail, int last_returns, int ret_is_unit) {
  if ((has_tail == 1)) {
    return 0;
  } else {
  }
  if ((last_returns == 1)) {
    return 1;
  } else {
  }
  if ((ret_is_unit == 1)) {
    return 2;
  } else {
  }
  return 3;
}

static int stmt_returns(int stmt_tag, int inner_is_return) {
  if ((stmt_tag == 1)) {
    if ((inner_is_return == 1)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((stmt_tag == 2)) {
    if ((inner_is_return == 1)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int contract_extends_env(int kind, int ret_is_unit) {
  if ((kind == 0)) {
    return 0;
  } else {
  }
  if ((kind == 1)) {
    if ((ret_is_unit == 1)) {
      return 0;
    } else {
    }
    return 1;
  } else {
  }
  return 0;
}

static int variant_shape_lookup(int enum_found, int variant_found) {
  if ((enum_found == 0)) {
    return 422;
  } else {
  }
  if ((variant_found == 0)) {
    return 421;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = check_fn_env_size(3, 1);
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = check_fn_env_size(0, 1);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = check_fn_env_size(2, 0);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = contract_param_count(4);
  if ((_sv0t3 != 4)) {
    return 1;
  } else {
  }
  int _sv0t4 = block_tail_rule(1, 0, 0);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = block_tail_rule(0, 1, 0);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = block_tail_rule(0, 0, 1);
  if ((_sv0t6 != 2)) {
    return 1;
  } else {
  }
  int _sv0t7 = block_tail_rule(0, 0, 0);
  if ((_sv0t7 != 3)) {
    return 1;
  } else {
  }
  int _sv0t8 = stmt_returns(1, 1);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = stmt_returns(1, 0);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = stmt_returns(0, 1);
  if ((_sv0t10 != 0)) {
    return 1;
  } else {
  }
  int _sv0t11 = contract_extends_env(0, 0);
  if ((_sv0t11 != 0)) {
    return 1;
  } else {
  }
  int _sv0t12 = contract_extends_env(1, 0);
  if ((_sv0t12 != 1)) {
    return 1;
  } else {
  }
  int _sv0t13 = contract_extends_env(1, 1);
  if ((_sv0t13 != 0)) {
    return 1;
  } else {
  }
  int _sv0t14 = variant_shape_lookup(1, 1);
  if ((_sv0t14 != 0)) {
    return 1;
  } else {
  }
  int _sv0t15 = variant_shape_lookup(0, 0);
  if ((_sv0t15 != 422)) {
    return 1;
  } else {
  }
  int _sv0t16 = variant_shape_lookup(1, 0);
  if ((_sv0t16 != 421)) {
    return 1;
  } else {
  }
  return 0;
}

