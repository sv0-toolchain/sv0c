#include "sv0_runtime.h"

static int sf_err_unknown_struct(void);
static int struct_fields_field_count(int struct_table_hit, int field_count_if_hit);

static int sf_err_unknown_struct(void) {
  return 255;
}

static int struct_fields_field_count(int struct_table_hit, int field_count_if_hit) {
  int _sv0t0;
  int _sv0t1;
  if ((struct_table_hit == 0)) {
    int _sv0t2 = sf_err_unknown_struct();
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    return field_count_if_hit;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = struct_fields_field_count(0, 0);
  int _sv0t1 = sf_err_unknown_struct();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = struct_fields_field_count(0, 99);
  int _sv0t3 = sf_err_unknown_struct();
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = struct_fields_field_count(1, 0);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = struct_fields_field_count(1, 1);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = struct_fields_field_count(1, 12);
  if ((_sv0t6 != 12)) {
    return 1;
  } else {
  }
  return 0;
}

