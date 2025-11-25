#include "mbm/moon.h"
#include "mbm/scene.h"
#include "mbm/spritesheet.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <stdio.h>
#include <stdlib.h>

// declare properties of `struct moon`
struct moon {
    SDL_FRect sim;
    SDL_FRect tgt;
};

// define pointer to singleton instance of `struct moon`
static struct moon * singleton = nullptr;

void moon_delete (struct moon ** self) {
    free(*self);
    *self = nullptr;
}

void moon_draw (struct moon * self, struct spritesheet * spritesheet, SDL_Renderer * renderer) {
    SDL_Texture * texture = spritesheet_get_texture(spritesheet);
    SDL_FRect * src = spritesheet_get_moon(spritesheet);
    SDL_RenderTexture(renderer, texture, src, &self->tgt);
}

void moon_init (struct moon * self, struct scene * scene) {
    *self = (struct moon) {
        .sim = (SDL_FRect) {
            .h = 90.0,
            .w = 90.0,
            .x = scene_get_sim_w(scene) - 130.0,
            .y = 40.0,
        },
        .tgt = (SDL_FRect) {
            .h = -1.0,
            .w = -1.0,
            .x = -1.0,
            .y = -1.0,
        },
    };
}

struct moon * moon_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct moon *) calloc(1, sizeof(struct moon));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct moon, aborting.\n");
        exit(1);
    }
    return singleton;
}

void moon_update (struct moon * self, struct scene * scene) {
    float scale = scene_get_scale(scene);
    self->tgt = (SDL_FRect) {
        .h = self->sim.h * scale,
        .w = self->sim.w * scale,
        .x = scene_get_tgt_x(scene) + self->sim.x * scale,
        .y = scene_get_tgt_y(scene) + self->sim.y * scale,
    };
}
