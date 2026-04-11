#include "sv0_runtime.h"

static int value_width(int val_tag, int struct_width, int enum_width);
static int lookup_slot_result(int found, int slot);
static int slot_search(int name, int s0_name, int s0_slot, int s1_name, int s1_slot);
static int total_slots_for_params(int w0, int w1, int w2);
static int enc_len_for_value(int width);

static int value_width(int val_tag, int struct_width, int enum_width) {
  if ((val_tag == 0)) {
    return 1;
  } else {
  }
  if ((val_tag == 1)) {
    return 1;
  } else {
  }
  if ((val_tag == 2)) {
    return 1;
  } else {
  }
  if ((val_tag == 3)) {
    return 1;
  } else {
  }
  if ((val_tag == 4)) {
    return struct_width;
  } else {
  }
  if ((val_tag == 5)) {
    return enum_width;
  } else {
  }
  if ((val_tag == 6)) {
    return 1;
  } else {
  }
  if ((val_tag == 7)) {
    return 1;
  } else {
  }
  return 1;
}

static int lookup_slot_result(int found, int slot) {
  if (found) {
    return slot;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int slot_search(int name, int s0_name, int s0_slot, int s1_name, int s1_slot) {
  if ((name == s0_name)) {
    return s0_slot;
  } else {
  }
  if ((name == s1_name)) {
    return s1_slot;
  } else {
  }
  int _sv0t0 = (-1);
  return _sv0t0;
}

static int total_slots_for_params(int w0, int w1, int w2) {
  int _sv0t0 = (w0 + w1);
  int _sv0t1 = (_sv0t0 + w2);
  return _sv0t1;
}

static int enc_len_for_value(int width) {
  return width;
}

int main(void) {
  int _sv0t0 = value_width(0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = value_width(4, 3, 0);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = value_width(5, 0, 4);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  int _sv0t3 = value_width(6, 0, 0);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = lookup_slot_result(1, 5);
  if ((_sv0t4 != 5)) {
    return 5;
  } else {
  }
  int _sv0t5 = lookup_slot_result(0, 5);
  int _sv0t6 = (-1);
  if ((_sv0t5 != _sv0t6)) {
    return 6;
  } else {
  }
  int _sv0t7 = slot_search(10, 10, 0, 20, 3);
  if ((_sv0t7 != 0)) {
    return 7;
  } else {
  }
  int _sv0t8 = slot_search(20, 10, 0, 20, 3);
  if ((_sv0t8 != 3)) {
    return 8;
  } else {
  }
  int _sv0t9 = slot_search(30, 10, 0, 20, 3);
  int _sv0t10 = (-1);
  if ((_sv0t9 != _sv0t10)) {
    return 9;
  } else {
  }
  int _sv0t11 = total_slots_for_params(1, 3, 1);
  if ((_sv0t11 != 5)) {
    return 10;
  } else {
  }
  int _sv0t12 = enc_len_for_value(3);
  if ((_sv0t12 != 3)) {
    return 11;
  } else {
  }
  return 0;
}

