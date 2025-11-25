#include "mbm/background.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include <stdio.h>
#include <stdlib.h>

// declare properties of `struct background`
struct background {
    SDL_Color color;
};

// define pointer to singleton instance of `struct background`
static struct background * singleton = nullptr;

void background_delete (struct background ** self) {
    free(*self);
    *self = nullptr;
}

void background_draw (struct background * self, SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, self->color.r, self->color.g, self->color.b, self->color.a);
    SDL_RenderClear(renderer);
}

void background_init (struct background * self) {
    *self = (struct background) {
        .color = (SDL_Color) {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = SDL_ALPHA_OPAQUE,
        },
    };
}

struct background * background_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct background *) calloc(1, sizeof(struct background));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct background, aborting.\n");
        exit(1);
    }
    return singleton;
}

void background_update (struct background *) {}
