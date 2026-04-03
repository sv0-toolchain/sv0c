#include "sv0_runtime.h"

typedef struct {
  int sev_tag;
  int code_nonempty;
  int msg_len;
  int line;
  int col;
  int related_len;
  int help_len;
} Diagnostic;
typedef struct {
  int tag;
} Severity;

static int is_error_sev(Severity s);

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
  Diagnostic e;
  e.sev_tag = 0;
  e.code_nonempty = 1;
  e.msg_len = 24;
  e.line = 10;
  e.col = 3;
  e.related_len = 0;
  e.help_len = 1;
  Diagnostic w;
  w.sev_tag = 1;
  w.code_nonempty = 0;
  w.msg_len = 5;
  w.line = 20;
  w.col = 1;
  w.related_len = 2;
  w.help_len = 0;
  int tag_e = e.sev_tag;
  int _sv0t0;
  if ((tag_e == 0)) {
    _sv0t0 = 1;
  } else {
    if (1) {
      _sv0t0 = 0;
    } else {
    }
  }
  int c_e = _sv0t0;
  int tag_w = w.sev_tag;
  int _sv0t1;
  if ((tag_w == 0)) {
    _sv0t1 = 1;
  } else {
    if (1) {
      _sv0t1 = 0;
    } else {
    }
  }
  int c_w = _sv0t1;
  Severity _sv0t2;
  _sv0t2.tag = 1;
  int _sv0t3 = is_error_sev(_sv0t2);
  int warn_is_not_err = _sv0t3;
  int _sv0t4 = (c_e + c_w);
  int _sv0t5 = (_sv0t4 + warn_is_not_err);
  int _sv0t6 = (_sv0t5 + e.line);
  int _sv0t7 = (_sv0t6 + e.col);
  int _sv0t8 = (_sv0t7 + e.help_len);
  int _sv0t9 = (_sv0t8 - 15);
  return _sv0t9;
}

