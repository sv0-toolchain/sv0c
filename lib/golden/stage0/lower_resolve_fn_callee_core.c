#include "sv0_runtime.h"

static int rfc_err_bad_alias_target(void);
static int resolve_fn_callee(int alias_hit, int alias_target_is_simple, int name_if_no_alias, int resolved_target_if_simple);

static int rfc_err_bad_alias_target(void) {
  return 255;
}

static int resolve_fn_callee(int alias_hit, int alias_target_is_simple, int name_if_no_alias, int resolved_target_if_simple) {
  int _sv0t0;
  int _sv0t1;
  if ((alias_hit == 0)) {
    return name_if_no_alias;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((alias_target_is_simple == 0)) {
      int _sv0t3 = rfc_err_bad_alias_target();
      return _sv0t3;
      _sv0t2 = 0;
    } else {
      return resolved_target_if_simple;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = resolve_fn_callee(0, 0, 7, 99);
  if ((_sv0t0 != 7)) {
    return 1;
  } else {
  }
  int _sv0t1 = resolve_fn_callee(0, 1, 7, 99);
  if ((_sv0t1 != 7)) {
    return 1;
  } else {
  }
  int _sv0t2 = resolve_fn_callee(1, 0, 7, 99);
  int _sv0t3 = rfc_err_bad_alias_target();
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = resolve_fn_callee(1, 1, 7, 42);
  if ((_sv0t4 != 42)) {
    return 1;
  } else {
  }
  int _sv0t5 = resolve_fn_callee(1, 1, 7, 0);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

