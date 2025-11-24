#ifndef MBM_GAME_H_INCLUDED
#define MBM_GAME_H_INCLUDED
#include "mbm/abi.h"
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <SDL3/SDL_video.h>       // type `SDL_Window`

// `struct game` is an opaque data structure
struct game;

// define `Game` as an alias for `struct game`
typedef struct game Game;

MBM_ABI void game_delete (struct game ** self);
MBM_ABI void game_draw (struct game * self);
MBM_ABI void game_init (struct game * self, SDL_Window * window, SDL_Renderer * renderer);
MBM_ABI struct game * game_new (void);
MBM_ABI void game_update (struct game * self);

#endif
