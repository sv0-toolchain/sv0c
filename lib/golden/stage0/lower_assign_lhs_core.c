#include "sv0_runtime.h"

static int classify_assign_lhs_kind(int path_segment_count, int is_field_access);
static int kind_none(void);
static int kind_var(void);
static int kind_field(void);

static int classify_assign_lhs_kind(int path_segment_count, int is_field_access) {
  int _sv0t0;
  int _sv0t1;
  if ((path_segment_count != 1)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((is_field_access != 0)) {
      return 2;
      _sv0t2 = 0;
    } else {
      return 1;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int kind_none(void) {
  return 0;
}

static int kind_var(void) {
  return 1;
}

static int kind_field(void) {
  return 2;
}

int main(void) {
  int _sv0t0 = classify_assign_lhs_kind(1, 0);
  int _sv0t1 = kind_var();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = classify_assign_lhs_kind(1, 1);
  int _sv0t3 = kind_field();
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = classify_assign_lhs_kind(0, 0);
  int _sv0t5 = kind_none();
  if ((_sv0t4 != _sv0t5)) {
    return 1;
  } else {
  }
  int _sv0t6 = classify_assign_lhs_kind(2, 0);
  int _sv0t7 = kind_none();
  if ((_sv0t6 != _sv0t7)) {
    return 1;
  } else {
  }
  int _sv0t8 = classify_assign_lhs_kind(2, 1);
  int _sv0t9 = kind_none();
  if ((_sv0t8 != _sv0t9)) {
    return 1;
  } else {
  }
  int _sv0t10 = classify_assign_lhs_kind(3, 0);
  int _sv0t11 = kind_none();
  if ((_sv0t10 != _sv0t11)) {
    return 1;
  } else {
  }
  return 0;
}

