#include "sv0_runtime.h"

static int ot_nm_eq(int a, int b);
static int ot_mentions_has(int x, int m0, int m1, int m2, int m3, int mlen);
static int old_targets_hits(int pk0, int nm0, int pk1, int nm1, int pk2, int nm2, int pk3, int nm3, int param_len, int m0, int m1, int m2, int m3, int m_len);

static int ot_nm_eq(int a, int b) {
  int _sv0t0;
  int _sv0t1;
  if ((a == b)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int ot_mentions_has(int x, int m0, int m1, int m2, int m3, int mlen) {
  int _sv0t0;
  int _sv0t1;
  if ((mlen >= 1)) {
    int _sv0t3 = ot_nm_eq(x, m0);
    int _sv0t2;
    if ((_sv0t3 == 1)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t4;
      if ((mlen >= 2)) {
        int _sv0t6 = ot_nm_eq(x, m1);
        int _sv0t5;
        if ((_sv0t6 == 1)) {
          return 1;
          _sv0t5 = 0;
        } else {
          int _sv0t7;
          if ((mlen >= 3)) {
            int _sv0t9 = ot_nm_eq(x, m2);
            int _sv0t8;
            if ((_sv0t9 == 1)) {
              return 1;
              _sv0t8 = 0;
            } else {
              int _sv0t10;
              if ((mlen >= 4)) {
                int _sv0t12 = ot_nm_eq(x, m3);
                int _sv0t11;
                if ((_sv0t12 == 1)) {
                  return 1;
                  _sv0t11 = 0;
                } else {
                  return 0;
                  _sv0t11 = 0;
                }
                _sv0t10 = _sv0t11;
              } else {
                return 0;
                _sv0t10 = 0;
              }
              _sv0t8 = _sv0t10;
            }
            _sv0t7 = _sv0t8;
          } else {
            return 0;
            _sv0t7 = 0;
          }
          _sv0t5 = _sv0t7;
        }
        _sv0t4 = _sv0t5;
      } else {
        return 0;
        _sv0t4 = 0;
      }
      _sv0t2 = _sv0t4;
    }
    _sv0t1 = _sv0t2;
  } else {
    return 0;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int old_targets_hits(int pk0, int nm0, int pk1, int nm1, int pk2, int nm2, int pk3, int nm3, int param_len, int m0, int m1, int m2, int m3, int m_len) {
  int c = 0;
  if ((param_len >= 1)) {
    if ((pk0 == 1)) {
      int _sv0t0 = ot_mentions_has(nm0, m0, m1, m2, m3, m_len);
      if ((_sv0t0 == 1)) {
        c = (c + 1);
      } else {
      }
    } else {
    }
  } else {
  }
  if ((param_len >= 2)) {
    if ((pk1 == 1)) {
      int _sv0t1 = ot_mentions_has(nm1, m0, m1, m2, m3, m_len);
      if ((_sv0t1 == 1)) {
        c = (c + 1);
      } else {
      }
    } else {
    }
  } else {
  }
  if ((param_len >= 3)) {
    if ((pk2 == 1)) {
      int _sv0t2 = ot_mentions_has(nm2, m0, m1, m2, m3, m_len);
      if ((_sv0t2 == 1)) {
        c = (c + 1);
      } else {
      }
    } else {
    }
  } else {
  }
  if ((param_len >= 4)) {
    if ((pk3 == 1)) {
      int _sv0t3 = ot_mentions_has(nm3, m0, m1, m2, m3, m_len);
      if ((_sv0t3 == 1)) {
        c = (c + 1);
      } else {
      }
    } else {
    }
  } else {
  }
  return c;
}

int main(void) {
  int _sv0t0 = old_targets_hits(1, 5, 1, 7, 0, 0, 0, 0, 2, 5, 0, 0, 0, 1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = old_targets_hits(1, 1, 1, 2, 0, 0, 0, 0, 2, 1, 2, 0, 0, 2);
  if ((_sv0t1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t2 = old_targets_hits(0, 5, 1, 5, 0, 0, 0, 0, 2, 5, 0, 0, 0, 1);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = old_targets_hits(1, 3, 1, 3, 0, 0, 0, 0, 2, 9, 0, 0, 0, 1);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = old_targets_hits(1, 2, 0, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 1);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = old_targets_hits(0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 4);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

