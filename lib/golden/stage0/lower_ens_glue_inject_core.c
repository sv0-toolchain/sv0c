#include "sv0_runtime.h"

static int ens_clause_ir_count(int pre_len);
static int ens_glue_uniform(int num_ens, int pre_each);
static int inject_skip_transform(int num_ens_clauses);
static int inject_return_expand_len(int use_ret_slot, int glue_len, int store_tail_len);

static int ens_clause_ir_count(int pre_len) {
  if ((pre_len < 0)) {
    return 255;
  } else {
  }
  int _sv0t0 = (pre_len + 1);
  return _sv0t0;
}

static int ens_glue_uniform(int num_ens, int pre_each) {
  if ((num_ens < 0)) {
    return 255;
  } else {
  }
  if ((pre_each < 0)) {
    return 255;
  } else {
  }
  int _sv0t0 = (pre_each + 1);
  int _sv0t1 = (num_ens * _sv0t0);
  return _sv0t1;
}

static int inject_skip_transform(int num_ens_clauses) {
  if ((num_ens_clauses == 0)) {
    return 1;
  } else {
  }
  return 0;
}

static int inject_return_expand_len(int use_ret_slot, int glue_len, int store_tail_len) {
  if ((use_ret_slot != 0)) {
    if ((use_ret_slot != 1)) {
      return 255;
    } else {
    }
  } else {
  }
  if ((glue_len < 0)) {
    return 255;
  } else {
  }
  if ((store_tail_len < 0)) {
    return 255;
  } else {
  }
  if ((use_ret_slot == 0)) {
    int _sv0t0 = (glue_len + 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = (store_tail_len + glue_len);
  int _sv0t2 = (_sv0t1 + 1);
  return _sv0t2;
}

int main(void) {
  int _sv0t0 = ens_clause_ir_count(2);
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = (-5);
  int _sv0t2 = ens_clause_ir_count(_sv0t1);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = ens_glue_uniform(2, 3);
  if ((_sv0t3 != 8)) {
    return 1;
  } else {
  }
  int _sv0t4 = inject_skip_transform(0);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = inject_skip_transform(3);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = inject_return_expand_len(0, 4, 9);
  if ((_sv0t6 != 5)) {
    return 1;
  } else {
  }
  int _sv0t7 = inject_return_expand_len(1, 4, 0);
  if ((_sv0t7 != 5)) {
    return 1;
  } else {
  }
  int _sv0t8 = inject_return_expand_len(1, 4, 1);
  if ((_sv0t8 != 6)) {
    return 1;
  } else {
  }
  int _sv0t9 = inject_return_expand_len(2, 1, 0);
  if ((_sv0t9 != 255)) {
    return 1;
  } else {
  }
  int _sv0t10 = (-1);
  int _sv0t11 = inject_return_expand_len(0, _sv0t10, 0);
  if ((_sv0t11 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

