#include "sv0_runtime.h"

static int tag_delim_lparen(void);
static int tag_delim_rparen(void);
static int tag_delim_lbrace(void);
static int tag_delim_rbrace(void);
static int tag_delim_comma(void);
static int tag_delim_semi(void);
static int tag_delim_lbracket(void);
static int tag_delim_rbracket(void);
static int is_delimiter_tag(int t);

static int tag_delim_lparen(void) {
  return 10;
}

static int tag_delim_rparen(void) {
  return 11;
}

static int tag_delim_lbrace(void) {
  return 12;
}

static int tag_delim_rbrace(void) {
  return 13;
}

static int tag_delim_comma(void) {
  return 14;
}

static int tag_delim_semi(void) {
  return 15;
}

static int tag_delim_lbracket(void) {
  return 16;
}

static int tag_delim_rbracket(void) {
  return 17;
}

static int is_delimiter_tag(int t) {
  int _sv0t0;
  int _sv0t1;
  if ((t == 10)) {
    return 1;
    _sv0t1 = 0;
  } else {
    int _sv0t2;
    if ((t == 11)) {
      return 1;
      _sv0t2 = 0;
    } else {
      int _sv0t3;
      if ((t == 12)) {
        return 1;
        _sv0t3 = 0;
      } else {
        int _sv0t4;
        if ((t == 13)) {
          return 1;
          _sv0t4 = 0;
        } else {
          int _sv0t5;
          if ((t == 14)) {
            return 1;
            _sv0t5 = 0;
          } else {
            int _sv0t6;
            if ((t == 15)) {
              return 1;
              _sv0t6 = 0;
            } else {
              int _sv0t7;
              if ((t == 16)) {
                return 1;
                _sv0t7 = 0;
              } else {
                int _sv0t8;
                if ((t == 17)) {
                  return 1;
                  _sv0t8 = 0;
                } else {
                  return 0;
                  _sv0t8 = 0;
                }
                _sv0t7 = _sv0t8;
              }
              _sv0t6 = _sv0t7;
            }
            _sv0t5 = _sv0t6;
          }
          _sv0t4 = _sv0t5;
        }
        _sv0t3 = _sv0t4;
      }
      _sv0t2 = _sv0t3;
    }
    _sv0t1 = _sv0t2;
  }
  _sv0t0 = _sv0t1;
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = tag_delim_lparen();
  int _sv0t1 = is_delimiter_tag(_sv0t0);
  int ok_lp = (_sv0t1 - 1);
  int _sv0t2 = tag_delim_rparen();
  int _sv0t3 = is_delimiter_tag(_sv0t2);
  int ok_rp = (_sv0t3 - 1);
  int _sv0t4 = tag_delim_lbrace();
  int _sv0t5 = is_delimiter_tag(_sv0t4);
  int ok_lb = (_sv0t5 - 1);
  int _sv0t6 = tag_delim_rbrace();
  int _sv0t7 = is_delimiter_tag(_sv0t6);
  int ok_rb = (_sv0t7 - 1);
  int _sv0t8 = tag_delim_comma();
  int _sv0t9 = is_delimiter_tag(_sv0t8);
  int ok_cm = (_sv0t9 - 1);
  int _sv0t10 = tag_delim_semi();
  int _sv0t11 = is_delimiter_tag(_sv0t10);
  int ok_sm = (_sv0t11 - 1);
  int _sv0t12 = tag_delim_lbracket();
  int _sv0t13 = is_delimiter_tag(_sv0t12);
  int ok_lbk = (_sv0t13 - 1);
  int _sv0t14 = tag_delim_rbracket();
  int _sv0t15 = is_delimiter_tag(_sv0t14);
  int ok_rbk = (_sv0t15 - 1);
  int _sv0t16 = is_delimiter_tag(3);
  int ok_non = _sv0t16;
  int _sv0t17 = is_delimiter_tag(1);
  int ok_kw = _sv0t17;
  int _sv0t18 = (ok_lp + ok_rp);
  int _sv0t19 = (_sv0t18 + ok_lb);
  int _sv0t20 = (_sv0t19 + ok_rb);
  int _sv0t21 = (_sv0t20 + ok_cm);
  int _sv0t22 = (_sv0t21 + ok_sm);
  int _sv0t23 = (_sv0t22 + ok_lbk);
  int _sv0t24 = (_sv0t23 + ok_rbk);
  int _sv0t25 = (_sv0t24 + ok_non);
  int _sv0t26 = (_sv0t25 + ok_kw);
  return _sv0t26;
}

