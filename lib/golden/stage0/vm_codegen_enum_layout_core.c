#include "sv0_runtime.h"

static int variant_slots(int has_payload, int payload_fields);
static int max_variant_slots(int s0, int s1, int s2, int s3);
static int enum_layout_width(int max_payload);
static int index_of_field(int target, int f0, int f1, int f2, int f3);
static int enum_field_name_tag(int index);
static int is_enum_item(int item_kind);

static int variant_slots(int has_payload, int payload_fields) {
  if ((!has_payload)) {
    return 0;
  } else {
  }
  return payload_fields;
}

static int max_variant_slots(int s0, int s1, int s2, int s3) {
  int m = s0;
  if ((s1 > m)) {
    m = s1;
  } else {
  }
  if ((s2 > m)) {
    m = s2;
  } else {
  }
  if ((s3 > m)) {
    m = s3;
  } else {
  }
  return m;
}

static int enum_layout_width(int max_payload) {
  int _sv0t0 = (1 + max_payload);
  return _sv0t0;
}

static int index_of_field(int target, int f0, int f1, int f2, int f3) {
  if ((f0 == target)) {
    return 0;
  } else {
  }
  if ((f1 == target)) {
    return 1;
  } else {
  }
  if ((f2 == target)) {
    return 2;
  } else {
  }
  if ((f3 == target)) {
    return 3;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int enum_field_name_tag(int index) {
  if ((index == 0)) {
    return 100;
  } else {
  }
  int _sv0t0 = (200 + index);
  int _sv0t1 = (_sv0t0 - 1);
  return _sv0t1;
}

static int is_enum_item(int item_kind) {
  int _sv0t0 = (item_kind == 3);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = variant_slots(0, 3);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_slots(1, 3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_slots(1, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = max_variant_slots(1, 3, 2, 0);
  if ((_sv0t3 != 3)) {
    return 10;
  } else {
  }
  int _sv0t4 = max_variant_slots(0, 0, 0, 0);
  if ((_sv0t4 != 0)) {
    return 11;
  } else {
  }
  int _sv0t5 = max_variant_slots(5, 2, 5, 1);
  if ((_sv0t5 != 5)) {
    return 12;
  } else {
  }
  int _sv0t6 = enum_layout_width(0);
  if ((_sv0t6 != 1)) {
    return 20;
  } else {
  }
  int _sv0t7 = enum_layout_width(3);
  if ((_sv0t7 != 4)) {
    return 21;
  } else {
  }
  int _sv0t8 = enum_layout_width(1);
  if ((_sv0t8 != 2)) {
    return 22;
  } else {
  }
  int _sv0t9 = max_variant_slots(1, 1, 0, 0);
  int max_r = _sv0t9;
  int _sv0t10 = enum_layout_width(max_r);
  if ((_sv0t10 != 2)) {
    return 23;
  } else {
  }
  int _sv0t11 = max_variant_slots(1, 0, 0, 0);
  int max_o = _sv0t11;
  int _sv0t12 = enum_layout_width(max_o);
  if ((_sv0t12 != 2)) {
    return 24;
  } else {
  }
  int _sv0t13 = index_of_field(10, 10, 20, 30, 40);
  if ((_sv0t13 != 0)) {
    return 30;
  } else {
  }
  int _sv0t14 = index_of_field(30, 10, 20, 30, 40);
  if ((_sv0t14 != 2)) {
    return 31;
  } else {
  }
  int _sv0t15 = index_of_field(99, 10, 20, 30, 40);
  int _sv0t16 = (-1);
  if ((_sv0t15 != _sv0t16)) {
    return 32;
  } else {
  }
  int _sv0t17 = enum_field_name_tag(0);
  if ((_sv0t17 != 100)) {
    return 40;
  } else {
  }
  int _sv0t18 = enum_field_name_tag(1);
  if ((_sv0t18 != 200)) {
    return 41;
  } else {
  }
  int _sv0t19 = enum_field_name_tag(2);
  if ((_sv0t19 != 201)) {
    return 42;
  } else {
  }
  int _sv0t20 = is_enum_item(3);
  if ((!_sv0t20)) {
    return 50;
  } else {
  }
  int _sv0t21 = is_enum_item(1);
  if (_sv0t21) {
    return 51;
  } else {
  }
  return 0;
}

