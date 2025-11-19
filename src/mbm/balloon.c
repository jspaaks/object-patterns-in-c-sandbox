#include "mbm/balloon.h"
#include "mbm/timing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// `struct balloon` is an opaque data structure
struct balloon {
    float h;
    float u;
    float v;
    float w;
    float x;
    float y;
    int n;
    struct shared * shared;
};

// define static properties of `struct balloon`
static struct shared {
    int h_src;
    int w_src;
    int x_src;
    int y_src;
} shared = {
    .h_src = 12,
    .w_src = 9,
    .x_src = 184,
    .y_src = 1
};

void balloon_delete (struct balloon ** self) {
    free(*self);
    *self = nullptr;
}

int balloon_get_n (struct balloon * self) {
    return self->n;
}

size_t balloon_get_size (void) {
    return sizeof(struct balloon);
}

void balloon_increment_n (struct balloon * self) {
    self->n++;
}

void balloon_init (struct balloon * self) {
    self->h = 0.0;
    self->n = 0.0;
    self->shared = &shared;
    self->u = 0.0;
    self->v = -30.0;
    self->w = 0.0;
    self->x = 0.0;
    self->y = 0.0;
}

struct balloon * balloon_new (void) {
    struct balloon * p = (struct balloon *) calloc(1, sizeof(struct balloon));
    if (p == nullptr) {
        fprintf(stderr, "ERROR allocating dynamic memory for struct balloon, aborting.\n");
        exit(1);
    }
    return p;
}

void balloon_print (struct balloon * self, const char * name) {
    static const char * name_default = "Untitled balloon";
    if (name == nullptr) {
        name = name_default;
    }
    fprintf(stdout,
            "%s (Balloon@%p): {\n"
            "  .h = %.3f\n"
            "  .n = %d\n"
            "  .shared@%p: {\n"
            "    .h_src = %d\n"
            "    .w_src = %d\n"
            "    .x_src = %d\n"
            "    .y_src = %d\n"
            "  }\n"
            "  .u = %.3f\n"
            "  .v = %.3f\n"
            "  .w = %.3f\n"
            "  .x = %.3f\n"
            "  .y = %.3f\n"
            "}\n",
            name, (void *) self,
            self->h,
            self->n,
            (void *) self->shared,
            self->shared->h_src,
            self->shared->w_src,
            self->shared->x_src,
            self->shared->y_src,
            self->u,
            self->v,
            self->w,
            self->x,
            self->y);
}

void balloon_update_pos (struct balloon * self, const struct timing * timing) {
    self->x += self->u * timing->dt.frame;
    self->y += self->v * timing->dt.frame;
}