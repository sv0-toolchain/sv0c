#include "sv0_runtime.h"

typedef struct {
  int an;
  int af0;
  int at0;
  int af1;
  int at1;
} TyAlias2;

static TyAlias2 alias_empty(void);
static TyAlias2 alias_register(TyAlias2 e, int from_id, int to_id);
static int has_ty_alias_name(TyAlias2 e, int from_id);
static int alias_step(TyAlias2 e, int t);
static int resolve_canonical_ty(TyAlias2 e, int t);

static TyAlias2 alias_empty(void) {
  TyAlias2 _sv0t0;
  _sv0t0.an = 0;
  _sv0t0.af0 = 0;
  _sv0t0.at0 = 0;
  _sv0t0.af1 = 0;
  _sv0t0.at1 = 0;
  return _sv0t0;
}

static TyAlias2 alias_register(TyAlias2 e, int from_id, int to_id) {
  TyAlias2 out;
  out = e;
  if ((e.an >= 1)) {
    if ((e.af0 != from_id)) {
      if ((e.an < 2)) {
        TyAlias2 _sv0t0;
        _sv0t0.an = 2;
        _sv0t0.af0 = e.af0;
        _sv0t0.at0 = e.at0;
        _sv0t0.af1 = from_id;
        _sv0t0.at1 = to_id;
        out = _sv0t0;
      } else {
      }
    } else {
    }
  } else {
    TyAlias2 _sv0t1;
    _sv0t1.an = 1;
    _sv0t1.af0 = from_id;
    _sv0t1.at0 = to_id;
    _sv0t1.af1 = 0;
    _sv0t1.at1 = 0;
    out = _sv0t1;
  }
  return out;
}

static int has_ty_alias_name(TyAlias2 e, int from_id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.an >= 1)) {
    int _sv0t2;
    if ((e.af0 == from_id)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.an >= 2)) {
        int _sv0t4;
        if ((e.af1 == from_id)) {
          return 1;
          _sv0t4 = 0;
        } else {
          return 0;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        return 0;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int alias_step(TyAlias2 e, int t) {
  int _sv0t0;
  int _sv0t1;
  if ((e.an >= 1)) {
    int _sv0t2;
    if ((e.af0 == t)) {
      return e.at0;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.an >= 2)) {
        int _sv0t4;
        if ((e.af1 == t)) {
          return e.at1;
          _sv0t4 = 0;
        } else {
          return t;
          _sv0t4 = 0;
        }
        _sv0t3 = _sv0t4;
      } else {
        return t;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    return t;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int resolve_canonical_ty(TyAlias2 e, int t) {
  int _sv0t0 = alias_step(e, t);
  int s1 = _sv0t0;
  int _sv0t1;
  int _sv0t2;
  if ((s1 == t)) {
    return t;
    _sv0t2 = 0;
  } else {
    int _sv0t3 = alias_step(e, s1);
    int s2 = _sv0t3;
    int _sv0t4;
    if ((s2 == s1)) {
      return s1;
      _sv0t4 = 0;
    } else {
      int _sv0t5 = alias_step(e, s2);
      int s3 = _sv0t5;
      int _sv0t6;
      if ((s3 == s2)) {
        return s2;
        _sv0t6 = 0;
      } else {
        int _sv0t7 = alias_step(e, s3);
        int s4 = _sv0t7;
        int _sv0t8;
        if ((s4 == s3)) {
          return s3;
          _sv0t8 = 0;
        } else {
          return s4;
          _sv0t8 = 0;
        }
        _sv0t6 = _sv0t8;
      }
      _sv0t4 = _sv0t6;
    }
    _sv0t2 = _sv0t4;
  }
  _sv0t1 = _sv0t2;
  return _sv0t1;
}

int main(void) {
  TyAlias2 _sv0t0 = alias_empty();
  TyAlias2 e0;
  e0 = _sv0t0;
  TyAlias2 _sv0t1 = alias_register(e0, 10, 20);
  TyAlias2 e1;
  e1 = _sv0t1;
  TyAlias2 _sv0t2 = alias_register(e1, 20, 1);
  TyAlias2 e2;
  e2 = _sv0t2;
  int _sv0t3 = resolve_canonical_ty(e2, 10);
  int ok_chain_a = (_sv0t3 - 1);
  int _sv0t4 = resolve_canonical_ty(e2, 20);
  int ok_chain_b = (_sv0t4 - 1);
  int _sv0t5 = resolve_canonical_ty(e2, 30);
  int ok_free = (_sv0t5 - 30);
  int _sv0t6 = has_ty_alias_name(e2, 10);
  int ok_has = (1 - _sv0t6);
  int _sv0t7 = has_ty_alias_name(e2, 77);
  int ok_no_has = _sv0t7;
  TyAlias2 _sv0t8 = alias_empty();
  TyAlias2 _sv0t9 = alias_register(_sv0t8, 15, 15);
  TyAlias2 e_id;
  e_id = _sv0t9;
  int _sv0t10 = resolve_canonical_ty(e_id, 15);
  int ok_idem = (_sv0t10 - 15);
  int _sv0t11 = (ok_chain_a + ok_chain_b);
  int _sv0t12 = (_sv0t11 + ok_free);
  int _sv0t13 = (_sv0t12 + ok_has);
  int _sv0t14 = (_sv0t13 + ok_no_has);
  int _sv0t15 = (_sv0t14 + ok_idem);
  return _sv0t15;
}

