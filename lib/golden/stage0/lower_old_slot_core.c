#include "sv0_runtime.h"

static int old_slot_prefix_len(void);
static int old_slot_total_len(int param_name_len);

static int old_slot_prefix_len(void) {
  return 8;
}

static int old_slot_total_len(int param_name_len) {
  int _sv0t0 = old_slot_prefix_len();
  int _sv0t1 = (_sv0t0 + param_name_len);
  return _sv0t1;
}

int main(void) {
  int _sv0t0 = old_slot_prefix_len();
  if ((_sv0t0 != 8)) {
    return 1;
  } else {
  }
  int _sv0t1 = old_slot_total_len(0);
  if ((_sv0t1 != 8)) {
    return 1;
  } else {
  }
  int _sv0t2 = old_slot_total_len(1);
  if ((_sv0t2 != 9)) {
    return 1;
  } else {
  }
  int _sv0t3 = old_slot_total_len(3);
  if ((_sv0t3 != 11)) {
    return 1;
  } else {
  }
  return 0;
}

