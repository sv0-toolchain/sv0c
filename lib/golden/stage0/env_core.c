#include "sv0_runtime.h"

typedef struct {
  int v0;
  int v1;
  int n;
} Env2;

static Env2 env_empty(void);
static Env2 env_register(Env2 e, int id);
static int env_lookup(Env2 e, int id);

static Env2 env_empty(void) {
  Env2 _sv0t0;
  _sv0t0.v0 = 0;
  _sv0t0.v1 = 0;
  _sv0t0.n = 0;
  return _sv0t0;
}

static Env2 env_register(Env2 e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.n == 0)) {
    Env2 _sv0t2;
    _sv0t2.v0 = id;
    _sv0t2.v1 = 0;
    _sv0t2.n = 1;
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((e.n == 1)) {
      Env2 _sv0t4;
      _sv0t4.v0 = e.v0;
      _sv0t4.v1 = id;
      _sv0t4.n = 2;
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

static int env_lookup(Env2 e, int id) {
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

int main(void) {
  Env2 _sv0t0 = env_empty();
  Env2 e0;
  e0 = _sv0t0;
  Env2 _sv0t1 = env_register(e0, 7);
  Env2 e1;
  e1 = _sv0t1;
  Env2 _sv0t2 = env_register(e1, 9);
  Env2 e2;
  e2 = _sv0t2;
  int _sv0t3 = env_lookup(e2, 7);
  int ok_hit_a = (_sv0t3 - 1);
  int _sv0t4 = env_lookup(e2, 9);
  int ok_hit_b = (_sv0t4 - 1);
  int _sv0t5 = env_lookup(e2, 3);
  int ok_miss = _sv0t5;
  int ok_n = (e2.n - 2);
  int _sv0t6 = (ok_hit_a + ok_hit_b);
  int _sv0t7 = (_sv0t6 + ok_miss);
  int _sv0t8 = (_sv0t7 + ok_n);
  return _sv0t8;
}

