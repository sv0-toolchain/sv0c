#include "sv0_runtime.h"


static int sv0_user_main(void) {
  int a;
  a = 54;
  int b;
  b = 24;
  while ((b != 0)) {
    int t;
    t = (a % b);
    a = b;
    b = t;
  }
  return a;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

