/*
 * Test-only stand-in for the native coverage runtime (sv0c CV-113 tests).
 *
 * The real runtime (sv0cov runtime/c, CV-114) owns the atomic arena, the
 * transport, and the raw profile. This stub implements the same generated-C
 * protocol 1 interface so run_emit_c.py can link and run instrumented C:
 * __sv0cov_start validates the registration the way SPEC 14.1 requires for
 * sv0c's shape (CV-206: one module per fragment, in map order, protocol 1,
 * one 64-hex map, 64-hex fragment IDs, slices tiling the program), and
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

static const struct __sv0cov_module *const *stub_modules;
static uint32_t stub_module_count;
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
  if (path == NULL || stub_modules == NULL)
    return;
  FILE *f = fopen(path, "w");
  if (f == NULL)
    return;
  fprintf(f, "%s\n", stub_modules[0]->map_id);
  for (uint32_t i = 0; i < stub_modules[0]->program_counter_count; i++)
    fprintf(f, "%llu\n", (unsigned long long)stub_counts[i]);
  fclose(f);
}

void __sv0cov_start(const struct __sv0cov_module *const *modules, uint32_t module_count) {
  if (stub_modules != NULL)
    stub_fail("registered twice");
  if (module_count == 0 || modules == NULL)
    stub_fail("no module");
  uint64_t total = 0, covered = 0;
  for (uint32_t j = 0; j < module_count; j++) {
    const struct __sv0cov_module *m = modules[j];
    if (m == NULL)
      stub_fail("a module is missing");
    if (j == 0)
      total = m->program_counter_count;
    if (m->protocol_major != 1)
      stub_fail("protocol major is not 1");
    if (!stub_is_hex64(m->map_id) || strcmp(m->map_id, modules[0]->map_id) != 0 ||
        m->program_counter_count != total)
      stub_fail("modules name different or malformed maps");
    if (m->target == NULL || m->target[0] == '\0' || m->compiler_identity == NULL ||
        m->compiler_identity[0] == '\0')
      stub_fail("missing target or compiler identity");
    /* sv0c emits one module per fragment (CV-206), in map order. */
    if (m->fragment_count != 1 || m->fragments == NULL)
      stub_fail("a module does not hold exactly one fragment");
    if (!stub_is_hex64(m->fragments[0].fragment_id))
      stub_fail("fragment ID is not 64 lowercase hex digits");
    if (m->fragments[0].slice_base != m->slice_base || m->fragments[0].slice_length != m->slice_length)
      stub_fail("the fragment does not cover its module's slice");
    if (m->slice_base != covered)
      stub_fail("module slices overlap or leave a gap");
    covered += m->slice_length;
  }
  if (covered != total)
    stub_fail("the modules do not cover the program");
  stub_counts = calloc(total ? total : 1, sizeof(uint64_t));
  if (stub_counts == NULL)
    stub_fail("out of memory");
  stub_modules = modules;
  stub_module_count = module_count;
  atexit(stub_dump);
}

void __sv0cov_hit(const struct __sv0cov_module *module, uint32_t local_index) {
  if (stub_modules == NULL)
    stub_fail("hit before registration");
  int known = 0;
  for (uint32_t j = 0; j < stub_module_count; j++)
    known |= stub_modules[j] == module;
  if (!known)
    stub_fail("hit names an unregistered module");
  if (local_index >= module->slice_length)
    stub_fail("hit outside the module slice");
  stub_counts[module->slice_base + local_index]++;
}
