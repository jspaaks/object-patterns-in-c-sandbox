#include "mbm/duck.h"
#include "animations.h"           // struct animations and associated functions
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture, SDL_RenderTextureRotated
#include "SDL3/SDL_stdinc.h"      // SDL_free, SDL_calloc
#include "SDL3/SDL_surface.h"     // SDL_FlipMode
#include <stdlib.h>               // exit

// define enum for animation states
enum animation_state: uint8_t {
    ANIMATION_STATE_IDLE = 0,
    ANIMATION_STATE_WALKING,
    ANIMATION_STATE_COUNT,
};

// declare properties of `struct duck`
struct duck {
    int64_t anim_phase_shift;
    struct animations * animations;
    enum animation_state ianim;
    int iframe;
    bool is_facing_right;
    struct {
        struct {
            float current;
            float walking;
        } x;
        struct {
            float current;
        } y;
    } v;
    struct {
         float y;
    } vmax;
    int64_t t_frame_expires;
    SDL_FRect pos;
};

static float clamp (float v, float vmin, float vmax);

// define pointer to singleton instance of `struct duck`
static struct duck * singleton = nullptr;

static float clamp (float v, float vmin, float vmax) {
    if (v < vmin) return vmin;
    if (v > vmax) return vmax;
    return v;
}

void duck_delete (struct duck ** self) {
    animations_delete(&(*self)->animations);
    (*self)->animations = nullptr;
    SDL_free(*self);
    *self = nullptr;
}

void duck_draw (const struct duck * self, SDL_Renderer * renderer) {
    SDL_FRect src = animations_get_frame(self->animations, self->ianim, self->iframe);
    SDL_Texture * texture = animations_get_texture(self->animations);
    SDL_FlipMode flipmode = self->is_facing_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderTextureRotated(renderer, texture, &src, &self->pos, 0, nullptr, flipmode);
}

void duck_halt (struct duck * self) {
    self->v.x.current = 0.0f;
    if (self->ianim != ANIMATION_STATE_IDLE) {
        self->ianim = ANIMATION_STATE_IDLE;
        // trigger animations_update() in duck_update()
        self->t_frame_expires = INT64_MIN;
    }

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
        .anim_phase_shift = (int64_t) 0,
        .animations = animations,
        .ianim = ANIMATION_STATE_IDLE,
        .iframe = 0,
        .is_facing_right = true,
        .t_frame_expires = INT64_MIN,
        .v = {
            .x = {
                .current = 0.0f,
                .walking = 20.0f,
            },
            .y = {
                .current = 0.0f,
            }
        },
        .vmax = {
            .y = 100.0f,
        },
        .pos = (SDL_FRect) {
            .h = 32.0f,
            .w = 32.0f,
            .x = 100.0f,
            .y = dims->world.h - 32.0f - 32.0f - 100.0f,
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

void duck_update (struct duck * self, const struct world * world, const struct timings * timings) {
    int64_t tnow = timings_get_frame_timestamp(timings);
    if (self->t_frame_expires == INT64_MIN) {
        // determine the animation phase shift the first time after starting animation
        self->anim_phase_shift = tnow % animations_get_animation_duration(self->animations, ANIMATION_STATE_IDLE);
    }
    if (tnow > self->t_frame_expires) {
        animations_update(self->animations, self->ianim, self->anim_phase_shift, tnow, &self->t_frame_expires, &self->iframe);
    }
    float dt = timings_get_frame_duration(timings);
    float g = world_get_gravity(world);
    self->v.y.current = clamp(self->v.y.current + 0.5 * g * dt, -1 * self->vmax.y, self->vmax.y);
    self->pos.x += self->v.x.current * dt;
    self->pos.y += self->v.y.current * dt;
}

void duck_walk_left (struct duck * self) {
    self->is_facing_right = false;
    self->v.x.current = -1.0f * self->v.x.walking;
    if (self->ianim != ANIMATION_STATE_WALKING) {
        self->ianim = ANIMATION_STATE_WALKING;
        // trigger animations_update() in duck_update()
        self->t_frame_expires = INT64_MIN;
    }
}

void duck_walk_right (struct duck * self) {
    self->is_facing_right = true;
    self->v.x.current = self->v.x.walking;
    if (self->ianim != ANIMATION_STATE_WALKING) {
        self->ianim = ANIMATION_STATE_WALKING;
        // trigger animations_update() in duck_update()
        self->t_frame_expires = INT64_MIN;
    }
}
