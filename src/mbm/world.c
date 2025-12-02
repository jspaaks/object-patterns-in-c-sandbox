#include "mbm/world.h"
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_filesystem.h"  // SDL_GetBasePath
#include "SDL3/SDL_log.h"         // SDL_Log
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include <SDL3/SDL_render.h>      // SDL_Renderer, SDL_Texture, SDL_CreateTextureFromSurface, SDL_DestroyTexture, SDL_RenderTexture
#include "SDL3/SDL_stdinc.h"      // SDL_asprintf, SDL_free
#include "SDL3/SDL_surface.h"     // SDL_Surface, SDL_LoadBMP, SDL_DestroySurface
#include <stdlib.h>               // calloc, free, exit
#include <sys/param.h>            // MIN

typedef enum {
    TILE_TYPE_AIR = 0,
    TILE_TYPE_GROUND,
    TILE_TYPE_COUNT,
} TileType;

// declare properties of `struct world`
struct world {
    int ncols;
    int nrows;
    struct {
        int height;
        SDL_Texture * index;
        SDL_FRect srcs[TILE_TYPE_COUNT];
        TileType ** types;
        int width;
    } tile;
    SDL_FRect view;
};

// forward declaration of static functions
static TileType ** allocate_tiles (int nrows, int ncols);
static SDL_Texture * load_tile_index (const char * relpath, SDL_Renderer * renderer);

// define pointer to singleton instance of `struct world`
static struct world * singleton = nullptr;

static TileType ** allocate_tiles (const int nrows, const int ncols) {
    TileType * mem = (TileType *) calloc(nrows * ncols, sizeof(TileType));
    if (mem == nullptr) {
        SDL_Log("Error allocating dynamic memory for contiguous memory for tile types, aborting.\n");
        exit(1);
    }
    TileType ** tile_types = (TileType **) calloc(nrows, sizeof(TileType *));
    if (tile_types == nullptr) {
        SDL_Log("Error allocating dynamic memory for memory for tile types, aborting.\n");
        exit(1);
    }
    for (int irow = 0; irow < nrows; ++irow) {
        tile_types[irow] = &mem[irow * ncols];
    }
    return tile_types;
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

void world_delete (struct world ** self) {
    // free dynamically allocated memory used by .index
    SDL_DestroyTexture((*self)->tile.index);
    (*self)->tile.index = nullptr;

    // free memory holding the contiguous tile type data
    free((*self)->tile.types[0]);
    (*self)->tile.types[0] = nullptr;

    // free memory holding the row/column tile type data
    free((*self)->tile.types);
    (*self)->tile.types = nullptr;

    // free own resources
    free(*self);
    *self = nullptr;
}

void world_draw (struct world * self, SDL_Renderer * renderer) {
    int icol_s = (int) (self->view.x) / self->tile.width;
    int icol_e = MIN((int) (self->view.x + self->view.w) / self->tile.width + 1, self->ncols);
    for (int irow = 0; irow < self->nrows; irow++) {
        for (int icol = icol_s; icol < icol_e; icol++) {
            TileType t = self->tile.types[irow][icol];
            SDL_FRect src = self->tile.srcs[t];
            SDL_FRect wld = {
                .h = (float) (self->tile.height),
                .w = (float) (self->tile.width),
                .x = (float) (icol * self->tile.width) - self->view.x,
                .y = (float) (irow * self->tile.height),
            };
            SDL_RenderTexture(renderer, self->tile.index, &src, &wld);
        }
    }
}

void world_init (struct world * self, SDL_Renderer * renderer, int view_width, int view_height) {
    int tile_width = 32;
    int tile_height = tile_width;
    int nrows = view_height / tile_height;
    int ncols = 25;

    // load the tile index into a texture 
    SDL_Texture * index = load_tile_index("../share/mbm/assets/images/tiles.bmp", renderer);

    // allocate memory for accessing the tiles consecutively and by row/col; 
    TileType ** tile_types = allocate_tiles(nrows, ncols);

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

    *self = (struct world) {
        .ncols = ncols,
        .nrows = nrows,
        .tile = {
            .height = tile_height,
            .index = index,
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
            .types = tile_types,
            .width = tile_width,
        },
        .view = (SDL_FRect) {
            .h = view_height,
            .w = view_width,
            .x = 0,
            .y = 0,
        },
    };
}

struct world * world_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct world *) calloc(1, sizeof(struct world));
    if (singleton == nullptr) {
        SDL_Log("ERROR allocating dynamic memory for struct world, aborting.\n");
        exit(1);
    }
    return singleton;
}

void world_update (struct world * self) {
    float a = self->view.x;
    float b = self->view.x + 0.1;
    float c = self->ncols * self->tile.width - self->view.w;
    self->view.x = MIN(MAX(a, b),c);
}
