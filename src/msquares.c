// MSQUARES :: https://github.com/prideout/parg
// Converts fp32 grayscale images, or 8-bit color images, into triangles.
//
// For grayscale images, a threshold is specified to determine insideness.
// Color images can be r8, rg16, rgb24, or rgba32.  In all cases, the exact
// color of each pixel determines insideness.  For a visual overview of
// the API, see:
//
//     https://....
//
// The MIT License
// Copyright (c) 2015 Philip Rideout

#include <stdint.h>

// -----------------------------------------------------------------------------
// BEGIN PUBLIC API
// -----------------------------------------------------------------------------

typedef uint8_t par_byte;

typedef struct {
    float* points;
    int npoints;
    int dim;
    uint16_t* triangles;
    int ntriangles;
} par_trimesh;

#define PAR_MSQUARES_INVERT   (1 << 0)
#define PAR_MSQUARES_DUAL     (1 << 1)
#define PAR_MSQUARES_WELD     (1 << 2)
#define PAR_MSQUARES_CONNECT  (1 << 3)
#define PAR_MSQUARES_SIMPLIFY (1 << 4)
#define PAR_MSQUARES_HEIGHTS  (1 << 5)

par_msquares_meshlist* par_msquares_from_grayscale(
    float const* data, int width, int height, int cellsize,
    float threshold, int flags);

par_msquares_meshlist* par_msquares_from_levels(
    float const* data, int width, int height, int cellsize,
    float const* thresholds, int nthresholds, int flags);

par_msquares_meshlist* par_msquares_from_color(
    par_byte const* data, int width, int height, int cellsize,
    par_byte color, int bpp, int flags);

par_msquares_meshlist* par_msquares_from_colors(
    par_byte const* data, int width, int height, int cellsize,
    par_byte const* colors, int ncolors, int bpp, int flags);

par_trimesh* par_msquares_get_mesh(par_msquares_meshlist*, int n);

int par_msquares_get_count(par_msquares_meshlist* );

void par_msquares_free(par_msquares_meshlist* );

// -----------------------------------------------------------------------------
// END PUBLIC API
// -----------------------------------------------------------------------------
