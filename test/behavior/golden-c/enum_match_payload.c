#include "sv0_runtime.h"

typedef struct {
  int tag;
  int p0;
} E;


static int sv0_user_main(void) {
  E e;
  e.tag = 0;
  e.p0 = 41;
  int _sv0t0;
  _sv0t0 = 0;
  if ((e.tag == 0)) {
    int n = e.p0;
    _sv0t0 = n;
  } else {
    if ((e.tag == 1)) {
      _sv0t0 = 0;
    } else {
    }
  }
  return _sv0t0;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

