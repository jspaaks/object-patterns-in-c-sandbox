#include "mbm/tiles.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_surface.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TILE_TYPE_AIR = 0,
    TILE_TYPE_GROUND,
    TILE_TYPE_COUNT,
} TileType;

// declare properties of `struct tiles`
struct tiles {
    SDL_Texture * index;
    int ncols;
    int nrows;
    SDL_FRect srcs[TILE_TYPE_COUNT];
    int tile_height;
    TileType ** tile_types;
    TileType * tile_types_mem;
    int tile_width;
};

// define pointer to singleton instance of `struct tiles`
static struct tiles * singleton = nullptr;

void tiles_delete (struct tiles ** self) {
    // free dynamically allocated memory used by .index
    SDL_DestroyTexture((*self)->index);
    (*self)->index = nullptr;

    // free memory holding the row/column tile type data
    free((*self)->tile_types);
    (*self)->tile_types = nullptr;

    // free contiguous memory holding the tile type data
    free((*self)->tile_types_mem);
    (*self)->tile_types_mem = nullptr;

    // free own resources
    free(*self);
    *self = nullptr;
}

void tiles_draw (struct tiles * self, SDL_Renderer * renderer) {
    for (int irow = 0; irow < self->nrows; irow++) {
        for (int icol = 0; icol < self->ncols; icol++) {
            TileType t = self->tile_types[irow][icol];
            SDL_FRect src = self->srcs[t];
            SDL_FRect wld = {
                .h = (float) (self->tile_height),
                .w = (float) (self->tile_width),
                .x = (float) (icol * self->tile_width) - 16,
                .y = (float) (irow * self->tile_height),
            };
            SDL_RenderTexture(renderer, self->index, &src, &wld);
        }
    }
}

void tiles_init (struct tiles * self, SDL_Renderer * renderer) {
    {
        self->nrows = 12;
        self->ncols = 30;
        self->tile_height = 32;
        self->tile_width = 32;
    }
    {
        const char * relpath = "../share/mbm/assets/images/tiles.bmp";
        char * path = nullptr;
        SDL_asprintf(&path, "%s%s", SDL_GetBasePath(), relpath);
        SDL_Surface * surface = SDL_LoadBMP(path);
        if (surface == nullptr) {
            SDL_Log("Couldn't load tiles bitmap: %s", SDL_GetError());
            exit(1);
        }
        self->index = SDL_CreateTextureFromSurface(renderer, surface);
        if (self->index == nullptr) {
            SDL_Log("Couldn't create index: %s", SDL_GetError());
            exit(1);
        }

        // free dynamically allocated memory used by surface and by path
        SDL_DestroySurface(surface);
        surface = nullptr;
        SDL_free(path);
        path = nullptr;
    }
    {
        self->srcs[TILE_TYPE_AIR] = (SDL_FRect) {
            .h = (float) (self->tile_height - 2),
            .w = (float) (self->tile_width - 2),
            .x = 1.0f,
            .y = 1.0f,
        };
        self->srcs[TILE_TYPE_GROUND] = (SDL_FRect) {
            .h = (float) (self->tile_height - 2),
            .w = (float) (self->tile_width - 2),
            .x = 32.0f + 10.0f + 1.0f,
            .y = 1.0f,
        };
    }
    {
        self->tile_types_mem = (TileType *) calloc(self->nrows * self->ncols, sizeof(TileType));
        self->tile_types = (TileType **) calloc(self->nrows, sizeof(TileType *));
        for (int irow = 0; irow < self->nrows; irow++) {
            self->tile_types[irow] = &self->tile_types_mem[irow * self->ncols];
        }
        for (int irow = 0; irow < self->nrows; irow++) {
            for (int icol = 0; icol < self->ncols; icol++) {
                if (irow == icol % self->nrows) {
                    self->tile_types[irow][icol] = TILE_TYPE_GROUND;
                } else {
                    self->tile_types[irow][icol] = TILE_TYPE_AIR;
                }
            }
        }
    }
}

struct tiles * tiles_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct tiles *) calloc(1, sizeof(struct tiles));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct tiles, aborting.\n");
        exit(1);
    }
    return singleton;
}

void tiles_update (struct tiles *) {}
