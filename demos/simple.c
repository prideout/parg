#include <par.h>
#include <parwin.h>
#include <pargl.h>
#include <vmath.h>
#include <stdio.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(U_COLOR, "u_color")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
par_buffer* tricoords;

void init(float winwidth, float winheight, float pixratio)
{
    glClearColor(167 / 255.0, 190 / 255.0, 211 / 255.0, 1.0);
    glEnable(GL_CULL_FACE);

    par_shader_load_from_asset("simple.glsl");

    const float h = 5.0f;
    const float w = h * winwidth / winheight;
    const float znear = 65;
    const float zfar = 90;
    projection = M4MakeFrustum(-w, w, -h, h, znear, zfar);

    Point3 eye = {0, 0, 75};
    Point3 target = {0, 0, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    model = M4MakeIdentity();

    tricoords = par_buffer_alloc(sizeof(Point3) * 3, PAR_GPU_ARRAY);
    Point3* pdata = (Point3*) par_buffer_lock(tricoords, PAR_WRITE);
    *pdata++ = (Point3){1, 1, 0};
    *pdata++ = (Point3){-1, 1, 0};
    *pdata++ = (Point3){0, -1, 0};
    par_buffer_unlock(tricoords);
}

int draw()
{
    Matrix4 modelview = M4Mul(view, model);
    Matrix4 mvp = M4Mul(projection, modelview);

    int position = par_shader_attrib_get(A_POSITION);
    glClear(GL_COLOR_BUFFER_BIT);
    par_shader_bind(P_SIMPLE);
    glUniform4f(par_shader_uniform_get(U_COLOR), 218 / 255.0, 184 / 255.0,
        148 / 255.0, 1);
    glUniformMatrix4fv(par_shader_uniform_get(U_MVP), 1, 0, &mvp.col0.x);

    // VAO START
    glBindBuffer(GL_ARRAY_BUFFER, par_buffer_gpu_handle(tricoords));
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // VAO END

    glDrawArrays(GL_TRIANGLES, 0, 3);
    return 1;
}

void tick(float seconds, float winwidth, float winheight, float pixratio) {}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_buffer_free(tricoords);
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
