#pragma once

#include <stdint.h>

#ifndef GLAPI
typedef unsigned int GLuint;
typedef int GLint;
#endif

// BUFFER

typedef struct par_buffer_s par_buffer;

typedef enum {
    PAR_CPU,
    PAR_CPU_LZ4,
    PAR_GPU_ARRAY,
    PAR_GPU_ELEMENTS
} par_buffer_type;

typedef enum { PAR_READ, PAR_WRITE, PAR_MODIFY } par_buffer_mode;

par_buffer* par_buffer_alloc(int nbytes, par_buffer_type);
par_buffer* par_buffer_dup(par_buffer*, par_buffer_type);
void par_buffer_free(par_buffer*);
int par_buffer_length(par_buffer*);
GLuint par_buffer_gpu_handle(par_buffer*);
char* par_buffer_lock(par_buffer*, par_buffer_mode);
void par_buffer_unlock(par_buffer*);

par_buffer* par_buffer_from_file(const char* filepath);
par_buffer* par_buffer_from_asset(const char* filename);

// BUFVIEW

#define PAR_DECLARE_BUFVIEW(name, type)
#define PAR_DEFINE_BUFVIEW(name, type)

PAR_DECLARE_BUFVIEW(par_fp32, float);
PAR_DECLARE_BUFVIEW(par_u16, unsigned short);

typedef struct par_fp32_s par_fp32;
typedef struct par_u16_s par_u16;

par_fp32* par_fp32_wrap(par_buffer*);
par_buffer* par_fp32_get(par_fp32*);

par_fp32* par_fp32_alloc(int nfloats);
void par_fp32_free(par_fp32*);
int par_fp32_length(par_fp32*);
GLuint par_fp32_gpu_handle(par_fp32*);
float* par_fp32_lock(par_fp32*, par_buffer_mode);
void par_fp32_unlock(par_fp32*);

// TOKEN

typedef uint32_t par_token;
#define PAR_TOKEN_DECLARE(NAME, VAL) static par_token NAME;
#define PAR_TOKEN_DEFINE(NAME, VAL) NAME = par_token_from_string(VAL);
const char* par_token_to_string(par_token);
par_token par_token_from_string(const char*);

// SURFACE

typedef void (*par_surface)(float* domain, float* range);
par_surface par_surface_klein();

// MESH

void par_mesh_from_surface(par_surface, par_u16* indices, par_fp32* coords);

// SHADER

void par_shader_load_from_buffer(par_buffer*);
void par_shader_load_from_asset(const char* filename);
GLuint par_shader_attrib_get(par_token);
GLint par_shader_uniform_get(par_token);
void par_shader_bind(par_token);
void par_shader_free(par_token);

// ASSET

const char* par_asset_whereami();
