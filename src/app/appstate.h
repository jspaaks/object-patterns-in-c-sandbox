#ifndef MBM_APP_APPSTATE_H_INCLUDED
#define MBM_APP_APPSTATE_H_INCLUDED
#include "mbm/game.h"             // struct game and associated functions
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_render.h"      // SDL_Renderer
#include "SDL3/SDL_video.h"       // SDL_Window

struct appstate {
    struct game * game;
    SDL_Renderer * renderer;
    struct timings * timings;
    SDL_Window * window;
};

#endif
