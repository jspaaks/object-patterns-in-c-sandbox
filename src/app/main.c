#define SDL_MAIN_USE_CALLBACKS 1  // use the callbacks instead of main()
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>        // definition of main() that calls the callback functions
#include <SDL3/SDL_init.h>        // SDL_InitFlags type, defines
#include <SDL3/SDL_video.h>       // SDL_WindowFlags type, defines
#include "mbm/spritesheet.h"
#include "mbm/balloon.h"

// We will use this renderer to draw into this window every frame.
static SDL_Window * window = nullptr;
static SDL_Renderer * renderer = nullptr;
static Spritesheet * spritesheet = nullptr;
static Balloon * balloon = nullptr;

static int texture_width = 0;
static int texture_height = 0;

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
        bool success = SDL_CreateWindowAndRenderer("SDL3 demo", WINDOW_WIDTH, WINDOW_HEIGHT, flags, &window, &renderer);
        if (!success) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }

    // load the spritesheet from file
    {
        spritesheet = spritesheet_new();
        spritesheet_init();

        balloon = balloon_new();
    }

    // continue with the rest of the program
    return SDL_APP_CONTINUE;
}


// `SDL_AppIterate` runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void * appstate) {
    //SDL_FRect dst_rect;
    //const Uint64 now = SDL_GetTicks();

    ///* we'll have some textures move around over a few seconds. */
    //const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    //const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

    ///* as you can see from this, rendering draws over whatever was drawn before it. */
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    //SDL_RenderClear(renderer);  /* start with a blank canvas. */

    ///* Just draw the static texture a few times. You can think of it like a
    //   stamp, there isn't a limit to the number of times you can draw with it. */

    ///* top left */
    //dst_rect.x = (100.0f * scale);
    //dst_rect.y = 0.0f;
    //dst_rect.w = (float) texture_width;
    //dst_rect.h = (float) texture_height;
    //SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    ///* center this one. */
    //dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) / 2.0f;
    //dst_rect.y = ((float) (WINDOW_HEIGHT - texture_height)) / 2.0f;
    //dst_rect.w = (float) texture_width;
    //dst_rect.h = (float) texture_height;
    //SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    ///* bottom right. */
    //dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) - (100.0f * scale);
    //dst_rect.y = (float) (WINDOW_HEIGHT - texture_height);
    //dst_rect.w = (float) texture_width;
    //dst_rect.h = (float) texture_height;
    //SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    //SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}


// SDL_AppQuit runs once at shutdown
void SDL_AppQuit(void * appstate, SDL_AppResult result) {
    //SDL_DestroyTexture(texture);
    /* SDL will clean up the window/renderer for us. */
    balloon_delete(&balloon);
    spritesheet_deinit();
    spritesheet_delete(&spritesheet);
}
