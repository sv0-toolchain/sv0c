#include "sv0_runtime.h"

static int variant_slots(int kind, int tuple_fields, int struct_fields);
static int max2(int a, int b);
static int max3(int a, int b, int c);
static int enum_layout_width(int v0_kind, int v0_tup, int v0_str, int v1_kind, int v1_tup, int v1_str, int v2_kind, int v2_tup, int v2_str);
static int struct_layout_width(int field_count);
static int enum_field_names_count(int enum_width);
static int test_variant_slots(void);
static int test_enum_layout(void);
static int test_struct_layout(void);

static int variant_slots(int kind, int tuple_fields, int struct_fields) {
  if ((kind == 0)) {
    return 0;
  } else {
  }
  if ((kind == 1)) {
    return tuple_fields;
  } else {
  }
  if ((kind == 2)) {
    return struct_fields;
  } else {
  }
  return 0;
}

static int max2(int a, int b) {
  if ((a > b)) {
    return a;
  } else {
  }
  return b;
}

static int max3(int a, int b, int c) {
  int _sv0t0 = max2(a, b);
  int _sv0t1 = max2(_sv0t0, c);
  return _sv0t1;
}

static int enum_layout_width(int v0_kind, int v0_tup, int v0_str, int v1_kind, int v1_tup, int v1_str, int v2_kind, int v2_tup, int v2_str) {
  int _sv0t0 = variant_slots(v0_kind, v0_tup, v0_str);
  int s0 = _sv0t0;
  int _sv0t1 = variant_slots(v1_kind, v1_tup, v1_str);
  int s1 = _sv0t1;
  int _sv0t2 = variant_slots(v2_kind, v2_tup, v2_str);
  int s2 = _sv0t2;
  int _sv0t3 = max3(s0, s1, s2);
  int _sv0t4 = (1 + _sv0t3);
  return _sv0t4;
}

static int struct_layout_width(int field_count) {
  return field_count;
}

static int enum_field_names_count(int enum_width) {
  return enum_width;
}

static int test_variant_slots(void) {
  int _sv0t0 = variant_slots(0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_slots(1, 3, 0);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_slots(2, 0, 5);
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  int _sv0t3 = variant_slots(1, 1, 0);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_enum_layout(void) {
  int _sv0t0 = enum_layout_width(0, 0, 0, 1, 2, 0, 1, 3, 0);
  int w1 = _sv0t0;
  if ((w1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = enum_layout_width(0, 0, 0, 0, 0, 0, 0, 0, 0);
  int w2 = _sv0t1;
  if ((w2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = enum_layout_width(2, 0, 4, 1, 2, 0, 0, 0, 0);
  int w3 = _sv0t2;
  if ((w3 != 5)) {
    return 3;
  } else {
  }
  int _sv0t3 = enum_layout_width(1, 1, 0, 1, 1, 0, 1, 1, 0);
  int w4 = _sv0t3;
  if ((w4 != 2)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_struct_layout(void) {
  int _sv0t0 = struct_layout_width(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = struct_layout_width(3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = struct_layout_width(7);
  if ((_sv0t2 != 7)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_variant_slots();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_enum_layout();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_struct_layout();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  return 0;
}

