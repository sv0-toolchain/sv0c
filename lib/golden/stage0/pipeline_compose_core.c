#include "sv0_runtime.h"

static int pipeline_run(int pass_count, int fail_at);
static int pipeline_c_ok(int fail_at);
static int pipeline_vm_ok(int fail_at);
static int error_category(int fail_pass);
static int output_kind(int target_tag, int success);

static int pipeline_run(int pass_count, int fail_at) {
  if ((fail_at >= 0)) {
    if ((fail_at < pass_count)) {
      return fail_at;
    } else {
    }
  } else {
  }
  return 255;
}

static int pipeline_c_ok(int fail_at) {
  int _sv0t0 = pipeline_run(7, fail_at);
  if ((_sv0t0 == 255)) {
    return 1;
  } else {
  }
  return 0;
}

static int pipeline_vm_ok(int fail_at) {
  int _sv0t0 = pipeline_run(8, fail_at);
  if ((_sv0t0 == 255)) {
    return 1;
  } else {
  }
  return 0;
}

static int error_category(int fail_pass) {
  if ((fail_pass == 0)) {
    return 100;
  } else {
  }
  if ((fail_pass == 1)) {
    return 200;
  } else {
  }
  if ((fail_pass == 2)) {
    return 400;
  } else {
  }
  if ((fail_pass == 3)) {
    return 500;
  } else {
  }
  if ((fail_pass == 4)) {
    return 600;
  } else {
  }
  if ((fail_pass == 5)) {
    return 700;
  } else {
  }
  if ((fail_pass == 6)) {
    return 800;
  } else {
  }
  if ((fail_pass == 7)) {
    return 900;
  } else {
  }
  return 0;
}

static int output_kind(int target_tag, int success) {
  if ((success == 0)) {
    return 0;
  } else {
  }
  if ((target_tag == 0)) {
    return 1;
  } else {
  }
  if ((target_tag == 1)) {
    return 2;
  } else {
  }
  return 255;
}

int main(void) {
  int _sv0t0 = pipeline_run(7, 255);
  if ((_sv0t0 != 255)) {
    return 1;
  } else {
  }
  int _sv0t1 = pipeline_run(7, 2);
  if ((_sv0t1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t2 = pipeline_run(7, 10);
  if ((_sv0t2 != 255)) {
    return 1;
  } else {
  }
  int _sv0t3 = pipeline_c_ok(255);
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = pipeline_c_ok(3);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = pipeline_vm_ok(255);
  if ((_sv0t5 != 1)) {
    return 1;
  } else {
  }
  int _sv0t6 = pipeline_vm_ok(7);
  if ((_sv0t6 != 0)) {
    return 1;
  } else {
  }
  int _sv0t7 = error_category(0);
  if ((_sv0t7 != 100)) {
    return 1;
  } else {
  }
  int _sv0t8 = error_category(2);
  if ((_sv0t8 != 400)) {
    return 1;
  } else {
  }
  int _sv0t9 = error_category(5);
  if ((_sv0t9 != 700)) {
    return 1;
  } else {
  }
  int _sv0t10 = output_kind(0, 1);
  if ((_sv0t10 != 1)) {
    return 1;
  } else {
  }
  int _sv0t11 = output_kind(1, 1);
  if ((_sv0t11 != 2)) {
    return 1;
  } else {
  }
  int _sv0t12 = output_kind(0, 0);
  if ((_sv0t12 != 0)) {
    return 1;
  } else {
  }
  return 0;
}

