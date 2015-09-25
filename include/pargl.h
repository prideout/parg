#pragma once

#include "par.h"

#ifndef GLAPI
typedef int GLint;
typedef unsigned int GLuint;
#endif

GLint pargl_create_buffer_u8(par_buffer_u8* );
GLint pargl_get_uniform(par_token);
void pargl_bind_shader(par_token);
