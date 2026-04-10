#include "sv0_runtime.h"

static int pipeline_pass_count(int target_tag);
static int base_name_strips(int name_len, int ends_sv0);
static int cli_dispatch(int arg_count, int has_target_vm, int has_project);
static int project_vm_entry_stem(int main_exists);

static int pipeline_pass_count(int target_tag) {
  if ((target_tag == 0)) {
    return 8;
  } else {
  }
  if ((target_tag == 1)) {
    return 8;
  } else {
  }
  return 255;
}

static int base_name_strips(int name_len, int ends_sv0) {
  if ((name_len > 4)) {
    if ((ends_sv0 == 1)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int cli_dispatch(int arg_count, int has_target_vm, int has_project) {
  if ((arg_count == 2)) {
    if ((has_target_vm == 1)) {
      return 0;
    } else {
    }
  } else {
  }
  if ((arg_count == 3)) {
    if ((has_target_vm == 1)) {
      if ((has_project == 1)) {
        return 1;
      } else {
      }
    } else {
    }
    if ((has_project == 1)) {
      if ((has_target_vm == 1)) {
        return 2;
      } else {
      }
    } else {
    }
  } else {
  }
  if ((arg_count == 2)) {
    if ((has_project == 1)) {
      return 3;
    } else {
    }
  } else {
  }
  if ((arg_count == 1)) {
    return 4;
  } else {
  }
  return 5;
}

static int project_vm_entry_stem(int main_exists) {
  if ((main_exists == 1)) {
    return 0;
  } else {
  }
  return 1;
}

int main(void) {
  int _sv0t0 = pipeline_pass_count(0);
  if ((_sv0t0 != 8)) {
    return 1;
  } else {
  }
  int _sv0t1 = pipeline_pass_count(1);
  if ((_sv0t1 != 8)) {
    return 1;
  } else {
  }
  int _sv0t2 = pipeline_pass_count(2);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = base_name_strips(8, 1);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = base_name_strips(4, 1);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = base_name_strips(8, 0);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = base_name_strips(3, 1);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  int _sv0t7 = cli_dispatch(2, 1, 0);
  if ((_sv0t7 != 0)) {
    return 1;
  } else {
  }
  int _sv0t8 = cli_dispatch(3, 1, 1);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = cli_dispatch(2, 0, 1);
  if ((_sv0t9 != 3)) {
    return 1;
  } else {
  }
  int _sv0t10 = cli_dispatch(1, 0, 0);
  if ((_sv0t10 != 4)) {
    return 1;
  } else {
  }
  int _sv0t11 = cli_dispatch(0, 0, 0);
  if ((_sv0t11 != 5)) {
    return 1;
  } else {
  }
  int _sv0t12 = project_vm_entry_stem(1);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  int _sv0t13 = project_vm_entry_stem(0);
  if ((_sv0t13 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

