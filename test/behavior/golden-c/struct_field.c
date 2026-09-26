#include "sv0_runtime.h"

typedef struct {
  int sv0u_x;
  int sv0u_y;
} P;


static int sv0_user_main(void) {
  P sv0u_p;
  sv0u_p.sv0u_x = 40;
  sv0u_p.sv0u_y = 2;
  int _sv0t0 = (sv0u_p.sv0u_x + sv0u_p.sv0u_y);
  return _sv0t0;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

