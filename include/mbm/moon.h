#ifndef MBM_MOON_H_INCLUDED
#define MBM_MOON_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/scene.h"
#include "mbm/spritesheet.h"
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`

// `struct moon` is an opaque data structure
struct moon;

// define `Moon` as an alias for `struct moon`
typedef struct moon Moon;

MBM_ABI void moon_delete (struct moon ** self);
MBM_ABI void moon_draw (struct moon * self, struct spritesheet * spritesheet, SDL_Renderer * renderer);
MBM_ABI void moon_init (struct moon * self, struct scene * scene);
MBM_ABI struct moon * moon_new (void);
MBM_ABI void moon_update (struct moon * self, struct scene * scene);

#endif
