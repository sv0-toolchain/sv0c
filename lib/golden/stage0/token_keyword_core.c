#include "sv0_runtime.h"

static int tag_kw_fn(void);
static int tag_kw_let(void);
static int tag_kw_if(void);
static int is_keyword_tag(int t);

static int tag_kw_fn(void) {
  return 1;
}

static int tag_kw_let(void) {
  return 2;
}

static int tag_kw_if(void) {
  return 3;
}

static int is_keyword_tag(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 1)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 2)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 3)) {
        return 1;
        _sv0t3 = 0;
      } else {
        return 0;
        _sv0t3 = 0;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = tag_kw_fn();
  int _sv0t1 = is_keyword_tag(_sv0t0);
  int ok_fn = (_sv0t1 - 1);
  int _sv0t2 = tag_kw_let();
  int _sv0t3 = is_keyword_tag(_sv0t2);
  int ok_let = (_sv0t3 - 1);
  int _sv0t4 = tag_kw_if();
  int _sv0t5 = is_keyword_tag(_sv0t4);
  int ok_if = (_sv0t5 - 1);
  int _sv0t6 = is_keyword_tag(99);
  int ok_non = _sv0t6;
  int _sv0t7 = (ok_fn + ok_let);
  int _sv0t8 = (_sv0t7 + ok_if);
  int _sv0t9 = (_sv0t8 + ok_non);
  return _sv0t9;
}

