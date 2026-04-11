#include "sv0_runtime.h"

static int pool_lookup(int target, int e0, int e1, int e2);
static int pool_add_result(int existing_index, int pool_size);
static int pool_size_after_add(int existing_index, int pool_size);
static int pool_is_duplicate(int existing_index);
static int pool_index_valid(int index, int pool_size);
static int simulate_pool_adds(int s0, int s1, int s2, int s3);

static int pool_lookup(int target, int e0, int e1, int e2) {
  if ((e0 == target)) {
    return 0;
  } else {
  }
  if ((e1 == target)) {
    return 1;
  } else {
  }
  if ((e2 == target)) {
    return 2;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int pool_add_result(int existing_index, int pool_size) {
  if ((existing_index >= 0)) {
    return existing_index;
  } else {
  }
  return pool_size;
}

static int pool_size_after_add(int existing_index, int pool_size) {
  if ((existing_index >= 0)) {
    return pool_size;
  } else {
  }
  int _sv0t0 = (pool_size + 1);
  return _sv0t0;
}

static int pool_is_duplicate(int existing_index) {
  int _sv0t0 = (existing_index >= 0);
  return _sv0t0;
}

static int pool_index_valid(int index, int pool_size) {
  int _sv0t0 = (index >= 0);
  int _sv0t1 = (index < pool_size);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int simulate_pool_adds(int s0, int s1, int s2, int s3) {
  int size = 0;
  int distinct = 0;
  size = (size + 1);
  distinct = (distinct + 1);
  if ((s1 != s0)) {
    size = (size + 1);
    distinct = (distinct + 1);
  } else {
  }
  int _sv0t0 = (s2 != s0);
  int _sv0t1 = (s2 != s1);
  if ((_sv0t0 && _sv0t1)) {
    size = (size + 1);
    distinct = (distinct + 1);
  } else {
  }
  int _sv0t2 = (s3 != s0);
  int _sv0t3 = (s3 != s1);
  int _sv0t4 = (_sv0t2 && _sv0t3);
  int _sv0t5 = (s3 != s2);
  if ((_sv0t4 && _sv0t5)) {
    size = (size + 1);
    distinct = (distinct + 1);
  } else {
  }
  return distinct;
}

int main(void) {
  int _sv0t0 = pool_lookup(20, 10, 20, 30);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = pool_lookup(99, 10, 20, 30);
  int _sv0t2 = (-1);
  if ((_sv0t1 != _sv0t2)) {
    return 2;
  } else {
  }
  int _sv0t3 = pool_lookup(10, 10, 20, 30);
  if ((_sv0t3 != 0)) {
    return 3;
  } else {
  }
  int _sv0t4 = pool_add_result(1, 5);
  if ((_sv0t4 != 1)) {
    return 10;
  } else {
  }
  int _sv0t5 = (-1);
  int _sv0t6 = pool_add_result(_sv0t5, 5);
  if ((_sv0t6 != 5)) {
    return 11;
  } else {
  }
  int _sv0t7 = pool_size_after_add(1, 5);
  if ((_sv0t7 != 5)) {
    return 20;
  } else {
  }
  int _sv0t8 = (-1);
  int _sv0t9 = pool_size_after_add(_sv0t8, 5);
  if ((_sv0t9 != 6)) {
    return 21;
  } else {
  }
  int _sv0t10 = pool_is_duplicate(0);
  if ((!_sv0t10)) {
    return 30;
  } else {
  }
  int _sv0t11 = (-1);
  int _sv0t12 = pool_is_duplicate(_sv0t11);
  if (_sv0t12) {
    return 31;
  } else {
  }
  int _sv0t13 = pool_index_valid(0, 3);
  if ((!_sv0t13)) {
    return 40;
  } else {
  }
  int _sv0t14 = pool_index_valid(2, 3);
  if ((!_sv0t14)) {
    return 41;
  } else {
  }
  int _sv0t15 = pool_index_valid(3, 3);
  if (_sv0t15) {
    return 42;
  } else {
  }
  int _sv0t16 = (-1);
  int _sv0t17 = pool_index_valid(_sv0t16, 3);
  if (_sv0t17) {
    return 43;
  } else {
  }
  int _sv0t18 = simulate_pool_adds(1, 2, 3, 4);
  if ((_sv0t18 != 4)) {
    return 50;
  } else {
  }
  int _sv0t19 = simulate_pool_adds(1, 1, 1, 1);
  if ((_sv0t19 != 1)) {
    return 51;
  } else {
  }
  int _sv0t20 = simulate_pool_adds(1, 2, 1, 2);
  if ((_sv0t20 != 2)) {
    return 52;
  } else {
  }
  int _sv0t21 = simulate_pool_adds(1, 2, 3, 2);
  if ((_sv0t21 != 3)) {
    return 53;
  } else {
  }
  return 0;
}

