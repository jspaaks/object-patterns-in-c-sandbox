#include "mbm/barrel.h"           // type `struct barrel`
#include "mbm/scene.h"            // type `struct scene`
#include "mbm/spritesheet.h"      // type `struct spritesheet`
#include "mbm/turret.h"           // type `Turret` and associated functions
#include "SDL3/SDL_pixels.h"      // type `SDL_Color`
#include "SDL3/SDL_rect.h"        // type `SDL_FRect`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <stdio.h>
#include <stdlib.h>

// declare properties of `struct barrel`
struct barrel {
    SDL_FRect sim;
    SDL_FRect * src;
    SDL_FRect tgt;
};

// define pointer to singleton instance of `struct barrel`
static struct barrel * singleton = nullptr;

void barrel_delete (struct barrel ** self) {
    free(*self);
    *self = nullptr;
}

void barrel_draw (struct barrel * self, struct spritesheet * spritesheet, SDL_Renderer * renderer) {
    SDL_Texture * texture = spritesheet_get_texture(spritesheet);
    SDL_RenderTexture(renderer, texture, self->src, &self->tgt);
}

void barrel_init (struct barrel * self, struct spritesheet * spritesheet, struct turret * turret) {
    const float h = 11.0;
    *self = (struct barrel) {};
    self->src = spritesheet_get_barrel(spritesheet);
    self->sim = (SDL_FRect) {
        .h = h,
        .w = 55.0,
        .x = turret_get_sim_x(turret) + turret_get_sim_w(turret) / 2,
        .y = turret_get_sim_y(turret) + turret_get_sim_w(turret) / 2 - (h - 1.0) / 2,
    };
}

struct barrel * barrel_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct barrel *) calloc(1, sizeof(struct barrel));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct barrel, aborting.\n");
        exit(1);
    }
    return singleton;
}

void barrel_update (struct barrel * self, struct scene * scene) {
    float scale = scene_get_scale(scene);
    self->tgt = (SDL_FRect) {
        .h = self->sim.h * scale,
        .w = self->sim.w * scale,
        .x = scene_get_tgt_x(scene) + self->sim.x * scale,
        .y = scene_get_tgt_y(scene) + self->sim.y * scale,
    };
}
