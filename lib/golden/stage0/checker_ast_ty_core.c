#include "sv0_runtime.h"

static int name_to_ty_tag(int name_tag);
static int ast_ty_kind(int ast_tag);
static int is_generic_container(int name_tag);
static int resolve_named_ty(int name_tag, int is_struct, int is_enum, int is_tyvar);
static int tuple_ty_count(int has_elem0, int has_elem1, int has_elem2);

static int name_to_ty_tag(int name_tag) {
  if ((name_tag == 0)) {
    return 0;
  } else {
  }
  if ((name_tag == 1)) {
    return 1;
  } else {
  }
  if ((name_tag == 2)) {
    return 2;
  } else {
  }
  if ((name_tag == 3)) {
    return 3;
  } else {
  }
  if ((name_tag == 4)) {
    return 4;
  } else {
  }
  if ((name_tag == 5)) {
    return 5;
  } else {
  }
  if ((name_tag == 6)) {
    return 6;
  } else {
  }
  if ((name_tag == 7)) {
    return 7;
  } else {
  }
  if ((name_tag == 8)) {
    return 8;
  } else {
  }
  if ((name_tag == 9)) {
    return 9;
  } else {
  }
  if ((name_tag == 10)) {
    return 10;
  } else {
  }
  if ((name_tag == 11)) {
    return 11;
  } else {
  }
  if ((name_tag == 12)) {
    return 12;
  } else {
  }
  if ((name_tag == 13)) {
    return 13;
  } else {
  }
  if ((name_tag == 14)) {
    return 14;
  } else {
  }
  if ((name_tag == 15)) {
    return 15;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int ast_ty_kind(int ast_tag) {
  if ((ast_tag == 0)) {
    return 0;
  } else {
  }
  if ((ast_tag == 1)) {
    return 1;
  } else {
  }
  if ((ast_tag == 2)) {
    return 2;
  } else {
  }
  if ((ast_tag == 3)) {
    return 3;
  } else {
  }
  if ((ast_tag == 4)) {
    return 4;
  } else {
  }
  if ((ast_tag == 5)) {
    return 5;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int is_generic_container(int name_tag) {
  int _sv0t0 = (name_tag == 30);
  int _sv0t1 = (name_tag == 31);
  int _sv0t2 = (_sv0t0 || _sv0t1);
  return _sv0t2;
}

static int resolve_named_ty(int name_tag, int is_struct, int is_enum, int is_tyvar) {
  int _sv0t0 = name_to_ty_tag(name_tag);
  int base = _sv0t0;
  if ((base >= 0)) {
    return base;
  } else {
  }
  if (is_struct) {
    return 16;
  } else {
  }
  if (is_enum) {
    return 17;
  } else {
  }
  if (is_tyvar) {
    return 18;
  } else {
  }
  int _sv0t1 = (-1);
  return _sv0t1;
}

static int tuple_ty_count(int has_elem0, int has_elem1, int has_elem2) {
  int count = 0;
  if (has_elem0) {
    count = (count + 1);
  } else {
  }
  if (has_elem1) {
    count = (count + 1);
  } else {
  }
  if (has_elem2) {
    count = (count + 1);
  } else {
  }
  return count;
}

int main(void) {
  int _sv0t0 = name_to_ty_tag(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = name_to_ty_tag(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = name_to_ty_tag(15);
  if ((_sv0t2 != 15)) {
    return 3;
  } else {
  }
  int _sv0t3 = name_to_ty_tag(99);
  int _sv0t4 = (-1);
  if ((_sv0t3 != _sv0t4)) {
    return 4;
  } else {
  }
  int _sv0t5 = ast_ty_kind(0);
  if ((_sv0t5 != 0)) {
    return 5;
  } else {
  }
  int _sv0t6 = ast_ty_kind(5);
  if ((_sv0t6 != 5)) {
    return 6;
  } else {
  }
  int _sv0t7 = ast_ty_kind(99);
  int _sv0t8 = (-1);
  if ((_sv0t7 != _sv0t8)) {
    return 7;
  } else {
  }
  int _sv0t9 = is_generic_container(30);
  if ((!_sv0t9)) {
    return 8;
  } else {
  }
  int _sv0t10 = is_generic_container(31);
  if ((!_sv0t10)) {
    return 9;
  } else {
  }
  int _sv0t11 = is_generic_container(0);
  if (_sv0t11) {
    return 10;
  } else {
  }
  int _sv0t12 = resolve_named_ty(1, 0, 0, 0);
  if ((_sv0t12 != 1)) {
    return 11;
  } else {
  }
  int _sv0t13 = resolve_named_ty(99, 1, 0, 0);
  if ((_sv0t13 != 16)) {
    return 12;
  } else {
  }
  int _sv0t14 = resolve_named_ty(99, 0, 1, 0);
  if ((_sv0t14 != 17)) {
    return 13;
  } else {
  }
  int _sv0t15 = resolve_named_ty(99, 0, 0, 1);
  if ((_sv0t15 != 18)) {
    return 14;
  } else {
  }
  int _sv0t16 = resolve_named_ty(99, 0, 0, 0);
  int _sv0t17 = (-1);
  if ((_sv0t16 != _sv0t17)) {
    return 15;
  } else {
  }
  int _sv0t18 = tuple_ty_count(1, 1, 0);
  if ((_sv0t18 != 2)) {
    return 16;
  } else {
  }
  int _sv0t19 = tuple_ty_count(1, 1, 1);
  if ((_sv0t19 != 3)) {
    return 17;
  } else {
  }
  return 0;
}

