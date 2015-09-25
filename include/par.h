#pragma once

#include <stdint.h>

typedef uint32_t par_token;

typedef void (*par_surface)(float* domain, float* range);

#define par_buffer_t(TYPE)  \
    struct {                \
        int size, reserved; \
        TYPE* ptr;          \
    }
#define par_buffer_f32 par_buffer_t(float)
#define par_buffer_u32 par_buffer_t(uint32_t)
#define par_buffer_u16 par_buffer_t(uint16_t)
#define par_buffer_u8 par_buffer_t(uint8_t)

const char* par_token_to_string(par_token);
par_token par_token_from_string(const char*);

par_surface par_surface_klein();

void par_mesh_from_surface(
    par_surface, par_buffer_u16* indices, par_buffer_f32* coords);

par_buffer_f32* par_buffer_f32_create(int nfloats);
void par_buffer_f32_destroy(par_buffer_f32*);

par_buffer_u8 par_buffer_from_file(const char* filpath);
