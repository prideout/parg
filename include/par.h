#pragma once

#include <stdint.h>
#include <vmath.h>

// ENUMS

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

// BUFFER

typedef struct par_buffer_s par_buffer;
par_buffer* par_buffer_alloc(int nbytes, par_buffer_type);
par_buffer* par_buffer_dup(par_buffer*, par_buffer_type);
void par_buffer_free(par_buffer*);
int par_buffer_length(par_buffer*);
char* par_buffer_lock(par_buffer*, par_buffer_mode);
void par_buffer_unlock(par_buffer*);
void par_buffer_gpu_bind(par_buffer*);
int par_buffer_gpu_check(par_buffer*);
par_buffer* par_buffer_from_file(const char* filepath);
par_buffer* par_buffer_from_asset(const char* filename);

// TOKEN

typedef uint32_t par_token;
#define PAR_TOKEN_DECLARE(NAME, VAL) static par_token NAME;
#define PAR_TOKEN_DEFINE(NAME, VAL) NAME = par_token_from_string(VAL);
const char* par_token_to_string(par_token);
par_token par_token_from_string(const char*);

// MESH

typedef void (*par_surface)(float* domain, float* range);
par_surface par_mesh_surface_klein();
par_surface par_mesh_surface_sphere();
par_surface par_mesh_surface_torus();
par_surface par_mesh_surface_disk();
void par_mesh_from_surface(
    par_surface, par_buffer* indices, par_buffer* coords);

// SHADER

void par_shader_load_from_buffer(par_buffer*);
void par_shader_load_from_asset(const char* filename);
void par_shader_bind(par_token);
void par_shader_free(par_token);

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
void par_draw_triangles(int start, int count);
void par_draw_triangles_u16(int start, int count);

// ASSET

const char* par_asset_whereami();
