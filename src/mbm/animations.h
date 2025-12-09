#ifndef MBM_ANIMATIONS_H_INCLUDED
#define MBM_ANIMATIONS_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include "SDL3/SDL_render.h"      // SDL_Renderer

// `struct animations` is an opaque data structure
struct animations;

MBM_NO_ABI void animations_delete (struct animations ** self);
MBM_NO_ABI struct animations * animations_new (int nanims_cap, int nframes_cap, const char * relpath, SDL_Renderer * renderer);
MBM_NO_ABI void animations_append_anim (struct animations * self);
MBM_NO_ABI void animations_append_frame (struct animations * self, uint64_t duration, SDL_FRect src);
MBM_NO_ABI uint64_t animations_get_animation_duration (const struct animations * self, int ianim);
MBM_NO_ABI int animations_get_animation_nframes (const struct animations * self, int ianim);
MBM_NO_ABI uint64_t animations_get_frame_duration (const struct animations * self, int ianim, int iframe);
MBM_NO_ABI SDL_FRect animations_get_frame_src (const struct animations * self, int ianim, int iframe);
MBM_NO_ABI SDL_Texture * animations_get_texture (const struct animations * self);

#endif
