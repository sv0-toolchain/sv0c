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
} FullEnv;

static FullEnv full_empty(void);
static FullEnv full_mod_register(FullEnv e, int id);
static int mod_lookup(FullEnv e, int id);
static int frame_has(int f_u0, int f_u1, int f_n, int id);
static int locals_lookup(FullEnv e, int id);
static int lookup_value(FullEnv e, int id);
static FullEnv full_scope_bind(FullEnv e, int id);
static FullEnv full_scope_enter(FullEnv e);
static FullEnv full_scope_exit(FullEnv e);

static FullEnv full_empty(void) {
  FullEnv _sv0t0;
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
  return _sv0t0;
}

static FullEnv full_mod_register(FullEnv e, int id) {
  FullEnv out;
  out = e;
  if ((e.mn == 0)) {
    FullEnv _sv0t0;
    _sv0t0.mv0 = id;
    _sv0t0.mv1 = e.mv1;
    _sv0t0.mn = 1;
    _sv0t0.d = e.d;
    _sv0t0.i0 = e.i0;
    _sv0t0.i1 = e.i1;
    _sv0t0.in_ = e.in_;
    _sv0t0.o0 = e.o0;
    _sv0t0.o1 = e.o1;
    _sv0t0.on = e.on;
    out = _sv0t0;
  } else {
    if ((e.mn == 1)) {
      FullEnv _sv0t1;
      _sv0t1.mv0 = e.mv0;
      _sv0t1.mv1 = id;
      _sv0t1.mn = 2;
      _sv0t1.d = e.d;
      _sv0t1.i0 = e.i0;
      _sv0t1.i1 = e.i1;
      _sv0t1.in_ = e.in_;
      _sv0t1.o0 = e.o0;
      _sv0t1.o1 = e.o1;
      _sv0t1.on = e.on;
      out = _sv0t1;
    } else {
    }
  }
  return out;
}

static int mod_lookup(FullEnv e, int id) {
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

static int frame_has(int f_u0, int f_u1, int f_n, int id) {
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

static int locals_lookup(FullEnv e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.d >= 1)) {
    int _sv0t3 = frame_has(e.i0, e.i1, e.in_, id);
    int _sv0t2;
    if ((_sv0t3 == 1)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t4;
      if ((e.d >= 2)) {
        int _sv0t5 = frame_has(e.o0, e.o1, e.on, id);
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

static int lookup_value(FullEnv e, int id) {
  int _sv0t2 = locals_lookup(e, id);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t3 = mod_lookup(e, id);
    return _sv0t3;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static FullEnv full_scope_bind(FullEnv e, int id) {
  FullEnv out;
  out = e;
  if ((e.d == 0)) {
    FullEnv _sv0t0;
    _sv0t0.mv0 = e.mv0;
    _sv0t0.mv1 = e.mv1;
    _sv0t0.mn = e.mn;
    _sv0t0.d = 1;
    _sv0t0.i0 = id;
    _sv0t0.i1 = 0;
    _sv0t0.in_ = 1;
    _sv0t0.o0 = 0;
    _sv0t0.o1 = 0;
    _sv0t0.on = 0;
    out = _sv0t0;
  } else {
    if ((e.d == 1)) {
      if ((e.in_ == 0)) {
        FullEnv _sv0t1;
        _sv0t1.mv0 = e.mv0;
        _sv0t1.mv1 = e.mv1;
        _sv0t1.mn = e.mn;
        _sv0t1.d = 1;
        _sv0t1.i0 = id;
        _sv0t1.i1 = 0;
        _sv0t1.in_ = 1;
        _sv0t1.o0 = 0;
        _sv0t1.o1 = 0;
        _sv0t1.on = 0;
        out = _sv0t1;
      } else {
        if ((e.in_ == 1)) {
          FullEnv _sv0t2;
          _sv0t2.mv0 = e.mv0;
          _sv0t2.mv1 = e.mv1;
          _sv0t2.mn = e.mn;
          _sv0t2.d = 1;
          _sv0t2.i0 = e.i0;
          _sv0t2.i1 = id;
          _sv0t2.in_ = 2;
          _sv0t2.o0 = 0;
          _sv0t2.o1 = 0;
          _sv0t2.on = 0;
          out = _sv0t2;
        } else {
        }
      }
    } else {
      if ((e.in_ == 0)) {
        FullEnv _sv0t3;
        _sv0t3.mv0 = e.mv0;
        _sv0t3.mv1 = e.mv1;
        _sv0t3.mn = e.mn;
        _sv0t3.d = 2;
        _sv0t3.i0 = id;
        _sv0t3.i1 = 0;
        _sv0t3.in_ = 1;
        _sv0t3.o0 = e.o0;
        _sv0t3.o1 = e.o1;
        _sv0t3.on = e.on;
        out = _sv0t3;
      } else {
        if ((e.in_ == 1)) {
          FullEnv _sv0t4;
          _sv0t4.mv0 = e.mv0;
          _sv0t4.mv1 = e.mv1;
          _sv0t4.mn = e.mn;
          _sv0t4.d = 2;
          _sv0t4.i0 = e.i0;
          _sv0t4.i1 = id;
          _sv0t4.in_ = 2;
          _sv0t4.o0 = e.o0;
          _sv0t4.o1 = e.o1;
          _sv0t4.on = e.on;
          out = _sv0t4;
        } else {
        }
      }
    }
  }
  return out;
}

static FullEnv full_scope_enter(FullEnv e) {
  FullEnv out;
  out = e;
  if ((e.d == 0)) {
    FullEnv _sv0t0;
    _sv0t0.mv0 = e.mv0;
    _sv0t0.mv1 = e.mv1;
    _sv0t0.mn = e.mn;
    _sv0t0.d = 1;
    _sv0t0.i0 = 0;
    _sv0t0.i1 = 0;
    _sv0t0.in_ = 0;
    _sv0t0.o0 = 0;
    _sv0t0.o1 = 0;
    _sv0t0.on = 0;
    out = _sv0t0;
  } else {
    if ((e.d == 1)) {
      FullEnv _sv0t1;
      _sv0t1.mv0 = e.mv0;
      _sv0t1.mv1 = e.mv1;
      _sv0t1.mn = e.mn;
      _sv0t1.d = 2;
      _sv0t1.i0 = 0;
      _sv0t1.i1 = 0;
      _sv0t1.in_ = 0;
      _sv0t1.o0 = e.i0;
      _sv0t1.o1 = e.i1;
      _sv0t1.on = e.in_;
      out = _sv0t1;
    } else {
    }
  }
  return out;
}

static FullEnv full_scope_exit(FullEnv e) {
  FullEnv out;
  out = e;
  if ((e.d == 2)) {
    FullEnv _sv0t0;
    _sv0t0.mv0 = e.mv0;
    _sv0t0.mv1 = e.mv1;
    _sv0t0.mn = e.mn;
    _sv0t0.d = 1;
    _sv0t0.i0 = e.o0;
    _sv0t0.i1 = e.o1;
    _sv0t0.in_ = e.on;
    _sv0t0.o0 = 0;
    _sv0t0.o1 = 0;
    _sv0t0.on = 0;
    out = _sv0t0;
  } else {
    if ((e.d == 1)) {
      FullEnv _sv0t1;
      _sv0t1.mv0 = e.mv0;
      _sv0t1.mv1 = e.mv1;
      _sv0t1.mn = e.mn;
      _sv0t1.d = 0;
      _sv0t1.i0 = 0;
      _sv0t1.i1 = 0;
      _sv0t1.in_ = 0;
      _sv0t1.o0 = 0;
      _sv0t1.o1 = 0;
      _sv0t1.on = 0;
      out = _sv0t1;
    } else {
    }
  }
  return out;
}

int main(void) {
  FullEnv _sv0t0 = full_empty();
  FullEnv e0;
  e0 = _sv0t0;
  FullEnv _sv0t1 = full_mod_register(e0, 100);
  FullEnv e1;
  e1 = _sv0t1;
  FullEnv _sv0t2 = full_mod_register(e1, 200);
  FullEnv e2;
  e2 = _sv0t2;
  int _sv0t3 = lookup_value(e2, 200);
  int ok_mod_only = (1 - _sv0t3);
  FullEnv _sv0t4 = full_scope_enter(e2);
  FullEnv e3;
  e3 = _sv0t4;
  FullEnv _sv0t5 = full_scope_bind(e3, 100);
  FullEnv e4;
  e4 = _sv0t5;
  int _sv0t6 = lookup_value(e4, 100);
  int ok_shadow = (1 - _sv0t6);
  int _sv0t7 = lookup_value(e4, 200);
  int ok_inner_other = (1 - _sv0t7);
  FullEnv _sv0t8 = full_scope_exit(e4);
  FullEnv e5;
  e5 = _sv0t8;
  int _sv0t9 = lookup_value(e5, 100);
  int ok_mod_after = (1 - _sv0t9);
  int _sv0t10 = lookup_value(e5, 99);
  int ok_miss = _sv0t10;
  int ok_mn = (e5.mn - 2);
  int _sv0t11 = (ok_mod_only + ok_shadow);
  int _sv0t12 = (_sv0t11 + ok_inner_other);
  int _sv0t13 = (_sv0t12 + ok_mod_after);
  int _sv0t14 = (_sv0t13 + ok_miss);
  int _sv0t15 = (_sv0t14 + ok_mn);
  return _sv0t15;
}

