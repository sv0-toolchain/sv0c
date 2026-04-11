#include "sv0_runtime.h"

static int synth_result_class(int expr_tag);
static int loop_ctx_check(int loop_depth, int is_break);
static int match_scrut_ok(int scrut_type_class);
static int unop_result_class(int unop_tag, int in_contract);

static int synth_result_class(int expr_tag) {
  if ((expr_tag == 0)) {
    return 6;
  } else {
  }
  if ((expr_tag == 1)) {
    return 6;
  } else {
  }
  if ((expr_tag == 2)) {
    return 6;
  } else {
  }
  if ((expr_tag == 3)) {
    return 6;
  } else {
  }
  if ((expr_tag == 4)) {
    return 6;
  } else {
  }
  if ((expr_tag == 5)) {
    return 6;
  } else {
  }
  if ((expr_tag == 6)) {
    return 6;
  } else {
  }
  if ((expr_tag == 7)) {
    return 6;
  } else {
  }
  if ((expr_tag == 8)) {
    return 3;
  } else {
  }
  if ((expr_tag == 9)) {
    return 6;
  } else {
  }
  if ((expr_tag == 10)) {
    return 6;
  } else {
  }
  if ((expr_tag == 11)) {
    return 6;
  } else {
  }
  if ((expr_tag == 12)) {
    return 2;
  } else {
  }
  if ((expr_tag == 13)) {
    return 2;
  } else {
  }
  if ((expr_tag == 14)) {
    return 2;
  } else {
  }
  if ((expr_tag == 15)) {
    return 2;
  } else {
  }
  if ((expr_tag == 16)) {
    return 2;
  } else {
  }
  if ((expr_tag == 17)) {
    return 2;
  } else {
  }
  return 255;
}

static int loop_ctx_check(int loop_depth, int is_break) {
  if ((loop_depth > 0)) {
    return 0;
  } else {
  }
  if ((is_break == 1)) {
    return 411;
  } else {
  }
  return 412;
}

static int match_scrut_ok(int scrut_type_class) {
  if ((scrut_type_class == 0)) {
    return 1;
  } else {
  }
  if ((scrut_type_class == 1)) {
    return 1;
  } else {
  }
  if ((scrut_type_class == 4)) {
    return 1;
  } else {
  }
  return 0;
}

static int unop_result_class(int unop_tag, int in_contract) {
  if ((unop_tag == 0)) {
    return 0;
  } else {
  }
  if ((unop_tag == 1)) {
    return 1;
  } else {
  }
  if ((unop_tag == 2)) {
    if ((in_contract == 1)) {
      return 6;
    } else {
    }
    return 255;
  } else {
  }
  if ((unop_tag == 3)) {
    if ((in_contract == 1)) {
      return 6;
    } else {
    }
    return 255;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = synth_result_class(0);
  if ((_sv0t0 != 6)) {
    return 1;
  } else {
  }
  int _sv0t1 = synth_result_class(8);
  if ((_sv0t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t2 = synth_result_class(12);
  if ((_sv0t2 != 2)) {
    return 1;
  } else {
  }
  int _sv0t3 = synth_result_class(15);
  if ((_sv0t3 != 2)) {
    return 1;
  } else {
  }
  int _sv0t4 = synth_result_class(17);
  if ((_sv0t4 != 2)) {
    return 1;
  } else {
  }
  int _sv0t5 = synth_result_class(99);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = loop_ctx_check(1, 1);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  int _sv0t7 = loop_ctx_check(0, 1);
  if ((_sv0t7 != 411)) {
    return 1;
  } else {
  }
  int _sv0t8 = loop_ctx_check(0, 0);
  if ((_sv0t8 != 412)) {
    return 1;
  } else {
  }
  int _sv0t9 = match_scrut_ok(0);
  if ((_sv0t9 != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = match_scrut_ok(1);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = match_scrut_ok(4);
  if ((_sv0t11 != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = match_scrut_ok(3);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  int _sv0t13 = unop_result_class(0, 0);
  if ((_sv0t13 != 0)) {
    return 1;
  } else {
  }
  int _sv0t14 = unop_result_class(1, 0);
  if ((_sv0t14 != 1)) {
    return 1;
  } else {
  }
  int _sv0t15 = unop_result_class(2, 1);
  if ((_sv0t15 != 6)) {
    return 1;
  } else {
  }
  int _sv0t16 = unop_result_class(2, 0);
  if ((_sv0t16 != 255)) {
    return 1;
  } else {
  }
  int _sv0t17 = unop_result_class(3, 1);
  if ((_sv0t17 != 6)) {
    return 1;
  } else {
  }
  return 0;
}

