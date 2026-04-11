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
static int severity_ord(Severity a, Severity b);
static int test_severity_tags(void);
static int test_constructors(void);
static int test_has_errors(void);
static int test_error_count(void);
static int test_severity_ord(void);

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
  return 0;
}

