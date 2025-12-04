#include "mbm/wheel.h"
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_filesystem.h"  // SDL_GetBasePath
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture, SDL_CreateTextureFromSurface, SDL_DestroyTexture, SDL_RenderTexture
#include "SDL3/SDL_stdinc.h"      // SDL_asprintf, SDL_free, SDL_calloc
#include "SDL3/SDL_surface.h"     // SDL_Surface, SDL_LoadBMP, SDL_DestroySurface
#include <stdlib.h>               // exit

// declare properties of `struct wheel`
struct wheel {
    float angle;                    // degrees
    float angular_velocity;         // degrees / second
    SDL_FPoint pivot;
    SDL_FRect src;
    SDL_Texture * texture;
    SDL_FRect wld;
};

// define pointer to singleton instance of `struct wheel`
static struct wheel * singleton = nullptr;

// forward declarations of functions defined below
static SDL_Texture * load_wheel_texture (const char * relpath, SDL_Renderer * renderer);

static SDL_Texture * load_wheel_texture (const char * relpath, SDL_Renderer * renderer) {
    char * path = nullptr;
    SDL_asprintf(&path, "%s%s", SDL_GetBasePath(), relpath);
    SDL_Surface * surface = SDL_LoadBMP(path);
    if (surface == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't load surface for wheel, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't create texture for wheel, aborting; %s\n",
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

void wheel_delete (struct wheel ** self) {
    SDL_DestroyTexture((*self)->texture);
    (*self)->texture = nullptr;
    SDL_free(*self);
    *self = nullptr;
}

void wheel_draw (const struct wheel * self, SDL_Renderer * renderer) {
    SDL_RenderTextureRotated(renderer, self->texture, &self->src, &self->wld, self->angle,
                             &self->pivot, SDL_FLIP_NONE);
    SDL_SetTextureScaleMode(self->texture, SDL_SCALEMODE_NEAREST);
}

void wheel_init (struct wheel * self, SDL_Renderer * renderer, const struct dims * dims) {
    *self = (struct wheel) {
        .angle = 0.0f,
        .angular_velocity = 60.0f,
        .pivot = (SDL_FPoint) {
            .x = 8.0f,
            .y = 8.0f,
        },
        .src = (SDL_FRect) {
            .h = 32.0f,
            .w = 32.0f,
            .x = 0.0f,
            .y = 0.0f,
        },
        .texture = load_wheel_texture("../share/mbm/assets/images/wheel.bmp", renderer),
        .wld = (SDL_FRect) {
            .h = 16.0f,
            .w = 16.0f,
            .x = 100.0f,
            .y = dims->world.h - 18.0f,
        },
    };
}

struct wheel * wheel_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct wheel *) SDL_calloc(1, sizeof(struct wheel));
    if (singleton == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "ERROR allocating dynamic memory for struct wheel, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    return singleton;
}

void wheel_update (struct wheel * self, const struct timings * timings) {
    float dt = timings_get_frame_duration(timings);
    self->angle += self->angular_velocity * dt;
}
