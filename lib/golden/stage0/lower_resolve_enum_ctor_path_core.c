#include "sv0_runtime.h"

static int recp_en(int alias_hit, int alias_target_is_pair, int en_in, int vn_in, int e2, int v2);
static int recp_vn(int alias_hit, int alias_target_is_pair, int en_in, int vn_in, int e2, int v2);

static int recp_en(int alias_hit, int alias_target_is_pair, int en_in, int vn_in, int e2, int v2) {
  int _sv0t0;
  int _sv0t1;
  if ((alias_hit == 0)) {
    return en_in;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((alias_target_is_pair == 0)) {
      return en_in;
      _sv0t2 = 0;
    } else {
      return e2;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int recp_vn(int alias_hit, int alias_target_is_pair, int en_in, int vn_in, int e2, int v2) {
  int _sv0t0;
  int _sv0t1;
  if ((alias_hit == 0)) {
    return vn_in;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((alias_target_is_pair == 0)) {
      return vn_in;
      _sv0t2 = 0;
    } else {
      return v2;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = recp_en(0, 0, 10, 20, 99, 88);
  if ((_sv0t0 != 10)) {
    return 1;
  } else {
  }
  int _sv0t1 = recp_vn(0, 0, 10, 20, 99, 88);
  if ((_sv0t1 != 20)) {
    return 1;
  } else {
  }
  int _sv0t2 = recp_en(0, 1, 10, 20, 99, 88);
  if ((_sv0t2 != 10)) {
    return 1;
  } else {
  }
  int _sv0t3 = recp_vn(0, 1, 10, 20, 99, 88);
  if ((_sv0t3 != 20)) {
    return 1;
  } else {
  }
  int _sv0t4 = recp_en(1, 0, 10, 20, 99, 88);
  if ((_sv0t4 != 10)) {
    return 1;
  } else {
  }
  int _sv0t5 = recp_vn(1, 0, 10, 20, 99, 88);
  if ((_sv0t5 != 20)) {
    return 1;
  } else {
  }
  int _sv0t6 = recp_en(1, 1, 10, 20, 99, 88);
  if ((_sv0t6 != 99)) {
    return 1;
  } else {
  }
  int _sv0t7 = recp_vn(1, 1, 10, 20, 99, 88);
  if ((_sv0t7 != 88)) {
    return 1;
  } else {
  }
  int _sv0t8 = recp_en(1, 1, 1, 2, 0, 0);
  if ((_sv0t8 != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = recp_vn(1, 1, 1, 2, 0, 0);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

