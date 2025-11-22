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
    int w = -1;
    int h = -1;
    SDL_GetWindowSize(self->window, &w, &h);

    //int ratio = h / w;
    //if (ratio < self->ratio) {
    //} else {
    //}

    self->tgt = (SDL_FRect) {
        .h = h - 20,
        .w = w - 20,
        .x = 10,
        .y = 10,
    };
}

void scene_init (struct scene * self, SDL_Window * window, SDL_Renderer * renderer) {
    self->color = (SDL_Color) {
        .r = 128,
        .g = 0,
        .b = 128,
        .a = SDL_ALPHA_OPAQUE,
    };
    int h = 720;
    int w = 1024;
    self->ratio = (float) w / h;
    self->sim = (SDL_FRect) {
        .h = h,
        .w = w,
        .x = 0,
        .y = 0,
    };
    self->renderer = renderer;
    self->window = window;
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
