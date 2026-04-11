#include "sv0_runtime.h"

static int variant_arity(int kind, int field_count);
static int variant_is_unit(int kind);
static int variant_has_fields(int kind);

static int variant_arity(int kind, int field_count) {
  if ((kind == 0)) {
    return 0;
  } else {
  }
  if ((kind == 1)) {
    return field_count;
  } else {
  }
  if ((kind == 2)) {
    return field_count;
  } else {
  }
  return 0;
}

static int variant_is_unit(int kind) {
  int _sv0t0 = (kind == 0);
  return _sv0t0;
}

static int variant_has_fields(int kind) {
  if ((kind == 1)) {
    return 1;
  } else {
  }
  if ((kind == 2)) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = variant_arity(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_arity(0, 5);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_arity(1, 3);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = variant_arity(1, 1);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = variant_arity(1, 0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = variant_arity(2, 4);
  if ((_sv0t5 != 4)) {
    return 6;
  } else {
  }
  int _sv0t6 = variant_arity(2, 2);
  if ((_sv0t6 != 2)) {
    return 7;
  } else {
  }
  int _sv0t7 = variant_is_unit(0);
  if ((_sv0t7 != 1)) {
    return 8;
  } else {
  }
  int _sv0t8 = variant_is_unit(1);
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  int _sv0t9 = variant_is_unit(2);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = variant_has_fields(0);
  if ((_sv0t10 != 0)) {
    return 11;
  } else {
  }
  int _sv0t11 = variant_has_fields(1);
  if ((_sv0t11 != 1)) {
    return 12;
  } else {
  }
  int _sv0t12 = variant_has_fields(2);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  int _sv0t13 = variant_arity(99, 5);
  if ((_sv0t13 != 0)) {
    return 14;
  } else {
  }
  return 0;
}

