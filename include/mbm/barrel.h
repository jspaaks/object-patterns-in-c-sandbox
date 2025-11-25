#ifndef MBM_BARREL_H_INCLUDED
#define MBM_BARREL_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/ground.h"           // type `struct ground`
#include "mbm/scene.h"            // type `struct scene`
#include "mbm/spritesheet.h"      // type `struct spritesheet`
#include "mbm/turret.h"           // type `Turret` and associated functions
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`

// `struct barrel` is an opaque data structure
struct barrel;

// define `Barrel` as an alias for `struct barrel`
typedef struct barrel Barrel;

MBM_ABI void barrel_delete (struct barrel ** self);
MBM_ABI void barrel_draw (struct barrel * self, struct spritesheet * spritesheet, SDL_Renderer * renderer);
MBM_ABI void barrel_init (struct barrel * self, struct spritesheet * spritesheet, struct turret * turret);
MBM_ABI struct barrel * barrel_new (void);
MBM_ABI void barrel_update (struct barrel * self, struct scene * scene);

#endif
