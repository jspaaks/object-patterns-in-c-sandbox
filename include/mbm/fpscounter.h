#ifndef MBM_FPSCOUNTER_H_INCLUDED
#define MBM_FPSCOUNTER_H_INCLUDED
#include "mbm/abi.h"
#include "mbm/timings.h"          // struct timings and associated functions
#include "SDL3/SDL_render.h"      // SDL_Renderer

// `struct fpscounter` is an opaque data structure;
// only the implementation has access to its layout
struct fpscounter;

void fpscounter_delete (struct fpscounter ** self);
void fpscounter_draw (const struct fpscounter * self, SDL_Renderer * renderer);
void fpscounter_init (struct fpscounter * self);
struct fpscounter * fpscounter_new (void);
void fpscounter_toggle (struct fpscounter * self);
void fpscounter_update (struct fpscounter * self, const struct timings * timings);

#endif
