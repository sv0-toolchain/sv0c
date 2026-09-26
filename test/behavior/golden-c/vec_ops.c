#include "sv0_runtime.h"


static int sv0_user_main(void) {
  int _sv0t0 = sv0_vec_new();
  int sv0u_v;
  sv0u_v = _sv0t0;
  sv0_vec_push(sv0u_v, 40);
  sv0_vec_push(sv0u_v, 2);
  int _sv0t3 = sv0_vec_get(sv0u_v, 0);
  int _sv0t4 = sv0_vec_get(sv0u_v, 1);
  int _sv0t5 = (_sv0t3 + _sv0t4);
  return _sv0t5;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

