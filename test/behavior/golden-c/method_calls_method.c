#include "sv0_runtime.h"

typedef struct {
  int v;
} N;

static int dbl(N self);
static int quad(N self);

static int dbl(N self) {
  int _sv0t0 = (self.v * 2);
  return _sv0t0;
}

static int quad(N self) {
  int _sv0t0 = dbl(self);
  int _sv0t1 = dbl(self);
  int _sv0t2 = (_sv0t0 + _sv0t1);
  return _sv0t2;
}

static int sv0_user_main(void) {
  N n;
  n.v = 3;
  int _sv0t0 = quad(n);
  return _sv0t0;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

