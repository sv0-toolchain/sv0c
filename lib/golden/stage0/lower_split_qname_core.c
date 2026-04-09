#include "sv0_runtime.h"

static int sq_colon(void);
static int sq_char(int pos, int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7);
static int sq_find_dbl_k(int off, int sublen, int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7, int kk);
static int sq_seg_count_at_rel(int off, int sublen, int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7, int rel);
static int sq_seg_count(int off, int sublen, int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7);
static int split_qname_segment_count(int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7);

static int sq_colon(void) {
  return 58;
}

static int sq_char(int pos, int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7) {
  int _sv0t0;
  int _sv0t1;
  if ((pos < 0)) {
    int _sv0t2 = (-1);
    return _sv0t2;
    _sv0t1 = 0;
  } else {
    int _sv0t3;
    if ((pos >= len)) {
      int _sv0t4 = (-1);
      return _sv0t4;
      _sv0t3 = 0;
    } else {
      int _sv0t5;
      if ((pos >= 8)) {
        int _sv0t6 = (-1);
        return _sv0t6;
        _sv0t5 = 0;
      } else {
        int _sv0t7;
        if ((pos == 0)) {
          return c0;
          _sv0t7 = 0;
        } else {
          int _sv0t8;
          if ((pos == 1)) {
            return c1;
            _sv0t8 = 0;
          } else {
            int _sv0t9;
            if ((pos == 2)) {
              return c2;
              _sv0t9 = 0;
            } else {
              int _sv0t10;
              if ((pos == 3)) {
                return c3;
                _sv0t10 = 0;
              } else {
                int _sv0t11;
                if ((pos == 4)) {
                  return c4;
                  _sv0t11 = 0;
                } else {
                  int _sv0t12;
                  if ((pos == 5)) {
                    return c5;
                    _sv0t12 = 0;
                  } else {
                    int _sv0t13;
                    if ((pos == 6)) {
                      return c6;
                      _sv0t13 = 0;
                    } else {
                      return c7;
                      _sv0t13 = 0;
                    }
                    _sv0t12 = _sv0t13;
                  }
                  _sv0t11 = _sv0t12;
                }
                _sv0t10 = _sv0t11;
              }
              _sv0t9 = _sv0t10;
            }
            _sv0t8 = _sv0t9;
          }
          _sv0t7 = _sv0t8;
        }
        _sv0t5 = _sv0t7;
      }
      _sv0t3 = _sv0t5;
    }
    _sv0t1 = _sv0t3;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int sq_find_dbl_k(int off, int sublen, int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7, int kk) {
  int _sv0t2 = (kk + 1);
  int _sv0t0;
  int _sv0t1;
  if ((_sv0t2 >= sublen)) {
    int _sv0t3 = (-1);
    return _sv0t3;
    _sv0t1 = 0;
  } else {
    int p0 = (off + kk);
    int _sv0t4 = (off + kk);
    int p1 = (_sv0t4 + 1);
    int _sv0t6 = sq_char(p0, len, c0, c1, c2, c3, c4, c5, c6, c7);
    int _sv0t7 = sq_colon();
    int _sv0t5;
    if ((_sv0t6 == _sv0t7)) {
      int _sv0t9 = sq_char(p1, len, c0, c1, c2, c3, c4, c5, c6, c7);
      int _sv0t10 = sq_colon();
      int _sv0t8;
      if ((_sv0t9 == _sv0t10)) {
        return kk;
        _sv0t8 = 0;
      } else {
        int _sv0t11 = (kk + 1);
        int _sv0t12 = sq_find_dbl_k(off, sublen, len, c0, c1, c2, c3, c4, c5, c6, c7, _sv0t11);
        return _sv0t12;
        _sv0t8 = 0;
      }
      _sv0t5 = _sv0t8;
    } else {
      int _sv0t13 = (kk + 1);
      int _sv0t14 = sq_find_dbl_k(off, sublen, len, c0, c1, c2, c3, c4, c5, c6, c7, _sv0t13);
      return _sv0t14;
      _sv0t5 = 0;
    }
    _sv0t1 = _sv0t5;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int sq_seg_count_at_rel(int off, int sublen, int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7, int rel) {
  int _sv0t2 = (-1);
  int _sv0t0;
  int _sv0t1;
  if ((rel == _sv0t2)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t3 = (off + rel);
    int _sv0t4 = (_sv0t3 + 2);
    int _sv0t5 = (sublen - rel);
    int _sv0t6 = (_sv0t5 - 2);
    int _sv0t7 = sq_seg_count(_sv0t4, _sv0t6, len, c0, c1, c2, c3, c4, c5, c6, c7);
    int _sv0t8 = (1 + _sv0t7);
    return _sv0t8;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int sq_seg_count(int off, int sublen, int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7) {
  int _sv0t0 = sq_find_dbl_k(off, sublen, len, c0, c1, c2, c3, c4, c5, c6, c7, 0);
  int _sv0t1 = sq_seg_count_at_rel(off, sublen, len, c0, c1, c2, c3, c4, c5, c6, c7, _sv0t0);
  return _sv0t1;
}

static int split_qname_segment_count(int len, int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7) {
  int _sv0t0 = sq_seg_count(0, len, len, c0, c1, c2, c3, c4, c5, c6, c7);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = split_qname_segment_count(0, 0, 0, 0, 0, 0, 0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = split_qname_segment_count(3, 102, 111, 111, 0, 0, 0, 0, 0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = split_qname_segment_count(4, 97, 58, 58, 98, 0, 0, 0, 0);
  if ((_sv0t2 != 2)) {
    return 1;
  } else {
  }
  int _sv0t3 = split_qname_segment_count(7, 97, 58, 58, 98, 58, 58, 99, 0);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = split_qname_segment_count(2, 58, 58, 0, 0, 0, 0, 0, 0);
  if ((_sv0t4 != 2)) {
    return 1;
  } else {
  }
  int _sv0t5 = split_qname_segment_count(6, 120, 58, 58, 58, 58, 121, 0, 0);
  if ((_sv0t5 != 3)) {
    return 1;
  } else {
  }
  return 0;
}

