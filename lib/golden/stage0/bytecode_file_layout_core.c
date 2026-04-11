#include "sv0_runtime.h"

static int header_size(void);
static int func_record_bytes(void);
static int string_section_size(int pool_payload_bytes);
static int func_section_size(int func_count);
static int code_section_size(int code_blob_bytes);
static int total_file_size(int pool_bytes, int func_count, int code_bytes);
static int str_section_offset(void);
static int func_section_offset(int pool_bytes);
static int code_section_offset(int pool_bytes, int func_count);
static int func_record_offset(int base, int index);
static int func_code_end_offset(int func_offset, int func_code_len);

static int header_size(void) {
  return 6;
}

static int func_record_bytes(void) {
  return 20;
}

static int string_section_size(int pool_payload_bytes) {
  int _sv0t0 = (4 + pool_payload_bytes);
  return _sv0t0;
}

static int func_section_size(int func_count) {
  int _sv0t0 = (4 + 4);
  int _sv0t1 = func_record_bytes();
  int _sv0t2 = (func_count * _sv0t1);
  int _sv0t3 = (_sv0t0 + _sv0t2);
  return _sv0t3;
}

static int code_section_size(int code_blob_bytes) {
  int _sv0t0 = (4 + code_blob_bytes);
  return _sv0t0;
}

static int total_file_size(int pool_bytes, int func_count, int code_bytes) {
  int _sv0t0 = header_size();
  int _sv0t1 = string_section_size(pool_bytes);
  int _sv0t2 = (_sv0t0 + _sv0t1);
  int _sv0t3 = func_section_size(func_count);
  int _sv0t4 = (_sv0t2 + _sv0t3);
  int _sv0t5 = code_section_size(code_bytes);
  int _sv0t6 = (_sv0t4 + _sv0t5);
  return _sv0t6;
}

static int str_section_offset(void) {
  int _sv0t0 = header_size();
  return _sv0t0;
}

static int func_section_offset(int pool_bytes) {
  int _sv0t0 = header_size();
  int _sv0t1 = string_section_size(pool_bytes);
  int _sv0t2 = (_sv0t0 + _sv0t1);
  return _sv0t2;
}

static int code_section_offset(int pool_bytes, int func_count) {
  int _sv0t0 = func_section_offset(pool_bytes);
  int _sv0t1 = func_section_size(func_count);
  int _sv0t2 = (_sv0t0 + _sv0t1);
  return _sv0t2;
}

static int func_record_offset(int base, int index) {
  int _sv0t0 = (base + 4);
  int _sv0t1 = (_sv0t0 + 4);
  int _sv0t2 = func_record_bytes();
  int _sv0t3 = (index * _sv0t2);
  int _sv0t4 = (_sv0t1 + _sv0t3);
  return _sv0t4;
}

static int func_code_end_offset(int func_offset, int func_code_len) {
  int _sv0t0 = (func_offset + func_code_len);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = header_size();
  if ((_sv0t0 != 6)) {
    return 1;
  } else {
  }
  int _sv0t1 = func_record_bytes();
  if ((_sv0t1 != 20)) {
    return 2;
  } else {
  }
  int _sv0t2 = string_section_size(0);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  int _sv0t3 = string_section_size(100);
  if ((_sv0t3 != 104)) {
    return 4;
  } else {
  }
  int _sv0t4 = func_section_size(0);
  if ((_sv0t4 != 8)) {
    return 5;
  } else {
  }
  int _sv0t5 = func_section_size(1);
  if ((_sv0t5 != 28)) {
    return 6;
  } else {
  }
  int _sv0t6 = func_section_size(3);
  if ((_sv0t6 != 68)) {
    return 7;
  } else {
  }
  int _sv0t7 = code_section_size(0);
  if ((_sv0t7 != 4)) {
    return 8;
  } else {
  }
  int _sv0t8 = code_section_size(50);
  if ((_sv0t8 != 54)) {
    return 9;
  } else {
  }
  int _sv0t9 = total_file_size(0, 0, 0);
  if ((_sv0t9 != 18)) {
    return 10;
  } else {
  }
  int _sv0t10 = total_file_size(100, 3, 50);
  if ((_sv0t10 != 232)) {
    return 11;
  } else {
  }
  int _sv0t11 = str_section_offset();
  if ((_sv0t11 != 6)) {
    return 12;
  } else {
  }
  int _sv0t12 = func_section_offset(100);
  if ((_sv0t12 != 110)) {
    return 13;
  } else {
  }
  int _sv0t13 = code_section_offset(100, 3);
  if ((_sv0t13 != 178)) {
    return 14;
  } else {
  }
  int _sv0t14 = func_record_offset(110, 0);
  if ((_sv0t14 != 118)) {
    return 15;
  } else {
  }
  int _sv0t15 = func_record_offset(110, 1);
  if ((_sv0t15 != 138)) {
    return 16;
  } else {
  }
  int _sv0t16 = func_record_offset(110, 2);
  if ((_sv0t16 != 158)) {
    return 17;
  } else {
  }
  int _sv0t17 = func_code_end_offset(0, 25);
  if ((_sv0t17 != 25)) {
    return 18;
  } else {
  }
  return 0;
}

