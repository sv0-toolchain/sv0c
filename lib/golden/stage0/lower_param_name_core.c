#include "sv0_runtime.h"

static int param_name_tag(int pat_kind);

static int param_name_tag(int pat_kind) {
  if ((pat_kind == 1)) {
    return 1;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = param_name_tag(1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = param_name_tag(0);
  if ((_sv0t1 != 255)) {
    return 1;
  } else {
  }
  int _sv0t2 = param_name_tag(2);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = (-1);
  int _sv0t4 = param_name_tag(_sv0t3);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

