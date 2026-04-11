#include "sv0_runtime.h"

static int is_ident_start(int c);
static int is_ident_cont(int c);
static int scan_ident_length(int c0, int c1, int c2, int c3, int c4);
static int ident_result_tag(int is_keyword);
static int ident_token_class(int first_char);

static int is_ident_start(int c) {
  int _sv0t0 = (c >= 97);
  int _sv0t1 = (c <= 122);
  if ((_sv0t0 && _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = (c >= 65);
  int _sv0t3 = (c <= 90);
  if ((_sv0t2 && _sv0t3)) {
    return 1;
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
  int _sv0t1 = (c >= 48);
  int _sv0t2 = (c <= 57);
  if ((_sv0t1 && _sv0t2)) {
    return 1;
  } else {
  }
  return 0;
}

static int scan_ident_length(int c0, int c1, int c2, int c3, int c4) {
  int _sv0t0 = is_ident_start(c0);
  if ((!_sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = is_ident_cont(c1);
  if ((!_sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = is_ident_cont(c2);
  if ((!_sv0t2)) {
    return 2;
  } else {
  }
  int _sv0t3 = is_ident_cont(c3);
  if ((!_sv0t3)) {
    return 3;
  } else {
  }
  int _sv0t4 = is_ident_cont(c4);
  if ((!_sv0t4)) {
    return 4;
  } else {
  }
  return 5;
}

static int ident_result_tag(int is_keyword) {
  if (is_keyword) {
    return 1;
  } else {
  }
  return 0;
}

static int ident_token_class(int first_char) {
  if ((first_char == 95)) {
    return 2;
  } else {
  }
  int _sv0t0 = (first_char >= 65);
  int _sv0t1 = (first_char <= 90);
  if ((_sv0t0 && _sv0t1)) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = is_ident_start(97);
  if ((!_sv0t0)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_ident_start(65);
  if ((!_sv0t1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_ident_start(95);
  if ((!_sv0t2)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_ident_start(48);
  if (_sv0t3) {
    return 4;
  } else {
  }
  int _sv0t4 = is_ident_start(32);
  if (_sv0t4) {
    return 5;
  } else {
  }
  int _sv0t5 = is_ident_cont(97);
  if ((!_sv0t5)) {
    return 10;
  } else {
  }
  int _sv0t6 = is_ident_cont(48);
  if ((!_sv0t6)) {
    return 11;
  } else {
  }
  int _sv0t7 = is_ident_cont(95);
  if ((!_sv0t7)) {
    return 12;
  } else {
  }
  int _sv0t8 = is_ident_cont(32);
  if (_sv0t8) {
    return 13;
  } else {
  }
  int _sv0t9 = scan_ident_length(102, 110, 32, 0, 0);
  int len1 = _sv0t9;
  if ((len1 != 2)) {
    return 20;
  } else {
  }
  int _sv0t10 = scan_ident_length(120, 32, 0, 0, 0);
  int len2 = _sv0t10;
  if ((len2 != 1)) {
    return 21;
  } else {
  }
  int _sv0t11 = scan_ident_length(32, 0, 0, 0, 0);
  int len3 = _sv0t11;
  if ((len3 != 0)) {
    return 22;
  } else {
  }
  int _sv0t12 = scan_ident_length(97, 98, 99, 100, 101);
  int len4 = _sv0t12;
  if ((len4 != 5)) {
    return 23;
  } else {
  }
  int _sv0t13 = ident_result_tag(1);
  if ((_sv0t13 != 1)) {
    return 30;
  } else {
  }
  int _sv0t14 = ident_result_tag(0);
  if ((_sv0t14 != 0)) {
    return 31;
  } else {
  }
  int _sv0t15 = ident_token_class(95);
  if ((_sv0t15 != 2)) {
    return 40;
  } else {
  }
  int _sv0t16 = ident_token_class(65);
  if ((_sv0t16 != 1)) {
    return 41;
  } else {
  }
  int _sv0t17 = ident_token_class(97);
  if ((_sv0t17 != 0)) {
    return 42;
  } else {
  }
  return 0;
}

