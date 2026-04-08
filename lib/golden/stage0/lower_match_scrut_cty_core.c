#include "sv0_runtime.h"

static int msc_int_fallback(void);
static int msc_enum_ty(void);
static int msc_callee_ret(void);
static int param_ty_cstring_class(int ty_shape_tag);
static int match_scrut_cty_class(int shape, int path_aux, int ty_slot);

static int msc_int_fallback(void) {
  return 1;
}

static int msc_enum_ty(void) {
  return 3;
}

static int msc_callee_ret(void) {
  return 4;
}

static int param_ty_cstring_class(int ty_shape_tag) {
  int _sv0t0;
  int _sv0t1;
  if ((ty_shape_tag == 0)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((ty_shape_tag == 1)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((ty_shape_tag == 2)) {
        return 2;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((ty_shape_tag == 3)) {
          return 2;
          _sv0t4 = 0;
        } else {
          int _sv0t5;
          if ((ty_shape_tag == 13)) {
            return 11;
            _sv0t5 = 0;
          } else {
            return 2;
            _sv0t5 = 0;
          }
          _sv0t4 = _sv0t5;
        }
        _sv0t3 = _sv0t4;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int match_scrut_cty_class(int shape, int path_aux, int ty_slot) {
  int _sv0t0;
  int _sv0t1;
  if ((shape == 1)) {
    int _sv0t2 = msc_enum_ty();
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((shape == 2)) {
      int _sv0t4;
      if ((path_aux == 0)) {
        int _sv0t5 = msc_int_fallback();
        return _sv0t5;
        _sv0t4 = 0;
      } else {
        int _sv0t6;
        if ((path_aux == 1)) {
          return ty_slot;
          _sv0t6 = 0;
        } else {
          int _sv0t7;
          if ((path_aux == 2)) {
            int _sv0t8 = param_ty_cstring_class(ty_slot);
            return _sv0t8;
            _sv0t7 = 0;
          } else {
            int _sv0t9 = msc_int_fallback();
            return _sv0t9;
            _sv0t7 = 0;
          }
          _sv0t6 = _sv0t7;
        }
        _sv0t4 = _sv0t6;
      }
      _sv0t3 = _sv0t4;
    } else {
      int _sv0t10;
      if ((shape == 3)) {
        int _sv0t11 = msc_callee_ret();
        return _sv0t11;
        _sv0t10 = 0;
      } else {
        int _sv0t12;
        if ((shape == 4)) {
          int _sv0t13 = match_scrut_cty_class(path_aux, ty_slot, 0);
          return _sv0t13;
          _sv0t12 = 0;
        } else {
          int _sv0t14;
          if ((shape == 5)) {
            int _sv0t15 = msc_int_fallback();
            return _sv0t15;
            _sv0t14 = 0;
          } else {
            int _sv0t16;
            if ((shape == 6)) {
              int _sv0t17 = msc_int_fallback();
              return _sv0t17;
              _sv0t16 = 0;
            } else {
              int _sv0t18 = msc_int_fallback();
              return _sv0t18;
              _sv0t16 = 0;
            }
            _sv0t14 = _sv0t16;
          }
          _sv0t12 = _sv0t14;
        }
        _sv0t10 = _sv0t12;
      }
      _sv0t3 = _sv0t10;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = match_scrut_cty_class(1, 0, 0);
  int _sv0t1 = msc_enum_ty();
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = match_scrut_cty_class(2, 0, 0);
  int _sv0t3 = msc_int_fallback();
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = match_scrut_cty_class(2, 1, 42);
  if ((_sv0t4 != 42)) {
    return 1;
  } else {
  }
  int _sv0t5 = match_scrut_cty_class(2, 2, 2);
  if ((_sv0t5 != 2)) {
    return 1;
  } else {
  }
  int _sv0t6 = match_scrut_cty_class(2, 2, 0);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = match_scrut_cty_class(3, 0, 0);
  int _sv0t8 = msc_callee_ret();
  if ((_sv0t7 != _sv0t8)) {
    return 1;
  } else {
  }
  int _sv0t9 = match_scrut_cty_class(4, 1, 0);
  int _sv0t10 = msc_enum_ty();
  if ((_sv0t9 != _sv0t10)) {
    return 1;
  } else {
  }
  int _sv0t11 = match_scrut_cty_class(4, 2, 0);
  int _sv0t12 = msc_int_fallback();
  if ((_sv0t11 != _sv0t12)) {
    return 1;
  } else {
  }
  int _sv0t13 = match_scrut_cty_class(5, 0, 0);
  int _sv0t14 = msc_int_fallback();
  if ((_sv0t13 != _sv0t14)) {
    return 1;
  } else {
  }
  int _sv0t15 = match_scrut_cty_class(6, 0, 0);
  int _sv0t16 = msc_int_fallback();
  if ((_sv0t15 != _sv0t16)) {
    return 1;
  } else {
  }
  int _sv0t17 = match_scrut_cty_class(99, 0, 0);
  int _sv0t18 = msc_int_fallback();
  if ((_sv0t17 != _sv0t18)) {
    return 1;
  } else {
  }
  return 0;
}

