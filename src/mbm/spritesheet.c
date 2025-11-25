#include "mbm/spritesheet.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_surface.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// declare properties of `struct spritesheet`
struct spritesheet {
    struct {
        SDL_FRect orange;
        SDL_FRect red;
        SDL_FRect yellow;
    } balloon;
    SDL_FRect barrel;
    SDL_FRect bullet;
    SDL_FRect collision;
    SDL_FRect flash;
    SDL_FRect moon;
    char * path;
    SDL_Texture * texture;
    SDL_FRect turret;
};

// define pointer to singleton instance of `struct singleton`
static struct spritesheet * singleton = nullptr;

void spritesheet_delete (struct spritesheet ** self) {
    // free dynamically allocated memory used by .texture
    SDL_DestroyTexture((*self)->texture);
    (*self)->texture = nullptr;

    // free dynamically allocated memory used by .path
    SDL_free((*self)->path);
    (*self)->path = nullptr;

    // free own resources
    free(*self);
    *self = nullptr;
}

char * spritesheet_get_path (struct spritesheet * self) {
    return self->path;
}

SDL_FRect * spritesheet_get_balloon_orange (struct spritesheet * self) {
    return &self->balloon.orange;
}

SDL_FRect * spritesheet_get_balloon_red (struct spritesheet * self) {
    return &self->balloon.red;
}

SDL_FRect * spritesheet_get_balloon_yellow (struct spritesheet * self) {
    return &self->balloon.yellow;
}

SDL_FRect * spritesheet_get_barrel (struct spritesheet * self) {
    return &self->barrel;
}

SDL_FRect * spritesheet_get_bullet (struct spritesheet * self) {
    return &self->bullet;
}

SDL_FRect * spritesheet_get_collision (struct spritesheet * self) {
    return &self->collision;
}

SDL_FRect * spritesheet_get_flash (struct spritesheet * self) {
    return &self->flash;
}

SDL_FRect * spritesheet_get_moon (struct spritesheet * self) {
    return &self->moon;
}

SDL_Texture * spritesheet_get_texture (struct spritesheet * self) {
    return self->texture;
}

void spritesheet_init (struct spritesheet * self, SDL_Renderer * renderer) {
    self->balloon.orange = (SDL_FRect) {
        .h = 12.0,
        .w = 9.0,
        .x = 184.0,
        .y = 1.0,
    };
    self->balloon.red = (SDL_FRect) {
        .h = 8.0,
        .w = 6.0,
        .x = 184.0,
        .y = 20.0,
    };
    self->balloon.yellow = (SDL_FRect) {
        .h = 16.0,
        .w = 12.0,
        .x = 166.0,
        .y = 1.0,
    };
    self->barrel = (SDL_FRect) {
        .h = 11.0,
        .w = 55.0,
        .x = 4.0,
        .y = 68.0,
    };
    self->bullet = (SDL_FRect) {
        .x = 188.0,
        .y = 38.0,
        .w = 5.0,
        .h = 5.0
    };
    self->collision = (SDL_FRect) {
         .x = 172.0,
         .y = 38.0,
         .w = 3.0,
         .h = 3.0,
    };
    self->flash = (SDL_FRect) {
         .x = 166.0,
         .y = 63.0,
         .w = 30.0,
         .h = 21.0,
    };
    self->moon = (SDL_FRect) {
        .x = 75.0,
        .y = 1.0,
        .w = 90.0,
        .h = 90.0,
    };
    self->turret = (SDL_FRect) {
       .h = 47.0,
       .w = 69.0,
       .x = 4.0,
       .y = 1.0,
    };
    const char * relpath = "../share/mbm/assets/images/sprites.bmp";
    SDL_asprintf(&self->path, "%s%s", SDL_GetBasePath(), relpath);
    SDL_Surface * surface = SDL_LoadBMP(self->path);
    if (surface == nullptr) {
        SDL_Log("Couldn't load sprites bitmap: %s", SDL_GetError());
        exit(1);
    }
    self->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (self->texture == nullptr) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        exit(1);
    }
    SDL_DestroySurface(surface);
    surface = nullptr;
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
