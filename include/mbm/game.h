#ifndef MBM_GAME_H_INCLUDED
#define MBM_GAME_H_INCLUDED
#include "mbm/abi.h"
#include <SDL3/SDL_events.h>      // type `SDL_Event`
#include <SDL3/SDL_init.h>        // type `SDL_AppResult`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <SDL3/SDL_video.h>       // type `SDL_Window`

// `struct game` is an opaque data structure
struct game;

// define `Game` as an alias for `struct game`
typedef struct game Game;

MBM_ABI void game_delete (struct game ** self);
MBM_ABI void game_draw (struct game * self, SDL_Renderer * renderer);
MBM_ABI SDL_AppResult game_handle_event (struct game * self, SDL_Event * event);
MBM_ABI void game_init (struct game * self, SDL_Renderer * renderer, int view_width, int view_height);
MBM_ABI struct game * game_new (void);
MBM_ABI void game_update (struct game * self);

#endif
