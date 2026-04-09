#include "sv0_runtime.h"

static int opc_err_unknown_enum(void);
static int opc_err_no_ok_variant(void);
static int opc_err_ok_not_tuple_one(void);
static int variant_shape_unit(void);
static int variant_shape_tuple(void);
static int c_ty_void(void);
static int c_ty_int(void);
static int payload_ast_ty_cstring_class(int ty_shape_tag);
static int ok_payload_cty_class(int enum_ast_hit, int ok_variant_hit, int variant_shape, int tuple_arity, int payload_ty_shape_tag);

static int opc_err_unknown_enum(void) {
  return 255;
}

static int opc_err_no_ok_variant(void) {
  return 254;
}

static int opc_err_ok_not_tuple_one(void) {
  return 253;
}

static int variant_shape_unit(void) {
  return 1;
}

static int variant_shape_tuple(void) {
  return 2;
}

static int c_ty_void(void) {
  return 1;
}

static int c_ty_int(void) {
  return 2;
}

static int payload_ast_ty_cstring_class(int ty_shape_tag) {
  int _sv0t0;
  int _sv0t1;
  if ((ty_shape_tag == 0)) {
    int _sv0t2 = c_ty_void();
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((ty_shape_tag == 1)) {
      int _sv0t4 = c_ty_void();
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      int _sv0t5;
      if ((ty_shape_tag == 2)) {
        int _sv0t6 = c_ty_int();
        return _sv0t6;
        _sv0t5 = 0;
      } else {
        int _sv0t7 = c_ty_int();
        return _sv0t7;
        _sv0t5 = 0;
      }
      _sv0t3 = _sv0t5;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ok_payload_cty_class(int enum_ast_hit, int ok_variant_hit, int variant_shape, int tuple_arity, int payload_ty_shape_tag) {
  int _sv0t0;
  int _sv0t1;
  if ((enum_ast_hit == 0)) {
    int _sv0t2 = opc_err_unknown_enum();
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((ok_variant_hit == 0)) {
      int _sv0t4 = opc_err_no_ok_variant();
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      int _sv0t6 = variant_shape_tuple();
      int _sv0t5;
      if ((variant_shape != _sv0t6)) {
        int _sv0t7 = opc_err_ok_not_tuple_one();
        return _sv0t7;
        _sv0t5 = 0;
      } else {
        int _sv0t8;
        if ((tuple_arity != 1)) {
          int _sv0t9 = opc_err_ok_not_tuple_one();
          return _sv0t9;
          _sv0t8 = 0;
        } else {
          int _sv0t10 = payload_ast_ty_cstring_class(payload_ty_shape_tag);
          return _sv0t10;
          _sv0t8 = 0;
        }
        _sv0t5 = _sv0t8;
      }
      _sv0t3 = _sv0t5;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = variant_shape_tuple();
  int _sv0t1 = ok_payload_cty_class(0, 0, _sv0t0, 1, 2);
  int _sv0t2 = opc_err_unknown_enum();
  if ((_sv0t1 != _sv0t2)) {
    return 1;
  } else {
  }
  int _sv0t3 = variant_shape_tuple();
  int _sv0t4 = ok_payload_cty_class(1, 0, _sv0t3, 1, 2);
  int _sv0t5 = opc_err_no_ok_variant();
  if ((_sv0t4 != _sv0t5)) {
    return 1;
  } else {
  }
  int _sv0t6 = variant_shape_unit();
  int _sv0t7 = ok_payload_cty_class(1, 1, _sv0t6, 1, 2);
  int _sv0t8 = opc_err_ok_not_tuple_one();
  if ((_sv0t7 != _sv0t8)) {
    return 1;
  } else {
  }
  int _sv0t9 = variant_shape_tuple();
  int _sv0t10 = ok_payload_cty_class(1, 1, _sv0t9, 2, 2);
  int _sv0t11 = opc_err_ok_not_tuple_one();
  if ((_sv0t10 != _sv0t11)) {
    return 1;
  } else {
  }
  int _sv0t12 = variant_shape_tuple();
  int _sv0t13 = ok_payload_cty_class(1, 1, _sv0t12, 1, 2);
  int _sv0t14 = c_ty_int();
  if ((_sv0t13 != _sv0t14)) {
    return 1;
  } else {
  }
  int _sv0t15 = variant_shape_tuple();
  int _sv0t16 = ok_payload_cty_class(1, 1, _sv0t15, 1, 0);
  int _sv0t17 = c_ty_void();
  if ((_sv0t16 != _sv0t17)) {
    return 1;
  } else {
  }
  return 0;
}

