#ifndef MBM_SPRITESHEET_H_INCLUDED
#define MBM_SPRITESHEET_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_rect.h"        // type `SDL_Rect`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`, `SDL_Texture`

// `struct spritesheet` is an opaque data structure
struct spritesheet;

// define `Spritesheet` as an alias for `struct spritesheet`
typedef struct spritesheet Spritesheet;

MBM_ABI void spritesheet_delete (struct spritesheet ** self);
MBM_ABI char * spritesheet_get_path (struct spritesheet * self);
MBM_ABI SDL_FRect * spritesheet_get_balloon_orange (struct spritesheet * self);
MBM_ABI SDL_FRect * spritesheet_get_balloon_red (struct spritesheet * self);
MBM_ABI SDL_FRect * spritesheet_get_balloon_yellow (struct spritesheet * self);
MBM_ABI SDL_FRect * spritesheet_get_barrel (struct spritesheet * self);
MBM_ABI SDL_FRect * spritesheet_get_bullet (struct spritesheet * self);
MBM_ABI SDL_FRect * spritesheet_get_collision (struct spritesheet * self);
MBM_ABI SDL_FRect * spritesheet_get_flash (struct spritesheet * self);
MBM_ABI SDL_FRect * spritesheet_get_moon (struct spritesheet * self);
MBM_ABI SDL_Texture * spritesheet_get_texture (struct spritesheet * self);
MBM_ABI void spritesheet_init (struct spritesheet * self, SDL_Renderer * renderer);
MBM_ABI struct spritesheet * spritesheet_new (void);

#endif
