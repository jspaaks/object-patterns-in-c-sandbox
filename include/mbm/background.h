#ifndef MBM_BACKGROUND_H_INCLUDED
#define MBM_BACKGROUND_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_render.h"      // SDL_Renderer

// `struct background` is an opaque data structure
struct background;

MBM_ABI void background_delete (struct background ** self);
MBM_ABI void background_draw (const struct background * self, SDL_Renderer * renderer);
MBM_ABI void background_init (struct background * self);
MBM_ABI struct background * background_new (void);
MBM_ABI void background_update (struct background * self, const struct timings * timings);

#endif
