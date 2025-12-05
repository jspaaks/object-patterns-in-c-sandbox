#ifndef MBM_WHEEL_H_INCLUDED
#define MBM_WHEEL_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/dims.h"             // struct dims
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture

// `struct wheel` is an opaque data structure
struct wheel;

MBM_ABI void wheel_delete (struct wheel ** self);
MBM_ABI void wheel_draw (const struct wheel * self, SDL_Renderer * renderer);
MBM_ABI void wheel_init (struct wheel * self, SDL_Renderer * renderer, const struct dims * dims);
MBM_ABI struct wheel * wheel_new (void);
MBM_ABI void wheel_update (struct wheel * self, const struct timings * timings);

#endif
