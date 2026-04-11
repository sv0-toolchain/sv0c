#include "sv0_runtime.h"

static int pat_binding_action(int pat_tag);
static int pat_introduces_binding(int pat_tag);
static int pat_needs_type_check(int pat_tag);
static int pat_has_sub_patterns(int pat_tag);
static int bind_count_in_tuple(int p0_bind, int p1_bind, int p2_bind);
static int enum_pat_scrutinee_check(int has_enum, int has_variant);

static int pat_binding_action(int pat_tag) {
  if ((pat_tag == 0)) {
    return 0;
  } else {
  }
  if ((pat_tag == 1)) {
    return 1;
  } else {
  }
  if ((pat_tag == 2)) {
    return 0;
  } else {
  }
  if ((pat_tag == 3)) {
    return 0;
  } else {
  }
  if ((pat_tag == 4)) {
    return 0;
  } else {
  }
  if ((pat_tag == 5)) {
    return 0;
  } else {
  }
  if ((pat_tag == 6)) {
    return 0;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int pat_introduces_binding(int pat_tag) {
  int _sv0t0 = (pat_tag == 1);
  return _sv0t0;
}

static int pat_needs_type_check(int pat_tag) {
  if ((pat_tag == 2)) {
    return 1;
  } else {
  }
  if ((pat_tag == 3)) {
    return 1;
  } else {
  }
  if ((pat_tag == 4)) {
    return 1;
  } else {
  }
  if ((pat_tag == 5)) {
    return 1;
  } else {
  }
  return 0;
}

static int pat_has_sub_patterns(int pat_tag) {
  if ((pat_tag == 3)) {
    return 1;
  } else {
  }
  if ((pat_tag == 4)) {
    return 1;
  } else {
  }
  if ((pat_tag == 5)) {
    return 1;
  } else {
  }
  if ((pat_tag == 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int bind_count_in_tuple(int p0_bind, int p1_bind, int p2_bind) {
  int count = 0;
  if (p0_bind) {
    count = (count + 1);
  } else {
  }
  if (p1_bind) {
    count = (count + 1);
  } else {
  }
  if (p2_bind) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int enum_pat_scrutinee_check(int has_enum, int has_variant) {
  int _sv0t0 = (has_enum && has_variant);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = pat_binding_action(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = pat_binding_action(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = pat_binding_action(2);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = pat_binding_action(99);
  int _sv0t4 = (-1);
  if ((_sv0t3 != _sv0t4)) {
    return 4;
  } else {
  }
  int _sv0t5 = pat_introduces_binding(1);
  if ((!_sv0t5)) {
    return 5;
  } else {
  }
  int _sv0t6 = pat_introduces_binding(0);
  if (_sv0t6) {
    return 6;
  } else {
  }
  int _sv0t7 = pat_needs_type_check(2);
  if ((!_sv0t7)) {
    return 7;
  } else {
  }
  int _sv0t8 = pat_needs_type_check(0);
  if (_sv0t8) {
    return 8;
  } else {
  }
  int _sv0t9 = pat_needs_type_check(1);
  if (_sv0t9) {
    return 9;
  } else {
  }
  int _sv0t10 = pat_has_sub_patterns(3);
  if ((!_sv0t10)) {
    return 10;
  } else {
  }
  int _sv0t11 = pat_has_sub_patterns(6);
  if ((!_sv0t11)) {
    return 11;
  } else {
  }
  int _sv0t12 = pat_has_sub_patterns(1);
  if (_sv0t12) {
    return 12;
  } else {
  }
  int _sv0t13 = bind_count_in_tuple(1, 0, 1);
  if ((_sv0t13 != 2)) {
    return 13;
  } else {
  }
  int _sv0t14 = bind_count_in_tuple(1, 1, 1);
  if ((_sv0t14 != 3)) {
    return 14;
  } else {
  }
  int _sv0t15 = enum_pat_scrutinee_check(1, 1);
  if ((!_sv0t15)) {
    return 15;
  } else {
  }
  int _sv0t16 = enum_pat_scrutinee_check(0, 1);
  if (_sv0t16) {
    return 16;
  } else {
  }
  return 0;
}

