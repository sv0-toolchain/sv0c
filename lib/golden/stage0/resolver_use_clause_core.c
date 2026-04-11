#include "sv0_runtime.h"

static int use_path_segments(int seg_count);
static int mangle_target(int mod_tag, int name_tag);
static int use_resolve_kind(int has_fn_arity, int has_type, int is_enum);
static int is_value_alias(int kind);
static int is_type_alias(int kind);
static int is_enum_import(int kind);
static int is_unknown_use(int kind);
static int enum_import_alias_count(int variant_count);
static int test_use_path(void);
static int test_mangle(void);
static int test_resolve_kind(void);
static int test_enum_alias_count(void);

static int use_path_segments(int seg_count) {
  int _sv0t0 = (seg_count == 2);
  return _sv0t0;
}

static int mangle_target(int mod_tag, int name_tag) {
  int _sv0t0 = (mod_tag * 1000);
  int _sv0t1 = (_sv0t0 + name_tag);
  return _sv0t1;
}

static int use_resolve_kind(int has_fn_arity, int has_type, int is_enum) {
  if (has_fn_arity) {
    return 1;
  } else {
  }
  if (has_type) {
    if (is_enum) {
      return 3;
    } else {
    }
    return 2;
  } else {
  }
  return 0;
}

static int is_value_alias(int kind) {
  int _sv0t0 = (kind == 1);
  return _sv0t0;
}

static int is_type_alias(int kind) {
  int _sv0t0 = (kind == 2);
  return _sv0t0;
}

static int is_enum_import(int kind) {
  int _sv0t0 = (kind == 3);
  return _sv0t0;
}

static int is_unknown_use(int kind) {
  int _sv0t0 = (kind == 0);
  return _sv0t0;
}

static int enum_import_alias_count(int variant_count) {
  int _sv0t0 = (1 + variant_count);
  return _sv0t0;
}

static int test_use_path(void) {
  int _sv0t0 = use_path_segments(1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = use_path_segments(2);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = use_path_segments(3);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_mangle(void) {
  int _sv0t0 = mangle_target(1, 2);
  if ((_sv0t0 != 1002)) {
    return 1;
  } else {
  }
  int _sv0t1 = mangle_target(5, 10);
  if ((_sv0t1 != 5010)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_resolve_kind(void) {
  int _sv0t0 = use_resolve_kind(1, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = use_resolve_kind(0, 1, 0);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = use_resolve_kind(0, 1, 1);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = use_resolve_kind(0, 0, 0);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_value_alias(1);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_type_alias(2);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_enum_import(3);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_unknown_use(0);
  if ((_sv0t7 != 1)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_enum_alias_count(void) {
  int _sv0t0 = enum_import_alias_count(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = enum_import_alias_count(3);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_use_path();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_mangle();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_resolve_kind();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_enum_alias_count();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

