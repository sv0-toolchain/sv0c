#include "sv0_runtime.h"

static int match_arm_setup_total_len(int pat_kind, int scr_is_int, int tuple_bind_count);

static int match_arm_setup_total_len(int pat_kind, int scr_is_int, int tuple_bind_count) {
  int cpre_len = 0;
  int cond_len = 1;
  if ((pat_kind == 0)) {
    int _sv0t0 = (cpre_len + cond_len);
    int _sv0t1 = (_sv0t0 + 0);
    return _sv0t1;
  } else {
  }
  if ((pat_kind == 1)) {
    if ((scr_is_int < 0)) {
      return 255;
    } else {
    }
    if ((scr_is_int > 1)) {
      return 255;
    } else {
    }
    if ((scr_is_int == 1)) {
      int _sv0t2 = (cpre_len + cond_len);
      int _sv0t3 = (_sv0t2 + 1);
      return _sv0t3;
    } else {
    }
    int _sv0t4 = (cpre_len + cond_len);
    int _sv0t5 = (_sv0t4 + 2);
    return _sv0t5;
  } else {
  }
  if ((pat_kind == 2)) {
    int _sv0t6 = (cpre_len + cond_len);
    int _sv0t7 = (_sv0t6 + 0);
    return _sv0t7;
  } else {
  }
  if ((pat_kind == 3)) {
    int _sv0t8 = (cpre_len + cond_len);
    int _sv0t9 = (_sv0t8 + 0);
    return _sv0t9;
  } else {
  }
  if ((pat_kind == 4)) {
    int _sv0t10 = (cpre_len + cond_len);
    int _sv0t11 = (_sv0t10 + 0);
    return _sv0t11;
  } else {
  }
  if ((pat_kind == 5)) {
    if ((tuple_bind_count != 0)) {
      return 255;
    } else {
    }
    int _sv0t12 = (cpre_len + cond_len);
    int _sv0t13 = (_sv0t12 + 0);
    return _sv0t13;
  } else {
  }
  if ((pat_kind == 6)) {
    if ((tuple_bind_count < 0)) {
      return 255;
    } else {
    }
    int _sv0t14 = (cpre_len + cond_len);
    int _sv0t15 = (_sv0t14 + tuple_bind_count);
    return _sv0t15;
  } else {
  }
  if ((pat_kind == 7)) {
    return 255;
  } else {
  }
  if ((pat_kind == 8)) {
    return 254;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = match_arm_setup_total_len(0, 0, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = match_arm_setup_total_len(1, 1, 0);
  if ((_sv0t1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t2 = match_arm_setup_total_len(1, 0, 0);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = match_arm_setup_total_len(2, 0, 0);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = match_arm_setup_total_len(5, 0, 0);
  if ((_sv0t4 != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = match_arm_setup_total_len(6, 0, 3);
  if ((_sv0t5 != 4)) {
    return 1;
  } else {
  }
  int _sv0t6 = match_arm_setup_total_len(5, 0, 1);
  if ((_sv0t6 != 255)) {
    return 1;
  } else {
  }
  int _sv0t7 = match_arm_setup_total_len(7, 0, 0);
  if ((_sv0t7 != 255)) {
    return 1;
  } else {
  }
  int _sv0t8 = match_arm_setup_total_len(8, 0, 0);
  if ((_sv0t8 != 254)) {
    return 1;
  } else {
  }
  int _sv0t9 = match_arm_setup_total_len(9, 0, 0);
  if ((_sv0t9 != 255)) {
    return 1;
  } else {
  }
  int _sv0t10 = (-1);
  int _sv0t11 = match_arm_setup_total_len(1, _sv0t10, 0);
  if ((_sv0t11 != 255)) {
    return 1;
  } else {
  }
  int _sv0t12 = (-1);
  int _sv0t13 = match_arm_setup_total_len(6, 0, _sv0t12);
  if ((_sv0t13 != 255)) {
    return 1;
  } else {
  }
  return 0;
}

