#include "sv0_runtime.h"

typedef struct {
  int tag;
  int p0;
} Literal;
typedef struct {
  int tag;
  int p0;
} Ty;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
} Expr;

static int eval_lit(Literal lit);
static int eval(Expr e);

static int eval_lit(Literal lit) {
  int _sv0t0;
  if ((lit.tag == 0)) {
    int n = lit.p0;
    return n;
    _sv0t0 = 0;
  } else {
    if ((lit.tag == 1)) {
      int b = lit.p0;
      int _sv0t1;
      if (b) {
        return 1;
        _sv0t1 = 0;
      } else {
        return 0;
        _sv0t1 = 0;
      }
      _sv0t0 = _sv0t1;
    } else {
    }
  }
  return _sv0t0;
}

static int eval(Expr e) {
  int _sv0t0;
  if ((e.tag == 0)) {
    int bl = e.p0;
    Literal _sv0t36;
    int _sv0t37 = sv0_box_load(bl, 0);
    _sv0t36.tag = _sv0t37;
    int _sv0t38 = sv0_box_load(bl, 1);
    _sv0t36.p0 = _sv0t38;
    Literal lit;
    lit.tag = (_sv0t36).tag;
    lit.p0 = (_sv0t36).p0;
    int _sv0t39 = eval_lit(lit);
    return _sv0t39;
    _sv0t0 = 0;
  } else {
    if ((e.tag == 1)) {
      int id = e.p0;
      return id;
      _sv0t0 = 0;
    } else {
      if ((e.tag == 2)) {
        int op = e.p0;
        int bl = e.p1;
        int br = e.p2;
        Expr _sv0t21;
        int _sv0t22 = sv0_box_load(bl, 0);
        _sv0t21.tag = _sv0t22;
        int _sv0t23 = sv0_box_load(bl, 1);
        _sv0t21.p0 = _sv0t23;
        int _sv0t24 = sv0_box_load(bl, 2);
        _sv0t21.p1 = _sv0t24;
        int _sv0t25 = sv0_box_load(bl, 3);
        _sv0t21.p2 = _sv0t25;
        Expr le;
        le.tag = (_sv0t21).tag;
        le.p0 = (_sv0t21).p0;
        le.p1 = (_sv0t21).p1;
        le.p2 = (_sv0t21).p2;
        Expr _sv0t26;
        int _sv0t27 = sv0_box_load(br, 0);
        _sv0t26.tag = _sv0t27;
        int _sv0t28 = sv0_box_load(br, 1);
        _sv0t26.p0 = _sv0t28;
        int _sv0t29 = sv0_box_load(br, 2);
        _sv0t26.p1 = _sv0t29;
        int _sv0t30 = sv0_box_load(br, 3);
        _sv0t26.p2 = _sv0t30;
        Expr re;
        re.tag = (_sv0t26).tag;
        re.p0 = (_sv0t26).p0;
        re.p1 = (_sv0t26).p1;
        re.p2 = (_sv0t26).p2;
        int _sv0t31 = eval(le);
        int lv = _sv0t31;
        int _sv0t32 = eval(re);
        int rv = _sv0t32;
        int _sv0t33;
        if ((op == 0)) {
          int _sv0t34 = (lv + rv);
          return _sv0t34;
          _sv0t33 = 0;
        } else {
          int _sv0t35 = (lv - rv);
          return _sv0t35;
          _sv0t33 = 0;
        }
        _sv0t0 = _sv0t33;
      } else {
        if ((e.tag == 3)) {
          int bc = e.p0;
          int bt = e.p1;
          int be = e.p2;
          Expr _sv0t2;
          int _sv0t3 = sv0_box_load(bc, 0);
          _sv0t2.tag = _sv0t3;
          int _sv0t4 = sv0_box_load(bc, 1);
          _sv0t2.p0 = _sv0t4;
          int _sv0t5 = sv0_box_load(bc, 2);
          _sv0t2.p1 = _sv0t5;
          int _sv0t6 = sv0_box_load(bc, 3);
          _sv0t2.p2 = _sv0t6;
          Expr ce;
          ce.tag = (_sv0t2).tag;
          ce.p0 = (_sv0t2).p0;
          ce.p1 = (_sv0t2).p1;
          ce.p2 = (_sv0t2).p2;
          int _sv0t7 = eval(ce);
          int cv = _sv0t7;
          int _sv0t8;
          if ((cv != 0)) {
            Expr _sv0t9;
            int _sv0t10 = sv0_box_load(bt, 0);
            _sv0t9.tag = _sv0t10;
            int _sv0t11 = sv0_box_load(bt, 1);
            _sv0t9.p0 = _sv0t11;
            int _sv0t12 = sv0_box_load(bt, 2);
            _sv0t9.p1 = _sv0t12;
            int _sv0t13 = sv0_box_load(bt, 3);
            _sv0t9.p2 = _sv0t13;
            Expr te;
            te.tag = (_sv0t9).tag;
            te.p0 = (_sv0t9).p0;
            te.p1 = (_sv0t9).p1;
            te.p2 = (_sv0t9).p2;
            int _sv0t14 = eval(te);
            return _sv0t14;
            _sv0t8 = 0;
          } else {
            Expr _sv0t15;
            int _sv0t16 = sv0_box_load(be, 0);
            _sv0t15.tag = _sv0t16;
            int _sv0t17 = sv0_box_load(be, 1);
            _sv0t15.p0 = _sv0t17;
            int _sv0t18 = sv0_box_load(be, 2);
            _sv0t15.p1 = _sv0t18;
            int _sv0t19 = sv0_box_load(be, 3);
            _sv0t15.p2 = _sv0t19;
            Expr ee;
            ee.tag = (_sv0t15).tag;
            ee.p0 = (_sv0t15).p0;
            ee.p1 = (_sv0t15).p1;
            ee.p2 = (_sv0t15).p2;
            int _sv0t20 = eval(ee);
            return _sv0t20;
            _sv0t8 = 0;
          }
          _sv0t0 = _sv0t8;
        } else {
          if ((e.tag == 4)) {
            int stmts = e.p0;
            int _sv0t1 = sv0_vec_len(stmts);
            return _sv0t1;
            _sv0t0 = 0;
          } else {
          }
        }
      }
    }
  }
  return _sv0t0;
}

int main(void) {
  Expr lit1;
  Literal _sv0t0;
  _sv0t0.tag = 0;
  _sv0t0.p0 = 42;
  int _sv0t1 = sv0_box_alloc(2);
  sv0_box_store(_sv0t1, 0, _sv0t0.tag);
  sv0_box_store(_sv0t1, 1, _sv0t0.p0);
  lit1.tag = 0;
  lit1.p0 = _sv0t1;
  int _sv0t2 = eval(lit1);
  int r1 = _sv0t2;
  if ((r1 != 42)) {
    return 1;
  } else {
  }
  Expr left;
  Literal _sv0t3;
  _sv0t3.tag = 0;
  _sv0t3.p0 = 10;
  int _sv0t4 = sv0_box_alloc(2);
  sv0_box_store(_sv0t4, 0, _sv0t3.tag);
  sv0_box_store(_sv0t4, 1, _sv0t3.p0);
  left.tag = 0;
  left.p0 = _sv0t4;
  Expr right;
  Literal _sv0t5;
  _sv0t5.tag = 0;
  _sv0t5.p0 = 32;
  int _sv0t6 = sv0_box_alloc(2);
  sv0_box_store(_sv0t6, 0, _sv0t5.tag);
  sv0_box_store(_sv0t6, 1, _sv0t5.p0);
  right.tag = 0;
  right.p0 = _sv0t6;
  Expr add;
  int _sv0t7 = sv0_box_alloc(4);
  sv0_box_store(_sv0t7, 0, left.tag);
  sv0_box_store(_sv0t7, 1, left.p0);
  sv0_box_store(_sv0t7, 2, left.p1);
  sv0_box_store(_sv0t7, 3, left.p2);
  int _sv0t8 = sv0_box_alloc(4);
  sv0_box_store(_sv0t8, 0, right.tag);
  sv0_box_store(_sv0t8, 1, right.p0);
  sv0_box_store(_sv0t8, 2, right.p1);
  sv0_box_store(_sv0t8, 3, right.p2);
  add.tag = 2;
  add.p0 = 0;
  add.p1 = _sv0t7;
  add.p2 = _sv0t8;
  int _sv0t9 = eval(add);
  int r2 = _sv0t9;
  if ((r2 != 42)) {
    return 2;
  } else {
  }
  Expr cond;
  Literal _sv0t10;
  _sv0t10.tag = 1;
  _sv0t10.p0 = 1;
  int _sv0t11 = sv0_box_alloc(2);
  sv0_box_store(_sv0t11, 0, _sv0t10.tag);
  sv0_box_store(_sv0t11, 1, _sv0t10.p0);
  cond.tag = 0;
  cond.p0 = _sv0t11;
  Expr then_e;
  Literal _sv0t12;
  _sv0t12.tag = 0;
  _sv0t12.p0 = 42;
  int _sv0t13 = sv0_box_alloc(2);
  sv0_box_store(_sv0t13, 0, _sv0t12.tag);
  sv0_box_store(_sv0t13, 1, _sv0t12.p0);
  then_e.tag = 0;
  then_e.p0 = _sv0t13;
  Expr else_e;
  Literal _sv0t14;
  _sv0t14.tag = 0;
  _sv0t14.p0 = 0;
  int _sv0t15 = sv0_box_alloc(2);
  sv0_box_store(_sv0t15, 0, _sv0t14.tag);
  sv0_box_store(_sv0t15, 1, _sv0t14.p0);
  else_e.tag = 0;
  else_e.p0 = _sv0t15;
  Expr if_e;
  int _sv0t16 = sv0_box_alloc(4);
  sv0_box_store(_sv0t16, 0, cond.tag);
  sv0_box_store(_sv0t16, 1, cond.p0);
  sv0_box_store(_sv0t16, 2, cond.p1);
  sv0_box_store(_sv0t16, 3, cond.p2);
  int _sv0t17 = sv0_box_alloc(4);
  sv0_box_store(_sv0t17, 0, then_e.tag);
  sv0_box_store(_sv0t17, 1, then_e.p0);
  sv0_box_store(_sv0t17, 2, then_e.p1);
  sv0_box_store(_sv0t17, 3, then_e.p2);
  int _sv0t18 = sv0_box_alloc(4);
  sv0_box_store(_sv0t18, 0, else_e.tag);
  sv0_box_store(_sv0t18, 1, else_e.p0);
  sv0_box_store(_sv0t18, 2, else_e.p1);
  sv0_box_store(_sv0t18, 3, else_e.p2);
  if_e.tag = 3;
  if_e.p0 = _sv0t16;
  if_e.p1 = _sv0t17;
  if_e.p2 = _sv0t18;
  int _sv0t19 = eval(if_e);
  int r3 = _sv0t19;
  if ((r3 != 42)) {
    return 3;
  } else {
  }
  Expr sub;
  Expr _sv0t20;
  Literal _sv0t21;
  _sv0t21.tag = 0;
  _sv0t21.p0 = 50;
  int _sv0t22 = sv0_box_alloc(2);
  sv0_box_store(_sv0t22, 0, _sv0t21.tag);
  sv0_box_store(_sv0t22, 1, _sv0t21.p0);
  _sv0t20.tag = 0;
  _sv0t20.p0 = _sv0t22;
  int _sv0t23 = sv0_box_alloc(4);
  sv0_box_store(_sv0t23, 0, _sv0t20.tag);
  sv0_box_store(_sv0t23, 1, _sv0t20.p0);
  sv0_box_store(_sv0t23, 2, _sv0t20.p1);
  sv0_box_store(_sv0t23, 3, _sv0t20.p2);
  Expr _sv0t24;
  Literal _sv0t25;
  _sv0t25.tag = 0;
  _sv0t25.p0 = 8;
  int _sv0t26 = sv0_box_alloc(2);
  sv0_box_store(_sv0t26, 0, _sv0t25.tag);
  sv0_box_store(_sv0t26, 1, _sv0t25.p0);
  _sv0t24.tag = 0;
  _sv0t24.p0 = _sv0t26;
  int _sv0t27 = sv0_box_alloc(4);
  sv0_box_store(_sv0t27, 0, _sv0t24.tag);
  sv0_box_store(_sv0t27, 1, _sv0t24.p0);
  sv0_box_store(_sv0t27, 2, _sv0t24.p1);
  sv0_box_store(_sv0t27, 3, _sv0t24.p2);
  sub.tag = 2;
  sub.p0 = 1;
  sub.p1 = _sv0t23;
  sub.p2 = _sv0t27;
  int _sv0t28 = eval(sub);
  int r4 = _sv0t28;
  if ((r4 != 42)) {
    return 4;
  } else {
  }
  Ty inner;
  inner.tag = 0;
  Ty ref_ty;
  int _sv0t29 = sv0_box_alloc(2);
  sv0_box_store(_sv0t29, 0, inner.tag);
  sv0_box_store(_sv0t29, 1, inner.p0);
  ref_ty.tag = 3;
  ref_ty.p0 = _sv0t29;
  int _sv0t30;
  if ((ref_ty.tag == 3)) {
    int b = ref_ty.p0;
    Ty _sv0t31;
    int _sv0t32 = sv0_box_load(b, 0);
    _sv0t31.tag = _sv0t32;
    int _sv0t33 = sv0_box_load(b, 1);
    _sv0t31.p0 = _sv0t33;
    Ty t;
    t.tag = (_sv0t31).tag;
    t.p0 = (_sv0t31).p0;
    int _sv0t34;
    if ((t.tag == 0)) {
      _sv0t34 = 0;
    } else {
      if ((t.tag == 1)) {
        return 5;
        _sv0t34 = 0;
      } else {
        if ((t.tag == 2)) {
          return 5;
          _sv0t34 = 0;
        } else {
          if ((t.tag == 3)) {
            int b2 = t.p0;
            return 5;
            _sv0t34 = 0;
          } else {
          }
        }
      }
    }
    _sv0t30 = _sv0t34;
  } else {
    if ((ref_ty.tag == 0)) {
      return 5;
      _sv0t30 = 0;
    } else {
      if ((ref_ty.tag == 1)) {
        return 5;
        _sv0t30 = 0;
      } else {
        if ((ref_ty.tag == 2)) {
          return 5;
          _sv0t30 = 0;
        } else {
        }
      }
    }
  }
  Expr nested;
  Expr _sv0t35;
  Expr _sv0t36;
  Literal _sv0t37;
  _sv0t37.tag = 0;
  _sv0t37.p0 = 10;
  int _sv0t38 = sv0_box_alloc(2);
  sv0_box_store(_sv0t38, 0, _sv0t37.tag);
  sv0_box_store(_sv0t38, 1, _sv0t37.p0);
  _sv0t36.tag = 0;
  _sv0t36.p0 = _sv0t38;
  int _sv0t39 = sv0_box_alloc(4);
  sv0_box_store(_sv0t39, 0, _sv0t36.tag);
  sv0_box_store(_sv0t39, 1, _sv0t36.p0);
  sv0_box_store(_sv0t39, 2, _sv0t36.p1);
  sv0_box_store(_sv0t39, 3, _sv0t36.p2);
  Expr _sv0t40;
  Literal _sv0t41;
  _sv0t41.tag = 0;
  _sv0t41.p0 = 20;
  int _sv0t42 = sv0_box_alloc(2);
  sv0_box_store(_sv0t42, 0, _sv0t41.tag);
  sv0_box_store(_sv0t42, 1, _sv0t41.p0);
  _sv0t40.tag = 0;
  _sv0t40.p0 = _sv0t42;
  int _sv0t43 = sv0_box_alloc(4);
  sv0_box_store(_sv0t43, 0, _sv0t40.tag);
  sv0_box_store(_sv0t43, 1, _sv0t40.p0);
  sv0_box_store(_sv0t43, 2, _sv0t40.p1);
  sv0_box_store(_sv0t43, 3, _sv0t40.p2);
  _sv0t35.tag = 2;
  _sv0t35.p0 = 0;
  _sv0t35.p1 = _sv0t39;
  _sv0t35.p2 = _sv0t43;
  int _sv0t44 = sv0_box_alloc(4);
  sv0_box_store(_sv0t44, 0, _sv0t35.tag);
  sv0_box_store(_sv0t44, 1, _sv0t35.p0);
  sv0_box_store(_sv0t44, 2, _sv0t35.p1);
  sv0_box_store(_sv0t44, 3, _sv0t35.p2);
  Expr _sv0t45;
  Literal _sv0t46;
  _sv0t46.tag = 0;
  _sv0t46.p0 = 12;
  int _sv0t47 = sv0_box_alloc(2);
  sv0_box_store(_sv0t47, 0, _sv0t46.tag);
  sv0_box_store(_sv0t47, 1, _sv0t46.p0);
  _sv0t45.tag = 0;
  _sv0t45.p0 = _sv0t47;
  int _sv0t48 = sv0_box_alloc(4);
  sv0_box_store(_sv0t48, 0, _sv0t45.tag);
  sv0_box_store(_sv0t48, 1, _sv0t45.p0);
  sv0_box_store(_sv0t48, 2, _sv0t45.p1);
  sv0_box_store(_sv0t48, 3, _sv0t45.p2);
  nested.tag = 2;
  nested.p0 = 0;
  nested.p1 = _sv0t44;
  nested.p2 = _sv0t48;
  int _sv0t49 = eval(nested);
  int r6 = _sv0t49;
  if ((r6 != 42)) {
    return 6;
  } else {
  }
  return 0;
}

