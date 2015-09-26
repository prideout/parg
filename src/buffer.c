#include <par.h>
#include <stdio.h>
#include <stdlib.h>
#include "sds.h"
#include "verify.h"

struct par_buffer_s {
    char* data;
    int nbytes;
    par_buffer_type memtype;
};

par_buffer* par_buffer_alloc(int nbytes, par_buffer_type memtype)
{
    par_buffer* retval = malloc(sizeof(struct par_buffer_s));
    retval->data = malloc(nbytes);
    retval->nbytes = nbytes;
    retval->memtype = memtype;
    return retval;
}

void par_buffer_free(par_buffer* buf)
{
    free(buf->data);
    free(buf);
}

char* par_buffer_lock(par_buffer* buf, par_buffer_mode access)
{
    return buf->data;
}

void par_buffer_unlock(par_buffer* buf) {}

par_buffer* par_buffer_from_file(const char* filepath)
{
    FILE* f = fopen(filepath, "rb");
    par_verify(f, "Unable to open file", filepath);
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    par_buffer* retval = par_buffer_alloc(fsize + 1, PAR_CPU);
    char* contents = par_buffer_lock(retval, PAR_READ);
    fread(contents, fsize, 1, f);
    fclose(f);
    contents[fsize] = 0;
    par_buffer_unlock(retval);
    return retval;
}

par_buffer* par_buffer_from_asset(const char* filename)
{
    sds path = (sds) par_asset_whereami();
    sds assetpath = sdscat(sdsdup(path), filename);
    par_buffer* retval = par_buffer_from_file(assetpath);
    sdsfree(assetpath);
    return retval;
}
