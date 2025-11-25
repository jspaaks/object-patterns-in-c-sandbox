#include "mbm/balloon.h"
#include "mbm/spritesheet.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declare properties of `struct balloon`
struct balloon {
    float u;
    float v;
    SDL_FRect sim;
};

//// declare properties of `struct balloon_shared`
//struct balloon_shared {
//    SDL_Rect src;
//    SDL_Texture * texture;
//};

//// define properties of `struct balloon_shared`
//static struct balloon_shared shared = {};

void balloon_delete (struct balloon ** self) {
    free(*self);
    *self = nullptr;
}

SDL_FRect balloon_get_sim (struct balloon * self) {
    return self->sim;
}

void balloon_init (struct balloon * self) {
    self->sim = (SDL_FRect) {
        .h = 12.0,
        .w = 9.0,
        .x = 30.0,
        .y = 40.0,
    };
    self->u = 0.0;
    self->v = -30.0;

    //shared.texture = SDL_CreateTextureFromSurface(renderer, surface);
    //if (shared.texture == nullptr) {
    //    SDL_Log("Couldn't create static texture: %s", SDL_GetError());
    //    return SDL_APP_FAILURE;
    //}
    //SDL_DestroySurface(surface);

}

size_t balloon_memsize (void) {
    return sizeof(struct balloon);
}

struct balloon * balloon_new (void) {
    struct balloon * p = (struct balloon *) calloc(1, sizeof(struct balloon));
    if (p == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct balloon, aborting.\n");
        exit(1);
    }
    return p;
}
