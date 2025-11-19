#ifndef MBM_BALLOON_H_INCLUDED
#define MBM_BALLOON_H_INCLUDED
#include <stddef.h>
#include "mbm/balloon_abi.h"
#include "mbm/timing.h"

// `struct baloon` is an opaque data structure
struct balloon;

MBM_BALLOON_ABI void balloon_delete (struct balloon ** self);
MBM_BALLOON_ABI int balloon_get_n (struct balloon * self);
MBM_BALLOON_ABI size_t balloon_get_size (void);
MBM_BALLOON_ABI void balloon_increment_n (struct balloon * self);
MBM_BALLOON_ABI void balloon_init (struct balloon * self);
MBM_BALLOON_ABI struct balloon * balloon_new (void);
MBM_BALLOON_ABI void balloon_print (struct balloon * self, const char * name);
MBM_BALLOON_ABI void balloon_update_pos (struct balloon * self, const struct timing * timing);

#endif
