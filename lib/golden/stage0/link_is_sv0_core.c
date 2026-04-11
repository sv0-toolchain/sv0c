#include "sv0_runtime.h"

static int is_sv0(const char* name);

static int is_sv0(const char* name) {
  int _sv0t0 = sv0_string_len(name);
  int len = _sv0t0;
  if ((len < 4)) {
    return 0;
  } else {
  }
  int _sv0t1 = (len - 4);
  const char* _sv0t2 = sv0_string_substr(name, _sv0t1, 4);
  const char* suffix;
  suffix = _sv0t2;
  int _sv0t3 = sv0_string_eq(suffix, ".sv0");
  return _sv0t3;
}

int main(void) {
  int _sv0t0 = is_sv0("hello.sv0");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_sv0("parser.sv0");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_sv0("a.sv0");
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_sv0(".sv0");
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_sv0("hello.txt");
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_sv0("sv0");
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_sv0("foo");
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_sv0("ab");
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_sv0("");
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  int _sv0t9 = is_sv0("hello.sv0.bak");
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = is_sv0("file.SV0");
  if ((_sv0t10 != 0)) {
    return 11;
  } else {
  }
  return 0;
}

