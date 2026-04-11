#include "sv0_runtime.h"

static int variant_kind_from_token(int after_name_tag);
static int variant_has_payload(int kind);
static int variant_field_count(int kind, int tuple_fields, int struct_fields);
static int count_variants(int v0, int v1, int v2, int v3, int v4, int v5);
static int is_variant_separator(int token_tag);
static int is_enum_end(int token_tag);
static int enum_needs_generic_params(int has_lt);

static int variant_kind_from_token(int after_name_tag) {
  if ((after_name_tag == 40)) {
    return 1;
  } else {
  }
  if ((after_name_tag == 123)) {
    return 2;
  } else {
  }
  return 0;
}

static int variant_has_payload(int kind) {
  int _sv0t0 = (kind != 0);
  return _sv0t0;
}

static int variant_field_count(int kind, int tuple_fields, int struct_fields) {
  if ((kind == 0)) {
    return 0;
  } else {
  }
  if ((kind == 1)) {
    return tuple_fields;
  } else {
  }
  if ((kind == 2)) {
    return struct_fields;
  } else {
  }
  return 0;
}

static int count_variants(int v0, int v1, int v2, int v3, int v4, int v5) {
  int count = 0;
  if (v0) {
    count = (count + 1);
  } else {
  }
  if (v1) {
    count = (count + 1);
  } else {
  }
  if (v2) {
    count = (count + 1);
  } else {
  }
  if (v3) {
    count = (count + 1);
  } else {
  }
  if (v4) {
    count = (count + 1);
  } else {
  }
  if (v5) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int is_variant_separator(int token_tag) {
  int _sv0t0 = (token_tag == 44);
  return _sv0t0;
}

static int is_enum_end(int token_tag) {
  int _sv0t0 = (token_tag == 125);
  return _sv0t0;
}

static int enum_needs_generic_params(int has_lt) {
  return has_lt;
}

int main(void) {
  int _sv0t0 = variant_kind_from_token(40);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_kind_from_token(123);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_kind_from_token(44);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = variant_kind_from_token(125);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = variant_has_payload(1);
  if ((!_sv0t4)) {
    return 5;
  } else {
  }
  int _sv0t5 = variant_has_payload(2);
  if ((!_sv0t5)) {
    return 6;
  } else {
  }
  int _sv0t6 = variant_has_payload(0);
  if (_sv0t6) {
    return 7;
  } else {
  }
  int _sv0t7 = variant_field_count(0, 3, 2);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = variant_field_count(1, 3, 2);
  if ((_sv0t8 != 3)) {
    return 9;
  } else {
  }
  int _sv0t9 = variant_field_count(2, 3, 2);
  if ((_sv0t9 != 2)) {
    return 10;
  } else {
  }
  int _sv0t10 = count_variants(1, 1, 1, 0, 0, 0);
  if ((_sv0t10 != 3)) {
    return 11;
  } else {
  }
  int _sv0t11 = count_variants(1, 1, 1, 1, 1, 1);
  if ((_sv0t11 != 6)) {
    return 12;
  } else {
  }
  int _sv0t12 = is_variant_separator(44);
  if ((!_sv0t12)) {
    return 13;
  } else {
  }
  int _sv0t13 = is_variant_separator(59);
  if (_sv0t13) {
    return 14;
  } else {
  }
  int _sv0t14 = is_enum_end(125);
  if ((!_sv0t14)) {
    return 15;
  } else {
  }
  int _sv0t15 = is_enum_end(44);
  if (_sv0t15) {
    return 16;
  } else {
  }
  int _sv0t16 = enum_needs_generic_params(1);
  if ((!_sv0t16)) {
    return 17;
  } else {
  }
  int _sv0t17 = enum_needs_generic_params(0);
  if (_sv0t17) {
    return 18;
  } else {
  }
  return 0;
}

