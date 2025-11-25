#include "mbm/spritesheet.h"
#include <SDL3/SDL_error.h>       // function `SDL_GetError`
#include <SDL3/SDL_log.h>         // function `SDL_Log`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <SDL3/SDL_init.h>        // types `SDL_InitFlags`, `SDL_AppResult`, defines
#include <SDL3/SDL_video.h>       // types `SDL_Window`, `SDL_WindowFlags`, defines
#include "SDL3/SDL_rect.h"
#include <criterion/criterion.h>
#include <stddef.h>

static Spritesheet * spritesheet = nullptr;
static SDL_Renderer * renderer = nullptr;
static SDL_Window * window = nullptr;

void spritesheet_setup (void) {
    // initialize the subsystems
    {
        const SDL_InitFlags flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
        bool success = SDL_Init(flags);
        if (!success) {
            SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
            return;
        }
    }

    // initialize the window and renderer
    {
        const int window_width = 640+260;
        const int window_height = 360;
        const SDL_WindowFlags flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE;
        bool success = SDL_CreateWindowAndRenderer("SDL3 demo", window_width, window_height,
                                                   flags, &window, &renderer);
        if (!success) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return;
        }
    }

    spritesheet = spritesheet_new();
    spritesheet_init(spritesheet, renderer);
}

void spritesheet_teardown(void) {
    spritesheet_delete(&spritesheet);
}

Test (Spritesheet, test_get_path, .init=spritesheet_setup, .fini=spritesheet_teardown) {
    const char * actual = spritesheet_get_path(spritesheet);
    cr_assert(actual != nullptr);
}

Test (Spritesheet, test_get_balloon_orange, .init=spritesheet_setup, .fini=spritesheet_teardown) {
    const SDL_FRect * actual = spritesheet_get_balloon_orange(spritesheet);
    const SDL_FRect expected = (SDL_FRect) {
        .h = 12.0,
        .w = 9.0,
        .x = 184.0,
        .y = 1.0,
    };
    const char * subject = "Orange balloon";
    cr_assert(actual->h == expected.h,
              "%s: actual height (%d) not equal to expected height (%d)",
              subject, actual->h, expected.h);
    cr_assert(actual->w == expected.w,
              "%s: actual width (%d) not equal to expected width (%d)",
              subject, actual->w, expected.w);
    cr_assert(actual->x == expected.x,
              "%s: actual x (%d) not equal to expected x (%d)",
              subject, actual->x, expected.x);
    cr_assert(actual->y == expected.y,
              "%s: actual y (%d) not equal to expected y (%d)",
              subject, actual->y, expected.y);
}

Test (Spritesheet, test_get_balloon_red, .init=spritesheet_setup, .fini=spritesheet_teardown) {
    const SDL_FRect * actual = spritesheet_get_balloon_red(spritesheet);
    const SDL_FRect expected = (SDL_FRect) {
        .h = 8.0,
        .w = 6.0,
        .x = 184.0,
        .y = 20.0,
    };
    const char * subject = "Red balloon";
    cr_assert(actual->h == expected.h,
              "%s: actual height (%d) not equal to expected height (%d)",
              subject, actual->h, expected.h);
    cr_assert(actual->w == expected.w,
              "%s: actual width (%d) not equal to expected width (%d)",
              subject, actual->w, expected.w);
    cr_assert(actual->x == expected.x,
              "%s: actual x (%d) not equal to expected x (%d)",
              subject, actual->x, expected.x);
    cr_assert(actual->y == expected.y,
              "%s: actual y (%d) not equal to expected y (%d)",
              subject, actual->y, expected.y);
}

Test (Spritesheet, test_get_balloon_yellow, .init=spritesheet_setup, .fini=spritesheet_teardown) {
    const SDL_FRect * actual = spritesheet_get_balloon_yellow(spritesheet);
    const SDL_FRect expected = (SDL_FRect) {
        .h = 16.0,
        .w = 12.0,
        .x = 166.0,
        .y = 1.0,
    };
    const char * subject = "Yellow balloon";
    cr_assert(actual->h == expected.h,
              "%s: actual height (%d) not equal to expected height (%d)",
              subject, actual->h, expected.h);
    cr_assert(actual->w == expected.w,
              "%s: actual width (%d) not equal to expected width (%d)",
              subject, actual->w, expected.w);
    cr_assert(actual->x == expected.x,
              "%s: actual x (%d) not equal to expected x (%d)",
              subject, actual->x, expected.x);
    cr_assert(actual->y == expected.y,
              "%s: actual y (%d) not equal to expected y (%d)",
              subject, actual->y, expected.y);
}


Test (Spritesheet, test_new, .init=spritesheet_setup, .fini=spritesheet_teardown) {
    Spritesheet * again = spritesheet_new();
    cr_assert(spritesheet == again,
              "spritesheet instantiation should only allow a single instance");
}
