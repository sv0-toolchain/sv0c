#include "sv0_runtime.h"

static int find_last_slash(const char* path);
static int find_last_dot(const char* s, int start);
static int path_has_sv0_ext(const char* path);
static int sv0_ext_len(void);
static int base_name_stem_end(const char* path);
static const char* vm_build_dir(void);
static const char* sv0b_ext(void);
static const char* vm_entry_stem_main(void);
static const char* vm_entry_stem_program(void);
static const char* has_main_sv0(int has_main_file);
static int PHASE_RESOLVE(void);
static int PHASE_CHECK(void);
static int PHASE_ANALYZE(void);
static int PHASE_STRIP(void);
static int PHASE_LOWER(void);
static int PHASE_EMIT_C(void);
static int PHASE_EMIT_VM(void);
static int PHASE_COUNT(void);
static const char* phase_name(int phase);
static int is_backend_phase(int phase);
static int CLI_FILE(void);
static int CLI_VM_FILE(void);
static int CLI_PROJECT(void);
static int CLI_VM_PROJECT(void);
static int CLI_USAGE(void);
static int is_target_vm_flag(const char* arg);
static int is_project_flag(const char* arg);
static int classify_cli(int argc, int has_vm, int has_project);
static const char* error_prefix(void);
static const char* error_cannot_open(void);
static int test_path_helpers(void);
static int test_vm_output(void);
static int test_phases(void);
static int test_cli(void);
static int test_error_prefix(void);

static int find_last_slash(const char* path) {
  int _sv0t0 = sv0_string_len(path);
  int len = _sv0t0;
  int i = (len - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_string_char_at(path, i);
    int c = _sv0t1;
    if ((c == 47)) {
      return i;
    } else {
    }
    if ((c == 92)) {
      return i;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int find_last_dot(const char* s, int start) {
  int _sv0t0 = sv0_string_len(s);
  int len = _sv0t0;
  int i = (len - 1);
  while ((i >= start)) {
    int _sv0t1 = sv0_string_char_at(s, i);
    if ((_sv0t1 == 46)) {
      return i;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int path_has_sv0_ext(const char* path) {
  int _sv0t0 = sv0_string_len(path);
  int len = _sv0t0;
  if ((len < 5)) {
    return 0;
  } else {
  }
  int dot = (len - 4);
  int _sv0t1 = sv0_string_char_at(path, dot);
  if ((_sv0t1 != 46)) {
    return 0;
  } else {
  }
  int _sv0t2 = (dot + 1);
  int _sv0t3 = sv0_string_char_at(path, _sv0t2);
  if ((_sv0t3 != 115)) {
    return 0;
  } else {
  }
  int _sv0t4 = (dot + 2);
  int _sv0t5 = sv0_string_char_at(path, _sv0t4);
  if ((_sv0t5 != 118)) {
    return 0;
  } else {
  }
  int _sv0t6 = (dot + 3);
  int _sv0t7 = sv0_string_char_at(path, _sv0t6);
  if ((_sv0t7 != 48)) {
    return 0;
  } else {
  }
  return 1;
}

static int sv0_ext_len(void) {
  return 4;
}

static int base_name_stem_end(const char* path) {
  int _sv0t0 = sv0_string_len(path);
  int len = _sv0t0;
  int _sv0t1 = path_has_sv0_ext(path);
  if (_sv0t1) {
    int _sv0t2 = (len - 4);
    return _sv0t2;
  } else {
  }
  return len;
}

static const char* vm_build_dir(void) {
  return "build/vm/";
}

static const char* sv0b_ext(void) {
  return ".sv0b";
}

static const char* vm_entry_stem_main(void) {
  return "main";
}

static const char* vm_entry_stem_program(void) {
  return "program";
}

static const char* has_main_sv0(int has_main_file) {
  if (has_main_file) {
    return "main";
  } else {
  }
  return "program";
}

static int PHASE_RESOLVE(void) {
  return 0;
}

static int PHASE_CHECK(void) {
  return 1;
}

static int PHASE_ANALYZE(void) {
  return 2;
}

static int PHASE_STRIP(void) {
  return 3;
}

static int PHASE_LOWER(void) {
  return 4;
}

static int PHASE_EMIT_C(void) {
  return 5;
}

static int PHASE_EMIT_VM(void) {
  return 6;
}

static int PHASE_COUNT(void) {
  return 7;
}

static const char* phase_name(int phase) {
  if ((phase == 0)) {
    return "resolve";
  } else {
  }
  if ((phase == 1)) {
    return "check";
  } else {
  }
  if ((phase == 2)) {
    return "analyze";
  } else {
  }
  if ((phase == 3)) {
    return "strip";
  } else {
  }
  if ((phase == 4)) {
    return "lower";
  } else {
  }
  if ((phase == 5)) {
    return "emit-c";
  } else {
  }
  if ((phase == 6)) {
    return "emit-vm";
  } else {
  }
  return "unknown";
}

static int is_backend_phase(int phase) {
  if ((phase == 5)) {
    return 1;
  } else {
  }
  if ((phase == 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int CLI_FILE(void) {
  return 0;
}

static int CLI_VM_FILE(void) {
  return 1;
}

static int CLI_PROJECT(void) {
  return 2;
}

static int CLI_VM_PROJECT(void) {
  return 3;
}

static int CLI_USAGE(void) {
  return 4;
}

static int is_target_vm_flag(const char* arg) {
  int _sv0t0 = sv0_string_eq(arg, "--target=vm");
  return _sv0t0;
}

static int is_project_flag(const char* arg) {
  int _sv0t0 = sv0_string_eq(arg, "--project");
  return _sv0t0;
}

static int classify_cli(int argc, int has_vm, int has_project) {
  if ((argc == 1)) {
    if ((has_vm == 0)) {
      if ((has_project == 0)) {
        return 0;
      } else {
      }
    } else {
    }
  } else {
  }
  if ((argc == 2)) {
    if (has_vm) {
      if ((has_project == 0)) {
        return 1;
      } else {
      }
    } else {
    }
    if (has_project) {
      if ((has_vm == 0)) {
        return 2;
      } else {
      }
    } else {
    }
  } else {
  }
  if ((argc == 3)) {
    if (has_vm) {
      if (has_project) {
        return 3;
      } else {
      }
    } else {
    }
  } else {
  }
  return 4;
}

static const char* error_prefix(void) {
  return "sv0c error: ";
}

static const char* error_cannot_open(void) {
  return "sv0c error: cannot open ";
}

static int test_path_helpers(void) {
  int _sv0t0 = find_last_slash("foo/bar.sv0");
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = find_last_slash("bar.sv0");
  int _sv0t2 = (0 - 1);
  if ((_sv0t1 != _sv0t2)) {
    return 2;
  } else {
  }
  int _sv0t3 = find_last_slash("a/b/c.sv0");
  if ((_sv0t3 != 3)) {
    return 3;
  } else {
  }
  int _sv0t4 = find_last_dot("bar.sv0", 0);
  if ((_sv0t4 != 3)) {
    return 4;
  } else {
  }
  int _sv0t5 = find_last_dot("noext", 0);
  int _sv0t6 = (0 - 1);
  if ((_sv0t5 != _sv0t6)) {
    return 5;
  } else {
  }
  int _sv0t7 = path_has_sv0_ext("test.sv0");
  if ((_sv0t7 != 1)) {
    return 6;
  } else {
  }
  int _sv0t8 = path_has_sv0_ext("test.c");
  if ((_sv0t8 != 0)) {
    return 7;
  } else {
  }
  int _sv0t9 = path_has_sv0_ext("a.sv0");
  if ((_sv0t9 != 1)) {
    return 8;
  } else {
  }
  int _sv0t10 = path_has_sv0_ext(".sv0");
  if ((_sv0t10 != 0)) {
    return 9;
  } else {
  }
  int _sv0t11 = base_name_stem_end("test.sv0");
  if ((_sv0t11 != 4)) {
    return 10;
  } else {
  }
  int _sv0t12 = base_name_stem_end("test.c");
  if ((_sv0t12 != 6)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_vm_output(void) {
  const char* _sv0t0 = vm_build_dir();
  int _sv0t1 = sv0_string_eq(_sv0t0, "build/vm/");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = sv0b_ext();
  int _sv0t3 = sv0_string_eq(_sv0t2, ".sv0b");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = has_main_sv0(1);
  int _sv0t5 = sv0_string_eq(_sv0t4, "main");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = has_main_sv0(0);
  int _sv0t7 = sv0_string_eq(_sv0t6, "program");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_phases(void) {
  int _sv0t0 = PHASE_RESOLVE();
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = PHASE_EMIT_C();
  if ((_sv0t1 != 5)) {
    return 2;
  } else {
  }
  int _sv0t2 = PHASE_EMIT_VM();
  if ((_sv0t2 != 6)) {
    return 3;
  } else {
  }
  int _sv0t3 = PHASE_COUNT();
  if ((_sv0t3 != 7)) {
    return 4;
  } else {
  }
  const char* _sv0t4 = phase_name(0);
  int _sv0t5 = sv0_string_eq(_sv0t4, "resolve");
  if ((_sv0t5 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t6 = phase_name(5);
  int _sv0t7 = sv0_string_eq(_sv0t6, "emit-c");
  if ((_sv0t7 != 1)) {
    return 6;
  } else {
  }
  int _sv0t8 = is_backend_phase(4);
  if ((_sv0t8 != 0)) {
    return 7;
  } else {
  }
  int _sv0t9 = is_backend_phase(5);
  if ((_sv0t9 != 1)) {
    return 8;
  } else {
  }
  int _sv0t10 = is_backend_phase(6);
  if ((_sv0t10 != 1)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_cli(void) {
  int _sv0t0 = is_target_vm_flag("--target=vm");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_target_vm_flag("--project");
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_project_flag("--project");
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = classify_cli(1, 0, 0);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = classify_cli(2, 1, 0);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = classify_cli(2, 0, 1);
  if ((_sv0t5 != 2)) {
    return 6;
  } else {
  }
  int _sv0t6 = classify_cli(3, 1, 1);
  if ((_sv0t6 != 3)) {
    return 7;
  } else {
  }
  int _sv0t7 = classify_cli(0, 0, 0);
  if ((_sv0t7 != 4)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_error_prefix(void) {
  const char* _sv0t0 = error_prefix();
  int _sv0t1 = sv0_string_eq(_sv0t0, "sv0c error: ");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = error_cannot_open();
  int _sv0t3 = sv0_string_eq(_sv0t2, "sv0c error: cannot open ");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_path_helpers();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_vm_output();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (20 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_phases();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (30 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_cli();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (40 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_error_prefix();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (50 + r5);
    return _sv0t8;
  } else {
  }
  return 0;
}

