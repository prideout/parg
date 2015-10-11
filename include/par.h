#pragma once

#include <stdint.h>
#include <vmath.h>
#include <dmath.h>

// ENUMS & CONSTANTS

#define PAR_PI 3.14159265359
#define PAR_TWOPI 6.28318530718
#define PAR_BYTE 0x1400
#define PAR_UNSIGNED_BYTE 0x1401
#define PAR_SHORT 0x1402
#define PAR_UNSIGNED_SHORT 0x1403
#define PAR_INT 0x1404
#define PAR_UNSIGNED_INT 0x1405
#define PAR_FLOAT 0x1406
#define PAR_DOUBLE 0x140A

typedef unsigned int par_data_type;

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
par_buffer* par_buffer_alloc(int nbytes, par_buffer_type);
par_buffer* par_buffer_dup(par_buffer*, par_buffer_type);
void par_buffer_free(par_buffer*);
int par_buffer_length(par_buffer*);
void* par_buffer_lock(par_buffer*, par_buffer_mode);
void par_buffer_unlock(par_buffer*);
void par_buffer_gpu_bind(par_buffer*);
int par_buffer_gpu_check(par_buffer*);
par_buffer* par_buffer_from_asset(par_token id);

// MESH

typedef struct par_mesh_s par_mesh;
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
par_texture* par_texture_from_asset(par_token id);
void par_texture_bind(par_texture*, int stage);
void par_texture_info(par_texture*, int* width, int* height);
void par_texture_free(par_texture*);
void par_texture_fliprows(void* data, int rowsize, int nrows);

// UNIFORM

void par_uniform1f(par_token tok, float val);
void par_uniform4f(par_token, const Vector4* val);
void par_uniform_matrix4f(par_token, const Matrix4* val);
void par_uniform_matrix3f(par_token, const Matrix3* val);

// STATE

void par_state_clearcolor(Vector4 color);
void par_state_cullfaces(int enabled);
void par_state_depthtest(int enabled);

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

// ZCAM

void par_zcam_init(float world_width, float world_height, float fovy);
void par_zcam_tick(float window_aspect, float seconds);
void par_zcam_grab_begin(float winx, float winy);
void par_zcam_grab_update(float winx, float winy, float scrolldelta);
void par_zcam_grab_end();
Point3 par_zcam_matrices(Matrix4* proj, Matrix4* view);
DPoint3 par_zcam_dmatrices(DMatrix4* proj, DMatrix4* view);
 