#include "sv0_runtime.h"

static int et_err_unknown_enum(void);
static int et_err_unknown_variant(void);
static int enum_tag_value(int enum_table_hit, int variant_table_hit, int discriminant_k);

static int et_err_unknown_enum(void) {
  return 255;
}

static int et_err_unknown_variant(void) {
  return 254;
}

static int enum_tag_value(int enum_table_hit, int variant_table_hit, int discriminant_k) {
  int _sv0t0;
  int _sv0t1;
  if ((enum_table_hit == 0)) {
    int _sv0t2 = et_err_unknown_enum();
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((variant_table_hit == 0)) {
      int _sv0t4 = et_err_unknown_variant();
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      return discriminant_k;
      _sv0t3 = 0;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = enum_tag_value(0, 0, 0);
  int _sv0t1 = et_err_unknown_enum();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = enum_tag_value(0, 1, 7);
  int _sv0t3 = et_err_unknown_enum();
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = enum_tag_value(1, 0, 0);
  int _sv0t5 = et_err_unknown_variant();
  if ((_sv0t4 != _sv0t5)) {
    return 1;
  } else {
  }
  int _sv0t6 = enum_tag_value(1, 1, 0);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  int _sv0t7 = enum_tag_value(1, 1, 42);
  if ((_sv0t7 != 42)) {
    return 1;
  } else {
  }
  return 0;
}

