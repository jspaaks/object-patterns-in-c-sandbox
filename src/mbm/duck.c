#include "mbm/duck.h"
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_filesystem.h"  // SDL_GetBasePath
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture, SDL_CreateTextureFromSurface, SDL_DestroyTexture, SDL_RenderTexture
#include "SDL3/SDL_stdinc.h"      // SDL_asprintf, SDL_free, SDL_calloc
#include "SDL3/SDL_surface.h"     // SDL_Surface, SDL_LoadBMP, SDL_DestroySurface
#include <stdlib.h>               // exit

// declare properties of `struct duck`
struct duck {
    SDL_FRect src;
    SDL_Texture * texture;
    SDL_FRect wld;
};

// define pointer to singleton instance of `struct duck`
static struct duck * singleton = nullptr;

// forward declarations of functions defined below
static SDL_Texture * load_duck_texture (const char * relpath, SDL_Renderer * renderer);

static SDL_Texture * load_duck_texture (const char * relpath, SDL_Renderer * renderer) {
    char * path = nullptr;
    SDL_asprintf(&path, "%s%s", SDL_GetBasePath(), relpath);
    SDL_Surface * surface = SDL_LoadBMP(path);
    if (surface == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't load surface for duck, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't create texture for duck, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    // free dynamically allocated memory used by surface and by path
    SDL_DestroySurface(surface);
    surface = nullptr;
    SDL_free(path);
    path = nullptr;
    return texture;
}

void duck_delete (struct duck ** self) {
    SDL_DestroyTexture((*self)->texture);
    (*self)->texture = nullptr;
    SDL_free(*self);
    *self = nullptr;
}

void duck_draw (const struct duck * self, SDL_Renderer * renderer) {
    SDL_RenderTexture(renderer, self->texture, &self->src, &self->wld);
    SDL_SetTextureScaleMode(self->texture, SDL_SCALEMODE_NEAREST);
}

void duck_init (struct duck * self, SDL_Renderer * renderer, const struct dims * dims) {
    *self = (struct duck) {
        .src = (SDL_FRect) {
            .h = 32.0f,
            .w = 32.0f,
            .x = 0.0f,
            .y = 0.0f,
        },
        .texture = load_duck_texture("../share/mbm/assets/images/duck.bmp", renderer),
        .wld = (SDL_FRect) {
            .h = 32.0f,
            .w = 32.0f,
            .x = 100.0f,
            .y = dims->world.h - 32.0f - 32.0f,
        },
    };
}

struct duck * duck_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct duck *) SDL_calloc(1, sizeof(struct duck));
    if (singleton == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "ERROR allocating dynamic memory for struct duck, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    return singleton;
}

void duck_update (struct duck * self, const struct timings * timings) {
    (void) self;
    (void) timings;
}
