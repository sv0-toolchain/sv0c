#include "sv0_runtime.h"

static int needs_escape(int c);
static int escape_char_tag(int c);
static int escaped_len(int c);
static int total_escaped_len(int c0, int c1, int c2, int c3);
static int is_printable_ascii(int c);
static int test_needs_escape(void);
static int test_escape_tag(void);
static int test_escaped_len(void);
static int test_total_len(void);
static int test_printable(void);

static int needs_escape(int c) {
  if ((c == 34)) {
    return 1;
  } else {
  }
  if ((c == 92)) {
    return 1;
  } else {
  }
  if ((c == 10)) {
    return 1;
  } else {
  }
  if ((c == 9)) {
    return 1;
  } else {
  }
  if ((c == 13)) {
    return 1;
  } else {
  }
  if ((c < 32)) {
    return 1;
  } else {
  }
  return 0;
}

static int escape_char_tag(int c) {
  if ((c == 34)) {
    return 1;
  } else {
  }
  if ((c == 92)) {
    return 2;
  } else {
  }
  if ((c == 10)) {
    return 3;
  } else {
  }
  if ((c == 9)) {
    return 4;
  } else {
  }
  if ((c == 13)) {
    return 5;
  } else {
  }
  if ((c < 32)) {
    return 6;
  } else {
  }
  return 0;
}

static int escaped_len(int c) {
  int _sv0t0 = escape_char_tag(c);
  int tag = _sv0t0;
  if ((tag == 0)) {
    return 1;
  } else {
  }
  if ((tag == 1)) {
    return 2;
  } else {
  }
  if ((tag == 2)) {
    return 2;
  } else {
  }
  if ((tag == 3)) {
    return 2;
  } else {
  }
  if ((tag == 4)) {
    return 2;
  } else {
  }
  if ((tag == 5)) {
    return 2;
  } else {
  }
  if ((tag == 6)) {
    if ((c < 10)) {
      return 2;
    } else {
    }
    return 3;
  } else {
  }
  return 1;
}

static int total_escaped_len(int c0, int c1, int c2, int c3) {
  int len = 0;
  if ((c0 >= 0)) {
    int _sv0t0 = escaped_len(c0);
    len = (len + _sv0t0);
  } else {
  }
  if ((c1 >= 0)) {
    int _sv0t1 = escaped_len(c1);
    len = (len + _sv0t1);
  } else {
  }
  if ((c2 >= 0)) {
    int _sv0t2 = escaped_len(c2);
    len = (len + _sv0t2);
  } else {
  }
  if ((c3 >= 0)) {
    int _sv0t3 = escaped_len(c3);
    len = (len + _sv0t3);
  } else {
  }
  return len;
}

static int is_printable_ascii(int c) {
  if ((c < 32)) {
    return 0;
  } else {
  }
  if ((c > 126)) {
    return 0;
  } else {
  }
  return 1;
}

static int test_needs_escape(void) {
  int _sv0t0 = needs_escape(34);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = needs_escape(92);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = needs_escape(10);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = needs_escape(9);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = needs_escape(13);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = needs_escape(0);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = needs_escape(65);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = needs_escape(48);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = needs_escape(32);
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_escape_tag(void) {
  int _sv0t0 = escape_char_tag(34);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = escape_char_tag(92);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = escape_char_tag(10);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = escape_char_tag(9);
  if ((_sv0t3 != 4)) {
    return 4;
  } else {
  }
  int _sv0t4 = escape_char_tag(13);
  if ((_sv0t4 != 5)) {
    return 5;
  } else {
  }
  int _sv0t5 = escape_char_tag(1);
  if ((_sv0t5 != 6)) {
    return 6;
  } else {
  }
  int _sv0t6 = escape_char_tag(65);
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_escaped_len(void) {
  int _sv0t0 = escaped_len(65);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = escaped_len(34);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = escaped_len(10);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = escaped_len(1);
  if ((_sv0t3 != 2)) {
    return 4;
  } else {
  }
  int _sv0t4 = escaped_len(15);
  if ((_sv0t4 != 3)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_total_len(void) {
  int _sv0t0 = (0 - 1);
  int _sv0t1 = total_escaped_len(65, 66, 67, _sv0t0);
  int t1 = _sv0t1;
  if ((t1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t2 = (0 - 1);
  int _sv0t3 = total_escaped_len(34, 65, 10, _sv0t2);
  int t2 = _sv0t3;
  if ((t2 != 5)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_printable(void) {
  int _sv0t0 = is_printable_ascii(65);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_printable_ascii(32);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_printable_ascii(126);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_printable_ascii(31);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_printable_ascii(127);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_needs_escape();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_escape_tag();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_escaped_len();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_total_len();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_printable();
  int r5 = _sv0t8;
  if ((r5 != 0)) {
    int _sv0t9 = (500 + r5);
    return _sv0t9;
  } else {
  }
  return 0;
}

