// BLUENOISE :: https://github.com/prideout/parg
// Generator for 2D point sample sequences using Recursive Wang Tiles.
//
// In addition to this source code, you'll need to download the following
// dataset, which is about 2 MB:
//
//     http://github.prideout.net/assets/bluenoise.bin
//
// This is an implementation of the algorithm described in:
//
//     Recursive Wang Tiles for Real-Time Blue Noise
//     Johannes Kopf, Daniel Cohen-Or, Oliver Deussen, Dani Lischinski
//     ACM Transactions on Graphics 25, 3 (Proc. SIGGRAPH 2006)
//
// If you use this software for research purposes, please cite the above paper
// in any resulting publication.
//
// The MIT License
// Copyright (c) 2015 Philip Rideout and Johannes Kopf
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

// Encapsulates a tile set and an optional density function.
typedef struct par_bluenoise_context_s par_bluenoise_context;

// Creates a bluenoise context using the given tileset.  The first argument is
// either a filepath to the tileset, or the contents of the tileset.  For the
// latter option, the caller should specify a non-zero buffer length (bytes).
par_bluenoise_context* par_bluenoise_create(
    const char* filepath_or_buffer, int buffer_length, int maxpts);

// Frees all memory associated with the given bluenoise context.
void par_bluenoise_free(par_bluenoise_context* ctx);

// Copies a grayscale image into the bluenoise context to guide point density.
// Darker regions generate a higher number of points. The given bytes-per-pixel
// value is the stride between pixels.
void par_bluenoise_density_from_gray(par_bluenoise_context* ctx,
    const unsigned char* pixels, int width, int height, int bpp);

// Create a binary mask to guide point density. The given bytes-per-pixel
// value is the stride between pixels, which must be 4 or less.
void par_bluenoise_density_from_color(par_bluenoise_context* ctx,
    const unsigned char* pixels, int width, int height, int bpp,
    unsigned int background_color, int invert);

// Generates samples using Recursive Wang Tiles.  This is really fast!
// The returned pointer is a list of three-tuples, where XY are in [0,1],
// and Z is a rank value that can be used to create a progressive ordering.
// The caller should not free the returned pointer.  The xyz arguments
// control a square region within the density function.
float* par_bluenoise_generate(par_bluenoise_context* ctx, float density,
    float x, float y, float z, int* npts);

#define clampi(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define sqri(a) (a * a)
#define mini(a, b) ((a < b) ? a : b)
#define maxi(a, b) ((a > b) ? a : b)

typedef struct {
    float x;
    float y;
} par_vec2;

typedef struct {
    float x;
    float y;
    float rank;
} par_vec3;

typedef struct {
    int n, e, s, w;
    int nsubtiles, nsubdivs, npoints, nsubpts;
    int** subdivs;
    par_vec2* points;
    par_vec2* subpts;
} par_tile;

struct par_bluenoise_context_s {
    par_vec3* points;
    par_tile* tiles;
    float global_density;
    float clipMinX, clipMaxX, clipMinY, clipMaxY;
    int ntiles, nsubtiles, nsubdivs;
    float vpos[3];
    int npoints;
    int maxpoints;
    int density_width;
    int density_height;
    float* density;
};

static float sample_density(par_bluenoise_context* ctx, float x, float y)
{
    float* density = ctx->density;
    int width = ctx->density_width;
    int height = ctx->density_height;

    x -= 0.5;
    y -= 0.5;
    float tx = x * maxi(width, height);
    float ty = y * maxi(width, height);
    x += 0.5;
    y += 0.5;
    tx += width / 2;
    ty += height / 2;

    int ix = clampi((int) tx, 0, width - 2);
    int iy = clampi((int) ty, 0, height - 2);
    tx -= ix;
    ty -= iy;
    float sample = (density[iy * width + ix] * (1 - tx) * (1 - ty) +
        density[iy * width + ix + 1] * tx * (1 - ty) +
        density[(iy + 1) * width + ix] * (1 - tx) * ty +
        density[(iy + 1) * width + ix + 1] * tx * ty);
    return sample;
}

static void recurse_tile(
    par_bluenoise_context* ctx, par_tile* tile, float x, float y, int level)
{
    float tileSize = 1.f / powf(ctx->nsubtiles, level);
    if ((x + tileSize < ctx->clipMinX) || (x > ctx->clipMaxX) ||
        (y + tileSize < ctx->clipMinY) || (y > ctx->clipMaxY)) {
        return;
    }
    float mag = powf(ctx->vpos[2], -2.f);
    float threshold =
        mag / powf(ctx->nsubtiles, 2.f * level) * ctx->global_density -
        tile->npoints;
    int ntests = mini(tile->nsubpts, threshold);
    float factor =
        1.f / mag * powf(ctx->nsubtiles, 2.f * level) / ctx->global_density;
    for (int i = 0; i < ntests; i++) {
        float px = x + tile->subpts[i].x * tileSize,
            py = y + tile->subpts[i].y * tileSize;
        if ((px < ctx->clipMinX) || (px > ctx->clipMaxX) ||
            (py < ctx->clipMinY) || (py > ctx->clipMaxY)) {
            continue;
        }
        if (sample_density(ctx, px, py) < (i + tile->npoints) * factor) {
            continue;
        }
        ctx->points[ctx->npoints].x = px;
        ctx->points[ctx->npoints].y = py;
        ctx->points[ctx->npoints].rank = (level + 1) + i * factor;
        ctx->npoints++;
    }
    const float scale = tileSize / ctx->nsubtiles;
    if (threshold > tile->nsubpts) {
        for (int ty = 0; ty < ctx->nsubtiles; ty++) {
            for (int tx = 0; tx < ctx->nsubtiles; tx++) {
                int tileIndex = tile->subdivs[0][ty * ctx->nsubtiles + tx];
                par_tile* subtile = &ctx->tiles[tileIndex];
                recurse_tile(
                    ctx, subtile, x + tx * scale, y + ty * scale, level + 1);
            }
        }
    }
}

float* par_bluenoise_generate(par_bluenoise_context* ctx, float density,
    float x, float y, float z, int* npts)
{
    ctx->global_density = density;
    ctx->vpos[0] = x;
    ctx->vpos[1] = y;
    ctx->vpos[2] = z;
    ctx->clipMinX = x;
    ctx->clipMaxX = x + z;
    ctx->clipMinY = y;
    ctx->clipMaxY = y + z;
    ctx->npoints = 0;
    int ntests =
        mini(ctx->tiles[0].npoints, powf(z, -2.f) * ctx->global_density);
    float factor = 1.f / powf(z, -2) / ctx->global_density;
    for (int i = 0; i < ntests; i++) {
        float px = ctx->tiles[0].points[i].x, py = ctx->tiles[0].points[i].y;
        if ((px < ctx->clipMinX) || (px > ctx->clipMaxX) ||
            (py < ctx->clipMinY) || (py > ctx->clipMaxY)) {
            continue;
        }
        if (sample_density(ctx, px, py) < i * factor) {
            continue;
        }
        ctx->points[ctx->npoints].x = px;
        ctx->points[ctx->npoints].y = py;
        ctx->points[ctx->npoints].rank = i * factor;
        ctx->npoints++;
    }
    recurse_tile(ctx, &ctx->tiles[0], 0, 0, 0);
    *npts = ctx->npoints;
    return &ctx->points->x;
}

#define freadi()   \
    *((int*) ptr); \
    ptr += sizeof(int)

#define freadf()     \
    *((float*) ptr); \
    ptr += sizeof(float)

par_bluenoise_context* par_bluenoise_create(
    const char* filepath, int nbytes, int maxpts)
{
    par_bluenoise_context* ctx = malloc(sizeof(par_bluenoise_context));
    ctx->maxpoints = maxpts;
    ctx->points = malloc(maxpts * sizeof(par_vec3));
    ctx->density = 0;

    char* buf = 0;
    if (nbytes == 0) {
        FILE* fin = fopen(filepath, "rb");
        assert(fin);
        fseek(fin, 0, SEEK_END);
        nbytes = ftell(fin);
        fseek(fin, 0, SEEK_SET);
        buf = malloc(nbytes);
        fread(buf, nbytes, 1, fin);
        fclose(fin);
    }

    const char* ptr = buf ? buf : filepath;
    int ntiles = ctx->ntiles = freadi();
    int nsubtiles = ctx->nsubtiles = freadi();
    int nsubdivs = ctx->nsubdivs = freadi();
    par_tile* tiles = ctx->tiles = malloc(sizeof(par_tile) * ntiles);
    for (int i = 0; i < ntiles; i++) {
        tiles[i].n = freadi();
        tiles[i].e = freadi();
        tiles[i].s = freadi();
        tiles[i].w = freadi();
        tiles[i].subdivs = malloc(sizeof(int) * nsubdivs);
        for (int j = 0; j < nsubdivs; j++) {
            int* subdiv = malloc(sizeof(int) * sqri(nsubtiles));
            for (int k = 0; k < sqri(nsubtiles); k++) {
                subdiv[k] = freadi();
            }
            tiles[i].subdivs[j] = subdiv;
        }
        tiles[i].npoints = freadi();
        tiles[i].points = malloc(sizeof(par_vec2) * tiles[i].npoints);
        for (int j = 0; j < tiles[i].npoints; j++) {
            tiles[i].points[j].x = freadf();
            tiles[i].points[j].y = freadf();
        }
        tiles[i].nsubpts = freadi();
        tiles[i].subpts = malloc(sizeof(par_vec2) * tiles[i].nsubpts);
        for (int j = 0; j < tiles[i].nsubpts; j++) {
            tiles[i].subpts[j].x = freadf();
            tiles[i].subpts[j].y = freadf();
        }
    }
    free(buf);
    return ctx;
}

void par_bluenoise_density_from_gray(par_bluenoise_context* ctx,
    const unsigned char* pixels, int width, int height, int bpp)
{
    ctx->density_width = width;
    ctx->density_height = height;
    ctx->density = malloc(width * height * sizeof(float));
    float scale = 1.0f / 255.0f;
    float* dst = ctx->density;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            *dst++ = 1 - (*pixels) * scale;
            pixels += bpp;
        }
    }
}

void par_bluenoise_density_from_color(par_bluenoise_context* ctx,
    const unsigned char* pixels, int width, int height, int bpp,
    unsigned int background_color, int invert)
{
    unsigned int bkgd = background_color;
    ctx->density_width = width;
    ctx->density_height = height;
    ctx->density = malloc(width * height * sizeof(float));
    float* dst = ctx->density;
    unsigned int mask = 0x000000ffu;
    if (bpp > 1)
        mask |= 0x0000ff00u;
    if (bpp > 2)
        mask |= 0x00ff0000u;
    if (bpp > 3)
        mask |= 0xff000000u;
    assert(bpp <= 4);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            unsigned int val = (*((unsigned int*) pixels)) & mask;
            *dst++ = invert ? (val == bkgd) : (val != bkgd);
            pixels += bpp;
        }
    }
}

void par_bluenoise_free(par_bluenoise_context* ctx)
{
    free(ctx->points);
    for (int t = 0; t < ctx->ntiles; t++) {
        for (int s = 0; s < ctx->nsubdivs; s++) {
            free(ctx->tiles[t].subdivs[s]);
        }
        free(ctx->tiles[t].subdivs);
        free(ctx->tiles[t].points);
        free(ctx->tiles[t].subpts);
    }
    free(ctx->tiles);
    free(ctx->density);
}
