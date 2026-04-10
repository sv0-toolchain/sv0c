#include "sv0_runtime.h"

static int unify_simple(int kind_a, int kind_b);
static int unify_payload(int kind_a, int kind_b, int payload_a, int payload_b);
static int payload_eq(int a, int b);
static int unify_list_ok(int len_a, int len_b, int all_pairs_match);

static int unify_simple(int kind_a, int kind_b) {
  if ((kind_a == 18)) {
    if ((kind_b == 18)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((kind_b == 18)) {
    return 0;
  } else {
  }
  if ((kind_a == 11)) {
    if ((kind_b == 11)) {
      return 1;
    } else {
    }
    if ((kind_b == 12)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((kind_a == 12)) {
    if ((kind_b == 11)) {
      return 1;
    } else {
    }
    if ((kind_b == 12)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((kind_a == kind_b)) {
    return 1;
  } else {
  }
  return 0;
}

static int unify_payload(int kind_a, int kind_b, int payload_a, int payload_b) {
  int _sv0t0 = unify_simple(kind_a, kind_b);
  if ((_sv0t0 == 0)) {
    return 0;
  } else {
  }
  if ((kind_a == 5)) {
    int _sv0t1 = payload_eq(payload_a, payload_b);
    return _sv0t1;
  } else {
  }
  if ((kind_a == 6)) {
    int _sv0t2 = payload_eq(payload_a, payload_b);
    return _sv0t2;
  } else {
  }
  if ((kind_a == 7)) {
    int _sv0t3 = payload_eq(payload_a, payload_b);
    return _sv0t3;
  } else {
  }
  if ((kind_a == 3)) {
    int _sv0t4 = payload_eq(payload_a, payload_b);
    return _sv0t4;
  } else {
  }
  if ((kind_a == 4)) {
    int _sv0t5 = payload_eq(payload_a, payload_b);
    return _sv0t5;
  } else {
  }
  if ((kind_a == 13)) {
    int _sv0t6 = payload_eq(payload_a, payload_b);
    return _sv0t6;
  } else {
  }
  if ((kind_a == 17)) {
    int _sv0t7 = payload_eq(payload_a, payload_b);
    return _sv0t7;
  } else {
  }
  if ((kind_a == 18)) {
    int _sv0t8 = payload_eq(payload_a, payload_b);
    return _sv0t8;
  } else {
  }
  return 1;
}

static int payload_eq(int a, int b) {
  if ((a == b)) {
    return 1;
  } else {
  }
  return 0;
}

static int unify_list_ok(int len_a, int len_b, int all_pairs_match) {
  if ((len_a != len_b)) {
    return 0;
  } else {
  }
  return all_pairs_match;
}

int main(void) {
  int _sv0t0 = unify_simple(0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = unify_simple(1, 1);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = unify_simple(10, 10);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = unify_simple(0, 1);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = unify_simple(5, 6);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = unify_simple(18, 18);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = unify_simple(18, 0);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  int _sv0t7 = unify_simple(0, 18);
  if ((_sv0t7 != 0)) {
    return 1;
  } else {
  }
  int _sv0t8 = unify_simple(11, 11);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = unify_simple(11, 12);
  if ((_sv0t9 != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = unify_simple(12, 11);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = unify_simple(12, 12);
  if ((_sv0t11 != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = unify_simple(11, 0);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  int _sv0t13 = unify_payload(5, 5, 32, 32);
  if ((_sv0t13 != 1)) {
    return 1;
  } else {
  }
  int _sv0t14 = unify_payload(5, 5, 32, 64);
  if ((_sv0t14 != 0)) {
    return 1;
  } else {
  }
  int _sv0t15 = unify_payload(18, 18, 0, 0);
  if ((_sv0t15 != 1)) {
    return 1;
  } else {
  }
  int _sv0t16 = unify_payload(18, 18, 0, 1);
  if ((_sv0t16 != 0)) {
    return 1;
  } else {
  }
  int _sv0t17 = unify_payload(3, 3, 1, 1);
  if ((_sv0t17 != 1)) {
    return 1;
  } else {
  }
  int _sv0t18 = unify_payload(3, 3, 1, 2);
  if ((_sv0t18 != 0)) {
    return 1;
  } else {
  }
  int _sv0t19 = unify_list_ok(2, 2, 1);
  if ((_sv0t19 != 1)) {
    return 1;
  } else {
  }
  int _sv0t20 = unify_list_ok(2, 3, 1);
  if ((_sv0t20 != 0)) {
    return 1;
  } else {
  }
  int _sv0t21 = unify_list_ok(2, 2, 0);
  if ((_sv0t21 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

