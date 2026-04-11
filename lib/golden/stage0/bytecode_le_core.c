#include "sv0_runtime.h"

static int byte0_of_i32(int v);
static int byte1_of_i32(int v);
static int byte2_of_i32(int v);
static int byte3_of_i32(int v);
static int reconstruct_i32(int b0, int b1, int b2, int b3);
static int u16_le_byte0(int v);
static int u16_le_byte1(int v);
static int reconstruct_u16(int b0, int b1);
static int encoded_size_i32(void);
static int encoded_size_u16(void);
static int encoded_size_i64(void);
static int test_byte_extract(void);
static int test_reconstruct(void);
static int test_roundtrip(void);
static int test_sizes(void);

static int byte0_of_i32(int v) {
  int _sv0t0 = (v / 256);
  int _sv0t1 = (_sv0t0 * 256);
  int _sv0t2 = (v - _sv0t1);
  return _sv0t2;
}

static int byte1_of_i32(int v) {
  int shifted = (v / 256);
  int _sv0t0 = (shifted / 256);
  int _sv0t1 = (_sv0t0 * 256);
  int _sv0t2 = (shifted - _sv0t1);
  return _sv0t2;
}

static int byte2_of_i32(int v) {
  int shifted = (v / 65536);
  int _sv0t0 = (shifted / 256);
  int _sv0t1 = (_sv0t0 * 256);
  int _sv0t2 = (shifted - _sv0t1);
  return _sv0t2;
}

static int byte3_of_i32(int v) {
  int shifted = (v / 16777216);
  int _sv0t0 = (shifted / 256);
  int _sv0t1 = (_sv0t0 * 256);
  int _sv0t2 = (shifted - _sv0t1);
  return _sv0t2;
}

static int reconstruct_i32(int b0, int b1, int b2, int b3) {
  int _sv0t0 = (b1 * 256);
  int _sv0t1 = (b0 + _sv0t0);
  int _sv0t2 = (b2 * 65536);
  int _sv0t3 = (_sv0t1 + _sv0t2);
  int _sv0t4 = (b3 * 16777216);
  int _sv0t5 = (_sv0t3 + _sv0t4);
  return _sv0t5;
}

static int u16_le_byte0(int v) {
  int _sv0t0 = (v / 256);
  int _sv0t1 = (_sv0t0 * 256);
  int _sv0t2 = (v - _sv0t1);
  return _sv0t2;
}

static int u16_le_byte1(int v) {
  int _sv0t0 = (v / 256);
  return _sv0t0;
}

static int reconstruct_u16(int b0, int b1) {
  int _sv0t0 = (b1 * 256);
  int _sv0t1 = (b0 + _sv0t0);
  return _sv0t1;
}

static int encoded_size_i32(void) {
  return 4;
}

static int encoded_size_u16(void) {
  return 2;
}

static int encoded_size_i64(void) {
  return 8;
}

static int test_byte_extract(void) {
  int _sv0t0 = byte0_of_i32(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = byte0_of_i32(255);
  if ((_sv0t1 != 255)) {
    return 2;
  } else {
  }
  int _sv0t2 = byte0_of_i32(256);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = byte1_of_i32(256);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = byte1_of_i32(512);
  if ((_sv0t4 != 2)) {
    return 5;
  } else {
  }
  int _sv0t5 = byte0_of_i32(300);
  if ((_sv0t5 != 44)) {
    return 6;
  } else {
  }
  int _sv0t6 = byte1_of_i32(300);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_reconstruct(void) {
  int _sv0t0 = reconstruct_i32(0, 0, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = reconstruct_i32(1, 0, 0, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = reconstruct_i32(0, 1, 0, 0);
  if ((_sv0t2 != 256)) {
    return 3;
  } else {
  }
  int _sv0t3 = reconstruct_i32(44, 1, 0, 0);
  if ((_sv0t3 != 300)) {
    return 4;
  } else {
  }
  int _sv0t4 = reconstruct_i32(0, 0, 1, 0);
  if ((_sv0t4 != 65536)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_roundtrip(void) {
  int v = 12345;
  int _sv0t0 = byte0_of_i32(v);
  int b0 = _sv0t0;
  int _sv0t1 = byte1_of_i32(v);
  int b1 = _sv0t1;
  int _sv0t2 = byte2_of_i32(v);
  int b2 = _sv0t2;
  int _sv0t3 = byte3_of_i32(v);
  int b3 = _sv0t3;
  int _sv0t4 = reconstruct_i32(b0, b1, b2, b3);
  if ((_sv0t4 != 12345)) {
    return 1;
  } else {
  }
  int v2 = 1000;
  int _sv0t5 = u16_le_byte0(v2);
  int u0 = _sv0t5;
  int _sv0t6 = u16_le_byte1(v2);
  int u1 = _sv0t6;
  int _sv0t7 = reconstruct_u16(u0, u1);
  if ((_sv0t7 != 1000)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_sizes(void) {
  int _sv0t0 = encoded_size_i32();
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = encoded_size_u16();
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = encoded_size_i64();
  if ((_sv0t2 != 8)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_byte_extract();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_reconstruct();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_roundtrip();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_sizes();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

