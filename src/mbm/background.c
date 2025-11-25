#include "mbm/background.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include <stdio.h>
#include <stdlib.h>

// declare properties of `struct background`
struct background {
    SDL_Color color;
    SDL_Renderer * renderer;
};

// define pointer to singleton instance of `struct background`
static struct background * singleton = nullptr;

void background_delete (struct background ** self) {
    free(*self);
    *self = nullptr;
}

void background_draw (struct background * self) {
    SDL_SetRenderDrawColor(self->renderer,
                           self->color.r, self->color.g, self->color.b, self->color.a);
    SDL_RenderClear(self->renderer);
}

void background_init (struct background * self, SDL_Renderer * renderer) {
    *self = (struct background) {
        .color = (SDL_Color) {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = SDL_ALPHA_OPAQUE,
        },
        .renderer = renderer,
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
