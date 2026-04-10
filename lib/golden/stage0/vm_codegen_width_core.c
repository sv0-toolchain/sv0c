#include "sv0_runtime.h"

static int width_of_cty(int cty_class, int lookup_width);
static int index_of_field(int field_count, int target_index);
static int enum_field_name_count(int enum_width);
static int value_width(int val_tag, int env_width);

static int width_of_cty(int cty_class, int lookup_width) {
  if ((cty_class == 0)) {
    return 0;
  } else {
  }
  if ((cty_class == 1)) {
    return 1;
  } else {
  }
  if ((cty_class == 2)) {
    return lookup_width;
  } else {
  }
  if ((cty_class == 3)) {
    return lookup_width;
  } else {
  }
  return 1;
}

static int index_of_field(int field_count, int target_index) {
  if ((target_index >= 0)) {
    if ((target_index < field_count)) {
      return target_index;
    } else {
    }
  } else {
  }
  return 255;
}

static int enum_field_name_count(int enum_width) {
  if ((enum_width <= 0)) {
    return 1;
  } else {
  }
  return enum_width;
}

static int value_width(int val_tag, int env_width) {
  if ((val_tag == 0)) {
    return env_width;
  } else {
  }
  return 1;
}

int main(void) {
  int _sv0t0 = width_of_cty(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = width_of_cty(1, 0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = width_of_cty(2, 3);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = width_of_cty(3, 4);
  if ((_sv0t3 != 4)) {
    return 1;
  } else {
  }
  int _sv0t4 = width_of_cty(4, 0);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = index_of_field(3, 0);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = index_of_field(3, 2);
  if ((_sv0t6 != 2)) {
    return 1;
  } else {
  }
  int _sv0t7 = index_of_field(3, 3);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  int _sv0t8 = (0 - 1);
  int _sv0t9 = index_of_field(3, _sv0t8);
  if ((_sv0t9 != 255)) {
    return 1;
  } else {
  }
  int _sv0t10 = enum_field_name_count(0);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = enum_field_name_count(1);
  if ((_sv0t11 != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = enum_field_name_count(3);
  if ((_sv0t12 != 3)) {
    return 1;
  } else {
  }
  int _sv0t13 = value_width(0, 3);
  if ((_sv0t13 != 3)) {
    return 1;
  } else {
  }
  int _sv0t14 = value_width(1, 3);
  if ((_sv0t14 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

