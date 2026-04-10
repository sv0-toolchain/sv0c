#include "sv0_runtime.h"

static int emit_enum_td_section_count(int max_payload);

static int emit_enum_td_section_count(int max_payload) {
  if ((max_payload < 0)) {
    return 255;
  } else {
  }
  if ((max_payload == 0)) {
    return 2;
  } else {
  }
  int _sv0t0 = (2 + max_payload);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = emit_enum_td_section_count(0);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = emit_enum_td_section_count(1);
  if ((_sv0t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t2 = emit_enum_td_section_count(3);
  if ((_sv0t2 != 5)) {
    return 1;
  } else {
  }
  int _sv0t3 = (-1);
  int _sv0t4 = emit_enum_td_section_count(_sv0t3);
  if ((_sv0t4 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

