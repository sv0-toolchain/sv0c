#include "sv0_runtime.h"

typedef struct {
  int sev_tag;
  int code_nonempty;
  int code_len;
  int msg_len;
  int line;
  int col;
  int span_len;
  int file_nonempty;
  int file_len;
  int related_len;
  int related_fmt_len;
  int help_len;
  int help_fmt_len;
} Diagnostic;
typedef struct {
  int tag;
} Severity;

static int is_error_sev(Severity s);
static int max_i32(int a, int b);
static int decimal_digits_non_negative(int n);
static int decimal_digits_i32(int n);
static int span_len_at_least_one(int slen);
static int rel_col_pad(int col);
static int gutter_width(int line);
static int sev_label_len(int sev_tag);
static int bracket_code_len(int code_nonempty, int code_len);
static int format_header_len(int sev_tag, int code_nonempty, int code_len, int msg_len);
static int file_colon_part(int file_nonempty, int file_len);
static int format_loc_len(int file_nonempty, int file_len, int line, int col);
static int format_snippet_len(int line, int col, int span_len, int has_source, int src_line_len);
static int format_diagnostic_len(int sev_tag, int code_nonempty, int code_len, int msg_len, int line, int col, int span_len, int file_nonempty, int file_len, int related_fmt_len, int help_fmt_len, int has_source, int src_line_len);

static int is_error_sev(Severity s) {
  int _sv0t0;
  if ((s.tag == 0)) {
    _sv0t0 = 1;
  } else {
    if ((s.tag == 1)) {
      _sv0t0 = 0;
    } else {
      if ((s.tag == 2)) {
        _sv0t0 = 0;
      } else {
      }
    }
  }
  return _sv0t0;
}

static int max_i32(int a, int b) {
  int _sv0t0;
  int _sv0t1;
  if ((a > b)) {
    return a;
    _sv0t1 = 0;
  } else {
    return b;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int decimal_digits_non_negative(int n) {
  int _sv0t0;
  int _sv0t1;
  if ((n == 0)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((n < 10)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3 = (n / 10);
      int _sv0t4 = decimal_digits_non_negative(_sv0t3);
      int _sv0t5 = (1 + _sv0t4);
      return _sv0t5;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int decimal_digits_i32(int n) {
  int _sv0t0;
  int _sv0t1;
  if ((n < 0)) {
    int _sv0t2 = (0 - n);
    int _sv0t3 = decimal_digits_non_negative(_sv0t2);
    int _sv0t4 = (1 + _sv0t3);
    return _sv0t4;
    _sv0t1 = 0;
  } else {
    int _sv0t5 = decimal_digits_non_negative(n);
    return _sv0t5;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int span_len_at_least_one(int slen) {
  int _sv0t0;
  int _sv0t1;
  if ((slen < 1)) {
    return 1;
    _sv0t1 = 0;
  } else {
    return slen;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int rel_col_pad(int col) {
  int _sv0t0;
  int _sv0t1;
  if ((col <= 1)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2 = (col - 1);
    return _sv0t2;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int gutter_width(int line) {
  int _sv0t0 = decimal_digits_i32(line);
  int _sv0t1 = max_i32(_sv0t0, 2);
  return _sv0t1;
}

static int sev_label_len(int sev_tag) {
  int _sv0t0;
  if ((sev_tag == 0)) {
    _sv0t0 = 5;
  } else {
    if ((sev_tag == 1)) {
      _sv0t0 = 7;
    } else {
      if (1) {
        _sv0t0 = 4;
      } else {
      }
    }
  }
  return _sv0t0;
}

static int bracket_code_len(int code_nonempty, int code_len) {
  int _sv0t0;
  int _sv0t1;
  if ((code_nonempty == 0)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2 = (2 + code_len);
    return _sv0t2;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int format_header_len(int sev_tag, int code_nonempty, int code_len, int msg_len) {
  int _sv0t0 = bracket_code_len(code_nonempty, code_len);
  int suf = _sv0t0;
  int _sv0t1 = sev_label_len(sev_tag);
  int _sv0t2 = (_sv0t1 + suf);
  int _sv0t3 = (_sv0t2 + 2);
  int head = (_sv0t3 + msg_len);
  int _sv0t4 = (head + 1);
  return _sv0t4;
}

static int file_colon_part(int file_nonempty, int file_len) {
  int _sv0t0;
  int _sv0t1;
  if ((file_nonempty == 0)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2 = (file_len + 1);
    return _sv0t2;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int format_loc_len(int file_nonempty, int file_len, int line, int col) {
  int _sv0t0 = file_colon_part(file_nonempty, file_len);
  int _sv0t1 = (6 + _sv0t0);
  int _sv0t2 = decimal_digits_i32(line);
  int prefix = (_sv0t1 + _sv0t2);
  int _sv0t3 = decimal_digits_i32(col);
  int colon_and_col = (1 + _sv0t3);
  int loc_nl = 1;
  int _sv0t4 = (prefix + colon_and_col);
  int _sv0t5 = (_sv0t4 + loc_nl);
  return _sv0t5;
}

static int format_snippet_len(int line, int col, int span_len, int has_source, int src_line_len) {
  int _sv0t0;
  int _sv0t1;
  if ((has_source == 0)) {
    return 0;
    _sv0t1 = 0;
  } else {
    int _sv0t2 = gutter_width(line);
    int gw = _sv0t2;
    int pref = (gw + 3);
    int blank = (pref + 1);
    int _sv0t3 = (pref + src_line_len);
    int numbered = (_sv0t3 + 1);
    int _sv0t4 = rel_col_pad(col);
    int _sv0t5 = (pref + _sv0t4);
    int _sv0t6 = span_len_at_least_one(span_len);
    int _sv0t7 = (_sv0t5 + _sv0t6);
    int underline = (_sv0t7 + 1);
    int _sv0t8 = (blank + numbered);
    int _sv0t9 = (_sv0t8 + underline);
    return _sv0t9;
    _sv0t1 = 0;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

static int format_diagnostic_len(int sev_tag, int code_nonempty, int code_len, int msg_len, int line, int col, int span_len, int file_nonempty, int file_len, int related_fmt_len, int help_fmt_len, int has_source, int src_line_len) {
  int _sv0t0 = format_header_len(sev_tag, code_nonempty, code_len, msg_len);
  int h = _sv0t0;
  int _sv0t1 = format_loc_len(file_nonempty, file_len, line, col);
  int l = _sv0t1;
  int _sv0t2 = format_snippet_len(line, col, span_len, has_source, src_line_len);
  int s = _sv0t2;
  int _sv0t3 = (h + l);
  int _sv0t4 = (_sv0t3 + s);
  int _sv0t5 = (_sv0t4 + related_fmt_len);
  int _sv0t6 = (_sv0t5 + help_fmt_len);
  int _sv0t7 = (_sv0t6 + 1);
  return _sv0t7;
}

int main(void) {
  Diagnostic d;
  d.sev_tag = 0;
  d.code_nonempty = 1;
  d.code_len = 3;
  d.msg_len = 24;
  d.line = 10;
  d.col = 3;
  d.span_len = 4;
  d.file_nonempty = 1;
  d.file_len = 8;
  d.related_len = 0;
  d.related_fmt_len = 0;
  d.help_len = 1;
  d.help_fmt_len = 19;
  int _sv0t0 = format_diagnostic_len(d.sev_tag, d.code_nonempty, d.code_len, d.msg_len, d.line, d.col, d.span_len, d.file_nonempty, d.file_len, d.related_fmt_len, d.help_fmt_len, 1, 15);
  int n = _sv0t0;
  int expect = 116;
  int ok_len = (n - expect);
  int _sv0t1;
  int _sv0t2;
  if ((d.sev_tag == 0)) {
    _sv0t2 = 1;
  } else {
    _sv0t2 = 0;
  }
  _sv0t1 = _sv0t2;
  int is_err = _sv0t1;
  int ok_sev = (is_err - 1);
  Severity _sv0t3;
  _sv0t3.tag = 1;
  int _sv0t4 = is_error_sev(_sv0t3);
  int ok_warn = _sv0t4;
  int _sv0t5 = (ok_len + ok_sev);
  int _sv0t6 = (_sv0t5 + ok_warn);
  return _sv0t6;
}

