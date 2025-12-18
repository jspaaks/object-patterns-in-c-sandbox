#ifndef MBM_CAPTION_FPS_H_INCLUDED
#define MBM_CAPTION_FPS_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_render.h"      // SDL_Renderer

// `struct caption_fps` is an opaque data structure;
// only the implementation has access to its layout
struct caption_fps;

MBM_ABI void caption_fps_delete (struct caption_fps ** self);
MBM_ABI void caption_fps_draw (const struct caption_fps * self, SDL_Renderer * renderer);
MBM_ABI void caption_fps_init (struct caption_fps * self);
MBM_ABI struct caption_fps * caption_fps_new (void);
MBM_ABI void caption_fps_toggle (struct caption_fps * self);
MBM_ABI void caption_fps_update (struct caption_fps * self, const struct timings * timings);

#endif
