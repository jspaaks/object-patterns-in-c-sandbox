#ifndef MBM_BALLOON_H_INCLUDED
#define MBM_BALLOON_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_rect.h"
#include <stddef.h>


// `struct balloon` is an opaque data structure
struct balloon;

// define `Balloon` as an alias for `struct balloon`
typedef struct balloon Balloon;

// forward function declarations
MBM_ABI void balloon_delete (struct balloon ** self);
MBM_ABI SDL_FRect balloon_get_sim (struct balloon * self);
MBM_ABI size_t balloon_memsize (void);
MBM_ABI void balloon_init (struct balloon * self);
MBM_ABI struct balloon * balloon_new (void);

#endif
