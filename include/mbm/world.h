#ifndef MBM_WORLD_H_INCLUDED
#define MBM_WORLD_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_rect.h"        // type `SDL_Rect`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`, `SDL_Texture`

// `struct world` is an opaque data structure
struct world;

// define `World` as an alias for `struct world`
typedef struct world World;

MBM_ABI void world_delete (struct world ** self);
MBM_ABI void world_draw (struct world * self, SDL_Renderer * renderer);
MBM_ABI void world_init (struct world * self, SDL_Renderer * renderer, int view_width, int view_height);
MBM_ABI struct world * world_new (void);
MBM_ABI void world_update (struct world * self);

#endif
