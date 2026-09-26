#include "sv0_runtime.h"


static int sv0_user_main(void) {
  int sv0u_r;
  sv0u_r = 1;
  int _sv0t0 = 5;
  int _sv0t1 = 1;
  while ((_sv0t1 < _sv0t0)) {
    int sv0u_i = _sv0t1;
    _sv0t1 = (_sv0t1 + 1);
    sv0u_r = (sv0u_r * sv0u_i);
  }
  return sv0u_r;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

