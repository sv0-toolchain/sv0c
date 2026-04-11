#include "sv0_runtime.h"

static int variant_shape_tag(int kind);
static int variant_field_count(int kind, int tuple_fields, int struct_fields);
static int variant_needs_payload_slots(int kind, int tuple_fields, int struct_fields);
static int variant_shape_name_tag(int kind);
static int max_payload_across(int k0, int f0, int k1, int f1, int k2, int f2);

static int variant_shape_tag(int kind) {
  if ((kind == 0)) {
    return 0;
  } else {
  }
  if ((kind == 1)) {
    return 1;
  } else {
  }
  if ((kind == 2)) {
    return 2;
  } else {
  }
  return 255;
}

static int variant_field_count(int kind, int tuple_fields, int struct_fields) {
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

static int variant_needs_payload_slots(int kind, int tuple_fields, int struct_fields) {
  int _sv0t0 = variant_field_count(kind, tuple_fields, struct_fields);
  int fc = _sv0t0;
  int _sv0t1 = (fc > 0);
  return _sv0t1;
}

static int variant_shape_name_tag(int kind) {
  if ((kind == 0)) {
    return 0;
  } else {
  }
  if ((kind == 1)) {
    return 1;
  } else {
  }
  if ((kind == 2)) {
    return 2;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int max_payload_across(int k0, int f0, int k1, int f1, int k2, int f2) {
  int _sv0t0 = variant_field_count(k0, f0, f0);
  int c0 = _sv0t0;
  int _sv0t1 = variant_field_count(k1, f1, f1);
  int c1 = _sv0t1;
  int _sv0t2 = variant_field_count(k2, f2, f2);
  int c2 = _sv0t2;
  int mx = c0;
  if ((c1 > mx)) {
    mx = c1;
  } else {
  }
  if ((c2 > mx)) {
    mx = c2;
  } else {
  }
  return mx;
}

int main(void) {
  int _sv0t0 = variant_shape_tag(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_shape_tag(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_shape_tag(2);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = variant_shape_tag(99);
  if ((_sv0t3 != 255)) {
    return 4;
  } else {
  }
  int _sv0t4 = variant_field_count(0, 3, 2);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = variant_field_count(1, 3, 2);
  if ((_sv0t5 != 3)) {
    return 6;
  } else {
  }
  int _sv0t6 = variant_field_count(2, 3, 2);
  if ((_sv0t6 != 2)) {
    return 7;
  } else {
  }
  int _sv0t7 = variant_needs_payload_slots(0, 0, 0);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = variant_needs_payload_slots(1, 3, 0);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = variant_needs_payload_slots(2, 0, 2);
  if ((_sv0t9 != 1)) {
    return 10;
  } else {
  }
  int _sv0t10 = variant_needs_payload_slots(1, 0, 0);
  if ((_sv0t10 != 0)) {
    return 11;
  } else {
  }
  int _sv0t11 = max_payload_across(0, 0, 1, 3, 2, 2);
  if ((_sv0t11 != 3)) {
    return 12;
  } else {
  }
  int _sv0t12 = max_payload_across(0, 0, 0, 0, 0, 0);
  if ((_sv0t12 != 0)) {
    return 13;
  } else {
  }
  int _sv0t13 = max_payload_across(1, 5, 1, 2, 1, 7);
  if ((_sv0t13 != 7)) {
    return 14;
  } else {
  }
  return 0;
}

