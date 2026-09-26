#include "sv0_runtime.h"

typedef struct {
  int sv0u_v;
} N;

static int sv0u_dbl(N sv0u_self);
static int sv0u_quad(N sv0u_self);

static int sv0u_dbl(N sv0u_self) {
  int _sv0t0 = (sv0u_self.sv0u_v * 2);
  return _sv0t0;
}

static int sv0u_quad(N sv0u_self) {
  int _sv0t0 = sv0u_dbl(sv0u_self);
  int _sv0t1 = sv0u_dbl(sv0u_self);
  int _sv0t2 = (_sv0t0 + _sv0t1);
  return _sv0t2;
}

static int sv0_user_main(void) {
  N sv0u_n;
  sv0u_n.sv0u_v = 3;
  int _sv0t0 = sv0u_quad(sv0u_n);
  return _sv0t0;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

