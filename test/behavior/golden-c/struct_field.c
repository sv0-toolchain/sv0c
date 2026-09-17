#include "sv0_runtime.h"

typedef struct {
  int x;
  int y;
} P;


static int sv0_user_main(void) {
  P p;
  p.x = 40;
  p.y = 2;
  int _sv0t0 = (p.x + p.y);
  return _sv0t0;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

