#include "sv0_runtime.h"

static int type_dispatch_tag(int token_tag);
static int is_primitive_type_name(int name_tag);
static int tuple_or_single(int comma_count);
static int ref_mut_or_plain(int has_mut);
static int array_or_slice(int has_semicolon);
static int type_arg_count(int has_lt, int p0, int p1, int p2, int has_gt);

static int type_dispatch_tag(int token_tag) {
  if ((token_tag == 40)) {
    return 0;
  } else {
  }
  if ((token_tag == 38)) {
    return 1;
  } else {
  }
  if ((token_tag == 91)) {
    return 2;
  } else {
  }
  if ((token_tag == 80)) {
    return 3;
  } else {
  }
  if ((token_tag == 1)) {
    return 4;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int is_primitive_type_name(int name_tag) {
  int _sv0t0 = (name_tag >= 1);
  int _sv0t1 = (name_tag <= 10);
  if ((_sv0t0 && _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = (name_tag == 11);
  int _sv0t3 = (name_tag == 12);
  if ((_sv0t2 || _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = (name_tag >= 13);
  int _sv0t5 = (name_tag <= 18);
  if ((_sv0t4 && _sv0t5)) {
    return 1;
  } else {
  }
  return 0;
}

static int tuple_or_single(int comma_count) {
  if ((comma_count == 0)) {
    return 0;
  } else {
  }
  return 1;
}

static int ref_mut_or_plain(int has_mut) {
  if (has_mut) {
    return 1;
  } else {
  }
  return 0;
}

static int array_or_slice(int has_semicolon) {
  if (has_semicolon) {
    return 0;
  } else {
  }
  return 1;
}

static int type_arg_count(int has_lt, int p0, int p1, int p2, int has_gt) {
  if ((!has_lt)) {
    return 0;
  } else {
  }
  if ((!has_gt)) {
    return 0;
  } else {
  }
  int count = 0;
  if (p0) {
    count = (count + 1);
  } else {
  }
  if (p1) {
    count = (count + 1);
  } else {
  }
  if (p2) {
    count = (count + 1);
  } else {
  }
  return count;
}

int main(void) {
  int _sv0t0 = type_dispatch_tag(40);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = type_dispatch_tag(38);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = type_dispatch_tag(91);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = type_dispatch_tag(80);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = type_dispatch_tag(1);
  if ((_sv0t4 != 4)) {
    return 5;
  } else {
  }
  int _sv0t5 = type_dispatch_tag(99);
  int _sv0t6 = (-1);
  if ((_sv0t5 != _sv0t6)) {
    return 6;
  } else {
  }
  int _sv0t7 = is_primitive_type_name(1);
  if ((!_sv0t7)) {
    return 7;
  } else {
  }
  int _sv0t8 = is_primitive_type_name(18);
  if ((!_sv0t8)) {
    return 8;
  } else {
  }
  int _sv0t9 = is_primitive_type_name(0);
  if (_sv0t9) {
    return 9;
  } else {
  }
  int _sv0t10 = is_primitive_type_name(19);
  if (_sv0t10) {
    return 10;
  } else {
  }
  int _sv0t11 = tuple_or_single(0);
  if ((_sv0t11 != 0)) {
    return 11;
  } else {
  }
  int _sv0t12 = tuple_or_single(1);
  if ((_sv0t12 != 1)) {
    return 12;
  } else {
  }
  int _sv0t13 = ref_mut_or_plain(1);
  if ((_sv0t13 != 1)) {
    return 13;
  } else {
  }
  int _sv0t14 = ref_mut_or_plain(0);
  if ((_sv0t14 != 0)) {
    return 14;
  } else {
  }
  int _sv0t15 = array_or_slice(1);
  if ((_sv0t15 != 0)) {
    return 15;
  } else {
  }
  int _sv0t16 = array_or_slice(0);
  if ((_sv0t16 != 1)) {
    return 16;
  } else {
  }
  int _sv0t17 = type_arg_count(0, 0, 0, 0, 0);
  if ((_sv0t17 != 0)) {
    return 17;
  } else {
  }
  int _sv0t18 = type_arg_count(1, 1, 0, 0, 1);
  if ((_sv0t18 != 1)) {
    return 18;
  } else {
  }
  int _sv0t19 = type_arg_count(1, 1, 1, 1, 1);
  if ((_sv0t19 != 3)) {
    return 19;
  } else {
  }
  return 0;
}

