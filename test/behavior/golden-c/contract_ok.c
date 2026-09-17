#include "sv0_runtime.h"

static int half(int x);

static int half(int x) {
  sv0_requires((x > 0), "half");
  int _sv0t0 = (x / 2);
  return _sv0t0;
}

static int sv0_user_main(void) {
  int _sv0t0 = half(12);
  return _sv0t0;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

