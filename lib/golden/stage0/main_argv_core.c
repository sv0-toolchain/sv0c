#include "sv0_runtime.h"

static int mode_from_args(int arg_count, int first_arg_tag);
static int is_vm_mode(int mode);
static int is_project_mode(int mode);
static int needs_output_path(int mode);
static int project_entry_stem(int has_main_sv0);
static int pipeline_step_count(int mode);

static int mode_from_args(int arg_count, int first_arg_tag) {
  if ((arg_count == 0)) {
    int _sv0t0 = (-1);
    return _sv0t0;
  } else {
  }
  if ((arg_count == 1)) {
    if ((first_arg_tag == 0)) {
      return 0;
    } else {
    }
    if ((first_arg_tag == 1)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((arg_count == 2)) {
    if ((first_arg_tag == 2)) {
      return 2;
    } else {
    }
    if ((first_arg_tag == 3)) {
      return 3;
    } else {
    }
    if ((first_arg_tag == 4)) {
      return 4;
    } else {
    }
    return 0;
  } else {
  }
  int _sv0t1 = (-1);
  return _sv0t1;
}

static int is_vm_mode(int mode) {
  int _sv0t0 = (mode == 2);
  int _sv0t1 = (mode == 3);
  int _sv0t2 = (_sv0t0 || _sv0t1);
  return _sv0t2;
}

static int is_project_mode(int mode) {
  int _sv0t0 = (mode == 1);
  int _sv0t1 = (mode == 3);
  int _sv0t2 = (_sv0t0 || _sv0t1);
  return _sv0t2;
}

static int needs_output_path(int mode) {
  int _sv0t0 = (mode >= 0);
  int _sv0t1 = (mode <= 4);
  int _sv0t2 = (_sv0t0 && _sv0t1);
  return _sv0t2;
}

static int project_entry_stem(int has_main_sv0) {
  if (has_main_sv0) {
    return 0;
  } else {
  }
  return 1;
}

static int pipeline_step_count(int mode) {
  if ((mode == 0)) {
    return 7;
  } else {
  }
  if ((mode == 1)) {
    return 8;
  } else {
  }
  if ((mode == 2)) {
    return 6;
  } else {
  }
  if ((mode == 3)) {
    return 7;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = mode_from_args(0, 0);
  int _sv0t1 = (-1);
  if ((_sv0t0 != _sv0t1)) {
    return 1;
  } else {
  }
  int _sv0t2 = mode_from_args(1, 0);
  if ((_sv0t2 != 0)) {
    return 2;
  } else {
  }
  int _sv0t3 = mode_from_args(1, 1);
  if ((_sv0t3 != 1)) {
    return 3;
  } else {
  }
  int _sv0t4 = mode_from_args(2, 2);
  if ((_sv0t4 != 2)) {
    return 4;
  } else {
  }
  int _sv0t5 = mode_from_args(2, 3);
  if ((_sv0t5 != 3)) {
    return 5;
  } else {
  }
  int _sv0t6 = is_vm_mode(2);
  if ((!_sv0t6)) {
    return 6;
  } else {
  }
  int _sv0t7 = is_vm_mode(3);
  if ((!_sv0t7)) {
    return 7;
  } else {
  }
  int _sv0t8 = is_vm_mode(0);
  if (_sv0t8) {
    return 8;
  } else {
  }
  int _sv0t9 = is_vm_mode(1);
  if (_sv0t9) {
    return 9;
  } else {
  }
  int _sv0t10 = is_project_mode(1);
  if ((!_sv0t10)) {
    return 10;
  } else {
  }
  int _sv0t11 = is_project_mode(3);
  if ((!_sv0t11)) {
    return 11;
  } else {
  }
  int _sv0t12 = is_project_mode(0);
  if (_sv0t12) {
    return 12;
  } else {
  }
  int _sv0t13 = project_entry_stem(1);
  if ((_sv0t13 != 0)) {
    return 13;
  } else {
  }
  int _sv0t14 = project_entry_stem(0);
  if ((_sv0t14 != 1)) {
    return 14;
  } else {
  }
  int _sv0t15 = pipeline_step_count(0);
  if ((_sv0t15 != 7)) {
    return 15;
  } else {
  }
  int _sv0t16 = pipeline_step_count(2);
  if ((_sv0t16 != 6)) {
    return 16;
  } else {
  }
  return 0;
}

