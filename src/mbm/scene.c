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
    float scale;
    SDL_FRect sim;
    SDL_FRect tgt;
};

// define pointer to singleton instance of `struct scene`
static struct scene * singleton = nullptr;

void scene_delete (struct scene ** self) {
    free(*self);
    *self = nullptr;
}

void scene_draw (struct scene * self, SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, self->color.r, self->color.g, self->color.b, self->color.a);
    SDL_RenderFillRect(renderer, &self->tgt);
}

float scene_get_scale (struct scene * self) {
    return self->scale;
}

float scene_get_sim_h (struct scene * self) {
    return self->sim.h;
}

float scene_get_sim_w (struct scene * self) {
    return self->sim.w;
}

float scene_get_sim_x (struct scene * self) {
    return self->sim.x;
}

float scene_get_sim_y (struct scene * self) {
    return self->sim.y;
}

float scene_get_tgt_h (struct scene * self) {
    return self->tgt.h;
}

float scene_get_tgt_w (struct scene * self) {
    return self->tgt.w;
}

float scene_get_tgt_x (struct scene * self) {
    return self->tgt.x;
}

float scene_get_tgt_y (struct scene * self) {
    return self->tgt.y;
}

void scene_init (struct scene * self) {
    const int height = 720;
    const int width = 1280;
    *self = (struct scene) {
        .color = (SDL_Color) {
            .r = 0,
            .g = 22,
            .b = 43,
            .a = SDL_ALPHA_OPAQUE,
        },
        .ratio = (float) width / height,
        .scale = 1.0f,
        .sim = (SDL_FRect) {
            .h = height,
            .w = width,
            .x = 0,
            .y = 0,
        },
        .tgt = (SDL_FRect) {
            .h = height,
            .w = width,
            .x = 0,
            .y = 0,
        },
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

void scene_update (struct scene * self, SDL_Window * window) {
    int h = -1;
    int w = -1;
    SDL_GetWindowSize(window, &w, &h);

    float ratio = (float) w / h;
    if (ratio < self->ratio) {
        // too tall
        self->scale = (float) w / self->sim.w;
        float h0 = self->scale * self->sim.w / self->ratio;
        self->tgt = (SDL_FRect) {
            .h = h0,
            .w = self->scale * self->sim.w,
            .x = 0,
            .y = ((float) h - h0) / 2,
        };
    } else {
        // too wide
        self->scale = (float) h / self->sim.h;
        float w0 = self->scale * self->sim.h * self->ratio;
        self->tgt = (SDL_FRect) {
            .h = self->scale * self->sim.h,
            .w = w0,
            .x = ((float) w - w0) / 2,
            .y = 0,
        };
    }
}
