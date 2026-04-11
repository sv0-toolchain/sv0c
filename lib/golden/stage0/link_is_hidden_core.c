#include "sv0_runtime.h"

static int is_hidden(const char* name);

static int is_hidden(const char* name) {
  int _sv0t0 = sv0_string_eq(name, ".");
  if (_sv0t0) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "..");
  if (_sv0t1) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_string_len(name);
  int len = _sv0t2;
  if ((len == 0)) {
    return 0;
  } else {
  }
  int _sv0t3 = sv0_string_char_at(name, 0);
  int first = _sv0t3;
  if ((first == 46)) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = is_hidden(".");
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_hidden("..");
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_hidden(".gitignore");
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_hidden(".hidden");
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_hidden(".a");
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_hidden("README.md");
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_hidden("main.sv0");
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_hidden("a");
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_hidden("");
  if ((_sv0t8 != 0)) {
    return 9;
  } else {
  }
  return 0;
}

