#ifndef MBM_GAME_H_INCLUDED
#define MBM_GAME_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/dims.h"             // struct dims
#include "mbm/timings.h"          // struct timings and associated functions
#include "mbm/world.h"            // struct world and associated functions
#include "SDL3/SDL_events.h"      // SDL_Event
#include "SDL3/SDL_init.h"        // SDL_AppResult
#include "SDL3/SDL_render.h"      // SDL_Renderer
#include "SDL3/SDL_video.h"       // SDL_Window

// `struct game` is an opaque data structure;
// only the implementation has access to its layout
struct game;

MBM_ABI void game_delete (struct game ** self);
MBM_ABI void game_draw (const struct game * self, SDL_Renderer * renderer);
MBM_ABI SDL_AppResult game_handle_event (struct game * self, const SDL_Event * event);
MBM_ABI void game_init (struct game * self, SDL_Renderer * renderer, const struct dims * dims);
MBM_ABI struct game * game_new (void);
MBM_ABI void game_update (struct game * self, const struct timings * timings);

#endif
