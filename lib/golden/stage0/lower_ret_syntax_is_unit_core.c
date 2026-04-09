#include "sv0_runtime.h"

static int ret_syntax_is_unit(int has_ret, int ret_ty_class);
static int use_ret_slot(int ret_syntax_is_unit_flag, int ens_mentions_result);

static int ret_syntax_is_unit(int has_ret, int ret_ty_class) {
  int _sv0t0;
  int _sv0t1;
  if ((has_ret == 0)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((ret_ty_class == 1)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((ret_ty_class == 2)) {
        return 1;
        _sv0t3 = 0;
      } else {
        return 0;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int use_ret_slot(int ret_syntax_is_unit_flag, int ens_mentions_result) {
  int _sv0t0;
  int _sv0t1;
  if ((ret_syntax_is_unit_flag != 0)) {
    return 0;
    _sv0t1 = 0;
  } else {
    return ens_mentions_result;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = ret_syntax_is_unit(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = ret_syntax_is_unit(1, 1);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = ret_syntax_is_unit(1, 2);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = ret_syntax_is_unit(1, 3);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = use_ret_slot(1, 1);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = use_ret_slot(0, 0);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = use_ret_slot(0, 1);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

