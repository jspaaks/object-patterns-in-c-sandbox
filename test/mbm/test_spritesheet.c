#include "mbm/spritesheet.h"
#include "SDL3/SDL_rect.h"
#include <criterion/criterion.h>
#include <stddef.h>

static Spritesheet * spritesheet = nullptr;

void spritesheet_setup (void) {
    spritesheet = spritesheet_new();
    spritesheet_init(spritesheet);
}

void spritesheet_teardown(void) {
    spritesheet_delete(&spritesheet);
}

Test (Spritesheet, test_get_path, .init=spritesheet_setup, .fini=spritesheet_teardown) {
    const char * actual = spritesheet_get_path(spritesheet);
    cr_assert(actual != nullptr);
}

Test (Spritesheet, test_get_rect_balloon_orange, .init=spritesheet_setup, .fini=spritesheet_teardown) {
    const SDL_Rect actual = spritesheet_get_rect_balloon_orange(spritesheet);
    const SDL_Rect expected = (SDL_Rect) {
        .h = 12,
        .w = 9,
        .x = 184,
        .y = 1,
    };
    const char * subject = "Orange balloon";
    cr_assert(actual.h == expected.h,
              "%s: actual height (%d) not equal to expected height (%d)",
              subject, actual.h, expected.h);
    cr_assert(actual.w == expected.w,
              "%s: actual width (%d) not equal to expected width (%d)",
              subject, actual.w, expected.w);
    cr_assert(actual.x == expected.x,
              "%s: actual x (%d) not equal to expected x (%d)",
              subject, actual.x, expected.x);
    cr_assert(actual.y == expected.y,
              "%s: actual y (%d) not equal to expected y (%d)",
              subject, actual.y, expected.y);
}

Test (Spritesheet, test_get_rect_balloon_red, .init=spritesheet_setup, .fini=spritesheet_teardown) {
    const SDL_Rect actual = spritesheet_get_rect_balloon_red(spritesheet);
    const SDL_Rect expected = (SDL_Rect) {
        .h = 8,
        .w = 6,
        .x = 184,
        .y = 20,
    };
    const char * subject = "Red balloon";
    cr_assert(actual.h == expected.h,
              "%s: actual height (%d) not equal to expected height (%d)",
              subject, actual.h, expected.h);
    cr_assert(actual.w == expected.w,
              "%s: actual width (%d) not equal to expected width (%d)",
              subject, actual.w, expected.w);
    cr_assert(actual.x == expected.x,
              "%s: actual x (%d) not equal to expected x (%d)",
              subject, actual.x, expected.x);
    cr_assert(actual.y == expected.y,
              "%s: actual y (%d) not equal to expected y (%d)",
              subject, actual.y, expected.y);
}

Test (Spritesheet, test_get_rect_balloon_yellow, .init=spritesheet_setup, .fini=spritesheet_teardown) {
    const SDL_Rect actual = spritesheet_get_rect_balloon_yellow(spritesheet);
    const SDL_Rect expected = (SDL_Rect) {
        .h = 16,
        .w = 12,
        .x = 166,
        .y = 1,
    };
    const char * subject = "Yellow balloon";
    cr_assert(actual.h == expected.h,
              "%s: actual height (%d) not equal to expected height (%d)",
              subject, actual.h, expected.h);
    cr_assert(actual.w == expected.w,
              "%s: actual width (%d) not equal to expected width (%d)",
              subject, actual.w, expected.w);
    cr_assert(actual.x == expected.x,
              "%s: actual x (%d) not equal to expected x (%d)",
              subject, actual.x, expected.x);
    cr_assert(actual.y == expected.y,
              "%s: actual y (%d) not equal to expected y (%d)",
              subject, actual.y, expected.y);
}
