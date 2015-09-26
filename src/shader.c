#include <par.h>
#include <stdio.h>
#include <assert.h>

void par_shader_load_from_buffer(par_buffer* buf)
{
    printf("TODO: parse shader\n");
}

void par_shader_load_from_asset(const char* filename)
{
    par_buffer* buf = par_buffer_from_asset(filename);
    // assert(buf);
    par_shader_load_from_buffer(buf);
    par_buffer_free(buf);
}

GLuint par_shader_attrib(par_token tok)
{
    return 0;
}

GLint par_shader_uniform(par_token tok)
{
    return 0;
}

void par_shader_bind(par_token tok)
{
}
