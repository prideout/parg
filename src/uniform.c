#include <par.h>
#include "pargl.h"

void par_uniform4f(par_token tok, const Vector4* val)
{
    glUniform4fv(par_shader_uniform_get(tok), 1, &val->x);
}

void par_uniform_matrix4f(par_token tok, const Matrix4* val)
{
    glUniformMatrix4fv(par_shader_uniform_get(tok), 1, 0, &val->col0.x);
}
