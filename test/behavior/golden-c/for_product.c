#include "sv0_runtime.h"


static int sv0_user_main(void) {
  int r;
  r = 1;
  int _sv0t0 = 5;
  int _sv0t1 = 1;
  while ((_sv0t1 < _sv0t0)) {
    int i = _sv0t1;
    _sv0t1 = (_sv0t1 + 1);
    r = (r * i);
  }
  return r;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

