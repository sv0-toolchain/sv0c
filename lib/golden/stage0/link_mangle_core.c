#include "sv0_runtime.h"

static int mangle_name(int mod_id, int name);
static int is_in_tops(int name, int t0, int t1, int t2, int t3);
static int map_path_seg(int name, int mod_id, int t0, int t1, int t2, int t3);
static int map_one_seg_path(int name, int mod_id, int t0, int t1, int t2, int t3);
static int map_two_seg_path(int seg0, int seg1, int mod_id, int t0, int t1, int t2, int t3);
static int collect_tops_count(int item0_kind, int item1_kind, int item2_kind);
static int file_stem_len(const char* path);
static int test_mangle(void);
static int test_in_tops(void);
static int test_map_path(void);
static int test_collect_tops(void);
static int test_file_stem(void);

static int mangle_name(int mod_id, int name) {
  int _sv0t0 = (mod_id * 10000);
  int _sv0t1 = (_sv0t0 + name);
  return _sv0t1;
}

static int is_in_tops(int name, int t0, int t1, int t2, int t3) {
  if ((name == t0)) {
    return 1;
  } else {
  }
  if ((name == t1)) {
    return 1;
  } else {
  }
  if ((name == t2)) {
    return 1;
  } else {
  }
  if ((name == t3)) {
    return 1;
  } else {
  }
  return 0;
}

static int map_path_seg(int name, int mod_id, int t0, int t1, int t2, int t3) {
  int _sv0t0 = is_in_tops(name, t0, t1, t2, t3);
  if (_sv0t0) {
    int _sv0t1 = mangle_name(mod_id, name);
    return _sv0t1;
  } else {
  }
  return name;
}

static int map_one_seg_path(int name, int mod_id, int t0, int t1, int t2, int t3) {
  int _sv0t0 = map_path_seg(name, mod_id, t0, t1, t2, t3);
  return _sv0t0;
}

static int map_two_seg_path(int seg0, int seg1, int mod_id, int t0, int t1, int t2, int t3) {
  int _sv0t0 = map_path_seg(seg0, mod_id, t0, t1, t2, t3);
  int new_seg0 = _sv0t0;
  int _sv0t1 = (new_seg0 * 10000);
  int _sv0t2 = (_sv0t1 + seg1);
  return _sv0t2;
}

static int collect_tops_count(int item0_kind, int item1_kind, int item2_kind) {
  int count = 0;
  if ((item0_kind <= 4)) {
    count = (count + 1);
  } else {
  }
  if ((item1_kind <= 4)) {
    count = (count + 1);
  } else {
  }
  if ((item2_kind <= 4)) {
    count = (count + 1);
  } else {
  }
  return count;
}

static int file_stem_len(const char* path) {
  int _sv0t0 = sv0_string_len(path);
  int n = _sv0t0;
  if ((n > 4)) {
    int _sv0t1 = (n - 4);
    const char* _sv0t2 = sv0_string_substr(path, _sv0t1, 4);
    const char* suffix;
    suffix = _sv0t2;
    int _sv0t3 = sv0_string_eq(suffix, ".sv0");
    if (_sv0t3) {
      int _sv0t4 = (n - 4);
      return _sv0t4;
    } else {
    }
  } else {
  }
  return n;
}

static int test_mangle(void) {
  int _sv0t0 = mangle_name(1, 5);
  if ((_sv0t0 != 10005)) {
    return 1;
  } else {
  }
  int _sv0t1 = mangle_name(2, 10);
  if ((_sv0t1 != 20010)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_in_tops(void) {
  int _sv0t0 = is_in_tops(1, 1, 2, 3, 4);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_in_tops(4, 1, 2, 3, 4);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_in_tops(5, 1, 2, 3, 4);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_map_path(void) {
  int _sv0t0 = map_path_seg(1, 10, 1, 2, 3, 4);
  if ((_sv0t0 != 100001)) {
    return 1;
  } else {
  }
  int _sv0t1 = map_path_seg(5, 10, 1, 2, 3, 4);
  if ((_sv0t1 != 5)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_collect_tops(void) {
  int _sv0t0 = collect_tops_count(0, 1, 2);
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = collect_tops_count(0, 5, 6);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = collect_tops_count(5, 6, 7);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_file_stem(void) {
  int _sv0t0 = file_stem_len("test.sv0");
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = file_stem_len("abc.sv0");
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = file_stem_len("noext");
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_mangle();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    int _sv0t1 = (100 + r1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_in_tops();
  int r2 = _sv0t2;
  if ((r2 != 0)) {
    int _sv0t3 = (200 + r2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_map_path();
  int r3 = _sv0t4;
  if ((r3 != 0)) {
    int _sv0t5 = (300 + r3);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_collect_tops();
  int r4 = _sv0t6;
  if ((r4 != 0)) {
    int _sv0t7 = (400 + r4);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_file_stem();
  int r5 = _sv0t8;
  if ((r5 != 0)) {
    int _sv0t9 = (500 + r5);
    return _sv0t9;
  } else {
  }
  return 0;
}

