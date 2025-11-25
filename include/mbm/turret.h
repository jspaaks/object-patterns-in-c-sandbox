#ifndef MBM_TURRET_H_INCLUDED
#define MBM_TURRET_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/ground.h"           // type `struct ground`
#include "mbm/scene.h"            // type `struct scene`
#include "mbm/spritesheet.h"      // type `struct spritesheet`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`

// `struct turret` is an opaque data structure
struct turret;

// define `Turret` as an alias for `struct turret`
typedef struct turret Turret;

MBM_ABI void turret_delete (struct turret ** self);
MBM_ABI void turret_draw (struct turret * self, struct spritesheet * spritesheet, SDL_Renderer * renderer);
MBM_ABI float turret_get_sim_w (struct turret * self);
MBM_ABI float turret_get_sim_h (struct turret * self);
MBM_ABI float turret_get_sim_x (struct turret * self);
MBM_ABI float turret_get_sim_y (struct turret * self);
MBM_ABI void turret_init (struct turret * self, struct scene * scene, struct ground * ground);
MBM_ABI struct turret * turret_new (void);
MBM_ABI void turret_update (struct turret * self, struct scene * scene);

#endif
