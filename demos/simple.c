#include <par.h>
#include <parwin.h>
#include <pargl.h>
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
    const Vector4 bgcolor = V4ScalarDiv((Vector4){78, 61, 66, 255}, 255);

    par_state_clearcolor(bgcolor);
    par_state_cullfaces(1);
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
    const Vector4 fgcolor = V4ScalarDiv((Vector4){198, 226, 233, 255}, 255);
    Matrix4 mvp = M4Mul(projection, M4Mul(view, model));

    int position = par_shader_attrib_get(A_POSITION);
    par_draw_clear();
    par_shader_bind(P_SIMPLE);
    par_uniform4f(U_COLOR, &fgcolor);
    par_uniform_matrix4f(U_MVP, &mvp);

    // VAO START
    glBindBuffer(GL_ARRAY_BUFFER, par_buffer_gpu_handle(tricoords));
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // VAO END

    par_draw_triangles(0, 1);

    return 1;
}

void tick(float winwidth, float winheight, float pixratio, float seconds)
{
    const float RADIANS_PER_SECOND = 3.14;
    float theta = seconds * RADIANS_PER_SECOND;
    model = M4MakeRotationZ(theta);
}

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
