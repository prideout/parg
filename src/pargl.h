#pragma once

#if EMSCRIPTEN
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <emscripten.h>
#define PARG_HALF_FLOAT GL_HALF_FLOAT_OES
#define PARGL_STRING const GLchar* *


/*
    emscripten::val GLctx = emscripten::val::global("GLctx");
    assert(GLctx.as<bool>() && "Can't find GLctx");
    emscripten::val glext = GLctx.call<emscripten::val>("getExtension",
        string("ANGLE_instanced_arrays"));
    if (glext.as<bool>()) {
        this->instancing_extension = new emscripten::val(glext);
    }
*/

    #define pargVertexAttribDivisor(a, b) \
        glext->call<void>("vertexAttribDivisorANGLE", a, b)

    #define pargDrawElementsInstanced(a, b, c, d, e) \
        glext->call<void>("drawElementsInstancedANGLE", a, b, c, (int) d, (int) e)

    #define pargDrawArraysInstanced(a, b, c, d) \
        glext->call<void>("drawArraysInstancedANGLE", a, b, c, d)

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

#define pargDrawArraysInstanced(a, b, c, d) \
    glDrawArraysInstanced(a, b, c, d)

#endif

void glGenerateMipmap(GLenum target);

GLuint parg_buffer_gpu_handle(parg_buffer*);
GLuint parg_shader_attrib_get(parg_token);
GLint parg_shader_uniform_get(parg_token);

extern int _parg_depthtest;
