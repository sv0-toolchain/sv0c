#include "sv0_runtime.h"

static int analyze(int prog);

static int analyze(int prog) {
  return prog;
}

int main(void) {
  int _sv0t0 = analyze(42);
  int r = _sv0t0;
  if ((r != 42)) {
    return 1;
  } else {
  }
  int _sv0t1 = analyze(0);
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

