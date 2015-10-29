// BLUENOISE :: https://github.com/prideout/parg
// Generator for infinite 2D point sequences using Recursive Wang Tiles.
//
// In addition to this source code, you'll need to download the following
// tileset, which is about 2 MB.  This might seem onerous, but keep in mind
// that it enables the creation of an _infinite_ progressive sequence.
// For example, you could create 7.3 billion point samples if you'd like.
// And it's fast.
//
//     http://github.prideout.net/assets/bluenoise.bin
//
// The code herein is an implementation of the algorithm described in:
//
//     Recursive Wang Tiles for Real-Time Blue Noise
//     Johannes Kopf, Daniel Cohen-Or, Oliver Deussen, Dani Lischinski
//     ACM Transactions on Graphics 25, 3 (Proc. SIGGRAPH 2006)
//
// If you use this software for research purposes, please cite the above paper
// in any resulting publication.
//
// EXAMPLE
//
// Generate point samples whose density is guided by a 512x512 grayscale image:
//
//     int npoints;
//     float* points;
//     int maxpoints = 1e6;
//     float density = 30000;
//     par_bluenoise_context* ctx;
//     ctx = par_bluenoise_create("bluenoise.bin", 0, maxpoints);
//     par_bluenoise_density_from_gray(ctx, source_pixels, 512, 512, 1);
//     points = par_bluenoise_generate(ctx, density, &npoints);
//     ... Draw points here.  Each point is a three-tuple of (X Y RANK).
//     par_bluenoise_free(ctx);
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

// -----------------------------------------------------------------------------
// BEGIN PUBLIC API
// -----------------------------------------------------------------------------

// Encapsulates a tile set and an optional density function.
typedef struct par_bluenoise_context_s par_bluenoise_context;

// Creates a bluenoise context using the given tileset.  The first argument is
// the file path the bin file.  The second argument is the maximum number of
// points that you expect to ever be generated.
par_bluenoise_context* par_bluenoise_from_file(const char* path, int maxpts);

// Creates a bluenoise context using the given tileset.  The first and second
// arguments describe a memory buffer containing the contents of the bin file.
// The third argument is the maximum number of points that you expect to ever
// be generated.
par_bluenoise_context* par_bluenoise_from_buffer(
    const char* buffer, int nbytes, int maxpts);

// Sets up a scissoring rectangle using the given lower-left and upper-right
// coordinates.  By default the scissor encompasses [-0.5, -0.5] - [0.5, 0.5],
// which is the entire sampling domain for the two "generate" methods.
void par_bluenoise_set_viewport(
    par_bluenoise_context*, float left, float bottom, float right, float top);

// Frees all memory associated with the given bluenoise context.
void par_bluenoise_free(par_bluenoise_context* ctx);

// Copies a grayscale image into the bluenoise context to guide point density.
// Darker regions generate a higher number of points. The given bytes-per-pixel
// value is the stride between pixels.
void par_bluenoise_density_from_gray(par_bluenoise_context* ctx,
    const unsigned char* pixels, int width, int height, int bpp);

// Creates a binary mask to guide point density. The given bytes-per-pixel
// value is the stride between pixels, which must be 4 or less.
void par_bluenoise_density_from_color(par_bluenoise_context* ctx,
    const unsigned char* pixels, int width, int height, int bpp,
    unsigned int background_color, int invert);

// Generates samples using Recursive Wang Tiles.  This is really fast!
// The returned pointer is a list of three-tuples, where XY are in [-0.5, +0.5]
// and Z is a rank value that can be used to create a progressive ordering.
// The caller should not free the returned pointer.
float* par_bluenoise_generate(
    par_bluenoise_context* ctx, float density, int* npts);

// Generates an ordered sequence of tuples with the specified sequence length.
// This is slower than the other "generate" method because it uses a dumb
// backtracking method to determine a reasonable density value, and it
// automatically sorts the output by rank.  The dims argument must be 2 or more;
// it represents the desired stride (in floats) between consecutive verts in the
// returned data buffer.
float* par_bluenoise_generate_exact(
    par_bluenoise_context* ctx, int npts, int dims);

// Performs an in-place sort of 3-tuples, based on the 3rd component, then
// replaces the 3rd component with an index.
void par_bluenoise_sort_by_rank(float* pts, int npts);

// -----------------------------------------------------------------------------
// END PUBLIC API
// -----------------------------------------------------------------------------

#define clamp(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define sqr(a) (a * a)
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
    float left, bottom, right, top;
    int ntiles, nsubtiles, nsubdivs;
    int npoints;
    int maxpoints;
    int density_width;
    int density_height;
    float* density;
    float mag;
};

static float sample_density(par_bluenoise_context* ctx, float x, float y)
{
    float* density = ctx->density;
    if (!density) {
        return 1;
    }
    int width = ctx->density_width;
    int height = ctx->density_height;
    y = 1 - y;
    x -= 0.5;
    y -= 0.5;
    float tx = x * maxi(width, height);
    float ty = y * maxi(width, height);
    x += 0.5;
    y += 0.5;
    tx += width / 2;
    ty += height / 2;
    int ix = clamp((int) tx, 0, width - 2);
    int iy = clamp((int) ty, 0, height - 2);
    return density[iy * width + ix];
}

static void recurse_tile(
    par_bluenoise_context* ctx, par_tile* tile, float x, float y, int level)
{
    float left = ctx->left, right = ctx->right;
    float top = ctx->top, bottom = ctx->bottom;
    float mag = ctx->mag;
    float tileSize = 1.f / powf(ctx->nsubtiles, level);
    if (x + tileSize < left || x > right || y + tileSize < bottom || y > top) {
        return;
    }
    float depth = powf(ctx->nsubtiles, 2 * level);
    float threshold = mag / depth * ctx->global_density - tile->npoints;
    int ntests = mini(tile->nsubpts, threshold);
    float factor = 1.f / mag * depth / ctx->global_density;
    for (int i = 0; i < ntests; i++) {
        float px = x + tile->subpts[i].x * tileSize;
        float py = y + tile->subpts[i].y * tileSize;
        if (px < left || px > right || py < bottom || py > top) {
            continue;
        }
        if (sample_density(ctx, px, py) < (i + tile->npoints) * factor) {
            continue;
        }
        ctx->points[ctx->npoints].x = px - 0.5;
        ctx->points[ctx->npoints].y = py - 0.5;
        ctx->points[ctx->npoints].rank = (level + 1) + i * factor;
        ctx->npoints++;
        if (ctx->npoints >= ctx->maxpoints) {
            return;
        }
    }
    const float scale = tileSize / ctx->nsubtiles;
    if (threshold <= tile->nsubpts) {
        return;
    }
    level++;
    for (int ty = 0; ty < ctx->nsubtiles; ty++) {
        for (int tx = 0; tx < ctx->nsubtiles; tx++) {
            int tileIndex = tile->subdivs[0][ty * ctx->nsubtiles + tx];
            par_tile* subtile = &ctx->tiles[tileIndex];
            recurse_tile(ctx, subtile, x + tx * scale, y + ty * scale, level);
        }
    }
}

void par_bluenoise_set_viewport(par_bluenoise_context* ctx, float left,
    float bottom, float right, float top)
{
    // Transform [-.5, +.5] to [0, 1]
    left = ctx->left = left + 0.5;
    right = ctx->right = right + 0.5;
    bottom = ctx->bottom = bottom + 0.5;
    top = ctx->top = top + 0.5;

    // Determine magnification factor BEFORE clamping.
    ctx->mag = powf(top - bottom, -2);

    // The density function is only sampled in [0, +1].
    ctx->left = clamp(left, 0, 1);
    ctx->right = clamp(right, 0, 1);
    ctx->bottom = clamp(bottom, 0, 1);
    ctx->top = clamp(top, 0, 1);
}

float* par_bluenoise_generate(
    par_bluenoise_context* ctx, float density, int* npts)
{
    ctx->global_density = density;
    ctx->npoints = 0;
    float left = ctx->left;
    float right = ctx->right;
    float bottom = ctx->bottom;
    float top = ctx->top;
    float mag = ctx->mag;

    int ntests = mini(ctx->tiles[0].npoints, mag * ctx->global_density);
    float factor = 1.f / mag / ctx->global_density;
    for (int i = 0; i < ntests; i++) {
        float px = ctx->tiles[0].points[i].x;
        float py = ctx->tiles[0].points[i].y;
        if (px < left || px > right || py < bottom || py > top) {
            continue;
        }
        if (sample_density(ctx, px, py) < (i + 1) * factor) {
            continue;
        }
        ctx->points[ctx->npoints].x = px - 0.5;
        ctx->points[ctx->npoints].y = py - 0.5;
        ctx->points[ctx->npoints].rank = i * factor;
        ctx->npoints++;
        if (ctx->npoints >= ctx->maxpoints) {
            break;
        }
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

static par_bluenoise_context* par_bluenoise_create(
    const char* filepath, int nbytes, int maxpts)
{
    par_bluenoise_context* ctx = malloc(sizeof(par_bluenoise_context));
    ctx->maxpoints = maxpts;
    ctx->points = malloc(maxpts * sizeof(par_vec3));
    ctx->density = 0;
    par_bluenoise_set_viewport(ctx, -.5, -.5, .5, .5);

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
            int* subdiv = malloc(sizeof(int) * sqr(nsubtiles));
            for (int k = 0; k < sqr(nsubtiles); k++) {
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

par_bluenoise_context* par_bluenoise_from_file(const char* path, int maxpts)
{
    return par_bluenoise_create(path, 0, maxpts);
}

par_bluenoise_context* par_bluenoise_from_buffer(
    const char* buffer, int nbytes, int maxpts)
{
    return par_bluenoise_create(buffer, nbytes, maxpts);
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
    if (bpp > 1) {
        mask |= 0x0000ff00u;
    }
    if (bpp > 2) {
        mask |= 0x00ff0000u;
    }
    if (bpp > 3) {
        mask |= 0xff000000u;
    }
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

int cmp(const void* a, const void* b)
{
    const par_vec3* v1 = a;
    const par_vec3* v2 = b;
    if (v1->rank < v2->rank) {
        return -1;
    }
    if (v1->rank > v2->rank) {
        return 1;
    }
    return 0;
}

void par_bluenoise_sort_by_rank(float* floats, int npts)
{
    par_vec3* vecs = (par_vec3*) floats;
    qsort(vecs, npts, sizeof(vecs[0]), cmp);
    for (int i = 0; i < npts; i++) {
        vecs[i].rank = i;
    }
}

float* par_bluenoise_generate_exact(
    par_bluenoise_context* ctx, int npts, int stride)
{
    assert(stride >= 2);
    int maxpoints = npts * 2;
    if (ctx->maxpoints < maxpoints) {
        free(ctx->points);
        ctx->maxpoints = maxpoints;
        ctx->points = malloc(maxpoints * sizeof(par_vec3));
    }
    int ngenerated = 0;
    int nprevious = 0;
    int ndesired = npts;
    float density = 2048;
    while (ngenerated < ndesired) {
        par_bluenoise_generate(ctx, density, &ngenerated);

        // Might be paranoid, but break if something fishy is going on:
        if (ngenerated == nprevious) {
            return 0;
        }

        // Perform crazy heuristic to approach a nice density:
        if (ndesired / ngenerated >= 2) {
            density *= 2;
        } else {
            density += density / 10;
        }

        nprevious = ngenerated;
    }
    par_bluenoise_sort_by_rank(&ctx->points->x, ngenerated);
    if (stride != 3) {
        int nbytes = sizeof(float) * stride * ndesired;
        float* pts = malloc(nbytes);
        float* dst = pts;
        const float* src = &ctx->points->x;
        for (int i = 0; i < ndesired; i++, src++) {
            *dst++ = *src++;
            *dst++ = *src++;
            if (stride > 3) {
                *dst++ = *src;
                dst += stride - 3;
            }
        }
        memcpy(ctx->points, pts, nbytes);
        free(pts);
    }
    return &ctx->points->x;
}
