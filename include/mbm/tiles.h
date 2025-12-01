#ifndef MBM_TILES_H_INCLUDED
#define MBM_TILES_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_rect.h"        // type `SDL_Rect`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`, `SDL_Texture`

// `struct tiles` is an opaque data structure
struct tiles;

// define `tiles` as an alias for `struct tiles`
typedef struct tiles Tiles;

MBM_ABI void tiles_delete (struct tiles ** self);
MBM_ABI void tiles_draw (struct tiles * self, SDL_Renderer * renderer);
MBM_ABI void tiles_init (struct tiles * self, SDL_Renderer * renderer);
MBM_ABI struct tiles * tiles_new (void);
MBM_ABI void tiles_update (struct tiles * self);

#endif
