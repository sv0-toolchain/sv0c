#include "sv0_runtime.h"

static int build_enum_variants_len(int item_count, int enum_count);
static int tagl_tag_count(int variant_count);
static int one_enum_max_payload(int slot_a, int slot_b, int slot_c);

static int build_enum_variants_len(int item_count, int enum_count) {
  if ((item_count < 0)) {
    return 255;
  } else {
  }
  if ((enum_count < 0)) {
    return 255;
  } else {
  }
  if ((enum_count > item_count)) {
    return 255;
  } else {
  }
  return enum_count;
}

static int tagl_tag_count(int variant_count) {
  if ((variant_count < 0)) {
    return 255;
  } else {
  }
  return variant_count;
}

static int one_enum_max_payload(int slot_a, int slot_b, int slot_c) {
  if ((slot_a < 0)) {
    return 255;
  } else {
  }
  if ((slot_b < 0)) {
    return 255;
  } else {
  }
  if ((slot_c < 0)) {
    return 255;
  } else {
  }
  int m = slot_a;
  if ((slot_b > m)) {
    m = slot_b;
  } else {
  }
  if ((slot_c > m)) {
    m = slot_c;
  } else {
  }
  return m;
}

int main(void) {
  int _sv0t0 = build_enum_variants_len(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = build_enum_variants_len(5, 2);
  if ((_sv0t1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t2 = build_enum_variants_len(2, 3);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = tagl_tag_count(3);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = tagl_tag_count(0);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = (-1);
  int _sv0t6 = tagl_tag_count(_sv0t5);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  int _sv0t7 = one_enum_max_payload(0, 2, 1);
  if ((_sv0t7 != 2)) {
    return 1;
  } else {
  }
  int _sv0t8 = one_enum_max_payload(3, 0, 0);
  if ((_sv0t8 != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = one_enum_max_payload(0, 0, 0);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = (-1);
  int _sv0t11 = one_enum_max_payload(_sv0t10, 0, 0);
  if ((_sv0t11 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

