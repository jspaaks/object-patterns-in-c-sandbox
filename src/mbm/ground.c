#include "mbm/ground.h"
#include "mbm/scene.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <stdio.h>
#include <stdlib.h>

// declare properties of `struct ground`
struct ground {
    SDL_Color color;
    SDL_FRect sim;
    SDL_FRect tgt;
    float thickness;
};

// define pointer to singleton instance of `struct ground`
static struct ground * singleton = nullptr;

void ground_delete (struct ground ** self) {
    free(*self);
    *self = nullptr;
}

void ground_draw (struct ground * self, SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, self->color.r, self->color.g, self->color.b, self->color.a);
    SDL_RenderFillRect(renderer, &self->tgt);
}

float ground_get_thickness (struct ground * self) {
    return self->thickness;
}

void ground_init (struct ground * self) {
    const float thickness = 80.0;
    const float width = 1280.0;
    const float height = 720.0;
    *self = (struct ground) {
        .color = (SDL_Color) {
            .r = 11,
            .g = 1,
            .b = 26,
            .a = SDL_ALPHA_OPAQUE,
        },
        .sim = (SDL_FRect) {
            .h = thickness,
            .w = width,
            .x = 0,
            .y = height - thickness,
        },
        .tgt = (SDL_FRect) {
            .h = thickness,
            .w = width,
            .x = 0,
            .y = height - thickness,
        },
        .thickness = thickness,
    };
}

struct ground * ground_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct ground *) calloc(1, sizeof(struct ground));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct ground, aborting.\n");
        exit(1);
    }
    return singleton;
}

void ground_update (struct ground * self, struct scene * scene) {
    float scale = scene_get_scale(scene);
    self->tgt = (SDL_FRect) {
        .h = self->sim.h * scale,
        .w = self->sim.w * scale,
        .x = scene_get_tgt_x(scene) + self->sim.x * scale,
        .y = scene_get_tgt_y(scene) + self->sim.y * scale,
    };
}
