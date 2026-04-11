#include "sv0_runtime.h"

static int has_duplicate(int n0, int n1, int n2, int n3);
static int canon_ty_import(int name, int alias_from, int alias_to);
static int canon_chain(int name, int a0_from, int a0_to, int a1_from, int a1_to);
static int try_success_shape(int has_ok, int has_err, int has_some, int has_none);
static int is_result_shape(int shape);
static int is_option_shape(int shape);
static int test_dup(void);
static int test_canon(void);
static int test_try_success(void);

static int has_duplicate(int n0, int n1, int n2, int n3) {
  if ((n1 != 0)) {
    if ((n1 == n0)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((n2 != 0)) {
    if ((n2 == n0)) {
      return 1;
    } else {
    }
    if ((n2 == n1)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((n3 != 0)) {
    if ((n3 == n0)) {
      return 1;
    } else {
    }
    if ((n3 == n1)) {
      return 1;
    } else {
    }
    if ((n3 == n2)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int canon_ty_import(int name, int alias_from, int alias_to) {
  if ((name == alias_from)) {
    if ((alias_to == alias_from)) {
      return name;
    } else {
    }
    return alias_to;
  } else {
  }
  return name;
}

static int canon_chain(int name, int a0_from, int a0_to, int a1_from, int a1_to) {
  int _sv0t0 = canon_ty_import(name, a0_from, a0_to);
  int step1 = _sv0t0;
  int _sv0t1 = canon_ty_import(step1, a1_from, a1_to);
  int step2 = _sv0t1;
  return step2;
}

static int try_success_shape(int has_ok, int has_err, int has_some, int has_none) {
  if (has_ok) {
    if (has_err) {
      return 1;
    } else {
    }
  } else {
  }
  if (has_some) {
    if (has_none) {
      return 2;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_result_shape(int shape) {
  int _sv0t0 = (shape == 1);
  return _sv0t0;
}

static int is_option_shape(int shape) {
  int _sv0t0 = (shape == 2);
  return _sv0t0;
}

static int test_dup(void) {
  int _sv0t0 = has_duplicate(1, 2, 3, 4);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = has_duplicate(1, 1, 0, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = has_duplicate(1, 2, 1, 0);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = has_duplicate(1, 2, 3, 2);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = has_duplicate(5, 0, 0, 0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_canon(void) {
  int _sv0t0 = canon_ty_import(10, 10, 20);
  if ((_sv0t0 != 20)) {
    return 1;
  } else {
  }
  int _sv0t1 = canon_ty_import(10, 10, 10);
  if ((_sv0t1 != 10)) {
    return 2;
  } else {
  }
  int _sv0t2 = canon_ty_import(10, 30, 40);
  if ((_sv0t2 != 10)) {
    return 3;
  } else {
  }
  int _sv0t3 = canon_chain(10, 10, 20, 20, 30);
  if ((_sv0t3 != 30)) {
    return 4;
  } else {
  }
  int _sv0t4 = canon_chain(10, 10, 20, 30, 40);
  if ((_sv0t4 != 20)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_try_success(void) {
  int _sv0t0 = try_success_shape(1, 1, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = try_success_shape(0, 0, 1, 1);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = try_success_shape(1, 0, 0, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = try_success_shape(0, 0, 0, 1);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_result_shape(1);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_option_shape(2);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_result_shape(2);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_dup();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_canon();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_try_success();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  return 0;
}

