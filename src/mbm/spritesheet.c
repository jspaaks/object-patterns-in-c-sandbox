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

// define pointer to singleton instance of `struct singleton`
static struct spritesheet * spritesheet = nullptr;

void spritesheet_deinit (void) {
    SDL_free(spritesheet->path);
}

void spritesheet_delete (struct spritesheet ** spritesheet) {
    free(*spritesheet);
    *spritesheet = nullptr;
}

SDL_Rect spritesheet_get_rect_balloon_orange (void) {
    return spritesheet->rects.balloon.orange;
}

SDL_Rect spritesheet_get_rect_balloon_red (void) {
    return spritesheet->rects.balloon.red;
}

SDL_Rect spritesheet_get_rect_balloon_yellow (void) {
    return spritesheet->rects.balloon.yellow;
}

char * spritesheet_get_path (void) {
    return spritesheet->path;
}

size_t spritesheet_memsize (void) {
    return sizeof(struct spritesheet);
}

SDL_Surface * spritesheet_get_surface (void) {
    return spritesheet->surface;
}

void spritesheet_init (void) {
    spritesheet->rects.balloon.orange = (SDL_Rect) {
        .h = 12,
        .w = 9,
        .x = 184,
        .y = 1,
    };
    spritesheet->rects.balloon.red = (SDL_Rect) {
        .h = 8,
        .w = 6,
        .x = 184,
        .y = 20,
    };
    spritesheet->rects.balloon.yellow = (SDL_Rect) {
        .h = 16,
        .w = 12,
        .x = 166,
        .y = 1,
    };
    spritesheet->rects.barrel = (SDL_Rect) {
        .h = 11,
        .w = 55,
        .x = 4,
        .y = 68,
    };
    spritesheet->rects.bullet = (SDL_Rect) {
        .x = 188,
        .y = 38,
        .w = 5,
        .h = 5
    };
    spritesheet->rects.collision = (SDL_Rect) {
         .x = 172,
         .y = 38,
         .w = 3,
         .h = 3,
    };
    spritesheet->rects.flash = (SDL_Rect) {
         .x = 166,
         .y = 63,
         .w = 30,
         .h = 21,
    };
    spritesheet->rects.moon = (SDL_Rect) {
        .x = 75,
        .y = 1,
        .w = 90,
        .h = 90,
    };
    spritesheet->rects.turret = (SDL_Rect) {
       .h = 47,
       .w = 69,
       .x = 4,
       .y = 1,
    };
    const char * relpath = "../share/mbm/assets/images/sprites.bmp";
    SDL_asprintf(&spritesheet->path, "%s%s", SDL_GetBasePath(), relpath);
    spritesheet->surface = SDL_LoadBMP(spritesheet->path);
    if (spritesheet->surface == nullptr) {
        SDL_Log("Couldn't load sprites bitmap: %s", SDL_GetError());
        exit(1);
    }
}

struct spritesheet * spritesheet_new (void) {
    if (spritesheet != nullptr) {
        // return a pointer to existing singleton instance of spritesheet
        return spritesheet;
    }
    spritesheet = (struct spritesheet *) calloc(1, sizeof(struct spritesheet));
    if (spritesheet == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct spritesheet, aborting.\n");
        exit(1);
    }
    return spritesheet;
}
