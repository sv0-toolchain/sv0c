#include "sv0_runtime.h"

static int binding_assignable(int is_let_mut);
static int err_assign_immutable(void);
static int err_assign_lhs(void);

static int binding_assignable(int is_let_mut) {
  return is_let_mut;
}

static int err_assign_immutable(void) {
  return 448;
}

static int err_assign_lhs(void) {
  return 449;
}

int main(void) {
  int _sv0t0 = binding_assignable(1);
  int c0 = (_sv0t0 - 1);
  int _sv0t1 = binding_assignable(0);
  int c1 = _sv0t1;
  int _sv0t2 = err_assign_immutable();
  int c2 = (_sv0t2 - 448);
  int _sv0t3 = err_assign_lhs();
  int c3 = (_sv0t3 - 449);
  int _sv0t4 = (c0 + c1);
  int _sv0t5 = (_sv0t4 + c2);
  int _sv0t6 = (_sv0t5 + c3);
  return _sv0t6;
}

