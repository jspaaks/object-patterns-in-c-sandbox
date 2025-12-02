#ifndef MBM_APP_APPSTATE_H_INCLUDED
#define MBM_APP_APPSTATE_H_INCLUDED
#include "mbm/game.h"             // type `Game` and associated functions
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <SDL3/SDL_video.h>       // types `SDL_Window`

typedef struct appstate {
    Game * game;
    SDL_Renderer * renderer;
    SDL_Window * window;
} AppState;

#endif