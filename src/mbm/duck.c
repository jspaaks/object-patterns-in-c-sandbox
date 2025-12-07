#include "mbm/duck.h"
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_filesystem.h"  // SDL_GetBasePath
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture, SDL_CreateTextureFromSurface, SDL_DestroyTexture, SDL_RenderTexture
#include "SDL3/SDL_stdinc.h"      // SDL_asprintf, SDL_free, SDL_calloc
#include "SDL3/SDL_surface.h"     // SDL_Surface, SDL_LoadBMP, SDL_DestroySurface
#include <stdlib.h>               // exit

// define enum for animation states
enum animation_state: uint8_t {
    ANIMATION_STATE_IDLE = 0,
    ANIMATION_STATE_WALKING,
    ANIMATION_STATE_COUNT,
};

// declare properties of `struct frame`
struct frame {
    SDL_FRect src;
    uint64_t duration;
};

// declare properties of `struct animation`
struct animation {
    uint64_t duration;
    struct frame frames[10];
    int nframes;
};

// declare properties of `struct duck`
struct duck {
    struct animation animations[ANIMATION_STATE_COUNT];
    enum animation_state ianim;
    int iframe;
    uint64_t t_frame_expires;
    uint64_t t_anim_started;
    SDL_Texture * texture;
    SDL_FRect wld;
};

// define pointer to singleton instance of `struct duck`
static struct duck * singleton = nullptr;

// forward declarations of functions defined below
static SDL_Texture * load_duck_texture (const char * relpath, SDL_Renderer * renderer);

static SDL_Texture * load_duck_texture (const char * relpath, SDL_Renderer * renderer) {

    // create surface given relative path
    char * path = nullptr;
    SDL_asprintf(&path, "%s%s", SDL_GetBasePath(), relpath);
    SDL_Surface * surface = SDL_LoadBMP(path);
    if (surface == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't load surface for duck, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }

    // create texture from surface
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't create texture for duck, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }

    // set texture scale mode
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    // free dynamically allocated memory used by surface and by path
    SDL_DestroySurface(surface);
    surface = nullptr;
    SDL_free(path);
    path = nullptr;

    // return texture
    return texture;
}

void duck_delete (struct duck ** self) {
    SDL_DestroyTexture((*self)->texture);
    (*self)->texture = nullptr;
    SDL_free(*self);
    *self = nullptr;
}

void duck_draw (const struct duck * self, SDL_Renderer * renderer) {
    uint8_t ianim = self->ianim;
    uint8_t iframe = self->iframe;
    SDL_FRect src = self->animations[ianim].frames[iframe].src;
    SDL_RenderTexture(renderer, self->texture, &src, &self->wld);
}

void duck_init (struct duck * self, SDL_Renderer * renderer, const struct dims * dims) {

    float h = 32.0f;
    float w = 32.0f;

    *self = (struct duck) {
        .animations = {
            [ANIMATION_STATE_IDLE] = (struct animation) {
                .frames = {
                    [0] = (struct frame) {
                        .src = (SDL_FRect) {
                            .h = h,
                            .w = w,
                            .x = 0 * w,
                            .y = ANIMATION_STATE_IDLE * h,
                        },
                        .duration = (uint64_t) 1e5,
                    },
                },
                .duration = (uint64_t) 1e5,
                .nframes = 1,
            },
            [ANIMATION_STATE_WALKING] = (struct animation) {
                .frames = {
                    [0] = (struct frame) {
                        .src = (SDL_FRect) {
                            .h = h,
                            .w = w,
                            .x = 0 * w,
                            .y = ANIMATION_STATE_WALKING * h,
                        },
                        .duration = (uint64_t) 1e5,
                    },
                    [1] = (struct frame) {
                        .src = (SDL_FRect) {
                            .h = h,
                            .w = w,
                            .x = 1 * w,
                            .y = ANIMATION_STATE_WALKING * h,
                        },
                        .duration = (uint64_t) 1e5,
                    },
                    [2] = (struct frame) {
                        .src = (SDL_FRect) {
                            .h = h,
                            .w = w,
                            .x = 2 * w,
                            .y = ANIMATION_STATE_WALKING * h,
                        },
                        .duration = (uint64_t) 1e5,
                    },
                    [3] = (struct frame) {
                        .src = (SDL_FRect) {
                            .h = h,
                            .w = w,
                            .x = 3 * w,
                            .y = ANIMATION_STATE_WALKING * h,
                        },
                        .duration = (uint64_t) 1e5,
                    },
                    [4] = (struct frame) {
                        .src = (SDL_FRect) {
                            .h = h,
                            .w = w,
                            .x = 4 * w,
                            .y = ANIMATION_STATE_WALKING * h,
                        },
                        .duration = (uint64_t) 1e5,
                    },
                    [5] = (struct frame) {
                        .src = (SDL_FRect) {
                            .h = h,
                            .w = w,
                            .x = 5 * w,
                            .y = ANIMATION_STATE_WALKING * h,
                        },
                        .duration = (uint64_t) 1e5,
                    },
                },
                .duration = (uint64_t) 6e5,
                .nframes = 6,
            },
        },
        .ianim = ANIMATION_STATE_IDLE,
        .iframe = 0,
        .t_anim_started = (uint64_t) 0,
        .t_frame_expires = (uint64_t) 0,
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

void duck_set_animation_state_idle (struct duck * self, const struct timings * timings) {
    self->ianim = ANIMATION_STATE_IDLE;
    self->iframe = 0;
    self->t_anim_started = timings_get_frame_timestamp(timings);
    self->t_frame_expires = self->animations[self->ianim].frames[self->iframe].duration;

}

void duck_set_animation_state_walking (struct duck * self, const struct timings * timings) {
    self->ianim = ANIMATION_STATE_WALKING;
    self->iframe = 0;
    self->t_anim_started = timings_get_frame_timestamp(timings);
    self->t_frame_expires = self->animations[self->ianim].frames[self->iframe].duration;
}

void duck_update (struct duck * self, const struct timings * timings) {
    uint64_t tnow = timings_get_frame_timestamp(timings);
    if (tnow > self->t_frame_expires) {
        struct animation * animation = &self->animations[self->ianim];
        self->iframe = 0;
        uint64_t tmod = (tnow - self->t_anim_started) % animation->duration;
        uint64_t tacc = animation->frames[self->iframe].duration;
        while (tacc < tmod && self->iframe < animation->nframes - 1) {
            self->iframe++;
            tacc += animation->frames[self->iframe].duration;
        }
        self->t_frame_expires = tacc;
    }
}
