#include "sv0_runtime.h"

typedef struct {
  int n;
  int n0;
  int a0;
  int n1;
  int a1;
} FnArity2;

static FnArity2 arity_empty(void);
static FnArity2 arity_register(FnArity2 e, int name_id, int arity);
static int lookup_fn_arity(FnArity2 e, int name_id);
static int call_arity_ok(FnArity2 e, int name_id, int argc);

static FnArity2 arity_empty(void) {
  FnArity2 _sv0t0;
  _sv0t0.n = 0;
  _sv0t0.n0 = 0;
  _sv0t0.a0 = 0;
  _sv0t0.n1 = 0;
  _sv0t0.a1 = 0;
  return _sv0t0;
}

static FnArity2 arity_register(FnArity2 e, int name_id, int arity) {
  int _sv0t0;
  int _sv0t1;
  if ((e.n >= 1)) {
    int _sv0t2;
    if ((e.n0 == name_id)) {
      return e;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.n >= 2)) {
        int _sv0t4;
        if ((e.n1 == name_id)) {
          return e;
          _sv0t4 = 0;
        } else {
          return e;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        FnArity2 _sv0t5;
        _sv0t5.n = 2;
        _sv0t5.n0 = e.n0;
        _sv0t5.a0 = e.a0;
        _sv0t5.n1 = name_id;
        _sv0t5.a1 = arity;
        return _sv0t5;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    FnArity2 _sv0t6;
    _sv0t6.n = 1;
    _sv0t6.n0 = name_id;
    _sv0t6.a0 = arity;
    _sv0t6.n1 = 0;
    _sv0t6.a1 = 0;
    return _sv0t6;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int lookup_fn_arity(FnArity2 e, int name_id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.n >= 1)) {
    int _sv0t2;
    if ((e.n0 == name_id)) {
      return e.a0;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.n >= 2)) {
        int _sv0t4;
        if ((e.n1 == name_id)) {
          return e.a1;
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

static int call_arity_ok(FnArity2 e, int name_id, int argc) {
  int _sv0t0 = lookup_fn_arity(e, name_id);
  int ar = _sv0t0;
  int _sv0t1;
  int _sv0t2;
  if ((ar < 0)) {
    return 1;
    _sv0t2 = 0;
  } else {
    int _sv0t3;
    if ((ar == argc)) {
      return 1;
      _sv0t3 = 0;
    } else {
      return 0;
      _sv0t3 = 0;
    }
    _sv0t2 = _sv0t3;
  }
  _sv0t1 = _sv0t2;
  return _sv0t1;
}

int main(void) {
  FnArity2 _sv0t0 = arity_empty();
  FnArity2 e0;
  e0 = _sv0t0;
  FnArity2 _sv0t1 = arity_register(e0, 5, 2);
  FnArity2 e1;
  e1 = _sv0t1;
  FnArity2 _sv0t2 = arity_register(e1, 6, 1);
  FnArity2 e2;
  e2 = _sv0t2;
  int _sv0t3 = call_arity_ok(e2, 5, 2);
  int fail_right = (1 - _sv0t3);
  int _sv0t4 = call_arity_ok(e2, 5, 3);
  int fail_wrong = _sv0t4;
  int _sv0t5 = call_arity_ok(e2, 6, 1);
  int fail_other = (1 - _sv0t5);
  int _sv0t6 = call_arity_ok(e2, 99, 0);
  int fail_absent = (1 - _sv0t6);
  int _sv0t7 = lookup_fn_arity(e2, 7);
  int fail_miss = (_sv0t7 + 1);
  int _sv0t8 = (fail_right + fail_wrong);
  int _sv0t9 = (_sv0t8 + fail_other);
  int _sv0t10 = (_sv0t9 + fail_absent);
  int _sv0t11 = (_sv0t10 + fail_miss);
  return _sv0t11;
}

