#ifndef MBM_TIMINGS_H_INCLUDED
#define MBM_TIMINGS_H_INCLUDED
#include "mbm/abi.h"
#include <stdint.h>

// `struct timings` is an opaque data structure
struct timings;

MBM_ABI void timings_delete (struct timings ** self);
MBM_ABI float timings_get_frame_duration (const struct timings * self);
MBM_ABI uint64_t timings_get_frame_timestamp (const struct timings * self);
MBM_ABI void timings_init (struct timings * self);
MBM_ABI struct timings * timings_new (void);
MBM_ABI void timings_update (struct timings * self);

#endif
