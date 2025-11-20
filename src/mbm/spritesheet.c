#include "mbm/spritesheet.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_surface.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


// declare properties of `struct spritesheet`
struct spritesheet {
    void * placeholder;
};

// declare properties of `struct spritesheet`
struct spritesheet_shared {
    char * path;
    struct {
        struct {
            SDL_Rect orange;
            SDL_Rect red;
            SDL_Rect yellow;
        } balloon;
        SDL_Rect barrel;
        SDL_Rect bullet;
        SDL_Rect collision;
        SDL_Rect flash;
        SDL_Rect moon;
        SDL_Rect turret;
    } rects;
    SDL_Surface * surface;
};

// define properties of `struct spritesheet_shared`
static struct spritesheet_shared shared = {
    .rects = {
        .balloon = {
            .orange = {},
            .red = {},
            .yellow = {},
        },
        .barrel = {},
        .bullet = {},
        .collision = {},
        .flash = {},
        .moon = {},
        .turret = {},
    },
    .path = nullptr,
    .surface = nullptr,
};

void spritesheet_delete (struct spritesheet ** self) {
    SDL_free(shared.path);
    free(*self);
    *self = nullptr;
}

SDL_Rect spritesheet_get_rect_balloon_orange (void) {
    return shared.rects.balloon.orange;
}

SDL_Rect spritesheet_get_rect_balloon_red (void) {
    return shared.rects.balloon.red;
}

SDL_Rect spritesheet_get_rect_balloon_yellow (void) {
    return shared.rects.balloon.yellow;
}

char * spritesheet_get_path (void) {
    return shared.path;
}

size_t spritesheet_memsize (void) {
    return sizeof(struct spritesheet);
}

SDL_Surface * spritesheet_get_surface (void) {
    return shared.surface;
}

void spritesheet_init (void) {
    shared.rects.balloon.orange = (SDL_Rect) {
        .h = 12,
        .w = 9,
        .x = 184,
        .y = 1,
    };
    shared.rects.balloon.red = (SDL_Rect) {
        .h = 8,
        .w = 6,
        .x = 184,
        .y = 20,
    };
    shared.rects.balloon.yellow = (SDL_Rect) {
        .h = 16,
        .w = 12,
        .x = 166,
        .y = 1,
    };
    shared.rects.barrel = (SDL_Rect) {
        .h = 11,
        .w = 55,
        .x = 4,
        .y = 68,
    };
    shared.rects.bullet = (SDL_Rect) {
        .x = 188,
        .y = 38,
        .w = 5,
        .h = 5
    };
    shared.rects.collision = (SDL_Rect) {
         .x = 172,
         .y = 38,
         .w = 3,
         .h = 3,
    };
    shared.rects.flash = (SDL_Rect) {
         .x = 166,
         .y = 63,
         .w = 30,
         .h = 21,
    };
    shared.rects.moon = (SDL_Rect) {
        .x = 75,
        .y = 1,
        .w = 90,
        .h = 90,
    };
    shared.rects.turret = (SDL_Rect) {
       .h = 47,
       .w = 69,
       .x = 4,
       .y = 1,
    };
    const char * relpath = "../share/mbm/assets/images/sprites.bmp";

    SDL_asprintf(&shared.path, "%s%s", SDL_GetBasePath(), relpath);
    shared.surface = SDL_LoadBMP(shared.path);
    if (shared.surface == nullptr) {
        SDL_Log("Couldn't load sprites bitmap: %s", SDL_GetError());
        exit(1);
    }
}

struct spritesheet * spritesheet_new (void) {
    struct spritesheet * p = (struct spritesheet *) calloc(1, sizeof(struct spritesheet));
    if (p == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct spritesheet, aborting.\n");
        exit(1);
    }
    return p;
}
