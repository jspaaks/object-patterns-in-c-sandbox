#include "mbm/background.h"       // type `Background` and associated functions
#include "mbm/game.h"             // type `Game` and associated functions
#include "mbm/world.h"            // type `World` and associated functions
#include <SDL3/SDL_events.h>      // type `SDL_Event`
#include <SDL3/SDL_init.h>        // type `SDL_AppResult`
#include <SDL3/SDL_keyboard.h>    // function `SDL_GetKeyboardState`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <SDL3/SDL_video.h>       // type `SDL_Window`
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    MBM_GAME_STATE_PLAYING,
    MBM_GAME_STATE_PAUSED,
    MBM_GAME_STATE_LEN,
} State;

typedef void (*DrawFunction)(struct game * game, SDL_Renderer * renderer);
typedef SDL_AppResult (*HandleEventFunction)(SDL_Event * event);
typedef void (*UpdateFunction)(struct game * game);

// declare properties of `struct game`
struct game {
    Background * background;
    DrawFunction draw_functions[MBM_GAME_STATE_LEN];
    HandleEventFunction handle_event_functions[MBM_GAME_STATE_LEN];
    State state;
    World * world;
    UpdateFunction update_functions[MBM_GAME_STATE_LEN];
};

// define pointer to singleton instance of `struct game`
static struct game * singleton = nullptr;

// forward function declarations
static void game_draw_paused (struct game * self, SDL_Renderer * renderer);
static void game_draw_playing (struct game * self, SDL_Renderer * renderer);
static SDL_AppResult game_handle_event_paused (SDL_Event * event);
static SDL_AppResult game_handle_event_playing (SDL_Event * event);
static void game_update_paused (struct game * self);
static void game_update_playing (struct game * self);

void game_delete (struct game ** self) {

    // delegate freeing dynamically allocated memory to the respective objects
    background_delete(&(*self)->background);
    world_delete(&(*self)->world);

    // release own resources
    free(*self);
    *self = nullptr;
}

void game_draw (struct game * self, SDL_Renderer * renderer) {
    self->draw_functions[self->state](self, renderer);
}

static void game_draw_paused (struct game * self, SDL_Renderer * renderer) {
    background_draw(self->background, renderer);
    world_draw(self->world, renderer);
}

static void game_draw_playing (struct game * self, SDL_Renderer * renderer) {
    background_draw(self->background, renderer);
    world_draw(self->world, renderer);
}

SDL_AppResult game_handle_event (struct game * self, SDL_Event * event) {
    return self->handle_event_functions[self->state](event);
}

static SDL_AppResult game_handle_event_paused (SDL_Event * event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE || event->key.key == SDLK_Q)
            return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult game_handle_event_playing (SDL_Event * event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE || event->key.key == SDLK_Q)
            return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

void game_init (struct game * self, SDL_Renderer * renderer, int view_width, int view_height) {

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
    world_init(self->world, renderer, view_width, view_height);
}

struct game * game_new (void) {
    if (singleton != nullptr) {
        // memory has already been allocated for `singleton`
        return singleton;
    }
    singleton = (struct game *) calloc(1, sizeof(struct game));
    if (singleton == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct game, aborting.\n");
        exit(1);
    }
    return singleton;
}

void game_update (struct game * self) {
    self->update_functions[self->state](self);
}

static void game_update_paused (struct game *) {}

static void game_update_playing (struct game * self) {
    const bool * key_states = SDL_GetKeyboardState(nullptr);
    if (key_states[SDL_SCANCODE_LEFT]) {
        world_move_view_left(self->world);
    }
    if (key_states[SDL_SCANCODE_RIGHT]) {
        world_move_view_right(self->world);
    }
    background_update(self->background);
    world_update(self->world);
}
