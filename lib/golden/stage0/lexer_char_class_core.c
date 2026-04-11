#include "sv0_runtime.h"

static int is_ident_start(int c);
static int is_ident_cont(int c);
static int is_digit(int c);
static int is_hex_digit(int c);
static int is_bin_digit(int c);
static int is_oct_digit(int c);
static int is_whitespace(int c);
static int test_ident_start(void);
static int test_ident_cont(void);
static int test_hex_digit(void);
static int test_bin_oct(void);
static int test_whitespace(void);

static int is_ident_start(int c) {
  if ((c >= 65)) {
    if ((c <= 90)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((c >= 97)) {
    if ((c <= 122)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((c == 95)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_ident_cont(int c) {
  int _sv0t0 = is_ident_start(c);
  if (_sv0t0) {
    return 1;
  } else {
  }
  if ((c >= 48)) {
    if ((c <= 57)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_digit(int c) {
  if ((c >= 48)) {
    if ((c <= 57)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_hex_digit(int c) {
  int _sv0t0 = is_digit(c);
  if (_sv0t0) {
    return 1;
  } else {
  }
  if ((c >= 65)) {
    if ((c <= 70)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((c >= 97)) {
    if ((c <= 102)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_bin_digit(int c) {
  if ((c == 48)) {
    return 1;
  } else {
  }
  if ((c == 49)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_oct_digit(int c) {
  if ((c >= 48)) {
    if ((c <= 55)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_whitespace(int c) {
  if ((c == 32)) {
    return 1;
  } else {
  }
  if ((c == 9)) {
    return 1;
  } else {
  }
  if ((c == 10)) {
    return 1;
  } else {
  }
  if ((c == 13)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_ident_start(void) {
  int _sv0t0 = is_ident_start(65);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_ident_start(90);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_ident_start(97);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_ident_start(122);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_ident_start(95);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_ident_start(48);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_ident_start(32);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_ident_start(64);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_ident_cont(void) {
  int _sv0t0 = is_ident_cont(65);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_ident_cont(95);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_ident_cont(48);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_ident_cont(57);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_ident_cont(32);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_ident_cont(64);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_hex_digit(void) {
  int _sv0t0 = is_hex_digit(48);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_hex_digit(57);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_hex_digit(65);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_hex_digit(70);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_hex_digit(97);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_hex_digit(102);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_hex_digit(103);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_hex_digit(71);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_bin_oct(void) {
  int _sv0t0 = is_bin_digit(48);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_bin_digit(49);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_bin_digit(50);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_oct_digit(48);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_oct_digit(55);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_oct_digit(56);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_whitespace(void) {
  int _sv0t0 = is_whitespace(32);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_whitespace(9);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_whitespace(10);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_whitespace(13);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_whitespace(65);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_ident_start();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_ident_cont();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_hex_digit();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_bin_oct();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_whitespace();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  return 0;
}

