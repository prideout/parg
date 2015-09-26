#include <par.h>
#include <stdio.h>
#include "sds.h"

par_buffer* par_buffer_from_file(const char* filepath)
{
    printf("TODO: slurp file from %s\n", filepath);
    return 0;
}

par_buffer* par_buffer_from_asset(const char* filename)
{
    sds path = (sds) par_asset_whereami();
    sds assetpath = sdscat(sdsdup(path), filename);
    par_buffer* retval = par_buffer_from_file(assetpath);
    sdsfree(assetpath);
    return retval;
}

void par_buffer_free(par_buffer* buf)
{
}

