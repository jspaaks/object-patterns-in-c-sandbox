#include "mbm/turret.h"           // type `struct turret`
#include "mbm/scene.h"            // type `struct scene`
#include "mbm/spritesheet.h"      // type `struct spritesheet`
#include "SDL3/SDL_pixels.h"      // type `SDL_Color`
#include "SDL3/SDL_rect.h"        // type `SDL_FRect`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <stdio.h>
#include <stdlib.h>

// declare properties of `struct turret`
struct turret {
    SDL_FRect sim;
    SDL_FRect tgt;
};

// define pointer to singleton instance of `struct turret`
static struct turret * singleton = nullptr;

void turret_delete (struct turret ** self) {
    free(*self);
    *self = nullptr;
}

void turret_draw (struct turret * self, struct spritesheet * spritesheet, SDL_Renderer * renderer) {
    SDL_Texture * texture = spritesheet_get_texture(spritesheet);
    SDL_FRect * src = spritesheet_get_turret(spritesheet);
    SDL_RenderTexture(renderer, texture, src, &self->tgt);
}

float turret_get_sim_w (struct turret * self) {
    return self->sim.w;
}

float turret_get_sim_h (struct turret * self) {
    return self->sim.h;
}

float turret_get_sim_x (struct turret * self) {
    return self->sim.x;
}

float turret_get_sim_y (struct turret * self) {
    return self->sim.y;
}

void turret_init (struct turret * self, struct scene * scene, struct ground * ground) {
    const float height = 47.0;
    *self = (struct turret) {
        .sim = (SDL_FRect) {
            .h = height,
            .w = 69.0,
            .x = 180.0,
            .y = scene_get_sim_h(scene) - ground_get_thickness(ground) - height,
        },
        .tgt = (SDL_FRect) {
            .h = -1.0,
            .w = -1.0,
            .x = -1.0,
            .y = -1.0,
        },
    };
}

struct turret * turret_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct turret *) calloc(1, sizeof(struct turret));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct turret, aborting.\n");
        exit(1);
    }
    return singleton;
}

void turret_update (struct turret * self, struct scene * scene) {
    float scale = scene_get_scale(scene);
    self->tgt = (SDL_FRect) {
        .h = self->sim.h * scale,
        .w = self->sim.w * scale,
        .x = scene_get_tgt_x(scene) + self->sim.x * scale,
        .y = scene_get_tgt_y(scene) + self->sim.y * scale,
    };
}
