#include "mbm/background.h"       // type `Background` and associated functions
#include "mbm/game.h"             // type `Game` and associated functions
#include "mbm/scene.h"            // type `Scene` and associated functions
#include "mbm/spritesheet.h"      // type `Spritesheet` and associated functions
#include <SDL3/SDL_render.h>      // type `SDL_Renderer`
#include <SDL3/SDL_video.h>       // type `SDL_Window`
#include <stdio.h>
#include <stdlib.h>

// declare properties of `struct game`
struct game {
    Background * background;
    Scene * scene;
    Spritesheet * spritesheet;
};

// define pointer to singleton instance of `struct game`
static struct game * singleton = nullptr;

void game_delete (struct game ** self) {

    // delete objects
    scene_delete(&(*self)->scene);
    background_delete(&(*self)->background);
    spritesheet_delete(&(*self)->spritesheet);

    // release own resources
    free(*self);
    *self = nullptr;
}

void game_draw (struct game * self) {
    background_draw(self->background);
    scene_draw(self->scene);
}

void game_update (struct game * self) {
    background_update(self->background);
    scene_update(self->scene);
}

void game_init (struct game * self, SDL_Window * window, SDL_Renderer * renderer) {

    // empty-initialize the singleton instance of `struct game`
    *self = (struct game) {};

    // initialize the spritesheet
    self->spritesheet = spritesheet_new();
    spritesheet_init(self->spritesheet);

    // initialize the scene
    self->scene = scene_new();
    scene_init(self->scene, window, renderer);

    // initialize the background
    self->background = background_new();
    background_init(self->background, renderer);

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
