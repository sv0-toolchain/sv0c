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

int main(void) {
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
  Pos be;
  be = _sv0t18;
  int _sv0t19 = pos_line(be);
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

