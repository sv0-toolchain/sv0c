#include "sv0_runtime.h"

static int wsl_depth_after_enter(int old_n, int extra_n);
static int wsl_depth_after_exit(int old_n);

static int wsl_depth_after_enter(int old_n, int extra_n) {
  int _sv0t0 = (old_n + extra_n);
  return _sv0t0;
}

static int wsl_depth_after_exit(int old_n) {
  return old_n;
}

int main(void) {
  int _sv0t0 = wsl_depth_after_enter(3, 2);
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  int _sv0t1 = wsl_depth_after_enter(0, 7);
  if ((_sv0t1 != 7)) {
    return 1;
  } else {
  }
  int _sv0t2 = wsl_depth_after_exit(3);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = wsl_depth_after_exit(10);
  if ((_sv0t3 != 10)) {
    return 1;
  } else {
  }
  return 0;
}

