#include <parg.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"
#include "pargl.h"

#if EMSCRIPTEN
#define HALF_FLOAT GL_HALF_FLOAT_OES
#elif defined(__APPLE__) && defined(__MACH__)
#define HALF_FLOAT GL_HALF_FLOAT
#endif

struct parg_framebuffer_s {
    int width;
    int height;
    GLuint tex;
    GLuint fbo;
    GLuint depth;
};

static GLint pushed_fbo = 0;
static GLint pushed_viewport[4];

parg_framebuffer* parg_framebuffer_create_empty(
    int width, int height, int flags)
{
    GLuint tex, fbo, depth = 0;
    GLenum filter = (flags & PARG_FBO_LINEAR) ? GL_LINEAR : GL_NEAREST;
    GLenum format = (flags & PARG_FBO_ALPHA) ? GL_RGBA : GL_RGB;
    GLenum type = GL_UNSIGNED_BYTE;
    if (flags & PARG_FBO_FLOAT) {
        type = GL_FLOAT;
    } else if (flags & PARG_FBO_HALF) {
        type = HALF_FLOAT;
    }

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, 0);

    GLint previous;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previous);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    if (flags & PARG_FBO_DEPTH) {
        glGenRenderbuffers(1, &depth);
        glBindRenderbuffer(GL_RENDERBUFFER, depth);
        glRenderbufferStorage(
            GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("Failed to create FBO.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, previous);

    parg_framebuffer* framebuffer = malloc(sizeof(struct parg_framebuffer_s));
    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->tex = tex;
    framebuffer->fbo = fbo;
    framebuffer->depth = depth;
    return framebuffer;
}

parg_framebuffer* parg_framebuffer_create(
    int width, int height, void* src, int nbytes, int flags)
{
    GLenum format = (flags & PARG_FBO_ALPHA) ? GL_RGBA : GL_RGB;
    GLenum type = GL_UNSIGNED_BYTE;
    if (flags & PARG_FBO_FLOAT) {
        type = GL_FLOAT;
    } else if (flags & PARG_FBO_HALF) {
        type = HALF_FLOAT;
    }
    parg_framebuffer* fbo = parg_framebuffer_create_empty(width, height, flags);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, src);
    return fbo;
}

void parg_framebuffer_free(parg_framebuffer* framebuffer)
{
    glDeleteTextures(1, &framebuffer->tex);
    glDeleteFramebuffers(1, &framebuffer->fbo);
    free(framebuffer);
}

void parg_framebuffer_bindtex(parg_framebuffer* fbo, int stage)
{
    glActiveTexture(GL_TEXTURE0 + stage);
    glBindTexture(GL_TEXTURE_2D, fbo->tex);
}

void parg_framebuffer_bindfbo(parg_framebuffer* fbo, int mrt_index)
{
    // MRT is not supported.
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo);
}

void parg_framebuffer_swap(parg_framebuffer* a, parg_framebuffer* b)
{
    assert(a->width == b->width && a->height == b->height);
    PARG_SWAP(GLuint, a->tex, b->tex);
    PARG_SWAP(GLuint, a->fbo, b->fbo);
    PARG_SWAP(GLuint, a->depth, b->depth);
}

void parg_framebuffer_pushfbo(parg_framebuffer* fbo, int mrt_index)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &pushed_fbo);
    glGetIntegerv(GL_VIEWPORT, pushed_viewport);
    glViewport(0, 0, fbo->width, fbo->height);
    parg_framebuffer_bindfbo(fbo, mrt_index);
}

void parg_framebuffer_popfbo()
{
    glBindFramebuffer(GL_FRAMEBUFFER, pushed_fbo);
    glViewport(pushed_viewport[0], pushed_viewport[1], pushed_viewport[2],
        pushed_viewport[3]);
}
