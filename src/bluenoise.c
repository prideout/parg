// The MIT License
// Copyright (c) 2015 Philip Rideout and Johannes Kopf
// https://github.com/prideout/parg
//
// Implementation of the algorithms described in:
//
// Recursive Wang Tiles for Real-Time Blue Noise
// Johannes Kopf, Daniel Cohen-Or, Oliver Deussen, Dani Lischinski
// In ACM Transactions on Graphics 25, 3 (Proc. SIGGRAPH 2006)
//
// If you use this software for research purposes, please cite
// the aforementioned paper in any resulting publication.
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
#include <math.h>

// Encapsulates a tile set and an optional density function.
typedef struct par_bluenoise_context_s par_bluenoise_context;

// Create a bluenoise context using the given tile set.  The first argument is
// a filepath if the second argument is 0.  If the second argument is a non-zero
// byte count, then the first argument is a pointer to an in-memory tile set.
par_bluenoise_context* par_bluenoise_create(
    const char* file_or_data, int data_length);

// Free all memory associated with the given bluenoise context.
void par_bluenoise_free(par_bluenoise_context* ctx);

// Copy a grayscale one-byte-per-pixel image into the bluenoise context.
// Darker regions create a higher number of samples.
void par_bluenoise_set_density(
    par_bluenoise_context* ctx, const unsigned char* pixels, int size);

// Generate samples using the Recursive Wang Tiles algorithm.  This is fast!
// The returner pointer is a set of two-tuples in the [0,1] interval.
// The caller should not free the returned pointer.  The given xyz values
// control a square region within the density function.
float* par_bluenoise_generate(
    par_bluenoise_context* ctx, float x, float y, float z, int* npts);

#define MAX_POINTS 1024 * 1024

static int sqri(int a) { return a * a; };
static int mini(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
};
static int floori(float a) { return floor(a); };
static int clampi(int x, int min, int max)
{
    if (x < min)
        x = min;
    if (x > max)
        x = max;
    return x;
}

static int freadi(FILE* fIn)
{
    int iTemp;
    fread(&iTemp, sizeof(int), 1, fIn);
    return iTemp;
}

static float freadf(FILE* fIn)
{
    float fTemp;
    fread(&fTemp, sizeof(float), 1, fIn);
    return fTemp;
}

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    int n, e, s, w;
    int numSubtiles, numSubdivs, numPoints, numSubPoints;
    int** subdivs;
    Vec2* points;
    Vec2* subPoints;
} Tile;

struct par_bluenoise_context_s {
    Vec2* points;
    Tile* tiles;
    float toneScale;
    float clipMinX, clipMaxX, clipMinY, clipMaxY;
    int numTiles, numSubtiles, numSubdivs;
    float vpos[3];
    int numPoints;
    int densTexSize;
    float* densTex;
};

static float sampleDensMap(par_bluenoise_context* ctx, float x, float y)
{
    int densTexSize = ctx->densTexSize;
    float* densTex = ctx->densTex;
    float tx = x * densTexSize;
    float ty = y * densTexSize;
    int ix = clampi(floori(tx), 0, densTexSize - 2);
    int iy = clampi(floori(ty), 0, densTexSize - 2);
    tx -= ix;
    ty -= iy;
    float sample = (densTex[iy * densTexSize + ix] * (1 - tx) * (1 - ty) +
        densTex[iy * densTexSize + ix + 1] * tx * (1 - ty) +
        densTex[(iy + 1) * densTexSize + ix] * (1 - tx) * ty +
        densTex[(iy + 1) * densTexSize + ix + 1] * tx * ty);
    return sample;
}

static void recurseTile(
    par_bluenoise_context* ctx, Tile* tile, float x, float y, int level)
{
    float tileSize = 1.f / powf(ctx->numSubtiles, level);
    if ((x + tileSize < ctx->clipMinX) || (x > ctx->clipMaxX) ||
        (y + tileSize < ctx->clipMinY) || (y > ctx->clipMaxY)) {
        return;
    }
    int numTests = mini(tile->numSubPoints,
            powf(ctx->vpos[2], -2.f) / powf(ctx->numSubtiles, 2.f * level) *
            ctx->toneScale -
            tile->numPoints);
    float factor = 1.f / powf(ctx->vpos[2], -2.f) *
        powf(ctx->numSubtiles, 2.f * level) / ctx->toneScale;
    for (int i = 0; i < numTests; i++) {
        float px = x + tile->subPoints[i].x * tileSize,
            py = y + tile->subPoints[i].y * tileSize;
        if ((px < ctx->clipMinX) || (px > ctx->clipMaxX) ||
            (py < ctx->clipMinY) || (py > ctx->clipMaxY)) {
            continue;
        }
        if (sampleDensMap(ctx, px, py) < (i + tile->numPoints) * factor) {
            continue;
        }
        ctx->points[ctx->numPoints].x = px;
        ctx->points[ctx->numPoints].y = py;
        ctx->numPoints++;
    }
    const float scale = tileSize / ctx->numSubtiles;
    if (powf(ctx->vpos[2], -2.f) / powf(ctx->numSubtiles, 2.f * level) *
        ctx->toneScale -
        tile->numPoints >
        tile->numSubPoints) {
        for (int ty = 0; ty < ctx->numSubtiles; ty++) {
            for (int tx = 0; tx < ctx->numSubtiles; tx++) {
                int tileIndex = tile->subdivs[0][ty * ctx->numSubtiles + tx];
                Tile* subtile = &ctx->tiles[tileIndex];
                recurseTile(
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
    ctx->numPoints = 0;
    int numTests =
        mini(ctx->tiles[0].numPoints, powf(z, -2.f) * ctx->toneScale);
    float factor = 1.f / powf(z, -2) / ctx->toneScale;
    for (int i = 0; i < numTests; i++) {
        float px = ctx->tiles[0].points[i].x, py = ctx->tiles[0].points[i].y;
        if ((px < ctx->clipMinX) || (px > ctx->clipMaxX) ||
            (py < ctx->clipMinY) || (py > ctx->clipMaxY)) {
            continue;
        }
        if (sampleDensMap(ctx, px, py) < i * factor) {
            continue;
        }
        ctx->points[ctx->numPoints].x = px;
        ctx->points[ctx->numPoints].y = py;
        ctx->numPoints++;
    }
    recurseTile(ctx, &ctx->tiles[0], 0, 0, 0);
    *npts = ctx->numPoints;
    return &ctx->points->x;
}

par_bluenoise_context* par_bluenoise_create(const char* filepath, int nbytes)
{
    par_bluenoise_context* ctx = malloc(sizeof(par_bluenoise_context));
    ctx->points = malloc(MAX_POINTS * sizeof(Vec2));
    ctx->toneScale = 200000;
    ctx->densTex = 0;
    FILE* fin = fopen(filepath, "rb");
    int numTiles = ctx->numTiles = freadi(fin);
    int numSubtiles = ctx->numSubtiles = freadi(fin);
    int numSubdivs = ctx->numSubdivs = freadi(fin);
    Tile* tiles = ctx->tiles = malloc(sizeof(Tile) * numTiles);
    for (int i = 0; i < numTiles; i++) {
        tiles[i].n = freadi(fin);
        tiles[i].e = freadi(fin);
        tiles[i].s = freadi(fin);
        tiles[i].w = freadi(fin);
        tiles[i].subdivs = malloc(sizeof(int) * numSubdivs);
        for (int j = 0; j < numSubdivs; j++) {
            int* subdiv = malloc(sizeof(int) * sqri(numSubtiles));
            for (int k = 0; k < sqri(numSubtiles); k++) {
                subdiv[k] = freadi(fin);
            }
            tiles[i].subdivs[j] = subdiv;
        }
        tiles[i].numPoints = freadi(fin);
        tiles[i].points = malloc(sizeof(Vec2) * tiles[i].numPoints);
        for (int j = 0; j < tiles[i].numPoints; j++) {
            tiles[i].points[j].x = freadf(fin);
            tiles[i].points[j].y = freadf(fin);
        }
        tiles[i].numSubPoints = freadi(fin);
        tiles[i].subPoints = malloc(sizeof(Vec2) * tiles[i].numSubPoints);
        for (int j = 0; j < tiles[i].numSubPoints; j++) {
            tiles[i].subPoints[j].x = freadf(fin);
            tiles[i].subPoints[j].y = freadf(fin);
        }
    }
    fclose(fin);
    return ctx;
}

void par_bluenoise_set_density(
    par_bluenoise_context* ctx, const unsigned char* pixels, int size)
{
    ctx->densTexSize = size;
    ctx->densTex = malloc(sqri(size) * sizeof(float));
    float scale = 1.0f / 255.0f;
    for (int i = 0; i < sqri(size); i++) {
        ctx->densTex[i] = 1 - pixels[i] * scale;
    }
}

void par_bluenoise_free(par_bluenoise_context* ctx)
{
    // TODO free files
    free(ctx->points);
    ctx->points = 0;
}
