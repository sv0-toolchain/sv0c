#include "sv0_runtime.h"

typedef struct {
  int d;
  int i0;
  int i1;
  int in_;
  int o0;
  int o1;
  int on;
} ScopeMini;

static ScopeMini scope_empty(void);
static int frame_has(int f_u0, int f_u1, int f_n, int id);
static int scope_lookup(ScopeMini e, int id);
static ScopeMini scope_bind(ScopeMini e, int id);
static ScopeMini scope_enter(ScopeMini e);
static ScopeMini scope_exit(ScopeMini e);

static ScopeMini scope_empty(void) {
  ScopeMini _sv0t0;
  _sv0t0.d = 0;
  _sv0t0.i0 = 0;
  _sv0t0.i1 = 0;
  _sv0t0.in_ = 0;
  _sv0t0.o0 = 0;
  _sv0t0.o1 = 0;
  _sv0t0.on = 0;
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

static int scope_lookup(ScopeMini e, int id) {
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

static ScopeMini scope_bind(ScopeMini e, int id) {
  ScopeMini out;
  out = e;
  if ((e.d == 0)) {
    ScopeMini _sv0t0;
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
        ScopeMini _sv0t1;
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
          ScopeMini _sv0t2;
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
        ScopeMini _sv0t3;
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
          ScopeMini _sv0t4;
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

static ScopeMini scope_enter(ScopeMini e) {
  ScopeMini out;
  out = e;
  if ((e.d == 0)) {
    ScopeMini _sv0t0;
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
      ScopeMini _sv0t1;
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

static ScopeMini scope_exit(ScopeMini e) {
  ScopeMini out;
  out = e;
  if ((e.d == 2)) {
    ScopeMini _sv0t0;
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
      ScopeMini _sv0t1 = scope_empty();
      out = _sv0t1;
    } else {
    }
  }
  return out;
}

int main(void) {
  ScopeMini _sv0t0 = scope_empty();
  ScopeMini e0;
  e0 = _sv0t0;
  ScopeMini _sv0t1 = scope_bind(e0, 5);
  ScopeMini e1;
  e1 = _sv0t1;
  ScopeMini _sv0t2 = scope_enter(e1);
  ScopeMini e2;
  e2 = _sv0t2;
  ScopeMini _sv0t3 = scope_bind(e2, 7);
  ScopeMini e3;
  e3 = _sv0t3;
  int _sv0t4 = scope_lookup(e3, 7);
  int ok_inner = (_sv0t4 - 1);
  int _sv0t5 = scope_lookup(e3, 5);
  int ok_outer = (_sv0t5 - 1);
  int _sv0t6 = scope_lookup(e3, 9);
  int ok_miss_inner = _sv0t6;
  ScopeMini _sv0t7 = scope_exit(e3);
  ScopeMini e4;
  e4 = _sv0t7;
  int _sv0t8 = scope_lookup(e4, 5);
  int ok_after = (_sv0t8 - 1);
  int _sv0t9 = scope_lookup(e4, 7);
  int ok_gone = _sv0t9;
  int _sv0t10 = (ok_inner + ok_outer);
  int _sv0t11 = (_sv0t10 + ok_miss_inner);
  int _sv0t12 = (_sv0t11 + ok_after);
  int _sv0t13 = (_sv0t12 + ok_gone);
  return _sv0t13;
}

