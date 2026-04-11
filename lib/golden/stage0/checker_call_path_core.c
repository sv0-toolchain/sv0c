#include "sv0_runtime.h"

static int call_path_tag(int seg_count, int first_seg_id);
static int contract_builtin_gate(int path_tag, int in_contract);
static int builtin_arity_check(int path_tag, int arg_count);
static int fn_arity_check(int arg_count, int param_count);

static int call_path_tag(int seg_count, int first_seg_id) {
  if ((seg_count == 1)) {
    if ((first_seg_id == 0)) {
      return 0;
    } else {
    }
    if ((first_seg_id == 1)) {
      return 1;
    } else {
    }
    if ((first_seg_id == 2)) {
      return 2;
    } else {
    }
    if ((first_seg_id == 3)) {
      return 3;
    } else {
    }
    if ((first_seg_id == 4)) {
      return 4;
    } else {
    }
    return 5;
  } else {
  }
  if ((seg_count >= 2)) {
    return 6;
  } else {
  }
  return 255;
}

static int contract_builtin_gate(int path_tag, int in_contract) {
  if ((path_tag == 0)) {
    if ((in_contract == 1)) {
      return 0;
    } else {
    }
    return 521;
  } else {
  }
  if ((path_tag == 1)) {
    if ((in_contract == 1)) {
      return 0;
    } else {
    }
    return 523;
  } else {
  }
  if ((path_tag == 2)) {
    if ((in_contract == 1)) {
      return 0;
    } else {
    }
    return 523;
  } else {
  }
  if ((path_tag == 3)) {
    if ((in_contract == 1)) {
      return 0;
    } else {
    }
    return 524;
  } else {
  }
  return 0;
}

static int builtin_arity_check(int path_tag, int arg_count) {
  if ((path_tag == 0)) {
    if ((arg_count == 1)) {
      return 0;
    } else {
    }
    return 521;
  } else {
  }
  if ((path_tag == 1)) {
    if ((arg_count == 2)) {
      return 0;
    } else {
    }
    return 523;
  } else {
  }
  if ((path_tag == 2)) {
    if ((arg_count == 2)) {
      return 0;
    } else {
    }
    return 523;
  } else {
  }
  if ((path_tag == 3)) {
    if ((arg_count == 2)) {
      return 0;
    } else {
    }
    return 524;
  } else {
  }
  if ((path_tag == 4)) {
    if ((arg_count == 1)) {
      return 0;
    } else {
    }
    return 444;
  } else {
  }
  return 0;
}

static int fn_arity_check(int arg_count, int param_count) {
  if ((arg_count == param_count)) {
    return 0;
  } else {
  }
  return 413;
}

int main(void) {
  int _sv0t0 = call_path_tag(1, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = call_path_tag(1, 4);
  if ((_sv0t1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t2 = call_path_tag(1, 99);
  if ((_sv0t2 != 5)) {
    return 1;
  } else {
  }
  int _sv0t3 = call_path_tag(2, 0);
  if ((_sv0t3 != 6)) {
    return 1;
  } else {
  }
  int _sv0t4 = contract_builtin_gate(0, 1);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = contract_builtin_gate(0, 0);
  if ((_sv0t5 != 521)) {
    return 1;
  } else {
  }
  int _sv0t6 = contract_builtin_gate(1, 0);
  if ((_sv0t6 != 523)) {
    return 1;
  } else {
  }
  int _sv0t7 = contract_builtin_gate(3, 0);
  if ((_sv0t7 != 524)) {
    return 1;
  } else {
  }
  int _sv0t8 = contract_builtin_gate(5, 0);
  if ((_sv0t8 != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = builtin_arity_check(0, 1);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = builtin_arity_check(0, 2);
  if ((_sv0t10 != 521)) {
    return 1;
  } else {
  }
  int _sv0t11 = builtin_arity_check(1, 2);
  if ((_sv0t11 != 0)) {
    return 1;
  } else {
  }
  int _sv0t12 = builtin_arity_check(4, 1);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  int _sv0t13 = builtin_arity_check(4, 0);
  if ((_sv0t13 != 444)) {
    return 1;
  } else {
  }
  int _sv0t14 = fn_arity_check(2, 2);
  if ((_sv0t14 != 0)) {
    return 1;
  } else {
  }
  int _sv0t15 = fn_arity_check(1, 2);
  if ((_sv0t15 != 413)) {
    return 1;
  } else {
  }
  return 0;
}

