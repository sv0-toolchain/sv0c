#include "sv0_runtime.h"

static int block_has_decls(int decl_count);
static int block_has_body(int stmt_count);
static int block_total_lines(int decl_count, int stmt_count, int has_return);
static int fn_defn_parts(int is_static, int has_params, int has_body);
static int emit_indent_level(int nesting);
static int needs_semicolon(int instr_tag);

static int block_has_decls(int decl_count) {
  int _sv0t0 = (decl_count > 0);
  return _sv0t0;
}

static int block_has_body(int stmt_count) {
  int _sv0t0 = (stmt_count > 0);
  return _sv0t0;
}

static int block_total_lines(int decl_count, int stmt_count, int has_return) {
  int lines = 2;
  lines = (lines + decl_count);
  lines = (lines + stmt_count);
  if (has_return) {
    lines = (lines + 1);
  } else {
  }
  return lines;
}

static int fn_defn_parts(int is_static, int has_params, int has_body) {
  int parts = 2;
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

static int emit_indent_level(int nesting) {
  int _sv0t0 = (nesting * 2);
  return _sv0t0;
}

static int needs_semicolon(int instr_tag) {
  if ((instr_tag == 0)) {
    return 1;
  } else {
  }
  if ((instr_tag == 1)) {
    return 1;
  } else {
  }
  if ((instr_tag == 2)) {
    return 1;
  } else {
  }
  if ((instr_tag == 3)) {
    return 0;
  } else {
  }
  if ((instr_tag == 4)) {
    return 0;
  } else {
  }
  if ((instr_tag == 5)) {
    return 1;
  } else {
  }
  return 1;
}

int main(void) {
  int _sv0t0 = block_has_decls(1);
  if ((!_sv0t0)) {
    return 1;
  } else {
  }
  int _sv0t1 = block_has_decls(0);
  if (_sv0t1) {
    return 2;
  } else {
  }
  int _sv0t2 = block_has_body(1);
  if ((!_sv0t2)) {
    return 3;
  } else {
  }
  int _sv0t3 = block_has_body(0);
  if (_sv0t3) {
    return 4;
  } else {
  }
  int _sv0t4 = block_total_lines(2, 3, 1);
  if ((_sv0t4 != 8)) {
    return 5;
  } else {
  }
  int _sv0t5 = block_total_lines(0, 0, 0);
  if ((_sv0t5 != 2)) {
    return 6;
  } else {
  }
  int _sv0t6 = block_total_lines(1, 1, 1);
  if ((_sv0t6 != 5)) {
    return 7;
  } else {
  }
  int _sv0t7 = fn_defn_parts(1, 1, 1);
  if ((_sv0t7 != 4)) {
    return 8;
  } else {
  }
  int _sv0t8 = fn_defn_parts(0, 0, 1);
  if ((_sv0t8 != 2)) {
    return 9;
  } else {
  }
  int _sv0t9 = emit_indent_level(0);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = emit_indent_level(1);
  if ((_sv0t10 != 2)) {
    return 11;
  } else {
  }
  int _sv0t11 = emit_indent_level(3);
  if ((_sv0t11 != 6)) {
    return 12;
  } else {
  }
  int _sv0t12 = needs_semicolon(0);
  if ((!_sv0t12)) {
    return 13;
  } else {
  }
  int _sv0t13 = needs_semicolon(3);
  if (_sv0t13) {
    return 14;
  } else {
  }
  int _sv0t14 = needs_semicolon(5);
  if ((!_sv0t14)) {
    return 15;
  } else {
  }
  return 0;
}

