#include <parg.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"
#include "pargl.h"
#include "lodepng.h"

struct parg_texture_s {
    int width;
    int height;
    GLuint handle;
};

parg_texture* parg_texture_from_asset(parg_token id)
{
    parg_texture* tex = malloc(sizeof(struct parg_texture_s));
    int* rawdata;
    parg_buffer* pngbuf = parg_buffer_slurp_asset(id, (void*) &rawdata);
    tex->width = *rawdata++;
    tex->height = *rawdata++;
    int ncomps = *rawdata++;
    assert(ncomps == 4);
    glGenTextures(1, &tex->handle);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    parg_texture_fliprows(rawdata, tex->width * ncomps, tex->height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, rawdata);
    parg_buffer_free(pngbuf);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    return tex;
}

parg_texture* parg_texture_from_buffer(parg_buffer* buf)
{
    unsigned char* decoded;
    unsigned dims[3] = {0, 0, 4};
    unsigned char* filedata = parg_buffer_lock(buf, PARG_READ);
    unsigned err = lodepng_decode_memory(&decoded, &dims[0], &dims[1], filedata,
            parg_buffer_length(buf), LCT_RGBA, 8);
    parg_assert(err == 0, "PNG decoding error");
    int nbytes = dims[0] * dims[1] * dims[2];
    assert(dims[2] == 4);
    parg_texture* tex = calloc(sizeof(struct parg_texture_s), 1);
    tex->width = dims[0];
    tex->height = dims[1];
    glGenTextures(1, &tex->handle);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    parg_texture_fliprows(decoded, tex->width * 4, tex->height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, decoded);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    parg_buffer_unlock(buf);
    return tex;
}

parg_texture* parg_texture_from_asset_linear(parg_token id)
{
    parg_texture* tex = malloc(sizeof(struct parg_texture_s));
    int* rawdata;
    parg_buffer* pngbuf = parg_buffer_slurp_asset(id, (void*) &rawdata);
    tex->width = *rawdata++;
    tex->height = *rawdata++;
    int ncomps = *rawdata++;
    assert(ncomps == 4);
    glGenTextures(1, &tex->handle);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    parg_texture_fliprows(rawdata, tex->width * ncomps, tex->height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, rawdata);
    parg_buffer_free(pngbuf);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return tex;
}

void parg_texture_bind(parg_texture* tex, int stage)
{
    glActiveTexture(GL_TEXTURE0 + stage);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
}

void parg_texture_info(parg_texture* tex, int* width, int* height)
{
    *width = tex->width;
    *height = tex->height;
}

void parg_texture_free(parg_texture* tex)
{
    if (tex) {
        glDeleteTextures(1, &tex->handle);
        free(tex);
    }
}

void parg_texture_fliprows(void* data, int rowsize, int nrows)
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

parg_texture* parg_texture_from_u8(
    parg_buffer* buf, int width, int height, int ncomps, int byteoffset)
{
    assert(ncomps == 4);
    parg_texture* tex = malloc(sizeof(struct parg_texture_s));
    tex->width = width;
    tex->height = height;
    char* rawdata = parg_buffer_lock(buf, PARG_READ);
    glGenTextures(1, &tex->handle);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, rawdata + byteoffset);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    parg_buffer_unlock(buf);
    return tex;
}

parg_texture* parg_texture_from_fp32(
    parg_buffer* buf, int width, int height, int ncomps, int byteoffset)
{
    assert(ncomps == 1);
    parg_texture* tex = malloc(sizeof(struct parg_texture_s));
    tex->width = width;
    tex->height = height;
    char* rawdata = parg_buffer_lock(buf, PARG_READ);
    glGenTextures(1, &tex->handle);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, tex->width, tex->height, 0,
        GL_ALPHA, GL_FLOAT, rawdata + byteoffset);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    parg_buffer_unlock(buf);
    return tex;
}
