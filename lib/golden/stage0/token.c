#include "sv0_runtime.h"

typedef struct {
  int tag;
  int p0;
} Token;

static int token_tag(Token t);
static int test_literals(void);
static int test_delimiters(void);
static int test_operators(void);
static int test_keywords(void);

static int token_tag(Token t) {
  int _sv0t0;
  if ((t.tag == 0)) {
    int v = t.p0;
    return 0;
    _sv0t0 = 0;
  } else {
    if ((t.tag == 1)) {
      int v = t.p0;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((t.tag == 2)) {
        int v = t.p0;
        return 2;
        _sv0t0 = 0;
      } else {
        if ((t.tag == 3)) {
          int v = t.p0;
          return 3;
          _sv0t0 = 0;
        } else {
          if ((t.tag == 4)) {
            int v = t.p0;
            return 4;
            _sv0t0 = 0;
          } else {
            if ((t.tag == 5)) {
              int v = t.p0;
              return 5;
              _sv0t0 = 0;
            } else {
              if ((t.tag == 6)) {
                return 6;
                _sv0t0 = 0;
              } else {
                if ((t.tag == 7)) {
                  return 7;
                  _sv0t0 = 0;
                } else {
                  if ((t.tag == 8)) {
                    return 8;
                    _sv0t0 = 0;
                  } else {
                    if ((t.tag == 9)) {
                      return 9;
                      _sv0t0 = 0;
                    } else {
                      if ((t.tag == 10)) {
                        return 10;
                        _sv0t0 = 0;
                      } else {
                        if ((t.tag == 11)) {
                          return 11;
                          _sv0t0 = 0;
                        } else {
                          if ((t.tag == 12)) {
                            return 12;
                            _sv0t0 = 0;
                          } else {
                            if ((t.tag == 13)) {
                              return 13;
                              _sv0t0 = 0;
                            } else {
                              if ((t.tag == 14)) {
                                return 14;
                                _sv0t0 = 0;
                              } else {
                                if ((t.tag == 15)) {
                                  return 15;
                                  _sv0t0 = 0;
                                } else {
                                  if ((t.tag == 16)) {
                                    return 16;
                                    _sv0t0 = 0;
                                  } else {
                                    if ((t.tag == 17)) {
                                      return 17;
                                      _sv0t0 = 0;
                                    } else {
                                      if ((t.tag == 18)) {
                                        return 18;
                                        _sv0t0 = 0;
                                      } else {
                                        if ((t.tag == 19)) {
                                          return 19;
                                          _sv0t0 = 0;
                                        } else {
                                          if ((t.tag == 20)) {
                                            return 20;
                                            _sv0t0 = 0;
                                          } else {
                                            if ((t.tag == 21)) {
                                              return 21;
                                              _sv0t0 = 0;
                                            } else {
                                              if ((t.tag == 22)) {
                                                return 22;
                                                _sv0t0 = 0;
                                              } else {
                                                if ((t.tag == 23)) {
                                                  return 23;
                                                  _sv0t0 = 0;
                                                } else {
                                                  if ((t.tag == 24)) {
                                                    return 24;
                                                    _sv0t0 = 0;
                                                  } else {
                                                    if ((t.tag == 25)) {
                                                      return 25;
                                                      _sv0t0 = 0;
                                                    } else {
                                                      if ((t.tag == 26)) {
                                                        return 26;
                                                        _sv0t0 = 0;
                                                      } else {
                                                        if ((t.tag == 27)) {
                                                          return 27;
                                                          _sv0t0 = 0;
                                                        } else {
                                                          if ((t.tag == 28)) {
                                                            return 28;
                                                            _sv0t0 = 0;
                                                          } else {
                                                            if ((t.tag == 29)) {
                                                              return 29;
                                                              _sv0t0 = 0;
                                                            } else {
                                                              if ((t.tag == 30)) {
                                                                return 30;
                                                                _sv0t0 = 0;
                                                              } else {
                                                                if ((t.tag == 31)) {
                                                                  return 31;
                                                                  _sv0t0 = 0;
                                                                } else {
                                                                  if ((t.tag == 32)) {
                                                                    return 32;
                                                                    _sv0t0 = 0;
                                                                  } else {
                                                                    if ((t.tag == 33)) {
                                                                      return 33;
                                                                      _sv0t0 = 0;
                                                                    } else {
                                                                      if ((t.tag == 34)) {
                                                                        return 34;
                                                                        _sv0t0 = 0;
                                                                      } else {
                                                                        if ((t.tag == 35)) {
                                                                          return 35;
                                                                          _sv0t0 = 0;
                                                                        } else {
                                                                          if ((t.tag == 36)) {
                                                                            return 36;
                                                                            _sv0t0 = 0;
                                                                          } else {
                                                                            if ((t.tag == 37)) {
                                                                              return 37;
                                                                              _sv0t0 = 0;
                                                                            } else {
                                                                              if ((t.tag == 38)) {
                                                                                return 38;
                                                                                _sv0t0 = 0;
                                                                              } else {
                                                                                if ((t.tag == 39)) {
                                                                                  return 39;
                                                                                  _sv0t0 = 0;
                                                                                } else {
                                                                                  if ((t.tag == 40)) {
                                                                                    return 40;
                                                                                    _sv0t0 = 0;
                                                                                  } else {
                                                                                    if ((t.tag == 41)) {
                                                                                      return 41;
                                                                                      _sv0t0 = 0;
                                                                                    } else {
                                                                                      if ((t.tag == 42)) {
                                                                                        return 42;
                                                                                        _sv0t0 = 0;
                                                                                      } else {
                                                                                        if ((t.tag == 43)) {
                                                                                          return 43;
                                                                                          _sv0t0 = 0;
                                                                                        } else {
                                                                                          if ((t.tag == 44)) {
                                                                                            return 44;
                                                                                            _sv0t0 = 0;
                                                                                          } else {
                                                                                            if ((t.tag == 45)) {
                                                                                              return 45;
                                                                                              _sv0t0 = 0;
                                                                                            } else {
                                                                                              if ((t.tag == 46)) {
                                                                                                return 46;
                                                                                                _sv0t0 = 0;
                                                                                              } else {
                                                                                                if ((t.tag == 47)) {
                                                                                                  return 47;
                                                                                                  _sv0t0 = 0;
                                                                                                } else {
                                                                                                  if ((t.tag == 48)) {
                                                                                                    return 48;
                                                                                                    _sv0t0 = 0;
                                                                                                  } else {
                                                                                                    if ((t.tag == 49)) {
                                                                                                      return 49;
                                                                                                      _sv0t0 = 0;
                                                                                                    } else {
                                                                                                      if ((t.tag == 50)) {
                                                                                                        return 50;
                                                                                                        _sv0t0 = 0;
                                                                                                      } else {
                                                                                                        if ((t.tag == 51)) {
                                                                                                          return 51;
                                                                                                          _sv0t0 = 0;
                                                                                                        } else {
                                                                                                          if ((t.tag == 52)) {
                                                                                                            return 52;
                                                                                                            _sv0t0 = 0;
                                                                                                          } else {
                                                                                                            if ((t.tag == 53)) {
                                                                                                              return 53;
                                                                                                              _sv0t0 = 0;
                                                                                                            } else {
                                                                                                              if ((t.tag == 54)) {
                                                                                                                return 54;
                                                                                                                _sv0t0 = 0;
                                                                                                              } else {
                                                                                                                if ((t.tag == 55)) {
                                                                                                                  return 55;
                                                                                                                  _sv0t0 = 0;
                                                                                                                } else {
                                                                                                                  if ((t.tag == 56)) {
                                                                                                                    return 56;
                                                                                                                    _sv0t0 = 0;
                                                                                                                  } else {
                                                                                                                    if ((t.tag == 57)) {
                                                                                                                      return 57;
                                                                                                                      _sv0t0 = 0;
                                                                                                                    } else {
                                                                                                                      if ((t.tag == 58)) {
                                                                                                                        return 58;
                                                                                                                        _sv0t0 = 0;
                                                                                                                      } else {
                                                                                                                        if ((t.tag == 59)) {
                                                                                                                          return 59;
                                                                                                                          _sv0t0 = 0;
                                                                                                                        } else {
                                                                                                                          if ((t.tag == 60)) {
                                                                                                                            return 60;
                                                                                                                            _sv0t0 = 0;
                                                                                                                          } else {
                                                                                                                            if ((t.tag == 61)) {
                                                                                                                              return 61;
                                                                                                                              _sv0t0 = 0;
                                                                                                                            } else {
                                                                                                                              if ((t.tag == 62)) {
                                                                                                                                return 62;
                                                                                                                                _sv0t0 = 0;
                                                                                                                              } else {
                                                                                                                                if ((t.tag == 63)) {
                                                                                                                                  return 63;
                                                                                                                                  _sv0t0 = 0;
                                                                                                                                } else {
                                                                                                                                  if ((t.tag == 64)) {
                                                                                                                                    return 64;
                                                                                                                                    _sv0t0 = 0;
                                                                                                                                  } else {
                                                                                                                                    if ((t.tag == 65)) {
                                                                                                                                      return 65;
                                                                                                                                      _sv0t0 = 0;
                                                                                                                                    } else {
                                                                                                                                      if ((t.tag == 66)) {
                                                                                                                                        return 66;
                                                                                                                                        _sv0t0 = 0;
                                                                                                                                      } else {
                                                                                                                                        if ((t.tag == 67)) {
                                                                                                                                          return 67;
                                                                                                                                          _sv0t0 = 0;
                                                                                                                                        } else {
                                                                                                                                          if ((t.tag == 68)) {
                                                                                                                                            return 68;
                                                                                                                                            _sv0t0 = 0;
                                                                                                                                          } else {
                                                                                                                                            if ((t.tag == 69)) {
                                                                                                                                              return 69;
                                                                                                                                              _sv0t0 = 0;
                                                                                                                                            } else {
                                                                                                                                              if ((t.tag == 70)) {
                                                                                                                                                return 70;
                                                                                                                                                _sv0t0 = 0;
                                                                                                                                              } else {
                                                                                                                                                if ((t.tag == 71)) {
                                                                                                                                                  return 71;
                                                                                                                                                  _sv0t0 = 0;
                                                                                                                                                } else {
                                                                                                                                                  if ((t.tag == 72)) {
                                                                                                                                                    return 72;
                                                                                                                                                    _sv0t0 = 0;
                                                                                                                                                  } else {
                                                                                                                                                    if ((t.tag == 73)) {
                                                                                                                                                      return 73;
                                                                                                                                                      _sv0t0 = 0;
                                                                                                                                                    } else {
                                                                                                                                                      if ((t.tag == 74)) {
                                                                                                                                                        return 74;
                                                                                                                                                        _sv0t0 = 0;
                                                                                                                                                      } else {
                                                                                                                                                        if ((t.tag == 75)) {
                                                                                                                                                          return 75;
                                                                                                                                                          _sv0t0 = 0;
                                                                                                                                                        } else {
                                                                                                                                                          if ((t.tag == 76)) {
                                                                                                                                                            return 76;
                                                                                                                                                            _sv0t0 = 0;
                                                                                                                                                          } else {
                                                                                                                                                            if ((t.tag == 77)) {
                                                                                                                                                              return 77;
                                                                                                                                                              _sv0t0 = 0;
                                                                                                                                                            } else {
                                                                                                                                                              if ((t.tag == 78)) {
                                                                                                                                                                return 78;
                                                                                                                                                                _sv0t0 = 0;
                                                                                                                                                              } else {
                                                                                                                                                                if ((t.tag == 79)) {
                                                                                                                                                                  return 79;
                                                                                                                                                                  _sv0t0 = 0;
                                                                                                                                                                } else {
                                                                                                                                                                  if ((t.tag == 80)) {
                                                                                                                                                                    return 80;
                                                                                                                                                                    _sv0t0 = 0;
                                                                                                                                                                  } else {
                                                                                                                                                                    if ((t.tag == 81)) {
                                                                                                                                                                      return 81;
                                                                                                                                                                      _sv0t0 = 0;
                                                                                                                                                                    } else {
                                                                                                                                                                      if ((t.tag == 82)) {
                                                                                                                                                                        return 82;
                                                                                                                                                                        _sv0t0 = 0;
                                                                                                                                                                      } else {
                                                                                                                                                                        if ((t.tag == 83)) {
                                                                                                                                                                          return 83;
                                                                                                                                                                          _sv0t0 = 0;
                                                                                                                                                                        } else {
                                                                                                                                                                          if ((t.tag == 84)) {
                                                                                                                                                                            return 84;
                                                                                                                                                                            _sv0t0 = 0;
                                                                                                                                                                          } else {
                                                                                                                                                                            if ((t.tag == 85)) {
                                                                                                                                                                              return 85;
                                                                                                                                                                              _sv0t0 = 0;
                                                                                                                                                                            } else {
                                                                                                                                                                              if ((t.tag == 86)) {
                                                                                                                                                                                return 86;
                                                                                                                                                                                _sv0t0 = 0;
                                                                                                                                                                              } else {
                                                                                                                                                                                if ((t.tag == 87)) {
                                                                                                                                                                                  return 87;
                                                                                                                                                                                  _sv0t0 = 0;
                                                                                                                                                                                } else {
                                                                                                                                                                                  if ((t.tag == 88)) {
                                                                                                                                                                                    return 88;
                                                                                                                                                                                    _sv0t0 = 0;
                                                                                                                                                                                  } else {
                                                                                                                                                                                    if ((t.tag == 89)) {
                                                                                                                                                                                      return 89;
                                                                                                                                                                                      _sv0t0 = 0;
                                                                                                                                                                                    } else {
                                                                                                                                                                                      if ((t.tag == 90)) {
                                                                                                                                                                                        return 90;
                                                                                                                                                                                        _sv0t0 = 0;
                                                                                                                                                                                      } else {
                                                                                                                                                                                        if ((t.tag == 91)) {
                                                                                                                                                                                          return 91;
                                                                                                                                                                                          _sv0t0 = 0;
                                                                                                                                                                                        } else {
                                                                                                                                                                                          if ((t.tag == 92)) {
                                                                                                                                                                                            return 92;
                                                                                                                                                                                            _sv0t0 = 0;
                                                                                                                                                                                          } else {
                                                                                                                                                                                            if ((t.tag == 93)) {
                                                                                                                                                                                              return 93;
                                                                                                                                                                                              _sv0t0 = 0;
                                                                                                                                                                                            } else {
                                                                                                                                                                                              if ((t.tag == 94)) {
                                                                                                                                                                                                return 94;
                                                                                                                                                                                                _sv0t0 = 0;
                                                                                                                                                                                              } else {
                                                                                                                                                                                                if ((t.tag == 95)) {
                                                                                                                                                                                                  return 95;
                                                                                                                                                                                                  _sv0t0 = 0;
                                                                                                                                                                                                } else {
                                                                                                                                                                                                  if ((t.tag == 96)) {
                                                                                                                                                                                                    return 96;
                                                                                                                                                                                                    _sv0t0 = 0;
                                                                                                                                                                                                  } else {
                                                                                                                                                                                                    if ((t.tag == 97)) {
                                                                                                                                                                                                      return 97;
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

static int test_literals(void) {
  Token ti;
  ti.tag = 0;
  ti.p0 = 42;
  int _sv0t0 = token_tag(ti);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Token ts;
  ts.tag = 2;
  ts.p0 = 100;
  int _sv0t1 = token_tag(ts);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  Token tb;
  tb.tag = 4;
  tb.p0 = 1;
  int _sv0t2 = token_tag(tb);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  Token id;
  id.tag = 5;
  id.p0 = 200;
  int _sv0t3 = token_tag(id);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_delimiters(void) {
  Token lp;
  lp.tag = 6;
  int _sv0t0 = token_tag(lp);
  if ((_sv0t0 != 6)) {
    return 1;
  } else {
  }
  Token rb;
  rb.tag = 9;
  int _sv0t1 = token_tag(rb);
  if ((_sv0t1 != 9)) {
    return 2;
  } else {
  }
  Token ar;
  ar.tag = 19;
  int _sv0t2 = token_tag(ar);
  if ((_sv0t2 != 19)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_operators(void) {
  Token pl;
  pl.tag = 22;
  int _sv0t0 = token_tag(pl);
  if ((_sv0t0 != 22)) {
    return 1;
  } else {
  }
  Token eq;
  eq.tag = 36;
  int _sv0t1 = token_tag(eq);
  if ((_sv0t1 != 36)) {
    return 2;
  } else {
  }
  Token ge;
  ge.tag = 53;
  int _sv0t2 = token_tag(ge);
  if ((_sv0t2 != 53)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_keywords(void) {
  Token kl;
  kl.tag = 71;
  int _sv0t0 = token_tag(kl);
  if ((_sv0t0 != 71)) {
    return 1;
  } else {
  }
  Token kw;
  kw.tag = 96;
  int _sv0t1 = token_tag(kw);
  if ((_sv0t1 != 96)) {
    return 2;
  } else {
  }
  Token eof;
  eof.tag = 97;
  int _sv0t2 = token_tag(eof);
  if ((_sv0t2 != 97)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_literals();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_delimiters();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_operators();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_keywords();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  return 0;
}

