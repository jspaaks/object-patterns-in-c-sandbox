#include "mbm/background.h"       // struct background and associated functions
#include "mbm/caption_fps.h"      // struct caption_fps and associated functions
#include "mbm/caption_paused.h"   // struct caption_paused and associated functions
#include "mbm/duck.h"             // struct duck and associated functions
#include "mbm/game.h"             // struct game and associated functions
#include "mbm/timings.h"          // struct timings and associated functions
#include "mbm/world.h"            // struct world and associated functions
#include "SDL3/SDL_error.h"       // SDL_GetError
#include "SDL3/SDL_events.h"      // SDL_Event
#include "SDL3/SDL_init.h"        // SDL_AppResult
#include "SDL3/SDL_keyboard.h"    // SDL_GetKeyboardState
#include "SDL3/SDL_log.h"         // SDL_LogCritical
#include "SDL3/SDL_render.h"      // SDL_Renderer
#include "SDL3/SDL_stdinc.h"      // SDL_free, SDL_calloc
#include "SDL3/SDL_video.h"       // SDL_Window
#include <stdlib.h>               // exit

typedef enum {
    MBM_GAME_STATE_PLAYING,
    MBM_GAME_STATE_PAUSED,
    MBM_GAME_STATE_LEN,
} State;

typedef void (*DrawFunction)(const struct game * game, SDL_Renderer * renderer);
typedef SDL_AppResult (*HandleEventFunction)(struct game * self, SDL_Renderer * renderer, const SDL_Event * event);
typedef void (*UpdateFunction)(struct game * game, const struct timings * timings);

struct delegation_functions {
    DrawFunction draw;
    HandleEventFunction handle_event;
    UpdateFunction update;
};

// declare properties of `struct game`
struct game {
    struct background * background;
    struct caption_fps * caption_fps;
    struct caption_paused * caption_paused;
    struct duck * duck;
    struct delegation_functions delegated_functions[MBM_GAME_STATE_LEN];
    State state;
    bool vsync_enabled;
    struct world * world;
};

// define pointer to singleton instance of `struct game`
static struct game * singleton = nullptr;

// forward function declarations
static void draw_paused (const struct game * self, SDL_Renderer * renderer);
static void draw_playing (const struct game * self, SDL_Renderer * renderer);
static SDL_AppResult handle_event_paused (struct game * self, SDL_Renderer * renderer, const SDL_Event * event);
static SDL_AppResult handle_event_playing (struct game * self, SDL_Renderer * renderer, const SDL_Event * event);
static void pause (struct game * self);
static void play (struct game * self);
static void toggle_vsync (struct game * self, SDL_Renderer * renderer);
static void update_paused (struct game * self, const struct timings * timings);
static void update_playing (struct game * self, const struct timings * timings);


void game_delete (struct game ** self) {

    // delegate freeing dynamically allocated memory to the respective objects
    caption_paused_delete(&(*self)->caption_paused);
    caption_fps_delete(&(*self)->caption_fps);
    duck_delete(&(*self)->duck);
    background_delete(&(*self)->background);
    world_delete(&(*self)->world);

    // release own resources
    SDL_free(*self);
    *self = nullptr;
}

void game_draw (const struct game * self, SDL_Renderer * renderer) {
    self->delegated_functions[self->state].draw(self, renderer);
}

static void draw_paused (const struct game * self, SDL_Renderer * renderer) {
    background_draw(self->background, renderer);
    world_draw(self->world, renderer);
    duck_draw(self->duck, renderer);
    caption_fps_draw(self->caption_fps, renderer);
    caption_paused_draw(self->caption_paused, renderer);
}

static void draw_playing (const struct game * self, SDL_Renderer * renderer) {
    background_draw(self->background, renderer);
    world_draw(self->world, renderer);
    duck_draw(self->duck, renderer);
    caption_fps_draw(self->caption_fps, renderer);
}

SDL_AppResult game_handle_event (struct game * self, SDL_Renderer * renderer, const SDL_Event * event) {
    return self->delegated_functions[self->state].handle_event(self, renderer, event);
}

static SDL_AppResult handle_event_paused (struct game * self, SDL_Renderer * renderer, const SDL_Event * event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        switch (event->key.key) {
        case SDLK_ESCAPE:
            play(self);
            break;
        case SDLK_Q:
            return SDL_APP_SUCCESS;
        case SDLK_F:
            caption_fps_toggle(self->caption_fps);
            break;
        case SDLK_V:
            toggle_vsync(self, renderer);
            break;
        }
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult handle_event_playing (struct game * self, SDL_Renderer * renderer, const SDL_Event * event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        switch (event->key.key) {
        case SDLK_ESCAPE:
            pause(self);
            break;
        case SDLK_SPACE:
            duck_jump(self->duck);
            break;
        case SDLK_F:
            caption_fps_toggle(self->caption_fps);
            break;
        case SDLK_V:
            toggle_vsync(self, renderer);
            break;
        }
    }
    return SDL_APP_CONTINUE;
}

void game_init (struct game * self, SDL_Renderer * renderer, const struct dims * dims) {

    // empty-initialize the singleton instance of `struct game`
    *self = (struct game) {};

    // initialize the state-based indirection to static functions for game state 'paused'
    self->delegated_functions[MBM_GAME_STATE_PAUSED] = (struct delegation_functions){
        .draw = draw_paused,
        .handle_event = handle_event_paused,
        .update = update_paused,
    };

    // initialize the state-based indirection to static functions for game state 'playing'
    self->delegated_functions[MBM_GAME_STATE_PLAYING] = (struct delegation_functions){
        .draw = draw_playing,
        .handle_event = handle_event_playing,
        .update = update_playing,
    };

    // initialize the gamestate
    self->state = MBM_GAME_STATE_PLAYING;

    // initialize vsync to false, then toggle it
    self->vsync_enabled = false;
    toggle_vsync(self, renderer);

    // initialize the background
    self->background = background_new();
    background_init(self->background);

    // initialize the world
    self->world = world_new();
    world_init(self->world, renderer, dims);

    // initialize the duck
    self->duck = duck_new();
    duck_init(self->duck, renderer, dims);

    // initialize the caption_fps
    self->caption_fps = caption_fps_new();
    caption_fps_init(self->caption_fps);

    // initialize the caption_paused
    self->caption_paused = caption_paused_new();
    caption_paused_init(self->caption_paused, renderer, dims);
}

struct game * game_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct game *) SDL_calloc(1, sizeof(struct game));
    if (singleton == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Could not allocate dynamic memory for struct game, aborting; %s\n",
                        SDL_GetError());
        exit(1);
    }
    return singleton;
}

void game_update (struct game * self, const struct timings * timings) {
    self->delegated_functions[self->state].update(self, timings);
}

static void pause (struct game * self) {
    self->state = MBM_GAME_STATE_PAUSED;
}

static void play (struct game * self) {
    self->state = MBM_GAME_STATE_PLAYING;
}

static void toggle_vsync (struct game * self, SDL_Renderer * renderer) {
    self->vsync_enabled = !self->vsync_enabled;
    SDL_SetRenderVSync(renderer, self->vsync_enabled ? SDL_RENDERER_VSYNC_ADAPTIVE : SDL_RENDERER_VSYNC_DISABLED);
}

static void update_paused (struct game * self, const struct timings * timings) {
    caption_fps_update(self->caption_fps, timings);
}

static void update_playing (struct game * self, const struct timings * timings) {
    duck_halt(self->duck);
    const bool * key_states = SDL_GetKeyboardState(nullptr);
    if (key_states[SDL_SCANCODE_LEFT]) {
        duck_walk_left(self->duck);
    }
    if (key_states[SDL_SCANCODE_RIGHT]) {
        duck_walk_right(self->duck);
    }
    background_update(self->background, timings);
    world_update(self->world, timings);
    duck_update(self->duck, self->world, timings);
    caption_fps_update(self->caption_fps, timings);

    duck_handle_collision_with_world(self->duck, self->world);
}
