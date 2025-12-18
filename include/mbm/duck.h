#ifndef MBM_DUCK_H_INCLUDED
#define MBM_DUCK_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/dims.h"             // struct dims
#include "mbm/timings.h"          // struct timings and associated functions
#include "mbm/world.h"            // struct world and associated functions
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture

// `struct duck` is an opaque data structure;
// only the implementation has access to its layout
struct duck;

MBM_ABI void duck_delete (struct duck ** self);
MBM_ABI void duck_draw (const struct duck * self, SDL_Renderer * renderer);
MBM_ABI void duck_face_left (struct duck * self);
MBM_ABI void duck_face_right (struct duck * self);
MBM_ABI SDL_FRect duck_get_bbox (const struct duck * self);
MBM_ABI void duck_halt (struct duck * self);
MBM_ABI void duck_init (struct duck * self, SDL_Renderer * renderer, const struct dims * dims);
MBM_ABI void duck_jump (struct duck * self);
MBM_ABI struct duck * duck_new (void);
MBM_ABI void duck_translate_y (struct duck * self, float dy);
MBM_ABI void duck_update (struct duck * self, const struct world * world, const struct timings * timings);
MBM_ABI void duck_walk_left (struct duck * self);
MBM_ABI void duck_walk_right (struct duck * self);

#endif
