#include "sv0_runtime.h"

typedef struct {
  int mv0;
  int mv1;
  int mn;
  int d;
  int i0;
  int i1;
  int in_;
  int o0;
  int o1;
  int on;
  int an;
  int an0;
  int aa0;
  int an1;
  int aa1;
} FullEnvCall;

static FullEnvCall ec_empty(void);
static FullEnvCall ec_mod_register(FullEnvCall e, int id);
static int ec_mod_lookup(FullEnvCall e, int id);
static int ec_frame_has(int f_u0, int f_u1, int f_n, int id);
static int ec_locals_lookup(FullEnvCall e, int id);
static int ec_lookup_value(FullEnvCall e, int id);
static FullEnvCall ec_scope_bind(FullEnvCall e, int id);
static FullEnvCall ec_scope_enter(FullEnvCall e);
static FullEnvCall ec_arity_register(FullEnvCall e, int name_id, int arity);
static int ec_lookup_fn_arity(FullEnvCall e, int name_id);
static int ec_expr_call_ok(FullEnvCall e, int fn_id, int argc);

static FullEnvCall ec_empty(void) {
  FullEnvCall _sv0t0;
  _sv0t0.mv0 = 0;
  _sv0t0.mv1 = 0;
  _sv0t0.mn = 0;
  _sv0t0.d = 0;
  _sv0t0.i0 = 0;
  _sv0t0.i1 = 0;
  _sv0t0.in_ = 0;
  _sv0t0.o0 = 0;
  _sv0t0.o1 = 0;
  _sv0t0.on = 0;
  _sv0t0.an = 0;
  _sv0t0.an0 = 0;
  _sv0t0.aa0 = 0;
  _sv0t0.an1 = 0;
  _sv0t0.aa1 = 0;
  return _sv0t0;
}

static FullEnvCall ec_mod_register(FullEnvCall e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.mn == 0)) {
    FullEnvCall _sv0t2;
    _sv0t2.mv0 = id;
    _sv0t2.mv1 = e.mv1;
    _sv0t2.mn = 1;
    _sv0t2.d = e.d;
    _sv0t2.i0 = e.i0;
    _sv0t2.i1 = e.i1;
    _sv0t2.in_ = e.in_;
    _sv0t2.o0 = e.o0;
    _sv0t2.o1 = e.o1;
    _sv0t2.on = e.on;
    _sv0t2.an = e.an;
    _sv0t2.an0 = e.an0;
    _sv0t2.aa0 = e.aa0;
    _sv0t2.an1 = e.an1;
    _sv0t2.aa1 = e.aa1;
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((e.mn == 1)) {
      FullEnvCall _sv0t4;
      _sv0t4.mv0 = e.mv0;
      _sv0t4.mv1 = id;
      _sv0t4.mn = 2;
      _sv0t4.d = e.d;
      _sv0t4.i0 = e.i0;
      _sv0t4.i1 = e.i1;
      _sv0t4.in_ = e.in_;
      _sv0t4.o0 = e.o0;
      _sv0t4.o1 = e.o1;
      _sv0t4.on = e.on;
      _sv0t4.an = e.an;
      _sv0t4.an0 = e.an0;
      _sv0t4.aa0 = e.aa0;
      _sv0t4.an1 = e.an1;
      _sv0t4.aa1 = e.aa1;
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      return e;
      _sv0t3 = 0;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ec_mod_lookup(FullEnvCall e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.mn >= 1)) {
    int _sv0t2;
    if ((e.mv0 == id)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.mn >= 2)) {
        int _sv0t4;
        if ((e.mv1 == id)) {
          return 1;
          _sv0t4 = 0;
        } else {
          return 0;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        return 0;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ec_frame_has(int f_u0, int f_u1, int f_n, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((f_n >= 1)) {
    int _sv0t2;
    if ((f_u0 == id)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((f_n >= 2)) {
        int _sv0t4;
        if ((f_u1 == id)) {
          return 1;
          _sv0t4 = 0;
        } else {
          return 0;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        return 0;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ec_locals_lookup(FullEnvCall e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.d >= 1)) {
    int _sv0t3 = ec_frame_has(e.i0, e.i1, e.in_, id);
    int _sv0t2;
    if ((_sv0t3 == 1)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t4;
      if ((e.d >= 2)) {
        int _sv0t5 = ec_frame_has(e.o0, e.o1, e.on, id);
        return _sv0t5;
        _sv0t4 = 0;
      } else {
        return 0;
        _sv0t4 = 0;
      }
      _sv0t2 = _sv0t4;
    }
    _sv0t1 = _sv0t2;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ec_lookup_value(FullEnvCall e, int id) {
  int _sv0t2 = ec_locals_lookup(e, id);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t3 = ec_mod_lookup(e, id);
    return _sv0t3;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static FullEnvCall ec_scope_bind(FullEnvCall e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.d == 0)) {
    FullEnvCall _sv0t2;
    _sv0t2.mv0 = e.mv0;
    _sv0t2.mv1 = e.mv1;
    _sv0t2.mn = e.mn;
    _sv0t2.d = 1;
    _sv0t2.i0 = id;
    _sv0t2.i1 = 0;
    _sv0t2.in_ = 1;
    _sv0t2.o0 = 0;
    _sv0t2.o1 = 0;
    _sv0t2.on = 0;
    _sv0t2.an = e.an;
    _sv0t2.an0 = e.an0;
    _sv0t2.aa0 = e.aa0;
    _sv0t2.an1 = e.an1;
    _sv0t2.aa1 = e.aa1;
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((e.d == 1)) {
      int _sv0t4;
      if ((e.in_ == 0)) {
        FullEnvCall _sv0t5;
        _sv0t5.mv0 = e.mv0;
        _sv0t5.mv1 = e.mv1;
        _sv0t5.mn = e.mn;
        _sv0t5.d = 1;
        _sv0t5.i0 = id;
        _sv0t5.i1 = 0;
        _sv0t5.in_ = 1;
        _sv0t5.o0 = 0;
        _sv0t5.o1 = 0;
        _sv0t5.on = 0;
        _sv0t5.an = e.an;
        _sv0t5.an0 = e.an0;
        _sv0t5.aa0 = e.aa0;
        _sv0t5.an1 = e.an1;
        _sv0t5.aa1 = e.aa1;
        return _sv0t5;
        _sv0t4 = 0;
      } else {
        int _sv0t6;
        if ((e.in_ == 1)) {
          FullEnvCall _sv0t7;
          _sv0t7.mv0 = e.mv0;
          _sv0t7.mv1 = e.mv1;
          _sv0t7.mn = e.mn;
          _sv0t7.d = 1;
          _sv0t7.i0 = e.i0;
          _sv0t7.i1 = id;
          _sv0t7.in_ = 2;
          _sv0t7.o0 = 0;
          _sv0t7.o1 = 0;
          _sv0t7.on = 0;
          _sv0t7.an = e.an;
          _sv0t7.an0 = e.an0;
          _sv0t7.aa0 = e.aa0;
          _sv0t7.an1 = e.an1;
          _sv0t7.aa1 = e.aa1;
          return _sv0t7;
          _sv0t6 = 0;
        } else {
          return e;
          _sv0t6 = 0;
        }
        _sv0t4 = _sv0t6;
      }
      _sv0t3 = _sv0t4;
    } else {
      int _sv0t8;
      if ((e.in_ == 0)) {
        FullEnvCall _sv0t9;
        _sv0t9.mv0 = e.mv0;
        _sv0t9.mv1 = e.mv1;
        _sv0t9.mn = e.mn;
        _sv0t9.d = 2;
        _sv0t9.i0 = id;
        _sv0t9.i1 = 0;
        _sv0t9.in_ = 1;
        _sv0t9.o0 = e.o0;
        _sv0t9.o1 = e.o1;
        _sv0t9.on = e.on;
        _sv0t9.an = e.an;
        _sv0t9.an0 = e.an0;
        _sv0t9.aa0 = e.aa0;
        _sv0t9.an1 = e.an1;
        _sv0t9.aa1 = e.aa1;
        return _sv0t9;
        _sv0t8 = 0;
      } else {
        int _sv0t10;
        if ((e.in_ == 1)) {
          FullEnvCall _sv0t11;
          _sv0t11.mv0 = e.mv0;
          _sv0t11.mv1 = e.mv1;
          _sv0t11.mn = e.mn;
          _sv0t11.d = 2;
          _sv0t11.i0 = e.i0;
          _sv0t11.i1 = id;
          _sv0t11.in_ = 2;
          _sv0t11.o0 = e.o0;
          _sv0t11.o1 = e.o1;
          _sv0t11.on = e.on;
          _sv0t11.an = e.an;
          _sv0t11.an0 = e.an0;
          _sv0t11.aa0 = e.aa0;
          _sv0t11.an1 = e.an1;
          _sv0t11.aa1 = e.aa1;
          return _sv0t11;
          _sv0t10 = 0;
        } else {
          return e;
          _sv0t10 = 0;
        }
        _sv0t8 = _sv0t10;
      }
      _sv0t3 = _sv0t8;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static FullEnvCall ec_scope_enter(FullEnvCall e) {
  int _sv0t0;
  int _sv0t1;
  if ((e.d == 0)) {
    FullEnvCall _sv0t2;
    _sv0t2.mv0 = e.mv0;
    _sv0t2.mv1 = e.mv1;
    _sv0t2.mn = e.mn;
    _sv0t2.d = 1;
    _sv0t2.i0 = 0;
    _sv0t2.i1 = 0;
    _sv0t2.in_ = 0;
    _sv0t2.o0 = 0;
    _sv0t2.o1 = 0;
    _sv0t2.on = 0;
    _sv0t2.an = e.an;
    _sv0t2.an0 = e.an0;
    _sv0t2.aa0 = e.aa0;
    _sv0t2.an1 = e.an1;
    _sv0t2.aa1 = e.aa1;
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((e.d == 1)) {
      FullEnvCall _sv0t4;
      _sv0t4.mv0 = e.mv0;
      _sv0t4.mv1 = e.mv1;
      _sv0t4.mn = e.mn;
      _sv0t4.d = 2;
      _sv0t4.i0 = 0;
      _sv0t4.i1 = 0;
      _sv0t4.in_ = 0;
      _sv0t4.o0 = e.i0;
      _sv0t4.o1 = e.i1;
      _sv0t4.on = e.in_;
      _sv0t4.an = e.an;
      _sv0t4.an0 = e.an0;
      _sv0t4.aa0 = e.aa0;
      _sv0t4.an1 = e.an1;
      _sv0t4.aa1 = e.aa1;
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      return e;
      _sv0t3 = 0;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static FullEnvCall ec_arity_register(FullEnvCall e, int name_id, int arity) {
  int _sv0t0;
  int _sv0t1;
  if ((e.an >= 1)) {
    int _sv0t2;
    if ((e.an0 == name_id)) {
      return e;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.an >= 2)) {
        int _sv0t4;
        if ((e.an1 == name_id)) {
          return e;
          _sv0t4 = 0;
        } else {
          return e;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        FullEnvCall _sv0t5;
        _sv0t5.mv0 = e.mv0;
        _sv0t5.mv1 = e.mv1;
        _sv0t5.mn = e.mn;
        _sv0t5.d = e.d;
        _sv0t5.i0 = e.i0;
        _sv0t5.i1 = e.i1;
        _sv0t5.in_ = e.in_;
        _sv0t5.o0 = e.o0;
        _sv0t5.o1 = e.o1;
        _sv0t5.on = e.on;
        _sv0t5.an = 2;
        _sv0t5.an0 = e.an0;
        _sv0t5.aa0 = e.aa0;
        _sv0t5.an1 = name_id;
        _sv0t5.aa1 = arity;
        return _sv0t5;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    FullEnvCall _sv0t6;
    _sv0t6.mv0 = e.mv0;
    _sv0t6.mv1 = e.mv1;
    _sv0t6.mn = e.mn;
    _sv0t6.d = e.d;
    _sv0t6.i0 = e.i0;
    _sv0t6.i1 = e.i1;
    _sv0t6.in_ = e.in_;
    _sv0t6.o0 = e.o0;
    _sv0t6.o1 = e.o1;
    _sv0t6.on = e.on;
    _sv0t6.an = 1;
    _sv0t6.an0 = name_id;
    _sv0t6.aa0 = arity;
    _sv0t6.an1 = 0;
    _sv0t6.aa1 = 0;
    return _sv0t6;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ec_lookup_fn_arity(FullEnvCall e, int name_id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.an >= 1)) {
    int _sv0t2;
    if ((e.an0 == name_id)) {
      return e.aa0;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.an >= 2)) {
        int _sv0t4;
        if ((e.an1 == name_id)) {
          return e.aa1;
          _sv0t4 = 0;
        } else {
          int _sv0t5 = (-1);
          return _sv0t5;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        int _sv0t6 = (-1);
        return _sv0t6;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    int _sv0t7 = (-1);
    return _sv0t7;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ec_expr_call_ok(FullEnvCall e, int fn_id, int argc) {
  int _sv0t2 = ec_lookup_value(e, fn_id);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 0)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t3 = ec_lookup_fn_arity(e, fn_id);
    int ar = _sv0t3;
    int _sv0t4;
    if ((ar < 0)) {
      return 1;
      _sv0t4 = 0;
    } else {
      int _sv0t5;
      if ((ar == argc)) {
        return 1;
        _sv0t5 = 0;
      } else {
        return 0;
        _sv0t5 = 0;
      }
      _sv0t4 = _sv0t5;
    }
    _sv0t1 = _sv0t4;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  FullEnvCall _sv0t0 = ec_empty();
  FullEnvCall e0;
  e0 = _sv0t0;
  FullEnvCall _sv0t1 = ec_mod_register(e0, 5);
  FullEnvCall e1;
  e1 = _sv0t1;
  FullEnvCall _sv0t2 = ec_mod_register(e1, 6);
  FullEnvCall e2;
  e2 = _sv0t2;
  FullEnvCall _sv0t3 = ec_arity_register(e2, 5, 2);
  FullEnvCall e3;
  e3 = _sv0t3;
  FullEnvCall _sv0t4 = ec_arity_register(e3, 6, 0);
  FullEnvCall e4;
  e4 = _sv0t4;
  int _sv0t5 = ec_expr_call_ok(e4, 5, 2);
  int f_ok = (1 - _sv0t5);
  int _sv0t6 = ec_expr_call_ok(e4, 5, 3);
  int f_bad = _sv0t6;
  int _sv0t7 = ec_expr_call_ok(e4, 6, 0);
  int f_null = (1 - _sv0t7);
  int _sv0t8 = ec_expr_call_ok(e4, 99, 0);
  int f_no_val = _sv0t8;
  FullEnvCall _sv0t9 = ec_scope_enter(e4);
  FullEnvCall e5;
  e5 = _sv0t9;
  FullEnvCall _sv0t10 = ec_scope_bind(e5, 5);
  FullEnvCall e6;
  e6 = _sv0t10;
  int _sv0t11 = ec_expr_call_ok(e6, 5, 2);
  int f_shadow = (1 - _sv0t11);
  int _sv0t12 = ec_expr_call_ok(e6, 5, 1);
  int f_shadow_bad = _sv0t12;
  int _sv0t13 = (f_ok + f_bad);
  int _sv0t14 = (_sv0t13 + f_null);
  int _sv0t15 = (_sv0t14 + f_no_val);
  int _sv0t16 = (_sv0t15 + f_shadow);
  int _sv0t17 = (_sv0t16 + f_shadow_bad);
  return _sv0t17;
}

