#include "sv0_runtime.h"


static int sv0_user_main(void) {
  int sv0u_a;
  sv0u_a = 54;
  int sv0u_b;
  sv0u_b = 24;
  while ((sv0u_b != 0)) {
    int sv0u_t;
    sv0u_t = (sv0u_a % sv0u_b);
    sv0u_a = sv0u_b;
    sv0u_b = sv0u_t;
  }
  return sv0u_a;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

