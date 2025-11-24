#include "mbm/scene.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// declare properties of `struct scene`
struct scene {
    SDL_Color color;
    float ratio;
    SDL_FRect sim;
    SDL_FRect tgt;
    SDL_Renderer * renderer;
    SDL_Window * window;
};

// define pointer to singleton instance of `struct scene`
static struct scene * singleton = nullptr;

void scene_delete (struct scene ** self) {
    free(*self);
    *self = nullptr;
}

SDL_FRect scene_get_rect_scene (struct scene * self) {
    return self->sim;
}

void scene_draw (struct scene * self) {
    SDL_SetRenderDrawColor(self->renderer, self->color.r, self->color.g, self->color.b, self->color.a);
    SDL_RenderFillRect(self->renderer, &self->tgt);
}

void scene_update (struct scene * self) {
    int h = -1;
    int w = -1;
    SDL_GetWindowSize(self->window, &w, &h);

    float ratio = (float) w / h;
    if (ratio < self->ratio) {
        // too tall
        float scale = (float) w / self->sim.w;
        float h0 = scale * self->sim.w / self->ratio;
        self->tgt = (SDL_FRect) {
            .h = h0,
            .w = scale * self->sim.w,
            .x = 0,
            .y = ((float) h - h0) / 2,
        };
    } else {
        // too wide
        float scale = (float) h / self->sim.h;
        float w0 = scale * self->sim.h * self->ratio;
        self->tgt = (SDL_FRect) {
            .h = scale * self->sim.h,
            .w = w0,
            .x = ((float) w - w0) / 2,
            .y = 0,
        };
    }
}

void scene_init (struct scene * self, SDL_Window * window, SDL_Renderer * renderer) {
    int h = 720;
    int w = 1280;
    *self = (struct scene) {
        .color = (SDL_Color) {
            .r = 0,
            .g = 22,
            .b = 43,
            .a = SDL_ALPHA_OPAQUE,
        },
        .ratio = (float) w / h,
        .renderer = renderer,
        .sim = (SDL_FRect) {
            .h = h,
            .w = w,
            .x = 0,
            .y = 0,
        },
        .tgt = (SDL_FRect) {
            .h = h,
            .w = w,
            .x = 0,
            .y = 0,
        },
        .window = window,
    };
}

struct scene * scene_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct scene *) calloc(1, sizeof(struct scene));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct scene, aborting.\n");
        exit(1);
    }
    return singleton;
}
