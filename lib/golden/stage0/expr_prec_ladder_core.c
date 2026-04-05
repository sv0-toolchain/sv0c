#include "sv0_runtime.h"

static int tag_op_plus(void);
static int tag_op_minus(void);
static int tag_op_star(void);
static int tag_op_slash(void);
static int tag_op_eqeq(void);
static int prec_binary_rank(int t);
static int mul_tier_binds_tighter_than_add_tier(void);
static int is_add_layer_binop(int t);
static int is_mul_layer_binop(int t);

static int tag_op_plus(void) {
  return 20;
}

static int tag_op_minus(void) {
  return 21;
}

static int tag_op_star(void) {
  return 22;
}

static int tag_op_slash(void) {
  return 23;
}

static int tag_op_eqeq(void) {
  return 24;
}

static int prec_binary_rank(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 20)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 21)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 22)) {
        return 2;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((t == 23)) {
          return 2;
          _sv0t4 = 0;
        } else {
          return 0;
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

static int mul_tier_binds_tighter_than_add_tier(void) {
  int _sv0t2 = tag_op_star();
  int _sv0t3 = prec_binary_rank(_sv0t2);
  int _sv0t4 = tag_op_plus();
  int _sv0t5 = prec_binary_rank(_sv0t4);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t3 > _sv0t5)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int is_add_layer_binop(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 20)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 21)) {
      return 1;
      _sv0t2 = 0;
    } else {
      return 0;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int is_mul_layer_binop(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 22)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 23)) {
      return 1;
      _sv0t2 = 0;
    } else {
      return 0;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = mul_tier_binds_tighter_than_add_tier();
  int e0 = (_sv0t0 - 1);
  int _sv0t1 = tag_op_plus();
  int _sv0t2 = prec_binary_rank(_sv0t1);
  int e1 = (_sv0t2 - 1);
  int _sv0t3 = tag_op_minus();
  int _sv0t4 = prec_binary_rank(_sv0t3);
  int e2 = (_sv0t4 - 1);
  int _sv0t5 = tag_op_star();
  int _sv0t6 = prec_binary_rank(_sv0t5);
  int e3 = (_sv0t6 - 2);
  int _sv0t7 = tag_op_slash();
  int _sv0t8 = prec_binary_rank(_sv0t7);
  int e4 = (_sv0t8 - 2);
  int _sv0t9 = tag_op_eqeq();
  int _sv0t10 = prec_binary_rank(_sv0t9);
  int e5 = _sv0t10;
  int _sv0t11 = tag_op_plus();
  int _sv0t12 = is_add_layer_binop(_sv0t11);
  int e6 = (_sv0t12 - 1);
  int _sv0t13 = tag_op_star();
  int _sv0t14 = is_mul_layer_binop(_sv0t13);
  int e7 = (_sv0t14 - 1);
  int _sv0t15 = tag_op_plus();
  int _sv0t16 = is_mul_layer_binop(_sv0t15);
  int e8 = _sv0t16;
  int _sv0t17 = (e0 + e1);
  int _sv0t18 = (_sv0t17 + e2);
  int _sv0t19 = (_sv0t18 + e3);
  int _sv0t20 = (_sv0t19 + e4);
  int _sv0t21 = (_sv0t20 + e5);
  int _sv0t22 = (_sv0t21 + e6);
  int _sv0t23 = (_sv0t22 + e7);
  int _sv0t24 = (_sv0t23 + e8);
  return _sv0t24;
}

