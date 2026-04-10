#include "sv0_runtime.h"

static int emit_value_segment_count(int value_kind);
static int emit_call_args_segment_count(int arg_count, int total_value_segments);
static int c_escape_string_len(int plain_count, int escaped_count);

static int emit_value_segment_count(int value_kind) {
  if ((value_kind == 1)) {
    return 1;
  } else {
  }
  if ((value_kind == 2)) {
    return 1;
  } else {
  }
  if ((value_kind == 3)) {
    return 1;
  } else {
  }
  if ((value_kind == 4)) {
    return 1;
  } else {
  }
  if ((value_kind == 5)) {
    return 1;
  } else {
  }
  if ((value_kind == 6)) {
    return 3;
  } else {
  }
  if ((value_kind == 7)) {
    return 3;
  } else {
  }
  if ((value_kind == 8)) {
    return 4;
  } else {
  }
  if ((value_kind == 9)) {
    return 3;
  } else {
  }
  return 255;
}

static int emit_call_args_segment_count(int arg_count, int total_value_segments) {
  if ((arg_count == 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = (total_value_segments + arg_count);
  int _sv0t1 = (_sv0t0 - 1);
  return _sv0t1;
}

static int c_escape_string_len(int plain_count, int escaped_count) {
  int _sv0t0 = (escaped_count * 2);
  int _sv0t1 = (plain_count + _sv0t0);
  return _sv0t1;
}

int main(void) {
  int _sv0t0 = emit_value_segment_count(1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = emit_value_segment_count(2);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = emit_value_segment_count(3);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = emit_value_segment_count(4);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = emit_value_segment_count(5);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = emit_value_segment_count(6);
  if ((_sv0t5 != 3)) {
    return 1;
  } else {
  }
  int _sv0t6 = emit_value_segment_count(7);
  if ((_sv0t6 != 3)) {
    return 1;
  } else {
  }
  int _sv0t7 = emit_value_segment_count(8);
  if ((_sv0t7 != 4)) {
    return 1;
  } else {
  }
  int _sv0t8 = emit_value_segment_count(9);
  if ((_sv0t8 != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = emit_value_segment_count(99);
  if ((_sv0t9 != 255)) {
    return 1;
  } else {
  }
  int _sv0t10 = emit_call_args_segment_count(0, 0);
  if ((_sv0t10 != 0)) {
    return 1;
  } else {
  }
  int _sv0t11 = emit_call_args_segment_count(1, 1);
  if ((_sv0t11 != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = emit_call_args_segment_count(3, 5);
  if ((_sv0t12 != 7)) {
    return 1;
  } else {
  }
  int _sv0t13 = c_escape_string_len(5, 2);
  if ((_sv0t13 != 9)) {
    return 1;
  } else {
  }
  int _sv0t14 = c_escape_string_len(10, 0);
  if ((_sv0t14 != 10)) {
    return 1;
  } else {
  }
  return 0;
}

