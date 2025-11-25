#ifndef MBM_SCENE_H_INCLUDED
#define MBM_SCENE_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

// `struct scene` is an opaque data structure
struct scene;

// define `Scene` as an alias for `struct scene`
typedef struct scene Scene;

MBM_ABI void scene_delete (struct scene ** self);
MBM_ABI void scene_draw (struct scene * self);
MBM_ABI float scene_get_scale (struct scene * self);
MBM_ABI float scene_get_tgt_x (struct scene * self);
MBM_ABI float scene_get_tgt_y (struct scene * self);
MBM_ABI void scene_init (struct scene * self, SDL_Window * window, SDL_Renderer * renderer);
MBM_ABI struct scene * scene_new (void);
MBM_ABI void scene_update (struct scene * self);

#endif
