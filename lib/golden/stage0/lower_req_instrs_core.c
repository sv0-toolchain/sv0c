#include "sv0_runtime.h"

static int req_clause_ir_count(int pre_len);
static int req_instrs_uniform(int num_reqs, int pre_each);

static int req_clause_ir_count(int pre_len) {
  if ((pre_len < 0)) {
    return 255;
  } else {
  }
  int _sv0t0 = (pre_len + 1);
  return _sv0t0;
}

static int req_instrs_uniform(int num_reqs, int pre_each) {
  if ((num_reqs < 0)) {
    return 255;
  } else {
  }
  if ((pre_each < 0)) {
    return 255;
  } else {
  }
  int _sv0t0 = (pre_each + 1);
  int _sv0t1 = (num_reqs * _sv0t0);
  return _sv0t1;
}

int main(void) {
  int _sv0t0 = req_clause_ir_count(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = req_clause_ir_count(5);
  if ((_sv0t1 != 6)) {
    return 1;
  } else {
  }
  int _sv0t2 = (-1);
  int _sv0t3 = req_clause_ir_count(_sv0t2);
  if ((_sv0t3 != 255)) {
    return 1;
  } else {
  }
  int _sv0t4 = req_instrs_uniform(0, 0);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = req_instrs_uniform(1, 0);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = req_instrs_uniform(4, 0);
  if ((_sv0t6 != 4)) {
    return 1;
  } else {
  }
  int _sv0t7 = req_instrs_uniform(3, 7);
  if ((_sv0t7 != 24)) {
    return 1;
  } else {
  }
  int _sv0t8 = (-1);
  int _sv0t9 = req_instrs_uniform(_sv0t8, 0);
  if ((_sv0t9 != 255)) {
    return 1;
  } else {
  }
  int _sv0t10 = (-3);
  int _sv0t11 = req_instrs_uniform(2, _sv0t10);
  if ((_sv0t11 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

