#include "sv0_runtime.h"

typedef struct {
  int tag;
  int p0;
} MainPhaseResult;

static int DRIVER_TOKENIZE_ERR_EMPTY(void);
static int DRIVER_TOKENIZE_ERR_BAD_EXT(void);
static const char* compile_read_source(const char* path);
static void compile_write_text(const char* path, const char* text);
static int find_last_slash(const char* path);
static int find_last_dot(const char* s, int start);
static int path_has_sv0_ext(const char* path);
static int sv0_ext_len(void);
static int base_name_stem_end(const char* path);
static const char* base_name_sv0(const char* path);
static const char* vm_build_dir(void);
static const char* sv0b_ext(void);
static const char* vm_entry_stem_main(void);
static const char* vm_entry_stem_program(void);
static const char* has_main_sv0(int has_main_file);
static const char* vm_output_sv0b_path(const char* stem);
static const char* vm_output_sv0b_for_entry(int has_main_file);
static const char* vm_output_sv0b_for_input_path(const char* path);
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
static MainPhaseResult driver_tokenize_sketch(const char* source_path);
static int driver_result_code(MainPhaseResult r);
static int test_path_helpers(void);
static int test_vm_output(void);
static int test_phases(void);
static int test_cli(void);
static int test_error_prefix(void);
static int test_driver_tokenize_sketch(void);
static int test_base_name_sv0(void);

static int DRIVER_TOKENIZE_ERR_EMPTY(void) {
  return 1;
}

static int DRIVER_TOKENIZE_ERR_BAD_EXT(void) {
  return 2;
}

static const char* compile_read_source(const char* path) {
  const char* _sv0t0 = sv0_read_file(path);
  return _sv0t0;
}

static void compile_write_text(const char* path, const char* text) {
  sv0_write_file(path, text);
}

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

static const char* base_name_sv0(const char* path) {
  int _sv0t0 = sv0_string_len(path);
  int len = _sv0t0;
  int _sv0t1 = find_last_slash(path);
  int slash = _sv0t1;
  int start = (slash + 1);
  int flen = (len - start);
  const char* _sv0t2 = sv0_string_substr(path, start, flen);
  const char* fname;
  fname = _sv0t2;
  int _sv0t3 = sv0_string_len(fname);
  int fnlen = _sv0t3;
  if ((fnlen > 4)) {
    int dot = (fnlen - 4);
    int _sv0t4 = sv0_string_char_at(fname, dot);
    if ((_sv0t4 == 46)) {
      int _sv0t5 = (dot + 1);
      int _sv0t6 = sv0_string_char_at(fname, _sv0t5);
      if ((_sv0t6 == 115)) {
        int _sv0t7 = (dot + 2);
        int _sv0t8 = sv0_string_char_at(fname, _sv0t7);
        if ((_sv0t8 == 118)) {
          int _sv0t9 = (dot + 3);
          int _sv0t10 = sv0_string_char_at(fname, _sv0t9);
          if ((_sv0t10 == 48)) {
            const char* _sv0t11 = sv0_string_substr(fname, 0, dot);
            return _sv0t11;
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
  } else {
  }
  return fname;
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

static const char* vm_output_sv0b_path(const char* stem) {
  const char* _sv0t0 = vm_build_dir();
  const char* out;
  out = _sv0t0;
  const char* _sv0t1 = sv0_string_concat(out, stem);
  out = _sv0t1;
  const char* _sv0t2 = sv0b_ext();
  const char* _sv0t3 = sv0_string_concat(out, _sv0t2);
  out = _sv0t3;
  return out;
}

static const char* vm_output_sv0b_for_entry(int has_main_file) {
  const char* _sv0t0 = has_main_sv0(has_main_file);
  const char* _sv0t1 = vm_output_sv0b_path(_sv0t0);
  return _sv0t1;
}

static const char* vm_output_sv0b_for_input_path(const char* path) {
  const char* _sv0t0 = base_name_sv0(path);
  const char* _sv0t1 = vm_output_sv0b_path(_sv0t0);
  return _sv0t1;
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

static MainPhaseResult driver_tokenize_sketch(const char* source_path) {
  int _sv0t0 = sv0_string_len(source_path);
  if ((_sv0t0 == 0)) {
    MainPhaseResult _sv0t1;
    int _sv0t2 = DRIVER_TOKENIZE_ERR_EMPTY();
    _sv0t1.tag = 1;
    _sv0t1.p0 = _sv0t2;
    return _sv0t1;
  } else {
  }
  int _sv0t3 = path_has_sv0_ext(source_path);
  if ((_sv0t3 != 1)) {
    MainPhaseResult _sv0t4;
    int _sv0t5 = DRIVER_TOKENIZE_ERR_BAD_EXT();
    _sv0t4.tag = 1;
    _sv0t4.p0 = _sv0t5;
    return _sv0t4;
  } else {
  }
  const char* _sv0t6 = sv0_read_file(source_path);
  const char* src;
  src = _sv0t6;
  MainPhaseResult _sv0t7;
  int _sv0t8 = sv0_string_len(src);
  _sv0t7.tag = 0;
  _sv0t7.p0 = _sv0t8;
  return _sv0t7;
}

static int driver_result_code(MainPhaseResult r) {
  int _sv0t0;
  if ((r.tag == 0)) {
    int v = r.p0;
    _sv0t0 = v;
  } else {
    if ((r.tag == 1)) {
      int e = r.p0;
      _sv0t0 = (0 - e);
    } else {
    }
  }
  return _sv0t0;
}

static int test_path_helpers(void) {
  int _sv0t0 = find_last_slash("foo/bar.sv0");
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = find_last_slash("x\\y.sv0");
  if ((_sv0t1 != 1)) {
    return 12;
  } else {
  }
  int _sv0t2 = find_last_slash("bar.sv0");
  int _sv0t3 = (0 - 1);
  if ((_sv0t2 != _sv0t3)) {
    return 2;
  } else {
  }
  int _sv0t4 = find_last_slash("a/b/c.sv0");
  if ((_sv0t4 != 3)) {
    return 3;
  } else {
  }
  int _sv0t5 = find_last_dot("bar.sv0", 0);
  if ((_sv0t5 != 3)) {
    return 4;
  } else {
  }
  int _sv0t6 = find_last_dot("noext", 0);
  int _sv0t7 = (0 - 1);
  if ((_sv0t6 != _sv0t7)) {
    return 5;
  } else {
  }
  int _sv0t8 = path_has_sv0_ext("test.sv0");
  if ((_sv0t8 != 1)) {
    return 6;
  } else {
  }
  int _sv0t9 = path_has_sv0_ext("test.c");
  if ((_sv0t9 != 0)) {
    return 7;
  } else {
  }
  int _sv0t10 = path_has_sv0_ext("a.sv0");
  if ((_sv0t10 != 1)) {
    return 8;
  } else {
  }
  int _sv0t11 = path_has_sv0_ext(".sv0");
  if ((_sv0t11 != 0)) {
    return 9;
  } else {
  }
  int _sv0t12 = base_name_stem_end("test.sv0");
  if ((_sv0t12 != 4)) {
    return 10;
  } else {
  }
  int _sv0t13 = base_name_stem_end("test.c");
  if ((_sv0t13 != 6)) {
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
  const char* _sv0t8 = vm_output_sv0b_path("foo");
  int _sv0t9 = sv0_string_eq(_sv0t8, "build/vm/foo.sv0b");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t10 = vm_output_sv0b_for_entry(1);
  int _sv0t11 = sv0_string_eq(_sv0t10, "build/vm/main.sv0b");
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  const char* _sv0t12 = vm_output_sv0b_for_entry(0);
  int _sv0t13 = sv0_string_eq(_sv0t12, "build/vm/program.sv0b");
  if ((_sv0t13 != 1)) {
    return 7;
  } else {
  }
  const char* _sv0t14 = vm_output_sv0b_for_input_path("src/app.sv0");
  int _sv0t15 = sv0_string_eq(_sv0t14, "build/vm/app.sv0b");
  if ((_sv0t15 != 1)) {
    return 8;
  } else {
  }
  const char* _sv0t16 = vm_output_sv0b_for_input_path("tool");
  int _sv0t17 = sv0_string_eq(_sv0t16, "build/vm/tool.sv0b");
  if ((_sv0t17 != 1)) {
    return 9;
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

static int test_driver_tokenize_sketch(void) {
  const char* probe;
  probe = "/tmp/sv0_g2_main_tokenize.sv0";
  sv0_write_file(probe, "//\n");
  MainPhaseResult _sv0t0 = driver_tokenize_sketch(probe);
  MainPhaseResult ok1;
  ok1 = _sv0t0;
  int _sv0t1 = driver_result_code(ok1);
  int v1 = _sv0t1;
  if ((v1 != 3)) {
    return 1;
  } else {
  }
  MainPhaseResult _sv0t2 = driver_tokenize_sketch("");
  MainPhaseResult bad_empty;
  bad_empty = _sv0t2;
  int _sv0t3 = driver_result_code(bad_empty);
  int c2 = _sv0t3;
  int _sv0t4 = DRIVER_TOKENIZE_ERR_EMPTY();
  int _sv0t5 = (0 - _sv0t4);
  if ((c2 != _sv0t5)) {
    return 2;
  } else {
  }
  MainPhaseResult _sv0t6 = driver_tokenize_sketch("x.c");
  MainPhaseResult bad_ext;
  bad_ext = _sv0t6;
  int _sv0t7 = driver_result_code(bad_ext);
  int c3 = _sv0t7;
  int _sv0t8 = DRIVER_TOKENIZE_ERR_BAD_EXT();
  int _sv0t9 = (0 - _sv0t8);
  if ((c3 != _sv0t9)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_base_name_sv0(void) {
  const char* _sv0t0 = base_name_sv0("foo/bar.sv0");
  int _sv0t1 = sv0_string_eq(_sv0t0, "bar");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = base_name_sv0("pkg\\app.sv0");
  int _sv0t3 = sv0_string_eq(_sv0t2, "app");
  if ((_sv0t3 != 1)) {
    return 7;
  } else {
  }
  const char* _sv0t4 = base_name_sv0("bar.sv0");
  int _sv0t5 = sv0_string_eq(_sv0t4, "bar");
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t6 = base_name_sv0("a/b/main.sv0");
  int _sv0t7 = sv0_string_eq(_sv0t6, "main");
  if ((_sv0t7 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t8 = base_name_sv0("test.c");
  int _sv0t9 = sv0_string_eq(_sv0t8, "test.c");
  if ((_sv0t9 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t10 = base_name_sv0("noext");
  int _sv0t11 = sv0_string_eq(_sv0t10, "noext");
  if ((_sv0t11 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t12 = base_name_sv0("/abs/path/lib.sv0");
  int _sv0t13 = sv0_string_eq(_sv0t12, "lib");
  if ((_sv0t13 != 1)) {
    return 6;
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
  int _sv0t9 = test_driver_tokenize_sketch();
  int r5b = _sv0t9;
  if ((r5b != 0)) {
    int _sv0t10 = (55 + r5b);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_base_name_sv0();
  int r6 = _sv0t11;
  if ((r6 != 0)) {
    int _sv0t12 = (60 + r6);
    return _sv0t12;
  } else {
  }
  return 0;
}

