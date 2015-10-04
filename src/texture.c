#include <par.h>
#include <assert.h>
#include <stdlib.h>
#include "pargl.h"
#include "lodepng.h"

struct par_texture_s {
    int width;
    int height;
    GLuint handle;
};

par_texture* par_texture_from_asset(const char* filename)
{
    par_buffer* pngbuf = par_buffer_from_asset(filename);
    assert(pngbuf);
    par_texture* tex = malloc(sizeof(struct par_texture_s));
    unsigned char* grayvals;
    const unsigned char* pngbytes = par_buffer_lock(pngbuf, PAR_READ);
    unsigned err = lodepng_decode_memory(&grayvals, (unsigned int*) &tex->width,
            (unsigned int*) &tex->height, pngbytes, par_buffer_length(pngbuf),
            LCT_GREY, 8);
    assert(err == 0);
    par_buffer_unlock(pngbuf);
    par_buffer_free(pngbuf);
    glGenTextures(1, &tex->handle);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    glTexImage2D(GL_TEXTURE_2D,
     	0,
     	GL_LUMINANCE,
     	tex->width,
     	tex->height,
     	0,
     	GL_LUMINANCE,
     	GL_UNSIGNED_BYTE,
     	grayvals);
    free(grayvals);
    return tex;
}

void par_texture_bind(par_texture* tex, int stage)
{
    glActiveTexture(GL_TEXTURE0 + stage);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
}

void par_texture_info(par_texture* tex, int* width, int* height)
{
    *width = tex->width;
    *height = tex->height;
}

void par_texture_free(par_texture* tex)
{
    glDeleteTextures(1, &tex->handle);
    free(tex);
}