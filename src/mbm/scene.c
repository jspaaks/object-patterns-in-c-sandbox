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
    struct {
        SDL_FRect sim;
        SDL_FRect tgt;
    } rects;
    SDL_Renderer * renderer;
    SDL_Window * window;
};

// define pointer to singleton instance of `struct scene`
static struct scene * scenep = nullptr;

void scene_delete (struct scene ** self) {
    free(*self);
    *self = nullptr;
}

SDL_FRect scene_get_rect_scene (struct scene * self) {
    return self->rects.sim;
}

void scene_draw (struct scene * self) {
    SDL_SetRenderDrawColor(self->renderer, self->color.r, self->color.g, self->color.b, self->color.a);
    SDL_RenderFillRect(self->renderer, &self->rects.tgt);
}

void scene_update (struct scene * self) {
    int w = -1;
    int h = -1;
    SDL_GetWindowSize(self->window, &w, &h);

    //int ratio = h / w;
    //if (ratio < scene->ratio) {
    //} else {
    //}

    self->rects.tgt = (SDL_FRect) {
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
    self->rects.sim = (SDL_FRect) {
        .h = h,
        .w = w,
        .x = 0,
        .y = 0,
    };
    self->renderer = renderer;
    self->window = window;
}

struct scene * scene_new (void) {
    if (scenep != nullptr) {
        // memory has already been allocated for `scene`, and `scene` is singleton
        return scenep;
    }
    scenep = (struct scene *) calloc(1, sizeof(struct scene));
    if (scenep == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct scene, aborting.\n");
        exit(1);
    }
    return scenep;
}
