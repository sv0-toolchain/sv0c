#include "sv0_runtime.h"

static int path_ok(int path_len, int has_dotdot, int starts_slash);
static int parse_include_line(int has_prefix, int has_closing_quote, int rest_is_semicolon, int path_is_ok);
static int expand_includes_count(int line_count, int include_count);
static int is_cycle(int visited_count, int path_in_visited);

static int path_ok(int path_len, int has_dotdot, int starts_slash) {
  if ((path_len == 0)) {
    return 0;
  } else {
  }
  if ((has_dotdot == 1)) {
    return 0;
  } else {
  }
  if ((starts_slash == 1)) {
    return 0;
  } else {
  }
  return 1;
}

static int parse_include_line(int has_prefix, int has_closing_quote, int rest_is_semicolon, int path_is_ok) {
  if ((has_prefix == 1)) {
    if ((has_closing_quote == 1)) {
      if ((rest_is_semicolon == 1)) {
        if ((path_is_ok == 1)) {
          return 1;
        } else {
        }
      } else {
      }
    } else {
    }
  } else {
  }
  return 0;
}

static int expand_includes_count(int line_count, int include_count) {
  return include_count;
}

static int is_cycle(int visited_count, int path_in_visited) {
  if ((path_in_visited == 1)) {
    return 1;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = path_ok(5, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = path_ok(0, 0, 0);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = path_ok(5, 1, 0);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = path_ok(5, 0, 1);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = path_ok(5, 1, 1);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = parse_include_line(1, 1, 1, 1);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = parse_include_line(0, 1, 1, 1);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  int _sv0t7 = parse_include_line(1, 0, 1, 1);
  if ((_sv0t7 != 0)) {
    return 1;
  } else {
  }
  int _sv0t8 = parse_include_line(1, 1, 0, 1);
  if ((_sv0t8 != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = parse_include_line(1, 1, 1, 0);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  int _sv0t10 = expand_includes_count(10, 2);
  if ((_sv0t10 != 2)) {
    return 1;
  } else {
  }
  int _sv0t11 = expand_includes_count(5, 0);
  if ((_sv0t11 != 0)) {
    return 1;
  } else {
  }
  int _sv0t12 = is_cycle(3, 1);
  if ((_sv0t12 != 1)) {
    return 1;
  } else {
  }
  int _sv0t13 = is_cycle(3, 0);
  if ((_sv0t13 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

