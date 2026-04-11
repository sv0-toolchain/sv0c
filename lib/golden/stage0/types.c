#include "sv0_runtime.h"

typedef struct {
  int tag;
  int p0;
  int p1;
} Ty;

static Ty fresh_var(int ctr);
static int ty_tag(Ty t);

static Ty fresh_var(int ctr) {
  Ty _sv0t0;
  _sv0t0.tag = 10;
  _sv0t0.p0 = ctr;
  return _sv0t0;
}

static int ty_tag(Ty t) {
  int _sv0t0;
  if ((t.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((t.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((t.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
        if ((t.tag == 3)) {
          return 3;
          _sv0t0 = 0;
        } else {
          if ((t.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
            if ((t.tag == 5)) {
              return 5;
              _sv0t0 = 0;
            } else {
              if ((t.tag == 6)) {
                return 6;
                _sv0t0 = 0;
              } else {
                if ((t.tag == 7)) {
                  int w = t.p0;
                  return 7;
                  _sv0t0 = 0;
                } else {
                  if ((t.tag == 8)) {
                    int w = t.p0;
                    return 8;
                    _sv0t0 = 0;
                  } else {
                    if ((t.tag == 9)) {
                      int w = t.p0;
                      return 9;
                      _sv0t0 = 0;
                    } else {
                      if ((t.tag == 10)) {
                        int n = t.p0;
                        return 10;
                        _sv0t0 = 0;
                      } else {
                        if ((t.tag == 11)) {
                          int id = t.p0;
                          return 11;
                          _sv0t0 = 0;
                        } else {
                          if ((t.tag == 12)) {
                            int id = t.p0;
                            return 12;
                            _sv0t0 = 0;
                          } else {
                            if ((t.tag == 13)) {
                              int b = t.p0;
                              return 13;
                              _sv0t0 = 0;
                            } else {
                              if ((t.tag == 14)) {
                                int b = t.p0;
                                return 14;
                                _sv0t0 = 0;
                              } else {
                                if ((t.tag == 15)) {
                                  int b = t.p0;
                                  return 15;
                                  _sv0t0 = 0;
                                } else {
                                  if ((t.tag == 16)) {
                                    int b = t.p0;
                                    int n = t.p1;
                                    return 16;
                                    _sv0t0 = 0;
                                  } else {
                                    if ((t.tag == 17)) {
                                      int v = t.p0;
                                      return 17;
                                      _sv0t0 = 0;
                                    } else {
                                      if ((t.tag == 18)) {
                                        int ps = t.p0;
                                        int rb = t.p1;
                                        return 18;
                                        _sv0t0 = 0;
                                      } else {
                                        if ((t.tag == 19)) {
                                          int id = t.p0;
                                          int args = t.p1;
                                          return 19;
                                          _sv0t0 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return _sv0t0;
}

int main(void) {
  Ty u;
  u.tag = 0;
  int _sv0t0 = ty_tag(u);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Ty b;
  b.tag = 1;
  int _sv0t1 = ty_tag(b);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  Ty ch;
  ch.tag = 2;
  int _sv0t2 = ty_tag(ch);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  Ty s;
  s.tag = 3;
  int _sv0t3 = ty_tag(s);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  Ty is;
  is.tag = 4;
  int _sv0t4 = ty_tag(is);
  if ((_sv0t4 != 4)) {
    return 5;
  } else {
  }
  Ty us;
  us.tag = 5;
  int _sv0t5 = ty_tag(us);
  if ((_sv0t5 != 5)) {
    return 6;
  } else {
  }
  Ty e;
  e.tag = 6;
  int _sv0t6 = ty_tag(e);
  if ((_sv0t6 != 6)) {
    return 7;
  } else {
  }
  Ty i32t;
  i32t.tag = 7;
  i32t.p0 = 32;
  int _sv0t7 = ty_tag(i32t);
  if ((_sv0t7 != 7)) {
    return 8;
  } else {
  }
  int _sv0t8;
  if ((i32t.tag == 7)) {
    int w = i32t.p0;
    if ((w != 32)) {
      return 9;
      _sv0t8 = 0;
    } else {
      _sv0t8 = 0;
    }
  } else {
    if ((i32t.tag == 0)) {
      return 9;
      _sv0t8 = 0;
    } else {
      if ((i32t.tag == 1)) {
        return 9;
        _sv0t8 = 0;
      } else {
        if ((i32t.tag == 2)) {
          return 9;
          _sv0t8 = 0;
        } else {
          if ((i32t.tag == 3)) {
            return 9;
            _sv0t8 = 0;
          } else {
            if ((i32t.tag == 4)) {
              return 9;
              _sv0t8 = 0;
            } else {
              if ((i32t.tag == 5)) {
                return 9;
                _sv0t8 = 0;
              } else {
                if ((i32t.tag == 6)) {
                  return 9;
                  _sv0t8 = 0;
                } else {
                  if ((i32t.tag == 8)) {
                    int w = i32t.p0;
                    return 9;
                    _sv0t8 = 0;
                  } else {
                    if ((i32t.tag == 9)) {
                      int w = i32t.p0;
                      return 9;
                      _sv0t8 = 0;
                    } else {
                      if ((i32t.tag == 10)) {
                        int n = i32t.p0;
                        return 9;
                        _sv0t8 = 0;
                      } else {
                        if ((i32t.tag == 11)) {
                          int id = i32t.p0;
                          return 9;
                          _sv0t8 = 0;
                        } else {
                          if ((i32t.tag == 12)) {
                            int id = i32t.p0;
                            return 9;
                            _sv0t8 = 0;
                          } else {
                            if ((i32t.tag == 13)) {
                              int bt = i32t.p0;
                              return 9;
                              _sv0t8 = 0;
                            } else {
                              if ((i32t.tag == 14)) {
                                int bt = i32t.p0;
                                return 9;
                                _sv0t8 = 0;
                              } else {
                                if ((i32t.tag == 15)) {
                                  int bt = i32t.p0;
                                  return 9;
                                  _sv0t8 = 0;
                                } else {
                                  if ((i32t.tag == 16)) {
                                    int bt = i32t.p0;
                                    int n = i32t.p1;
                                    return 9;
                                    _sv0t8 = 0;
                                  } else {
                                    if ((i32t.tag == 17)) {
                                      int v = i32t.p0;
                                      return 9;
                                      _sv0t8 = 0;
                                    } else {
                                      if ((i32t.tag == 18)) {
                                        int ps = i32t.p0;
                                        int rb = i32t.p1;
                                        return 9;
                                        _sv0t8 = 0;
                                      } else {
                                        if ((i32t.tag == 19)) {
                                          int id = i32t.p0;
                                          int args = i32t.p1;
                                          return 9;
                                          _sv0t8 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  Ty _sv0t9 = fresh_var(42);
  Ty tv;
  tv = _sv0t9;
  int _sv0t10 = ty_tag(tv);
  if ((_sv0t10 != 10)) {
    return 10;
  } else {
  }
  int _sv0t11;
  if ((tv.tag == 10)) {
    int n = tv.p0;
    if ((n != 42)) {
      return 11;
      _sv0t11 = 0;
    } else {
      _sv0t11 = 0;
    }
  } else {
    if ((tv.tag == 0)) {
      return 11;
      _sv0t11 = 0;
    } else {
      if ((tv.tag == 1)) {
        return 11;
        _sv0t11 = 0;
      } else {
        if ((tv.tag == 2)) {
          return 11;
          _sv0t11 = 0;
        } else {
          if ((tv.tag == 3)) {
            return 11;
            _sv0t11 = 0;
          } else {
            if ((tv.tag == 4)) {
              return 11;
              _sv0t11 = 0;
            } else {
              if ((tv.tag == 5)) {
                return 11;
                _sv0t11 = 0;
              } else {
                if ((tv.tag == 6)) {
                  return 11;
                  _sv0t11 = 0;
                } else {
                  if ((tv.tag == 7)) {
                    int w = tv.p0;
                    return 11;
                    _sv0t11 = 0;
                  } else {
                    if ((tv.tag == 8)) {
                      int w = tv.p0;
                      return 11;
                      _sv0t11 = 0;
                    } else {
                      if ((tv.tag == 9)) {
                        int w = tv.p0;
                        return 11;
                        _sv0t11 = 0;
                      } else {
                        if ((tv.tag == 11)) {
                          int id = tv.p0;
                          return 11;
                          _sv0t11 = 0;
                        } else {
                          if ((tv.tag == 12)) {
                            int id = tv.p0;
                            return 11;
                            _sv0t11 = 0;
                          } else {
                            if ((tv.tag == 13)) {
                              int bt = tv.p0;
                              return 11;
                              _sv0t11 = 0;
                            } else {
                              if ((tv.tag == 14)) {
                                int bt = tv.p0;
                                return 11;
                                _sv0t11 = 0;
                              } else {
                                if ((tv.tag == 15)) {
                                  int bt = tv.p0;
                                  return 11;
                                  _sv0t11 = 0;
                                } else {
                                  if ((tv.tag == 16)) {
                                    int bt = tv.p0;
                                    int n = tv.p1;
                                    return 11;
                                    _sv0t11 = 0;
                                  } else {
                                    if ((tv.tag == 17)) {
                                      int v = tv.p0;
                                      return 11;
                                      _sv0t11 = 0;
                                    } else {
                                      if ((tv.tag == 18)) {
                                        int ps = tv.p0;
                                        int rb = tv.p1;
                                        return 11;
                                        _sv0t11 = 0;
                                      } else {
                                        if ((tv.tag == 19)) {
                                          int id = tv.p0;
                                          int args = tv.p1;
                                          return 11;
                                          _sv0t11 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  Ty inner;
  inner.tag = 1;
  Ty ref_t;
  int _sv0t12 = sv0_box_alloc(3);
  sv0_box_store(_sv0t12, 0, inner.tag);
  sv0_box_store(_sv0t12, 1, inner.p0);
  sv0_box_store(_sv0t12, 2, inner.p1);
  ref_t.tag = 13;
  ref_t.p0 = _sv0t12;
  int _sv0t13 = ty_tag(ref_t);
  if ((_sv0t13 != 13)) {
    return 12;
  } else {
  }
  int _sv0t14;
  if ((ref_t.tag == 13)) {
    int bt = ref_t.p0;
    Ty _sv0t15;
    int _sv0t16 = sv0_box_load(bt, 0);
    _sv0t15.tag = _sv0t16;
    int _sv0t17 = sv0_box_load(bt, 1);
    _sv0t15.p0 = _sv0t17;
    int _sv0t18 = sv0_box_load(bt, 2);
    _sv0t15.p1 = _sv0t18;
    Ty deref;
    deref.tag = (_sv0t15).tag;
    deref.p0 = (_sv0t15).p0;
    deref.p1 = (_sv0t15).p1;
    int _sv0t19 = ty_tag(deref);
    if ((_sv0t19 != 1)) {
      return 13;
      _sv0t14 = 0;
    } else {
      _sv0t14 = 0;
    }
  } else {
    if ((ref_t.tag == 0)) {
      return 13;
      _sv0t14 = 0;
    } else {
      if ((ref_t.tag == 1)) {
        return 13;
        _sv0t14 = 0;
      } else {
        if ((ref_t.tag == 2)) {
          return 13;
          _sv0t14 = 0;
        } else {
          if ((ref_t.tag == 3)) {
            return 13;
            _sv0t14 = 0;
          } else {
            if ((ref_t.tag == 4)) {
              return 13;
              _sv0t14 = 0;
            } else {
              if ((ref_t.tag == 5)) {
                return 13;
                _sv0t14 = 0;
              } else {
                if ((ref_t.tag == 6)) {
                  return 13;
                  _sv0t14 = 0;
                } else {
                  if ((ref_t.tag == 7)) {
                    int w = ref_t.p0;
                    return 13;
                    _sv0t14 = 0;
                  } else {
                    if ((ref_t.tag == 8)) {
                      int w = ref_t.p0;
                      return 13;
                      _sv0t14 = 0;
                    } else {
                      if ((ref_t.tag == 9)) {
                        int w = ref_t.p0;
                        return 13;
                        _sv0t14 = 0;
                      } else {
                        if ((ref_t.tag == 10)) {
                          int n = ref_t.p0;
                          return 13;
                          _sv0t14 = 0;
                        } else {
                          if ((ref_t.tag == 11)) {
                            int id = ref_t.p0;
                            return 13;
                            _sv0t14 = 0;
                          } else {
                            if ((ref_t.tag == 12)) {
                              int id = ref_t.p0;
                              return 13;
                              _sv0t14 = 0;
                            } else {
                              if ((ref_t.tag == 14)) {
                                int bt = ref_t.p0;
                                return 13;
                                _sv0t14 = 0;
                              } else {
                                if ((ref_t.tag == 15)) {
                                  int bt = ref_t.p0;
                                  return 13;
                                  _sv0t14 = 0;
                                } else {
                                  if ((ref_t.tag == 16)) {
                                    int bt = ref_t.p0;
                                    int n = ref_t.p1;
                                    return 13;
                                    _sv0t14 = 0;
                                  } else {
                                    if ((ref_t.tag == 17)) {
                                      int v = ref_t.p0;
                                      return 13;
                                      _sv0t14 = 0;
                                    } else {
                                      if ((ref_t.tag == 18)) {
                                        int ps = ref_t.p0;
                                        int rb = ref_t.p1;
                                        return 13;
                                        _sv0t14 = 0;
                                      } else {
                                        if ((ref_t.tag == 19)) {
                                          int id = ref_t.p0;
                                          int args = ref_t.p1;
                                          return 13;
                                          _sv0t14 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  Ty elem;
  elem.tag = 7;
  elem.p0 = 64;
  Ty arr;
  int _sv0t20 = sv0_box_alloc(3);
  sv0_box_store(_sv0t20, 0, elem.tag);
  sv0_box_store(_sv0t20, 1, elem.p0);
  sv0_box_store(_sv0t20, 2, elem.p1);
  arr.tag = 16;
  arr.p0 = _sv0t20;
  arr.p1 = 10;
  int _sv0t21 = ty_tag(arr);
  if ((_sv0t21 != 16)) {
    return 14;
  } else {
  }
  int _sv0t22;
  if ((arr.tag == 16)) {
    int bt = arr.p0;
    int len = arr.p1;
    if ((len != 10)) {
      return 15;
    } else {
    }
    Ty _sv0t23;
    int _sv0t24 = sv0_box_load(bt, 0);
    _sv0t23.tag = _sv0t24;
    int _sv0t25 = sv0_box_load(bt, 1);
    _sv0t23.p0 = _sv0t25;
    int _sv0t26 = sv0_box_load(bt, 2);
    _sv0t23.p1 = _sv0t26;
    Ty et;
    et.tag = (_sv0t23).tag;
    et.p0 = (_sv0t23).p0;
    et.p1 = (_sv0t23).p1;
    int _sv0t27 = ty_tag(et);
    if ((_sv0t27 != 7)) {
      return 16;
      _sv0t22 = 0;
    } else {
      _sv0t22 = 0;
    }
  } else {
    if ((arr.tag == 0)) {
      return 15;
      _sv0t22 = 0;
    } else {
      if ((arr.tag == 1)) {
        return 15;
        _sv0t22 = 0;
      } else {
        if ((arr.tag == 2)) {
          return 15;
          _sv0t22 = 0;
        } else {
          if ((arr.tag == 3)) {
            return 15;
            _sv0t22 = 0;
          } else {
            if ((arr.tag == 4)) {
              return 15;
              _sv0t22 = 0;
            } else {
              if ((arr.tag == 5)) {
                return 15;
                _sv0t22 = 0;
              } else {
                if ((arr.tag == 6)) {
                  return 15;
                  _sv0t22 = 0;
                } else {
                  if ((arr.tag == 7)) {
                    int w = arr.p0;
                    return 15;
                    _sv0t22 = 0;
                  } else {
                    if ((arr.tag == 8)) {
                      int w = arr.p0;
                      return 15;
                      _sv0t22 = 0;
                    } else {
                      if ((arr.tag == 9)) {
                        int w = arr.p0;
                        return 15;
                        _sv0t22 = 0;
                      } else {
                        if ((arr.tag == 10)) {
                          int n = arr.p0;
                          return 15;
                          _sv0t22 = 0;
                        } else {
                          if ((arr.tag == 11)) {
                            int id = arr.p0;
                            return 15;
                            _sv0t22 = 0;
                          } else {
                            if ((arr.tag == 12)) {
                              int id = arr.p0;
                              return 15;
                              _sv0t22 = 0;
                            } else {
                              if ((arr.tag == 13)) {
                                int bt = arr.p0;
                                return 15;
                                _sv0t22 = 0;
                              } else {
                                if ((arr.tag == 14)) {
                                  int bt = arr.p0;
                                  return 15;
                                  _sv0t22 = 0;
                                } else {
                                  if ((arr.tag == 15)) {
                                    int bt = arr.p0;
                                    return 15;
                                    _sv0t22 = 0;
                                  } else {
                                    if ((arr.tag == 17)) {
                                      int v = arr.p0;
                                      return 15;
                                      _sv0t22 = 0;
                                    } else {
                                      if ((arr.tag == 18)) {
                                        int ps = arr.p0;
                                        int rb = arr.p1;
                                        return 15;
                                        _sv0t22 = 0;
                                      } else {
                                        if ((arr.tag == 19)) {
                                          int id = arr.p0;
                                          int args = arr.p1;
                                          return 15;
                                          _sv0t22 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  int _sv0t28 = sv0_vec_new();
  int tup_elems = _sv0t28;
  sv0_vec_push(tup_elems, 0);
  sv0_vec_push(tup_elems, 1);
  sv0_vec_push(tup_elems, 2);
  Ty tup;
  tup.tag = 17;
  tup.p0 = tup_elems;
  int _sv0t29 = ty_tag(tup);
  if ((_sv0t29 != 17)) {
    return 17;
  } else {
  }
  int _sv0t30;
  if ((tup.tag == 17)) {
    int v = tup.p0;
    int _sv0t31 = sv0_vec_len(v);
    if ((_sv0t31 != 3)) {
      return 18;
      _sv0t30 = 0;
    } else {
      _sv0t30 = 0;
    }
  } else {
    if ((tup.tag == 0)) {
      return 18;
      _sv0t30 = 0;
    } else {
      if ((tup.tag == 1)) {
        return 18;
        _sv0t30 = 0;
      } else {
        if ((tup.tag == 2)) {
          return 18;
          _sv0t30 = 0;
        } else {
          if ((tup.tag == 3)) {
            return 18;
            _sv0t30 = 0;
          } else {
            if ((tup.tag == 4)) {
              return 18;
              _sv0t30 = 0;
            } else {
              if ((tup.tag == 5)) {
                return 18;
                _sv0t30 = 0;
              } else {
                if ((tup.tag == 6)) {
                  return 18;
                  _sv0t30 = 0;
                } else {
                  if ((tup.tag == 7)) {
                    int w = tup.p0;
                    return 18;
                    _sv0t30 = 0;
                  } else {
                    if ((tup.tag == 8)) {
                      int w = tup.p0;
                      return 18;
                      _sv0t30 = 0;
                    } else {
                      if ((tup.tag == 9)) {
                        int w = tup.p0;
                        return 18;
                        _sv0t30 = 0;
                      } else {
                        if ((tup.tag == 10)) {
                          int n = tup.p0;
                          return 18;
                          _sv0t30 = 0;
                        } else {
                          if ((tup.tag == 11)) {
                            int id = tup.p0;
                            return 18;
                            _sv0t30 = 0;
                          } else {
                            if ((tup.tag == 12)) {
                              int id = tup.p0;
                              return 18;
                              _sv0t30 = 0;
                            } else {
                              if ((tup.tag == 13)) {
                                int bt = tup.p0;
                                return 18;
                                _sv0t30 = 0;
                              } else {
                                if ((tup.tag == 14)) {
                                  int bt = tup.p0;
                                  return 18;
                                  _sv0t30 = 0;
                                } else {
                                  if ((tup.tag == 15)) {
                                    int bt = tup.p0;
                                    return 18;
                                    _sv0t30 = 0;
                                  } else {
                                    if ((tup.tag == 16)) {
                                      int bt = tup.p0;
                                      int n = tup.p1;
                                      return 18;
                                      _sv0t30 = 0;
                                    } else {
                                      if ((tup.tag == 18)) {
                                        int ps = tup.p0;
                                        int rb = tup.p1;
                                        return 18;
                                        _sv0t30 = 0;
                                      } else {
                                        if ((tup.tag == 19)) {
                                          int id = tup.p0;
                                          int args = tup.p1;
                                          return 18;
                                          _sv0t30 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  int _sv0t32 = sv0_vec_new();
  int params = _sv0t32;
  sv0_vec_push(params, 0);
  sv0_vec_push(params, 1);
  Ty ret;
  ret.tag = 1;
  Ty fn_t;
  int _sv0t33 = sv0_box_alloc(3);
  sv0_box_store(_sv0t33, 0, ret.tag);
  sv0_box_store(_sv0t33, 1, ret.p0);
  sv0_box_store(_sv0t33, 2, ret.p1);
  fn_t.tag = 18;
  fn_t.p0 = params;
  fn_t.p1 = _sv0t33;
  int _sv0t34 = ty_tag(fn_t);
  if ((_sv0t34 != 18)) {
    return 19;
  } else {
  }
  int _sv0t35;
  if ((fn_t.tag == 18)) {
    int ps = fn_t.p0;
    int rb = fn_t.p1;
    int _sv0t36 = sv0_vec_len(ps);
    if ((_sv0t36 != 2)) {
      return 20;
    } else {
    }
    Ty _sv0t37;
    int _sv0t38 = sv0_box_load(rb, 0);
    _sv0t37.tag = _sv0t38;
    int _sv0t39 = sv0_box_load(rb, 1);
    _sv0t37.p0 = _sv0t39;
    int _sv0t40 = sv0_box_load(rb, 2);
    _sv0t37.p1 = _sv0t40;
    Ty rt;
    rt.tag = (_sv0t37).tag;
    rt.p0 = (_sv0t37).p0;
    rt.p1 = (_sv0t37).p1;
    int _sv0t41 = ty_tag(rt);
    if ((_sv0t41 != 1)) {
      return 21;
      _sv0t35 = 0;
    } else {
      _sv0t35 = 0;
    }
  } else {
    if ((fn_t.tag == 0)) {
      return 20;
      _sv0t35 = 0;
    } else {
      if ((fn_t.tag == 1)) {
        return 20;
        _sv0t35 = 0;
      } else {
        if ((fn_t.tag == 2)) {
          return 20;
          _sv0t35 = 0;
        } else {
          if ((fn_t.tag == 3)) {
            return 20;
            _sv0t35 = 0;
          } else {
            if ((fn_t.tag == 4)) {
              return 20;
              _sv0t35 = 0;
            } else {
              if ((fn_t.tag == 5)) {
                return 20;
                _sv0t35 = 0;
              } else {
                if ((fn_t.tag == 6)) {
                  return 20;
                  _sv0t35 = 0;
                } else {
                  if ((fn_t.tag == 7)) {
                    int w = fn_t.p0;
                    return 20;
                    _sv0t35 = 0;
                  } else {
                    if ((fn_t.tag == 8)) {
                      int w = fn_t.p0;
                      return 20;
                      _sv0t35 = 0;
                    } else {
                      if ((fn_t.tag == 9)) {
                        int w = fn_t.p0;
                        return 20;
                        _sv0t35 = 0;
                      } else {
                        if ((fn_t.tag == 10)) {
                          int n = fn_t.p0;
                          return 20;
                          _sv0t35 = 0;
                        } else {
                          if ((fn_t.tag == 11)) {
                            int id = fn_t.p0;
                            return 20;
                            _sv0t35 = 0;
                          } else {
                            if ((fn_t.tag == 12)) {
                              int id = fn_t.p0;
                              return 20;
                              _sv0t35 = 0;
                            } else {
                              if ((fn_t.tag == 13)) {
                                int bt = fn_t.p0;
                                return 20;
                                _sv0t35 = 0;
                              } else {
                                if ((fn_t.tag == 14)) {
                                  int bt = fn_t.p0;
                                  return 20;
                                  _sv0t35 = 0;
                                } else {
                                  if ((fn_t.tag == 15)) {
                                    int bt = fn_t.p0;
                                    return 20;
                                    _sv0t35 = 0;
                                  } else {
                                    if ((fn_t.tag == 16)) {
                                      int bt = fn_t.p0;
                                      int n = fn_t.p1;
                                      return 20;
                                      _sv0t35 = 0;
                                    } else {
                                      if ((fn_t.tag == 17)) {
                                        int v = fn_t.p0;
                                        return 20;
                                        _sv0t35 = 0;
                                      } else {
                                        if ((fn_t.tag == 19)) {
                                          int id = fn_t.p0;
                                          int args = fn_t.p1;
                                          return 20;
                                          _sv0t35 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  int _sv0t42 = sv0_vec_new();
  int args = _sv0t42;
  sv0_vec_push(args, 5);
  Ty named;
  named.tag = 19;
  named.p0 = 99;
  named.p1 = args;
  int _sv0t43 = ty_tag(named);
  if ((_sv0t43 != 19)) {
    return 22;
  } else {
  }
  int _sv0t44;
  if ((named.tag == 19)) {
    int name_id = named.p0;
    int type_args = named.p1;
    if ((name_id != 99)) {
      return 23;
    } else {
    }
    int _sv0t45 = sv0_vec_len(type_args);
    if ((_sv0t45 != 1)) {
      return 24;
      _sv0t44 = 0;
    } else {
      _sv0t44 = 0;
    }
  } else {
    if ((named.tag == 0)) {
      return 23;
      _sv0t44 = 0;
    } else {
      if ((named.tag == 1)) {
        return 23;
        _sv0t44 = 0;
      } else {
        if ((named.tag == 2)) {
          return 23;
          _sv0t44 = 0;
        } else {
          if ((named.tag == 3)) {
            return 23;
            _sv0t44 = 0;
          } else {
            if ((named.tag == 4)) {
              return 23;
              _sv0t44 = 0;
            } else {
              if ((named.tag == 5)) {
                return 23;
                _sv0t44 = 0;
              } else {
                if ((named.tag == 6)) {
                  return 23;
                  _sv0t44 = 0;
                } else {
                  if ((named.tag == 7)) {
                    int w = named.p0;
                    return 23;
                    _sv0t44 = 0;
                  } else {
                    if ((named.tag == 8)) {
                      int w = named.p0;
                      return 23;
                      _sv0t44 = 0;
                    } else {
                      if ((named.tag == 9)) {
                        int w = named.p0;
                        return 23;
                        _sv0t44 = 0;
                      } else {
                        if ((named.tag == 10)) {
                          int n = named.p0;
                          return 23;
                          _sv0t44 = 0;
                        } else {
                          if ((named.tag == 11)) {
                            int id = named.p0;
                            return 23;
                            _sv0t44 = 0;
                          } else {
                            if ((named.tag == 12)) {
                              int id = named.p0;
                              return 23;
                              _sv0t44 = 0;
                            } else {
                              if ((named.tag == 13)) {
                                int bt = named.p0;
                                return 23;
                                _sv0t44 = 0;
                              } else {
                                if ((named.tag == 14)) {
                                  int bt = named.p0;
                                  return 23;
                                  _sv0t44 = 0;
                                } else {
                                  if ((named.tag == 15)) {
                                    int bt = named.p0;
                                    return 23;
                                    _sv0t44 = 0;
                                  } else {
                                    if ((named.tag == 16)) {
                                      int bt = named.p0;
                                      int n = named.p1;
                                      return 23;
                                      _sv0t44 = 0;
                                    } else {
                                      if ((named.tag == 17)) {
                                        int v = named.p0;
                                        return 23;
                                        _sv0t44 = 0;
                                      } else {
                                        if ((named.tag == 18)) {
                                          int ps = named.p0;
                                          int rb = named.p1;
                                          return 23;
                                          _sv0t44 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  Ty leaf;
  leaf.tag = 7;
  leaf.p0 = 8;
  Ty mid;
  int _sv0t46 = sv0_box_alloc(3);
  sv0_box_store(_sv0t46, 0, leaf.tag);
  sv0_box_store(_sv0t46, 1, leaf.p0);
  sv0_box_store(_sv0t46, 2, leaf.p1);
  mid.tag = 13;
  mid.p0 = _sv0t46;
  Ty outer;
  int _sv0t47 = sv0_box_alloc(3);
  sv0_box_store(_sv0t47, 0, mid.tag);
  sv0_box_store(_sv0t47, 1, mid.p0);
  sv0_box_store(_sv0t47, 2, mid.p1);
  outer.tag = 13;
  outer.p0 = _sv0t47;
  int _sv0t48;
  if ((outer.tag == 13)) {
    int b1 = outer.p0;
    Ty _sv0t49;
    int _sv0t50 = sv0_box_load(b1, 0);
    _sv0t49.tag = _sv0t50;
    int _sv0t51 = sv0_box_load(b1, 1);
    _sv0t49.p0 = _sv0t51;
    int _sv0t52 = sv0_box_load(b1, 2);
    _sv0t49.p1 = _sv0t52;
    Ty m;
    m.tag = (_sv0t49).tag;
    m.p0 = (_sv0t49).p0;
    m.p1 = (_sv0t49).p1;
    int _sv0t53;
    if ((m.tag == 13)) {
      int b2 = m.p0;
      Ty _sv0t54;
      int _sv0t55 = sv0_box_load(b2, 0);
      _sv0t54.tag = _sv0t55;
      int _sv0t56 = sv0_box_load(b2, 1);
      _sv0t54.p0 = _sv0t56;
      int _sv0t57 = sv0_box_load(b2, 2);
      _sv0t54.p1 = _sv0t57;
      Ty l;
      l.tag = (_sv0t54).tag;
      l.p0 = (_sv0t54).p0;
      l.p1 = (_sv0t54).p1;
      int _sv0t58;
      if ((l.tag == 7)) {
        int w = l.p0;
        if ((w != 8)) {
          return 25;
          _sv0t58 = 0;
        } else {
          _sv0t58 = 0;
        }
      } else {
        if ((l.tag == 0)) {
          return 25;
          _sv0t58 = 0;
        } else {
          if ((l.tag == 1)) {
            return 25;
            _sv0t58 = 0;
          } else {
            if ((l.tag == 2)) {
              return 25;
              _sv0t58 = 0;
            } else {
              if ((l.tag == 3)) {
                return 25;
                _sv0t58 = 0;
              } else {
                if ((l.tag == 4)) {
                  return 25;
                  _sv0t58 = 0;
                } else {
                  if ((l.tag == 5)) {
                    return 25;
                    _sv0t58 = 0;
                  } else {
                    if ((l.tag == 6)) {
                      return 25;
                      _sv0t58 = 0;
                    } else {
                      if ((l.tag == 8)) {
                        int w2 = l.p0;
                        return 25;
                        _sv0t58 = 0;
                      } else {
                        if ((l.tag == 9)) {
                          int w2 = l.p0;
                          return 25;
                          _sv0t58 = 0;
                        } else {
                          if ((l.tag == 10)) {
                            int n = l.p0;
                            return 25;
                            _sv0t58 = 0;
                          } else {
                            if ((l.tag == 11)) {
                              int id = l.p0;
                              return 25;
                              _sv0t58 = 0;
                            } else {
                              if ((l.tag == 12)) {
                                int id = l.p0;
                                return 25;
                                _sv0t58 = 0;
                              } else {
                                if ((l.tag == 13)) {
                                  int bt = l.p0;
                                  return 25;
                                  _sv0t58 = 0;
                                } else {
                                  if ((l.tag == 14)) {
                                    int bt = l.p0;
                                    return 25;
                                    _sv0t58 = 0;
                                  } else {
                                    if ((l.tag == 15)) {
                                      int bt = l.p0;
                                      return 25;
                                      _sv0t58 = 0;
                                    } else {
                                      if ((l.tag == 16)) {
                                        int bt = l.p0;
                                        int n = l.p1;
                                        return 25;
                                        _sv0t58 = 0;
                                      } else {
                                        if ((l.tag == 17)) {
                                          int v = l.p0;
                                          return 25;
                                          _sv0t58 = 0;
                                        } else {
                                          if ((l.tag == 18)) {
                                            int ps = l.p0;
                                            int rb = l.p1;
                                            return 25;
                                            _sv0t58 = 0;
                                          } else {
                                            if ((l.tag == 19)) {
                                              int id = l.p0;
                                              int ag = l.p1;
                                              return 25;
                                              _sv0t58 = 0;
                                            } else {
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      _sv0t53 = _sv0t58;
    } else {
      if ((m.tag == 0)) {
        return 25;
        _sv0t53 = 0;
      } else {
        if ((m.tag == 1)) {
          return 25;
          _sv0t53 = 0;
        } else {
          if ((m.tag == 2)) {
            return 25;
            _sv0t53 = 0;
          } else {
            if ((m.tag == 3)) {
              return 25;
              _sv0t53 = 0;
            } else {
              if ((m.tag == 4)) {
                return 25;
                _sv0t53 = 0;
              } else {
                if ((m.tag == 5)) {
                  return 25;
                  _sv0t53 = 0;
                } else {
                  if ((m.tag == 6)) {
                    return 25;
                    _sv0t53 = 0;
                  } else {
                    if ((m.tag == 7)) {
                      int w = m.p0;
                      return 25;
                      _sv0t53 = 0;
                    } else {
                      if ((m.tag == 8)) {
                        int w = m.p0;
                        return 25;
                        _sv0t53 = 0;
                      } else {
                        if ((m.tag == 9)) {
                          int w = m.p0;
                          return 25;
                          _sv0t53 = 0;
                        } else {
                          if ((m.tag == 10)) {
                            int n = m.p0;
                            return 25;
                            _sv0t53 = 0;
                          } else {
                            if ((m.tag == 11)) {
                              int id = m.p0;
                              return 25;
                              _sv0t53 = 0;
                            } else {
                              if ((m.tag == 12)) {
                                int id = m.p0;
                                return 25;
                                _sv0t53 = 0;
                              } else {
                                if ((m.tag == 14)) {
                                  int bt = m.p0;
                                  return 25;
                                  _sv0t53 = 0;
                                } else {
                                  if ((m.tag == 15)) {
                                    int bt = m.p0;
                                    return 25;
                                    _sv0t53 = 0;
                                  } else {
                                    if ((m.tag == 16)) {
                                      int bt = m.p0;
                                      int n = m.p1;
                                      return 25;
                                      _sv0t53 = 0;
                                    } else {
                                      if ((m.tag == 17)) {
                                        int v = m.p0;
                                        return 25;
                                        _sv0t53 = 0;
                                      } else {
                                        if ((m.tag == 18)) {
                                          int ps = m.p0;
                                          int rb = m.p1;
                                          return 25;
                                          _sv0t53 = 0;
                                        } else {
                                          if ((m.tag == 19)) {
                                            int id = m.p0;
                                            int ag = m.p1;
                                            return 25;
                                            _sv0t53 = 0;
                                          } else {
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    _sv0t48 = _sv0t53;
  } else {
    if ((outer.tag == 0)) {
      return 25;
      _sv0t48 = 0;
    } else {
      if ((outer.tag == 1)) {
        return 25;
        _sv0t48 = 0;
      } else {
        if ((outer.tag == 2)) {
          return 25;
          _sv0t48 = 0;
        } else {
          if ((outer.tag == 3)) {
            return 25;
            _sv0t48 = 0;
          } else {
            if ((outer.tag == 4)) {
              return 25;
              _sv0t48 = 0;
            } else {
              if ((outer.tag == 5)) {
                return 25;
                _sv0t48 = 0;
              } else {
                if ((outer.tag == 6)) {
                  return 25;
                  _sv0t48 = 0;
                } else {
                  if ((outer.tag == 7)) {
                    int w = outer.p0;
                    return 25;
                    _sv0t48 = 0;
                  } else {
                    if ((outer.tag == 8)) {
                      int w = outer.p0;
                      return 25;
                      _sv0t48 = 0;
                    } else {
                      if ((outer.tag == 9)) {
                        int w = outer.p0;
                        return 25;
                        _sv0t48 = 0;
                      } else {
                        if ((outer.tag == 10)) {
                          int n = outer.p0;
                          return 25;
                          _sv0t48 = 0;
                        } else {
                          if ((outer.tag == 11)) {
                            int id = outer.p0;
                            return 25;
                            _sv0t48 = 0;
                          } else {
                            if ((outer.tag == 12)) {
                              int id = outer.p0;
                              return 25;
                              _sv0t48 = 0;
                            } else {
                              if ((outer.tag == 14)) {
                                int bt = outer.p0;
                                return 25;
                                _sv0t48 = 0;
                              } else {
                                if ((outer.tag == 15)) {
                                  int bt = outer.p0;
                                  return 25;
                                  _sv0t48 = 0;
                                } else {
                                  if ((outer.tag == 16)) {
                                    int bt = outer.p0;
                                    int n = outer.p1;
                                    return 25;
                                    _sv0t48 = 0;
                                  } else {
                                    if ((outer.tag == 17)) {
                                      int v = outer.p0;
                                      return 25;
                                      _sv0t48 = 0;
                                    } else {
                                      if ((outer.tag == 18)) {
                                        int ps = outer.p0;
                                        int rb = outer.p1;
                                        return 25;
                                        _sv0t48 = 0;
                                      } else {
                                        if ((outer.tag == 19)) {
                                          int id = outer.p0;
                                          int ag = outer.p1;
                                          return 25;
                                          _sv0t48 = 0;
                                        } else {
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

