#include "sv0_runtime.h"

static int intrinsic_count(void);
static int intrinsic_arity(int idx);
static int is_string_intrinsic(int idx);
static int is_vec_intrinsic(int idx);
static int is_box_intrinsic(int idx);
static int env_size_after_intrinsics(int initial);

static int intrinsic_count(void) {
  return 16;
}

static int intrinsic_arity(int idx) {
  if ((idx == 0)) {
    return 1;
  } else {
  }
  if ((idx == 1)) {
    return 1;
  } else {
  }
  if ((idx == 2)) {
    return 1;
  } else {
  }
  if ((idx == 3)) {
    return 2;
  } else {
  }
  if ((idx == 4)) {
    return 2;
  } else {
  }
  if ((idx == 5)) {
    return 3;
  } else {
  }
  if ((idx == 6)) {
    return 0;
  } else {
  }
  if ((idx == 7)) {
    return 2;
  } else {
  }
  if ((idx == 8)) {
    return 1;
  } else {
  }
  if ((idx == 9)) {
    return 2;
  } else {
  }
  if ((idx == 10)) {
    return 3;
  } else {
  }
  if ((idx == 11)) {
    return 1;
  } else {
  }
  if ((idx == 12)) {
    return 1;
  } else {
  }
  if ((idx == 13)) {
    return 1;
  } else {
  }
  if ((idx == 14)) {
    return 2;
  } else {
  }
  if ((idx == 15)) {
    return 2;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int is_string_intrinsic(int idx) {
  int _sv0t0 = (idx >= 1);
  int _sv0t1 = (idx <= 5);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int is_vec_intrinsic(int idx) {
  int _sv0t0 = (idx >= 6);
  int _sv0t1 = (idx <= 10);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int is_box_intrinsic(int idx) {
  int _sv0t0 = (idx >= 11);
  int _sv0t1 = (idx <= 13);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int env_size_after_intrinsics(int initial) {
  int _sv0t0 = intrinsic_count();
  int _sv0t1 = (initial + _sv0t0);
  return _sv0t1;
}

int main(void) {
  int _sv0t0 = intrinsic_count();
  if ((_sv0t0 != 16)) {
    return 1;
  } else {
  }
  int _sv0t1 = intrinsic_arity(0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = intrinsic_arity(6);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = intrinsic_arity(5);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = intrinsic_arity(99);
  int _sv0t5 = (-1);
  if ((_sv0t4 != _sv0t5)) {
    return 5;
  } else {
  }
  int _sv0t6 = is_string_intrinsic(1);
  if ((!_sv0t6)) {
    return 6;
  } else {
  }
  int _sv0t7 = is_string_intrinsic(5);
  if ((!_sv0t7)) {
    return 7;
  } else {
  }
  int _sv0t8 = is_string_intrinsic(0);
  if (_sv0t8) {
    return 8;
  } else {
  }
  int _sv0t9 = is_string_intrinsic(6);
  if (_sv0t9) {
    return 9;
  } else {
  }
  int _sv0t10 = is_vec_intrinsic(6);
  if ((!_sv0t10)) {
    return 10;
  } else {
  }
  int _sv0t11 = is_vec_intrinsic(10);
  if ((!_sv0t11)) {
    return 11;
  } else {
  }
  int _sv0t12 = is_vec_intrinsic(5);
  if (_sv0t12) {
    return 12;
  } else {
  }
  int _sv0t13 = is_box_intrinsic(11);
  if ((!_sv0t13)) {
    return 13;
  } else {
  }
  int _sv0t14 = is_box_intrinsic(13);
  if ((!_sv0t14)) {
    return 14;
  } else {
  }
  int _sv0t15 = is_box_intrinsic(10);
  if (_sv0t15) {
    return 15;
  } else {
  }
  int _sv0t16 = env_size_after_intrinsics(0);
  if ((_sv0t16 != 16)) {
    return 16;
  } else {
  }
  int _sv0t17 = env_size_after_intrinsics(5);
  if ((_sv0t17 != 21)) {
    return 17;
  } else {
  }
  return 0;
}

