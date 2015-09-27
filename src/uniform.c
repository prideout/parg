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

void par_uniform_matrix3f(par_token tok, const Matrix3* val)
{
    float packed[9];
    for (int i = 0; i < 9; ++i) {
        packed[i] = M3GetElem(*val, i / 3, i % 3);
    }
    glUniformMatrix3fv(par_shader_uniform_get(tok), 1, 0, &packed[0]);
}
