#include "sv0_runtime.h"

static int aon_add(int x, int y);
static int aon_add3(int x, int y, int z);
static int all_old_names_count(int shape, int a, int b, int c);

static int aon_add(int x, int y) {
  int _sv0t0 = (x + y);
  return _sv0t0;
}

static int aon_add3(int x, int y, int z) {
  int _sv0t0 = (x + y);
  int _sv0t1 = (_sv0t0 + z);
  return _sv0t1;
}

static int all_old_names_count(int shape, int a, int b, int c) {
  int _sv0t0;
  int _sv0t1;
  if ((shape == 1)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((shape == 2)) {
      return 0;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((shape == 3)) {
        int _sv0t4 = aon_add(a, b);
        return _sv0t4;
        _sv0t3 = 0;
      } else {
        int _sv0t5;
        if ((shape == 4)) {
          return a;
          _sv0t5 = 0;
        } else {
          int _sv0t6;
          if ((shape == 5)) {
            int _sv0t7 = aon_add(a, b);
            return _sv0t7;
            _sv0t6 = 0;
          } else {
            int _sv0t8;
            if ((shape == 6)) {
              int _sv0t9 = aon_add3(a, b, c);
              return _sv0t9;
              _sv0t8 = 0;
            } else {
              int _sv0t10;
              if ((shape == 7)) {
                int _sv0t11 = aon_add3(a, b, c);
                return _sv0t11;
                _sv0t10 = 0;
              } else {
                int _sv0t12;
                if ((shape == 8)) {
                  int _sv0t13 = aon_add(a, b);
                  return _sv0t13;
                  _sv0t12 = 0;
                } else {
                  int _sv0t14;
                  if ((shape == 9)) {
                    return a;
                    _sv0t14 = 0;
                  } else {
                    int _sv0t15;
                    if ((shape == 10)) {
                      int _sv0t16 = aon_add3(a, b, c);
                      return _sv0t16;
                      _sv0t15 = 0;
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
                            return a;
                            _sv0t19 = 0;
                          } else {
                            int _sv0t20;
                            if ((shape == 14)) {
                              return a;
                              _sv0t20 = 0;
                            } else {
                              int _sv0t21;
                              if ((shape == 15)) {
                                return a;
                                _sv0t21 = 0;
                              } else {
                                int _sv0t22;
                                if ((shape == 16)) {
                                  return a;
                                  _sv0t22 = 0;
                                } else {
                                  int _sv0t23;
                                  if ((shape == 17)) {
                                    int _sv0t24 = aon_add(a, b);
                                    return _sv0t24;
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
                      _sv0t15 = _sv0t17;
                    }
                    _sv0t14 = _sv0t15;
                  }
                  _sv0t12 = _sv0t14;
                }
                _sv0t10 = _sv0t12;
              }
              _sv0t8 = _sv0t10;
            }
            _sv0t6 = _sv0t8;
          }
          _sv0t5 = _sv0t6;
        }
        _sv0t3 = _sv0t5;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = all_old_names_count(1, 0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = all_old_names_count(2, 9, 9, 9);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = all_old_names_count(3, 1, 2, 0);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = all_old_names_count(5, 0, 1, 0);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = all_old_names_count(10, 0, 1, 0);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = all_old_names_count(17, 2, 1, 0);
  if ((_sv0t5 != 3)) {
    return 1;
  } else {
  }
  int _sv0t6 = all_old_names_count(99, 5, 5, 5);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

