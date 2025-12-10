#include "animations.h"
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_filesystem.h"  // SDL_GetBasePath
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture, SDL_CreateTextureFromSurface, SDL_DestroyTexture
#include "SDL3/SDL_stdinc.h"      // SDL_asprintf, SDL_free, SDL_calloc
#include "SDL3/SDL_surface.h"     // SDL_Surface, SDL_LoadBMP, SDL_DestroySurface
#include <stdint.h>               // uint8_t, uint64_t
#include <stdlib.h>               // exit

// declare properties of `struct animations`
struct animations {
    uint64_t * durations;
    uint64_t ** frame_durations_acc;
    SDL_FRect ** frame_srcs;
    int nframes_cap;
    int nanims;
    int nanims_cap;
    int * nframes;
    SDL_Texture * texture;
};

// forward declarations of functions defined below
static SDL_Texture * load_texture (const char * relpath, SDL_Renderer * renderer);

void animations_delete (struct animations ** self) {

    SDL_DestroyTexture((*self)->texture);
    (*self)->texture = nullptr;

    SDL_free((*self)->nframes);
    (*self)->nframes = nullptr;

    SDL_free((*self)->frame_srcs[0]);
    (*self)->frame_srcs[0] = nullptr;

    SDL_free((*self)->frame_srcs);
    (*self)->frame_srcs = nullptr;

    SDL_free((*self)->frame_durations_acc[0]);
    (*self)->frame_durations_acc[0] = nullptr;

    SDL_free((*self)->frame_durations_acc);
    (*self)->frame_durations_acc = nullptr;

    SDL_free((*self)->durations);
    (*self)->durations = nullptr;

    SDL_free(*self);
    *self = nullptr;
}

struct animations * animations_new (int nanims_cap, int nframes_cap, const char * relpath, SDL_Renderer * renderer) {

    struct animations * animations = nullptr;
    uint64_t * durations = nullptr;
    SDL_FRect ** frame_srcs = nullptr;
    uint64_t ** frame_durations_acc = nullptr;
    int * nframes = nullptr;

    // allocate dynamic memory for holding the struct animations (`self`)
    {
        animations = SDL_calloc(1, sizeof(struct animations));
        if (animations == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create dynamic memory for storing struct animations, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }
    }

    // allocate dynamic memory for holding the 1d array with the animation durations (`.durations`)
    {
        durations = SDL_calloc(nanims_cap, sizeof(uint64_t));
        if (durations == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create dynamic memory for storing animation durations, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }
    }

    // allocate dynamic memory for holding the 2d array with the frame rectangles (`.frame_srcs`)
    {
        SDL_FRect * frame_srcs_contig = SDL_calloc(nanims_cap * nframes_cap, sizeof(SDL_FRect));
        if (frame_srcs_contig == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create contiguous dynamic memory for storing frame rects, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }
        frame_srcs = SDL_calloc(nanims_cap, sizeof(SDL_FRect *));
        if (frame_srcs == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create dynamic memory for storing pointers to frame rects, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }
        for (int i = 0; i < nanims_cap; ++i) {
            frame_srcs[i] = &frame_srcs_contig[i * nframes_cap];
        }
        // don't free `frame_srcs_contig` yet, since it's used via `.frame_srcs[0]`
    }

    // allocate dynamic memory for holding the 2d array with the accumulated frame durations (`.frame_durations_acc`)
    {
        uint64_t * frame_durations_acc_contig = SDL_calloc(nanims_cap * nframes_cap, sizeof(uint64_t));
        if (frame_durations_acc_contig == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create contiguous dynamic memory for storing accumated frame durations, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }
        frame_durations_acc = SDL_calloc(nanims_cap, sizeof(uint64_t *));
        if (frame_durations_acc == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create dynamic memory for storing pointers to accumulated frame durations, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }
        for (int i = 0; i < nanims_cap; ++i) {
            frame_durations_acc[i] = &frame_durations_acc_contig[i * nframes_cap];
        }
        // don't free `frame_durations_acc_contig` yet, since it's used via `.frame_durations_acc[0]`
    }

    // allocate dynamic memory for holding the 1d array with the number of frames (`.nframes`)
    nframes = SDL_calloc(nanims_cap, sizeof(int));
    if (nframes == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't create dynamic memory for storing nframes, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }

    // assemble the struct animations / self
    *animations = (struct animations) {
        .durations = durations,
        .frame_durations_acc = frame_durations_acc,
        .frame_srcs = frame_srcs,
        .nanims = 0,
        .nanims_cap = nanims_cap,
        .nframes = nframes,
        .nframes_cap = nframes_cap,
        .texture = load_texture(relpath, renderer),
    };

    return animations;
}

void animations_append_anim (struct animations * self) {

    // find the index of the next animation
    int ianim = self->nanims;
    if (ianim >= self->nanims_cap) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Can't append animation past the allocated space, aborting\n");
        exit(1);
    }

    // increment the number of animations
    self->nanims++;
}

void animations_append_frame (struct animations * self, uint64_t duration, SDL_FRect src) {

    // find the index of the next frame in the last animation
    const int ianim = self->nanims - 1;
    int iframe = self->nframes[ianim];
    if (iframe >= self->nframes_cap) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Can't append frame past the allocated space, aborting\n");
        exit(1);
    }

    // add the frame, update durations
    self->frame_srcs[ianim][iframe] = src;
    if (iframe == 0) {
        self->frame_durations_acc[ianim][iframe] = duration;
    } else {
        self->frame_durations_acc[ianim][iframe] = self->frame_durations_acc[ianim][iframe-1] + duration;
    }
    self->durations[ianim] += duration;
    self->nframes[ianim]++;
}

uint64_t animations_get_animation_duration (struct animations * self, int ianim) {
    return self->durations[ianim];
}

SDL_FRect animations_get_frame (const struct animations * self, int ianim, int iframe) {
    return self->frame_srcs[ianim][iframe];
}

SDL_Texture * animations_get_texture (const struct animations * self) {
    return self->texture;
}

void animations_update (const struct animations * self, int ianim, uint64_t anim_phase_shift, uint64_t tnow, uint64_t * t_frame_expires, int * iframe) {
    // TODO review this part
    *iframe = 0;
    const uint64_t tref = tnow - anim_phase_shift;
    const uint64_t tmod = tref % self->durations[ianim];
    while (self->frame_durations_acc[ianim][*iframe] < tmod && *iframe < self->nframes[ianim] - 1) {
        (*iframe)++;
    }
    *t_frame_expires = tmod + self->frame_durations_acc[ianim][*iframe];
}

static SDL_Texture * load_texture (const char * relpath, SDL_Renderer * renderer) {

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
