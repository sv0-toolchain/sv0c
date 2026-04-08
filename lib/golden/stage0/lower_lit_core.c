#include "sv0_runtime.h"

static int lit_kind_int(void);
static int lit_kind_bool(void);
static int lit_kind_unit(void);
static int lit_kind_string(void);
static int lit_kind_other(void);
static int lower_lit_supported(int k);

static int lit_kind_int(void) {
  return 1;
}

static int lit_kind_bool(void) {
  return 2;
}

static int lit_kind_unit(void) {
  return 3;
}

static int lit_kind_string(void) {
  return 4;
}

static int lit_kind_other(void) {
  return 99;
}

static int lower_lit_supported(int k) {
  int _sv0t0;
  int _sv0t1;
  if ((k == 1)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((k == 2)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((k == 3)) {
        return 1;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((k == 4)) {
          return 1;
          _sv0t4 = 0;
        } else {
          return 0;
          _sv0t4 = 0;
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

int main(void) {
  int _sv0t0 = lit_kind_int();
  int _sv0t1 = lower_lit_supported(_sv0t0);
  int a0 = (_sv0t1 - 1);
  int _sv0t2 = lit_kind_bool();
  int _sv0t3 = lower_lit_supported(_sv0t2);
  int a1 = (_sv0t3 - 1);
  int _sv0t4 = lit_kind_unit();
  int _sv0t5 = lower_lit_supported(_sv0t4);
  int a2 = (_sv0t5 - 1);
  int _sv0t6 = lit_kind_string();
  int _sv0t7 = lower_lit_supported(_sv0t6);
  int a3 = (_sv0t7 - 1);
  int _sv0t8 = lit_kind_other();
  int _sv0t9 = lower_lit_supported(_sv0t8);
  int a4 = _sv0t9;
  int _sv0t10 = (a0 + a1);
  int _sv0t11 = (_sv0t10 + a2);
  int _sv0t12 = (_sv0t11 + a3);
  int _sv0t13 = (_sv0t12 + a4);
  return _sv0t13;
}

