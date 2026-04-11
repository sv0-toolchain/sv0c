#include "sv0_runtime.h"

static int keyword_token_tag(int name_tag);
static int is_keyword(int name_tag);
static int keyword_count(void);
static int is_control_flow_keyword(int tok_tag);
static int is_type_keyword(int tok_tag);

static int keyword_token_tag(int name_tag) {
  if ((name_tag == 1)) {
    return 101;
  } else {
  }
  if ((name_tag == 2)) {
    return 102;
  } else {
  }
  if ((name_tag == 3)) {
    return 103;
  } else {
  }
  if ((name_tag == 4)) {
    return 104;
  } else {
  }
  if ((name_tag == 5)) {
    return 105;
  } else {
  }
  if ((name_tag == 6)) {
    return 106;
  } else {
  }
  if ((name_tag == 7)) {
    return 107;
  } else {
  }
  if ((name_tag == 8)) {
    return 108;
  } else {
  }
  if ((name_tag == 9)) {
    return 109;
  } else {
  }
  if ((name_tag == 10)) {
    return 110;
  } else {
  }
  if ((name_tag == 11)) {
    return 111;
  } else {
  }
  if ((name_tag == 12)) {
    return 112;
  } else {
  }
  if ((name_tag == 13)) {
    return 113;
  } else {
  }
  if ((name_tag == 14)) {
    return 114;
  } else {
  }
  if ((name_tag == 15)) {
    return 115;
  } else {
  }
  if ((name_tag == 16)) {
    return 116;
  } else {
  }
  if ((name_tag == 17)) {
    return 117;
  } else {
  }
  if ((name_tag == 18)) {
    return 118;
  } else {
  }
  if ((name_tag == 19)) {
    return 119;
  } else {
  }
  if ((name_tag == 20)) {
    return 120;
  } else {
  }
  if ((name_tag == 21)) {
    return 121;
  } else {
  }
  if ((name_tag == 22)) {
    return 122;
  } else {
  }
  if ((name_tag == 23)) {
    return 123;
  } else {
  }
  if ((name_tag == 24)) {
    return 124;
  } else {
  }
  if ((name_tag == 25)) {
    return 125;
  } else {
  }
  if ((name_tag == 26)) {
    return 126;
  } else {
  }
  if ((name_tag == 27)) {
    return 127;
  } else {
  }
  if ((name_tag == 28)) {
    return 128;
  } else {
  }
  if ((name_tag == 29)) {
    return 129;
  } else {
  }
  if ((name_tag == 30)) {
    return 130;
  } else {
  }
  if ((name_tag == 31)) {
    return 131;
  } else {
  }
  if ((name_tag == 32)) {
    return 132;
  } else {
  }
  if ((name_tag == 33)) {
    return 133;
  } else {
  }
  if ((name_tag == 34)) {
    return 134;
  } else {
  }
  if ((name_tag == 35)) {
    return 135;
  } else {
  }
  if ((name_tag == 36)) {
    return 136;
  } else {
  }
  if ((name_tag == 37)) {
    return 137;
  } else {
  }
  if ((name_tag == 38)) {
    return 138;
  } else {
  }
  if ((name_tag == 39)) {
    return 139;
  } else {
  }
  if ((name_tag == 40)) {
    return 140;
  } else {
  }
  if ((name_tag == 41)) {
    return 141;
  } else {
  }
  if ((name_tag == 42)) {
    return 142;
  } else {
  }
  if ((name_tag == 43)) {
    return 143;
  } else {
  }
  return 0;
}

static int is_keyword(int name_tag) {
  int _sv0t0 = (name_tag >= 1);
  int _sv0t1 = (name_tag <= 43);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int keyword_count(void) {
  return 43;
}

static int is_control_flow_keyword(int tok_tag) {
  if ((tok_tag == 115)) {
    return 1;
  } else {
  }
  if ((tok_tag == 107)) {
    return 1;
  } else {
  }
  if ((tok_tag == 121)) {
    return 1;
  } else {
  }
  if ((tok_tag == 143)) {
    return 1;
  } else {
  }
  if ((tok_tag == 113)) {
    return 1;
  } else {
  }
  if ((tok_tag == 119)) {
    return 1;
  } else {
  }
  if ((tok_tag == 132)) {
    return 1;
  } else {
  }
  if ((tok_tag == 104)) {
    return 1;
  } else {
  }
  if ((tok_tag == 106)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_type_keyword(int tok_tag) {
  if ((tok_tag == 136)) {
    return 1;
  } else {
  }
  if ((tok_tag == 109)) {
    return 1;
  } else {
  }
  if ((tok_tag == 137)) {
    return 1;
  } else {
  }
  if ((tok_tag == 139)) {
    return 1;
  } else {
  }
  if ((tok_tag == 116)) {
    return 1;
  } else {
  }
  if ((tok_tag == 125)) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = keyword_token_tag(12);
  if ((_sv0t0 != 112)) {
    return 1;
  } else {
  }
  int _sv0t1 = keyword_token_tag(18);
  if ((_sv0t1 != 118)) {
    return 2;
  } else {
  }
  int _sv0t2 = keyword_token_tag(15);
  if ((_sv0t2 != 115)) {
    return 3;
  } else {
  }
  int _sv0t3 = keyword_token_tag(99);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = keyword_token_tag(0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = keyword_token_tag(43);
  if ((_sv0t5 != 143)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_keyword(1);
  if ((!_sv0t6)) {
    return 10;
  } else {
  }
  int _sv0t7 = is_keyword(43);
  if ((!_sv0t7)) {
    return 11;
  } else {
  }
  int _sv0t8 = is_keyword(0);
  if (_sv0t8) {
    return 12;
  } else {
  }
  int _sv0t9 = is_keyword(44);
  if (_sv0t9) {
    return 13;
  } else {
  }
  int _sv0t10 = keyword_count();
  if ((_sv0t10 != 43)) {
    return 20;
  } else {
  }
  int _sv0t11 = is_control_flow_keyword(115);
  if ((!_sv0t11)) {
    return 30;
  } else {
  }
  int _sv0t12 = is_control_flow_keyword(121);
  if ((!_sv0t12)) {
    return 31;
  } else {
  }
  int _sv0t13 = is_control_flow_keyword(112);
  if (_sv0t13) {
    return 32;
  } else {
  }
  int _sv0t14 = is_control_flow_keyword(136);
  if (_sv0t14) {
    return 33;
  } else {
  }
  int _sv0t15 = is_type_keyword(136);
  if ((!_sv0t15)) {
    return 40;
  } else {
  }
  int _sv0t16 = is_type_keyword(109);
  if ((!_sv0t16)) {
    return 41;
  } else {
  }
  int _sv0t17 = is_type_keyword(115);
  if (_sv0t17) {
    return 42;
  } else {
  }
  return 0;
}

