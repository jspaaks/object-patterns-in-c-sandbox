#include "mbm/balloon.h"
#include "mbm/timing.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct balloon Balloon;

static struct timing timing = {
    .dt = {
        .frame = 0.1,
    }
};

int main (void) {
    {
        fprintf(stdout, "Object patterns in C demo\n\n");
        Balloon * balloon1 = alloca(balloon_get_size());
        balloon_init(balloon1);
        balloon_increment_n(balloon1);
        balloon_increment_n(balloon1);
        balloon_increment_n(balloon1);
        balloon_print(balloon1, "balloon1");
    }
    {
        Balloon * balloon2 = balloon_new();
        balloon_init(balloon2);
        balloon_print(balloon2, "balloon2");
        balloon_update_pos(balloon2, &timing);
        balloon_print(balloon2, "balloon2");
        // free dynamically allocated resources
        balloon_delete(&balloon2);
    }

    return EXIT_SUCCESS;
}
