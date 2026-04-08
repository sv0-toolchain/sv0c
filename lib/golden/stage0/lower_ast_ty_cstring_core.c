#include "sv0_runtime.h"

static int c_ty_void(void);
static int c_ty_int(void);
static int c_ty_int8(void);
static int c_ty_uint8(void);
static int c_ty_int16(void);
static int c_ty_uint16(void);
static int c_ty_int64(void);
static int c_ty_uint64(void);
static int c_ty_intptr(void);
static int c_ty_uintptr(void);
static int c_ty_passthru_name(void);
static int ast_ty_cstring_class(int ty_shape_tag);

static int c_ty_void(void) {
  return 1;
}

static int c_ty_int(void) {
  return 2;
}

static int c_ty_int8(void) {
  return 3;
}

static int c_ty_uint8(void) {
  return 4;
}

static int c_ty_int16(void) {
  return 5;
}

static int c_ty_uint16(void) {
  return 6;
}

static int c_ty_int64(void) {
  return 7;
}

static int c_ty_uint64(void) {
  return 8;
}

static int c_ty_intptr(void) {
  return 9;
}

static int c_ty_uintptr(void) {
  return 10;
}

static int c_ty_passthru_name(void) {
  return 11;
}

static int ast_ty_cstring_class(int ty_shape_tag) {
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
        int _sv0t7;
        if ((ty_shape_tag == 3)) {
          int _sv0t8 = c_ty_int();
          return _sv0t8;
          _sv0t7 = 0;
        } else {
          int _sv0t9;
          if ((ty_shape_tag == 4)) {
            int _sv0t10 = c_ty_int8();
            return _sv0t10;
            _sv0t9 = 0;
          } else {
            int _sv0t11;
            if ((ty_shape_tag == 5)) {
              int _sv0t12 = c_ty_uint8();
              return _sv0t12;
              _sv0t11 = 0;
            } else {
              int _sv0t13;
              if ((ty_shape_tag == 6)) {
                int _sv0t14 = c_ty_int16();
                return _sv0t14;
                _sv0t13 = 0;
              } else {
                int _sv0t15;
                if ((ty_shape_tag == 7)) {
                  int _sv0t16 = c_ty_uint16();
                  return _sv0t16;
                  _sv0t15 = 0;
                } else {
                  int _sv0t17;
                  if ((ty_shape_tag == 8)) {
                    int _sv0t18 = c_ty_int64();
                    return _sv0t18;
                    _sv0t17 = 0;
                  } else {
                    int _sv0t19;
                    if ((ty_shape_tag == 9)) {
                      int _sv0t20 = c_ty_uint64();
                      return _sv0t20;
                      _sv0t19 = 0;
                    } else {
                      int _sv0t21;
                      if ((ty_shape_tag == 10)) {
                        int _sv0t22 = c_ty_intptr();
                        return _sv0t22;
                        _sv0t21 = 0;
                      } else {
                        int _sv0t23;
                        if ((ty_shape_tag == 11)) {
                          int _sv0t24 = c_ty_uintptr();
                          return _sv0t24;
                          _sv0t23 = 0;
                        } else {
                          int _sv0t25;
                          if ((ty_shape_tag == 12)) {
                            int _sv0t26 = c_ty_int();
                            return _sv0t26;
                            _sv0t25 = 0;
                          } else {
                            int _sv0t27;
                            if ((ty_shape_tag == 13)) {
                              int _sv0t28 = c_ty_passthru_name();
                              return _sv0t28;
                              _sv0t27 = 0;
                            } else {
                              int _sv0t29 = c_ty_int();
                              return _sv0t29;
                              _sv0t27 = 0;
                            }
                            _sv0t25 = _sv0t27;
                          }
                          _sv0t23 = _sv0t25;
                        }
                        _sv0t21 = _sv0t23;
                      }
                      _sv0t19 = _sv0t21;
                    }
                    _sv0t17 = _sv0t19;
                  }
                  _sv0t15 = _sv0t17;
                }
                _sv0t13 = _sv0t15;
              }
              _sv0t11 = _sv0t13;
            }
            _sv0t9 = _sv0t11;
          }
          _sv0t7 = _sv0t9;
        }
        _sv0t5 = _sv0t7;
      }
      _sv0t3 = _sv0t5;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = ast_ty_cstring_class(0);
  int _sv0t1 = c_ty_void();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = ast_ty_cstring_class(1);
  int _sv0t3 = c_ty_void();
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = ast_ty_cstring_class(2);
  int _sv0t5 = c_ty_int();
  if ((_sv0t4 != _sv0t5)) {
    return 1;
  } else {
  }
  int _sv0t6 = ast_ty_cstring_class(3);
  int _sv0t7 = c_ty_int();
  if ((_sv0t6 != _sv0t7)) {
    return 1;
  } else {
  }
  int _sv0t8 = ast_ty_cstring_class(4);
  int _sv0t9 = c_ty_int8();
  if ((_sv0t8 != _sv0t9)) {
    return 1;
  } else {
  }
  int _sv0t10 = ast_ty_cstring_class(5);
  int _sv0t11 = c_ty_uint8();
  if ((_sv0t10 != _sv0t11)) {
    return 1;
  } else {
  }
  int _sv0t12 = ast_ty_cstring_class(6);
  int _sv0t13 = c_ty_int16();
  if ((_sv0t12 != _sv0t13)) {
    return 1;
  } else {
  }
  int _sv0t14 = ast_ty_cstring_class(7);
  int _sv0t15 = c_ty_uint16();
  if ((_sv0t14 != _sv0t15)) {
    return 1;
  } else {
  }
  int _sv0t16 = ast_ty_cstring_class(8);
  int _sv0t17 = c_ty_int64();
  if ((_sv0t16 != _sv0t17)) {
    return 1;
  } else {
  }
  int _sv0t18 = ast_ty_cstring_class(9);
  int _sv0t19 = c_ty_uint64();
  if ((_sv0t18 != _sv0t19)) {
    return 1;
  } else {
  }
  int _sv0t20 = ast_ty_cstring_class(10);
  int _sv0t21 = c_ty_intptr();
  if ((_sv0t20 != _sv0t21)) {
    return 1;
  } else {
  }
  int _sv0t22 = ast_ty_cstring_class(11);
  int _sv0t23 = c_ty_uintptr();
  if ((_sv0t22 != _sv0t23)) {
    return 1;
  } else {
  }
  int _sv0t24 = ast_ty_cstring_class(12);
  int _sv0t25 = c_ty_int();
  if ((_sv0t24 != _sv0t25)) {
    return 1;
  } else {
  }
  int _sv0t26 = ast_ty_cstring_class(13);
  int _sv0t27 = c_ty_passthru_name();
  if ((_sv0t26 != _sv0t27)) {
    return 1;
  } else {
  }
  int _sv0t28 = ast_ty_cstring_class(99);
  int _sv0t29 = c_ty_int();
  if ((_sv0t28 != _sv0t29)) {
    return 1;
  } else {
  }
  return 0;
}

