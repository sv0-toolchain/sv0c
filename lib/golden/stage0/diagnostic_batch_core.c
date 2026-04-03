#include "sv0_runtime.h"

static int sev_is_error(int tag);
static int has_errors3(int t0, int t1, int t2);
static int error_count3(int t0, int t1, int t2);

static int sev_is_error(int tag) {
  int _sv0t0;
  int _sv0t1;
  if ((tag == 0)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int has_errors3(int t0, int t1, int t2) {
  int _sv0t2 = sev_is_error(t0);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 == 1)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t4 = sev_is_error(t1);
    int _sv0t3;
    if ((_sv0t4 == 1)) {
      return 1;
      _sv0t3 = 0;
    } else {
      int _sv0t6 = sev_is_error(t2);
      int _sv0t5;
      if ((_sv0t6 == 1)) {
        return 1;
        _sv0t5 = 0;
      } else {
        return 0;
        _sv0t5 = 0;
      }
      _sv0t3 = _sv0t5;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int error_count3(int t0, int t1, int t2) {
  int _sv0t0 = sev_is_error(t0);
  int _sv0t1 = sev_is_error(t1);
  int _sv0t2 = (_sv0t0 + _sv0t1);
  int _sv0t3 = sev_is_error(t2);
  int _sv0t4 = (_sv0t2 + _sv0t3);
  return _sv0t4;
}

int main(void) {
  int _sv0t0 = has_errors3(0, 0, 2);
  int h = _sv0t0;
  int want_h = 1;
  int _sv0t1 = error_count3(0, 0, 2);
  int c = _sv0t1;
  int want_c = 2;
  int _sv0t2 = has_errors3(2, 1, 2);
  int h2 = _sv0t2;
  int want_h2 = 0;
  int _sv0t3 = (h - want_h);
  int _sv0t4 = (c - want_c);
  int _sv0t5 = (_sv0t3 + _sv0t4);
  int _sv0t6 = (h2 - want_h2);
  int _sv0t7 = (_sv0t5 + _sv0t6);
  return _sv0t7;
}

