#include "sv0_runtime.h"

static int sv0u_half(int sv0u_x);

static int sv0u_half(int sv0u_x) {
  sv0_requires((sv0u_x > 0), "half");
  int _sv0t0 = (sv0u_x / 2);
  return _sv0t0;
}

static int sv0_user_main(void) {
  int _sv0t0 = sv0u_half(12);
  return _sv0t0;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

