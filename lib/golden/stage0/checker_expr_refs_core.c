#include "sv0_runtime.h"

static int expr_is_result_path(int expr_tag, int path_len, int first_seg);
static int expr_refs_child_count(int expr_tag);
static int expr_can_reference_result(int expr_tag);
static int contract_kind_tag(int contract_tag);
static int is_requires(int tag);
static int is_ensures(int tag);
static int ensures_needs_result_binding(int tag);
static int test_result_path(void);
static int test_child_count(void);
static int test_can_ref(void);
static int test_contract(void);

static int expr_is_result_path(int expr_tag, int path_len, int first_seg) {
  if ((expr_tag != 0)) {
    return 0;
  } else {
  }
  if ((path_len != 1)) {
    return 0;
  } else {
  }
  int _sv0t0 = (first_seg == 1);
  return _sv0t0;
}

static int expr_refs_child_count(int expr_tag) {
  if ((expr_tag == 0)) {
    return 0;
  } else {
  }
  if ((expr_tag == 1)) {
    return 0;
  } else {
  }
  if ((expr_tag == 2)) {
    return 2;
  } else {
  }
  if ((expr_tag == 3)) {
    return 1;
  } else {
  }
  if ((expr_tag == 4)) {
    return 1;
  } else {
  }
  if ((expr_tag == 5)) {
    return 1;
  } else {
  }
  if ((expr_tag == 6)) {
    return 3;
  } else {
  }
  if ((expr_tag == 7)) {
    return 2;
  } else {
  }
  if ((expr_tag == 8)) {
    return 2;
  } else {
  }
  if ((expr_tag == 9)) {
    return 1;
  } else {
  }
  if ((expr_tag == 10)) {
    return 2;
  } else {
  }
  if ((expr_tag == 11)) {
    return 1;
  } else {
  }
  if ((expr_tag == 12)) {
    return 1;
  } else {
  }
  if ((expr_tag == 13)) {
    return 1;
  } else {
  }
  if ((expr_tag == 14)) {
    return 1;
  } else {
  }
  if ((expr_tag == 15)) {
    return 0;
  } else {
  }
  if ((expr_tag == 16)) {
    return 1;
  } else {
  }
  if ((expr_tag == 17)) {
    return 1;
  } else {
  }
  if ((expr_tag == 18)) {
    return 2;
  } else {
  }
  if ((expr_tag == 19)) {
    return 2;
  } else {
  }
  return 0;
}

static int expr_can_reference_result(int expr_tag) {
  int _sv0t0 = expr_refs_child_count(expr_tag);
  int _sv0t1 = (_sv0t0 > 0);
  return _sv0t1;
}

static int contract_kind_tag(int contract_tag) {
  if ((contract_tag == 0)) {
    return 1;
  } else {
  }
  if ((contract_tag == 1)) {
    return 2;
  } else {
  }
  if ((contract_tag == 2)) {
    return 3;
  } else {
  }
  return 0;
}

static int is_requires(int tag) {
  int _sv0t0 = (tag == 0);
  return _sv0t0;
}

static int is_ensures(int tag) {
  int _sv0t0 = (tag == 1);
  return _sv0t0;
}

static int ensures_needs_result_binding(int tag) {
  int _sv0t0 = is_ensures(tag);
  return _sv0t0;
}

static int test_result_path(void) {
  int _sv0t0 = expr_is_result_path(0, 1, 1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = expr_is_result_path(0, 1, 2);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = expr_is_result_path(0, 2, 1);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = expr_is_result_path(1, 1, 1);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_child_count(void) {
  int _sv0t0 = expr_refs_child_count(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = expr_refs_child_count(2);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = expr_refs_child_count(6);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = expr_refs_child_count(15);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_can_ref(void) {
  int _sv0t0 = expr_can_reference_result(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = expr_can_reference_result(2);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = expr_can_reference_result(6);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_contract(void) {
  int _sv0t0 = contract_kind_tag(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = contract_kind_tag(1);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = contract_kind_tag(2);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_requires(0);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_ensures(1);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = ensures_needs_result_binding(1);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = ensures_needs_result_binding(0);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_result_path();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_child_count();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_can_ref();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_contract();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

