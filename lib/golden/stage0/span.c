#include "sv0_runtime.h"

typedef struct {
  int line;
  int col;
  int offset;
} Pos;
typedef struct {
  const char* file;
  int start_line;
  int start_col;
  int start_offset;
  int stop_line;
  int stop_col;
  int stop_offset;
} Span;
typedef struct {
  const char* message;
  Span span;
} CompileError;

static CompileError compile_error_new(const char* msg, Span sp);
static const char* compile_error_message(CompileError e);
static Span compile_error_span(CompileError e);
static Pos pos_new(int l, int c, int off);
static Span span_new(const char* f, Pos s, Pos e);
static Span span_bogus(void);
static int pos_line(Pos p);
static int pos_col(Pos p);
static int pos_offset(Pos p);
static const char* span_file(Span sp);
static Pos span_start(Span sp);
static Pos span_stop(Span sp);
static int span_length(Span sp);
static Span span_merge(Span a, Span b);
static const char* span_digit_char(int d);
static const char* span_int_to_str(int n);
static const char* pos_to_string(Pos p);
static const char* span_to_string(Span sp);
static int test_basics(void);
static int test_merge(void);
static int test_pos_to_string(void);
static int test_span_to_string(void);
static int test_int_to_str(void);
static int test_int_to_str_negative(void);
static int test_compile_error_roundtrip(void);
static int test_digit_char_all(void);
static int test_int_to_str_boundary(void);
static int test_span_length_variants(void);
static int test_merge_keeps_start(void);
static int test_pos_accessors(void);
static int test_span_file_accessor(void);
static int test_span_to_string_single(void);

static CompileError compile_error_new(const char* msg, Span sp) {
  CompileError _sv0t0;
  _sv0t0.message = msg;
  _sv0t0.span = sp;
  return _sv0t0;
}

static const char* compile_error_message(CompileError e) {
  return e.message;
}

static Span compile_error_span(CompileError e) {
  return e.span;
}

static Pos pos_new(int l, int c, int off) {
  Pos _sv0t0;
  _sv0t0.line = l;
  _sv0t0.col = c;
  _sv0t0.offset = off;
  return _sv0t0;
}

static Span span_new(const char* f, Pos s, Pos e) {
  Span _sv0t0;
  _sv0t0.file = f;
  _sv0t0.start_line = s.line;
  _sv0t0.start_col = s.col;
  _sv0t0.start_offset = s.offset;
  _sv0t0.stop_line = e.line;
  _sv0t0.stop_col = e.col;
  _sv0t0.stop_offset = e.offset;
  return _sv0t0;
}

static Span span_bogus(void) {
  Pos _sv0t0 = pos_new(0, 0, 0);
  Pos z;
  z = _sv0t0;
  Span _sv0t1 = span_new("", z, z);
  return _sv0t1;
}

static int pos_line(Pos p) {
  return p.line;
}

static int pos_col(Pos p) {
  return p.col;
}

static int pos_offset(Pos p) {
  return p.offset;
}

static const char* span_file(Span sp) {
  return sp.file;
}

static Pos span_start(Span sp) {
  Pos _sv0t0 = pos_new(sp.start_line, sp.start_col, sp.start_offset);
  return _sv0t0;
}

static Pos span_stop(Span sp) {
  Pos _sv0t0 = pos_new(sp.stop_line, sp.stop_col, sp.stop_offset);
  return _sv0t0;
}

static int span_length(Span sp) {
  int _sv0t0 = (sp.stop_offset - sp.start_offset);
  return _sv0t0;
}

static Span span_merge(Span a, Span b) {
  Span _sv0t0;
  _sv0t0.file = a.file;
  _sv0t0.start_line = a.start_line;
  _sv0t0.start_col = a.start_col;
  _sv0t0.start_offset = a.start_offset;
  _sv0t0.stop_line = b.stop_line;
  _sv0t0.stop_col = b.stop_col;
  _sv0t0.stop_offset = b.stop_offset;
  return _sv0t0;
}

static const char* span_digit_char(int d) {
  if ((d == 0)) {
    return "0";
  } else {
  }
  if ((d == 1)) {
    return "1";
  } else {
  }
  if ((d == 2)) {
    return "2";
  } else {
  }
  if ((d == 3)) {
    return "3";
  } else {
  }
  if ((d == 4)) {
    return "4";
  } else {
  }
  if ((d == 5)) {
    return "5";
  } else {
  }
  if ((d == 6)) {
    return "6";
  } else {
  }
  if ((d == 7)) {
    return "7";
  } else {
  }
  if ((d == 8)) {
    return "8";
  } else {
  }
  return "9";
}

static const char* span_int_to_str(int n) {
  if ((n < 0)) {
    int _sv0t0 = (0 - n);
    const char* _sv0t1 = span_int_to_str(_sv0t0);
    const char* _sv0t2 = sv0_string_concat("-", _sv0t1);
    return _sv0t2;
  } else {
  }
  if ((n < 10)) {
    const char* _sv0t3 = span_digit_char(n);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = (n / 10);
  const char* _sv0t5 = span_int_to_str(_sv0t4);
  const char* rest;
  rest = _sv0t5;
  int _sv0t6 = (n / 10);
  int _sv0t7 = (_sv0t6 * 10);
  int _sv0t8 = (n - _sv0t7);
  const char* _sv0t9 = span_digit_char(_sv0t8);
  const char* last;
  last = _sv0t9;
  const char* _sv0t10 = sv0_string_concat(rest, last);
  return _sv0t10;
}

static const char* pos_to_string(Pos p) {
  const char* _sv0t0 = span_int_to_str(p.line);
  const char* l;
  l = _sv0t0;
  const char* _sv0t1 = span_int_to_str(p.col);
  const char* c;
  c = _sv0t1;
  const char* _sv0t2 = sv0_string_concat(l, ":");
  const char* _sv0t3 = sv0_string_concat(_sv0t2, c);
  return _sv0t3;
}

static const char* span_to_string(Span sp) {
  Pos _sv0t0 = span_start(sp);
  Pos s;
  s = _sv0t0;
  Pos _sv0t1 = span_stop(sp);
  Pos e;
  e = _sv0t1;
  const char* _sv0t2 = pos_to_string(s);
  const char* _sv0t3 = sv0_string_concat(_sv0t2, "-");
  const char* _sv0t4 = pos_to_string(e);
  const char* _sv0t5 = sv0_string_concat(_sv0t3, _sv0t4);
  const char* loc;
  loc = _sv0t5;
  int _sv0t6 = sv0_string_len(sp.file);
  if ((_sv0t6 == 0)) {
    return loc;
  } else {
  }
  const char* _sv0t7 = sv0_string_concat(sp.file, ":");
  const char* _sv0t8 = sv0_string_concat(_sv0t7, loc);
  return _sv0t8;
}

static int test_basics(void) {
  Pos _sv0t0 = pos_new(1, 5, 10);
  Pos p1;
  p1 = _sv0t0;
  int _sv0t1 = pos_line(p1);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = pos_col(p1);
  if ((_sv0t2 != 5)) {
    return 2;
  } else {
  }
  int _sv0t3 = pos_offset(p1);
  if ((_sv0t3 != 10)) {
    return 3;
  } else {
  }
  Pos _sv0t4 = pos_new(3, 12, 45);
  Pos p2;
  p2 = _sv0t4;
  Span _sv0t5 = span_new("test.sv0", p1, p2);
  Span sp;
  sp = _sv0t5;
  Pos _sv0t6 = span_start(sp);
  Pos s;
  s = _sv0t6;
  int _sv0t7 = pos_line(s);
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  int _sv0t8 = pos_col(s);
  if ((_sv0t8 != 5)) {
    return 5;
  } else {
  }
  Pos _sv0t9 = span_stop(sp);
  Pos e;
  e = _sv0t9;
  int _sv0t10 = pos_line(e);
  if ((_sv0t10 != 3)) {
    return 6;
  } else {
  }
  int _sv0t11 = pos_col(e);
  if ((_sv0t11 != 12)) {
    return 7;
  } else {
  }
  int _sv0t12 = span_length(sp);
  int len = _sv0t12;
  if ((len != 35)) {
    return 8;
  } else {
  }
  Span _sv0t13 = span_bogus();
  Span bog;
  bog = _sv0t13;
  Pos _sv0t14 = span_start(bog);
  Pos bs;
  bs = _sv0t14;
  int _sv0t15 = pos_line(bs);
  if ((_sv0t15 != 0)) {
    return 9;
  } else {
  }
  int _sv0t16 = pos_col(bs);
  if ((_sv0t16 != 0)) {
    return 10;
  } else {
  }
  int _sv0t17 = pos_offset(bs);
  if ((_sv0t17 != 0)) {
    return 11;
  } else {
  }
  Pos _sv0t18 = span_stop(bog);
  Pos be_pos;
  be_pos = _sv0t18;
  int _sv0t19 = pos_line(be_pos);
  if ((_sv0t19 != 0)) {
    return 12;
  } else {
  }
  int _sv0t20 = span_length(bog);
  if ((_sv0t20 != 0)) {
    return 13;
  } else {
  }
  return 0;
}

static int test_merge(void) {
  Pos _sv0t0 = pos_new(1, 5, 10);
  Pos pa;
  pa = _sv0t0;
  Pos _sv0t1 = pos_new(3, 12, 45);
  Pos pb;
  pb = _sv0t1;
  Span _sv0t2 = span_new("test.sv0", pa, pb);
  Span spa;
  spa = _sv0t2;
  Pos _sv0t3 = pos_new(5, 20, 80);
  Pos pc;
  pc = _sv0t3;
  Span _sv0t4 = span_new("test.sv0", pb, pc);
  Span spb;
  spb = _sv0t4;
  Span _sv0t5 = span_merge(spa, spb);
  Span merged;
  merged = _sv0t5;
  if ((merged.start_line != 1)) {
    return 1;
  } else {
  }
  if ((merged.start_col != 5)) {
    return 2;
  } else {
  }
  if ((merged.stop_line != 5)) {
    return 3;
  } else {
  }
  if ((merged.stop_col != 20)) {
    return 4;
  } else {
  }
  if ((merged.stop_offset != 80)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_pos_to_string(void) {
  Pos _sv0t0 = pos_new(10, 5, 0);
  Pos p1;
  p1 = _sv0t0;
  const char* _sv0t1 = pos_to_string(p1);
  int _sv0t2 = sv0_string_eq(_sv0t1, "10:5");
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  Pos _sv0t3 = pos_new(1, 1, 0);
  Pos p2;
  p2 = _sv0t3;
  const char* _sv0t4 = pos_to_string(p2);
  int _sv0t5 = sv0_string_eq(_sv0t4, "1:1");
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  Pos _sv0t6 = pos_new(100, 42, 0);
  Pos p3;
  p3 = _sv0t6;
  const char* _sv0t7 = pos_to_string(p3);
  int _sv0t8 = sv0_string_eq(_sv0t7, "100:42");
  if ((_sv0t8 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_span_to_string(void) {
  Pos _sv0t0 = pos_new(1, 5, 10);
  Pos s;
  s = _sv0t0;
  Pos _sv0t1 = pos_new(3, 12, 45);
  Pos e;
  e = _sv0t1;
  Span _sv0t2 = span_new("foo.sv0", s, e);
  Span sp1;
  sp1 = _sv0t2;
  const char* _sv0t3 = span_to_string(sp1);
  int _sv0t4 = sv0_string_eq(_sv0t3, "foo.sv0:1:5-3:12");
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  Span _sv0t5 = span_new("", s, e);
  Span sp2;
  sp2 = _sv0t5;
  const char* _sv0t6 = span_to_string(sp2);
  int _sv0t7 = sv0_string_eq(_sv0t6, "1:5-3:12");
  if ((_sv0t7 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_int_to_str(void) {
  const char* _sv0t0 = span_int_to_str(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "0");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = span_int_to_str(7);
  int _sv0t3 = sv0_string_eq(_sv0t2, "7");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = span_int_to_str(42);
  int _sv0t5 = sv0_string_eq(_sv0t4, "42");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = span_int_to_str(100);
  int _sv0t7 = sv0_string_eq(_sv0t6, "100");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_int_to_str_negative(void) {
  int _sv0t0 = (-1);
  const char* _sv0t1 = span_int_to_str(_sv0t0);
  int _sv0t2 = sv0_string_eq(_sv0t1, "-1");
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = (-42);
  const char* _sv0t4 = span_int_to_str(_sv0t3);
  int _sv0t5 = sv0_string_eq(_sv0t4, "-42");
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_compile_error_roundtrip(void) {
  Pos _sv0t0 = pos_new(4, 5, 9);
  Pos z;
  z = _sv0t0;
  Span _sv0t1 = span_new("x.sv0", z, z);
  Span sp;
  sp = _sv0t1;
  CompileError _sv0t2 = compile_error_new("E0999: probe", sp);
  CompileError e;
  e = _sv0t2;
  const char* _sv0t3 = compile_error_message(e);
  const char* m;
  m = _sv0t3;
  int _sv0t4 = sv0_string_len(m);
  if ((_sv0t4 < 8)) {
    return 1;
  } else {
  }
  Span _sv0t5 = compile_error_span(e);
  Span sp2;
  sp2 = _sv0t5;
  const char* _sv0t6 = span_file(sp2);
  int _sv0t7 = sv0_string_eq(_sv0t6, "x.sv0");
  if ((_sv0t7 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_digit_char_all(void) {
  const char* _sv0t0 = span_digit_char(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "0");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = span_digit_char(4);
  int _sv0t3 = sv0_string_eq(_sv0t2, "4");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = span_digit_char(9);
  int _sv0t5 = sv0_string_eq(_sv0t4, "9");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_int_to_str_boundary(void) {
  const char* _sv0t0 = span_int_to_str(9);
  int _sv0t1 = sv0_string_eq(_sv0t0, "9");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = span_int_to_str(10);
  int _sv0t3 = sv0_string_eq(_sv0t2, "10");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = span_int_to_str(1000);
  int _sv0t5 = sv0_string_eq(_sv0t4, "1000");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  int _sv0t6 = (0 - 100);
  const char* _sv0t7 = span_int_to_str(_sv0t6);
  int _sv0t8 = sv0_string_eq(_sv0t7, "-100");
  if ((_sv0t8 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_span_length_variants(void) {
  Pos _sv0t0 = pos_new(1, 1, 5);
  Pos a;
  a = _sv0t0;
  Span _sv0t1 = span_new("f", a, a);
  Span sp0;
  sp0 = _sv0t1;
  int _sv0t2 = span_length(sp0);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  Pos _sv0t3 = pos_new(1, 2, 6);
  Pos b;
  b = _sv0t3;
  Span _sv0t4 = span_new("f", a, b);
  Span sp1;
  sp1 = _sv0t4;
  int _sv0t5 = span_length(sp1);
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_merge_keeps_start(void) {
  Pos _sv0t0 = pos_new(2, 3, 20);
  Pos pa;
  pa = _sv0t0;
  Pos _sv0t1 = pos_new(2, 8, 25);
  Pos pb;
  pb = _sv0t1;
  Span _sv0t2 = span_new("a.sv0", pa, pb);
  Span spa;
  spa = _sv0t2;
  Pos _sv0t3 = pos_new(9, 1, 99);
  Pos pc;
  pc = _sv0t3;
  Span _sv0t4 = span_new("a.sv0", pb, pc);
  Span spb;
  spb = _sv0t4;
  Span _sv0t5 = span_merge(spa, spb);
  Span m;
  m = _sv0t5;
  if ((m.start_offset != 20)) {
    return 1;
  } else {
  }
  if ((m.stop_offset != 99)) {
    return 2;
  } else {
  }
  int _sv0t6 = span_length(m);
  if ((_sv0t6 != 79)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_pos_accessors(void) {
  Pos _sv0t0 = pos_new(7, 8, 9);
  Pos p;
  p = _sv0t0;
  int _sv0t1 = pos_line(p);
  if ((_sv0t1 != 7)) {
    return 1;
  } else {
  }
  int _sv0t2 = pos_col(p);
  if ((_sv0t2 != 8)) {
    return 2;
  } else {
  }
  int _sv0t3 = pos_offset(p);
  if ((_sv0t3 != 9)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_span_file_accessor(void) {
  Pos _sv0t0 = pos_new(1, 1, 0);
  Pos p;
  p = _sv0t0;
  Span _sv0t1 = span_new("main.sv0", p, p);
  Span sp;
  sp = _sv0t1;
  const char* _sv0t2 = span_file(sp);
  int _sv0t3 = sv0_string_eq(_sv0t2, "main.sv0");
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  Span _sv0t4 = span_bogus();
  Span bog;
  bog = _sv0t4;
  const char* _sv0t5 = span_file(bog);
  int _sv0t6 = sv0_string_len(_sv0t5);
  if ((_sv0t6 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_span_to_string_single(void) {
  Pos _sv0t0 = pos_new(1, 1, 0);
  Pos p;
  p = _sv0t0;
  Span _sv0t1 = span_new("z.sv0", p, p);
  Span sp;
  sp = _sv0t1;
  const char* _sv0t2 = span_to_string(sp);
  int _sv0t3 = sv0_string_eq(_sv0t2, "z.sv0:1:1-1:1");
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_basics();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_merge();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (20 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_pos_to_string();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (30 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_span_to_string();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (40 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_int_to_str();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (50 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_int_to_str_negative();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (60 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_compile_error_roundtrip();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (70 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_digit_char_all();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (80 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_int_to_str_boundary();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (90 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_span_length_variants();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (100 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_merge_keeps_start();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (110 + r11);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_pos_accessors();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (120 + r12);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_span_file_accessor();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (130 + r13);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_span_to_string_single();
  int r14 = _sv0t25;
  if ((r14 != 0)) {
    int _sv0t26 = (140 + r14);
    return _sv0t26;
  } else {
  }
  return 0;
}

