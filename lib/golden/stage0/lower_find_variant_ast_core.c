#include "sv0_runtime.h"

static int fva_err_unknown_enum(void);
static int fva_err_unknown_variant(void);
static int variant_shape_unit(void);
static int variant_shape_tuple(void);
static int variant_shape_struct(void);
static int find_variant_ast_shape(int enum_ast_hit, int variant_ast_hit, int shape_if_ok);

static int fva_err_unknown_enum(void) {
  return 255;
}

static int fva_err_unknown_variant(void) {
  return 254;
}

static int variant_shape_unit(void) {
  return 1;
}

static int variant_shape_tuple(void) {
  return 2;
}

static int variant_shape_struct(void) {
  return 3;
}

static int find_variant_ast_shape(int enum_ast_hit, int variant_ast_hit, int shape_if_ok) {
  int _sv0t0;
  int _sv0t1;
  if ((enum_ast_hit == 0)) {
    int _sv0t2 = fva_err_unknown_enum();
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((variant_ast_hit == 0)) {
      int _sv0t4 = fva_err_unknown_variant();
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      return shape_if_ok;
      _sv0t3 = 0;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = variant_shape_unit();
  int _sv0t1 = find_variant_ast_shape(0, 0, _sv0t0);
  int _sv0t2 = fva_err_unknown_enum();
  if ((_sv0t1 != _sv0t2)) {
    return 1;
  } else {
  }
  int _sv0t3 = variant_shape_tuple();
  int _sv0t4 = find_variant_ast_shape(0, 1, _sv0t3);
  int _sv0t5 = fva_err_unknown_enum();
  if ((_sv0t4 != _sv0t5)) {
    return 1;
  } else {
  }
  int _sv0t6 = variant_shape_unit();
  int _sv0t7 = find_variant_ast_shape(1, 0, _sv0t6);
  int _sv0t8 = fva_err_unknown_variant();
  if ((_sv0t7 != _sv0t8)) {
    return 1;
  } else {
  }
  int _sv0t9 = variant_shape_unit();
  int _sv0t10 = find_variant_ast_shape(1, 1, _sv0t9);
  int _sv0t11 = variant_shape_unit();
  if ((_sv0t10 != _sv0t11)) {
    return 1;
  } else {
  }
  int _sv0t12 = variant_shape_tuple();
  int _sv0t13 = find_variant_ast_shape(1, 1, _sv0t12);
  int _sv0t14 = variant_shape_tuple();
  if ((_sv0t13 != _sv0t14)) {
    return 1;
  } else {
  }
  int _sv0t15 = variant_shape_struct();
  int _sv0t16 = find_variant_ast_shape(1, 1, _sv0t15);
  int _sv0t17 = variant_shape_struct();
  if ((_sv0t16 != _sv0t17)) {
    return 1;
  } else {
  }
  return 0;
}

