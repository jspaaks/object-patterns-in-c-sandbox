#include "mbm/balloon.h"          // type `Balloon` and associated functions
#include "mbm/game.h"             // type `Game` and associated functions
#include <SDL3/SDL_error.h>       // function `SDL_GetError`
#include <SDL3/SDL_init.h>        // type `SDL_InitFlags`, defines
#include <SDL3/SDL_log.h>         // function `SDL_Log`
#define SDL_MAIN_USE_CALLBACKS 1  // use the callbacks instead of main()
#include <SDL3/SDL_main.h>        // definition of main() that calls the callback functions
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <SDL3/SDL_video.h>       // types `SDL_Window`, `SDL_WindowFlags`, defines
#include <SDL3/SDL.h>

static SDL_Window * window = nullptr;
static SDL_Renderer * renderer = nullptr;
static Balloon * balloon = nullptr;
static Game * game = nullptr;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// `SDL_AppEvent` runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void * appstate, SDL_Event * event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.scancode == SDL_SCANCODE_Q) {
                return SDL_APP_SUCCESS;
            }
    }
    return SDL_APP_CONTINUE;
}


// `SDL_AppInit` runs once at startup
SDL_AppResult SDL_AppInit(void ** appstate, int argc, char * argv[]) {
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
        const SDL_WindowFlags flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE;
        bool success = SDL_CreateWindowAndRenderer("SDL3 demo", WINDOW_WIDTH, WINDOW_HEIGHT,
                                                   flags, &window, &renderer);
        if (!success) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }

    // initialize the game object
    game = game_new();
    game_init(game, window, renderer);

    // initializee a balloon
    balloon = balloon_new();

    // continue with the rest of the program
    return SDL_APP_CONTINUE;
}


// `SDL_AppIterate` runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void * appstate) {

    // update relevant objects
    game_update(game);

    // draw relevant objects
    game_draw(game);

    // update the screen with this frame's rendering
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}


// SDL_AppQuit runs once at shutdown
void SDL_AppQuit(void * appstate, SDL_AppResult result) {
    balloon_delete(&balloon);
    game_delete(&game);
    // SDL will clean up the window and renderer for us
}
