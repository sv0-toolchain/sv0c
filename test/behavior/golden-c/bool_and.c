#include "sv0_runtime.h"


static int sv0_user_main(void) {
  int _sv0t0 = (1 < 2);
  int _sv0t1 = (3 < 4);
  if ((_sv0t0 && _sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

