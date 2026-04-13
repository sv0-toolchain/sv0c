#include "sv0_runtime.h"

static int is_space(int c);
static int trim_left(const char* s, int i, int len);
static int trim_right(const char* s, int j);
static const char* trim(const char* s);
static int path_ok(const char* rel);
static int has_include_prefix(const char* line);
static int find_closing_quote(const char* s, int start);
static const char* chomp_cr(const char* line);
static const char* parse_include_line(const char* line);
static int test_is_space(void);
static int test_path_ok(void);
static int test_has_include_prefix(void);
static int test_find_closing_quote(void);
static int test_trim(void);
static int test_chomp_cr(void);
static int test_parse_include_line(void);

static int is_space(int c) {
  if ((c == 32)) {
    return 1;
  } else {
  }
  if ((c == 9)) {
    return 1;
  } else {
  }
  if ((c == 10)) {
    return 1;
  } else {
  }
  if ((c == 11)) {
    return 1;
  } else {
  }
  if ((c == 12)) {
    return 1;
  } else {
  }
  return 0;
}

static int trim_left(const char* s, int i, int len) {
  if ((i >= len)) {
    return i;
  } else {
  }
  int _sv0t0 = sv0_string_char_at(s, i);
  int c = _sv0t0;
  int _sv0t1 = is_space(c);
  if (_sv0t1) {
    int _sv0t2 = (i + 1);
    int _sv0t3 = trim_left(s, _sv0t2, len);
    return _sv0t3;
  } else {
  }
  return i;
}

static int trim_right(const char* s, int j) {
  if ((j <= 0)) {
    return j;
  } else {
  }
  int _sv0t0 = (j - 1);
  int _sv0t1 = sv0_string_char_at(s, _sv0t0);
  int c = _sv0t1;
  int _sv0t2 = is_space(c);
  if (_sv0t2) {
    int _sv0t3 = (j - 1);
    int _sv0t4 = trim_right(s, _sv0t3);
    return _sv0t4;
  } else {
  }
  return j;
}

static const char* trim(const char* s) {
  int _sv0t0 = sv0_string_len(s);
  int len = _sv0t0;
  int _sv0t1 = trim_left(s, 0, len);
  int lo = _sv0t1;
  int _sv0t2 = trim_right(s, len);
  int hi = _sv0t2;
  if ((lo >= hi)) {
    return "";
  } else {
  }
  int _sv0t3 = (hi - lo);
  const char* _sv0t4 = sv0_string_substr(s, lo, _sv0t3);
  return _sv0t4;
}

static int path_ok(const char* rel) {
  int _sv0t0 = sv0_string_len(rel);
  int len = _sv0t0;
  if ((len == 0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_string_char_at(rel, 0);
  int first = _sv0t1;
  if ((first == 47)) {
    return 0;
  } else {
  }
  int i = 0;
  int _sv0t2 = (len - 1);
  while ((i < _sv0t2)) {
    int _sv0t3 = sv0_string_char_at(rel, i);
    int c0 = _sv0t3;
    int _sv0t4 = (i + 1);
    int _sv0t5 = sv0_string_char_at(rel, _sv0t4);
    int c1 = _sv0t5;
    if ((c0 == 46)) {
      if ((c1 == 46)) {
        return 0;
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return 1;
}

static int has_include_prefix(const char* line) {
  int _sv0t0 = sv0_string_len(line);
  int len = _sv0t0;
  if ((len < 10)) {
    return 0;
  } else {
  }
  const char* _sv0t1 = sv0_string_substr(line, 0, 9);
  const char* prefix;
  prefix = _sv0t1;
  int _sv0t2 = sv0_string_eq(prefix, "include \"");
  return _sv0t2;
}

static int find_closing_quote(const char* s, int start) {
  int _sv0t0 = sv0_string_len(s);
  int len = _sv0t0;
  int i = start;
  while ((i < len)) {
    int _sv0t1 = sv0_string_char_at(s, i);
    int c = _sv0t1;
    if ((c == 34)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static const char* chomp_cr(const char* line) {
  int _sv0t0 = sv0_string_len(line);
  int len = _sv0t0;
  if ((len == 0)) {
    return line;
  } else {
  }
  int _sv0t1 = (len - 1);
  int _sv0t2 = sv0_string_char_at(line, _sv0t1);
  int last = _sv0t2;
  if ((last == 13)) {
    int _sv0t3 = (len - 1);
    const char* _sv0t4 = sv0_string_substr(line, 0, _sv0t3);
    return _sv0t4;
  } else {
  }
  return line;
}

static const char* parse_include_line(const char* line) {
  const char* _sv0t0 = chomp_cr(line);
  const char* _sv0t1 = trim(_sv0t0);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = has_include_prefix(s);
  if ((_sv0t2 != 1)) {
    return "";
  } else {
  }
  int _sv0t3 = sv0_string_len(s);
  int _sv0t4 = (_sv0t3 - 9);
  const char* _sv0t5 = sv0_string_substr(s, 9, _sv0t4);
  const char* after;
  after = _sv0t5;
  int _sv0t6 = find_closing_quote(after, 0);
  int q = _sv0t6;
  if ((q < 0)) {
    return "";
  } else {
  }
  const char* _sv0t7 = sv0_string_substr(after, 0, q);
  const char* p;
  p = _sv0t7;
  int _sv0t8 = (q + 1);
  int _sv0t9 = sv0_string_len(after);
  int _sv0t10 = (_sv0t9 - q);
  int _sv0t11 = (_sv0t10 - 1);
  const char* _sv0t12 = sv0_string_substr(after, _sv0t8, _sv0t11);
  const char* _sv0t13 = trim(_sv0t12);
  const char* rest;
  rest = _sv0t13;
  int _sv0t14 = sv0_string_eq(rest, ";");
  if ((_sv0t14 != 1)) {
    return "";
  } else {
  }
  int _sv0t15 = path_ok(p);
  if ((_sv0t15 != 1)) {
    return "";
  } else {
  }
  return p;
}

static int test_is_space(void) {
  int _sv0t0 = is_space(32);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_space(9);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_space(10);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_space(11);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_space(12);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_space(65);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_space(0);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_space(48);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_path_ok(void) {
  int _sv0t0 = path_ok("foo.sv0");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = path_ok("src/foo.sv0");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = path_ok("");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = path_ok("/abs/path");
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = path_ok("../escape");
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = path_ok("foo/../bar");
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = path_ok("a.b");
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_has_include_prefix(void) {
  int _sv0t0 = has_include_prefix("include \"foo.sv0\";");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = has_include_prefix("let x = 1;");
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = has_include_prefix("include");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = has_include_prefix("");
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_find_closing_quote(void) {
  int _sv0t0 = find_closing_quote("hello\"world", 0);
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  int _sv0t1 = find_closing_quote("noquote", 0);
  int _sv0t2 = (0 - 1);
  if ((_sv0t1 != _sv0t2)) {
    return 2;
  } else {
  }
  int _sv0t3 = find_closing_quote("a\"b\"c", 2);
  if ((_sv0t3 != 3)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_trim(void) {
  const char* _sv0t0 = trim("hello");
  int _sv0t1 = sv0_string_eq(_sv0t0, "hello");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = trim("  hello  ");
  int _sv0t3 = sv0_string_eq(_sv0t2, "hello");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = trim("");
  int _sv0t5 = sv0_string_eq(_sv0t4, "");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = trim("   ");
  int _sv0t7 = sv0_string_eq(_sv0t6, "");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = trim("  a b  ");
  int _sv0t9 = sv0_string_eq(_sv0t8, "a b");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_chomp_cr(void) {
  const char* _sv0t0 = chomp_cr("hello");
  int _sv0t1 = sv0_string_eq(_sv0t0, "hello");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = chomp_cr("");
  int _sv0t3 = sv0_string_eq(_sv0t2, "");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_parse_include_line(void) {
  const char* _sv0t0 = parse_include_line("include \"foo.sv0\";");
  const char* r1;
  r1 = _sv0t0;
  int _sv0t1 = sv0_string_eq(r1, "foo.sv0");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = parse_include_line("  include \"bar.sv0\" ;  ");
  const char* r2;
  r2 = _sv0t2;
  int _sv0t3 = sv0_string_eq(r2, "bar.sv0");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = parse_include_line("let x = 1;");
  const char* r3;
  r3 = _sv0t4;
  int _sv0t5 = sv0_string_eq(r3, "");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = parse_include_line("include \"../escape\";");
  const char* r4;
  r4 = _sv0t6;
  int _sv0t7 = sv0_string_eq(r4, "");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = parse_include_line("include \"/abs/path\";");
  const char* r5;
  r5 = _sv0t8;
  int _sv0t9 = sv0_string_eq(r5, "");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t10 = parse_include_line("include \"foo.sv0\"");
  const char* r6;
  r6 = _sv0t10;
  int _sv0t11 = sv0_string_eq(r6, "");
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  const char* _sv0t12 = parse_include_line("include \"sub/mod.sv0\";");
  const char* r7;
  r7 = _sv0t12;
  int _sv0t13 = sv0_string_eq(r7, "sub/mod.sv0");
  if ((_sv0t13 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_is_space();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_trim();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_path_ok();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_has_include_prefix();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_find_closing_quote();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_chomp_cr();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (50 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_parse_include_line();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (60 + r7);
    return _sv0t12;
  } else {
  }
  return 0;
}

