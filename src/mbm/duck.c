#include "mbm/duck.h"
#include "animations.h"           // struct animations and associated functions
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture, SDL_RenderTexture
#include "SDL3/SDL_stdinc.h"      // SDL_free, SDL_calloc
#include <stdlib.h>               // exit

// define enum for animation states
enum animation_state: uint8_t {
    ANIMATION_STATE_IDLE = 0,
    ANIMATION_STATE_WALKING,
    ANIMATION_STATE_COUNT,
};

// declare properties of `struct duck`
struct duck {
    struct animations * animations;
    enum animation_state ianim;
    int iframe;
    int64_t t_frame_expires;
    int64_t anim_phase_shift;
    SDL_FRect wld;
};

// define pointer to singleton instance of `struct duck`
static struct duck * singleton = nullptr;

void duck_delete (struct duck ** self) {
    animations_delete(&(*self)->animations);
    (*self)->animations = nullptr;
    SDL_free(*self);
    *self = nullptr;
}

void duck_draw (const struct duck * self, SDL_Renderer * renderer) {
    SDL_FRect src = animations_get_frame(self->animations, self->ianim, self->iframe);
    SDL_Texture * texture = animations_get_texture(self->animations);
    SDL_RenderTexture(renderer, texture, &src, &self->wld);
}

void duck_init (struct duck * self, SDL_Renderer * renderer, const struct dims * dims) {

    float h = 32.0f;
    float w = 32.0f;
    int nanims_cap = 2;
    int nframes_cap = 6;
    const char * relpath = "../share/mbm/assets/images/duck.bmp";

    struct animations * animations = animations_new(nanims_cap, nframes_cap, relpath, renderer);
    animations_append_anim(animations);
    animations_append_frame(animations, (int64_t) 1e5, (SDL_FRect) { .h = h, .w = w, .x = 0 * w, .y = ANIMATION_STATE_IDLE * h });
    animations_append_anim(animations);
    animations_append_frame(animations, (int64_t) 1e5, (SDL_FRect) { .h = h, .w = w, .x = 0 * w, .y = ANIMATION_STATE_WALKING * h });
    animations_append_frame(animations, (int64_t) 1e5, (SDL_FRect) { .h = h, .w = w, .x = 1 * w, .y = ANIMATION_STATE_WALKING * h });
    animations_append_frame(animations, (int64_t) 1e5, (SDL_FRect) { .h = h, .w = w, .x = 2 * w, .y = ANIMATION_STATE_WALKING * h });
    animations_append_frame(animations, (int64_t) 1e5, (SDL_FRect) { .h = h, .w = w, .x = 3 * w, .y = ANIMATION_STATE_WALKING * h });
    animations_append_frame(animations, (int64_t) 1e5, (SDL_FRect) { .h = h, .w = w, .x = 4 * w, .y = ANIMATION_STATE_WALKING * h });
    animations_append_frame(animations, (int64_t) 1e5, (SDL_FRect) { .h = h, .w = w, .x = 5 * w, .y = ANIMATION_STATE_WALKING * h });

    *self = (struct duck) {
        .animations = animations,
        .ianim = ANIMATION_STATE_IDLE,
        .iframe = 0,
        .anim_phase_shift = (int64_t) 0,
        .t_frame_expires = INT64_MIN,
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

void duck_set_animation_state_idle (struct duck * self) {
    if (self->ianim == ANIMATION_STATE_IDLE) return;
    self->ianim = ANIMATION_STATE_IDLE;
    self->t_frame_expires = INT64_MIN;   // triggers animations_update() in duck_update()
}

void duck_set_animation_state_walking (struct duck * self) {
    if (self->ianim == ANIMATION_STATE_WALKING) return;
    self->ianim = ANIMATION_STATE_WALKING;
    self->t_frame_expires = INT64_MIN;   // triggers animations_update() in duck_update()
}

void duck_update (struct duck * self, const struct timings * timings) {
    int64_t tnow = timings_get_frame_timestamp(timings);
    if (self->t_frame_expires == INT64_MIN) {
        // determine the animation phase shift the first time after starting animation
        self->anim_phase_shift = tnow % animations_get_animation_duration(self->animations, ANIMATION_STATE_IDLE);
    }
    if (tnow > self->t_frame_expires) {
        animations_update(self->animations, self->ianim, self->anim_phase_shift, tnow, &self->t_frame_expires, &self->iframe);
    }
}
