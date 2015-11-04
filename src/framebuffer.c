#include <par.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"
#include "pargl.h"

struct par_framebuffer_s {
    int width;
    int height;
    GLuint tex;
    GLuint fbo;
    GLuint depth;
};

par_framebuffer* par_framebuffer_create(int width, int height)
{
    GLuint tex, fbo, depth;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, 0);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("Failed to create FBO.");
    }

    par_framebuffer* framebuffer = malloc(sizeof(struct par_framebuffer_s));
    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->tex = tex;
    framebuffer->fbo = fbo;
    framebuffer->depth = depth;
    return framebuffer;
}

void par_framebuffer_free(par_framebuffer* framebuffer)
{
    glDeleteTextures(1, &framebuffer->tex);
    glDeleteFramebuffers(1, &framebuffer->fbo);
    free(framebuffer);
}
