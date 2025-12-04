#include "mbm/world.h"
#include "mbm/dims.h"             // struct dims
#include "mbm/timings.h"          // struct timings and associated functions
#include "idx/idx.h"              // functionality related to reading binary data from file
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_filesystem.h"  // SDL_GetBasePath
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_rect.h"        // SDL_FRect
#include "SDL3/SDL_render.h"      // SDL_Renderer, SDL_Texture, SDL_CreateTextureFromSurface, SDL_DestroyTexture, SDL_RenderTexture
#include "SDL3/SDL_stdinc.h"      // SDL_asprintf, SDL_calloc, SDL_free
#include "SDL3/SDL_surface.h"     // SDL_Surface, SDL_LoadBMP, SDL_DestroySurface
#include <assert.h>               // assert
#include <stdint.h>               // uint64_t, uint8_t
#include <stdlib.h>               // exit
#include <sys/param.h>            // MIN

typedef enum : uint8_t {
    TILE_TYPE_AIR = 0,
    TILE_TYPE_GROUND,
    TILE_TYPE_COUNT,
} TileType;

// declare properties of `struct world`
struct world {
    int h;
    int ncols;
    int nrows;
    struct {
        int h;
        SDL_Texture * index;
        SDL_FRect srcs[TILE_TYPE_COUNT];
        TileType ** types;
        int w;
    } tile;
    struct {
        int dx;       // pixels per second
        int h;
        int w;
        float x;
        float y;
    } view;
    int w;
};

// forward declaration of static functions
static TileType ** allocate_tiles (int nrows, int ncols);
static SDL_Texture * load_tile_index (const char * relpath, SDL_Renderer * renderer);
static void load_tile_map (const char * relpath, uint32_t nrows, uint32_t ncols, uint8_t * bufffer);

// define pointer to singleton instance of `struct world`
static struct world * singleton = nullptr;

static TileType ** allocate_tiles (const int nrows, const int ncols) {
    TileType * mem = (TileType *) SDL_calloc(nrows * ncols, sizeof(TileType));
    if (mem == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Error allocating dynamic memory for contiguous memory for tile types, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    TileType ** tile_types = (TileType **) SDL_calloc(nrows, sizeof(TileType *));
    if (tile_types == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Error allocating dynamic memory for memory for tile types, aborting; %s\n",
                        SDL_GetError());
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
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't load tiles bitmap, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    SDL_Texture * index = SDL_CreateTextureFromSurface(renderer, surface);
    if (index == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Couldn't create texture for tiles, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    // free dynamically allocated memory used by surface and by path
    SDL_DestroySurface(surface);
    surface = nullptr;
    SDL_free(path);
    path = nullptr;
    return index;
}

static void load_tile_map (const char * relpath, uint32_t nrows, uint32_t ncols, uint8_t * buffer) {
    char * path = nullptr;
    SDL_asprintf(&path, "%s%s", SDL_GetBasePath(), relpath);
    const IdxHeader header = idx_read_header(path);
    if (header.lengths[0] != nrows) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "number of rows in tile map should be equal to number of rows in tilemap file, aborting.\n");
        exit(1);
    }
    if (header.lengths[1] != ncols) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "number of columns in tile map should be equal to number of columns in tilemap file, aborting.\n");
        exit(1);
    }
    idx_read_body_as_uint8(path, &header, buffer);
    SDL_free(path);
    path = nullptr;
}

void world_delete (struct world ** self) {
    // free dynamically allocated memory used by .index
    SDL_DestroyTexture((*self)->tile.index);
    (*self)->tile.index = nullptr;

    // free memory holding the contiguous tile type data
    SDL_free((*self)->tile.types[0]);
    (*self)->tile.types[0] = nullptr;

    // free memory holding the row/column tile type data
    SDL_free((*self)->tile.types);
    (*self)->tile.types = nullptr;

    // free own resources
    SDL_free(*self);
    *self = nullptr;
}

void world_draw (const struct world * self, SDL_Renderer * renderer) {
    int icol_s = self->view.x / self->tile.w;
    int icol_e = MIN(self->view.x + self->view.w / self->tile.w + 1, self->ncols);
    for (int irow = 0; irow < self->nrows; irow++) {
        for (int icol = icol_s; icol < icol_e; icol++) {
            TileType t = self->tile.types[irow][icol];
            SDL_FRect src = self->tile.srcs[t];
            SDL_FRect wld = {
                .h = (float) (self->tile.h),
                .w = (float) (self->tile.w),
                .x = (float) (icol * self->tile.w) - self->view.x,
                .y = (float) (irow * self->tile.h),
            };
            SDL_RenderTexture(renderer, self->tile.index, &src, &wld);
        }
    }
}

void world_init (struct world * self, SDL_Renderer * renderer, const struct dims * dims) {
    int nrows = dims->view.h / dims->tile.h;
    int ncols = dims->world.w / dims->tile.w;

    // load the tile index into a texture 
    SDL_Texture * index = load_tile_index("../share/mbm/assets/images/tiles.bmp", renderer);

    // allocate memory for accessing the tiles consecutively and by row/col; 
    TileType ** tile_types = allocate_tiles(nrows, ncols);

    // initialize a tile pattern by loading from file
    load_tile_map("../share/mbm/assets/tilemaps/level1.idx", nrows, ncols, tile_types[0]);

    *self = (struct world) {
        .h = dims->world.h,
        .ncols = ncols,
        .nrows = nrows,
        .tile = {
            .h = dims->tile.h,
            .index = index,
            .srcs = {
                [TILE_TYPE_AIR] = (SDL_FRect) {
                    .h = (float) (dims->tile.h - 2),
                    .w = (float) (dims->tile.w - 2),
                    .x = 1.0f,
                    .y = 1.0f,
                },
                [TILE_TYPE_GROUND] = (SDL_FRect) {
                    .h = (float) (dims->tile.h - 2),
                    .w = (float) (dims->tile.w - 2),
                    .x = 32.0f + 10.0f + 1.0f,
                    .y = 1.0f,
                },
            },
            .types = tile_types,
            .w = dims->tile.w,
        },
        .view = {
            .dx = 50.3,
            .h = dims->view.h,
            .w = dims->view.w,
            .x = 0.0f,
            .y = 0.0f,
        },
        .w = dims->world.w,
    };
}

void world_move_view_left (struct world * self, const struct timings * timings) {
    float dt = timings_get_frame_duration(timings);
    float a = 0;
    float b = self->view.x - (self->view.dx * dt);
    self->view.x = MAX(a, b);
}

void world_move_view_right (struct world * self, const struct timings * timings) {
    float dt = timings_get_frame_duration(timings);
    float a = self->view.x + (self->view.dx * dt);
    float b = self->w - self->view.w;
    self->view.x = MIN(a, b);

}

struct world * world_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct world *) SDL_calloc(1, sizeof(struct world));
    if (singleton == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "ERROR allocating dynamic memory for struct world, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    return singleton;
}

void world_update (struct world *) {}
