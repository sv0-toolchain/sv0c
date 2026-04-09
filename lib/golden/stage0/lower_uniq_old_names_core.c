#include "sv0_runtime.h"

static int uon_name_eq(int a, int b);
static int uon_acc_has(int x, int a0, int a1, int a2, int a3, int alen);
static int uniq_old_names_final_len(int n0, int n1, int n2, int n3, int in_len);

static int uon_name_eq(int a, int b) {
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

static int uon_acc_has(int x, int a0, int a1, int a2, int a3, int alen) {
  int _sv0t0;
  int _sv0t1;
  if ((alen >= 1)) {
    int _sv0t3 = uon_name_eq(x, a0);
    int _sv0t2;
    if ((_sv0t3 == 1)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t4;
      if ((alen >= 2)) {
        int _sv0t6 = uon_name_eq(x, a1);
        int _sv0t5;
        if ((_sv0t6 == 1)) {
          return 1;
          _sv0t5 = 0;
        } else {
          int _sv0t7;
          if ((alen >= 3)) {
            int _sv0t9 = uon_name_eq(x, a2);
            int _sv0t8;
            if ((_sv0t9 == 1)) {
              return 1;
              _sv0t8 = 0;
            } else {
              int _sv0t10;
              if ((alen >= 4)) {
                int _sv0t12 = uon_name_eq(x, a3);
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

static int uniq_old_names_final_len(int n0, int n1, int n2, int n3, int in_len) {
  int a0 = 0;
  int a1 = 0;
  int a2 = 0;
  int a3 = 0;
  int alen = 0;
  if ((in_len >= 1)) {
    int x0 = n0;
    int _sv0t0 = uon_acc_has(x0, a0, a1, a2, a3, alen);
    if ((_sv0t0 == 0)) {
      a3 = a2;
      a2 = a1;
      a1 = a0;
      a0 = x0;
      alen = (alen + 1);
    } else {
    }
  } else {
  }
  if ((in_len >= 2)) {
    int x1 = n1;
    int _sv0t1 = uon_acc_has(x1, a0, a1, a2, a3, alen);
    if ((_sv0t1 == 0)) {
      a3 = a2;
      a2 = a1;
      a1 = a0;
      a0 = x1;
      alen = (alen + 1);
    } else {
    }
  } else {
  }
  if ((in_len >= 3)) {
    int x2 = n2;
    int _sv0t2 = uon_acc_has(x2, a0, a1, a2, a3, alen);
    if ((_sv0t2 == 0)) {
      a3 = a2;
      a2 = a1;
      a1 = a0;
      a0 = x2;
      alen = (alen + 1);
    } else {
    }
  } else {
  }
  if ((in_len >= 4)) {
    int x3 = n3;
    int _sv0t3 = uon_acc_has(x3, a0, a1, a2, a3, alen);
    if ((_sv0t3 == 0)) {
      a3 = a2;
      a2 = a1;
      a1 = a0;
      a0 = x3;
      alen = (alen + 1);
    } else {
    }
  } else {
  }
  return alen;
}

int main(void) {
  int _sv0t0 = uniq_old_names_final_len(1, 2, 3, 4, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = uniq_old_names_final_len(5, 0, 0, 0, 1);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = uniq_old_names_final_len(1, 2, 1, 3, 4);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = uniq_old_names_final_len(9, 9, 9, 9, 4);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = uniq_old_names_final_len(2, 1, 2, 0, 3);
  if ((_sv0t4 != 2)) {
    return 1;
  } else {
  }
  int _sv0t5 = uon_acc_has(3, 3, 0, 0, 0, 1);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = uon_acc_has(3, 2, 0, 0, 0, 1);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

