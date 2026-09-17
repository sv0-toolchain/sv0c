#include "sv0_runtime.h"


static int sv0_user_main(void) {
  int _sv0t0 = (3 * 4);
  int _sv0t1 = (2 + _sv0t0);
  return _sv0t1;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

