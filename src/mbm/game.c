#include "mbm/background.h"       // struct background and associated functions
#include "mbm/duck.h"             // struct duck and associated functions
#include "mbm/fpscounter.h"       // struct fpscounter and associated functions
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
typedef SDL_AppResult (*HandleEventFunction)(struct game * self, const SDL_Event * event);
typedef void (*UpdateFunction)(struct game * game, const struct timings * timings);

// declare properties of `struct game`
struct game {
    struct background * background;
    DrawFunction draw_functions[MBM_GAME_STATE_LEN];
    struct duck * duck;
    struct fpscounter * fpscounter;
    HandleEventFunction handle_event_functions[MBM_GAME_STATE_LEN];
    State state;
    struct world * world;
    UpdateFunction update_functions[MBM_GAME_STATE_LEN];
};

// define pointer to singleton instance of `struct game`
static struct game * singleton = nullptr;

// forward function declarations
static void game_draw_paused (const struct game * self, SDL_Renderer * renderer);
static void game_draw_playing (const struct game * self, SDL_Renderer * renderer);
static SDL_AppResult game_handle_event_paused (struct game * self, const SDL_Event * event);
static SDL_AppResult game_handle_event_playing (struct game * self, const SDL_Event * event);
static void game_update_paused (struct game * self, const struct timings * timings);
static void game_update_playing (struct game * self, const struct timings * timings);

void game_delete (struct game ** self) {

    // delegate freeing dynamically allocated memory to the respective objects
    fpscounter_delete(&(*self)->fpscounter);
    duck_delete(&(*self)->duck);
    background_delete(&(*self)->background);
    world_delete(&(*self)->world);

    // release own resources
    SDL_free(*self);
    *self = nullptr;
}

void game_draw (const struct game * self, SDL_Renderer * renderer) {
    self->draw_functions[self->state](self, renderer);
}

static void game_draw_paused (const struct game * self, SDL_Renderer * renderer) {
    (void) self;
    (void) renderer;
}

static void game_draw_playing (const struct game * self, SDL_Renderer * renderer) {
    background_draw(self->background, renderer);
    world_draw(self->world, renderer);
    duck_draw(self->duck, renderer);
    fpscounter_draw(self->fpscounter, renderer);
}

SDL_AppResult game_handle_event (struct game * self, const SDL_Event * event) {
    return self->handle_event_functions[self->state](self, event);
}

static SDL_AppResult game_handle_event_paused (struct game * self, const SDL_Event * event) {
    (void) self;
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE || event->key.key == SDLK_Q)
            return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult game_handle_event_playing (struct game * self, const SDL_Event * event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE || event->key.key == SDLK_Q) {
                return SDL_APP_SUCCESS;
            }
            if (event->key.key == SDLK_F) {
                fpscounter_toggle(self->fpscounter);
            }
    }
    return SDL_APP_CONTINUE;
}

void game_init (struct game * self, SDL_Renderer * renderer, const struct dims * dims) {

    // empty-initialize the singleton instance of `struct game`
    *self = (struct game) {};

    // initialize the state-based indirection to static draw functions
    self->draw_functions[MBM_GAME_STATE_PAUSED] = game_draw_paused;
    self->draw_functions[MBM_GAME_STATE_PLAYING] = game_draw_playing;

    // initialize the state-based indirection to static update functions
    self->handle_event_functions[MBM_GAME_STATE_PAUSED] = game_handle_event_paused;
    self->handle_event_functions[MBM_GAME_STATE_PLAYING] = game_handle_event_playing;

    // initialize the state-based indirection to static update functions
    self->update_functions[MBM_GAME_STATE_PAUSED] = game_update_paused;
    self->update_functions[MBM_GAME_STATE_PLAYING] = game_update_playing;

    // initialize the gamestate
    self->state = MBM_GAME_STATE_PLAYING;

    // initialize the background
    self->background = background_new();
    background_init(self->background);

    // initialize the world
    self->world = world_new();
    world_init(self->world, renderer, dims);

    // initialize the duck
    self->duck = duck_new();
    duck_init(self->duck, renderer, dims);

    // initialize the fpscounter
    self->fpscounter = fpscounter_new();
    fpscounter_init(self->fpscounter);
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
    self->update_functions[self->state](self, timings);
}

static void game_update_paused (struct game * self, const struct timings * timings) {
    (void) self;
    (void) timings;
}

static void game_update_playing (struct game * self, const struct timings * timings) {
    const bool * key_states = SDL_GetKeyboardState(nullptr);
    if (key_states[SDL_SCANCODE_LEFT]) {
        world_move_view_left(self->world, timings);
    }
    if (key_states[SDL_SCANCODE_RIGHT]) {
        world_move_view_right(self->world, timings);
    }
    if (key_states[SDL_SCANCODE_I]) {
        duck_set_animation_state_idle(self->duck);
    }
    if (key_states[SDL_SCANCODE_W]) {
        duck_set_animation_state_walking(self->duck);
    }
    background_update(self->background, timings);
    world_update(self->world, timings);
    duck_update(self->duck, timings);
    fpscounter_update(self->fpscounter, timings);
}
