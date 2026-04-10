#include "sv0_runtime.h"

static int emit_struct_td_section_count(int field_count);

static int emit_struct_td_section_count(int field_count) {
  if ((field_count < 0)) {
    return 255;
  } else {
  }
  int _sv0t0 = (field_count + 2);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = emit_struct_td_section_count(0);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = emit_struct_td_section_count(1);
  if ((_sv0t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t2 = emit_struct_td_section_count(5);
  if ((_sv0t2 != 7)) {
    return 1;
  } else {
  }
  int _sv0t3 = (-1);
  int _sv0t4 = emit_struct_td_section_count(_sv0t3);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

