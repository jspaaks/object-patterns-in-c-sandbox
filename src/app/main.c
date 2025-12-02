#include "mbm/game.h"             // type `Game` and associated functions
#include <SDL3/SDL_error.h>       // function `SDL_GetError`
#include <SDL3/SDL_events.h>      // type `SDL_Event`
#include <SDL3/SDL_init.h>        // types `SDL_InitFlags`, `SDL_AppResult`, defines
#include <SDL3/SDL_log.h>         // function `SDL_Log`
#define SDL_MAIN_USE_CALLBACKS 1  // use the callbacks instead of main()
#include <SDL3/SDL_main.h>        // definition of main() that calls the callback functions
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <SDL3/SDL_timer.h>       // type `SDL_Delay`
#include <SDL3/SDL_video.h>       // types `SDL_Window`, `SDL_WindowFlags`, defines
#include <SDL3/SDL.h>
#include <stdlib.h>               // atexit

static SDL_Window * window = nullptr;
static SDL_Renderer * renderer = nullptr;

// `SDL_AppEvent` runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void * game, SDL_Event * event) {
    return game_handle_event((Game *) game, event);
}

// `SDL_AppInit` runs once at startup
SDL_AppResult SDL_AppInit(void ** game, int argc, char * argv[]) {

    const int view_width = 640;
    const int view_height = 386;

    // register SDL_Quit function to run at exit
    atexit(SDL_Quit);

    SDL_SetAppMetadata("SDL3 demo", "1.0", "com.github.jspaaks.mbm");

    // initialize the subsystems
    {
        const SDL_InitFlags flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
        bool success = SDL_Init(flags);
        if (!success) {
            SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }

    // initialize the window and renderer
    {
        const int window_width = 800;
        const int window_height = 360;
        const SDL_WindowFlags flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE;
        bool success = SDL_CreateWindowAndRenderer("SDL3 demo", window_width, window_height,
                                                   flags, &window, &renderer);
        if (!success) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
        SDL_SetRenderLogicalPresentation(renderer, view_width, view_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    }

    // initialize the game object
    *game = (void *) game_new();
    game_init(*(Game **) game, renderer, view_width, view_height);

    // continue with the rest of the program
    return SDL_APP_CONTINUE;
}

// `SDL_AppIterate` runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void * game) {

    // update relevant objects
    game_update((Game *) game);

    // draw relevant objects
    game_draw((Game *) game, renderer);

    // update the screen with this frame's rendering
    SDL_RenderPresent(renderer);

    SDL_Delay(10);

    return SDL_APP_CONTINUE;
}

// SDL_AppQuit runs once at shutdown
void SDL_AppQuit(void * game, SDL_AppResult result) {
    game_delete((Game **) &game);
    // SDL will clean up the window and renderer for us
}
