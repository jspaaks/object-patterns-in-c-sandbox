#include "mbm/balloon.h"
#include "SDL3/SDL_rect.h"
#include <criterion/criterion.h>
#include <stddef.h>

Balloon * balloon = nullptr;

void balloon_setup (void) {
    balloon = balloon_new();
}

void balloon_teardown (void) {
    balloon_delete(&balloon);
}

Test (Balloon, test_init, .init=balloon_setup, .fini=balloon_teardown) {
    balloon_init(balloon);
    SDL_FRect actual = balloon_get_sim(balloon);
    const SDL_FRect expected = {
        .h = 12.0,
        .w = 9.0,
        .x = 30.0,
        .y = 40.0,
    };
    const char * subject = "Balloon";
    cr_assert(actual.h == expected.h,
              "%s: actual height (%.3f) not equal to expected height (%.3f)",
              subject, actual.h, expected.h);
    cr_assert(actual.w == expected.w,
              "%s: actual width (%.3f) not equal to expected width (%.3f)",
              subject, actual.w, expected.w);
    cr_assert(actual.x == expected.x,
              "%s: actual x (%.3f) not equal to expected x (%.3f)",
              subject, actual.x, expected.x);
    cr_assert(actual.y == expected.y,
              "%s: actual y (%.3f) not equal to expected y (%.3f)",
              subject, actual.y, expected.y);
}
