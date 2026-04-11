#include "sv0_runtime.h"

static int match_arm_parts(int has_guard);
static int match_arm_body_kind(int starts_with_lbrace);
static int match_arm_needs_comma(int next_is_rbrace);
static int count_arms(int a0, int a1, int a2, int a3, int a4);
static int match_expects_lbrace(void);
static int match_expects_rbrace(void);
static int match_ast_tag(void);
static int is_fatarrow(int token_tag);

static int match_arm_parts(int has_guard) {
  if (has_guard) {
    return 3;
  } else {
  }
  return 2;
}

static int match_arm_body_kind(int starts_with_lbrace) {
  if (starts_with_lbrace) {
    return 1;
  } else {
  }
  return 0;
}

static int match_arm_needs_comma(int next_is_rbrace) {
  int _sv0t0 = (!next_is_rbrace);
  return _sv0t0;
}

static int count_arms(int a0, int a1, int a2, int a3, int a4) {
  int count = 0;
  if (a0) {
    count = (count + 1);
  } else {
  }
  if (a1) {
    count = (count + 1);
  } else {
  }
  if (a2) {
    count = (count + 1);
  } else {
  }
  if (a3) {
    count = (count + 1);
  } else {
  }
  if (a4) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int match_expects_lbrace(void) {
  return 1;
}

static int match_expects_rbrace(void) {
  return 1;
}

static int match_ast_tag(void) {
  return 11;
}

static int is_fatarrow(int token_tag) {
  int _sv0t0 = (token_tag == 60);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = match_arm_parts(0);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = match_arm_parts(1);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = match_arm_body_kind(1);
  if ((_sv0t2 != 1)) {
    return 10;
  } else {
  }
  int _sv0t3 = match_arm_body_kind(0);
  if ((_sv0t3 != 0)) {
    return 11;
  } else {
  }
  int _sv0t4 = match_arm_needs_comma(0);
  if ((!_sv0t4)) {
    return 20;
  } else {
  }
  int _sv0t5 = match_arm_needs_comma(1);
  if (_sv0t5) {
    return 21;
  } else {
  }
  int _sv0t6 = count_arms(1, 1, 1, 0, 0);
  if ((_sv0t6 != 3)) {
    return 30;
  } else {
  }
  int _sv0t7 = count_arms(1, 1, 1, 1, 1);
  if ((_sv0t7 != 5)) {
    return 31;
  } else {
  }
  int _sv0t8 = count_arms(0, 0, 0, 0, 0);
  if ((_sv0t8 != 0)) {
    return 32;
  } else {
  }
  int _sv0t9 = count_arms(1, 0, 0, 0, 0);
  if ((_sv0t9 != 1)) {
    return 33;
  } else {
  }
  int _sv0t10 = match_expects_lbrace();
  if ((!_sv0t10)) {
    return 40;
  } else {
  }
  int _sv0t11 = match_expects_rbrace();
  if ((!_sv0t11)) {
    return 41;
  } else {
  }
  int _sv0t12 = match_ast_tag();
  if ((_sv0t12 != 11)) {
    return 50;
  } else {
  }
  int _sv0t13 = is_fatarrow(60);
  if ((!_sv0t13)) {
    return 60;
  } else {
  }
  int _sv0t14 = is_fatarrow(61);
  if (_sv0t14) {
    return 61;
  } else {
  }
  return 0;
}

