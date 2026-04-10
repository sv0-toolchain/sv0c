#include "sv0_runtime.h"

static int encode_insn_len(int insn_class);
static int enc_lens(int insn_count, int total_bytes);
static int file_header_len(void);
static int string_entry_len(int str_len);
static int func_entry_header_len(void);
static int func_entry_total_len(int code_bytes);

static int encode_insn_len(int insn_class) {
  if ((insn_class == 0)) {
    return 1;
  } else {
  }
  if ((insn_class == 1)) {
    return 2;
  } else {
  }
  if ((insn_class == 2)) {
    return 5;
  } else {
  }
  if ((insn_class == 3)) {
    return 9;
  } else {
  }
  if ((insn_class == 4)) {
    return 13;
  } else {
  }
  if ((insn_class == 5)) {
    return 5;
  } else {
  }
  return 255;
}

static int enc_lens(int insn_count, int total_bytes) {
  return total_bytes;
}

static int file_header_len(void) {
  return 16;
}

static int string_entry_len(int str_len) {
  int _sv0t0 = (4 + str_len);
  return _sv0t0;
}

static int func_entry_header_len(void) {
  return 16;
}

static int func_entry_total_len(int code_bytes) {
  int _sv0t0 = (16 + code_bytes);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = encode_insn_len(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = encode_insn_len(1);
  if ((_sv0t1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t2 = encode_insn_len(2);
  if ((_sv0t2 != 5)) {
    return 1;
  } else {
  }
  int _sv0t3 = encode_insn_len(3);
  if ((_sv0t3 != 9)) {
    return 1;
  } else {
  }
  int _sv0t4 = encode_insn_len(4);
  if ((_sv0t4 != 13)) {
    return 1;
  } else {
  }
  int _sv0t5 = encode_insn_len(5);
  if ((_sv0t5 != 5)) {
    return 1;
  } else {
  }
  int _sv0t6 = encode_insn_len(99);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  int _sv0t7 = enc_lens(3, 15);
  if ((_sv0t7 != 15)) {
    return 1;
  } else {
  }
  int _sv0t8 = enc_lens(0, 0);
  if ((_sv0t8 != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = file_header_len();
  if ((_sv0t9 != 16)) {
    return 1;
  } else {
  }
  int _sv0t10 = string_entry_len(5);
  if ((_sv0t10 != 9)) {
    return 1;
  } else {
  }
  int _sv0t11 = string_entry_len(0);
  if ((_sv0t11 != 4)) {
    return 1;
  } else {
  }
  int _sv0t12 = func_entry_header_len();
  if ((_sv0t12 != 16)) {
    return 1;
  } else {
  }
  int _sv0t13 = func_entry_total_len(20);
  if ((_sv0t13 != 36)) {
    return 1;
  } else {
  }
  int _sv0t14 = func_entry_total_len(0);
  if ((_sv0t14 != 16)) {
    return 1;
  } else {
  }
  return 0;
}

