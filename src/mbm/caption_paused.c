#include "mbm/caption_paused.h"
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture, SDL_RenderTexture
#include "SDL3/SDL_stdinc.h"      // SDL_snprintf, SDL_free, SDL_calloc
#include "SDL3/SDL_surface.h"     // SDL_surface
#include <SDL3_ttf/SDL_ttf.h>     // TTF_Font, TTF_OpenFont, TTF_CloseFont, TTF_RenderText_Solid
#include <stdlib.h>               // exit

// declare properties of `struct caption_paused`
struct caption_paused {
    SDL_Texture * texture;
    SDL_FRect wld;
};

// define pointer to singleton instance of `struct caption_paused`
static struct caption_paused * singleton = nullptr;

// forward function declarations
static TTF_Font * load_font (const char * relpath, float ptsize);

void caption_paused_delete (struct caption_paused ** self) {
    // free resources related to texture
    SDL_DestroyTexture((*self)->texture);
    (*self)->texture = nullptr;

    // free resources related to self
    SDL_free(*self);
    *self = nullptr;
}

void caption_paused_draw (const struct caption_paused * self, SDL_Renderer * renderer) {
    SDL_RenderTexture(renderer, self->texture, nullptr, &self->wld);
}

void caption_paused_init (struct caption_paused * self, SDL_Renderer * renderer, const struct dims * dims) {
    // create surface from string
    SDL_Surface * surface = nullptr;
    TTF_Font * font = nullptr;
    const char text[7] = "PAUSED";
    {
        const float ptsize = 28.0f;
        font = load_font("../share/mbm/assets/fonts/JetBrainsMono-SemiBold.ttf", ptsize);
        const SDL_Color fgcolor = (SDL_Color) {
            .r = 255,
            .g = 255,
            .b = 255,
            .a = SDL_ALPHA_OPAQUE,
        };
        surface = TTF_RenderText_Solid(font, text, 0, fgcolor);
        if (surface == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create surface for fpscounter text, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }
    }

    // create texture from surface
    SDL_Texture * texture = nullptr;
    int w = -1;
    int h = -1;
    {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                            "Couldn't create texture for fpscounter text, aborting; %s\n",
                            SDL_GetError());
            exit(1);
        }

        // set texture scale mode
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

        // retrieve the width and height of the rendered text
        TTF_GetStringSize(font, text, 0, &w, &h);
    }

    *self = (struct caption_paused) {
        .texture = texture,
        .wld = (SDL_FRect) {
            .h = (float) h,
            .w = (float) w,
            .x = dims->view.w / 2.0f - (float) w / 2.0f,
            .y = dims->view.h / 2.0f - (float) h / 2.0f,
        },
    };

    // free resources related to surface
    SDL_DestroySurface(surface);
    surface = nullptr;

    // free resources related to font
    TTF_CloseFont(font);
    font = nullptr;
}

struct caption_paused * caption_paused_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct caption_paused *) SDL_calloc(1, sizeof(struct caption_paused));
    if (singleton == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "ERROR allocating dynamic memory for struct caption_paused, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    return singleton;
}

void caption_paused_update (struct caption_paused * self) {
    (void) self;
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
