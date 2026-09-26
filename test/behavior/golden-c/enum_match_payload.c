#include "sv0_runtime.h"

typedef struct {
  int tag;
  int p0;
} E;


static int sv0_user_main(void) {
  E sv0u_e;
  sv0u_e.tag = 0;
  sv0u_e.p0 = 41;
  int _sv0t0;
  _sv0t0 = 0;
  if ((sv0u_e.tag == 0)) {
    int sv0u_n = sv0u_e.p0;
    _sv0t0 = sv0u_n;
  } else {
    if ((sv0u_e.tag == 1)) {
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

