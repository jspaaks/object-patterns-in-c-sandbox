#ifndef MBM_ANIMATIONS_H_INCLUDED
#define MBM_ANIMATIONS_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include "SDL3/SDL_render.h"      // SDL_Renderer
#include <stdint.h>               // int64_t

// `struct animations` is an opaque data structure
struct animations;

MBM_NO_ABI void animations_delete (struct animations ** self);
MBM_NO_ABI struct animations * animations_new (int nanims_cap, int nframes_cap, const char * relpath, SDL_Renderer * renderer);
MBM_NO_ABI void animations_append_anim (struct animations * self);
MBM_NO_ABI void animations_append_frame (struct animations * self, int64_t duration, SDL_FRect src);
MBM_NO_ABI int64_t animations_get_animation_duration (struct animations * self, int ianim);
MBM_NO_ABI SDL_FRect animations_get_frame (const struct animations * self, int ianim, int iframe);
MBM_NO_ABI SDL_Texture * animations_get_texture (const struct animations * self);
MBM_NO_ABI void animations_update (const struct animations * self, int ianim, int64_t anim_phase_shift, int64_t tnow, int64_t * t_frame_expires, int * iframe);

#endif
