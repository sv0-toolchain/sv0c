#include "sv0_runtime.h"

typedef struct {
  int t0;
  int t1;
  int n;
} TypeEnv2;

static int tag_ty_i32(void);
static int tag_ty_bool(void);
static int tag_ty_unit(void);
static int tag_ty_str(void);
static int tag_ty_self(void);
static int is_prelude_type_id(int t);
static TypeEnv2 ty_empty(void);
static TypeEnv2 ty_register(TypeEnv2 e, int id);
static int ty_mod_lookup(TypeEnv2 e, int id);
static int lookup_type_simple(TypeEnv2 e, int tid, int allow_self);

static int tag_ty_i32(void) {
  return 1;
}

static int tag_ty_bool(void) {
  return 2;
}

static int tag_ty_unit(void) {
  return 3;
}

static int tag_ty_str(void) {
  return 4;
}

static int tag_ty_self(void) {
  return 99;
}

static int is_prelude_type_id(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 1)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 2)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 3)) {
        return 1;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((t == 4)) {
          return 1;
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

static TypeEnv2 ty_empty(void) {
  TypeEnv2 _sv0t0;
  _sv0t0.t0 = 0;
  _sv0t0.t1 = 0;
  _sv0t0.n = 0;
  return _sv0t0;
}

static TypeEnv2 ty_register(TypeEnv2 e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.n == 0)) {
    TypeEnv2 _sv0t2;
    _sv0t2.t0 = id;
    _sv0t2.t1 = e.t1;
    _sv0t2.n = 1;
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((e.n == 1)) {
      TypeEnv2 _sv0t4;
      _sv0t4.t0 = e.t0;
      _sv0t4.t1 = id;
      _sv0t4.n = 2;
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      return e;
      _sv0t3 = 0;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ty_mod_lookup(TypeEnv2 e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.n >= 1)) {
    int _sv0t2;
    if ((e.t0 == id)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.n >= 2)) {
        int _sv0t4;
        if ((e.t1 == id)) {
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

static int lookup_type_simple(TypeEnv2 e, int tid, int allow_self) {
  int _sv0t0;
  int _sv0t1;
  if ((allow_self == 1)) {
    int _sv0t2;
    if ((tid == 99)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t4 = is_prelude_type_id(tid);
      int _sv0t3;
      if ((_sv0t4 == 1)) {
        return 1;
        _sv0t3 = 0;
      } else {
        int _sv0t5 = ty_mod_lookup(e, tid);
        return _sv0t5;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  } else {
    int _sv0t7 = is_prelude_type_id(tid);
    int _sv0t6;
    if ((_sv0t7 == 1)) {
      return 1;
      _sv0t6 = 0;
    } else {
      int _sv0t8 = ty_mod_lookup(e, tid);
      return _sv0t8;
      _sv0t6 = 0;
    }
    _sv0t1 = _sv0t6;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  TypeEnv2 _sv0t0 = ty_empty();
  TypeEnv2 e0;
  e0 = _sv0t0;
  int _sv0t1 = tag_ty_i32();
  int _sv0t2 = lookup_type_simple(e0, _sv0t1, 0);
  int ok_pre = (1 - _sv0t2);
  int _sv0t3 = tag_ty_bool();
  int _sv0t4 = lookup_type_simple(e0, _sv0t3, 0);
  int ok_pre2 = (1 - _sv0t4);
  TypeEnv2 _sv0t5 = ty_register(e0, 40);
  TypeEnv2 e1;
  e1 = _sv0t5;
  TypeEnv2 _sv0t6 = ty_register(e1, 41);
  TypeEnv2 e2;
  e2 = _sv0t6;
  int _sv0t7 = lookup_type_simple(e2, 40, 0);
  int ok_mod = (1 - _sv0t7);
  int _sv0t8 = lookup_type_simple(e2, 50, 0);
  int ok_miss = _sv0t8;
  int _sv0t9 = tag_ty_self();
  int _sv0t10 = lookup_type_simple(e2, _sv0t9, 1);
  int ok_self_yes = (1 - _sv0t10);
  int _sv0t11 = tag_ty_self();
  int _sv0t12 = lookup_type_simple(e2, _sv0t11, 0);
  int ok_self_no = _sv0t12;
  int ok_n = (e2.n - 2);
  int _sv0t13 = (ok_pre + ok_pre2);
  int _sv0t14 = (_sv0t13 + ok_mod);
  int _sv0t15 = (_sv0t14 + ok_miss);
  int _sv0t16 = (_sv0t15 + ok_self_yes);
  int _sv0t17 = (_sv0t16 + ok_self_no);
  int _sv0t18 = (_sv0t17 + ok_n);
  return _sv0t18;
}

