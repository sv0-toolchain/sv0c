#include "sv0_runtime.h"

static int old_instrs_per_target_count(int cty_path);
static int old_instrs_total(int old_targets_hits);

static int old_instrs_per_target_count(int cty_path) {
  int _sv0t0;
  int _sv0t1;
  if ((cty_path == 1)) {
    return 2;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((cty_path == 2)) {
      return 2;
      _sv0t2 = 0;
    } else {
      return 255;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int old_instrs_total(int old_targets_hits) {
  int _sv0t0 = (old_targets_hits * 2);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = old_instrs_per_target_count(1);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = old_instrs_per_target_count(2);
  if ((_sv0t1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t2 = old_instrs_per_target_count(0);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = old_instrs_total(0);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = old_instrs_total(1);
  if ((_sv0t4 != 2)) {
    return 1;
  } else {
  }
  int _sv0t5 = old_instrs_total(4);
  if ((_sv0t5 != 8)) {
    return 1;
  } else {
  }
  return 0;
}

