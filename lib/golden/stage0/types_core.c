#include "sv0_runtime.h"

static int ty_is_primitive(int kind);
static int ty_is_compound(int kind);
static int ty_has_inner(int kind);
static int fresh_var(int counter);
static int fresh_var_next(int counter);

static int ty_is_primitive(int kind) {
  if ((kind == 0)) {
    return 1;
  } else {
  }
  if ((kind == 1)) {
    return 1;
  } else {
  }
  if ((kind == 2)) {
    return 1;
  } else {
  }
  if ((kind >= 5)) {
    if ((kind <= 10)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int ty_is_compound(int kind) {
  if ((kind == 3)) {
    return 1;
  } else {
  }
  if ((kind == 4)) {
    return 1;
  } else {
  }
  if ((kind >= 11)) {
    if ((kind <= 17)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int ty_has_inner(int kind) {
  if ((kind == 11)) {
    return 1;
  } else {
  }
  if ((kind == 12)) {
    return 1;
  } else {
  }
  if ((kind == 13)) {
    return 1;
  } else {
  }
  if ((kind == 14)) {
    return 1;
  } else {
  }
  return 0;
}

static int fresh_var(int counter) {
  return counter;
}

static int fresh_var_next(int counter) {
  int _sv0t0 = (counter + 1);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = ty_is_primitive(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = ty_is_primitive(1);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = ty_is_primitive(2);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = ty_is_primitive(5);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = ty_is_primitive(10);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = ty_is_primitive(3);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = ty_is_primitive(15);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  int _sv0t7 = ty_is_compound(3);
  if ((_sv0t7 != 1)) {
    return 1;
  } else {
  }
  int _sv0t8 = ty_is_compound(4);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = ty_is_compound(11);
  if ((_sv0t9 != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = ty_is_compound(17);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = ty_is_compound(0);
  if ((_sv0t11 != 0)) {
    return 1;
  } else {
  }
  int _sv0t12 = ty_is_compound(18);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  int _sv0t13 = ty_has_inner(11);
  if ((_sv0t13 != 1)) {
    return 1;
  } else {
  }
  int _sv0t14 = ty_has_inner(12);
  if ((_sv0t14 != 1)) {
    return 1;
  } else {
  }
  int _sv0t15 = ty_has_inner(13);
  if ((_sv0t15 != 1)) {
    return 1;
  } else {
  }
  int _sv0t16 = ty_has_inner(14);
  if ((_sv0t16 != 1)) {
    return 1;
  } else {
  }
  int _sv0t17 = ty_has_inner(0);
  if ((_sv0t17 != 0)) {
    return 1;
  } else {
  }
  int _sv0t18 = ty_has_inner(15);
  if ((_sv0t18 != 0)) {
    return 1;
  } else {
  }
  int _sv0t19 = fresh_var(0);
  if ((_sv0t19 != 0)) {
    return 1;
  } else {
  }
  int _sv0t20 = fresh_var_next(0);
  if ((_sv0t20 != 1)) {
    return 1;
  } else {
  }
  int _sv0t21 = fresh_var(5);
  if ((_sv0t21 != 5)) {
    return 1;
  } else {
  }
  int _sv0t22 = fresh_var_next(5);
  if ((_sv0t22 != 6)) {
    return 1;
  } else {
  }
  return 0;
}

