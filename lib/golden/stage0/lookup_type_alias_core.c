#include "sv0_runtime.h"

typedef struct {
  int t0;
  int t1;
  int tn;
  int an;
  int af0;
  int at0;
  int af1;
  int at1;
} TypeEnvAlias;

static int tag_ty_self(void);
static int is_prelude_type_id(int t);
static TypeEnvAlias tea_empty(void);
static TypeEnvAlias tea_ty_register(TypeEnvAlias e, int id);
static TypeEnvAlias tea_alias_register(TypeEnvAlias e, int from_id, int to_id);
static int tea_ty_mod_lookup(TypeEnvAlias e, int id);
static int tea_alias_step(TypeEnvAlias e, int t);
static int tea_resolve_canonical(TypeEnvAlias e, int t);
static int lookup_type_resolved(TypeEnvAlias e, int tid, int allow_self);

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

static TypeEnvAlias tea_empty(void) {
  TypeEnvAlias _sv0t0;
  _sv0t0.t0 = 0;
  _sv0t0.t1 = 0;
  _sv0t0.tn = 0;
  _sv0t0.an = 0;
  _sv0t0.af0 = 0;
  _sv0t0.at0 = 0;
  _sv0t0.af1 = 0;
  _sv0t0.at1 = 0;
  return _sv0t0;
}

static TypeEnvAlias tea_ty_register(TypeEnvAlias e, int id) {
  TypeEnvAlias out;
  out = e;
  if ((e.tn == 0)) {
    TypeEnvAlias _sv0t0;
    _sv0t0.t0 = id;
    _sv0t0.t1 = e.t1;
    _sv0t0.tn = 1;
    _sv0t0.an = e.an;
    _sv0t0.af0 = e.af0;
    _sv0t0.at0 = e.at0;
    _sv0t0.af1 = e.af1;
    _sv0t0.at1 = e.at1;
    out = _sv0t0;
  } else {
    if ((e.tn == 1)) {
      TypeEnvAlias _sv0t1;
      _sv0t1.t0 = e.t0;
      _sv0t1.t1 = id;
      _sv0t1.tn = 2;
      _sv0t1.an = e.an;
      _sv0t1.af0 = e.af0;
      _sv0t1.at0 = e.at0;
      _sv0t1.af1 = e.af1;
      _sv0t1.at1 = e.at1;
      out = _sv0t1;
    } else {
    }
  }
  return out;
}

static TypeEnvAlias tea_alias_register(TypeEnvAlias e, int from_id, int to_id) {
  TypeEnvAlias out;
  out = e;
  if ((e.an >= 1)) {
    if ((e.af0 != from_id)) {
      if ((e.an < 2)) {
        TypeEnvAlias _sv0t0;
        _sv0t0.t0 = e.t0;
        _sv0t0.t1 = e.t1;
        _sv0t0.tn = e.tn;
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
    TypeEnvAlias _sv0t1;
    _sv0t1.t0 = e.t0;
    _sv0t1.t1 = e.t1;
    _sv0t1.tn = e.tn;
    _sv0t1.an = 1;
    _sv0t1.af0 = from_id;
    _sv0t1.at0 = to_id;
    _sv0t1.af1 = 0;
    _sv0t1.at1 = 0;
    out = _sv0t1;
  }
  return out;
}

static int tea_ty_mod_lookup(TypeEnvAlias e, int id) {
  int _sv0t0;
  int _sv0t1;
  if ((e.tn >= 1)) {
    int _sv0t2;
    if ((e.t0 == id)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((e.tn >= 2)) {
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

static int tea_alias_step(TypeEnvAlias e, int t) {
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

static int tea_resolve_canonical(TypeEnvAlias e, int t) {
  int _sv0t0 = tea_alias_step(e, t);
  int s1 = _sv0t0;
  int _sv0t1;
  int _sv0t2;
  if ((s1 == t)) {
    return t;
    _sv0t2 = 0;
  } else {
    int _sv0t3 = tea_alias_step(e, s1);
    int s2 = _sv0t3;
    int _sv0t4;
    if ((s2 == s1)) {
      return s1;
      _sv0t4 = 0;
    } else {
      int _sv0t5 = tea_alias_step(e, s2);
      int s3 = _sv0t5;
      int _sv0t6;
      if ((s3 == s2)) {
        return s2;
        _sv0t6 = 0;
      } else {
        int _sv0t7 = tea_alias_step(e, s3);
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

static int lookup_type_resolved(TypeEnvAlias e, int tid, int allow_self) {
  int _sv0t0;
  int _sv0t1;
  if ((allow_self == 1)) {
    int _sv0t2;
    if ((tid == 99)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3 = tea_resolve_canonical(e, tid);
      int c = _sv0t3;
      int _sv0t5 = is_prelude_type_id(c);
      int _sv0t4;
      if ((_sv0t5 == 1)) {
        return 1;
        _sv0t4 = 0;
      } else {
        int _sv0t6 = tea_ty_mod_lookup(e, c);
        return _sv0t6;
        _sv0t4 = 0;
      }
      _sv0t2 = _sv0t4;
    }
    _sv0t1 = _sv0t2;
  } else {
    int _sv0t7 = tea_resolve_canonical(e, tid);
    int c2 = _sv0t7;
    int _sv0t9 = is_prelude_type_id(c2);
    int _sv0t8;
    if ((_sv0t9 == 1)) {
      return 1;
      _sv0t8 = 0;
    } else {
      int _sv0t10 = tea_ty_mod_lookup(e, c2);
      return _sv0t10;
      _sv0t8 = 0;
    }
    _sv0t1 = _sv0t8;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  TypeEnvAlias _sv0t0 = tea_empty();
  TypeEnvAlias e0;
  e0 = _sv0t0;
  TypeEnvAlias _sv0t1 = tea_ty_register(e0, 40);
  TypeEnvAlias e1;
  e1 = _sv0t1;
  TypeEnvAlias _sv0t2 = tea_ty_register(e1, 41);
  TypeEnvAlias e2;
  e2 = _sv0t2;
  TypeEnvAlias _sv0t3 = tea_alias_register(e2, 50, 40);
  TypeEnvAlias e3;
  e3 = _sv0t3;
  TypeEnvAlias _sv0t4 = tea_alias_register(e3, 10, 1);
  TypeEnvAlias e4;
  e4 = _sv0t4;
  int _sv0t5 = lookup_type_resolved(e4, 50, 0);
  int ok_alias_mod = (1 - _sv0t5);
  int _sv0t6 = lookup_type_resolved(e4, 10, 0);
  int ok_chain_pre = (1 - _sv0t6);
  int _sv0t7 = lookup_type_resolved(e4, 2, 0);
  int ok_direct_pre = (1 - _sv0t7);
  TypeEnvAlias _sv0t8 = tea_empty();
  TypeEnvAlias _sv0t9 = tea_alias_register(_sv0t8, 60, 61);
  TypeEnvAlias e5;
  e5 = _sv0t9;
  int _sv0t10 = lookup_type_resolved(e5, 60, 0);
  int ok_bad_chain = _sv0t10;
  int _sv0t11 = tag_ty_self();
  int _sv0t12 = lookup_type_resolved(e4, _sv0t11, 1);
  int ok_self = (1 - _sv0t12);
  int _sv0t13 = tag_ty_self();
  int _sv0t14 = lookup_type_resolved(e4, _sv0t13, 0);
  int ok_self_block = _sv0t14;
  int _sv0t15 = lookup_type_resolved(e4, 77, 0);
  int ok_miss = _sv0t15;
  int _sv0t16 = (ok_alias_mod + ok_chain_pre);
  int _sv0t17 = (_sv0t16 + ok_direct_pre);
  int _sv0t18 = (_sv0t17 + ok_bad_chain);
  int _sv0t19 = (_sv0t18 + ok_self);
  int _sv0t20 = (_sv0t19 + ok_self_block);
  int _sv0t21 = (_sv0t20 + ok_miss);
  return _sv0t21;
}

