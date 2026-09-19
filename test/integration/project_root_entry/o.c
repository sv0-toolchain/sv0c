#include "sv0_runtime.h"

static int helper(void);

static int helper(void) {
  return 40;
}

static int sv0_user_main(void) {
  int _sv0t0 = helper();
  int _sv0t1 = (_sv0t0 + 2);
  return _sv0t1;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

