#include "sv0_runtime.h"

static int escape_result(int c);
static int escape_is_hex(int c);
static int escape_chars_consumed(int c);
static int hex_digit_value(int c);
static int hex_escape_value(int d1, int d2);
static int string_scan_advance(int c);
static int count_string_chars(int c0, int c1, int c2, int c3);

static int escape_result(int c) {
  if ((c == 110)) {
    return 10;
  } else {
  }
  if ((c == 116)) {
    return 9;
  } else {
  }
  if ((c == 114)) {
    return 13;
  } else {
  }
  if ((c == 92)) {
    return 92;
  } else {
  }
  if ((c == 34)) {
    return 34;
  } else {
  }
  if ((c == 39)) {
    return 39;
  } else {
  }
  if ((c == 48)) {
    return 0;
  } else {
  }
  if ((c == 120)) {
    int _sv0t0 = (-1);
    return _sv0t0;
  } else {
  }
  return c;
}

static int escape_is_hex(int c) {
  int _sv0t0 = (c == 120);
  return _sv0t0;
}

static int escape_chars_consumed(int c) {
  if ((c == 120)) {
    return 4;
  } else {
  }
  return 2;
}

static int hex_digit_value(int c) {
  int _sv0t0 = (c >= 48);
  int _sv0t1 = (c <= 57);
  if ((_sv0t0 && _sv0t1)) {
    int _sv0t2 = (c - 48);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = (c >= 65);
  int _sv0t4 = (c <= 70);
  if ((_sv0t3 && _sv0t4)) {
    int _sv0t5 = (c - 55);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = (c >= 97);
  int _sv0t7 = (c <= 102);
  if ((_sv0t6 && _sv0t7)) {
    int _sv0t8 = (c - 87);
    return _sv0t8;
  } else {
  }
  return 0;
}

static int hex_escape_value(int d1, int d2) {
  int _sv0t0 = hex_digit_value(d1);
  int _sv0t1 = (_sv0t0 * 16);
  int _sv0t2 = hex_digit_value(d2);
  int _sv0t3 = (_sv0t1 + _sv0t2);
  return _sv0t3;
}

static int string_scan_advance(int c) {
  if ((c == 34)) {
    return 0;
  } else {
  }
  if ((c == 92)) {
    return 2;
  } else {
  }
  return 1;
}

static int count_string_chars(int c0, int c1, int c2, int c3) {
  int count = 0;
  int _sv0t0 = (c0 != 34);
  int _sv0t1 = (c0 != 0);
  if ((_sv0t0 && _sv0t1)) {
    count = (count + 1);
  } else {
  }
  int _sv0t2 = (c1 != 34);
  int _sv0t3 = (c1 != 0);
  if ((_sv0t2 && _sv0t3)) {
    count = (count + 1);
  } else {
  }
  int _sv0t4 = (c2 != 34);
  int _sv0t5 = (c2 != 0);
  if ((_sv0t4 && _sv0t5)) {
    count = (count + 1);
  } else {
  }
  int _sv0t6 = (c3 != 34);
  int _sv0t7 = (c3 != 0);
  if ((_sv0t6 && _sv0t7)) {
    count = (count + 1);
  } else {
  }
  return count;
}

int main(void) {
  int _sv0t0 = escape_result(110);
  if ((_sv0t0 != 10)) {
    return 1;
  } else {
  }
  int _sv0t1 = escape_result(116);
  if ((_sv0t1 != 9)) {
    return 2;
  } else {
  }
  int _sv0t2 = escape_result(114);
  if ((_sv0t2 != 13)) {
    return 3;
  } else {
  }
  int _sv0t3 = escape_result(92);
  if ((_sv0t3 != 92)) {
    return 4;
  } else {
  }
  int _sv0t4 = escape_result(34);
  if ((_sv0t4 != 34)) {
    return 5;
  } else {
  }
  int _sv0t5 = escape_result(48);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = escape_result(120);
  int _sv0t7 = (-1);
  if ((_sv0t6 != _sv0t7)) {
    return 7;
  } else {
  }
  int _sv0t8 = escape_result(65);
  if ((_sv0t8 != 65)) {
    return 8;
  } else {
  }
  int _sv0t9 = escape_is_hex(120);
  if ((!_sv0t9)) {
    return 10;
  } else {
  }
  int _sv0t10 = escape_is_hex(110);
  if (_sv0t10) {
    return 11;
  } else {
  }
  int _sv0t11 = escape_chars_consumed(110);
  if ((_sv0t11 != 2)) {
    return 12;
  } else {
  }
  int _sv0t12 = escape_chars_consumed(120);
  if ((_sv0t12 != 4)) {
    return 13;
  } else {
  }
  int _sv0t13 = hex_digit_value(48);
  if ((_sv0t13 != 0)) {
    return 14;
  } else {
  }
  int _sv0t14 = hex_digit_value(57);
  if ((_sv0t14 != 9)) {
    return 15;
  } else {
  }
  int _sv0t15 = hex_digit_value(65);
  if ((_sv0t15 != 10)) {
    return 16;
  } else {
  }
  int _sv0t16 = hex_digit_value(70);
  if ((_sv0t16 != 15)) {
    return 17;
  } else {
  }
  int _sv0t17 = hex_digit_value(97);
  if ((_sv0t17 != 10)) {
    return 18;
  } else {
  }
  int _sv0t18 = hex_escape_value(52, 49);
  if ((_sv0t18 != 65)) {
    return 19;
  } else {
  }
  int _sv0t19 = hex_escape_value(48, 65);
  if ((_sv0t19 != 10)) {
    return 20;
  } else {
  }
  int _sv0t20 = string_scan_advance(34);
  if ((_sv0t20 != 0)) {
    return 21;
  } else {
  }
  int _sv0t21 = string_scan_advance(92);
  if ((_sv0t21 != 2)) {
    return 22;
  } else {
  }
  int _sv0t22 = string_scan_advance(65);
  if ((_sv0t22 != 1)) {
    return 23;
  } else {
  }
  int _sv0t23 = count_string_chars(72, 105, 34, 0);
  if ((_sv0t23 != 2)) {
    return 24;
  } else {
  }
  int _sv0t24 = count_string_chars(65, 66, 67, 68);
  if ((_sv0t24 != 4)) {
    return 25;
  } else {
  }
  return 0;
}

