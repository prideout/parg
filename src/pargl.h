#pragma once

#if EMSCRIPTEN
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <emscripten.h>
#define PAR_HALF_FLOAT GL_HALF_FLOAT_OES
#define PARGL_STRING const GLchar* *
#else
#define PARGL_STRING const GLchar* const *
#if defined(__APPLE_CC__)
#include <OpenGL/gl3.h>
#else
#include <GL/glcorearb.h>
#endif
#endif

void glGenerateMipmap(GLenum target);

GLuint par_buffer_gpu_handle(par_buffer*);
GLuint par_shader_attrib_get(par_token);
GLint par_shader_uniform_get(par_token);

extern int _par_depthtest;
