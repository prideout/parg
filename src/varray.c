#include <parg.h>
#include "pargl.h"
#include "internal.h"

void parg_varray_enable(parg_buffer* buf, parg_token attr, int ncomps,
    parg_data_type type, int stride, int offset)
{
    parg_buffer_gpu_bind(buf);
    GLint slot = parg_shader_attrib_get(attr);
    glEnableVertexAttribArray(slot);
    long offset64 = offset;
    const GLvoid* ptr = (const GLvoid*) offset64;
    glVertexAttribPointer(slot, ncomps, type, GL_FALSE, stride, ptr);
}

void parg_varray_bind(parg_buffer* buf) { parg_buffer_gpu_bind(buf); }

void parg_varray_disable(parg_token attr)
{
    GLint slot = parg_shader_attrib_get(attr);
    glDisableVertexAttribArray(slot);
}
