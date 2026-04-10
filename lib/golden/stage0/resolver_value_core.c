#include "sv0_runtime.h"

typedef struct {
  int v0;
  int v1;
  int v2;
  int n;
} Env3;

static Env3 env3_empty(void);
static Env3 env3_register(Env3 e, int id);
static int lookup_value(Env3 e, int id);
static int resolve_path_ok(Env3 e, int id);

static Env3 env3_empty(void) {
  Env3 _sv0t0;
  _sv0t0.v0 = 0;
  _sv0t0.v1 = 0;
  _sv0t0.v2 = 0;
  _sv0t0.n = 0;
  return _sv0t0;
}

static Env3 env3_register(Env3 e, int id) {
  Env3 out;
  out = e;
  if ((e.n == 0)) {
    Env3 _sv0t0;
    _sv0t0.v0 = id;
    _sv0t0.v1 = 0;
    _sv0t0.v2 = 0;
    _sv0t0.n = 1;
    out = _sv0t0;
  } else {
    if ((e.n == 1)) {
      Env3 _sv0t1;
      _sv0t1.v0 = e.v0;
      _sv0t1.v1 = id;
      _sv0t1.v2 = 0;
      _sv0t1.n = 2;
      out = _sv0t1;
    } else {
      if ((e.n == 2)) {
        Env3 _sv0t2;
        _sv0t2.v0 = e.v0;
        _sv0t2.v1 = e.v1;
        _sv0t2.v2 = id;
        _sv0t2.n = 3;
        out = _sv0t2;
      } else {
      }
    }
  }
  return out;
}

static int lookup_value(Env3 e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.n >= 1)) {
    int _sv0t2;
    if ((e.v0 == id)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.n >= 2)) {
        int _sv0t4;
        if ((e.v1 == id)) {
          return 1;
          _sv0t4 = 0;
        } else {
          int _sv0t5;
          if ((e.n >= 3)) {
            int _sv0t6;
            if ((e.v2 == id)) {
              return 1;
              _sv0t6 = 0;
            } else {
              return 0;
              _sv0t6 = 0;
            }
            _sv0t5 = _sv0t6;
          } else {
            return 0;
            _sv0t5 = 0;
          }
          _sv0t4 = _sv0t5;
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

static int resolve_path_ok(Env3 e, int id) {
  int _sv0t0 = lookup_value(e, id);
  return _sv0t0;
}

int main(void) {
  Env3 _sv0t0 = env3_empty();
  Env3 e0;
  e0 = _sv0t0;
  Env3 _sv0t1 = env3_register(e0, 7);
  Env3 e1;
  e1 = _sv0t1;
  Env3 _sv0t2 = env3_register(e1, 9);
  Env3 e2;
  e2 = _sv0t2;
  Env3 _sv0t3 = env3_register(e2, 11);
  Env3 e3;
  e3 = _sv0t3;
  int _sv0t4 = resolve_path_ok(e3, 7);
  int ok_a = (_sv0t4 - 1);
  int _sv0t5 = resolve_path_ok(e3, 9);
  int ok_b = (_sv0t5 - 1);
  int _sv0t6 = resolve_path_ok(e3, 11);
  int ok_c = (_sv0t6 - 1);
  int _sv0t7 = resolve_path_ok(e3, 3);
  int ok_miss = _sv0t7;
  int ok_n = (e3.n - 3);
  int _sv0t8 = (ok_a + ok_b);
  int _sv0t9 = (_sv0t8 + ok_c);
  int _sv0t10 = (_sv0t9 + ok_miss);
  int _sv0t11 = (_sv0t10 + ok_n);
  return _sv0t11;
}

