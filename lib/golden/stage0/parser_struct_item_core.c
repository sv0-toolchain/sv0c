#include "sv0_runtime.h"

static int struct_field_parts(void);
static int struct_field_separator(int next_tag);
static int struct_has_generic_params(int has_lt);
static int count_fields(int f0, int f1, int f2, int f3, int f4, int f5, int f6, int f7);
static int struct_item_ast_tag(void);
static int struct_expects_lbrace(void);
static int field_may_have_pub(int has_pub);
static int struct_span_parts(void);

static int struct_field_parts(void) {
  return 3;
}

static int struct_field_separator(int next_tag) {
  if ((next_tag == 50)) {
    return 0;
  } else {
  }
  if ((next_tag == 51)) {
    return 1;
  } else {
  }
  return 2;
}

static int struct_has_generic_params(int has_lt) {
  return has_lt;
}

static int count_fields(int f0, int f1, int f2, int f3, int f4, int f5, int f6, int f7) {
  int count = 0;
  if (f0) {
    count = (count + 1);
  } else {
  }
  if (f1) {
    count = (count + 1);
  } else {
  }
  if (f2) {
    count = (count + 1);
  } else {
  }
  if (f3) {
    count = (count + 1);
  } else {
  }
  if (f4) {
    count = (count + 1);
  } else {
  }
  if (f5) {
    count = (count + 1);
  } else {
  }
  if (f6) {
    count = (count + 1);
  } else {
  }
  if (f7) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int struct_item_ast_tag(void) {
  return 20;
}

static int struct_expects_lbrace(void) {
  return 1;
}

static int field_may_have_pub(int has_pub) {
  return has_pub;
}

static int struct_span_parts(void) {
  return 2;
}

int main(void) {
  int _sv0t0 = struct_field_parts();
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = struct_field_separator(50);
  if ((_sv0t1 != 0)) {
    return 10;
  } else {
  }
  int _sv0t2 = struct_field_separator(51);
  if ((_sv0t2 != 1)) {
    return 11;
  } else {
  }
  int _sv0t3 = struct_field_separator(52);
  if ((_sv0t3 != 2)) {
    return 12;
  } else {
  }
  int _sv0t4 = struct_has_generic_params(1);
  if ((!_sv0t4)) {
    return 20;
  } else {
  }
  int _sv0t5 = struct_has_generic_params(0);
  if (_sv0t5) {
    return 21;
  } else {
  }
  int _sv0t6 = count_fields(1, 1, 1, 0, 0, 0, 0, 0);
  if ((_sv0t6 != 3)) {
    return 30;
  } else {
  }
  int _sv0t7 = count_fields(1, 1, 1, 1, 1, 1, 1, 1);
  if ((_sv0t7 != 8)) {
    return 31;
  } else {
  }
  int _sv0t8 = count_fields(0, 0, 0, 0, 0, 0, 0, 0);
  if ((_sv0t8 != 0)) {
    return 32;
  } else {
  }
  int _sv0t9 = struct_item_ast_tag();
  if ((_sv0t9 != 20)) {
    return 40;
  } else {
  }
  int _sv0t10 = struct_expects_lbrace();
  if ((!_sv0t10)) {
    return 50;
  } else {
  }
  int _sv0t11 = field_may_have_pub(1);
  if ((!_sv0t11)) {
    return 60;
  } else {
  }
  int _sv0t12 = field_may_have_pub(0);
  if (_sv0t12) {
    return 61;
  } else {
  }
  int _sv0t13 = struct_span_parts();
  if ((_sv0t13 != 2)) {
    return 70;
  } else {
  }
  return 0;
}

