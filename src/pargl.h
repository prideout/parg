#pragma once

#if EMSCRIPTEN
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <emscripten.h>
#define PARG_HALF_FLOAT GL_HALF_FLOAT_OES
#define PARGL_STRING const GLchar* *

void pargVertexAttribDivisor(GLuint index, GLuint divisor);

void pargDrawElementsInstanced(GLenum mode,
 	GLsizei count,
 	GLenum type,
 	const void * indices,
 	GLsizei primcount);

#else

#define PARGL_STRING const GLchar* const *
#if defined(__APPLE_CC__)
#include <OpenGL/gl3.h>
#else
#include <GL/glcorearb.h>
#endif

#define pargVertexAttribDivisor(a, b) \
    glVertexAttribDivisor(a, b)

#define pargDrawElementsInstanced(a, b, c, d, e) \
    glDrawElementsInstanced(a, b, c, d, e)

#endif

void glGenerateMipmap(GLenum target);

GLuint parg_buffer_gpu_handle(parg_buffer*);
GLuint parg_shader_attrib_get(parg_token);
GLint parg_shader_uniform_get(parg_token);

extern int _parg_depthtest;
