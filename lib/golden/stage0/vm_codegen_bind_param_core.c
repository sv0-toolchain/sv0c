#include "sv0_runtime.h"

static int width_of_cty(int cty_tag, int struct_field_count, int enum_width);
static int bind_param_next_slot(int slot, int cty_tag, int struct_fields, int enum_width);
static int alloc_local_next_slot(int slot, int cty_tag, int struct_fields, int enum_width);
static int slot_has_fields(int cty_tag);
static int total_locals_for_params(int p0_cty, int p0_sf, int p0_ew, int p1_cty, int p1_sf, int p1_ew, int p2_cty, int p2_sf, int p2_ew);
static int ensure_var_int_slot(int existing_slot, int has_binding, int next_free);
static int test_width(void);
static int test_bind_param(void);
static int test_total_locals(void);
static int test_has_fields(void);
static int test_ensure_var(void);

static int width_of_cty(int cty_tag, int struct_field_count, int enum_width) {
  if ((cty_tag == 0)) {
    return 1;
  } else {
  }
  if ((cty_tag == 1)) {
    return struct_field_count;
  } else {
  }
  if ((cty_tag == 2)) {
    return enum_width;
  } else {
  }
  return 1;
}

static int bind_param_next_slot(int slot, int cty_tag, int struct_fields, int enum_width) {
  int _sv0t0 = width_of_cty(cty_tag, struct_fields, enum_width);
  int w = _sv0t0;
  int _sv0t1 = (slot + w);
  return _sv0t1;
}

static int alloc_local_next_slot(int slot, int cty_tag, int struct_fields, int enum_width) {
  int _sv0t0 = bind_param_next_slot(slot, cty_tag, struct_fields, enum_width);
  return _sv0t0;
}

static int slot_has_fields(int cty_tag) {
  if ((cty_tag == 1)) {
    return 1;
  } else {
  }
  if ((cty_tag == 2)) {
    return 1;
  } else {
  }
  return 0;
}

static int total_locals_for_params(int p0_cty, int p0_sf, int p0_ew, int p1_cty, int p1_sf, int p1_ew, int p2_cty, int p2_sf, int p2_ew) {
  int _sv0t0 = width_of_cty(p0_cty, p0_sf, p0_ew);
  int s0 = _sv0t0;
  int _sv0t1 = width_of_cty(p1_cty, p1_sf, p1_ew);
  int s1 = _sv0t1;
  int _sv0t2 = width_of_cty(p2_cty, p2_sf, p2_ew);
  int s2 = _sv0t2;
  int _sv0t3 = (s0 + s1);
  int _sv0t4 = (_sv0t3 + s2);
  return _sv0t4;
}

static int ensure_var_int_slot(int existing_slot, int has_binding, int next_free) {
  if (has_binding) {
    return existing_slot;
  } else {
  }
  return next_free;
}

static int test_width(void) {
  int _sv0t0 = width_of_cty(0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = width_of_cty(1, 5, 0);
  if ((_sv0t1 != 5)) {
    return 2;
  } else {
  }
  int _sv0t2 = width_of_cty(2, 0, 3);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = width_of_cty(3, 0, 0);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_bind_param(void) {
  int _sv0t0 = bind_param_next_slot(0, 0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = bind_param_next_slot(0, 1, 4, 0);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = bind_param_next_slot(5, 2, 0, 3);
  if ((_sv0t2 != 8)) {
    return 3;
  } else {
  }
  int _sv0t3 = bind_param_next_slot(10, 0, 0, 0);
  if ((_sv0t3 != 11)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_total_locals(void) {
  int _sv0t0 = total_locals_for_params(0, 0, 0, 0, 0, 0, 0, 0, 0);
  int t1 = _sv0t0;
  if ((t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = total_locals_for_params(0, 0, 0, 1, 3, 0, 2, 0, 4);
  int t2 = _sv0t1;
  if ((t2 != 8)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_has_fields(void) {
  int _sv0t0 = slot_has_fields(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = slot_has_fields(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = slot_has_fields(2);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_ensure_var(void) {
  int _sv0t0 = ensure_var_int_slot(5, 1, 10);
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  int _sv0t1 = ensure_var_int_slot(5, 0, 10);
  if ((_sv0t1 != 10)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_width();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_bind_param();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_total_locals();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_has_fields();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_ensure_var();
  int r5 = _sv0t8;
  if ((r5 != 0)) {
    int _sv0t9 = (500 + r5);
    return _sv0t9;
  } else {
  }
  return 0;
}

