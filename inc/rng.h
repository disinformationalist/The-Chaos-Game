#ifndef RNG_H
#define RNG_H

#include <stdint.h>

typedef struct {
    uint64_t state[2];
    uint64_t splitmix_seed;
} Xoro128;

#endif
