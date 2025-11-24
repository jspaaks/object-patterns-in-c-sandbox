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
    char * path;
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
    SDL_Surface * surface;
};

// define pointer to singleton instance of `struct singleton`
static struct spritesheet * singleton = nullptr;

void spritesheet_delete (struct spritesheet ** self) {
    SDL_free((*self)->path);
    free(*self);
    *self = nullptr;
}

SDL_Rect spritesheet_get_rect_balloon_orange (struct spritesheet * self) {
    return self->balloon.orange;
}

SDL_Rect spritesheet_get_rect_balloon_red (struct spritesheet * self) {
    return self->balloon.red;
}

SDL_Rect spritesheet_get_rect_balloon_yellow (struct spritesheet * self) {
    return self->balloon.yellow;
}

char * spritesheet_get_path (struct spritesheet * self) {
    return self->path;
}

SDL_Surface * spritesheet_get_surface (struct spritesheet * self) {
    return self->surface;
}

void spritesheet_init (struct spritesheet * self) {
    self->balloon.orange = (SDL_Rect) {
        .h = 12,
        .w = 9,
        .x = 184,
        .y = 1,
    };
    self->balloon.red = (SDL_Rect) {
        .h = 8,
        .w = 6,
        .x = 184,
        .y = 20,
    };
    self->balloon.yellow = (SDL_Rect) {
        .h = 16,
        .w = 12,
        .x = 166,
        .y = 1,
    };
    self->barrel = (SDL_Rect) {
        .h = 11,
        .w = 55,
        .x = 4,
        .y = 68,
    };
    self->bullet = (SDL_Rect) {
        .x = 188,
        .y = 38,
        .w = 5,
        .h = 5
    };
    self->collision = (SDL_Rect) {
         .x = 172,
         .y = 38,
         .w = 3,
         .h = 3,
    };
    self->flash = (SDL_Rect) {
         .x = 166,
         .y = 63,
         .w = 30,
         .h = 21,
    };
    self->moon = (SDL_Rect) {
        .x = 75,
        .y = 1,
        .w = 90,
        .h = 90,
    };
    self->turret = (SDL_Rect) {
       .h = 47,
       .w = 69,
       .x = 4,
       .y = 1,
    };
    const char * relpath = "../share/mbm/assets/images/sprites.bmp";
    SDL_asprintf(&self->path, "%s%s", SDL_GetBasePath(), relpath);
    self->surface = SDL_LoadBMP(self->path);
    if (self->surface == nullptr) {
        SDL_Log("Couldn't load sprites bitmap: %s", SDL_GetError());
        exit(1);
    }
}

struct spritesheet * spritesheet_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct spritesheet *) calloc(1, sizeof(struct spritesheet));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct spritesheet, aborting.\n");
        exit(1);
    }
    return singleton;
}
