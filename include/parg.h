#pragma once

#include <stdint.h>
#include <vmath.h>
#include <dmath.h>

#ifdef __cplusplus
extern "C" {
#endif

// MACROS

#define PARG_CLAMP(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define PARG_SQR(a) (a * a)
#define PARG_MIN(a, b) ((a < b) ? a : b)
#define PARG_MAX(a, b) ((a > b) ? a : b)
#define PARG_SWAP(T, A, B) \
    {                      \
        T tmp = B;         \
        B = A;             \
        A = tmp;           \
    }

// ENUMS & CONSTANTS

#define PARG_PI 3.14159265359
#define PARG_TWOPI 6.28318530718
#define PARG_BYTE 0x1400
#define PARG_UBYTE 0x1401
#define PARG_SHORT 0x1402
#define PARG_USHORT 0x1403
#define PARG_INT 0x1404
#define PARG_UINT 0x1405
#define PARG_FLOAT 0x1406
#define PARG_DOUBLE 0x140A

#define PARG_FBO_FLOAT (1 << 0)
#define PARG_FBO_ALPHA (1 << 1)
#define PARG_FBO_HALF (1 << 2)
#define PARG_FBO_LINEAR (1 << 3)
#define PARG_FBO_DEPTH (1 << 3)

typedef unsigned int parg_data_type;
typedef unsigned char parg_byte;

typedef enum {
    PARG_CPU,
    PARG_CPU_LZ4,
    PARG_GPU_ARRAY,
    PARG_GPU_ELEMENTS
} parg_buffer_type;

typedef enum { PARG_READ, PARG_WRITE, PARG_MODIFY } parg_buffer_mode;

// TOKENS

typedef uint32_t parg_token;
#define PARG_TOKEN_DECLARE(NAME, VAL) static parg_token NAME;
#define PARG_TOKEN_DEFINE(NAME, VAL) NAME = parg_token_from_string(VAL);
const char* parg_token_to_string(parg_token);
parg_token parg_token_from_string(const char*);

// ASSETS

#define PARG_ASSET_TABLE(NAME, VAL) \
    PARG_TOKEN_DEFINE(NAME, VAL);   \
    parg_asset_preload(NAME);
#define PARG_ASSET_LIST(VAL) parg_asset_preload(parg_token_from_string(VAL));
void parg_asset_preload(parg_token id);

// BUFFERS

typedef struct parg_buffer_s parg_buffer;
parg_buffer* parg_buffer_create(
    void* src, int nbytes, parg_buffer_type memtype);
parg_buffer* parg_buffer_alloc(int nbytes, parg_buffer_type);
parg_buffer* parg_buffer_dup(parg_buffer*, parg_buffer_type);
void parg_buffer_free(parg_buffer*);
int parg_buffer_length(parg_buffer*);
void* parg_buffer_lock(parg_buffer*, parg_buffer_mode);
void parg_buffer_unlock(parg_buffer*);
void parg_buffer_gpu_bind(parg_buffer*);
int parg_buffer_gpu_check(parg_buffer*);
parg_buffer* parg_buffer_from_asset(parg_token id);
parg_buffer* parg_buffer_slurp_asset(parg_token id, void** ptr);
void parg_buffer_to_file(parg_buffer*, const char* filepath);
parg_buffer* parg_buffer_to_gpu(parg_buffer* buf, parg_buffer_type memtype);
parg_buffer* parg_buffer_from_file(const char* filepath);

// AXIS-ALIGNED RECTANGLE

typedef struct {
    float left, bottom, right, top;
} parg_aar;

typedef struct {
    int x, y, z;
} parg_tilename;

typedef struct {
    parg_tilename mintile;
    parg_tilename maxtile;
} parg_tilerange;

float parg_aar_to_tilerange(parg_aar, Vector2 mapsize, parg_tilerange* range);
parg_aar parg_aar_from_tilename(parg_tilename tile, Vector2 mapsize);
parg_aar parg_aar_from_tilerange(parg_tilerange range, Vector2 mapsize);
parg_aar parg_aar_encompass(parg_aar a, parg_aar b);
parg_aar parg_aar_stretch_to_square(parg_aar rect);
float parg_aar_height(parg_aar rect);
float parg_aar_width(parg_aar rect);

// MESHES

typedef struct parg_mesh_s parg_mesh;
struct par_shapes_mesh_s;

parg_mesh* parg_mesh_create(float* pts, int npts, uint16_t* tris, int ntris);
parg_mesh* parg_mesh_from_shape(struct par_shapes_mesh_s const* src);
parg_mesh* parg_mesh_from_asset(parg_token id);
parg_mesh* parg_mesh_from_file(const char* filepath);
parg_mesh* parg_mesh_knot(int cols, int rows, float major, float minor);
parg_mesh* parg_mesh_torus(int cols, int rows, float major, float minor);
parg_mesh* parg_mesh_rectangle(float width, float height);
parg_mesh* parg_mesh_aar(parg_aar rect);
parg_mesh* parg_mesh_sierpinski(float width, int depth);
void parg_mesh_free(parg_mesh* m);
parg_buffer* parg_mesh_coord(parg_mesh* m);
parg_buffer* parg_mesh_uv(parg_mesh* m);
parg_buffer* parg_mesh_norml(parg_mesh* m);
parg_buffer* parg_mesh_index(parg_mesh* m);
int parg_mesh_ntriangles(parg_mesh* m);
void parg_mesh_compute_normals(parg_mesh* m);
void parg_mesh_send_to_gpu(parg_mesh* m);

// SHADERS

void parg_shader_load_from_buffer(parg_buffer*);
void parg_shader_load_from_asset(parg_token id);
void parg_shader_bind(parg_token);
void parg_shader_free(parg_token);

// TEXTURES

typedef struct parg_texture_s parg_texture;
parg_texture* parg_texture_from_buffer(parg_buffer* rgba);
parg_texture* parg_texture_from_asset(parg_token id);
parg_texture* parg_texture_from_asset_linear(parg_token id);
parg_texture* parg_texture_from_fp32(
    parg_buffer* buf, int width, int height, int ncomps, int bytoffset);
parg_texture* parg_texture_from_u8(
    parg_buffer* buf, int width, int height, int ncomps, int byteoffset);
void parg_texture_bind(parg_texture*, int stage);
void parg_texture_info(parg_texture*, int* width, int* height);
void parg_texture_free(parg_texture*);
void parg_texture_fliprows(void* data, int rowsize, int nrows);

// UNIFORMS

void parg_uniform1i(parg_token tok, int val);
void parg_uniform1f(parg_token tok, float val);
void parg_uniform2f(parg_token tok, float x, float y);
void parg_uniform3f(parg_token, const Vector3* val);
void parg_uniform4f(parg_token, const Vector4* val);
void parg_uniform_point(parg_token, const Point3* val);
void parg_uniform_matrix4f(parg_token, const Matrix4* val);
void parg_uniform_matrix3f(parg_token, const Matrix3* val);

// GL STATE MACHINE

void parg_state_clearcolor(Vector4 color);
void parg_state_cullfaces(int enabled);
void parg_state_depthtest(int enabled);
void parg_state_blending(int enabled);

// VERTEX ARRAYS

void parg_varray_disable(parg_token attr);
void parg_varray_bind(parg_buffer*);
void parg_varray_enable(parg_buffer*, parg_token attr, int ncomps,
    parg_data_type type, int stride, int offset);

// DRAW CALLS

void parg_draw_clear();
void parg_draw_one_quad();
void parg_draw_triangles(int start, int count);
void parg_draw_triangles_u16(int start, int count);
void parg_draw_wireframe_triangles_u16(int start, int count);
void parg_draw_lines(int nsegments);
void parg_draw_points(int npoints);

// MAP CAMERA

void parg_zcam_init(float world_width, float world_height, float fovy);
void parg_zcam_tick(float window_aspect, float seconds);
DPoint3 parg_zcam_to_world(float winx, float winy);
float parg_zcam_get_magnification();
void parg_zcam_get_viewport(float* lbrt);
parg_aar parg_zcam_get_rectangle();
void parg_zcam_grab_begin(float winx, float winy);
void parg_zcam_grab_update(float winx, float winy, float scrolldelta);
void parg_zcam_grab_end();
Point3 parg_zcam_matrices(Matrix4* proj, Matrix4* view);
DPoint3 parg_zcam_dmatrices(DMatrix4* proj, DMatrix4* view);
void parg_zcam_highprec(Matrix4* vp, Point3* eyepos_lo, Point3* eyepos_hi);
int parg_zcam_has_moved();
void parg_zcam_touch();
void parg_zcam_set_position(double x, double y, double z);

// OFFSCREEN FRAMEBUFFER

typedef struct parg_framebuffer_s parg_framebuffer;
parg_framebuffer* parg_framebuffer_create_empty(
    int width, int height, int flags);
parg_framebuffer* parg_framebuffer_create(
    int width, int height, void* src, int nbytes, int flags);
void parg_framebuffer_bindtex(parg_framebuffer*, int stage);
void parg_framebuffer_bindfbo(parg_framebuffer*, int mrt_index);
void parg_framebuffer_pushfbo(parg_framebuffer*, int mrt_index);
void parg_framebuffer_popfbo();
void parg_framebuffer_free(parg_framebuffer*);
void parg_framebuffer_swap(parg_framebuffer*, parg_framebuffer*);

#ifdef __cplusplus
}
#endif
