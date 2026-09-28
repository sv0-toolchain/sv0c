/*
 * Test-only stand-in for the native coverage runtime (sv0c CV-113 tests).
 *
 * The real runtime (sv0cov runtime/c, CV-114) owns the atomic arena, the
 * transport, and the raw profile. This stub implements the same generated-C
 * protocol 1 interface so run_emit_c.py can link and run instrumented C:
 * __sv0cov_start validates the registration the way SPEC 14.1 requires
 * (one module, protocol 1, 64-hex map and fragment IDs, contiguous
 * fragments covering the module slice, the slice inside the program), and
 * __sv0cov_hit counts into a plain array. At exit the counts go to the file
 * named by SV0COV_STUB_OUT: the map ID, then one count per line in counter
 * order. Any protocol violation exits 97.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static const struct __sv0cov_module *stub_module;
static uint64_t *stub_counts;

static void stub_fail(const char *why) {
  fprintf(stderr, "stub-rt: %s\n", why);
  exit(97);
}

static int stub_is_hex64(const char *s) {
  if (s == NULL || strlen(s) != 64)
    return 0;
  for (const char *p = s; *p; p++)
    if (!((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f')))
      return 0;
  return 1;
}

static void stub_dump(void) {
  const char *path = getenv("SV0COV_STUB_OUT");
  if (path == NULL || stub_module == NULL)
    return;
  FILE *f = fopen(path, "w");
  if (f == NULL)
    return;
  fprintf(f, "%s\n", stub_module->map_id);
  for (uint32_t i = 0; i < stub_module->program_counter_count; i++)
    fprintf(f, "%llu\n", (unsigned long long)stub_counts[i]);
  fclose(f);
}

void __sv0cov_start(const struct __sv0cov_module *const *modules, uint32_t module_count) {
  if (stub_module != NULL)
    stub_fail("registered twice");
  if (module_count != 1 || modules == NULL || modules[0] == NULL)
    stub_fail("expected exactly one module");
  const struct __sv0cov_module *m = modules[0];
  if (m->protocol_major != 1)
    stub_fail("protocol major is not 1");
  if (!stub_is_hex64(m->map_id))
    stub_fail("map ID is not 64 lowercase hex digits");
  if (m->target == NULL || m->target[0] == '\0' || m->compiler_identity == NULL ||
      m->compiler_identity[0] == '\0')
    stub_fail("missing target or compiler identity");
  if (m->fragment_count == 0 || m->fragments == NULL)
    stub_fail("no fragments");
  uint64_t next = m->slice_base;
  for (uint32_t k = 0; k < m->fragment_count; k++) {
    if (!stub_is_hex64(m->fragments[k].fragment_id))
      stub_fail("fragment ID is not 64 lowercase hex digits");
    if (m->fragments[k].slice_base != next)
      stub_fail("fragments overlap or leave a gap");
    next += m->fragments[k].slice_length;
  }
  if (next != (uint64_t)m->slice_base + m->slice_length)
    stub_fail("fragments do not cover the module slice");
  if ((uint64_t)m->slice_base + m->slice_length != m->program_counter_count)
    stub_fail("the single module does not cover the program");
  stub_counts = calloc(m->program_counter_count ? m->program_counter_count : 1, sizeof(uint64_t));
  if (stub_counts == NULL)
    stub_fail("out of memory");
  stub_module = m;
  atexit(stub_dump);
}

void __sv0cov_hit(const struct __sv0cov_module *module, uint32_t local_index) {
  if (stub_module == NULL)
    stub_fail("hit before registration");
  if (module != stub_module)
    stub_fail("hit names an unregistered module");
  if (local_index >= module->slice_length)
    stub_fail("hit outside the module slice");
  stub_counts[module->slice_base + local_index]++;
}
