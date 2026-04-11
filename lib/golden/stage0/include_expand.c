#include "sv0_runtime.h"

static int is_space(int c);
static int path_ok(const char* rel);
static int has_include_prefix(const char* line);
static int find_closing_quote(const char* s, int start);
static int chomp_cr(const char* line);
static int test_is_space(void);
static int test_path_ok(void);
static int test_has_include_prefix(void);
static int test_find_closing_quote(void);
static int test_chomp_cr(void);

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

static int chomp_cr(const char* line) {
  int _sv0t0 = sv0_string_len(line);
  int len = _sv0t0;
  if ((len == 0)) {
    return len;
  } else {
  }
  int _sv0t1 = (len - 1);
  int _sv0t2 = sv0_string_char_at(line, _sv0t1);
  int last = _sv0t2;
  if ((last == 13)) {
    int _sv0t3 = (len - 1);
    return _sv0t3;
  } else {
  }
  return len;
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

static int test_chomp_cr(void) {
  int _sv0t0 = chomp_cr("hello");
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  int _sv0t1 = chomp_cr("");
  if ((_sv0t1 != 0)) {
    return 2;
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
  int _sv0t1 = test_path_ok();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_has_include_prefix();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_find_closing_quote();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_chomp_cr();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (40 + r5);
    return _sv0t8;
  } else {
  }
  return 0;
}

