#include "sv0_runtime.h"

static int pat_kind_from_token(int token_tag);
static int is_pat_start_token(int token_tag);
static int pat_has_payload(int pat_tag);
static int pat_binding_count(int pat_tag, int sub_count);
static int path_is_enum_ctor(int path_len);
static int test_pat_kind(void);
static int test_pat_start(void);
static int test_pat_payload(void);
static int test_binding_count(void);
static int test_enum_ctor(void);

static int pat_kind_from_token(int token_tag) {
  if ((token_tag == 95)) {
    return 0;
  } else {
  }
  if ((token_tag == 5)) {
    return 1;
  } else {
  }
  if ((token_tag == 0)) {
    return 2;
  } else {
  }
  if ((token_tag == 1)) {
    return 2;
  } else {
  }
  if ((token_tag == 2)) {
    return 2;
  } else {
  }
  if ((token_tag == 3)) {
    return 2;
  } else {
  }
  if ((token_tag == 4)) {
    return 2;
  } else {
  }
  if ((token_tag == 7)) {
    return 3;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int is_pat_start_token(int token_tag) {
  int _sv0t0 = pat_kind_from_token(token_tag);
  if ((_sv0t0 >= 0)) {
    return 1;
  } else {
  }
  if ((token_tag == 5)) {
    return 1;
  } else {
  }
  return 0;
}

static int pat_has_payload(int pat_tag) {
  if ((pat_tag == 0)) {
    return 0;
  } else {
  }
  if ((pat_tag == 1)) {
    return 1;
  } else {
  }
  if ((pat_tag == 2)) {
    return 1;
  } else {
  }
  if ((pat_tag == 3)) {
    return 1;
  } else {
  }
  if ((pat_tag == 4)) {
    return 1;
  } else {
  }
  if ((pat_tag == 5)) {
    return 1;
  } else {
  }
  if ((pat_tag == 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int pat_binding_count(int pat_tag, int sub_count) {
  if ((pat_tag == 0)) {
    return 0;
  } else {
  }
  if ((pat_tag == 1)) {
    return 1;
  } else {
  }
  if ((pat_tag == 2)) {
    return 0;
  } else {
  }
  if ((pat_tag == 3)) {
    return sub_count;
  } else {
  }
  if ((pat_tag == 4)) {
    return sub_count;
  } else {
  }
  if ((pat_tag == 5)) {
    return sub_count;
  } else {
  }
  if ((pat_tag == 6)) {
    return sub_count;
  } else {
  }
  return 0;
}

static int path_is_enum_ctor(int path_len) {
  int _sv0t0 = (path_len >= 2);
  return _sv0t0;
}

static int test_pat_kind(void) {
  int _sv0t0 = pat_kind_from_token(95);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = pat_kind_from_token(5);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = pat_kind_from_token(0);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = pat_kind_from_token(7);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = pat_kind_from_token(99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_pat_start(void) {
  int _sv0t0 = is_pat_start_token(95);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_pat_start_token(5);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_pat_start_token(0);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_pat_start_token(99);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_pat_payload(void) {
  int _sv0t0 = pat_has_payload(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = pat_has_payload(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = pat_has_payload(6);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_binding_count(void) {
  int _sv0t0 = pat_binding_count(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = pat_binding_count(1, 0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = pat_binding_count(3, 3);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_enum_ctor(void) {
  int _sv0t0 = path_is_enum_ctor(1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = path_is_enum_ctor(2);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = path_is_enum_ctor(3);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_pat_kind();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_pat_start();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_pat_payload();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_binding_count();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_enum_ctor();
  int r5 = _sv0t8;
  if ((r5 != 0)) {
    int _sv0t9 = (500 + r5);
    return _sv0t9;
  } else {
  }
  return 0;
}

