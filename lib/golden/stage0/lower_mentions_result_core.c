#include "sv0_runtime.h"

static int mr_or2(int x, int y);
static int mr_or3(int x, int y, int z);
static int mentions_result_shape(int shape, int a, int b, int c);

static int mr_or2(int x, int y) {
  int _sv0t0;
  int _sv0t1;
  if ((x != 0)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return y;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int mr_or3(int x, int y, int z) {
  int _sv0t0;
  int _sv0t1;
  if ((x != 0)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((y != 0)) {
      return 1;
      _sv0t2 = 0;
    } else {
      return z;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int mentions_result_shape(int shape, int a, int b, int c) {
  int _sv0t0;
  int _sv0t1;
  if ((shape == 1)) {
    return a;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((shape == 2)) {
      int _sv0t3 = mr_or2(a, b);
      return _sv0t3;
      _sv0t2 = 0;
    } else {
      int _sv0t4;
      if ((shape == 3)) {
        return a;
        _sv0t4 = 0;
      } else {
        int _sv0t5;
        if ((shape == 4)) {
          int _sv0t6 = mr_or2(a, b);
          return _sv0t6;
          _sv0t5 = 0;
        } else {
          int _sv0t7;
          if ((shape == 5)) {
            int _sv0t8 = mr_or3(a, b, c);
            return _sv0t8;
            _sv0t7 = 0;
          } else {
            int _sv0t9;
            if ((shape == 6)) {
              int _sv0t10 = mr_or3(a, b, c);
              return _sv0t10;
              _sv0t9 = 0;
            } else {
              int _sv0t11;
              if ((shape == 7)) {
                int _sv0t12 = mr_or2(a, b);
                return _sv0t12;
                _sv0t11 = 0;
              } else {
                int _sv0t13;
                if ((shape == 8)) {
                  return a;
                  _sv0t13 = 0;
                } else {
                  int _sv0t14;
                  if ((shape == 9)) {
                    int _sv0t15 = mr_or3(a, b, c);
                    return _sv0t15;
                    _sv0t14 = 0;
                  } else {
                    int _sv0t16;
                    if ((shape == 10)) {
                      return a;
                      _sv0t16 = 0;
                    } else {
                      int _sv0t17;
                      if ((shape == 11)) {
                        return a;
                        _sv0t17 = 0;
                      } else {
                        int _sv0t18;
                        if ((shape == 12)) {
                          return a;
                          _sv0t18 = 0;
                        } else {
                          int _sv0t19;
                          if ((shape == 13)) {
                            return 0;
                            _sv0t19 = 0;
                          } else {
                            int _sv0t20;
                            if ((shape == 14)) {
                              return a;
                              _sv0t20 = 0;
                            } else {
                              int _sv0t21;
                              if ((shape == 15)) {
                                return 0;
                                _sv0t21 = 0;
                              } else {
                                int _sv0t22;
                                if ((shape == 16)) {
                                  return a;
                                  _sv0t22 = 0;
                                } else {
                                  int _sv0t23;
                                  if ((shape == 17)) {
                                    return a;
                                    _sv0t23 = 0;
                                  } else {
                                    return 0;
                                    _sv0t23 = 0;
                                  }
                                  _sv0t22 = _sv0t23;
                                }
                                _sv0t21 = _sv0t22;
                              }
                              _sv0t20 = _sv0t21;
                            }
                            _sv0t19 = _sv0t20;
                          }
                          _sv0t18 = _sv0t19;
                        }
                        _sv0t17 = _sv0t18;
                      }
                      _sv0t16 = _sv0t17;
                    }
                    _sv0t14 = _sv0t16;
                  }
                  _sv0t13 = _sv0t14;
                }
                _sv0t11 = _sv0t13;
              }
              _sv0t9 = _sv0t11;
            }
            _sv0t7 = _sv0t9;
          }
          _sv0t5 = _sv0t7;
        }
        _sv0t4 = _sv0t5;
      }
      _sv0t2 = _sv0t4;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = mentions_result_shape(1, 1, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = mentions_result_shape(1, 0, 0, 0);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = mentions_result_shape(2, 0, 0, 0);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = mentions_result_shape(2, 1, 0, 0);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = mentions_result_shape(2, 0, 1, 0);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = mentions_result_shape(3, 1, 0, 0);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = mentions_result_shape(4, 0, 1, 0);
  if ((_sv0t6 != 1)) {
    return 1;
  } else {
  }
  int _sv0t7 = mentions_result_shape(5, 0, 0, 1);
  if ((_sv0t7 != 1)) {
    return 1;
  } else {
  }
  int _sv0t8 = mentions_result_shape(6, 0, 1, 0);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = mentions_result_shape(9, 1, 0, 0);
  if ((_sv0t9 != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = mentions_result_shape(11, 1, 0, 0);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = mentions_result_shape(13, 0, 0, 0);
  if ((_sv0t11 != 0)) {
    return 1;
  } else {
  }
  int _sv0t12 = mentions_result_shape(14, 1, 0, 0);
  if ((_sv0t12 != 1)) {
    return 1;
  } else {
  }
  int _sv0t13 = mentions_result_shape(15, 1, 0, 0);
  if ((_sv0t13 != 0)) {
    return 1;
  } else {
  }
  int _sv0t14 = mentions_result_shape(99, 1, 1, 1);
  if ((_sv0t14 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

