#include "sv0_runtime.h"

static int try_arm_let(void);
static int try_arm_break(void);
static int try_arm_continue(void);
static int try_arm_assert(void);
static int try_arm_assign_fallback(void);
static int tok_stmt_break(void);
static int tok_stmt_continue(void);
static int tok_stmt_assert(void);
static int parse_try_stmt_dispatch(int t);
static int lexer_keyword_let_to_try_stmt_arm(int kw);
static int try_stmt_is_assign_fallback_arm(int arm);
static int parse_block_after_try_stmt_failed(void);

static int try_arm_let(void) {
  return 1;
}

static int try_arm_break(void) {
  return 2;
}

static int try_arm_continue(void) {
  return 3;
}

static int try_arm_assert(void) {
  return 4;
}

static int try_arm_assign_fallback(void) {
  return 5;
}

static int tok_stmt_break(void) {
  return 190;
}

static int tok_stmt_continue(void) {
  return 191;
}

static int tok_stmt_assert(void) {
  return 56;
}

static int parse_try_stmt_dispatch(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 2)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 190)) {
      return 2;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 191)) {
        return 3;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((t == 56)) {
          return 4;
          _sv0t4 = 0;
        } else {
          return 5;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int lexer_keyword_let_to_try_stmt_arm(int kw) {
  int _sv0t0;
  int _sv0t1;
  if ((kw == 2)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int try_stmt_is_assign_fallback_arm(int arm) {
  int _sv0t0;
  int _sv0t1;
  if ((arm == 5)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int parse_block_after_try_stmt_failed(void) {
  return 1;
}

int main(void) {
  int _sv0t0 = parse_try_stmt_dispatch(2);
  int _sv0t1 = try_arm_let();
  int f0 = (_sv0t0 - _sv0t1);
  int _sv0t2 = tok_stmt_break();
  int _sv0t3 = parse_try_stmt_dispatch(_sv0t2);
  int _sv0t4 = try_arm_break();
  int f1 = (_sv0t3 - _sv0t4);
  int _sv0t5 = tok_stmt_continue();
  int _sv0t6 = parse_try_stmt_dispatch(_sv0t5);
  int _sv0t7 = try_arm_continue();
  int f2 = (_sv0t6 - _sv0t7);
  int _sv0t8 = tok_stmt_assert();
  int _sv0t9 = parse_try_stmt_dispatch(_sv0t8);
  int _sv0t10 = try_arm_assert();
  int f3 = (_sv0t9 - _sv0t10);
  int _sv0t11 = parse_try_stmt_dispatch(73);
  int _sv0t12 = try_arm_assign_fallback();
  int f4 = (_sv0t11 - _sv0t12);
  int _sv0t13 = parse_try_stmt_dispatch(99);
  int _sv0t14 = try_arm_assign_fallback();
  int f5 = (_sv0t13 - _sv0t14);
  int _sv0t15 = lexer_keyword_let_to_try_stmt_arm(2);
  int _sv0t16 = try_arm_let();
  int f6 = (_sv0t15 - _sv0t16);
  int _sv0t17 = lexer_keyword_let_to_try_stmt_arm(1);
  int f7 = _sv0t17;
  int _sv0t18 = try_arm_assign_fallback();
  int _sv0t19 = try_stmt_is_assign_fallback_arm(_sv0t18);
  int f8 = (_sv0t19 - 1);
  int _sv0t20 = try_arm_let();
  int _sv0t21 = try_stmt_is_assign_fallback_arm(_sv0t20);
  int f9 = _sv0t21;
  int _sv0t22 = parse_block_after_try_stmt_failed();
  int f10 = (_sv0t22 - 1);
  int _sv0t23 = (f0 + f1);
  int _sv0t24 = (_sv0t23 + f2);
  int _sv0t25 = (_sv0t24 + f3);
  int _sv0t26 = (_sv0t25 + f4);
  int _sv0t27 = (_sv0t26 + f5);
  int _sv0t28 = (_sv0t27 + f6);
  int _sv0t29 = (_sv0t28 + f7);
  int _sv0t30 = (_sv0t29 + f8);
  int _sv0t31 = (_sv0t30 + f9);
  int _sv0t32 = (_sv0t31 + f10);
  return _sv0t32;
}

