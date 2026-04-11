#include "sv0_runtime.h"

static int is_prelude_type(int tag);
static int scope_depth_after_enter(int depth);
static int scope_depth_after_exit(int depth);
static int frame_has_name(int name, int f0, int f1, int f2, int f3);
static int lookup_value(int name, int frame0, int frame1, int mod0, int mod1);
static int dup_mod_val(int name, int existing0, int existing1, int existing2);
static int bind_local_new_size(int frame_size);
static int test_prelude_types(void);
static int test_scope_depth(void);
static int test_lookup(void);
static int test_dup_detection(void);
static int test_bind_local(void);

static int is_prelude_type(int tag) {
  if ((tag == 0)) {
    return 1;
  } else {
  }
  if ((tag == 1)) {
    return 1;
  } else {
  }
  if ((tag == 2)) {
    return 1;
  } else {
  }
  if ((tag == 3)) {
    return 1;
  } else {
  }
  if ((tag == 4)) {
    return 1;
  } else {
  }
  if ((tag == 5)) {
    return 1;
  } else {
  }
  if ((tag == 6)) {
    return 1;
  } else {
  }
  if ((tag == 7)) {
    return 1;
  } else {
  }
  if ((tag == 8)) {
    return 1;
  } else {
  }
  if ((tag == 9)) {
    return 1;
  } else {
  }
  if ((tag == 10)) {
    return 1;
  } else {
  }
  if ((tag == 11)) {
    return 1;
  } else {
  }
  if ((tag == 12)) {
    return 1;
  } else {
  }
  if ((tag == 13)) {
    return 1;
  } else {
  }
  if ((tag == 14)) {
    return 1;
  } else {
  }
  if ((tag == 15)) {
    return 1;
  } else {
  }
  if ((tag == 16)) {
    return 1;
  } else {
  }
  if ((tag == 17)) {
    return 1;
  } else {
  }
  if ((tag == 18)) {
    return 1;
  } else {
  }
  if ((tag == 19)) {
    return 1;
  } else {
  }
  if ((tag == 20)) {
    return 1;
  } else {
  }
  if ((tag == 21)) {
    return 1;
  } else {
  }
  return 0;
}

static int scope_depth_after_enter(int depth) {
  int _sv0t0 = (depth + 1);
  return _sv0t0;
}

static int scope_depth_after_exit(int depth) {
  if ((depth <= 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = (depth - 1);
  return _sv0t0;
}

static int frame_has_name(int name, int f0, int f1, int f2, int f3) {
  if ((f0 == name)) {
    return 1;
  } else {
  }
  if ((f1 == name)) {
    return 1;
  } else {
  }
  if ((f2 == name)) {
    return 1;
  } else {
  }
  if ((f3 == name)) {
    return 1;
  } else {
  }
  return 0;
}

static int lookup_value(int name, int frame0, int frame1, int mod0, int mod1) {
  if ((frame0 == name)) {
    return 1;
  } else {
  }
  if ((frame1 == name)) {
    return 1;
  } else {
  }
  if ((mod0 == name)) {
    return 1;
  } else {
  }
  if ((mod1 == name)) {
    return 1;
  } else {
  }
  return 0;
}

static int dup_mod_val(int name, int existing0, int existing1, int existing2) {
  if ((existing0 == name)) {
    return 1;
  } else {
  }
  if ((existing1 == name)) {
    return 1;
  } else {
  }
  if ((existing2 == name)) {
    return 1;
  } else {
  }
  return 0;
}

static int bind_local_new_size(int frame_size) {
  int _sv0t0 = (frame_size + 1);
  return _sv0t0;
}

static int test_prelude_types(void) {
  int _sv0t0 = is_prelude_type(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_prelude_type(10);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_prelude_type(21);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_prelude_type(22);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_prelude_type(100);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = (0 - 1);
  int _sv0t6 = is_prelude_type(_sv0t5);
  if ((_sv0t6 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_scope_depth(void) {
  int d0 = 0;
  int _sv0t0 = scope_depth_after_enter(d0);
  int d1 = _sv0t0;
  if ((d1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = scope_depth_after_enter(d1);
  int d2 = _sv0t1;
  if ((d2 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = scope_depth_after_exit(d2);
  int d3 = _sv0t2;
  if ((d3 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = scope_depth_after_exit(d3);
  int d4 = _sv0t3;
  if ((d4 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = scope_depth_after_exit(d4);
  int d5 = _sv0t4;
  if ((d5 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_lookup(void) {
  int _sv0t0 = lookup_value(42, 42, 0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = lookup_value(42, 0, 42, 0, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = lookup_value(42, 0, 0, 42, 0);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = lookup_value(42, 0, 0, 0, 42);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = lookup_value(42, 0, 0, 0, 0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = lookup_value(99, 1, 2, 3, 4);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_dup_detection(void) {
  int _sv0t0 = dup_mod_val(5, 5, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = dup_mod_val(5, 0, 5, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = dup_mod_val(5, 0, 0, 5);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = dup_mod_val(5, 1, 2, 3);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_bind_local(void) {
  int _sv0t0 = bind_local_new_size(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = bind_local_new_size(3);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = bind_local_new_size(10);
  if ((_sv0t2 != 11)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_prelude_types();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_scope_depth();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_lookup();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_dup_detection();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_bind_local();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  return 0;
}

