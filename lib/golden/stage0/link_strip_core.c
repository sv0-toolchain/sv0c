#include "sv0_runtime.h"

static int item_tag_fn(void);
static int item_tag_use(void);
static int item_tag_module(void);
static int item_tag_struct(void);
static int strip_link_keep_item(int tag);

static int item_tag_fn(void) {
  return 1;
}

static int item_tag_use(void) {
  return 2;
}

static int item_tag_module(void) {
  return 3;
}

static int item_tag_struct(void) {
  return 4;
}

static int strip_link_keep_item(int tag) {
  int _sv0t0;
  int _sv0t1;
  if ((tag == 2)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((tag == 3)) {
      return 0;
      _sv0t2 = 0;
    } else {
      return 1;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = item_tag_fn();
  int _sv0t1 = strip_link_keep_item(_sv0t0);
  int a_fn = (_sv0t1 - 1);
  int _sv0t2 = item_tag_use();
  int _sv0t3 = strip_link_keep_item(_sv0t2);
  int a_use = _sv0t3;
  int _sv0t4 = item_tag_module();
  int _sv0t5 = strip_link_keep_item(_sv0t4);
  int a_mod = _sv0t5;
  int _sv0t6 = item_tag_struct();
  int _sv0t7 = strip_link_keep_item(_sv0t6);
  int a_struct = (_sv0t7 - 1);
  int _sv0t8 = (a_fn + a_use);
  int _sv0t9 = (_sv0t8 + a_mod);
  int _sv0t10 = (_sv0t9 + a_struct);
  return _sv0t10;
}

