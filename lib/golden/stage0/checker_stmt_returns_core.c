#include "sv0_runtime.h"

static int stmt_returns(int stmt_tag, int inner_expr_tag);
static int last_stmt_returns(int count, int last_stmt_tag, int last_inner_tag);
static int classify_assign_lhs(int expr_tag, int path_len);
static int assign_lhs_is_var(int class);
static int assign_lhs_is_field(int class);
static int assign_lhs_is_bad(int class);
static int test_stmt_returns(void);
static int test_last_stmt_returns(void);
static int test_classify(void);

static int stmt_returns(int stmt_tag, int inner_expr_tag) {
  if ((stmt_tag == 0)) {
    int _sv0t0 = (inner_expr_tag == 24);
    return _sv0t0;
  } else {
  }
  if ((stmt_tag == 1)) {
    int _sv0t1 = (inner_expr_tag == 24);
    return _sv0t1;
  } else {
  }
  return 0;
}

static int last_stmt_returns(int count, int last_stmt_tag, int last_inner_tag) {
  if ((count == 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = stmt_returns(last_stmt_tag, last_inner_tag);
  return _sv0t0;
}

static int classify_assign_lhs(int expr_tag, int path_len) {
  if ((expr_tag == 0)) {
    if ((path_len == 1)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((expr_tag == 3)) {
    return 2;
  } else {
  }
  return 0;
}

static int assign_lhs_is_var(int class) {
  int _sv0t0 = (class == 1);
  return _sv0t0;
}

static int assign_lhs_is_field(int class) {
  int _sv0t0 = (class == 2);
  return _sv0t0;
}

static int assign_lhs_is_bad(int class) {
  int _sv0t0 = (class == 0);
  return _sv0t0;
}

static int test_stmt_returns(void) {
  int _sv0t0 = stmt_returns(0, 24);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = stmt_returns(1, 24);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = stmt_returns(0, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = stmt_returns(1, 5);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = stmt_returns(2, 24);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = stmt_returns(3, 24);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_last_stmt_returns(void) {
  int _sv0t0 = last_stmt_returns(0, 0, 24);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = last_stmt_returns(1, 0, 24);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = last_stmt_returns(3, 1, 24);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = last_stmt_returns(2, 0, 5);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_classify(void) {
  int _sv0t0 = classify_assign_lhs(0, 1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = classify_assign_lhs(0, 2);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = classify_assign_lhs(3, 1);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = classify_assign_lhs(5, 1);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = assign_lhs_is_var(1);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = assign_lhs_is_field(2);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = assign_lhs_is_bad(0);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_stmt_returns();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_last_stmt_returns();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_classify();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  return 0;
}

