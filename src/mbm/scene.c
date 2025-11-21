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
static struct scene * scene = nullptr;

void scene_deinit (void) {}

void scene_delete (void) {
    free(scene);
    scene = nullptr;
}

SDL_FRect scene_get_rect_scene (void) {
    return scene->rects.sim;
}

void scene_draw (void) {
    SDL_SetRenderDrawColor(scene->renderer, scene->color.r, scene->color.g, scene->color.b, scene->color.a);
    SDL_RenderFillRect(scene->renderer, &scene->rects.tgt);
}

void scene_update (void) {
    int w = -1;
    int h = -1;
    SDL_GetWindowSize(scene->window, &w, &h);

    //int ratio = h / w;
    //if (ratio < scene->ratio) {
    //} else {
    //}

    scene->rects.tgt = (SDL_FRect) {
        .h = h - 20,
        .w = w - 20,
        .x = 10,
        .y = 10,
    };
}

void scene_init (SDL_Window * window, SDL_Renderer * renderer) {
    scene->color = (SDL_Color) {
        .r = 128,
        .g = 0,
        .b = 128,
        .a = SDL_ALPHA_OPAQUE,
    };
    int h = 720;
    int w = 1024;
    scene->ratio = (float) w / h;
    scene->rects.sim = (SDL_FRect) {
        .h = h,
        .w = w,
        .x = 0,
        .y = 0,
    };
    scene->renderer = renderer;
    scene->window = window;
}

void scene_new (void) {
    if (scene != nullptr) {
        // memory has already been allocated for `scene`, and `scene` is singleton
        return;
    }
    scene = (struct scene *) calloc(1, sizeof(struct scene));
    if (scene == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct scene, aborting.\n");
        exit(1);
    }
    return;
}
