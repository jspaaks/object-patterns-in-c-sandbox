#ifndef MBM_CAPTION_PAUSED_H_INCLUDED
#define MBM_CAPTION_PAUSED_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/world.h"            // struct world and associated functions
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture

// `struct caption_paused` is an opaque data structure;
// only the implementation has access to its layout
struct caption_paused;

MBM_ABI void caption_paused_delete (struct caption_paused ** self);
MBM_ABI void caption_paused_draw (const struct caption_paused * self, SDL_Renderer * renderer);
MBM_ABI void caption_paused_init (struct caption_paused * self, SDL_Renderer * renderer, const struct dims * dims);
MBM_ABI struct caption_paused * caption_paused_new (void);
MBM_ABI void caption_paused_update (struct caption_paused * self);

#endif
