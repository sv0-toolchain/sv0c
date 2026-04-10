#include "sv0_runtime.h"

static int variant_slots(int variant_kind, int field_count);

static int variant_slots(int variant_kind, int field_count) {
  if ((variant_kind == 0)) {
    return 0;
  } else {
  }
  if ((variant_kind == 1)) {
    if ((field_count < 0)) {
      return 255;
    } else {
    }
    return field_count;
  } else {
  }
  if ((variant_kind == 2)) {
    if ((field_count < 0)) {
      return 255;
    } else {
    }
    return field_count;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = variant_slots(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_slots(1, 3);
  if ((_sv0t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t2 = variant_slots(2, 5);
  if ((_sv0t2 != 5)) {
    return 1;
  } else {
  }
  int _sv0t3 = variant_slots(1, 0);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = variant_slots(0, 5);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = variant_slots(3, 0);
  if ((_sv0t5 != 255)) {
    return 1;
  } else {
  }
  int _sv0t6 = (-1);
  int _sv0t7 = variant_slots(1, _sv0t6);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

