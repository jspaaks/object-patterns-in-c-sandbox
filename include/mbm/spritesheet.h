#ifndef MBM_SPRITESHEET_H_INCLUDED
#define MBM_SPRITESHEET_H_INCLUDED
#include "mbm/abi.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_rect.h"
#include <stddef.h>


// `struct spritesheet` is an opaque data structure
struct spritesheet;

// define an alias for struct spritesheet
typedef struct spritesheet Spritesheet;

MBM_ABI void spritesheet_delete (struct spritesheet ** self);
MBM_ABI SDL_Rect spritesheet_get_rect_balloon_orange (void);
MBM_ABI SDL_Rect spritesheet_get_rect_balloon_red (void);
MBM_ABI SDL_Rect spritesheet_get_rect_balloon_yellow (void);
MBM_ABI char * spritesheet_get_path (void);
MBM_ABI size_t spritesheet_memsize (void);
MBM_ABI SDL_Surface * spritesheet_get_surface (void);
MBM_ABI void spritesheet_init (void);
MBM_ABI struct spritesheet * spritesheet_new (void);

#endif
