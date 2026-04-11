#include "sv0_runtime.h"

static int decode_insn_tag(int opcode);
static int decode_payload_bytes(int opcode);
static int decode_total_bytes(int opcode);
static int decode_is_valid(int opcode);

static int decode_insn_tag(int opcode) {
  if ((opcode == 0)) {
    return 0;
  } else {
  }
  if ((opcode == 1)) {
    return 1;
  } else {
  }
  if ((opcode == 2)) {
    return 2;
  } else {
  }
  if ((opcode == 3)) {
    return 3;
  } else {
  }
  if ((opcode == 4)) {
    return 4;
  } else {
  }
  if ((opcode == 5)) {
    return 5;
  } else {
  }
  if ((opcode == 6)) {
    return 6;
  } else {
  }
  if ((opcode == 7)) {
    return 7;
  } else {
  }
  if ((opcode == 8)) {
    return 8;
  } else {
  }
  int _sv0t0 = (opcode >= 16);
  int _sv0t1 = (opcode <= 21);
  if ((_sv0t0 && _sv0t1)) {
    return 9;
  } else {
  }
  int _sv0t2 = (opcode >= 32);
  int _sv0t3 = (opcode <= 37);
  if ((_sv0t2 && _sv0t3)) {
    return 10;
  } else {
  }
  int _sv0t4 = (opcode >= 48);
  int _sv0t5 = (opcode <= 52);
  if ((_sv0t4 && _sv0t5)) {
    return 11;
  } else {
  }
  int _sv0t6 = (opcode >= 64);
  int _sv0t7 = (opcode <= 69);
  if ((_sv0t6 && _sv0t7)) {
    return 12;
  } else {
  }
  int _sv0t8 = (opcode >= 80);
  int _sv0t9 = (opcode <= 82);
  if ((_sv0t8 && _sv0t9)) {
    return 13;
  } else {
  }
  int _sv0t10 = (opcode >= 88);
  int _sv0t11 = (opcode <= 93);
  if ((_sv0t10 && _sv0t11)) {
    return 14;
  } else {
  }
  if ((opcode == 96)) {
    return 15;
  } else {
  }
  if ((opcode == 97)) {
    return 16;
  } else {
  }
  int _sv0t12 = (opcode >= 112);
  int _sv0t13 = (opcode <= 114);
  if ((_sv0t12 && _sv0t13)) {
    return 17;
  } else {
  }
  if ((opcode == 115)) {
    return 18;
  } else {
  }
  if ((opcode == 116)) {
    return 19;
  } else {
  }
  if ((opcode == 117)) {
    return 20;
  } else {
  }
  if ((opcode == 118)) {
    return 21;
  } else {
  }
  if ((opcode == 128)) {
    return 22;
  } else {
  }
  if ((opcode == 129)) {
    return 23;
  } else {
  }
  if ((opcode == 130)) {
    return 24;
  } else {
  }
  if ((opcode == 131)) {
    return 25;
  } else {
  }
  if ((opcode == 132)) {
    return 26;
  } else {
  }
  if ((opcode == 133)) {
    return 27;
  } else {
  }
  if ((opcode == 144)) {
    return 28;
  } else {
  }
  if ((opcode == 145)) {
    return 29;
  } else {
  }
  if ((opcode == 146)) {
    return 30;
  } else {
  }
  if ((opcode == 160)) {
    return 31;
  } else {
  }
  if ((opcode == 161)) {
    return 32;
  } else {
  }
  int _sv0t14 = (-1);
  return _sv0t14;
}

static int decode_payload_bytes(int opcode) {
  if ((opcode == 4)) {
    return 4;
  } else {
  }
  if ((opcode == 5)) {
    return 8;
  } else {
  }
  if ((opcode == 6)) {
    return 8;
  } else {
  }
  if ((opcode == 7)) {
    return 1;
  } else {
  }
  if ((opcode == 8)) {
    return 4;
  } else {
  }
  int _sv0t0 = (opcode == 96);
  int _sv0t1 = (opcode == 97);
  if ((_sv0t0 || _sv0t1)) {
    return 4;
  } else {
  }
  int _sv0t2 = (opcode >= 112);
  int _sv0t3 = (opcode <= 114);
  if ((_sv0t2 && _sv0t3)) {
    return 4;
  } else {
  }
  if ((opcode == 115)) {
    return 8;
  } else {
  }
  if ((opcode == 117)) {
    return 4;
  } else {
  }
  if ((opcode == 118)) {
    return 1;
  } else {
  }
  int _sv0t4 = (opcode == 128);
  int _sv0t5 = (opcode == 129);
  int _sv0t6 = (_sv0t4 || _sv0t5);
  int _sv0t7 = (opcode == 130);
  int _sv0t8 = (_sv0t6 || _sv0t7);
  int _sv0t9 = (opcode == 131);
  if ((_sv0t8 || _sv0t9)) {
    return 4;
  } else {
  }
  if ((opcode == 144)) {
    return 12;
  } else {
  }
  if ((opcode == 146)) {
    return 4;
  } else {
  }
  if ((opcode == 160)) {
    return 4;
  } else {
  }
  if ((opcode == 161)) {
    return 4;
  } else {
  }
  return 0;
}

static int decode_total_bytes(int opcode) {
  int _sv0t0 = decode_payload_bytes(opcode);
  int _sv0t1 = (1 + _sv0t0);
  return _sv0t1;
}

static int decode_is_valid(int opcode) {
  int _sv0t0 = decode_insn_tag(opcode);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

int main(void) {
  int _sv0t0 = decode_insn_tag(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = decode_insn_tag(4);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = decode_insn_tag(16);
  if ((_sv0t2 != 9)) {
    return 3;
  } else {
  }
  int _sv0t3 = decode_insn_tag(64);
  if ((_sv0t3 != 12)) {
    return 4;
  } else {
  }
  int _sv0t4 = decode_insn_tag(115);
  if ((_sv0t4 != 18)) {
    return 5;
  } else {
  }
  int _sv0t5 = decode_insn_tag(144);
  if ((_sv0t5 != 28)) {
    return 6;
  } else {
  }
  int _sv0t6 = decode_insn_tag(161);
  if ((_sv0t6 != 32)) {
    return 7;
  } else {
  }
  int _sv0t7 = decode_insn_tag(255);
  int _sv0t8 = (-1);
  if ((_sv0t7 != _sv0t8)) {
    return 8;
  } else {
  }
  int _sv0t9 = decode_payload_bytes(0);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = decode_payload_bytes(4);
  if ((_sv0t10 != 4)) {
    return 11;
  } else {
  }
  int _sv0t11 = decode_payload_bytes(5);
  if ((_sv0t11 != 8)) {
    return 12;
  } else {
  }
  int _sv0t12 = decode_payload_bytes(7);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  int _sv0t13 = decode_payload_bytes(115);
  if ((_sv0t13 != 8)) {
    return 14;
  } else {
  }
  int _sv0t14 = decode_payload_bytes(144);
  if ((_sv0t14 != 12)) {
    return 15;
  } else {
  }
  int _sv0t15 = decode_payload_bytes(161);
  if ((_sv0t15 != 4)) {
    return 16;
  } else {
  }
  int _sv0t16 = decode_total_bytes(0);
  if ((_sv0t16 != 1)) {
    return 20;
  } else {
  }
  int _sv0t17 = decode_total_bytes(4);
  if ((_sv0t17 != 5)) {
    return 21;
  } else {
  }
  int _sv0t18 = decode_total_bytes(5);
  if ((_sv0t18 != 9)) {
    return 22;
  } else {
  }
  int _sv0t19 = decode_total_bytes(115);
  if ((_sv0t19 != 9)) {
    return 23;
  } else {
  }
  int _sv0t20 = decode_total_bytes(144);
  if ((_sv0t20 != 13)) {
    return 24;
  } else {
  }
  int _sv0t21 = decode_is_valid(0);
  if ((!_sv0t21)) {
    return 30;
  } else {
  }
  int _sv0t22 = decode_is_valid(116);
  if ((!_sv0t22)) {
    return 31;
  } else {
  }
  int _sv0t23 = decode_is_valid(255);
  if (_sv0t23) {
    return 32;
  } else {
  }
  int _sv0t24 = decode_is_valid(200);
  if (_sv0t24) {
    return 33;
  } else {
  }
  return 0;
}

