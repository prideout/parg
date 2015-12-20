#include <parg.h>
#include "internal.h"
#include "pargl.h"
#include <stdlib.h>
#include <string.h>

struct parg_buffer_s {
    char* data;
    int nbytes;
    parg_buffer_type memtype;
    GLuint gpuhandle;
    char* gpumapped;
};

parg_buffer* parg_buffer_create(void* src, int nbytes, parg_buffer_type memtype)
{
    parg_buffer* retval = malloc(sizeof(struct parg_buffer_s));
    retval->nbytes = nbytes;
    retval->memtype = memtype;
    retval->gpuhandle = 0;
    retval->gpumapped = 0;
    if (parg_buffer_gpu_check(retval)) {
        glGenBuffers(1, &retval->gpuhandle);
        GLenum target = memtype == PARG_GPU_ARRAY ? GL_ARRAY_BUFFER
            : GL_ELEMENT_ARRAY_BUFFER;
        glBindBuffer(target, retval->gpuhandle);
        glBufferData(target, nbytes, src, GL_STATIC_DRAW);
    } else {
        retval->data = malloc(nbytes);
        memcpy(retval->data, src, nbytes);
    }
    return retval;
}

int parg_buffer_gpu_check(parg_buffer* buf)
{
    return buf->memtype == PARG_GPU_ARRAY || buf->memtype == PARG_GPU_ELEMENTS;
}

GLuint parg_buffer_gpu_handle(parg_buffer* buf) { return buf->gpuhandle; }

parg_buffer* parg_buffer_alloc(int nbytes, parg_buffer_type memtype)
{
    parg_buffer* retval = malloc(sizeof(struct parg_buffer_s));
    retval->data = (memtype == PARG_CPU) ? malloc(nbytes) : 0;
    retval->nbytes = nbytes;
    retval->memtype = memtype;
    retval->gpuhandle = 0;
    retval->gpumapped = 0;
    if (parg_buffer_gpu_check(retval)) {
        glGenBuffers(1, &retval->gpuhandle);
    }
    return retval;
}

parg_buffer* parg_buffer_dup(parg_buffer* srcbuf, parg_buffer_type memtype)
{
    int nbytes = parg_buffer_length(srcbuf);
    parg_buffer* dstbuf = parg_buffer_alloc(nbytes, memtype);
    void* src = parg_buffer_lock(srcbuf, PARG_READ);
    void* dst = parg_buffer_lock(dstbuf, PARG_WRITE);
    memcpy(dst, src, nbytes);
    parg_buffer_unlock(dstbuf);
    parg_buffer_unlock(srcbuf);
    return dstbuf;
}

void parg_buffer_free(parg_buffer* buf)
{
    if (!buf) {
        return;
    }
    if (parg_buffer_gpu_check(buf)) {
        glDeleteBuffers(1, &buf->gpuhandle);
    } else {
        free(buf->data);
    }
    free(buf);
}

int parg_buffer_length(parg_buffer* buf)
{
    parg_assert(buf, "Null buffer");
    return buf->nbytes;
}

void* parg_buffer_lock(parg_buffer* buf, parg_buffer_mode access)
{
    if (access == PARG_WRITE && parg_buffer_gpu_check(buf)) {
        buf->gpumapped = malloc(buf->nbytes);
        return buf->gpumapped;
    }
    return buf->data;
}

void parg_buffer_unlock(parg_buffer* buf)
{
    if (buf->gpumapped) {
        GLenum target = buf->memtype == PARG_GPU_ARRAY
            ? GL_ARRAY_BUFFER
            : GL_ELEMENT_ARRAY_BUFFER;
        glBindBuffer(target, buf->gpuhandle);
        glBufferData(target, buf->nbytes, buf->gpumapped, GL_STATIC_DRAW);
        free(buf->gpumapped);
        buf->gpumapped = 0;
    }
}

parg_buffer* parg_buffer_from_file(const char* filepath)
{
    FILE* f = fopen(filepath, "rb");
    parg_verify(f, "Unable to open file", filepath);
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    parg_buffer* retval = parg_buffer_alloc(fsize + 1, PARG_CPU);
    char* contents = parg_buffer_lock(retval, PARG_WRITE);
    fread(contents, fsize, 1, f);
    fclose(f);
    contents[fsize] = 0;
    parg_buffer_unlock(retval);
    return retval;
}

void parg_buffer_to_file(parg_buffer* buf, const char* filepath)
{
    FILE* f = fopen(filepath, "wb");
    parg_verify(f, "Unable to open file", filepath);
    char* contents = parg_buffer_lock(buf, PARG_READ);
    fwrite(contents, 1, parg_buffer_length(buf), f);
    fclose(f);
    parg_buffer_unlock(buf);
}

parg_buffer* parg_buffer_from_asset(parg_token id)
{
    return parg_asset_to_buffer(id);
}

parg_buffer* parg_buffer_slurp_asset(parg_token id, void** ptr)
{
    parg_buffer* buf = parg_asset_to_buffer(id);
    *ptr = parg_buffer_lock(buf, PARG_READ);
    return buf;
}

parg_buffer* parg_buffer_from_path(const char* filename)
{
#if EMSCRIPTEN
    sds baseurl = parg_asset_baseurl();
    sds fullurl = sdscat(sdsdup(baseurl), filename);
    parg_buffer* retval = 0;
    printf("TODO: download %s here\n", fullurl);
    sdsfree(fullurl);
#else
    sds execdir = parg_asset_whereami();
    sds fullpath = sdscat(sdsdup(execdir), filename);
    if (!parg_asset_fileexists(fullpath)) {
        parg_asset_download(filename, fullpath);
    }
    parg_buffer* retval = parg_buffer_from_file(fullpath);
    sdsfree(fullpath);
#endif
    return retval;
}

void parg_buffer_gpu_bind(parg_buffer* buf)
{
    parg_assert(parg_buffer_gpu_check(buf), "GPU buffer required");
    GLenum target = buf->memtype == PARG_GPU_ARRAY ? GL_ARRAY_BUFFER
        : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(target, parg_buffer_gpu_handle(buf));
}
