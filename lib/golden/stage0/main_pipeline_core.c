#include "sv0_runtime.h"

static int base_name_sv0(const char* path);
static int has_sv0_extension(const char* path);
static int cli_mode(const char* arg0);
static int pipeline_stage_count(void);
static int stage_name_tag(int stage);
static int is_valid_stage(int stage);
static int test_basename(void);
static int test_has_ext(void);
static int test_cli_mode(void);
static int test_stages(void);

static int base_name_sv0(const char* path) {
  int _sv0t0 = sv0_string_len(path);
  int n = _sv0t0;
  if ((n > 4)) {
    int _sv0t1 = (n - 4);
    const char* _sv0t2 = sv0_string_substr(path, _sv0t1, 4);
    const char* suffix;
    suffix = _sv0t2;
    int _sv0t3 = sv0_string_eq(suffix, ".sv0");
    if (_sv0t3) {
      int _sv0t4 = (n - 4);
      return _sv0t4;
    } else {
    }
  } else {
  }
  return n;
}

static int has_sv0_extension(const char* path) {
  int _sv0t0 = sv0_string_len(path);
  int n = _sv0t0;
  if ((n <= 4)) {
    return 0;
  } else {
  }
  int _sv0t1 = (n - 4);
  const char* _sv0t2 = sv0_string_substr(path, _sv0t1, 4);
  const char* suffix;
  suffix = _sv0t2;
  int _sv0t3 = sv0_string_eq(suffix, ".sv0");
  return _sv0t3;
}

static int cli_mode(const char* arg0) {
  int _sv0t0 = sv0_string_eq(arg0, "--target=vm");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(arg0, "--project");
  if (_sv0t1) {
    return 2;
  } else {
  }
  return 0;
}

static int pipeline_stage_count(void) {
  return 7;
}

static int stage_name_tag(int stage) {
  if ((stage == 0)) {
    return 10;
  } else {
  }
  if ((stage == 1)) {
    return 11;
  } else {
  }
  if ((stage == 2)) {
    return 12;
  } else {
  }
  if ((stage == 3)) {
    return 13;
  } else {
  }
  if ((stage == 4)) {
    return 14;
  } else {
  }
  if ((stage == 5)) {
    return 15;
  } else {
  }
  if ((stage == 6)) {
    return 16;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int is_valid_stage(int stage) {
  if ((stage < 0)) {
    return 0;
  } else {
  }
  if ((stage >= 7)) {
    return 0;
  } else {
  }
  return 1;
}

static int test_basename(void) {
  int _sv0t0 = base_name_sv0("test.sv0");
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = base_name_sv0("hello.sv0");
  if ((_sv0t1 != 5)) {
    return 2;
  } else {
  }
  int _sv0t2 = base_name_sv0("ab.sv0");
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = base_name_sv0(".sv0");
  if ((_sv0t3 != 4)) {
    return 4;
  } else {
  }
  int _sv0t4 = base_name_sv0("noext");
  if ((_sv0t4 != 5)) {
    return 5;
  } else {
  }
  int _sv0t5 = base_name_sv0("a.c");
  if ((_sv0t5 != 3)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_has_ext(void) {
  int _sv0t0 = has_sv0_extension("test.sv0");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = has_sv0_extension("noext");
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = has_sv0_extension(".sv0");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = has_sv0_extension("ab.sv0");
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_cli_mode(void) {
  int _sv0t0 = cli_mode("--target=vm");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = cli_mode("--project");
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = cli_mode("hello.sv0");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_stages(void) {
  int _sv0t0 = pipeline_stage_count();
  if ((_sv0t0 != 7)) {
    return 1;
  } else {
  }
  int _sv0t1 = stage_name_tag(0);
  if ((_sv0t1 != 10)) {
    return 2;
  } else {
  }
  int _sv0t2 = stage_name_tag(6);
  if ((_sv0t2 != 16)) {
    return 3;
  } else {
  }
  int _sv0t3 = stage_name_tag(7);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 4;
  } else {
  }
  int _sv0t5 = is_valid_stage(0);
  if ((_sv0t5 != 1)) {
    return 5;
  } else {
  }
  int _sv0t6 = is_valid_stage(6);
  if ((_sv0t6 != 1)) {
    return 6;
  } else {
  }
  int _sv0t7 = is_valid_stage(7);
  if ((_sv0t7 != 0)) {
    return 7;
  } else {
  }
  int _sv0t8 = (0 - 1);
  int _sv0t9 = is_valid_stage(_sv0t8);
  if ((_sv0t9 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_basename();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_has_ext();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_cli_mode();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_stages();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

