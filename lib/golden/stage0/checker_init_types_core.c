#include "sv0_runtime.h"

static int item_registers_type(int item_kind);
static int is_struct_item(int item_kind);
static int is_enum_item(int item_kind);
static int count_type_registrations(int k0, int k1, int k2, int k3);
static int struct_field_ty_resolves(int field_ty_tag);
static int enum_variant_count(int v0, int v1, int v2, int v3);
static int init_order_matters(void);

static int item_registers_type(int item_kind) {
  if ((item_kind == 3)) {
    return 1;
  } else {
  }
  if ((item_kind == 4)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_struct_item(int item_kind) {
  int _sv0t0 = (item_kind == 3);
  return _sv0t0;
}

static int is_enum_item(int item_kind) {
  int _sv0t0 = (item_kind == 4);
  return _sv0t0;
}

static int count_type_registrations(int k0, int k1, int k2, int k3) {
  int count = 0;
  int _sv0t0 = item_registers_type(k0);
  if (_sv0t0) {
    count = (count + 1);
  } else {
  }
  int _sv0t1 = item_registers_type(k1);
  if (_sv0t1) {
    count = (count + 1);
  } else {
  }
  int _sv0t2 = item_registers_type(k2);
  if (_sv0t2) {
    count = (count + 1);
  } else {
  }
  int _sv0t3 = item_registers_type(k3);
  if (_sv0t3) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int struct_field_ty_resolves(int field_ty_tag) {
  int _sv0t0 = (field_ty_tag >= 0);
  return _sv0t0;
}

static int enum_variant_count(int v0, int v1, int v2, int v3) {
  int count = 0;
  if (v0) {
    count = (count + 1);
  } else {
  }
  if (v1) {
    count = (count + 1);
  } else {
  }
  if (v2) {
    count = (count + 1);
  } else {
  }
  if (v3) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int init_order_matters(void) {
  return 1;
}

int main(void) {
  int _sv0t0 = item_registers_type(3);
  if ((!_sv0t0)) {
    return 1;
  } else {
  }
  int _sv0t1 = item_registers_type(4);
  if ((!_sv0t1)) {
    return 2;
  } else {
  }
  int _sv0t2 = item_registers_type(0);
  if (_sv0t2) {
    return 3;
  } else {
  }
  int _sv0t3 = item_registers_type(1);
  if (_sv0t3) {
    return 4;
  } else {
  }
  int _sv0t4 = is_struct_item(3);
  if ((!_sv0t4)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_struct_item(4);
  if (_sv0t5) {
    return 6;
  } else {
  }
  int _sv0t6 = is_enum_item(4);
  if ((!_sv0t6)) {
    return 7;
  } else {
  }
  int _sv0t7 = count_type_registrations(0, 3, 4, 1);
  if ((_sv0t7 != 2)) {
    return 8;
  } else {
  }
  int _sv0t8 = count_type_registrations(3, 3, 3, 4);
  if ((_sv0t8 != 4)) {
    return 9;
  } else {
  }
  int _sv0t9 = struct_field_ty_resolves(3);
  if ((!_sv0t9)) {
    return 10;
  } else {
  }
  int _sv0t10 = (-1);
  int _sv0t11 = struct_field_ty_resolves(_sv0t10);
  if (_sv0t11) {
    return 11;
  } else {
  }
  int _sv0t12 = enum_variant_count(1, 1, 0, 0);
  if ((_sv0t12 != 2)) {
    return 12;
  } else {
  }
  int _sv0t13 = enum_variant_count(1, 1, 1, 1);
  if ((_sv0t13 != 4)) {
    return 13;
  } else {
  }
  int _sv0t14 = init_order_matters();
  if ((!_sv0t14)) {
    return 14;
  } else {
  }
  return 0;
}

