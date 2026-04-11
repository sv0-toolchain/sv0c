#include "sv0_runtime.h"

static int enc_len(int instr_tag, int payload_slots);
static int enc_lens(int i0, int p0, int i1, int p1, int i2, int p2);
static int is_fixed_len(int instr_tag);
static int max_enc_len(void);
static int min_enc_len(void);

static int enc_len(int instr_tag, int payload_slots) {
  if ((instr_tag == 0)) {
    return 5;
  } else {
  }
  if ((instr_tag == 1)) {
    return 5;
  } else {
  }
  if ((instr_tag == 2)) {
    return 5;
  } else {
  }
  if ((instr_tag == 3)) {
    return 1;
  } else {
  }
  if ((instr_tag == 4)) {
    return 1;
  } else {
  }
  if ((instr_tag == 5)) {
    return 5;
  } else {
  }
  if ((instr_tag == 6)) {
    return 5;
  } else {
  }
  if ((instr_tag == 7)) {
    return 5;
  } else {
  }
  if ((instr_tag == 8)) {
    return 9;
  } else {
  }
  if ((instr_tag == 9)) {
    return 5;
  } else {
  }
  if ((instr_tag == 10)) {
    int _sv0t0 = (1 + payload_slots);
    return _sv0t0;
  } else {
  }
  return 1;
}

static int enc_lens(int i0, int p0, int i1, int p1, int i2, int p2) {
  int _sv0t0 = enc_len(i0, p0);
  int _sv0t1 = enc_len(i1, p1);
  int _sv0t2 = (_sv0t0 + _sv0t1);
  int _sv0t3 = enc_len(i2, p2);
  int _sv0t4 = (_sv0t2 + _sv0t3);
  return _sv0t4;
}

static int is_fixed_len(int instr_tag) {
  if ((instr_tag == 10)) {
    return 0;
  } else {
  }
  return 1;
}

static int max_enc_len(void) {
  return 13;
}

static int min_enc_len(void) {
  return 1;
}

int main(void) {
  int _sv0t0 = enc_len(0, 0);
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  int _sv0t1 = enc_len(3, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = enc_len(8, 0);
  if ((_sv0t2 != 9)) {
    return 3;
  } else {
  }
  int _sv0t3 = enc_len(10, 3);
  if ((_sv0t3 != 4)) {
    return 4;
  } else {
  }
  int _sv0t4 = enc_len(10, 0);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = enc_lens(0, 0, 3, 0, 8, 0);
  if ((_sv0t5 != 15)) {
    return 6;
  } else {
  }
  int _sv0t6 = enc_lens(3, 0, 3, 0, 3, 0);
  if ((_sv0t6 != 3)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_fixed_len(0);
  if ((!_sv0t7)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_fixed_len(3);
  if ((!_sv0t8)) {
    return 9;
  } else {
  }
  int _sv0t9 = is_fixed_len(10);
  if (_sv0t9) {
    return 10;
  } else {
  }
  int _sv0t10 = max_enc_len();
  if ((_sv0t10 != 13)) {
    return 11;
  } else {
  }
  int _sv0t11 = min_enc_len();
  if ((_sv0t11 != 1)) {
    return 12;
  } else {
  }
  return 0;
}

