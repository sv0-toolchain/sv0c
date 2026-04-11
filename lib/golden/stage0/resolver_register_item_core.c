#include "sv0_runtime.h"

static int variant_stem_tag(int variant_kind);
static int enum_variant_arity(int variant_kind, int tuple_fields, int struct_fields);
static int register_item_tag(int item_kind);
static int item_registers_fn(int item_kind);
static int item_registers_type(int item_kind);
static int item_registers_variants(int item_kind);
static int fn_arity_from_params(int param_count);
static int variant_count_total(int v0_exists, int v1_exists, int v2_exists, int v3_exists);
static int test_variant_arity(void);
static int test_register_item(void);
static int test_item_predicates(void);
static int test_variant_count(void);

static int variant_stem_tag(int variant_kind) {
  return variant_kind;
}

static int enum_variant_arity(int variant_kind, int tuple_fields, int struct_fields) {
  if ((variant_kind == 0)) {
    return 0;
  } else {
  }
  if ((variant_kind == 1)) {
    return tuple_fields;
  } else {
  }
  if ((variant_kind == 2)) {
    return struct_fields;
  } else {
  }
  return 0;
}

static int register_item_tag(int item_kind) {
  if ((item_kind == 0)) {
    return 1;
  } else {
  }
  if ((item_kind == 1)) {
    return 2;
  } else {
  }
  if ((item_kind == 2)) {
    return 3;
  } else {
  }
  if ((item_kind == 3)) {
    return 4;
  } else {
  }
  if ((item_kind == 4)) {
    return 5;
  } else {
  }
  if ((item_kind == 5)) {
    return 6;
  } else {
  }
  if ((item_kind == 6)) {
    return 7;
  } else {
  }
  if ((item_kind == 7)) {
    return 8;
  } else {
  }
  return 0;
}

static int item_registers_fn(int item_kind) {
  int _sv0t0 = (item_kind == 0);
  return _sv0t0;
}

static int item_registers_type(int item_kind) {
  if ((item_kind == 1)) {
    return 1;
  } else {
  }
  if ((item_kind == 2)) {
    return 1;
  } else {
  }
  if ((item_kind == 3)) {
    return 1;
  } else {
  }
  return 0;
}

static int item_registers_variants(int item_kind) {
  int _sv0t0 = (item_kind == 2);
  return _sv0t0;
}

static int fn_arity_from_params(int param_count) {
  return param_count;
}

static int variant_count_total(int v0_exists, int v1_exists, int v2_exists, int v3_exists) {
  int count = 0;
  if (v0_exists) {
    count = (count + 1);
  } else {
  }
  if (v1_exists) {
    count = (count + 1);
  } else {
  }
  if (v2_exists) {
    count = (count + 1);
  } else {
  }
  if (v3_exists) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int test_variant_arity(void) {
  int _sv0t0 = enum_variant_arity(0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = enum_variant_arity(1, 3, 0);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = enum_variant_arity(2, 0, 5);
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_register_item(void) {
  int _sv0t0 = register_item_tag(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = register_item_tag(2);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = register_item_tag(7);
  if ((_sv0t2 != 8)) {
    return 3;
  } else {
  }
  int _sv0t3 = register_item_tag(8);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_item_predicates(void) {
  int _sv0t0 = item_registers_fn(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = item_registers_fn(1);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = item_registers_type(1);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = item_registers_type(2);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = item_registers_type(0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = item_registers_variants(2);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = item_registers_variants(1);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_variant_count(void) {
  int _sv0t0 = variant_count_total(1, 1, 0, 0);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_count_total(1, 1, 1, 1);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_count_total(0, 0, 0, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_variant_arity();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_register_item();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_item_predicates();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_variant_count();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

