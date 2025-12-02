#include "appstate.h"             // type `AppState`
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

// forward declaration of static functions
static void init_sdl_subsystems (SDL_InitFlags flags);
static void init_sdl_window_and_renderer (SDL_WindowFlags flags, int window_width, int window_height,
                                          int view_width, int view_height,
                                          SDL_Renderer ** renderer, SDL_Window ** window);

static void init_sdl_subsystems (SDL_InitFlags flags) {
    bool success = SDL_Init(flags);
    if (!success) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        exit(1);
    }
}

static void init_sdl_window_and_renderer (SDL_WindowFlags flags, int window_width, int window_height,
                                          int view_width, int view_height,
                                          SDL_Renderer ** renderer, SDL_Window ** window) {

    bool success = SDL_CreateWindowAndRenderer("scroller", window_width, window_height, flags,
                                               window, renderer);
    if (!success) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderLogicalPresentation(*renderer, view_width, view_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

// `SDL_AppEvent` runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void * appstate_vp, SDL_Event * event) {
    // make the void pointer appstate_vp usable by casting it as an AppState pointer
    AppState * appstate = (AppState *) appstate_vp;
    return game_handle_event(appstate->game, event);
}

// `SDL_AppInit` runs once at startup
SDL_AppResult SDL_AppInit(void ** appstate_vpp, int argc, char * argv[]) {

    // define argc and argv as unused
    (void) argc;
    (void) argv;

    const int view_width = 640;
    const int view_height = 386;
    const int window_width = 800;
    const int window_height = 360;

    Game * game = nullptr;
    SDL_Renderer * renderer = nullptr;
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
    init_sdl_window_and_renderer(window_flags, window_width, window_height,
                                 view_width, view_height,
                                 &renderer, &window);

    // initialize the game object
    game = game_new();
    game_init(game, renderer, view_width, view_height);

    // facilitate sharing state between callbacks via void ** appstate_vpp
    *appstate_vpp = (void *) calloc(1, sizeof(AppState));

    // make the void pointer appstate_vp usable by casting it as an AppState pointer
    AppState ** appstate = (AppState **) appstate_vpp;
    (*appstate)->game = game;
    (*appstate)->renderer = renderer;
    (*appstate)->window = window;

    // continue with the rest of the program
    return SDL_APP_CONTINUE;
}

// `SDL_AppIterate` runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void * appstate_vp) {

    // make the void pointer appstate_vp usable by casting it as an AppState pointer
    AppState * appstate = (AppState *) appstate_vp;

    // update relevant objects
    game_update(appstate->game);

    // draw relevant objects
    game_draw(appstate->game, appstate->renderer);

    // update the screen with this frame's rendering
    SDL_RenderPresent(appstate->renderer);

    SDL_Delay(10);

    return SDL_APP_CONTINUE;
}

// SDL_AppQuit runs once at shutdown
void SDL_AppQuit(void * appstate_vp, SDL_AppResult result) {
    // make the void pointer appstate_vp usable by casting it as an AppState pointer
    AppState * appstate = (AppState *) appstate_vp;

    game_delete(&appstate->game);
    // SDL will clean up appstate->window for us
    // SDL will clean up appstate->renderer for us

    free(appstate_vp);

    // exit with result status
    exit(result);
}
