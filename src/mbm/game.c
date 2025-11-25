#include "mbm/background.h"       // type `Background` and associated functions
#include "mbm/game.h"             // type `Game` and associated functions
#include "mbm/ground.h"           // type `Ground` and associated functions
#include "mbm/moon.h"             // type `Moon` and associated functions
#include "mbm/scene.h"            // type `Scene` and associated functions
#include "mbm/spritesheet.h"      // type `Spritesheet` and associated functions
#include "mbm/turret.h"           // type `Turret` and associated functions
#include <SDL3/SDL_events.h>      // type `SDL_Event`
#include <SDL3/SDL_init.h>        // type `SDL_AppResult`
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <SDL3/SDL_video.h>       // type `SDL_Window`
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    MBM_GAME_STATE_LOBBY,
    MBM_GAME_STATE_PLAYING,
    MBM_GAME_STATE_LEN,
} State;

typedef void (*DrawFunction)(struct game * game, SDL_Renderer * renderer);
typedef SDL_AppResult (*HandleEventFunction)(SDL_Event * event);
typedef void (*UpdateFunction)(struct game * game, SDL_Window * window);

// declare properties of `struct game`
struct game {
    Background * background;
    DrawFunction draw_functions[MBM_GAME_STATE_LEN];
    HandleEventFunction handle_event_functions[MBM_GAME_STATE_LEN];
    Ground * ground;
    Moon * moon;
    State state;
    Scene * scene;
    Spritesheet * spritesheet;
    UpdateFunction update_functions[MBM_GAME_STATE_LEN];
    Turret * turret;
};

// define pointer to singleton instance of `struct game`
static struct game * singleton = nullptr;

// forward function declarations
static void game_draw_lobby (struct game * self, SDL_Renderer * renderer);
static void game_draw_playing (struct game * self, SDL_Renderer * renderer);
static SDL_AppResult game_handle_event_lobby (SDL_Event * event);
static SDL_AppResult game_handle_event_playing (SDL_Event * event);
static void game_update_lobby (struct game * self, SDL_Window * window);
static void game_update_playing (struct game * self, SDL_Window * window);

void game_delete (struct game ** self) {

    // delegate freeing dynamically allocated memory to the respective objects
    moon_delete(&(*self)->moon);
    ground_delete(&(*self)->ground);
    scene_delete(&(*self)->scene);
    background_delete(&(*self)->background);
    spritesheet_delete(&(*self)->spritesheet);
    turret_delete(&(*self)->turret);

    // release own resources
    free(*self);
    *self = nullptr;
}

void game_draw (struct game * self, SDL_Renderer * renderer) {
    self->draw_functions[self->state](self, renderer);
}

static void game_draw_lobby (struct game * self, SDL_Renderer * renderer) {
    background_draw(self->background, renderer);
    scene_draw(self->scene, renderer);
    ground_draw(self->ground, renderer);
    moon_draw(self->moon, self->spritesheet, renderer);
    turret_draw(self->turret, self->spritesheet, renderer);
}

static void game_draw_playing (struct game * self, SDL_Renderer * renderer) {
    background_draw(self->background, renderer);
    scene_draw(self->scene, renderer);
    ground_draw(self->ground, renderer);
    moon_draw(self->moon, self->spritesheet, renderer);
    turret_draw(self->turret, self->spritesheet, renderer);
}

SDL_AppResult game_handle_event (struct game * self, SDL_Event * event) {
    return self->handle_event_functions[self->state](event);
}

static SDL_AppResult game_handle_event_lobby (SDL_Event * event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.scancode == SDL_SCANCODE_Q) {
                return SDL_APP_SUCCESS;
            }
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult game_handle_event_playing (SDL_Event * event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.scancode == SDL_SCANCODE_Q) {
                return SDL_APP_SUCCESS;
            }
    }
    return SDL_APP_CONTINUE;
}

void game_init (struct game * self, SDL_Renderer * renderer) {

    // empty-initialize the singleton instance of `struct game`
    *self = (struct game) {};

    // initialize the state-based indirection to static draw functions
    self->draw_functions[MBM_GAME_STATE_LOBBY] = game_draw_lobby;
    self->draw_functions[MBM_GAME_STATE_PLAYING] = game_draw_playing;

    // initialize the state-based indirection to static update functions
    self->handle_event_functions[MBM_GAME_STATE_LOBBY] = game_handle_event_lobby;
    self->handle_event_functions[MBM_GAME_STATE_PLAYING] = game_handle_event_playing;

    // initialize the state-based indirection to static update functions
    self->update_functions[MBM_GAME_STATE_LOBBY] = game_update_lobby;
    self->update_functions[MBM_GAME_STATE_PLAYING] = game_update_playing;

    // initialize the gamestate
    self->state = MBM_GAME_STATE_LOBBY;

    // initialize the spritesheet
    self->spritesheet = spritesheet_new();
    spritesheet_init(self->spritesheet, renderer);

    // initialize the scene
    self->scene = scene_new();
    scene_init(self->scene);

    // initialize the background
    self->background = background_new();
    background_init(self->background);

    // initialize the ground
    self->ground = ground_new();
    ground_init(self->ground);

    // initialize the moon
    self->moon = moon_new();
    moon_init(self->moon, self->scene);

    // initialize the turret
    self->turret = turret_new();
    turret_init(self->turret, self->scene, self->ground);
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

void game_update (struct game * self, SDL_Window * window) {
    self->update_functions[self->state](self, window);
}

static void game_update_lobby (struct game * self, SDL_Window * window) {
    background_update(self->background);
    scene_update(self->scene, window);
    ground_update(self->ground, self->scene);
    moon_update(self->moon, self->scene);
    turret_update(self->turret, self->scene);
}

static void game_update_playing (struct game * self, SDL_Window * window) {
    background_update(self->background);
    scene_update(self->scene, window);
    ground_update(self->ground, self->scene);
    moon_update(self->moon, self->scene);
    turret_update(self->turret, self->scene);
}
