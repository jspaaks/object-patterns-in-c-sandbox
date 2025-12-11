#include "mbm/fpscounter.h"
#include "mbm/timings.h"            // struct timings and associated functions
#include "SDL3/SDL_error.h"         // SDL_GetError
#include "SDL3/SDL_log.h"           // SDL_LogCritical
#include "SDL3/SDL_pixels.h"        // SDL_Color
#include "SDL3/SDL_rect.h"          // SDL_FRect
#include "SDL3/SDL_render.h"        // SDL_Renderer
#include "SDL3/SDL_stdinc.h"        // SDL_free, SDL_calloc
#include <stdint.h>                 // int64_t
#include <stdlib.h>                 // exit

// declare properties of `struct fpscounter`
struct fpscounter {
    SDL_Color bgcolor;
    SDL_Color fgcolor;
    float fps;
    bool is_on;
    int64_t interval;
    int64_t texpires;
    SDL_FRect wld;
};

// define pointer to singleton instance of `struct fpscounter`
static struct fpscounter * singleton = nullptr;

void fpscounter_delete (struct fpscounter ** self) {
    SDL_free(*self);
    *self = nullptr;
}

void fpscounter_draw (const struct fpscounter * self, SDL_Renderer * renderer) {
    (void) renderer;
    if (self->is_on) {
        // draw the fps text
        SDL_Log("FPS: %.0f\n", self->fps);
    }
}

void fpscounter_init (struct fpscounter * self) {
    *self = (struct fpscounter) {
        .bgcolor = (SDL_Color) {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = SDL_ALPHA_TRANSPARENT,
        },
        .fgcolor = (SDL_Color) {
            .r = 0,
            .g = 255,
            .b = 255,
            .a = SDL_ALPHA_OPAQUE,
        },
        .fps = 0.0f,
        .interval = (int64_t) 1e6,
        .is_on = true,
        .texpires = (int64_t) 0,
        .wld = (SDL_FRect) {
            .h = 32.0f,
            .w = 100.0f,
            .x = 100.0f,
            .y = 100.0f,
        },
    };
}

struct fpscounter * fpscounter_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct fpscounter *) SDL_calloc(1, sizeof(struct fpscounter));
    if (singleton == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "ERROR allocating dynamic memory for struct fpscounter, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    return singleton;
}

void fpscounter_toggle (struct fpscounter * self) {
    self->is_on = false;
}

void fpscounter_update (struct fpscounter * self, const struct timings * timings) {
    int64_t tnow = timings_get_frame_timestamp(timings);
    if (tnow > self->texpires) {
        self->fps = 1.0f / timings_get_frame_duration(timings);
        self->texpires = tnow + self->interval;
    }
}
