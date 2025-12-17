#include "mbm/fpscounter.h"
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_pixels.h"      // SDL_Color
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include "SDL3/SDL_render.h"      // SDL_Renderer
#include "SDL3/SDL_stdinc.h"      // SDL_snprintf, SDL_free, SDL_calloc
#include <SDL3_ttf/SDL_ttf.h>     // TTF_Font, TTF_OpenFont, TTF_CloseFont, TTF_RenderText_Shaded
#include <stdint.h>               // int64_t
#include <stdlib.h>               // exit

// declare properties of `struct fpscounter`
struct fpscounter {
    SDL_Color bgcolor;
    SDL_Color fgcolor;
    TTF_Font * font;
    bool is_on;
    float ptsize;
    int64_t interval;
    float scale;
    int64_t texpires;
    char text[24];
    SDL_FPoint wld;
};

// define pointer to singleton instance of `struct fpscounter`
static struct fpscounter * singleton = nullptr;

// forward function declarations
static TTF_Font * load_font (const char * relpath, float ptsize);

void fpscounter_delete (struct fpscounter ** self) {
    TTF_CloseFont((*self)->font);

    SDL_free(*self);
    *self = nullptr;
}

void fpscounter_draw (const struct fpscounter * self, SDL_Renderer * renderer) {
    if (self->is_on) {
        // create surface from string
        SDL_Surface * surface = TTF_RenderText_Shaded(self->font, self->text, 0, self->fgcolor, self->bgcolor);
        if (surface == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create surface for fpscounter text, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }

        // create texture from surface
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create texture for fpscounter text, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }

        // set texture scale mode
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

        {
            // get the width and height of the rendered text
            int w = -1;
            int h = -1;
            TTF_GetStringSize(self->font, self->text, 0, &w, &h);

            // render the texture on the screen
            SDL_FRect wld = (SDL_FRect) {
                .w = (float) w * self->scale,
                .h = (float) h * self->scale,
                .x = self->wld.x,
                .y = self->wld.y,
            };
            SDL_RenderTexture(renderer, texture, nullptr, &wld);
        }

        // free resources related to texture
        SDL_DestroyTexture(texture);
        texture = nullptr;

        // free resources related to surface
        SDL_DestroySurface(surface);
        surface = nullptr;
    }
}

void fpscounter_init (struct fpscounter * self) {

    float ptsize = 48.0f;

    *self = (struct fpscounter) {
        .bgcolor = (SDL_Color) {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = SDL_ALPHA_TRANSPARENT,
        },
        .fgcolor = (SDL_Color) {
            .r = 255,
            .g = 255,
            .b = 0,
            .a = SDL_ALPHA_OPAQUE,
        },
        .font = load_font("../share/mbm/assets/fonts/JetBrainsMono-SemiBold.ttf", ptsize),
        .interval = (int64_t) 5e5,
        .is_on = true,
        .ptsize = ptsize,
        .scale = 0.25,
        .texpires = (int64_t) 0,
        .text = "--- FPS",
        .wld = (SDL_FPoint) {
            .x = 0.0f,
            .y = 0.0f,
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
    self->is_on = !self->is_on;
}

void fpscounter_update (struct fpscounter * self, const struct timings * timings) {
    int64_t tnow = timings_get_frame_timestamp(timings);
    if (tnow > self->texpires) {
        int fps = (int) (1.0f / timings_get_frame_duration(timings));
        SDL_snprintf(&self->text[0], 24, "%d FPS", fps);
        self->texpires = tnow + self->interval;
    }
}

static TTF_Font * load_font (const char * relpath, float ptsize) {

    char * path = nullptr;
    SDL_asprintf(&path, "%s%s", SDL_GetBasePath(), relpath);

    TTF_Font * font = TTF_OpenFont(path, ptsize);
    if (font == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't load font file, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }

    // free resources
    SDL_free(path);
    path = nullptr;

    return font;
}
