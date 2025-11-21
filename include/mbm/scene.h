#ifndef MBM_SCENE_H_INCLUDED
#define MBM_SCENE_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <stddef.h>

MBM_ABI void scene_deinit (void);
MBM_ABI void scene_delete (void);
MBM_ABI void scene_draw (void);
MBM_ABI void scene_init (SDL_Window * window, SDL_Renderer * renderer);
MBM_ABI void scene_new (void);
MBM_ABI void scene_update (void);

#endif
