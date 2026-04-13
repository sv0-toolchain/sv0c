#include "sv0_runtime.h"

static int is_ident_start(int c);
static int is_ident_cont(int c);
static int is_digit(int c);
static int is_hex_digit(int c);
static int is_bin_digit(int c);
static int is_oct_digit(int c);
static int is_whitespace(int c);
static int lookup_keyword(const char* s);
static int escape_char(int c);
static int hex_digit_value(int c);
static int hex_escape_value(int d1, int d2);
static int lex_at_end(const char* source, int pos);
static int lex_peek(const char* source, int pos);
static int lex_peek_at(const char* source, int pos, int offset);
static int advance_line(int line, int c);
static int advance_col(int col, int c);
static int skip_ws(const char* source, int pos);
static int skip_line_comment(const char* source, int pos);
static int skip_block_comment(const char* source, int pos, int depth);
static int skip_all(const char* source, int pos);
static int scan_ident_end(const char* source, int pos);
static int scan_dec_digits_end(const char* source, int pos);
static int scan_hex_digits_end(const char* source, int pos);
static int scan_bin_digits_end(const char* source, int pos);
static int scan_oct_digits_end(const char* source, int pos);
static int op_single_tag(int c);
static int op_double_tag(int c1, int c2);
static int op_fallback_tag(int c);
static int test_char_class(void);
static int test_keyword_lookup(void);
static int test_escape(void);
static int test_state(void);
static int test_skip_ws(void);
static int test_skip_line_comment(void);
static int test_skip_block_comment(void);
static int test_skip_all(void);
static int test_scan_ident(void);
static int test_scan_digits(void);
static int test_op_tags(void);

static int is_ident_start(int c) {
  if ((c >= 65)) {
    if ((c <= 90)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((c >= 97)) {
    if ((c <= 122)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((c == 95)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_ident_cont(int c) {
  int _sv0t0 = is_ident_start(c);
  if (_sv0t0) {
    return 1;
  } else {
  }
  if ((c >= 48)) {
    if ((c <= 57)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_digit(int c) {
  if ((c >= 48)) {
    if ((c <= 57)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_hex_digit(int c) {
  int _sv0t0 = is_digit(c);
  if (_sv0t0) {
    return 1;
  } else {
  }
  if ((c >= 65)) {
    if ((c <= 70)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((c >= 97)) {
    if ((c <= 102)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_bin_digit(int c) {
  if ((c == 48)) {
    return 1;
  } else {
  }
  if ((c == 49)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_oct_digit(int c) {
  if ((c >= 48)) {
    if ((c <= 55)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_whitespace(int c) {
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
  if ((c == 13)) {
    return 1;
  } else {
  }
  return 0;
}

static int lookup_keyword(const char* s) {
  int _sv0t0 = sv0_string_len(s);
  int len = _sv0t0;
  if ((len < 2)) {
    return 5;
  } else {
  }
  if ((len > 14)) {
    return 5;
  } else {
  }
  int _sv0t1 = sv0_string_char_at(s, 0);
  int c0 = _sv0t1;
  if ((c0 == 97)) {
    int _sv0t2 = sv0_string_eq(s, "as");
    if (_sv0t2) {
      return 54;
    } else {
    }
    int _sv0t3 = sv0_string_eq(s, "assert");
    if (_sv0t3) {
      return 55;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 98)) {
    int _sv0t4 = sv0_string_eq(s, "borrows");
    if (_sv0t4) {
      return 56;
    } else {
    }
    int _sv0t5 = sv0_string_eq(s, "break");
    if (_sv0t5) {
      return 57;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 99)) {
    int _sv0t6 = sv0_string_eq(s, "const");
    if (_sv0t6) {
      return 58;
    } else {
    }
    int _sv0t7 = sv0_string_eq(s, "continue");
    if (_sv0t7) {
      return 59;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 101)) {
    int _sv0t8 = sv0_string_eq(s, "else");
    if (_sv0t8) {
      return 60;
    } else {
    }
    int _sv0t9 = sv0_string_eq(s, "ensures");
    if (_sv0t9) {
      return 61;
    } else {
    }
    int _sv0t10 = sv0_string_eq(s, "enum");
    if (_sv0t10) {
      return 62;
    } else {
    }
    int _sv0t11 = sv0_string_eq(s, "exists");
    if (_sv0t11) {
      return 63;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 102)) {
    int _sv0t12 = sv0_string_eq(s, "false");
    if (_sv0t12) {
      return 64;
    } else {
    }
    int _sv0t13 = sv0_string_eq(s, "fn");
    if (_sv0t13) {
      return 65;
    } else {
    }
    int _sv0t14 = sv0_string_eq(s, "for");
    if (_sv0t14) {
      return 66;
    } else {
    }
    int _sv0t15 = sv0_string_eq(s, "forall");
    if (_sv0t15) {
      return 67;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 105)) {
    int _sv0t16 = sv0_string_eq(s, "if");
    if (_sv0t16) {
      return 68;
    } else {
    }
    int _sv0t17 = sv0_string_eq(s, "impl");
    if (_sv0t17) {
      return 69;
    } else {
    }
    int _sv0t18 = sv0_string_eq(s, "in");
    if (_sv0t18) {
      return 70;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 108)) {
    int _sv0t19 = sv0_string_eq(s, "let");
    if (_sv0t19) {
      return 71;
    } else {
    }
    int _sv0t20 = sv0_string_eq(s, "loop");
    if (_sv0t20) {
      return 72;
    } else {
    }
    int _sv0t21 = sv0_string_eq(s, "loop_invariant");
    if (_sv0t21) {
      return 73;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 109)) {
    int _sv0t22 = sv0_string_eq(s, "match");
    if (_sv0t22) {
      return 74;
    } else {
    }
    int _sv0t23 = sv0_string_eq(s, "module");
    if (_sv0t23) {
      return 75;
    } else {
    }
    int _sv0t24 = sv0_string_eq(s, "move");
    if (_sv0t24) {
      return 76;
    } else {
    }
    int _sv0t25 = sv0_string_eq(s, "mut");
    if (_sv0t25) {
      return 77;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 110)) {
    int _sv0t26 = sv0_string_eq(s, "newtype");
    if (_sv0t26) {
      return 78;
    } else {
    }
    int _sv0t27 = sv0_string_eq(s, "no_alias");
    if (_sv0t27) {
      return 79;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 111)) {
    int _sv0t28 = sv0_string_eq(s, "old");
    if (_sv0t28) {
      return 80;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 112)) {
    int _sv0t29 = sv0_string_eq(s, "project");
    if (_sv0t29) {
      return 81;
    } else {
    }
    int _sv0t30 = sv0_string_eq(s, "pub");
    if (_sv0t30) {
      return 82;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 114)) {
    int _sv0t31 = sv0_string_eq(s, "requires");
    if (_sv0t31) {
      return 83;
    } else {
    }
    int _sv0t32 = sv0_string_eq(s, "result");
    if (_sv0t32) {
      return 84;
    } else {
    }
    int _sv0t33 = sv0_string_eq(s, "return");
    if (_sv0t33) {
      return 85;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 115)) {
    int _sv0t34 = sv0_string_eq(s, "self");
    if (_sv0t34) {
      return 86;
    } else {
    }
    int _sv0t35 = sv0_string_eq(s, "static");
    if (_sv0t35) {
      return 88;
    } else {
    }
    int _sv0t36 = sv0_string_eq(s, "struct");
    if (_sv0t36) {
      return 89;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 83)) {
    int _sv0t37 = sv0_string_eq(s, "Self");
    if (_sv0t37) {
      return 87;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 116)) {
    int _sv0t38 = sv0_string_eq(s, "trait");
    if (_sv0t38) {
      return 90;
    } else {
    }
    int _sv0t39 = sv0_string_eq(s, "true");
    if (_sv0t39) {
      return 91;
    } else {
    }
    int _sv0t40 = sv0_string_eq(s, "type");
    if (_sv0t40) {
      return 92;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 117)) {
    int _sv0t41 = sv0_string_eq(s, "unsafe");
    if (_sv0t41) {
      return 93;
    } else {
    }
    int _sv0t42 = sv0_string_eq(s, "use");
    if (_sv0t42) {
      return 94;
    } else {
    }
    return 5;
  } else {
  }
  if ((c0 == 119)) {
    int _sv0t43 = sv0_string_eq(s, "where");
    if (_sv0t43) {
      return 95;
    } else {
    }
    int _sv0t44 = sv0_string_eq(s, "while");
    if (_sv0t44) {
      return 96;
    } else {
    }
    return 5;
  } else {
  }
  return 5;
}

static int escape_char(int c) {
  if ((c == 110)) {
    return 10;
  } else {
  }
  if ((c == 116)) {
    return 9;
  } else {
  }
  if ((c == 114)) {
    return 13;
  } else {
  }
  if ((c == 92)) {
    return 92;
  } else {
  }
  if ((c == 34)) {
    return 34;
  } else {
  }
  if ((c == 39)) {
    return 39;
  } else {
  }
  if ((c == 48)) {
    return 0;
  } else {
  }
  return c;
}

static int hex_digit_value(int c) {
  if ((c >= 48)) {
    if ((c <= 57)) {
      int _sv0t0 = (c - 48);
      return _sv0t0;
    } else {
    }
  } else {
  }
  if ((c >= 65)) {
    if ((c <= 70)) {
      int _sv0t1 = (c - 55);
      return _sv0t1;
    } else {
    }
  } else {
  }
  if ((c >= 97)) {
    if ((c <= 102)) {
      int _sv0t2 = (c - 87);
      return _sv0t2;
    } else {
    }
  } else {
  }
  return 0;
}

static int hex_escape_value(int d1, int d2) {
  int _sv0t0 = hex_digit_value(d1);
  int _sv0t1 = (_sv0t0 * 16);
  int _sv0t2 = hex_digit_value(d2);
  int _sv0t3 = (_sv0t1 + _sv0t2);
  return _sv0t3;
}

static int lex_at_end(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int _sv0t1 = (pos >= _sv0t0);
  return _sv0t1;
}

static int lex_peek(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  if ((pos >= _sv0t0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_string_char_at(source, pos);
  return _sv0t2;
}

static int lex_peek_at(const char* source, int pos, int offset) {
  int i = (pos + offset);
  if ((i < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_string_len(source);
  if ((i >= _sv0t1)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_string_char_at(source, i);
  return _sv0t3;
}

static int advance_line(int line, int c) {
  if ((c == 10)) {
    int _sv0t0 = (line + 1);
    return _sv0t0;
  } else {
  }
  return line;
}

static int advance_col(int col, int c) {
  if ((c == 10)) {
    return 1;
  } else {
  }
  int _sv0t0 = (col + 1);
  return _sv0t0;
}

static int skip_ws(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int len = _sv0t0;
  int p = pos;
  while ((p < len)) {
    int _sv0t1 = sv0_string_char_at(source, p);
    int c = _sv0t1;
    int _sv0t2 = is_whitespace(c);
    if ((_sv0t2 != 1)) {
      return p;
    } else {
    }
    p = (p + 1);
  }
  return p;
}

static int skip_line_comment(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int len = _sv0t0;
  int p = pos;
  while ((p < len)) {
    int _sv0t1 = sv0_string_char_at(source, p);
    int c = _sv0t1;
    if ((c == 10)) {
      return p;
    } else {
    }
    p = (p + 1);
  }
  return p;
}

static int skip_block_comment(const char* source, int pos, int depth) {
  int _sv0t0 = sv0_string_len(source);
  int len = _sv0t0;
  int p = pos;
  int d = depth;
  while ((d > 0)) {
    if ((p >= len)) {
      return p;
    } else {
    }
    int _sv0t1 = sv0_string_char_at(source, p);
    int c = _sv0t1;
    int _sv0t2 = lex_peek_at(source, p, 1);
    int nx = _sv0t2;
    if ((c == 47)) {
      if ((nx == 42)) {
        d = (d + 1);
        p = (p + 2);
      } else {
        p = (p + 1);
      }
    } else {
      if ((c == 42)) {
        if ((nx == 47)) {
          d = (d - 1);
          p = (p + 2);
        } else {
          p = (p + 1);
        }
      } else {
        p = (p + 1);
      }
    }
  }
  return p;
}

static int skip_all(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int len = _sv0t0;
  int _sv0t1 = skip_ws(source, pos);
  int p = _sv0t1;
  int done = 0;
  while ((done != 1)) {
    if ((p >= len)) {
      return p;
    } else {
    }
    int _sv0t2 = sv0_string_char_at(source, p);
    int c = _sv0t2;
    if ((c == 47)) {
      int _sv0t3 = lex_peek_at(source, p, 1);
      int nx = _sv0t3;
      if ((nx == 47)) {
        int _sv0t4 = (p + 2);
        int _sv0t5 = skip_line_comment(source, _sv0t4);
        p = _sv0t5;
        int _sv0t6 = skip_ws(source, p);
        p = _sv0t6;
      } else {
        if ((nx == 42)) {
          int _sv0t7 = (p + 2);
          int _sv0t8 = skip_block_comment(source, _sv0t7, 1);
          p = _sv0t8;
          int _sv0t9 = skip_ws(source, p);
          p = _sv0t9;
        } else {
          done = 1;
        }
      }
    } else {
      done = 1;
    }
  }
  return p;
}

static int scan_ident_end(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int len = _sv0t0;
  int p = pos;
  while ((p < len)) {
    int _sv0t1 = sv0_string_char_at(source, p);
    int c = _sv0t1;
    int _sv0t2 = is_ident_cont(c);
    if ((_sv0t2 != 1)) {
      return p;
    } else {
    }
    p = (p + 1);
  }
  return p;
}

static int scan_dec_digits_end(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int len = _sv0t0;
  int p = pos;
  while ((p < len)) {
    int _sv0t1 = sv0_string_char_at(source, p);
    int c = _sv0t1;
    int _sv0t2 = is_digit(c);
    if ((_sv0t2 != 1)) {
      if ((c != 95)) {
        return p;
      } else {
      }
    } else {
    }
    p = (p + 1);
  }
  return p;
}

static int scan_hex_digits_end(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int len = _sv0t0;
  int p = pos;
  while ((p < len)) {
    int _sv0t1 = sv0_string_char_at(source, p);
    int c = _sv0t1;
    int _sv0t2 = is_hex_digit(c);
    if ((_sv0t2 != 1)) {
      if ((c != 95)) {
        return p;
      } else {
      }
    } else {
    }
    p = (p + 1);
  }
  return p;
}

static int scan_bin_digits_end(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int len = _sv0t0;
  int p = pos;
  while ((p < len)) {
    int _sv0t1 = sv0_string_char_at(source, p);
    int c = _sv0t1;
    int _sv0t2 = is_bin_digit(c);
    if ((_sv0t2 != 1)) {
      if ((c != 95)) {
        return p;
      } else {
      }
    } else {
    }
    p = (p + 1);
  }
  return p;
}

static int scan_oct_digits_end(const char* source, int pos) {
  int _sv0t0 = sv0_string_len(source);
  int len = _sv0t0;
  int p = pos;
  while ((p < len)) {
    int _sv0t1 = sv0_string_char_at(source, p);
    int c = _sv0t1;
    int _sv0t2 = is_oct_digit(c);
    if ((_sv0t2 != 1)) {
      if ((c != 95)) {
        return p;
      } else {
      }
    } else {
    }
    p = (p + 1);
  }
  return p;
}

static int op_single_tag(int c) {
  if ((c == 40)) {
    return 6;
  } else {
  }
  if ((c == 41)) {
    return 7;
  } else {
  }
  if ((c == 123)) {
    return 8;
  } else {
  }
  if ((c == 125)) {
    return 9;
  } else {
  }
  if ((c == 91)) {
    return 10;
  } else {
  }
  if ((c == 93)) {
    return 11;
  } else {
  }
  if ((c == 44)) {
    return 12;
  } else {
  }
  if ((c == 59)) {
    return 13;
  } else {
  }
  if ((c == 35)) {
    return 21;
  } else {
  }
  if ((c == 126)) {
    return 32;
  } else {
  }
  if ((c == 63)) {
    return 34;
  } else {
  }
  return 0;
}

static int op_double_tag(int c1, int c2) {
  if ((c1 == 58)) {
    if ((c2 == 58)) {
      return 15;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 46)) {
    if ((c2 == 46)) {
      return 17;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 43)) {
    if ((c2 == 61)) {
      return 44;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 45)) {
    if ((c2 == 62)) {
      return 19;
    } else {
    }
    if ((c2 == 61)) {
      return 45;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 42)) {
    if ((c2 == 61)) {
      return 46;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 47)) {
    if ((c2 == 61)) {
      return 47;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 37)) {
    if ((c2 == 61)) {
      return 48;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 61)) {
    if ((c2 == 61)) {
      return 36;
    } else {
    }
    if ((c2 == 62)) {
      return 20;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 33)) {
    if ((c2 == 61)) {
      return 37;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 60)) {
    if ((c2 == 61)) {
      return 40;
    } else {
    }
    if ((c2 == 60)) {
      return 42;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 62)) {
    if ((c2 == 61)) {
      return 41;
    } else {
    }
    if ((c2 == 62)) {
      return 43;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 38)) {
    if ((c2 == 38)) {
      return 28;
    } else {
    }
    if ((c2 == 61)) {
      return 49;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 124)) {
    if ((c2 == 124)) {
      return 30;
    } else {
    }
    if ((c2 == 61)) {
      return 50;
    } else {
    }
    return 0;
  } else {
  }
  if ((c1 == 94)) {
    if ((c2 == 61)) {
      return 51;
    } else {
    }
    return 0;
  } else {
  }
  return 0;
}

static int op_fallback_tag(int c) {
  if ((c == 58)) {
    return 14;
  } else {
  }
  if ((c == 46)) {
    return 16;
  } else {
  }
  if ((c == 43)) {
    return 22;
  } else {
  }
  if ((c == 45)) {
    return 23;
  } else {
  }
  if ((c == 42)) {
    return 24;
  } else {
  }
  if ((c == 47)) {
    return 25;
  } else {
  }
  if ((c == 37)) {
    return 26;
  } else {
  }
  if ((c == 38)) {
    return 27;
  } else {
  }
  if ((c == 124)) {
    return 29;
  } else {
  }
  if ((c == 94)) {
    return 31;
  } else {
  }
  if ((c == 33)) {
    return 33;
  } else {
  }
  if ((c == 61)) {
    return 35;
  } else {
  }
  if ((c == 60)) {
    return 38;
  } else {
  }
  if ((c == 62)) {
    return 39;
  } else {
  }
  return 0;
}

static int test_char_class(void) {
  int _sv0t0 = is_ident_start(65);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_ident_start(95);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_ident_start(48);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_ident_cont(48);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_ident_cont(32);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_digit(48);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_digit(57);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_digit(65);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_hex_digit(65);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = is_hex_digit(103);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = is_bin_digit(48);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = is_bin_digit(50);
  if ((_sv0t11 != 0)) {
    return 12;
  } else {
  }
  int _sv0t12 = is_oct_digit(55);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  int _sv0t13 = is_oct_digit(56);
  if ((_sv0t13 != 0)) {
    return 14;
  } else {
  }
  int _sv0t14 = is_whitespace(32);
  if ((_sv0t14 != 1)) {
    return 15;
  } else {
  }
  int _sv0t15 = is_whitespace(65);
  if ((_sv0t15 != 0)) {
    return 16;
  } else {
  }
  return 0;
}

static int test_keyword_lookup(void) {
  int _sv0t0 = lookup_keyword("fn");
  if ((_sv0t0 != 65)) {
    return 1;
  } else {
  }
  int _sv0t1 = lookup_keyword("let");
  if ((_sv0t1 != 71)) {
    return 2;
  } else {
  }
  int _sv0t2 = lookup_keyword("if");
  if ((_sv0t2 != 68)) {
    return 3;
  } else {
  }
  int _sv0t3 = lookup_keyword("while");
  if ((_sv0t3 != 96)) {
    return 4;
  } else {
  }
  int _sv0t4 = lookup_keyword("struct");
  if ((_sv0t4 != 89)) {
    return 5;
  } else {
  }
  int _sv0t5 = lookup_keyword("enum");
  if ((_sv0t5 != 62)) {
    return 6;
  } else {
  }
  int _sv0t6 = lookup_keyword("return");
  if ((_sv0t6 != 85)) {
    return 7;
  } else {
  }
  int _sv0t7 = lookup_keyword("Self");
  if ((_sv0t7 != 87)) {
    return 8;
  } else {
  }
  int _sv0t8 = lookup_keyword("self");
  if ((_sv0t8 != 86)) {
    return 9;
  } else {
  }
  int _sv0t9 = lookup_keyword("loop_invariant");
  if ((_sv0t9 != 73)) {
    return 10;
  } else {
  }
  int _sv0t10 = lookup_keyword("foo");
  if ((_sv0t10 != 5)) {
    return 11;
  } else {
  }
  int _sv0t11 = lookup_keyword("x");
  if ((_sv0t11 != 5)) {
    return 12;
  } else {
  }
  int _sv0t12 = lookup_keyword("letter");
  if ((_sv0t12 != 5)) {
    return 13;
  } else {
  }
  int _sv0t13 = lookup_keyword("true");
  if ((_sv0t13 != 91)) {
    return 14;
  } else {
  }
  int _sv0t14 = lookup_keyword("false");
  if ((_sv0t14 != 64)) {
    return 15;
  } else {
  }
  return 0;
}

static int test_escape(void) {
  int _sv0t0 = escape_char(110);
  if ((_sv0t0 != 10)) {
    return 1;
  } else {
  }
  int _sv0t1 = escape_char(116);
  if ((_sv0t1 != 9)) {
    return 2;
  } else {
  }
  int _sv0t2 = escape_char(114);
  if ((_sv0t2 != 13)) {
    return 3;
  } else {
  }
  int _sv0t3 = escape_char(92);
  if ((_sv0t3 != 92)) {
    return 4;
  } else {
  }
  int _sv0t4 = escape_char(34);
  if ((_sv0t4 != 34)) {
    return 5;
  } else {
  }
  int _sv0t5 = escape_char(48);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = hex_digit_value(48);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = hex_digit_value(57);
  if ((_sv0t7 != 9)) {
    return 8;
  } else {
  }
  int _sv0t8 = hex_digit_value(65);
  if ((_sv0t8 != 10)) {
    return 9;
  } else {
  }
  int _sv0t9 = hex_escape_value(52, 49);
  if ((_sv0t9 != 65)) {
    return 10;
  } else {
  }
  return 0;
}

static int test_state(void) {
  const char* src;
  src = "hello";
  int _sv0t0 = lex_at_end(src, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = lex_at_end(src, 5);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = lex_peek(src, 0);
  if ((_sv0t2 != 104)) {
    return 3;
  } else {
  }
  int _sv0t3 = lex_peek(src, 4);
  if ((_sv0t3 != 111)) {
    return 4;
  } else {
  }
  int _sv0t4 = lex_peek(src, 5);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 5;
  } else {
  }
  int _sv0t6 = lex_peek_at(src, 0, 1);
  if ((_sv0t6 != 101)) {
    return 6;
  } else {
  }
  int _sv0t7 = lex_peek_at(src, 4, 1);
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 7;
  } else {
  }
  int _sv0t9 = advance_line(1, 65);
  if ((_sv0t9 != 1)) {
    return 8;
  } else {
  }
  int _sv0t10 = advance_line(1, 10);
  if ((_sv0t10 != 2)) {
    return 9;
  } else {
  }
  int _sv0t11 = advance_col(5, 65);
  if ((_sv0t11 != 6)) {
    return 10;
  } else {
  }
  int _sv0t12 = advance_col(5, 10);
  if ((_sv0t12 != 1)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_skip_ws(void) {
  const char* s1;
  s1 = "  hello";
  int _sv0t0 = skip_ws(s1, 0);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  const char* s2;
  s2 = "abc";
  int _sv0t1 = skip_ws(s2, 0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  const char* s3;
  s3 = "   ";
  int _sv0t2 = skip_ws(s3, 0);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_skip_line_comment(void) {
  const char* src;
  src = "comment here\nnext";
  int _sv0t0 = skip_line_comment(src, 0);
  int end = _sv0t0;
  if ((end != 12)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_skip_block_comment(void) {
  const char* src;
  src = "inner */ rest";
  int _sv0t0 = skip_block_comment(src, 0, 1);
  int end = _sv0t0;
  if ((end != 8)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_skip_all(void) {
  const char* s1;
  s1 = "  fn";
  int _sv0t0 = skip_all(s1, 0);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  const char* s2;
  s2 = "// comment\nfn";
  int _sv0t1 = skip_all(s2, 0);
  if ((_sv0t1 != 11)) {
    return 2;
  } else {
  }
  const char* s3;
  s3 = "/* block */ fn";
  int _sv0t2 = skip_all(s3, 0);
  if ((_sv0t2 != 12)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_scan_ident(void) {
  const char* s1;
  s1 = "hello world";
  int _sv0t0 = scan_ident_end(s1, 0);
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  const char* s2;
  s2 = "x123+y";
  int _sv0t1 = scan_ident_end(s2, 0);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  const char* s3;
  s3 = "_foo bar";
  int _sv0t2 = scan_ident_end(s3, 0);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_scan_digits(void) {
  const char* s1;
  s1 = "12345abc";
  int _sv0t0 = scan_dec_digits_end(s1, 0);
  if ((_sv0t0 != 5)) {
    return 1;
  } else {
  }
  const char* s2;
  s2 = "ff00gg";
  int _sv0t1 = scan_hex_digits_end(s2, 0);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  const char* s3;
  s3 = "0110x";
  int _sv0t2 = scan_bin_digits_end(s3, 0);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  const char* s4;
  s4 = "3778";
  int _sv0t3 = scan_oct_digits_end(s4, 0);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  const char* s5;
  s5 = "1_000_000x";
  int _sv0t4 = scan_dec_digits_end(s5, 0);
  if ((_sv0t4 != 9)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_op_tags(void) {
  int _sv0t0 = op_single_tag(40);
  if ((_sv0t0 != 6)) {
    return 1;
  } else {
  }
  int _sv0t1 = op_single_tag(41);
  if ((_sv0t1 != 7)) {
    return 2;
  } else {
  }
  int _sv0t2 = op_single_tag(44);
  if ((_sv0t2 != 12)) {
    return 3;
  } else {
  }
  int _sv0t3 = op_single_tag(65);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = op_double_tag(61, 61);
  if ((_sv0t4 != 36)) {
    return 5;
  } else {
  }
  int _sv0t5 = op_double_tag(45, 62);
  if ((_sv0t5 != 19)) {
    return 6;
  } else {
  }
  int _sv0t6 = op_double_tag(60, 60);
  if ((_sv0t6 != 42)) {
    return 7;
  } else {
  }
  int _sv0t7 = op_double_tag(65, 66);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = op_fallback_tag(43);
  if ((_sv0t8 != 22)) {
    return 9;
  } else {
  }
  int _sv0t9 = op_fallback_tag(61);
  if ((_sv0t9 != 35)) {
    return 10;
  } else {
  }
  int _sv0t10 = op_fallback_tag(65);
  if ((_sv0t10 != 0)) {
    return 11;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_char_class();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_keyword_lookup();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (100 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_escape();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (120 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_state();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (130 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_skip_ws();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (140 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_skip_line_comment();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (150 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_skip_block_comment();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (160 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_skip_all();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (170 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_scan_ident();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (180 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_scan_digits();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (190 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_op_tags();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (200 + r11);
    return _sv0t20;
  } else {
  }
  return 0;
}

