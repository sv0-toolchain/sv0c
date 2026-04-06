#include "sv0_runtime.h"

static int tok_ident(void);
static int cast_ty_primitive_ident(int t);
static int tok_lparen(void);
static int tok_rparen(void);
static int parse_type_unit_ok(int t0, int t1);
static int parse_type_ident_ok(int t0);
static int tok_int_lit(void);

static int tok_ident(void) {
  return 73;
}

static int cast_ty_primitive_ident(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 73)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int tok_lparen(void) {
  return 10;
}

static int tok_rparen(void) {
  return 11;
}

static int parse_type_unit_ok(int t0, int t1) {
  int _sv0t0;
  int _sv0t1;
  if ((t0 == 10)) {
    int _sv0t2;
    if ((t1 == 11)) {
      return 1;
      _sv0t2 = 0;
    } else {
      return 0;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int parse_type_ident_ok(int t0) {
  int _sv0t0 = cast_ty_primitive_ident(t0);
  return _sv0t0;
}

static int tok_int_lit(void) {
  return 40;
}

int main(void) {
  int _sv0t0 = tok_lparen();
  int _sv0t1 = tok_rparen();
  int _sv0t2 = parse_type_unit_ok(_sv0t0, _sv0t1);
  int e0 = (_sv0t2 - 1);
  int _sv0t3 = tok_ident();
  int _sv0t4 = parse_type_ident_ok(_sv0t3);
  int e1 = (_sv0t4 - 1);
  int _sv0t5 = tok_lparen();
  int _sv0t6 = tok_lparen();
  int _sv0t7 = parse_type_unit_ok(_sv0t5, _sv0t6);
  int bad_unit_open = _sv0t7;
  int _sv0t8 = tok_ident();
  int _sv0t9 = tok_rparen();
  int _sv0t10 = parse_type_unit_ok(_sv0t8, _sv0t9);
  int bad_unit_ident = _sv0t10;
  int _sv0t11 = tok_int_lit();
  int _sv0t12 = parse_type_ident_ok(_sv0t11);
  int bad_ident_lit = _sv0t12;
  int _sv0t13 = (bad_unit_open + bad_unit_ident);
  int e2 = (_sv0t13 + bad_ident_lit);
  int _sv0t14 = (e0 + e1);
  int _sv0t15 = (_sv0t14 + e2);
  return _sv0t15;
}

