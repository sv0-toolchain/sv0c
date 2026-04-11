#include "sv0_runtime.h"

static int param_list_len(int count);
static int param_list_is_void(int count);
static int fn_proto_parts(int is_static, int has_params);
static int split_main_is_main(const char* label);
static int count_static_fns(int f0_main, int f1_main, int f2_main);
static int emit_section_count(int has_typedefs, int static_count, int has_main);
static int test_param_list(void);
static int test_fn_proto_parts(void);
static int test_split_main(void);
static int test_static_count(void);
static int test_emit_sections(void);

static int param_list_len(int count) {
  if ((count == 0)) {
    return 4;
  } else {
  }
  return count;
}

static int param_list_is_void(int count) {
  int _sv0t0 = (count == 0);
  return _sv0t0;
}

static int fn_proto_parts(int is_static, int has_params) {
  int parts = 3;
  if (is_static) {
    parts = (parts + 1);
  } else {
  }
  if (has_params) {
    parts = (parts + 1);
  } else {
  }
  return parts;
}

static int split_main_is_main(const char* label) {
  int _sv0t0 = sv0_string_eq(label, "main");
  return _sv0t0;
}

static int count_static_fns(int f0_main, int f1_main, int f2_main) {
  int count = 0;
  if ((f0_main == 0)) {
    count = (count + 1);
  } else {
  }
  if ((f1_main == 0)) {
    count = (count + 1);
  } else {
  }
  if ((f2_main == 0)) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int emit_section_count(int has_typedefs, int static_count, int has_main) {
  int sections = 1;
  if (has_typedefs) {
    sections = (sections + 1);
  } else {
  }
  if ((static_count > 0)) {
    sections = (sections + 2);
  } else {
  }
  if (has_main) {
    sections = (sections + 1);
  } else {
  }
  return sections;
}

static int test_param_list(void) {
  int _sv0t0 = param_list_len(0);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = param_list_len(3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = param_list_is_void(0);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = param_list_is_void(1);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_fn_proto_parts(void) {
  int _sv0t0 = fn_proto_parts(0, 0);
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = fn_proto_parts(1, 0);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = fn_proto_parts(0, 1);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  int _sv0t3 = fn_proto_parts(1, 1);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_split_main(void) {
  int _sv0t0 = split_main_is_main("main");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = split_main_is_main("helper");
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = split_main_is_main("test_fn");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_static_count(void) {
  int _sv0t0 = count_static_fns(1, 0, 0);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = count_static_fns(0, 0, 0);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = count_static_fns(1, 1, 1);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_emit_sections(void) {
  int _sv0t0 = emit_section_count(0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = emit_section_count(1, 0, 0);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = emit_section_count(0, 2, 0);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = emit_section_count(1, 2, 1);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_param_list();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_fn_proto_parts();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_split_main();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_static_count();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_emit_sections();
  int r5 = _sv0t8;
  if ((r5 != 0)) {
    int _sv0t9 = (500 + r5);
    return _sv0t9;
  } else {
  }
  return 0;
}

