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
    const char* filepath_or_buffer, int buffer_length);

// Frees all memory associated with the given bluenoise context.
void par_bluenoise_free(par_bluenoise_context* ctx);

// Copies a grayscale one-byte-per-pixel image into the bluenoise context to
// guide point density.  Darker regions generate a higher number of points.
void par_bluenoise_set_density(
    par_bluenoise_context* ctx, const unsigned char* pixels, int size);

// Generates samples using Recursive Wang Tiles.  This is really fast!
// The returned pointer points to a list of two-tuples in the [0,1] range.
// The caller should not free the returned pointer.  The xyz arguments
// control a square region within the density function.
float* par_bluenoise_generate(
    par_bluenoise_context* ctx, float x, float y, float z, int* npts);

#define MAX_POINTS 1024 * 1024
#define clampi(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define sqri(a) (a * a)
#define mini(a, b) ((a < b) ? a : b)

typedef struct {
    float x;
    float y;
} par_vec2;

typedef struct {
    int n, e, s, w;
    int nsubtiles, nsubdivs, npoints, nsubpts;
    int** subdivs;
    par_vec2* points;
    par_vec2* subpts;
} par_tile;

struct par_bluenoise_context_s {
    par_vec2* points;
    par_tile* tiles;
    float toneScale;
    float clipMinX, clipMaxX, clipMinY, clipMaxY;
    int ntiles, nsubtiles, nsubdivs;
    float vpos[3];
    int npoints;
    int ndensity;
    float* density;
};

static float sample_density(par_bluenoise_context* ctx, float x, float y)
{
    int ndensity = ctx->ndensity;
    float* density = ctx->density;
    float tx = x * ndensity;
    float ty = y * ndensity;
    int ix = clampi((int) tx, 0, ndensity - 2);
    int iy = clampi((int) ty, 0, ndensity - 2);
    tx -= ix;
    ty -= iy;
    float sample = (density[iy * ndensity + ix] * (1 - tx) * (1 - ty) +
        density[iy * ndensity + ix + 1] * tx * (1 - ty) +
        density[(iy + 1) * ndensity + ix] * (1 - tx) * ty +
        density[(iy + 1) * ndensity + ix + 1] * tx * ty);
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
    int numTests = mini(tile->nsubpts,
            powf(ctx->vpos[2], -2.f) / powf(ctx->nsubtiles, 2.f * level) *
            ctx->toneScale -
            tile->npoints);
    float factor = 1.f / powf(ctx->vpos[2], -2.f) *
        powf(ctx->nsubtiles, 2.f * level) / ctx->toneScale;
    for (int i = 0; i < numTests; i++) {
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
        ctx->npoints++;
    }
    const float scale = tileSize / ctx->nsubtiles;
    if (powf(ctx->vpos[2], -2.f) / powf(ctx->nsubtiles, 2.f * level) *
        ctx->toneScale -
        tile->npoints >
        tile->nsubpts) {
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

float* par_bluenoise_generate(
    par_bluenoise_context* ctx, float x, float y, float z, int* npts)
{
    ctx->vpos[0] = x;
    ctx->vpos[1] = y;
    ctx->vpos[2] = z;
    ctx->clipMinX = x;
    ctx->clipMaxX = x + z;
    ctx->clipMinY = y;
    ctx->clipMaxY = y + z;
    ctx->npoints = 0;
    int numTests = mini(ctx->tiles[0].npoints, powf(z, -2.f) * ctx->toneScale);
    float factor = 1.f / powf(z, -2) / ctx->toneScale;
    for (int i = 0; i < numTests; i++) {
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

par_bluenoise_context* par_bluenoise_create(const char* filepath, int nbytes)
{
    par_bluenoise_context* ctx = malloc(sizeof(par_bluenoise_context));
    ctx->points = malloc(MAX_POINTS * sizeof(par_vec2));
    ctx->toneScale = 200000;
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

void par_bluenoise_set_density(
    par_bluenoise_context* ctx, const unsigned char* pixels, int size)
{
    ctx->ndensity = size;
    ctx->density = malloc(sqri(size) * sizeof(float));
    float scale = 1.0f / 255.0f;
    for (int i = 0; i < sqri(size); i++) {
        ctx->density[i] = 1 - pixels[i] * scale;
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
