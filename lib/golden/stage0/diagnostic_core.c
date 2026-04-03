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

static int dec_pos(int n);
static int is_error_sev(Severity s);

static int dec_pos(int n) {
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
      int _sv0t4 = dec_pos(_sv0t3);
      int _sv0t5 = (1 + _sv0t4);
      return _sv0t5;
      _sv0t2 = 0;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

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
  int has_source = 1;
  int src_line_len = 15;
  int sev_tag = d.sev_tag;
  int code_nonempty = d.code_nonempty;
  int code_len = d.code_len;
  int msg_len = d.msg_len;
  int line = d.line;
  int col = d.col;
  int span_len = d.span_len;
  int file_nonempty = d.file_nonempty;
  int file_len = d.file_len;
  int related_fmt_len = d.related_fmt_len;
  int help_fmt_len = d.help_fmt_len;
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
  int sev_word = _sv0t0;
  int _sv0t1;
  int _sv0t2;
  if ((code_nonempty == 0)) {
    _sv0t2 = 0;
  } else {
    _sv0t2 = (2 + code_len);
  }
  _sv0t1 = _sv0t2;
  int code_bracket = _sv0t1;
  int _sv0t3 = (sev_word + code_bracket);
  int _sv0t4 = (_sv0t3 + 2);
  int header_core = (_sv0t4 + msg_len);
  int header_len = (header_core + 1);
  int _sv0t5;
  int _sv0t6;
  if ((file_nonempty == 0)) {
    _sv0t6 = 0;
  } else {
    _sv0t6 = (file_len + 1);
  }
  _sv0t5 = _sv0t6;
  int file_part = _sv0t5;
  int _sv0t7;
  int _sv0t8;
  if ((line < 0)) {
    int _sv0t9 = (0 - line);
    int _sv0t10 = dec_pos(_sv0t9);
    _sv0t8 = (1 + _sv0t10);
  } else {
    int _sv0t11 = dec_pos(line);
    _sv0t8 = _sv0t11;
  }
  _sv0t7 = _sv0t8;
  int line_digits = _sv0t7;
  int _sv0t12;
  int _sv0t13;
  if ((col < 0)) {
    int _sv0t14 = (0 - col);
    int _sv0t15 = dec_pos(_sv0t14);
    _sv0t13 = (1 + _sv0t15);
  } else {
    int _sv0t16 = dec_pos(col);
    _sv0t13 = _sv0t16;
  }
  _sv0t12 = _sv0t13;
  int col_digits = _sv0t12;
  int _sv0t17 = (6 + file_part);
  int _sv0t18 = (_sv0t17 + line_digits);
  int _sv0t19 = (_sv0t18 + 1);
  int _sv0t20 = (_sv0t19 + col_digits);
  int loc_len = (_sv0t20 + 1);
  int _sv0t21;
  int _sv0t22;
  if ((line_digits > 2)) {
    _sv0t22 = line_digits;
  } else {
    _sv0t22 = 2;
  }
  _sv0t21 = _sv0t22;
  int gw = _sv0t21;
  int pref = (gw + 3);
  int blank = (pref + 1);
  int _sv0t23 = (pref + src_line_len);
  int numbered = (_sv0t23 + 1);
  int _sv0t24;
  int _sv0t25;
  if ((col <= 1)) {
    _sv0t25 = 0;
  } else {
    _sv0t25 = (col - 1);
  }
  _sv0t24 = _sv0t25;
  int col_pad = _sv0t24;
  int _sv0t26;
  int _sv0t27;
  if ((span_len < 1)) {
    _sv0t27 = 1;
  } else {
    _sv0t27 = span_len;
  }
  _sv0t26 = _sv0t27;
  int carets = _sv0t26;
  int _sv0t28 = (pref + col_pad);
  int _sv0t29 = (_sv0t28 + carets);
  int underline = (_sv0t29 + 1);
  int _sv0t30;
  int _sv0t31;
  if ((has_source == 0)) {
    _sv0t31 = 0;
  } else {
    int _sv0t32 = (blank + numbered);
    _sv0t31 = (_sv0t32 + underline);
  }
  _sv0t30 = _sv0t31;
  int snippet_len = _sv0t30;
  int _sv0t33 = (header_len + loc_len);
  int _sv0t34 = (_sv0t33 + snippet_len);
  int _sv0t35 = (_sv0t34 + related_fmt_len);
  int body_len = (_sv0t35 + help_fmt_len);
  int n = (body_len + 1);
  int expect = 116;
  int ok_len = (n - expect);
  int _sv0t36;
  int _sv0t37;
  if ((sev_tag == 0)) {
    _sv0t37 = 1;
  } else {
    _sv0t37 = 0;
  }
  _sv0t36 = _sv0t37;
  int is_err = _sv0t36;
  int ok_sev = (is_err - 1);
  Severity _sv0t38;
  _sv0t38.tag = 1;
  int _sv0t39 = is_error_sev(_sv0t38);
  int ok_warn = _sv0t39;
  int _sv0t40 = (ok_len + ok_sev);
  int _sv0t41 = (_sv0t40 + ok_warn);
  return _sv0t41;
}

