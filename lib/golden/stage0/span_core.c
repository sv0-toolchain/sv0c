#include "sv0_runtime.h"

typedef struct {
  int line;
  int col;
  int offset;
} Pos;

static int pos_line(Pos p);
static int pos_col(Pos p);
static int span_length(int start_off, int stop_off);

static int pos_line(Pos p) {
  return p.line;
}

static int pos_col(Pos p) {
  return p.col;
}

static int span_length(int start_off, int stop_off) {
  int _sv0t0 = (stop_off - start_off);
  return _sv0t0;
}

int main(void) {
  Pos p;
  p.line = 2;
  p.col = 5;
  p.offset = 42;
  int _sv0t0 = pos_line(p);
  int _sv0t1 = pos_col(p);
  int _sv0t2 = (_sv0t0 + _sv0t1);
  int _sv0t3 = span_length(10, 14);
  int sum = (_sv0t2 + _sv0t3);
  int _sv0t4 = (sum - sum);
  return _sv0t4;
}

