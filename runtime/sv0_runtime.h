/*
 * sv0_runtime.h -- sv0 runtime support for C backend
 *
 * Provides panic, contract checking, string/vec/box operations,
 * and memory management primitives used by sv0c-generated C code.
 */
#ifndef SV0_RUNTIME_H
#define SV0_RUNTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* panic and assertion */
static inline void sv0_panic(const char *msg) {
    fprintf(stderr, "sv0 panic: %s\n", msg);
    exit(1);
}

static inline void sv0_assert(bool cond, const char *msg) {
    if (!cond) sv0_panic(msg);
}

/* contract checking (runtime mode) */
static inline void sv0_requires(bool cond, const char *fn_name) {
    if (!cond) {
        fprintf(stderr, "sv0 contract violation: requires failed in %s\n", fn_name);
        exit(1);
    }
}

static inline void sv0_ensures(bool cond, const char *fn_name) {
    if (!cond) {
        fprintf(stderr, "sv0 contract violation: ensures failed in %s\n", fn_name);
        exit(1);
    }
}

#endif /* SV0_RUNTIME_H */
