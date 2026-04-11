#include "sv0_runtime.h"

static int has_module_wrapper(int first_item_kind);
static int split_module_inner_count(int total_items, int has_wrapper);
static int map_ty_kind(int ty_tag);
static int needs_path_rewrite(int ty_tag);
static int needs_recursive_rewrite(int ty_tag);
static int needs_list_rewrite(int ty_tag);
static int is_leaf_ty(int ty_tag);
static int rewrite_depth(int ty_tag, int child_depth);
static int test_module_wrapper(void);
static int test_map_ty_kind(void);
static int test_predicates(void);
static int test_rewrite_depth(void);

static int has_module_wrapper(int first_item_kind) {
  int _sv0t0 = (first_item_kind == 6);
  return _sv0t0;
}

static int split_module_inner_count(int total_items, int has_wrapper) {
  if (has_wrapper) {
    int _sv0t0 = (total_items - 1);
    return _sv0t0;
  } else {
  }
  return total_items;
}

static int map_ty_kind(int ty_tag) {
  if ((ty_tag == 0)) {
    return 1;
  } else {
  }
  if ((ty_tag == 1)) {
    return 2;
  } else {
  }
  if ((ty_tag == 2)) {
    return 2;
  } else {
  }
  if ((ty_tag == 3)) {
    return 2;
  } else {
  }
  if ((ty_tag == 4)) {
    return 2;
  } else {
  }
  if ((ty_tag == 5)) {
    return 3;
  } else {
  }
  if ((ty_tag == 6)) {
    return 0;
  } else {
  }
  if ((ty_tag == 7)) {
    return 0;
  } else {
  }
  return 0;
}

static int needs_path_rewrite(int ty_tag) {
  int _sv0t0 = map_ty_kind(ty_tag);
  int _sv0t1 = (_sv0t0 == 1);
  return _sv0t1;
}

static int needs_recursive_rewrite(int ty_tag) {
  int _sv0t0 = map_ty_kind(ty_tag);
  int _sv0t1 = (_sv0t0 == 2);
  return _sv0t1;
}

static int needs_list_rewrite(int ty_tag) {
  int _sv0t0 = map_ty_kind(ty_tag);
  int _sv0t1 = (_sv0t0 == 3);
  return _sv0t1;
}

static int is_leaf_ty(int ty_tag) {
  int _sv0t0 = map_ty_kind(ty_tag);
  int _sv0t1 = (_sv0t0 == 0);
  return _sv0t1;
}

static int rewrite_depth(int ty_tag, int child_depth) {
  int _sv0t0 = is_leaf_ty(ty_tag);
  if (_sv0t0) {
    return 0;
  } else {
  }
  int _sv0t1 = (1 + child_depth);
  return _sv0t1;
}

static int test_module_wrapper(void) {
  int _sv0t0 = has_module_wrapper(6);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = has_module_wrapper(0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = split_module_inner_count(5, 1);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  int _sv0t3 = split_module_inner_count(5, 0);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_map_ty_kind(void) {
  int _sv0t0 = map_ty_kind(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = map_ty_kind(1);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = map_ty_kind(5);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = map_ty_kind(6);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = map_ty_kind(7);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_predicates(void) {
  int _sv0t0 = needs_path_rewrite(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = needs_recursive_rewrite(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = needs_list_rewrite(5);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_leaf_ty(6);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_leaf_ty(0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_rewrite_depth(void) {
  int _sv0t0 = rewrite_depth(6, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = rewrite_depth(0, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = rewrite_depth(1, 2);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_module_wrapper();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_map_ty_kind();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_predicates();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_rewrite_depth();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

