#include "sv0_runtime.h"

static int try_detect_result(int has_ok, int has_err, int ok_arity);
static int try_detect_option(int has_some, int has_none, int some_arity);
static int try_kind(int has_ok, int has_err, int ok_arity, int has_some, int has_none, int some_arity);
static int cast_allowed(int from_tag, int to_tag);
static int cast_is_widening(int from_bits, int to_bits);
static int cast_is_narrowing(int from_bits, int to_bits);
static int test_try_result(void);
static int test_try_option(void);
static int test_try_kind(void);
static int test_cast(void);

static int try_detect_result(int has_ok, int has_err, int ok_arity) {
  if ((has_ok == 0)) {
    return 0;
  } else {
  }
  if ((has_err == 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = (ok_arity == 1);
  return _sv0t0;
}

static int try_detect_option(int has_some, int has_none, int some_arity) {
  if ((has_some == 0)) {
    return 0;
  } else {
  }
  if ((has_none == 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = (some_arity == 1);
  return _sv0t0;
}

static int try_kind(int has_ok, int has_err, int ok_arity, int has_some, int has_none, int some_arity) {
  int _sv0t0 = try_detect_result(has_ok, has_err, ok_arity);
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = try_detect_option(has_some, has_none, some_arity);
  if (_sv0t1) {
    return 2;
  } else {
  }
  return 0;
}

static int cast_allowed(int from_tag, int to_tag) {
  if ((from_tag == 0)) {
    return 0;
  } else {
  }
  if ((from_tag >= 1)) {
    if ((from_tag <= 10)) {
      if ((to_tag >= 1)) {
        if ((to_tag <= 10)) {
          return 1;
        } else {
        }
      } else {
      }
    } else {
    }
  } else {
  }
  return 0;
}

static int cast_is_widening(int from_bits, int to_bits) {
  int _sv0t0 = (to_bits > from_bits);
  return _sv0t0;
}

static int cast_is_narrowing(int from_bits, int to_bits) {
  int _sv0t0 = (to_bits < from_bits);
  return _sv0t0;
}

static int test_try_result(void) {
  int _sv0t0 = try_detect_result(1, 1, 1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = try_detect_result(1, 1, 2);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = try_detect_result(1, 0, 1);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = try_detect_result(0, 1, 1);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_try_option(void) {
  int _sv0t0 = try_detect_option(1, 1, 1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = try_detect_option(1, 1, 0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = try_detect_option(0, 1, 1);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_try_kind(void) {
  int _sv0t0 = try_kind(1, 1, 1, 0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = try_kind(0, 0, 0, 1, 1, 1);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = try_kind(0, 0, 0, 0, 0, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_cast(void) {
  int _sv0t0 = cast_allowed(1, 5);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = cast_allowed(0, 1);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = cast_allowed(1, 11);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = cast_is_widening(8, 32);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = cast_is_widening(32, 8);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = cast_is_narrowing(32, 8);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_try_result();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_try_option();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_try_kind();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_cast();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

