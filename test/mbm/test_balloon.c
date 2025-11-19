#include "mbm/balloon.h"
#include <criterion/criterion.h>
#include <stddef.h>

typedef struct balloon Balloon;

Test (balloon_init, test_init) {
    size_t sz = balloon_get_size();
    Balloon * balloon = (Balloon *) alloca(sz);
    balloon_init(balloon);
    int actual = balloon_get_n(balloon);
    const int expected = 0;
    cr_assert(actual == expected);
}
