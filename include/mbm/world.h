#ifndef MBM_WORLD_H_INCLUDED
#define MBM_WORLD_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/dims.h"             // struct dims
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture

// `struct world` is an opaque data structure;
// only the implementation has access to its layout
struct world;

MBM_ABI void world_delete (struct world ** self);
MBM_ABI void world_draw (const struct world * self, SDL_Renderer * renderer);
MBM_ABI SDL_FRect world_get_bbox (const struct world * self);
MBM_ABI float world_get_gravity (const struct world * self);
MBM_ABI void world_init (struct world * self, SDL_Renderer * renderer, const struct dims * dims);
MBM_ABI struct world * world_new (void);
MBM_ABI void world_update (struct world * self, const struct timings * timings);

#endif
