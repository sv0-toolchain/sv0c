#include "sv0_runtime.h"

static int expr_span_tag(int expr_tag);
static int expr_has_span(int expr_tag);
static int merge_spans(int s1_line, int s1_col, int s2_line, int s2_col);
static int skip_generic_depth(int depth, int token_tag);
static int skip_generic_done(int depth);
static int token_is_comma(int token_tag);
static int count_generic_params_from_tokens(int t0, int t1, int t2, int t3, int t4);
static int test_expr_has_span(void);
static int test_merge_spans(void);
static int test_skip_depth(void);
static int test_count_params(void);

static int expr_span_tag(int expr_tag) {
  return expr_tag;
}

static int expr_has_span(int expr_tag) {
  if ((expr_tag >= 0)) {
    if ((expr_tag <= 26)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int merge_spans(int s1_line, int s1_col, int s2_line, int s2_col) {
  if ((s1_line < s2_line)) {
    int _sv0t0 = (s1_line * 1000);
    int _sv0t1 = (_sv0t0 + s1_col);
    return _sv0t1;
  } else {
  }
  if ((s1_line > s2_line)) {
    int _sv0t2 = (s2_line * 1000);
    int _sv0t3 = (_sv0t2 + s2_col);
    return _sv0t3;
  } else {
  }
  if ((s1_col < s2_col)) {
    int _sv0t4 = (s1_line * 1000);
    int _sv0t5 = (_sv0t4 + s1_col);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = (s2_line * 1000);
  int _sv0t7 = (_sv0t6 + s2_col);
  return _sv0t7;
}

static int skip_generic_depth(int depth, int token_tag) {
  if ((token_tag == 31)) {
    int _sv0t0 = (depth + 1);
    return _sv0t0;
  } else {
  }
  if ((token_tag == 32)) {
    if ((depth > 0)) {
      int _sv0t1 = (depth - 1);
      return _sv0t1;
    } else {
    }
  } else {
  }
  return depth;
}

static int skip_generic_done(int depth) {
  int _sv0t0 = (depth == 0);
  return _sv0t0;
}

static int token_is_comma(int token_tag) {
  int _sv0t0 = (token_tag == 12);
  return _sv0t0;
}

static int count_generic_params_from_tokens(int t0, int t1, int t2, int t3, int t4) {
  if ((t0 != 31)) {
    return 0;
  } else {
  }
  int count = 1;
  int _sv0t0 = token_is_comma(t1);
  if (_sv0t0) {
    count = (count + 1);
  } else {
  }
  int _sv0t1 = token_is_comma(t2);
  if (_sv0t1) {
    count = (count + 1);
  } else {
  }
  int _sv0t2 = token_is_comma(t3);
  if (_sv0t2) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int test_expr_has_span(void) {
  int _sv0t0 = expr_has_span(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = expr_has_span(26);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = expr_has_span(27);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = (0 - 1);
  int _sv0t4 = expr_has_span(_sv0t3);
  if ((_sv0t4 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_merge_spans(void) {
  int _sv0t0 = merge_spans(1, 5, 2, 3);
  if ((_sv0t0 != 1005)) {
    return 1;
  } else {
  }
  int _sv0t1 = merge_spans(3, 1, 1, 5);
  if ((_sv0t1 != 1005)) {
    return 2;
  } else {
  }
  int _sv0t2 = merge_spans(1, 1, 1, 5);
  if ((_sv0t2 != 1001)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_skip_depth(void) {
  int _sv0t0 = skip_generic_depth(0, 31);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = skip_generic_depth(1, 32);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = skip_generic_depth(2, 32);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = skip_generic_depth(1, 5);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = skip_generic_done(0);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = skip_generic_done(1);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_count_params(void) {
  int _sv0t0 = count_generic_params_from_tokens(31, 5, 32, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = count_generic_params_from_tokens(31, 12, 5, 32, 0);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = count_generic_params_from_tokens(5, 0, 0, 0, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_expr_has_span();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_merge_spans();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_skip_depth();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_count_params();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  return 0;
}

