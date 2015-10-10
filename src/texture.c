#include <par.h>
#include "internal.h"
#include "pargl.h"
#include <stdlib.h>
#include "lodepng.h"

struct par_texture_s {
    int width;
    int height;
    GLuint handle;
};

par_texture* par_texture_from_asset(par_token id)
{
    par_buffer* pngbuf = par_buffer_from_asset(id);
    par_texture* tex = malloc(sizeof(struct par_texture_s));
    unsigned char* grayvals;
    const unsigned char* pngbytes = par_buffer_lock(pngbuf, PAR_READ);
    unsigned err = lodepng_decode_memory(&grayvals, (unsigned int*) &tex->width,
            (unsigned int*) &tex->height, pngbytes, par_buffer_length(pngbuf),
            LCT_GREY, 8);
    par_verify(err == 0, "PNG decoding error", 0);
    par_buffer_unlock(pngbuf);
    par_buffer_free(pngbuf);
    glGenTextures(1, &tex->handle);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    par_texture_fliprows(grayvals, tex->width, tex->height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, tex->width, tex->height, 0,
        GL_LUMINANCE, GL_UNSIGNED_BYTE, grayvals);
    free(grayvals);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
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

void par_texture_fliprows(void* data, int rowsize, int nrows)
{
    char* tmp = malloc(rowsize);
    char* top = data;
    char* bottom = top + rowsize * nrows;
    for (int i = 0; i < nrows / 2; i++) {
        bottom -= rowsize;
        memcpy(tmp, top, rowsize);
        memcpy(top, bottom, rowsize);
        memcpy(bottom, tmp, rowsize);
        top += rowsize;
    }
    free(tmp);
}
