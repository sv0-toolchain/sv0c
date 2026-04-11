#include "sv0_runtime.h"

static int item_needs_rewrite(int item_kind);
static int item_has_body(int item_kind);
static int item_has_fields(int item_kind);
static int program_unit_parts(int item_count, int has_module_wrapper);
static int map_fn_components(int has_params, int has_body, int has_contracts);

static int item_needs_rewrite(int item_kind) {
  if ((item_kind == 0)) {
    return 1;
  } else {
  }
  if ((item_kind == 3)) {
    return 1;
  } else {
  }
  if ((item_kind == 4)) {
    return 1;
  } else {
  }
  if ((item_kind == 5)) {
    return 1;
  } else {
  }
  if ((item_kind == 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int item_has_body(int item_kind) {
  if ((item_kind == 0)) {
    return 1;
  } else {
  }
  if ((item_kind == 5)) {
    return 1;
  } else {
  }
  if ((item_kind == 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int item_has_fields(int item_kind) {
  if ((item_kind == 3)) {
    return 1;
  } else {
  }
  if ((item_kind == 4)) {
    return 1;
  } else {
  }
  return 0;
}

static int program_unit_parts(int item_count, int has_module_wrapper) {
  if (has_module_wrapper) {
    int _sv0t0 = (item_count + 1);
    return _sv0t0;
  } else {
  }
  return item_count;
}

static int map_fn_components(int has_params, int has_body, int has_contracts) {
  int parts = 1;
  if (has_params) {
    parts = (parts + 1);
  } else {
  }
  if (has_body) {
    parts = (parts + 1);
  } else {
  }
  if (has_contracts) {
    parts = (parts + 1);
  } else {
  }
  return parts;
}

int main(void) {
  int _sv0t0 = item_needs_rewrite(0);
  if ((!_sv0t0)) {
    return 1;
  } else {
  }
  int _sv0t1 = item_needs_rewrite(4);
  if ((!_sv0t1)) {
    return 2;
  } else {
  }
  int _sv0t2 = item_needs_rewrite(7);
  if (_sv0t2) {
    return 3;
  } else {
  }
  int _sv0t3 = item_needs_rewrite(2);
  if (_sv0t3) {
    return 4;
  } else {
  }
  int _sv0t4 = item_has_body(0);
  if ((!_sv0t4)) {
    return 5;
  } else {
  }
  int _sv0t5 = item_has_body(3);
  if (_sv0t5) {
    return 6;
  } else {
  }
  int _sv0t6 = item_has_body(6);
  if ((!_sv0t6)) {
    return 7;
  } else {
  }
  int _sv0t7 = item_has_fields(3);
  if ((!_sv0t7)) {
    return 8;
  } else {
  }
  int _sv0t8 = item_has_fields(4);
  if ((!_sv0t8)) {
    return 9;
  } else {
  }
  int _sv0t9 = item_has_fields(0);
  if (_sv0t9) {
    return 10;
  } else {
  }
  int _sv0t10 = program_unit_parts(5, 1);
  if ((_sv0t10 != 6)) {
    return 11;
  } else {
  }
  int _sv0t11 = program_unit_parts(5, 0);
  if ((_sv0t11 != 5)) {
    return 12;
  } else {
  }
  int _sv0t12 = map_fn_components(1, 1, 1);
  if ((_sv0t12 != 4)) {
    return 13;
  } else {
  }
  int _sv0t13 = map_fn_components(0, 1, 0);
  if ((_sv0t13 != 2)) {
    return 14;
  } else {
  }
  int _sv0t14 = map_fn_components(0, 0, 0);
  if ((_sv0t14 != 1)) {
    return 15;
  } else {
  }
  return 0;
}

