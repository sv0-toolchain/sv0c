#include "sv0_runtime.h"

static int prec_of_binop(int binop_tag);
static int is_right_assoc(int binop_tag);
static int next_prec(int current, int right_assoc);
static int binop_is_comparison(int binop_tag);
static int binop_is_arithmetic(int binop_tag);
static int binop_is_logical(int binop_tag);
static int test_prec(void);
static int test_next_prec(void);
static int test_categories(void);

static int prec_of_binop(int binop_tag) {
  if ((binop_tag == 10)) {
    return 1;
  } else {
  }
  if ((binop_tag == 11)) {
    return 1;
  } else {
  }
  if ((binop_tag == 5)) {
    return 2;
  } else {
  }
  if ((binop_tag == 6)) {
    return 2;
  } else {
  }
  if ((binop_tag == 7)) {
    return 2;
  } else {
  }
  if ((binop_tag == 12)) {
    return 3;
  } else {
  }
  if ((binop_tag == 13)) {
    return 3;
  } else {
  }
  if ((binop_tag == 14)) {
    return 3;
  } else {
  }
  if ((binop_tag == 15)) {
    return 3;
  } else {
  }
  if ((binop_tag == 16)) {
    return 3;
  } else {
  }
  if ((binop_tag == 17)) {
    return 3;
  } else {
  }
  if ((binop_tag == 8)) {
    return 4;
  } else {
  }
  if ((binop_tag == 9)) {
    return 4;
  } else {
  }
  if ((binop_tag == 0)) {
    return 5;
  } else {
  }
  if ((binop_tag == 1)) {
    return 5;
  } else {
  }
  if ((binop_tag == 2)) {
    return 6;
  } else {
  }
  if ((binop_tag == 3)) {
    return 6;
  } else {
  }
  if ((binop_tag == 4)) {
    return 6;
  } else {
  }
  return 0;
}

static int is_right_assoc(int binop_tag) {
  return 0;
}

static int next_prec(int current, int right_assoc) {
  if (right_assoc) {
    return current;
  } else {
  }
  int _sv0t0 = (current + 1);
  return _sv0t0;
}

static int binop_is_comparison(int binop_tag) {
  int _sv0t0 = prec_of_binop(binop_tag);
  int p = _sv0t0;
  int _sv0t1 = (p == 3);
  return _sv0t1;
}

static int binop_is_arithmetic(int binop_tag) {
  int _sv0t0 = prec_of_binop(binop_tag);
  int p = _sv0t0;
  if ((p == 5)) {
    return 1;
  } else {
  }
  if ((p == 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int binop_is_logical(int binop_tag) {
  int _sv0t0 = prec_of_binop(binop_tag);
  int p = _sv0t0;
  int _sv0t1 = (p == 1);
  return _sv0t1;
}

static int test_prec(void) {
  int _sv0t0 = prec_of_binop(10);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = prec_of_binop(12);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = prec_of_binop(0);
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  int _sv0t3 = prec_of_binop(2);
  if ((_sv0t3 != 6)) {
    return 4;
  } else {
  }
  int _sv0t4 = prec_of_binop(8);
  if ((_sv0t4 != 4)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_next_prec(void) {
  int _sv0t0 = next_prec(3, 0);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = next_prec(3, 1);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_categories(void) {
  int _sv0t0 = binop_is_comparison(12);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = binop_is_comparison(0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = binop_is_arithmetic(0);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = binop_is_arithmetic(2);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = binop_is_arithmetic(12);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = binop_is_logical(10);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = binop_is_logical(0);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_prec();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_next_prec();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_categories();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  return 0;
}

