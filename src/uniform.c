#include <parg.h>
#include "pargl.h"

void parg_uniform1i(parg_token tok, int val)
{
    GLint loc = parg_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform1i(loc, val);
    }
}

void parg_uniform1f(parg_token tok, float val)
{
    GLint loc = parg_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform1f(loc, val);
    }
}

void parg_uniform2f(parg_token tok, float x, float y)
{
    GLint loc = parg_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform2f(loc, x, y);
    }
}

void parg_uniform3f(parg_token tok, const Vector3* val)
{
    GLint loc = parg_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform3fv(loc, 1, &val->x);
    }
}

void parg_uniform4f(parg_token tok, const Vector4* val)
{
    GLint loc = parg_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform4fv(loc, 1, &val->x);
    }
}

void parg_uniform_point(parg_token tok, const Point3* val)
{
    GLint loc = parg_shader_uniform_get(tok);
    if (loc > -1) {
        glUniform3fv(loc, 1, &val->x);
    }
}

void parg_uniform_matrix4f(parg_token tok, const Matrix4* val)
{
    GLint loc = parg_shader_uniform_get(tok);
    if (loc > -1) {
        glUniformMatrix4fv(loc, 1, 0, &val->col0.x);
    }
}

void parg_uniform_matrix3f(parg_token tok, const Matrix3* val)
{
    GLint loc = parg_shader_uniform_get(tok);
    if (loc == -1) {
        return;
    }
    float packed[9];
    for (int i = 0; i < 9; ++i) {
        packed[i] = M3GetElem(*val, i / 3, i % 3);
    }
    glUniformMatrix3fv(loc, 1, 0, &packed[0]);
}
