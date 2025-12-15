#include "mbm/background.h"       // struct background and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_pixels.h"      // SDL_Color
#include "SDL3/SDL_render.h"      // SDL_Renderer
#include "SDL3/SDL_stdinc.h"      // SDL_free, SDL_calloc
#include <stdlib.h>               // exit

// declare properties of `struct background`
struct background {
    SDL_Color color;
};

// define pointer to singleton instance of `struct background`
static struct background * singleton = nullptr;

void background_delete (struct background ** self) {
    SDL_free(*self);
    *self = nullptr;
}

void background_draw (const struct background * self, SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, self->color.r, self->color.g, self->color.b, self->color.a);
    SDL_RenderClear(renderer);
}

void background_init (struct background * self) {
    *self = (struct background) {
        .color = (SDL_Color) {
            .r = 12,
            .g = 121,
            .b = 168,
            .a = SDL_ALPHA_OPAQUE,
        },
    };
}

struct background * background_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct background *) SDL_calloc(1, sizeof(struct background));
    if (singleton == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Could not allocate dynamic memory for struct background, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    return singleton;
}

void background_update (struct background * self, const struct timings * timings) {
    (void) self;
    (void) timings;
}
