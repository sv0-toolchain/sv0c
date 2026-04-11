#include "sv0_runtime.h"

static int is_integral_ty(int tag);
static int is_bool_ty(int tag);
static int is_unit_ty(int tag);
static int is_string_ty(int tag);
static int is_numeric_ty(int tag);
static int is_primitive_ty(int tag);
static int ctor_ty_result(int shape_tag, int field_count);
static int path_key_segment_count(int seg0, int seg1, int seg2);
static int test_integral(void);
static int test_bool_unit_string(void);
static int test_primitive(void);
static int test_ctor_ty(void);
static int test_path_key(void);

static int is_integral_ty(int tag) {
  if ((tag == 1)) {
    return 1;
  } else {
  }
  if ((tag == 2)) {
    return 1;
  } else {
  }
  if ((tag == 3)) {
    return 1;
  } else {
  }
  if ((tag == 4)) {
    return 1;
  } else {
  }
  if ((tag == 5)) {
    return 1;
  } else {
  }
  if ((tag == 6)) {
    return 1;
  } else {
  }
  if ((tag == 7)) {
    return 1;
  } else {
  }
  if ((tag == 8)) {
    return 1;
  } else {
  }
  if ((tag == 9)) {
    return 1;
  } else {
  }
  if ((tag == 10)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_bool_ty(int tag) {
  int _sv0t0 = (tag == 0);
  return _sv0t0;
}

static int is_unit_ty(int tag) {
  int _sv0t0 = (tag == 11);
  return _sv0t0;
}

static int is_string_ty(int tag) {
  int _sv0t0 = (tag == 12);
  return _sv0t0;
}

static int is_numeric_ty(int tag) {
  int _sv0t0 = is_integral_ty(tag);
  return _sv0t0;
}

static int is_primitive_ty(int tag) {
  int _sv0t0 = is_bool_ty(tag);
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = is_integral_ty(tag);
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = is_unit_ty(tag);
  if (_sv0t2) {
    return 1;
  } else {
  }
  int _sv0t3 = is_string_ty(tag);
  if (_sv0t3) {
    return 1;
  } else {
  }
  return 0;
}

static int ctor_ty_result(int shape_tag, int field_count) {
  if ((shape_tag == 0)) {
    return 0;
  } else {
  }
  if ((shape_tag == 1)) {
    return field_count;
  } else {
  }
  if ((shape_tag == 2)) {
    return field_count;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int path_key_segment_count(int seg0, int seg1, int seg2) {
  if ((seg0 == 0)) {
    return 0;
  } else {
  }
  if ((seg1 == 0)) {
    return 1;
  } else {
  }
  if ((seg2 == 0)) {
    return 2;
  } else {
  }
  return 3;
}

static int test_integral(void) {
  int _sv0t0 = is_integral_ty(1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_integral_ty(5);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_integral_ty(9);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_integral_ty(10);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_integral_ty(0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_integral_ty(11);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_integral_ty(12);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_integral_ty(13);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_integral_ty(19);
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_bool_unit_string(void) {
  int _sv0t0 = is_bool_ty(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_bool_ty(1);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_unit_ty(11);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_unit_ty(0);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_string_ty(12);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_string_ty(0);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_primitive(void) {
  int _sv0t0 = is_primitive_ty(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_primitive_ty(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_primitive_ty(11);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_primitive_ty(12);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_primitive_ty(13);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_primitive_ty(14);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_ctor_ty(void) {
  int _sv0t0 = ctor_ty_result(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = ctor_ty_result(1, 3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = ctor_ty_result(2, 5);
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_path_key(void) {
  int _sv0t0 = path_key_segment_count(0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = path_key_segment_count(1, 0, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = path_key_segment_count(1, 2, 0);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = path_key_segment_count(1, 2, 3);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_integral();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_bool_unit_string();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_primitive();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_ctor_ty();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_path_key();
  int r5 = _sv0t8;
  if ((r5 != 0)) {
    int _sv0t9 = (500 + r5);
    return _sv0t9;
  } else {
  }
  return 0;
}

