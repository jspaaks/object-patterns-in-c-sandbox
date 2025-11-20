#include "mbm/scene.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


// declare properties of `struct scene`
struct scene {
    float ratio;
    struct {
        SDL_FRect sim;
        SDL_Rect tgt;
    } rects;
};

// define pointer to singleton instance of `struct scene`
static struct scene * scene = nullptr;

void scene_deinit (void) {}

void scene_delete (struct scene ** scene) {
    free(*scene);
    *scene = nullptr;
}

SDL_FRect scene_get_rect_scene (void) {
    return scene->rects.sim;
}

void scene_draw (SDL_Renderer * renderer) {
    SDL_RenderRect(renderer, &scene->rects.sim);
}

void scene_update (void) {
    scene->rects.tgt = (SDL_Rect) {
        .h = 20,
        .w = 100,
        .x = 10,
        .y = 20,
    };
}

void scene_init (void) {
    int h = 720;
    int w = 1024;
    scene->rects.sim = (SDL_FRect) {
        .h = h,
        .w = w,
        .x = 0,
        .y = 0,
    };
    scene->ratio = (float) w / h;
}

struct scene * scene_new (void) {
    if (scene != nullptr) {
        // return a pointer to existing singleton instance of scene
        return scene;
    }
    scene = (struct scene *) calloc(1, sizeof(struct scene));
    if (scene == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct scene, aborting.\n");
        exit(1);
    }
    return scene;
}
