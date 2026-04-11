#include "sv0_runtime.h"

static int if_expr_kind(int has_else, int else_is_if);
static int if_expr_branch_count(int has_else, int else_is_if);
static int if_expr_has_value(int has_else);
static int if_expr_span_parts(int has_else);
static int if_expr_ast_tag(void);
static int chain_depth(int e0_is_if, int e1_is_if, int e2_is_if);

static int if_expr_kind(int has_else, int else_is_if) {
  if ((!has_else)) {
    return 0;
  } else {
  }
  if (else_is_if) {
    return 1;
  } else {
  }
  return 2;
}

static int if_expr_branch_count(int has_else, int else_is_if) {
  if ((!has_else)) {
    return 1;
  } else {
  }
  if (else_is_if) {
    return 3;
  } else {
  }
  return 2;
}

static int if_expr_has_value(int has_else) {
  return has_else;
}

static int if_expr_span_parts(int has_else) {
  if (has_else) {
    return 3;
  } else {
  }
  return 2;
}

static int if_expr_ast_tag(void) {
  return 10;
}

static int chain_depth(int e0_is_if, int e1_is_if, int e2_is_if) {
  int depth = 1;
  if (e0_is_if) {
    depth = (depth + 1);
  } else {
    return depth;
  }
  if (e1_is_if) {
    depth = (depth + 1);
  } else {
    return depth;
  }
  if (e2_is_if) {
    depth = (depth + 1);
  } else {
  }
  return depth;
}

int main(void) {
  int _sv0t0 = if_expr_kind(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = if_expr_branch_count(0, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = if_expr_has_value(0);
  if (_sv0t2) {
    return 3;
  } else {
  }
  int _sv0t3 = if_expr_kind(1, 0);
  if ((_sv0t3 != 2)) {
    return 10;
  } else {
  }
  int _sv0t4 = if_expr_branch_count(1, 0);
  if ((_sv0t4 != 2)) {
    return 11;
  } else {
  }
  int _sv0t5 = if_expr_has_value(1);
  if ((!_sv0t5)) {
    return 12;
  } else {
  }
  int _sv0t6 = if_expr_kind(1, 1);
  if ((_sv0t6 != 1)) {
    return 20;
  } else {
  }
  int _sv0t7 = if_expr_branch_count(1, 1);
  if ((_sv0t7 != 3)) {
    return 21;
  } else {
  }
  int _sv0t8 = if_expr_span_parts(0);
  if ((_sv0t8 != 2)) {
    return 30;
  } else {
  }
  int _sv0t9 = if_expr_span_parts(1);
  if ((_sv0t9 != 3)) {
    return 31;
  } else {
  }
  int _sv0t10 = if_expr_ast_tag();
  if ((_sv0t10 != 10)) {
    return 40;
  } else {
  }
  int _sv0t11 = chain_depth(0, 0, 0);
  if ((_sv0t11 != 1)) {
    return 50;
  } else {
  }
  int _sv0t12 = chain_depth(1, 0, 0);
  if ((_sv0t12 != 2)) {
    return 51;
  } else {
  }
  int _sv0t13 = chain_depth(1, 1, 0);
  if ((_sv0t13 != 3)) {
    return 52;
  } else {
  }
  int _sv0t14 = chain_depth(1, 1, 1);
  if ((_sv0t14 != 4)) {
    return 53;
  } else {
  }
  return 0;
}

