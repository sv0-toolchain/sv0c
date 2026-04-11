#include "sv0_runtime.h"

static int detect_base(int first, int second);
static int base_prefix_len(int base);
static int is_digit_for_base(int base, int c);
static int is_float_dot(int base, int dot_char, int next_after_dot);
static int is_exponent_start(int base, int c);
static int is_sign_char(int c);
static int number_result_tag(int is_float);

static int detect_base(int first, int second) {
  if ((first != 48)) {
    return 0;
  } else {
  }
  int _sv0t0 = (second == 120);
  int _sv0t1 = (second == 88);
  if ((_sv0t0 || _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = (second == 98);
  int _sv0t3 = (second == 66);
  if ((_sv0t2 || _sv0t3)) {
    return 2;
  } else {
  }
  int _sv0t4 = (second == 111);
  int _sv0t5 = (second == 79);
  if ((_sv0t4 || _sv0t5)) {
    return 3;
  } else {
  }
  return 0;
}

static int base_prefix_len(int base) {
  if ((base == 0)) {
    return 0;
  } else {
  }
  return 2;
}

static int is_digit_for_base(int base, int c) {
  if ((base == 1)) {
    int _sv0t0 = (c >= 48);
    int _sv0t1 = (c <= 57);
    if ((_sv0t0 && _sv0t1)) {
      return 1;
    } else {
    }
    int _sv0t2 = (c >= 65);
    int _sv0t3 = (c <= 70);
    if ((_sv0t2 && _sv0t3)) {
      return 1;
    } else {
    }
    int _sv0t4 = (c >= 97);
    int _sv0t5 = (c <= 102);
    if ((_sv0t4 && _sv0t5)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((base == 2)) {
    int _sv0t6 = (c == 48);
    int _sv0t7 = (c == 49);
    int _sv0t8 = (_sv0t6 || _sv0t7);
    return _sv0t8;
  } else {
  }
  if ((base == 3)) {
    int _sv0t9 = (c >= 48);
    int _sv0t10 = (c <= 55);
    int _sv0t11 = (_sv0t9 && _sv0t10);
    return _sv0t11;
  } else {
  }
  int _sv0t12 = (c >= 48);
  int _sv0t13 = (c <= 57);
  int _sv0t14 = (_sv0t12 && _sv0t13);
  return _sv0t14;
}

static int is_float_dot(int base, int dot_char, int next_after_dot) {
  if ((base != 0)) {
    return 0;
  } else {
  }
  if ((dot_char != 46)) {
    return 0;
  } else {
  }
  int _sv0t0 = (next_after_dot >= 48);
  int _sv0t1 = (next_after_dot <= 57);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int is_exponent_start(int base, int c) {
  if ((base != 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = (c == 101);
  int _sv0t1 = (c == 69);
  int _sv0t2 = (_sv0t0 || _sv0t1);
  return _sv0t2;
}

static int is_sign_char(int c) {
  int _sv0t0 = (c == 43);
  int _sv0t1 = (c == 45);
  int _sv0t2 = (_sv0t0 || _sv0t1);
  return _sv0t2;
}

static int number_result_tag(int is_float) {
  if (is_float) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = detect_base(48, 120);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = detect_base(48, 88);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = detect_base(48, 98);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = detect_base(48, 111);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = detect_base(48, 48);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = detect_base(49, 120);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = base_prefix_len(0);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = base_prefix_len(1);
  if ((_sv0t7 != 2)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_digit_for_base(0, 53);
  if ((!_sv0t8)) {
    return 10;
  } else {
  }
  int _sv0t9 = is_digit_for_base(0, 65);
  if (_sv0t9) {
    return 11;
  } else {
  }
  int _sv0t10 = is_digit_for_base(1, 65);
  if ((!_sv0t10)) {
    return 12;
  } else {
  }
  int _sv0t11 = is_digit_for_base(1, 102);
  if ((!_sv0t11)) {
    return 13;
  } else {
  }
  int _sv0t12 = is_digit_for_base(2, 50);
  if (_sv0t12) {
    return 14;
  } else {
  }
  int _sv0t13 = is_digit_for_base(2, 49);
  if ((!_sv0t13)) {
    return 15;
  } else {
  }
  int _sv0t14 = is_digit_for_base(3, 55);
  if ((!_sv0t14)) {
    return 16;
  } else {
  }
  int _sv0t15 = is_digit_for_base(3, 56);
  if (_sv0t15) {
    return 17;
  } else {
  }
  int _sv0t16 = is_float_dot(0, 46, 53);
  if ((!_sv0t16)) {
    return 20;
  } else {
  }
  int _sv0t17 = is_float_dot(1, 46, 53);
  if (_sv0t17) {
    return 21;
  } else {
  }
  int _sv0t18 = is_float_dot(0, 46, 65);
  if (_sv0t18) {
    return 22;
  } else {
  }
  int _sv0t19 = is_float_dot(0, 44, 53);
  if (_sv0t19) {
    return 23;
  } else {
  }
  int _sv0t20 = is_exponent_start(0, 101);
  if ((!_sv0t20)) {
    return 25;
  } else {
  }
  int _sv0t21 = is_exponent_start(0, 69);
  if ((!_sv0t21)) {
    return 26;
  } else {
  }
  int _sv0t22 = is_exponent_start(1, 101);
  if (_sv0t22) {
    return 27;
  } else {
  }
  int _sv0t23 = is_sign_char(43);
  if ((!_sv0t23)) {
    return 28;
  } else {
  }
  int _sv0t24 = is_sign_char(45);
  if ((!_sv0t24)) {
    return 29;
  } else {
  }
  int _sv0t25 = is_sign_char(48);
  if (_sv0t25) {
    return 30;
  } else {
  }
  int _sv0t26 = number_result_tag(1);
  if ((_sv0t26 != 1)) {
    return 31;
  } else {
  }
  int _sv0t27 = number_result_tag(0);
  if ((_sv0t27 != 0)) {
    return 32;
  } else {
  }
  return 0;
}

