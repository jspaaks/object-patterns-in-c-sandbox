#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_stdinc.h"      // SDL_free, SDL_calloc
#include "SDL3/SDL_timer.h"       // SDL_GetTicksNS
#include <stdint.h>               // int64_t
#include <stdlib.h>               // exit

// declare properties of `struct timings`
struct timings {
    struct {
        int64_t tprev;                        // microseconds
        int64_t tthis;                        // microseconds
        float duration;                       // seconds
    } frame;
};

// define pointer to singleton instance of `struct timings`
static struct timings * singleton = nullptr;

void timings_delete (struct timings ** self) {
    SDL_free(*self);
    *self = nullptr;
}

float timings_get_frame_duration (const struct timings * self) {
    return self->frame.duration;
}

int64_t timings_get_frame_timestamp (const struct timings * self) {
    return self->frame.tthis;
}

void timings_init (struct timings * self) {
    *self = (struct timings) {
        .frame = {
            .duration = 0.0f,                              // seconds
            .tprev = (int64_t) (SDL_GetTicksNS() / 1000),  // microseconds
            .tthis = (int64_t) (SDL_GetTicksNS() / 1000),  // microseconds
        },
    };
}

struct timings * timings_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct timings *) SDL_calloc(1, sizeof(struct timings));
    if (singleton == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Could not allocate dynamic memory for struct timings, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    return singleton;
}

void timings_update (struct timings * self) {
    self->frame.tprev = self->frame.tthis;
    self->frame.tthis = (int64_t) (SDL_GetTicksNS() / 1000); // microseconds
    self->frame.duration = (float) (self->frame.tthis - self->frame.tprev) / 1e6;
}
