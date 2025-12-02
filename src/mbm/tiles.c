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
#include <sys/param.h>

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
    int tile_width;
    SDL_FRect view;
};

// forward declaration of static functions
static SDL_Texture * load_tile_index (const char * relpath, SDL_Renderer * renderer);
static void allocate_tiles (int nrows, int ncols, TileType *** tile_types);

// define pointer to singleton instance of `struct tiles`
static struct tiles * singleton = nullptr;

static void allocate_tiles (const int nrows, const int ncols, TileType *** tile_types) {
    TileType * mem = (TileType *) calloc(nrows * ncols, sizeof(TileType));
    if (mem == nullptr) {
        SDL_Log("Error allocating dynamic memory for contiguous memory for tile types, aborting.\n");
        exit(1);
    }
    *tile_types = (TileType **) calloc(nrows, sizeof(TileType *));
    if (*tile_types == nullptr) {
        SDL_Log("Error allocating dynamic memory for memory for tile types, aborting.\n");
        exit(1);
    }
    for (int irow = 0; irow < nrows; ++irow) {
        (*tile_types)[irow] = &mem[irow * ncols];
    }
}

static SDL_Texture * load_tile_index (const char * relpath, SDL_Renderer * renderer) {
    char * path = nullptr;
    SDL_asprintf(&path, "%s%s", SDL_GetBasePath(), relpath);
    SDL_Surface * surface = SDL_LoadBMP(path);
    if (surface == nullptr) {
        SDL_Log("Couldn't load tiles bitmap: %s", SDL_GetError());
        exit(1);
    }
    SDL_Texture * index = SDL_CreateTextureFromSurface(renderer, surface);
    if (index == nullptr) {
        SDL_Log("Couldn't create index: %s", SDL_GetError());
        exit(1);
    }
    // free dynamically allocated memory used by surface and by path
    SDL_DestroySurface(surface);
    surface = nullptr;
    SDL_free(path);
    path = nullptr;
    return index;
}

void tiles_delete (struct tiles ** self) {
    // free dynamically allocated memory used by .index
    SDL_DestroyTexture((*self)->index);
    (*self)->index = nullptr;

    // free memory holding the contiguous tile type data
    free((*self)->tile_types[0]);
    (*self)->tile_types[0] = nullptr;

    // free memory holding the row/column tile type data
    free((*self)->tile_types);
    (*self)->tile_types = nullptr;

    // free own resources
    free(*self);
    *self = nullptr;
}

void tiles_draw (struct tiles * self, SDL_Renderer * renderer) {
    int icol_s = (int) (self->view.x) / self->tile_width;
    int icol_e = MIN((int) (self->view.x + self->view.w) / self->tile_width + 1, self->ncols);
    for (int irow = 0; irow < self->nrows; irow++) {
        for (int icol = icol_s; icol < icol_e; icol++) {
            TileType t = self->tile_types[irow][icol];
            SDL_FRect src = self->srcs[t];
            SDL_FRect wld = {
                .h = (float) (self->tile_height),
                .w = (float) (self->tile_width),
                .x = (float) (icol * self->tile_width) - self->view.x,
                .y = (float) (irow * self->tile_height),
            };
            SDL_RenderTexture(renderer, self->index, &src, &wld);
        }
    }
}

void tiles_init (struct tiles * self, SDL_Renderer * renderer) {
    SDL_Texture * index = nullptr;
    int ncols = 100;
    int nrows = 12;
    int tile_height = 32;
    TileType ** tile_types = nullptr;
    int tile_width = 32;

    // load the tile index into a texture 
    index = load_tile_index("../share/mbm/assets/images/tiles.bmp", renderer);

    // allocate memory for accessing the tiles consecutively and by row/col; 
    allocate_tiles(nrows, ncols, &tile_types);

    // initialize a tile pattern
    {
        for (int irow = 0; irow < nrows; irow++) {
            for (int icol = 0; icol < ncols; icol++) {
                if (irow == icol % nrows) {
                    tile_types[irow][icol] = TILE_TYPE_GROUND;
                } else {
                    tile_types[irow][icol] = TILE_TYPE_AIR;
                }
            }
        }
    }

    *self = (struct tiles) {
        .index = index,
        .ncols = ncols,
        .nrows = nrows,
        .srcs = {
            [TILE_TYPE_AIR] = (SDL_FRect) {
                .h = (float) (tile_height - 2),
                .w = (float) (tile_width - 2),
                .x = 1.0f,
                .y = 1.0f,
            },
            [TILE_TYPE_GROUND] = (SDL_FRect) {
                .h = (float) (tile_height - 2),
                .w = (float) (tile_width - 2),
                .x = 32.0f + 10.0f + 1.0f,
                .y = 1.0f,
            },
        },
        .tile_height = tile_height,
        .tile_types = tile_types,
        .tile_width = tile_width,
        .view = (SDL_FRect) {
            .h = 12 * tile_height,
            .w = 20 * tile_height,
            .x = 0,
            .y = 0,
        },
    };
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

void tiles_update (struct tiles * self) {
    float a = self->view.x + 0.1;
    float b = self->ncols * self->tile_width - self->view.w;
    self->view.x = MIN(a, b);
}
