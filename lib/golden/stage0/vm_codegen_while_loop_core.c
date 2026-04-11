#include "sv0_runtime.h"

static int loop_header_insns(void);
static int loop_exit_jump_size(void);
static int loop_back_edge_size(void);
static int loop_total_overhead(void);
static int loop_cond_offset(int header_insns);
static int loop_body_offset(int header_insns, int cond_size);
static int loop_back_edge_offset(int header_insns, int cond_size, int body_size);
static int loop_exit_target(int header_insns, int cond_size, int body_size);
static int back_edge_delta(int exit_target, int cond_offset);
static int exit_jump_delta(int exit_target, int body_offset);

static int loop_header_insns(void) {
  return 0;
}

static int loop_exit_jump_size(void) {
  return 5;
}

static int loop_back_edge_size(void) {
  return 5;
}

static int loop_total_overhead(void) {
  int _sv0t0 = loop_exit_jump_size();
  int _sv0t1 = loop_back_edge_size();
  int _sv0t2 = (_sv0t0 + _sv0t1);
  return _sv0t2;
}

static int loop_cond_offset(int header_insns) {
  return header_insns;
}

static int loop_body_offset(int header_insns, int cond_size) {
  int _sv0t0 = (header_insns + cond_size);
  int _sv0t1 = loop_exit_jump_size();
  int _sv0t2 = (_sv0t0 + _sv0t1);
  return _sv0t2;
}

static int loop_back_edge_offset(int header_insns, int cond_size, int body_size) {
  int _sv0t0 = loop_body_offset(header_insns, cond_size);
  int _sv0t1 = (_sv0t0 + body_size);
  return _sv0t1;
}

static int loop_exit_target(int header_insns, int cond_size, int body_size) {
  int _sv0t0 = loop_back_edge_offset(header_insns, cond_size, body_size);
  int _sv0t1 = loop_back_edge_size();
  int _sv0t2 = (_sv0t0 + _sv0t1);
  return _sv0t2;
}

static int back_edge_delta(int exit_target, int cond_offset) {
  int _sv0t0 = (cond_offset - exit_target);
  return _sv0t0;
}

static int exit_jump_delta(int exit_target, int body_offset) {
  int _sv0t0 = (exit_target - body_offset);
  return _sv0t0;
}

int main(void) {
  int _sv0t0 = loop_total_overhead();
  if ((_sv0t0 != 10)) {
    return 1;
  } else {
  }
  int cond = 3;
  int body = 8;
  int _sv0t1 = loop_body_offset(0, cond);
  int body_off = _sv0t1;
  if ((body_off != 8)) {
    return 2;
  } else {
  }
  int _sv0t2 = loop_back_edge_offset(0, cond, body);
  int back_off = _sv0t2;
  if ((back_off != 16)) {
    return 3;
  } else {
  }
  int _sv0t3 = loop_exit_target(0, cond, body);
  int exit = _sv0t3;
  if ((exit != 21)) {
    return 4;
  } else {
  }
  int _sv0t4 = back_edge_delta(exit, 0);
  int back = _sv0t4;
  int _sv0t5 = (-21);
  if ((back != _sv0t5)) {
    return 5;
  } else {
  }
  int _sv0t6 = exit_jump_delta(exit, body_off);
  int exit_d = _sv0t6;
  if ((exit_d != 13)) {
    return 6;
  } else {
  }
  return 0;
}

