#ifndef MBM_SCENE_H_INCLUDED
#define MBM_SCENE_H_INCLUDED
#include "mbm/abi.h"
#include <stddef.h>

// `struct scene` is an opaque data structure
struct scene;

// define `Scene` as an alias for `struct scene`
typedef struct scene Scene;

// forward function declarations
MBM_ABI void scene_deinit (void);
MBM_ABI void scene_delete (struct scene ** scene);
MBM_ABI void scene_init (void);
MBM_ABI struct scene * scene_new (void);

#endif
