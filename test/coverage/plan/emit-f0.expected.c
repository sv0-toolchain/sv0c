#include "sv0_runtime.h"

/* sv0cov coverage instrumentation, generated-C protocol 1 (sv0cov CV-113).
   Hits count module-local counters; the coverage runtime validates this
   module's registration before user entry. */
struct __sv0cov_fragment {
  const char *fragment_id;
  uint32_t slice_base;
  uint32_t slice_length;
};
struct __sv0cov_module {
  uint32_t protocol_major;
  const char *map_id;
  uint32_t program_counter_count;
  const char *target;
  const char *compiler_identity;
  uint32_t slice_base;
  uint32_t slice_length;
  uint32_t fragment_count;
  const struct __sv0cov_fragment *fragments;
};
void __sv0cov_start(const struct __sv0cov_module *const *modules, uint32_t module_count);
void __sv0cov_hit(const struct __sv0cov_module *module, uint32_t local_index);
static const struct __sv0cov_fragment __sv0cov_fragments[1] = {
  {"6d599e56e518833b373e55b1e8c222e77b7cdcc241f74d02a5a1a3a6f6b72971", 0u, 9u},
};
static const struct __sv0cov_module __sv0cov_module = {
  1u, "8e5dd7c230de96245d88c482a47196c3ccaf5c779dc88d6c270e7c7ed99e7b7c", 9u, "f0", "sv0c+test", 0u, 9u, 1u, __sv0cov_fragments
};
static const struct __sv0cov_module *const __sv0cov_modules[1] = {&__sv0cov_module};

typedef struct {
  int tag;
  int p0;
} Shape;

static int sv0u_classify(int sv0u_n);
static int sv0u_measure(Shape sv0u_s);

static int sv0u_classify(int sv0u_n) {
  __sv0cov_hit(&__sv0cov_module, 0u);
  int sv0u_total;
  sv0u_total = 0;
  if ((sv0u_n > 10)) {
    __sv0cov_hit(&__sv0cov_module, 1u);
    sv0u_total = (sv0u_total + 100);
  } else {
    __sv0cov_hit(&__sv0cov_module, 2u);
  }
  int sv0u_i;
  sv0u_i = 0;
  while (1) {
    if ((sv0u_i < sv0u_n)) {
      __sv0cov_hit(&__sv0cov_module, 3u);
    } else {
      __sv0cov_hit(&__sv0cov_module, 4u);
      break;
    }
    sv0u_total = (sv0u_total + sv0u_i);
    sv0u_i = (sv0u_i + 1);
  }
  return sv0u_total;
}

static int sv0u_measure(Shape sv0u_s) {
  __sv0cov_hit(&__sv0cov_module, 5u);
  int _sv0t0;
  _sv0t0 = 0;
  if ((sv0u_s.tag == 0)) {
    __sv0cov_hit(&__sv0cov_module, 6u);
    _sv0t0 = 0;
  } else {
    if ((sv0u_s.tag == 1)) {
      int sv0u_len = sv0u_s.p0;
      __sv0cov_hit(&__sv0cov_module, 7u);
      _sv0t0 = sv0u_len;
    } else {
    }
  }
  return _sv0t0;
}

static int sv0_user_main(void) {
  __sv0cov_hit(&__sv0cov_module, 8u);
  int sv0u_label;
  sv0u_label = sv0_str_lit("Größe", 7);
  int _sv0t0 = sv0u_classify(3);
  int sv0u_a;
  sv0u_a = _sv0t0;
  Shape _sv0t1;
  _sv0t1.tag = 1;
  _sv0t1.p0 = 2;
  int _sv0t2 = sv0u_measure(_sv0t1);
  int sv0u_b;
  sv0u_b = _sv0t2;
  int _sv0t3 = (sv0u_a + sv0u_b);
  int _sv0t4 = (_sv0t3 - 5);
  return _sv0t4;
}

int main(int argc, char **argv) {
  sv0_runtime_init(argc, argv);
  __sv0cov_start(__sv0cov_modules, 1u);
  return (int)sv0_user_main();
}

