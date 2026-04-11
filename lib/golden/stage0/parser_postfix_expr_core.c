#include "sv0_runtime.h"

static int postfix_kind(int token_tag, int next_tag, int third_tag);
static int is_postfix_start(int token_tag);
static int postfix_consumes_tokens(int kind);
static int postfix_needs_closing(int kind);
static int closing_token_for(int kind);

static int postfix_kind(int token_tag, int next_tag, int third_tag) {
  if ((token_tag == 40)) {
    return 0;
  } else {
  }
  int _sv0t0 = (token_tag == 46);
  int _sv0t1 = (next_tag == 1);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  int _sv0t3 = (third_tag == 40);
  if ((_sv0t2 && _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = (token_tag == 46);
  int _sv0t5 = (next_tag == 1);
  if ((_sv0t4 && _sv0t5)) {
    return 2;
  } else {
  }
  int _sv0t6 = (token_tag == 46);
  int _sv0t7 = (next_tag == 7);
  if ((_sv0t6 && _sv0t7)) {
    return 3;
  } else {
  }
  if ((token_tag == 91)) {
    return 4;
  } else {
  }
  if ((token_tag == 63)) {
    return 5;
  } else {
  }
  int _sv0t8 = (-1);
  return _sv0t8;
}

static int is_postfix_start(int token_tag) {
  if ((token_tag == 40)) {
    return 1;
  } else {
  }
  if ((token_tag == 46)) {
    return 1;
  } else {
  }
  if ((token_tag == 91)) {
    return 1;
  } else {
  }
  if ((token_tag == 63)) {
    return 1;
  } else {
  }
  return 0;
}

static int postfix_consumes_tokens(int kind) {
  if ((kind == 0)) {
    return 1;
  } else {
  }
  if ((kind == 1)) {
    return 3;
  } else {
  }
  if ((kind == 2)) {
    return 2;
  } else {
  }
  if ((kind == 3)) {
    return 2;
  } else {
  }
  if ((kind == 4)) {
    return 1;
  } else {
  }
  if ((kind == 5)) {
    return 1;
  } else {
  }
  return 0;
}

static int postfix_needs_closing(int kind) {
  if ((kind == 0)) {
    return 1;
  } else {
  }
  if ((kind == 1)) {
    return 1;
  } else {
  }
  if ((kind == 4)) {
    return 1;
  } else {
  }
  return 0;
}

static int closing_token_for(int kind) {
  if ((kind == 0)) {
    return 41;
  } else {
  }
  if ((kind == 1)) {
    return 41;
  } else {
  }
  if ((kind == 4)) {
    return 93;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = postfix_kind(40, 0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = postfix_kind(46, 1, 40);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = postfix_kind(46, 1, 0);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = postfix_kind(46, 7, 0);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = postfix_kind(91, 0, 0);
  if ((_sv0t4 != 4)) {
    return 5;
  } else {
  }
  int _sv0t5 = postfix_kind(63, 0, 0);
  if ((_sv0t5 != 5)) {
    return 6;
  } else {
  }
  int _sv0t6 = postfix_kind(44, 0, 0);
  int _sv0t7 = (-1);
  if ((_sv0t6 != _sv0t7)) {
    return 7;
  } else {
  }
  int _sv0t8 = is_postfix_start(40);
  if ((!_sv0t8)) {
    return 8;
  } else {
  }
  int _sv0t9 = is_postfix_start(46);
  if ((!_sv0t9)) {
    return 9;
  } else {
  }
  int _sv0t10 = is_postfix_start(63);
  if ((!_sv0t10)) {
    return 10;
  } else {
  }
  int _sv0t11 = is_postfix_start(44);
  if (_sv0t11) {
    return 11;
  } else {
  }
  int _sv0t12 = postfix_consumes_tokens(0);
  if ((_sv0t12 != 1)) {
    return 12;
  } else {
  }
  int _sv0t13 = postfix_consumes_tokens(1);
  if ((_sv0t13 != 3)) {
    return 13;
  } else {
  }
  int _sv0t14 = postfix_consumes_tokens(2);
  if ((_sv0t14 != 2)) {
    return 14;
  } else {
  }
  int _sv0t15 = postfix_needs_closing(0);
  if ((!_sv0t15)) {
    return 15;
  } else {
  }
  int _sv0t16 = postfix_needs_closing(4);
  if ((!_sv0t16)) {
    return 16;
  } else {
  }
  int _sv0t17 = postfix_needs_closing(2);
  if (_sv0t17) {
    return 17;
  } else {
  }
  int _sv0t18 = postfix_needs_closing(5);
  if (_sv0t18) {
    return 18;
  } else {
  }
  int _sv0t19 = closing_token_for(0);
  if ((_sv0t19 != 41)) {
    return 19;
  } else {
  }
  int _sv0t20 = closing_token_for(4);
  if ((_sv0t20 != 93)) {
    return 20;
  } else {
  }
  return 0;
}

