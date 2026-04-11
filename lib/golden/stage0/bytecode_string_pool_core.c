#include "sv0_runtime.h"

static int string_pool_header_size(void);
static int string_entry_overhead(void);
static int string_entry_size(int str_len);
static int string_pool_total_size(int count, int s0_len, int s1_len, int s2_len);
static int decode_count_offset(int pool_start);
static int decode_first_entry_offset(int pool_start);
static int decode_next_entry_offset(int current, int str_len);
static int func_entry_header_fields(void);
static int func_entry_header_bytes(void);
static int test_entry_size(void);
static int test_pool_total(void);
static int test_decode_offsets(void);
static int test_func_entry(void);

static int string_pool_header_size(void) {
  return 4;
}

static int string_entry_overhead(void) {
  return 4;
}

static int string_entry_size(int str_len) {
  int _sv0t0 = (4 + str_len);
  return _sv0t0;
}

static int string_pool_total_size(int count, int s0_len, int s1_len, int s2_len) {
  int total = 4;
  if ((count >= 1)) {
    int _sv0t0 = string_entry_size(s0_len);
    total = (total + _sv0t0);
  } else {
  }
  if ((count >= 2)) {
    int _sv0t1 = string_entry_size(s1_len);
    total = (total + _sv0t1);
  } else {
  }
  if ((count >= 3)) {
    int _sv0t2 = string_entry_size(s2_len);
    total = (total + _sv0t2);
  } else {
  }
  return total;
}

static int decode_count_offset(int pool_start) {
  return pool_start;
}

static int decode_first_entry_offset(int pool_start) {
  int _sv0t0 = (pool_start + 4);
  return _sv0t0;
}

static int decode_next_entry_offset(int current, int str_len) {
  int _sv0t0 = (current + 4);
  int _sv0t1 = (_sv0t0 + str_len);
  return _sv0t1;
}

static int func_entry_header_fields(void) {
  return 4;
}

static int func_entry_header_bytes(void) {
  int _sv0t0 = (4 * 4);
  return _sv0t0;
}

static int test_entry_size(void) {
  int _sv0t0 = string_entry_size(0);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = string_entry_size(5);
  if ((_sv0t1 != 9)) {
    return 2;
  } else {
  }
  int _sv0t2 = string_entry_size(100);
  if ((_sv0t2 != 104)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_pool_total(void) {
  int _sv0t0 = string_pool_total_size(0, 0, 0, 0);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = string_pool_total_size(1, 5, 0, 0);
  if ((_sv0t1 != 13)) {
    return 2;
  } else {
  }
  int _sv0t2 = string_pool_total_size(2, 5, 10, 0);
  if ((_sv0t2 != 27)) {
    return 3;
  } else {
  }
  int _sv0t3 = string_pool_total_size(3, 5, 10, 3);
  if ((_sv0t3 != 34)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_decode_offsets(void) {
  int _sv0t0 = decode_count_offset(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = decode_first_entry_offset(0);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = decode_first_entry_offset(100);
  if ((_sv0t2 != 104)) {
    return 3;
  } else {
  }
  int _sv0t3 = decode_next_entry_offset(4, 5);
  if ((_sv0t3 != 13)) {
    return 4;
  } else {
  }
  int _sv0t4 = decode_next_entry_offset(13, 10);
  if ((_sv0t4 != 27)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_func_entry(void) {
  int _sv0t0 = func_entry_header_fields();
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = func_entry_header_bytes();
  if ((_sv0t1 != 16)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_entry_size();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_pool_total();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_decode_offsets();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_func_entry();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

