#include <par.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"
#include "pargl.h"

struct par_texture_s {
    int width;
    int height;
    GLuint handle;
};

par_texture* par_texture_from_asset(par_token id)
{
    par_texture* tex = malloc(sizeof(struct par_texture_s));
    int* rawdata;
    par_buffer* pngbuf = par_buffer_slurp_asset(id, (void*) &rawdata);
    tex->width = *rawdata++;
    tex->height = *rawdata++;
    int ncomps = *rawdata++;
    assert(ncomps == 4);
    glGenTextures(1, &tex->handle);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    par_texture_fliprows(rawdata, tex->width * ncomps, tex->height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, rawdata);
    par_buffer_free(pngbuf);
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
