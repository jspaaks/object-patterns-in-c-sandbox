#ifndef MBM_GROUND_H_INCLUDED
#define MBM_GROUND_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/scene.h"
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`

// `struct ground` is an opaque data structure
struct ground;

// define `Ground` as an alias for `struct ground`
typedef struct ground Ground;

MBM_ABI void ground_delete (struct ground ** self);
MBM_ABI void ground_draw (struct ground * self);
MBM_ABI void ground_init (struct ground * self, SDL_Renderer * renderer);
MBM_ABI struct ground * ground_new (void);
MBM_ABI void ground_update (struct ground * self, struct scene * scene);

#endif
