#ifndef MBM_DIMS_H_INCLUDED
#define MBM_DIMS_H_INCLUDED

struct dims {
    struct {
        int h;
        int w;
    } tile;
    struct {
        int h;
        int w;
    } view;
    struct {
        int h;
        int w;
    } window;
    struct {
        int h;
        int w;
    } wld;
};

#endif
