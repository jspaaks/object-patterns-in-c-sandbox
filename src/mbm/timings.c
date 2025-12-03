#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_timer.h"       // SDL_GetTicksNS
#include <stdint.h>               // uint64_t
#include <stdio.h>                // fprintf
#include <stdlib.h>               // free

// declare properties of `struct timings`
struct timings {
    struct {
        uint64_t tprev;                        // microseconds
        uint64_t tthis;                        // microseconds
        float duration;                        // seconds
    } frame;
};

// define pointer to singleton instance of `struct timings`
static struct timings * singleton = nullptr;

void timings_delete (struct timings ** self) {
    free(*self);
    *self = nullptr;
}

float timings_get_frame_duration (const struct timings * self) {
    return self->frame.duration;
}

void timings_init (struct timings * self) {
    *self = (struct timings) {
        .frame = {
            .duration = 0,                     // seconds
            .tprev = SDL_GetTicksNS() / 1000,  // microseconds
            .tthis = SDL_GetTicksNS() / 1000,  // microseconds
        },
    };
}

struct timings * timings_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct timings *) calloc(1, sizeof(struct timings));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct timings, aborting.\n");
        exit(1);
    }
    return singleton;
}

void timings_update (struct timings * self) {
    self->frame.tprev = self->frame.tthis;
    self->frame.tthis = SDL_GetTicksNS() / 1000; // microseconds
    self->frame.duration = (float) (self->frame.tthis - self->frame.tprev) / 1e6;
}
