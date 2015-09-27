#include <par.h>
#include "pargl.h"
#include "verify.h"

void par_varray_enable(par_buffer* buf, par_token attr, int ncomps,
    par_data_type type, int stride, int offset)
{
    par_buffer_gpu_bind(buf);
    GLint slot = par_shader_attrib_get(attr);
    glEnableVertexAttribArray(slot);
    long offset64 = offset;
    const GLvoid* ptr = (const GLvoid*) offset64;
    glVertexAttribPointer(slot, ncomps, type, GL_FALSE, stride, ptr);
}

void par_varray_bind(par_buffer* buf) { par_buffer_gpu_bind(buf); }

void par_varray_disable(par_token attr)
{
    GLint slot = par_shader_attrib_get(attr);
    glDisableVertexAttribArray(slot);
}
