#pragma once

#include <stdint.h>

#ifndef GLAPI
typedef unsigned int GLuint;
typedef int GLint;
#endif

// BUFFER

#define par_buffer_t(TYPE)  \
    struct {                \
        int size, reserved; \
        TYPE* ptr;          \
    }
#define par_buffer_f32 par_buffer_t(float)
#define par_buffer_u32 par_buffer_t(uint32_t)
#define par_buffer_u16 par_buffer_t(uint16_t)
#define par_buffer_u8 par_buffer_t(uint8_t)

par_buffer_f32* par_buffer_f32_create(int nfloats);
void par_buffer_f32_destroy(par_buffer_f32*);

par_buffer_u8 par_buffer_from_file(const char* filpath);

GLint pargl_create_buffer_u8(par_buffer_u8* );

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

void par_mesh_from_surface(
    par_surface, par_buffer_u16* indices, par_buffer_f32* coords);

// SHADER

GLint pal_shader_uniform(par_token);
void par_shader_bind(par_token);
