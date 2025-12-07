#ifndef MBM_DUCK_H_INCLUDED
#define MBM_DUCK_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/dims.h"             // struct dims
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture

// `struct duck` is an opaque data structure
struct duck;

MBM_ABI void duck_delete (struct duck ** self);
MBM_ABI void duck_draw (const struct duck * self, SDL_Renderer * renderer);
MBM_ABI void duck_init (struct duck * self, SDL_Renderer * renderer, const struct dims * dims);
MBM_ABI struct duck * duck_new (void);
MBM_ABI void duck_set_animation_state_idle (struct duck * self, const struct timings * timings);
MBM_ABI void duck_set_animation_state_walking (struct duck * self, const struct timings * timings);
MBM_ABI void duck_update (struct duck * self, const struct timings * timings);

#endif
