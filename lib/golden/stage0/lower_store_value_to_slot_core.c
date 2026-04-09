#include "sv0_runtime.h"

static int store_value_to_slot_instr_count(int value_kind, int same_slot);

static int store_value_to_slot_instr_count(int value_kind, int same_slot) {
  if ((value_kind == 1)) {
    if ((same_slot == 1)) {
      return 0;
    } else {
    }
    if ((same_slot == 0)) {
      return 1;
    } else {
    }
    return 255;
  } else {
  }
  if ((value_kind == 2)) {
    return 1;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = store_value_to_slot_instr_count(1, 1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = store_value_to_slot_instr_count(1, 0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = store_value_to_slot_instr_count(2, 0);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = store_value_to_slot_instr_count(2, 1);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = store_value_to_slot_instr_count(1, 2);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  int _sv0t5 = store_value_to_slot_instr_count(0, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = store_value_to_slot_instr_count(3, 0);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

