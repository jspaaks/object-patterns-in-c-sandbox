#ifndef MBM_SPRITESHEET_H_INCLUDED
#define MBM_SPRITESHEET_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_rect.h"

// `struct spritesheet` is an opaque data structure
struct spritesheet;

// define `Spritesheet` as an alias for `struct spritesheet`
typedef struct spritesheet Spritesheet;

MBM_ABI void spritesheet_delete (struct spritesheet ** self);
MBM_ABI char * spritesheet_get_path (struct spritesheet * self);
MBM_ABI SDL_Rect spritesheet_get_rect_balloon_orange (struct spritesheet * self);
MBM_ABI SDL_Rect spritesheet_get_rect_balloon_red (struct spritesheet * self);
MBM_ABI SDL_Rect spritesheet_get_rect_balloon_yellow (struct spritesheet * self);
MBM_ABI SDL_Surface * spritesheet_get_surface (struct spritesheet * self);
MBM_ABI void spritesheet_init (struct spritesheet * self);
MBM_ABI struct spritesheet * spritesheet_new (void);

#endif
