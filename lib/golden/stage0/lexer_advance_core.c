#include "sv0_runtime.h"

static int advance_pos(int pos, int source_len);
static int advance_line(int line, int char_code);
static int advance_col(int col, int char_code);
static int at_end(int pos, int source_len);
static int extract_len(int start_off, int end_off);
static int span_from_positions(int start_line, int start_col, int end_line, int end_col);
static int test_advance_pos(void);
static int test_advance_line(void);
static int test_advance_col(void);
static int test_at_end(void);
static int test_extract_len(void);
static int test_span(void);

static int advance_pos(int pos, int source_len) {
  if ((pos >= source_len)) {
    return pos;
  } else {
  }
  int _sv0t0 = (pos + 1);
  return _sv0t0;
}

static int advance_line(int line, int char_code) {
  if ((char_code == 10)) {
    int _sv0t0 = (line + 1);
    return _sv0t0;
  } else {
  }
  return line;
}

static int advance_col(int col, int char_code) {
  if ((char_code == 10)) {
    return 1;
  } else {
  }
  int _sv0t0 = (col + 1);
  return _sv0t0;
}

static int at_end(int pos, int source_len) {
  int _sv0t0 = (pos >= source_len);
  return _sv0t0;
}

static int extract_len(int start_off, int end_off) {
  if ((end_off <= start_off)) {
    return 0;
  } else {
  }
  int _sv0t0 = (end_off - start_off);
  return _sv0t0;
}

static int span_from_positions(int start_line, int start_col, int end_line, int end_col) {
  int _sv0t0 = (end_line - start_line);
  int _sv0t1 = (_sv0t0 * 1000);
  int _sv0t2 = (end_col - start_col);
  int _sv0t3 = (_sv0t1 + _sv0t2);
  return _sv0t3;
}

static int test_advance_pos(void) {
  int _sv0t0 = advance_pos(0, 10);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = advance_pos(9, 10);
  if ((_sv0t1 != 10)) {
    return 2;
  } else {
  }
  int _sv0t2 = advance_pos(10, 10);
  if ((_sv0t2 != 10)) {
    return 3;
  } else {
  }
  int _sv0t3 = advance_pos(15, 10);
  if ((_sv0t3 != 15)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_advance_line(void) {
  int _sv0t0 = advance_line(1, 65);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = advance_line(1, 10);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = advance_line(5, 10);
  if ((_sv0t2 != 6)) {
    return 3;
  } else {
  }
  int _sv0t3 = advance_line(5, 32);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_advance_col(void) {
  int _sv0t0 = advance_col(1, 65);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = advance_col(5, 66);
  if ((_sv0t1 != 6)) {
    return 2;
  } else {
  }
  int _sv0t2 = advance_col(5, 10);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = advance_col(1, 10);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_at_end(void) {
  int _sv0t0 = at_end(0, 5);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = at_end(4, 5);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = at_end(5, 5);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = at_end(10, 5);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_extract_len(void) {
  int _sv0t0 = extract_len(0, 5);
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  int _sv0t1 = extract_len(3, 7);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = extract_len(5, 5);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = extract_len(7, 3);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_span(void) {
  int _sv0t0 = span_from_positions(1, 1, 1, 5);
  int s1 = _sv0t0;
  if ((s1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = span_from_positions(1, 1, 3, 2);
  int s2 = _sv0t1;
  if ((s2 != 2001)) {
    return 2;
  } else {
  }
  int _sv0t2 = span_from_positions(5, 10, 5, 10);
  int s3 = _sv0t2;
  if ((s3 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_advance_pos();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_advance_line();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_advance_col();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_at_end();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_extract_len();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_span();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  return 0;
}

