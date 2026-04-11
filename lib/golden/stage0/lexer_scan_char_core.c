#include "sv0_runtime.h"

static int char_lit_is_escape(int c);
static int char_lit_value(int is_escape, int plain_char, int escape_char);
static int char_lit_chars_consumed(int is_escape, int escape_char);
static int char_lit_has_closing_quote(int after_char);
static int char_lit_token_tag(void);

static int char_lit_is_escape(int c) {
  int _sv0t0 = (c == 92);
  return _sv0t0;
}

static int char_lit_value(int is_escape, int plain_char, int escape_char) {
  if (is_escape) {
    if ((escape_char == 110)) {
      return 10;
    } else {
    }
    if ((escape_char == 116)) {
      return 9;
    } else {
    }
    if ((escape_char == 114)) {
      return 13;
    } else {
    }
    if ((escape_char == 92)) {
      return 92;
    } else {
    }
    if ((escape_char == 39)) {
      return 39;
    } else {
    }
    if ((escape_char == 48)) {
      return 0;
    } else {
    }
    return escape_char;
  } else {
  }
  return plain_char;
}

static int char_lit_chars_consumed(int is_escape, int escape_char) {
  if (is_escape) {
    if ((escape_char == 120)) {
      return 6;
    } else {
    }
    return 4;
  } else {
  }
  return 3;
}

static int char_lit_has_closing_quote(int after_char) {
  int _sv0t0 = (after_char == 39);
  return _sv0t0;
}

static int char_lit_token_tag(void) {
  return 3;
}

int main(void) {
  int _sv0t0 = char_lit_value(0, 97, 0);
  int v1 = _sv0t0;
  if ((v1 != 97)) {
    return 1;
  } else {
  }
  int _sv0t1 = char_lit_value(1, 0, 110);
  int v2 = _sv0t1;
  if ((v2 != 10)) {
    return 2;
  } else {
  }
  int _sv0t2 = char_lit_value(1, 0, 116);
  int v3 = _sv0t2;
  if ((v3 != 9)) {
    return 3;
  } else {
  }
  int _sv0t3 = char_lit_value(1, 0, 92);
  int v4 = _sv0t3;
  if ((v4 != 92)) {
    return 4;
  } else {
  }
  int _sv0t4 = char_lit_value(1, 0, 48);
  int v5 = _sv0t4;
  if ((v5 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = char_lit_is_escape(92);
  if ((!_sv0t5)) {
    return 10;
  } else {
  }
  int _sv0t6 = char_lit_is_escape(97);
  if (_sv0t6) {
    return 11;
  } else {
  }
  int _sv0t7 = char_lit_chars_consumed(0, 0);
  if ((_sv0t7 != 3)) {
    return 20;
  } else {
  }
  int _sv0t8 = char_lit_chars_consumed(1, 110);
  if ((_sv0t8 != 4)) {
    return 21;
  } else {
  }
  int _sv0t9 = char_lit_chars_consumed(1, 120);
  if ((_sv0t9 != 6)) {
    return 22;
  } else {
  }
  int _sv0t10 = char_lit_has_closing_quote(39);
  if ((!_sv0t10)) {
    return 30;
  } else {
  }
  int _sv0t11 = char_lit_has_closing_quote(97);
  if (_sv0t11) {
    return 31;
  } else {
  }
  int _sv0t12 = char_lit_token_tag();
  if ((_sv0t12 != 3)) {
    return 40;
  } else {
  }
  return 0;
}

