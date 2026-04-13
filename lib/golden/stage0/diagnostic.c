#include "sv0_runtime.h"

typedef struct {
  int tag;
} Severity;

static int severity_tag(Severity s);
static Severity make_error(int code, int message, int span_line);
static Severity make_warning(int code, int message, int span_line);
static Severity make_note(int message, int span_line);
static int is_error(Severity s);
static int has_errors(int count);
static int error_count(int e0, int e1, int e2, int e3);
static const char* severity_to_string(Severity s);
static const char* spaces(int n);
static const char* carets(int n);
static const char* pad_left(const char* s, int width);
static const char* get_line(const char* source, int line_no);
static int severity_ord(Severity a, Severity b);
static int diag_list_len(int sevs);
static int diag_list_push(int sevs, int lines, int cols, int msgs, int sev, int line, int col, int msg);
static int diag_list_has_errors(int sevs);
static int diag_list_error_count(int sevs);
static int diag_span_less(int lines, int cols, int a, int b);
static int diag_sort_by_span(int sevs, int lines, int cols, int msgs, int order);
static const char* format_header(int sev_tag, const char* message);
static const char* format_snippet(const char* source, int line_no, int col, int span_len);
static int test_severity_tags(void);
static int test_constructors(void);
static int test_has_errors(void);
static int test_error_count(void);
static int test_severity_ord(void);
static int test_severity_to_string(void);
static int test_spaces(void);
static int test_carets(void);
static int test_pad_left(void);
static int test_get_line(void);
static int test_diag_list(void);
static int test_diag_sort(void);
static int test_format_header(void);
static int test_format_snippet(void);

static int severity_tag(Severity s) {
  int _sv0t0;
  if ((s.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((s.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((s.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
      }
    }
  }
  return _sv0t0;
}

static Severity make_error(int code, int message, int span_line) {
  Severity _sv0t0;
  _sv0t0.tag = 0;
  return _sv0t0;
}

static Severity make_warning(int code, int message, int span_line) {
  Severity _sv0t0;
  _sv0t0.tag = 1;
  return _sv0t0;
}

static Severity make_note(int message, int span_line) {
  Severity _sv0t0;
  _sv0t0.tag = 2;
  return _sv0t0;
}

static int is_error(Severity s) {
  int _sv0t0;
  if ((s.tag == 0)) {
    return 1;
    _sv0t0 = 0;
  } else {
    if (1) {
      return 0;
      _sv0t0 = 0;
    } else {
    }
  }
  return _sv0t0;
}

static int has_errors(int count) {
  int _sv0t0 = (count > 0);
  return _sv0t0;
}

static int error_count(int e0, int e1, int e2, int e3) {
  int count = 0;
  if ((e0 == 0)) {
    count = (count + 1);
  } else {
  }
  if ((e1 == 0)) {
    count = (count + 1);
  } else {
  }
  if ((e2 == 0)) {
    count = (count + 1);
  } else {
  }
  if ((e3 == 0)) {
    count = (count + 1);
  } else {
  }
  return count;
}

static const char* severity_to_string(Severity s) {
  int _sv0t0 = severity_tag(s);
  int tag = _sv0t0;
  if ((tag == 0)) {
    return "error";
  } else {
  }
  if ((tag == 1)) {
    return "warning";
  } else {
  }
  return "note";
}

static const char* spaces(int n) {
  if ((n <= 0)) {
    return "";
  } else {
  }
  const char* out;
  out = "";
  int i = 0;
  while ((i < n)) {
    const char* _sv0t0 = sv0_string_concat(out, " ");
    out = _sv0t0;
    i = (i + 1);
  }
  return out;
}

static const char* carets(int n) {
  int effective = n;
  if ((effective < 1)) {
    effective = 1;
  } else {
  }
  const char* out;
  out = "";
  int i = 0;
  while ((i < effective)) {
    const char* _sv0t0 = sv0_string_concat(out, "^");
    out = _sv0t0;
    i = (i + 1);
  }
  return out;
}

static const char* pad_left(const char* s, int width) {
  int _sv0t0 = sv0_string_len(s);
  int slen = _sv0t0;
  int pad = (width - slen);
  if ((pad <= 0)) {
    return s;
  } else {
  }
  const char* _sv0t1 = spaces(pad);
  const char* prefix;
  prefix = _sv0t1;
  const char* _sv0t2 = sv0_string_concat(prefix, s);
  return _sv0t2;
}

static const char* get_line(const char* source, int line_no) {
  if ((line_no < 1)) {
    return "";
  } else {
  }
  int _sv0t0 = sv0_string_len(source);
  int slen = _sv0t0;
  int i = 0;
  int start = 0;
  int cur_line = 1;
  while ((i <= slen)) {
    int at_end = (i == slen);
    int is_nl = 0;
    if ((at_end != 1)) {
      int _sv0t1 = sv0_string_char_at(source, i);
      int c = _sv0t1;
      if ((c == 10)) {
        is_nl = 1;
      } else {
      }
    } else {
    }
    if ((at_end == 1)) {
      if ((cur_line == line_no)) {
        int _sv0t2 = (i - start);
        const char* _sv0t3 = sv0_string_substr(source, start, _sv0t2);
        return _sv0t3;
      } else {
      }
      return "";
    } else {
    }
    if ((is_nl == 1)) {
      if ((cur_line == line_no)) {
        int _sv0t4 = (i - start);
        const char* _sv0t5 = sv0_string_substr(source, start, _sv0t4);
        return _sv0t5;
      } else {
      }
      cur_line = (cur_line + 1);
      start = (i + 1);
    } else {
    }
    i = (i + 1);
  }
  return "";
}

static int severity_ord(Severity a, Severity b) {
  int _sv0t0 = severity_tag(a);
  int ta = _sv0t0;
  int _sv0t1 = severity_tag(b);
  int tb = _sv0t1;
  if ((ta < tb)) {
    return 0;
  } else {
  }
  if ((ta == tb)) {
    return 1;
  } else {
  }
  return 2;
}

static int diag_list_len(int sevs) {
  int _sv0t0 = sv0_vec_len(sevs);
  return _sv0t0;
}

static int diag_list_push(int sevs, int lines, int cols, int msgs, int sev, int line, int col, int msg) {
  sv0_vec_push(sevs, sev);
  sv0_vec_push(lines, line);
  sv0_vec_push(cols, col);
  sv0_vec_push(msgs, msg);
  int _sv0t0 = sv0_vec_len(sevs);
  return _sv0t0;
}

static int diag_list_has_errors(int sevs) {
  int _sv0t0 = sv0_vec_len(sevs);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(sevs, i);
    if ((_sv0t1 == 0)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int diag_list_error_count(int sevs) {
  int _sv0t0 = sv0_vec_len(sevs);
  int n = _sv0t0;
  int count = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(sevs, i);
    if ((_sv0t1 == 0)) {
      count = (count + 1);
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int diag_span_less(int lines, int cols, int a, int b) {
  int _sv0t0 = sv0_vec_get(lines, a);
  int la = _sv0t0;
  int _sv0t1 = sv0_vec_get(lines, b);
  int lb = _sv0t1;
  if ((la < lb)) {
    return 1;
  } else {
  }
  if ((la > lb)) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_vec_get(cols, a);
  int _sv0t3 = sv0_vec_get(cols, b);
  int _sv0t4 = (_sv0t2 < _sv0t3);
  return _sv0t4;
}

static int diag_sort_by_span(int sevs, int lines, int cols, int msgs, int order) {
  int _sv0t0 = sv0_vec_len(sevs);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    sv0_vec_push(order, i);
    i = (i + 1);
  }
  int i2 = 1;
  while ((i2 < n)) {
    int _sv0t1 = sv0_vec_get(order, i2);
    int key = _sv0t1;
    int j = (i2 - 1);
    int shifting = 1;
    while (shifting) {
      if ((j < 0)) {
        shifting = 0;
      } else {
        int _sv0t2 = sv0_vec_get(order, j);
        int cur = _sv0t2;
        int _sv0t3 = diag_span_less(lines, cols, key, cur);
        if (_sv0t3) {
          int _sv0t4 = (j + 1);
          sv0_vec_set(order, _sv0t4, cur);
          j = (j - 1);
        } else {
          shifting = 0;
        }
      }
    }
    int _sv0t5 = (j + 1);
    sv0_vec_set(order, _sv0t5, key);
    i2 = (i2 + 1);
  }
  return n;
}

static const char* format_header(int sev_tag, const char* message) {
  const char* out;
  out = "";
  if ((sev_tag == 0)) {
    out = "error";
  } else {
  }
  if ((sev_tag == 1)) {
    out = "warning";
  } else {
  }
  if ((sev_tag == 2)) {
    out = "note";
  } else {
  }
  const char* _sv0t0 = sv0_string_concat(out, ": ");
  out = _sv0t0;
  const char* _sv0t1 = sv0_string_concat(out, message);
  out = _sv0t1;
  return out;
}

static const char* format_snippet(const char* source, int line_no, int col, int span_len) {
  const char* _sv0t0 = get_line(source, line_no);
  const char* line_text;
  line_text = _sv0t0;
  int _sv0t1 = sv0_string_len(line_text);
  if ((_sv0t1 == 0)) {
    return "";
  } else {
  }
  int effective_span = span_len;
  if ((effective_span < 1)) {
    effective_span = 1;
  } else {
  }
  const char* out;
  out = "  | ";
  const char* _sv0t2 = sv0_string_concat(out, line_text);
  out = _sv0t2;
  const char* _sv0t3 = sv0_string_concat(out, "\n");
  out = _sv0t3;
  const char* _sv0t4 = sv0_string_concat(out, "  | ");
  out = _sv0t4;
  int col_offset = (col - 1);
  if ((col_offset > 0)) {
    const char* _sv0t5 = spaces(col_offset);
    const char* _sv0t6 = sv0_string_concat(out, _sv0t5);
    out = _sv0t6;
  } else {
  }
  const char* _sv0t7 = carets(effective_span);
  const char* _sv0t8 = sv0_string_concat(out, _sv0t7);
  out = _sv0t8;
  return out;
}

static int test_severity_tags(void) {
  Severity _sv0t0;
  _sv0t0.tag = 0;
  int _sv0t1 = severity_tag(_sv0t0);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  Severity _sv0t2;
  _sv0t2.tag = 1;
  int _sv0t3 = severity_tag(_sv0t2);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  Severity _sv0t4;
  _sv0t4.tag = 2;
  int _sv0t5 = severity_tag(_sv0t4);
  if ((_sv0t5 != 2)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_constructors(void) {
  Severity _sv0t0 = make_error(400, 1, 10);
  Severity e;
  e = _sv0t0;
  int _sv0t1 = is_error(e);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  Severity _sv0t2 = make_warning(401, 2, 20);
  Severity w;
  w = _sv0t2;
  int _sv0t3 = is_error(w);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  Severity _sv0t4 = make_note(3, 30);
  Severity n;
  n = _sv0t4;
  int _sv0t5 = is_error(n);
  if ((_sv0t5 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_has_errors(void) {
  int _sv0t0 = has_errors(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = has_errors(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = has_errors(5);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_error_count(void) {
  int _sv0t0 = error_count(0, 0, 0, 0);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = error_count(0, 1, 0, 1);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = error_count(1, 1, 1, 1);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = error_count(0, 1, 1, 1);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_severity_ord(void) {
  Severity _sv0t0;
  _sv0t0.tag = 0;
  Severity _sv0t1;
  _sv0t1.tag = 1;
  int _sv0t2 = severity_ord(_sv0t0, _sv0t1);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  Severity _sv0t3;
  _sv0t3.tag = 0;
  Severity _sv0t4;
  _sv0t4.tag = 0;
  int _sv0t5 = severity_ord(_sv0t3, _sv0t4);
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  Severity _sv0t6;
  _sv0t6.tag = 1;
  Severity _sv0t7;
  _sv0t7.tag = 0;
  int _sv0t8 = severity_ord(_sv0t6, _sv0t7);
  if ((_sv0t8 != 2)) {
    return 3;
  } else {
  }
  Severity _sv0t9;
  _sv0t9.tag = 2;
  Severity _sv0t10;
  _sv0t10.tag = 0;
  int _sv0t11 = severity_ord(_sv0t9, _sv0t10);
  if ((_sv0t11 != 2)) {
    return 4;
  } else {
  }
  Severity _sv0t12;
  _sv0t12.tag = 0;
  Severity _sv0t13;
  _sv0t13.tag = 2;
  int _sv0t14 = severity_ord(_sv0t12, _sv0t13);
  if ((_sv0t14 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_severity_to_string(void) {
  Severity _sv0t0;
  _sv0t0.tag = 0;
  const char* _sv0t1 = severity_to_string(_sv0t0);
  int _sv0t2 = sv0_string_eq(_sv0t1, "error");
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  Severity _sv0t3;
  _sv0t3.tag = 1;
  const char* _sv0t4 = severity_to_string(_sv0t3);
  int _sv0t5 = sv0_string_eq(_sv0t4, "warning");
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  Severity _sv0t6;
  _sv0t6.tag = 2;
  const char* _sv0t7 = severity_to_string(_sv0t6);
  int _sv0t8 = sv0_string_eq(_sv0t7, "note");
  if ((_sv0t8 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_spaces(void) {
  const char* _sv0t0 = spaces(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = spaces(3);
  int _sv0t3 = sv0_string_len(_sv0t2);
  if ((_sv0t3 != 3)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = spaces(1);
  int _sv0t5 = sv0_string_eq(_sv0t4, " ");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_carets(void) {
  const char* _sv0t0 = carets(3);
  int _sv0t1 = sv0_string_eq(_sv0t0, "^^^");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = carets(1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "^");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = carets(0);
  int _sv0t5 = sv0_string_eq(_sv0t4, "^");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_pad_left(void) {
  const char* _sv0t0 = pad_left("42", 5);
  const char* r1;
  r1 = _sv0t0;
  int _sv0t1 = sv0_string_len(r1);
  if ((_sv0t1 != 5)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = pad_left("hello", 3);
  int _sv0t3 = sv0_string_eq(_sv0t2, "hello");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = pad_left("x", 1);
  int _sv0t5 = sv0_string_eq(_sv0t4, "x");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_get_line(void) {
  const char* src;
  src = "alpha\nbeta\ngamma";
  const char* _sv0t0 = get_line(src, 1);
  int _sv0t1 = sv0_string_eq(_sv0t0, "alpha");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = get_line(src, 2);
  int _sv0t3 = sv0_string_eq(_sv0t2, "beta");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = get_line(src, 3);
  int _sv0t5 = sv0_string_eq(_sv0t4, "gamma");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = get_line(src, 4);
  int _sv0t7 = sv0_string_eq(_sv0t6, "");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = get_line(src, 0);
  int _sv0t9 = sv0_string_eq(_sv0t8, "");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t10 = get_line("single", 1);
  int _sv0t11 = sv0_string_eq(_sv0t10, "single");
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_diag_list(void) {
  int _sv0t0 = sv0_vec_new();
  int sevs = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int lines = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int cols = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int msgs = _sv0t3;
  int _sv0t4 = diag_list_has_errors(sevs);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = diag_list_error_count(sevs);
  if ((_sv0t5 != 0)) {
    return 2;
  } else {
  }
  int _sv0t6 = diag_list_push(sevs, lines, cols, msgs, 1, 5, 3, 100);
  int _sv0t7 = diag_list_has_errors(sevs);
  if ((_sv0t7 != 0)) {
    return 3;
  } else {
  }
  int _sv0t8 = diag_list_push(sevs, lines, cols, msgs, 0, 2, 10, 200);
  int _sv0t9 = diag_list_has_errors(sevs);
  if ((_sv0t9 != 1)) {
    return 4;
  } else {
  }
  int _sv0t10 = diag_list_error_count(sevs);
  if ((_sv0t10 != 1)) {
    return 5;
  } else {
  }
  int _sv0t11 = diag_list_push(sevs, lines, cols, msgs, 0, 8, 1, 300);
  int _sv0t12 = diag_list_error_count(sevs);
  if ((_sv0t12 != 2)) {
    return 6;
  } else {
  }
  int _sv0t13 = diag_list_len(sevs);
  if ((_sv0t13 != 3)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_diag_sort(void) {
  int _sv0t0 = sv0_vec_new();
  int sevs = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int lines = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int cols = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int msgs = _sv0t3;
  int _sv0t4 = diag_list_push(sevs, lines, cols, msgs, 0, 10, 5, 1);
  int _sv0t5 = diag_list_push(sevs, lines, cols, msgs, 1, 2, 3, 2);
  int _sv0t6 = diag_list_push(sevs, lines, cols, msgs, 0, 2, 1, 3);
  int _sv0t7 = diag_list_push(sevs, lines, cols, msgs, 0, 10, 2, 4);
  int _sv0t8 = sv0_vec_new();
  int order = _sv0t8;
  int _sv0t9 = diag_sort_by_span(sevs, lines, cols, msgs, order);
  int _sv0t10 = sv0_vec_get(order, 0);
  if ((_sv0t10 != 2)) {
    return 1;
  } else {
  }
  int _sv0t11 = sv0_vec_get(order, 1);
  if ((_sv0t11 != 1)) {
    return 2;
  } else {
  }
  int _sv0t12 = sv0_vec_get(order, 2);
  if ((_sv0t12 != 3)) {
    return 3;
  } else {
  }
  int _sv0t13 = sv0_vec_get(order, 3);
  if ((_sv0t13 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_format_header(void) {
  const char* _sv0t0 = format_header(0, "undeclared variable");
  const char* h1;
  h1 = _sv0t0;
  int _sv0t1 = sv0_string_eq(h1, "error: undeclared variable");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = format_header(1, "unused import");
  const char* h2;
  h2 = _sv0t2;
  int _sv0t3 = sv0_string_eq(h2, "warning: unused import");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = format_header(2, "defined here");
  const char* h3;
  h3 = _sv0t4;
  int _sv0t5 = sv0_string_eq(h3, "note: defined here");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_format_snippet(void) {
  const char* src;
  src = "let x = 42;\nlet y = x + 1;";
  const char* _sv0t0 = format_snippet(src, 1, 5, 1);
  const char* snip;
  snip = _sv0t0;
  int _sv0t1 = sv0_string_len(snip);
  if ((_sv0t1 == 0)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = format_snippet(src, 99, 1, 1);
  const char* empty;
  empty = _sv0t2;
  int _sv0t3 = sv0_string_len(empty);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_severity_tags();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_constructors();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_has_errors();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_error_count();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_severity_ord();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_severity_to_string();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_spaces();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (60 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_carets();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (70 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_pad_left();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (80 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_get_line();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (90 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_diag_list();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (100 + r11);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_diag_sort();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (110 + r12);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_format_header();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (120 + r13);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_format_snippet();
  int r14 = _sv0t25;
  if ((r14 != 0)) {
    int _sv0t26 = (130 + r14);
    return _sv0t26;
  } else {
  }
  return 0;
}

