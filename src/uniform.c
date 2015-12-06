#include <parg.h>
#include "pargl.h"

void par_uniform1f(par_token tok, float val)
{
    GLint loc = par_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform1f(loc, val);
    }
}

void par_uniform3f(par_token tok, const Vector3* val)
{
    GLint loc = par_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform3fv(loc, 1, &val->x);
    }
}

void par_uniform4f(par_token tok, const Vector4* val)
{
    GLint loc = par_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform4fv(loc, 1, &val->x);
    }
}

void par_uniform_point(par_token tok, const Point3* val)
{
    GLint loc = par_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform3fv(loc, 1, &val->x);
    }
}

void par_uniform_matrix4f(par_token tok, const Matrix4* val)
{
    GLint loc = par_shader_uniform_get(tok);
    if (loc > -1) {
        glUniformMatrix4fv(loc, 1, 0, &val->col0.x);
    }
}

void par_uniform_matrix3f(par_token tok, const Matrix3* val)
{
    GLint loc = par_shader_uniform_get(tok);
    if (loc == -1) {
        return;
    }
    float packed[9];
    for (int i = 0; i < 9; ++i) {
        packed[i] = M3GetElem(*val, i / 3, i % 3);
    }
    glUniformMatrix3fv(loc, 1, 0, &packed[0]);
}
