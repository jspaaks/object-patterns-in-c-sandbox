#include "appstate.h"             // struct appstate
#include "mbm/dims.h"             // struct dims
#include "mbm/game.h"             // struct game and associated functions
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_events.h"      // SDL_Event
#include "SDL3/SDL_init.h"        // SDL_InitFlags, SDL_AppResult, defines
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#define SDL_MAIN_USE_CALLBACKS 1  // use the callbacks instead of main()
#include "SDL3/SDL_main.h"        // definition of main() that calls the callback functions
#include "SDL3/SDL_render.h"      // SDL_Renderer
#include "SDL3/SDL_stdinc.h"      // SDL_free, SDL_calloc
#include "SDL3/SDL_timer.h"       // SDL_GetTicksNS
#include "SDL3/SDL_video.h"       // SDL_Window, SDL_WindowFlags, defines
#include "SDL3/SDL.h"
#include <stdlib.h>               // atexit, exit

// forward declaration of static functions
static void init_sdl_subsystems (SDL_InitFlags flags);
static void init_sdl_window_and_renderer (SDL_WindowFlags flags, struct dims * dims,
                                          SDL_Renderer ** renderer, SDL_Window ** window);

static void init_sdl_subsystems (SDL_InitFlags flags) {
    bool success = SDL_Init(flags);
    if (!success) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't initialize SDL, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
}

static void init_sdl_window_and_renderer (SDL_WindowFlags flags, struct dims * dims,
                                          SDL_Renderer ** renderer, SDL_Window ** window) {

    bool success = SDL_CreateWindowAndRenderer("scroller", dims->window.w, dims->window.h, flags,
                                               window, renderer);
    if (!success) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't create window/renderer, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    SDL_SetRenderLogicalPresentation(*renderer, dims->view.w, dims->view.h, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

// `SDL_AppEvent` runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void * appstate_vp, SDL_Event * event) {
    // make the void pointer appstate_vp usable by casting it as a struct appstate pointer
    struct appstate * appstate = (struct appstate *) appstate_vp;
    return game_handle_event(appstate->game, event);
}

// `SDL_AppInit` runs once at startup`
SDL_AppResult SDL_AppInit(void ** appstate_vpp, int argc, char * argv[]) {

    // define argc and argv as unused
    (void) argc;
    (void) argv;

    struct dims dims = (struct dims) {
        .tile = {
            .h = 32,
            .w = 32,
        },
        .view = {
            .h = 7 * 32,
            .w = 20 * 32,
        },
        .window = {
            .h = 360,
            .w = 800,
        },
        .world = {
            .h = 7 * 32,
            .w = 41 * 32,
        }
    };

    struct game * game = nullptr;
    SDL_Renderer * renderer = nullptr;
    struct timings * timings = nullptr;
    SDL_Window * window = nullptr;

    const SDL_InitFlags init_flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    const SDL_WindowFlags window_flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE;

    // register SDL_Quit function to run at exit
    atexit(SDL_Quit);

    // set the app's metadata
    SDL_SetAppMetadata("scroller", "1.0", "com.github.jspaaks.scroller");

    // initialize the subsystems
    init_sdl_subsystems(init_flags);

    // initialize the window and renderer
    init_sdl_window_and_renderer(window_flags, &dims, &renderer, &window);

    // initialize the timings object
    timings = timings_new();
    timings_init(timings);

    // initialize the game object
    game = game_new();
    game_init(game, renderer, &dims);

    // facilitate sharing state between callbacks via void ** appstate_vpp
    *appstate_vpp = (void *) SDL_calloc(1, sizeof(struct appstate));
    if (*appstate_vpp == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Could not allocate dynamic memory for appstate, aborting: %s\n",
                        SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // make the void pointer appstate_vp usable by casting it as a struct appstate pointer
    struct appstate ** appstate = (struct appstate **) appstate_vpp;
    (*appstate)->game = game;
    (*appstate)->renderer = renderer;
    (*appstate)->timings = timings;
    (*appstate)->window = window;

    // continue with the rest of the program
    return SDL_APP_CONTINUE;
}

// `SDL_AppIterate` runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void * appstate_vp) {

    // make the void pointer appstate_vp usable by casting it as a struct appstate pointer
    struct appstate * appstate = (struct appstate *) appstate_vp;

    // promote the contents of appstate for easier handling further down in this function
    struct timings * timings = appstate->timings;
    struct game * game  = appstate->game;
    SDL_Renderer * renderer  = appstate->renderer;

    // update timings
    timings_update(timings);

    // update relevant objects
    game_update(game, timings);

    // draw relevant objects
    game_draw(game, renderer);

    // update the screen with this frame's rendering
    SDL_RenderPresent(renderer);

    // make sure frames last at least 1 microsecond to avoid numerical problems with integration
    SDL_DelayNS(1000);

    return SDL_APP_CONTINUE;
}

// SDL_AppQuit runs once at shutdown
void SDL_AppQuit(void * appstate_vp, SDL_AppResult result) {
    // make the void pointer appstate_vp usable by casting it as a struct appstate pointer
    struct appstate * appstate = (struct appstate *) appstate_vp;

    // clean up appstate and its members
    if (appstate != nullptr) {
        game_delete(&appstate->game);
        timings_delete(&appstate->timings);
        SDL_DestroyRenderer(appstate->renderer);
        appstate->renderer = nullptr;
        SDL_DestroyWindow(appstate->window);
        appstate->window = nullptr;
        SDL_free(appstate_vp);
        appstate_vp = nullptr;
    }

    // exit with result status
    exit(result);
}
