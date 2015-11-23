#pragma once

#include <stdint.h>
#include <vmath.h>
#include <dmath.h>

#ifdef __cplusplus
extern "C" {
#endif

// ENUMS & CONSTANTS

#define PAR_PI 3.14159265359
#define PAR_TWOPI 6.28318530718
#define PAR_BYTE 0x1400
#define PAR_UBYTE 0x1401
#define PAR_SHORT 0x1402
#define PAR_USHORT 0x1403
#define PAR_INT 0x1404
#define PAR_UINT 0x1405
#define PAR_FLOAT 0x1406
#define PAR_DOUBLE 0x140A

typedef unsigned int par_data_type;
typedef unsigned char par_byte;

typedef enum {
    PAR_CPU,
    PAR_CPU_LZ4,
    PAR_GPU_ARRAY,
    PAR_GPU_ELEMENTS
} par_buffer_type;

typedef enum { PAR_READ, PAR_WRITE, PAR_MODIFY } par_buffer_mode;

// TOKEN

typedef uint32_t par_token;
#define PAR_TOKEN_DECLARE(NAME, VAL) static par_token NAME;
#define PAR_TOKEN_DEFINE(NAME, VAL) NAME = par_token_from_string(VAL);
const char* par_token_to_string(par_token);
par_token par_token_from_string(const char*);

// ASSET

#define PAR_ASSET_TABLE(NAME, VAL) \
    PAR_TOKEN_DEFINE(NAME, VAL);   \
    par_asset_preload(NAME);
#define PAR_ASSET_LIST(VAL) par_asset_preload(par_token_from_string(VAL));
void par_asset_preload(par_token id);

// BUFFER

typedef struct par_buffer_s par_buffer;
par_buffer* par_buffer_create(void* src, int nbytes, par_buffer_type memtype);
par_buffer* par_buffer_alloc(int nbytes, par_buffer_type);
par_buffer* par_buffer_dup(par_buffer*, par_buffer_type);
void par_buffer_free(par_buffer*);
int par_buffer_length(par_buffer*);
void* par_buffer_lock(par_buffer*, par_buffer_mode);
void par_buffer_unlock(par_buffer*);
void par_buffer_gpu_bind(par_buffer*);
int par_buffer_gpu_check(par_buffer*);
par_buffer* par_buffer_from_asset(par_token id);
par_buffer* par_buffer_slurp_asset(par_token id, void** ptr);
void par_buffer_to_file(par_buffer*, const char* filepath);

// MESH

typedef struct par_mesh_s par_mesh;
par_mesh* par_mesh_create(float* pts, int npts, uint16_t* tris, int ntris);
par_mesh* par_mesh_knot(int cols, int rows, float major, float minor);
par_mesh* par_mesh_torus(int cols, int rows, float major, float minor);
par_mesh* par_mesh_rectangle(float width, float height);
par_mesh* par_mesh_sierpinski(float width, int depth);
void par_mesh_free(par_mesh* m);
par_buffer* par_mesh_coord(par_mesh* m);
par_buffer* par_mesh_uv(par_mesh* m);
par_buffer* par_mesh_norml(par_mesh* m);
par_buffer* par_mesh_index(par_mesh* m);
int par_mesh_ntriangles(par_mesh* m);

// SHADER

void par_shader_load_from_buffer(par_buffer*);
void par_shader_load_from_asset(par_token id);
void par_shader_bind(par_token);
void par_shader_free(par_token);

// TEXTURE

typedef struct par_texture_s par_texture;
par_texture* par_texture_from_buffer(par_buffer* rgba);
par_texture* par_texture_from_asset(par_token id);
par_texture* par_texture_from_fp32(
    par_buffer* buf, int width, int height, int ncomps, int bytoffset);
par_texture* par_texture_from_u8(
    par_buffer* buf, int width, int height, int ncomps, int byteoffset);
void par_texture_bind(par_texture*, int stage);
void par_texture_info(par_texture*, int* width, int* height);
void par_texture_free(par_texture*);
void par_texture_fliprows(void* data, int rowsize, int nrows);

// UNIFORM

void par_uniform1f(par_token tok, float val);
void par_uniform3f(par_token, const Vector3* val);
void par_uniform4f(par_token, const Vector4* val);
void par_uniform_point(par_token, const Point3* val);
void par_uniform_matrix4f(par_token, const Matrix4* val);
void par_uniform_matrix3f(par_token, const Matrix3* val);

// STATE

void par_state_clearcolor(Vector4 color);
void par_state_cullfaces(int enabled);
void par_state_depthtest(int enabled);
void par_state_blending(int enabled);

// VARRAY

void par_varray_disable(par_token attr);
void par_varray_bind(par_buffer*);
void par_varray_enable(par_buffer*, par_token attr, int ncomps,
    par_data_type type, int stride, int offset);

// DRAW

void par_draw_clear();
void par_draw_one_quad();
void par_draw_triangles(int start, int count);
void par_draw_triangles_u16(int start, int count);
void par_draw_lines(int nsegments);
void par_draw_points(int npoints);

// ZCAM

void par_zcam_init(float world_width, float world_height, float fovy);
void par_zcam_tick(float window_aspect, float seconds);
float par_zcam_get_magnification();
void par_zcam_get_viewport(float* lbrt);
void par_zcam_grab_begin(float winx, float winy);
void par_zcam_grab_update(float winx, float winy, float scrolldelta);
void par_zcam_grab_end();
Point3 par_zcam_matrices(Matrix4* proj, Matrix4* view);
DPoint3 par_zcam_dmatrices(DMatrix4* proj, DMatrix4* view);
void par_zcam_highprec(Matrix4* vp, Point3* eyepos_lo, Point3* eyepos_hi);
int par_zcam_has_moved();

// EASYCURL

void par_easycurl_init(uint32_t flags);
int par_easycurl_to_memory(const char* url, par_byte** data, int* nbytes);
int par_easycurl_to_file(const char* srcurl, const char* dstpath);

// FILECACHE

void par_filecache_init(const char* path, int maxsize);
int par_filecache_load(const char* name, par_byte** payload, int* payloadsize,
    par_byte* header, int headersize);
void par_filecache_save(const char* name, par_byte* payload, int payloadsize,
    par_byte* header, int headersize);
void par_filecache_evict_all();

// BLUENOISE

typedef struct par_bluenoise_context_s par_bluenoise_context;
par_bluenoise_context* par_bluenoise_from_file(const char* path, int maxpts);
par_bluenoise_context* par_bluenoise_from_buffer(
    const char* buffer, int nbytes, int maxpts);
void par_bluenoise_set_viewport(
    par_bluenoise_context*, float left, float bottom, float right, float top);
void par_bluenoise_set_window(par_bluenoise_context*, int width, int height);
void par_bluenoise_free(par_bluenoise_context* ctx);
void par_bluenoise_density_from_gray(par_bluenoise_context* ctx,
    const unsigned char* pixels, int width, int height, int bpp);
void par_bluenoise_density_from_color(par_bluenoise_context* ctx,
    const unsigned char* pixels, int width, int height, int bpp,
    unsigned int background_color, int invert);
float* par_bluenoise_generate(
    par_bluenoise_context* ctx, float density, int* npts);
float* par_bluenoise_generate_exact(
    par_bluenoise_context* ctx, int npts, int stride);
void par_bluenoise_sort_by_rank(float* floats, int npts);

// FRAMEBUFFER

typedef struct par_framebuffer_s par_framebuffer;
par_framebuffer* par_framebuffer_create(int width, int height);
void par_framebuffer_free(par_framebuffer*);

// MSQUARES

typedef struct par_msquares_meshlist_s par_msquares_meshlist;
typedef int (*par_msquares_inside_fn)(int, void*);
typedef float (*par_msquares_height_fn)(float, float, void*);

typedef struct {
    float* points;        // pointer to XY (or XYZ) vertex coordinates
    int npoints;          // number of vertex coordinates
    uint16_t* triangles;  // pointer to 3-tuples of vertex indices
    int ntriangles;       // number of 3-tuples
    int dim;              // number of floats per point (either 2 or 3)
} par_msquares_mesh;

#define PAR_MSQUARES_INVERT (1 << 0)
#define PAR_MSQUARES_DUAL (1 << 1)
#define PAR_MSQUARES_WELD (1 << 2)
#define PAR_MSQUARES_CONNECT (1 << 3)
#define PAR_MSQUARES_SIMPLIFY (1 << 4)
#define PAR_MSQUARES_HEIGHTS (1 << 5)

par_msquares_meshlist* par_msquares_from_grayscale(float const* data, int width,
    int height, int cellsize, float threshold, int flags);
par_msquares_meshlist* par_msquares_from_levels(float const* data, int width,
    int height, int cellsize, float const* thresholds, int nthresholds,
    int flags);
par_msquares_meshlist* par_msquares_from_color(par_byte const* data, int width,
    int height, int cellsize, uint32_t color, int bpp, int flags);
par_msquares_meshlist* par_msquares_from_colors(par_byte const* data, int width,
    int height, int cellsize, uint32_t const* colors, int ncolors, int bpp,
    int flags);
par_msquares_meshlist* par_msquares_from_function(int width, int height,
    int cellsize, int flags, void* context, par_msquares_inside_fn insidefn,
    par_msquares_inside_fn heightfn);
par_msquares_mesh* par_msquares_get_mesh(par_msquares_meshlist*, int n);
int par_msquares_get_count(par_msquares_meshlist*);
void par_msquares_free(par_msquares_meshlist*);

#ifdef __cplusplus
}
#endif
