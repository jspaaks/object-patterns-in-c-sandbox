#include "mbm/background.h"
#include "mbm/balloon.h"
#include "mbm/scene.h"
#include "mbm/spritesheet.h"
#include <SDL3/SDL_init.h>        // SDL_InitFlags type, defines
#define SDL_MAIN_USE_CALLBACKS 1  // use the callbacks instead of main()
#include <SDL3/SDL_main.h>        // definition of main() that calls the callback functions
#include <SDL3/SDL_video.h>       // SDL_WindowFlags type, defines
#include <SDL3/SDL.h>

static SDL_Window * window = nullptr;
static SDL_Renderer * renderer = nullptr;
static Balloon * balloon = nullptr;
static Scene * scene = nullptr;
static Spritesheet * spritesheet = nullptr;
static Background * background = nullptr;

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

    // load the spritesheet from file
    spritesheet = spritesheet_new();
    spritesheet_init(spritesheet);

    // initialize the background
    background = background_new();
    background_init(background, renderer);

    // initialize the scene
    scene = scene_new();
    scene_init(scene, window, renderer);

    // initializee a balloon
    balloon = balloon_new();

    // continue with the rest of the program
    return SDL_APP_CONTINUE;
}


// `SDL_AppIterate` runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void * appstate) {

    // update relevant objects
    background_update(background);
    scene_update(scene);

    // draw relevant objects
    background_draw(background);
    scene_draw(scene);

    // update the screen with this frame's rendering
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}


// SDL_AppQuit runs once at shutdown
void SDL_AppQuit(void * appstate, SDL_AppResult result) {
    background_delete(&background);
    balloon_delete(&balloon);
    scene_delete(&scene);
    spritesheet_delete(&spritesheet);
    // SDL will clean up the window and renderer for us
}
