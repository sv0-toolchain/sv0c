#include "sv0_runtime.h"


static int sv0_user_main(void) {
  int _sv0t0 = sv0_str_concat(sv0_str_lit("ab", 2), sv0_str_lit("cde", 3));
  int _sv0t1 = sv0_str_len(_sv0t0);
  return _sv0t1;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  return (int)sv0_user_main();
}

