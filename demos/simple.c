#include <parg.h>
#include <parwin.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(U_COLOR, "u_color")       \
    F(SHADER_SIMPLE, "simple.glsl")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
parg_buffer* tricoords;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = V4ScalarDiv((Vector4){78, 61, 66, 255}, 255);
    parg_state_clearcolor(bgcolor);
    parg_state_cullfaces(1);
    parg_shader_load_from_asset(SHADER_SIMPLE);
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
    tricoords = parg_buffer_alloc(sizeof(Point3) * 3, PARG_GPU_ARRAY);
    Point3* pdata = (Point3*) parg_buffer_lock(tricoords, PARG_WRITE);
    *pdata++ = (Point3){1, 1, 0};
    *pdata++ = (Point3){-1, 1, 0};
    *pdata++ = (Point3){0, -1, 0};
    parg_buffer_unlock(tricoords);
}

void draw()
{
    const Vector4 fgcolor = V4ScalarDiv((Vector4){198, 226, 233, 255}, 255);
    Matrix4 mvp = M4Mul(projection, M4Mul(view, model));
    parg_draw_clear();
    parg_shader_bind(P_SIMPLE);
    parg_uniform4f(U_COLOR, &fgcolor);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_varray_enable(tricoords, A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_draw_triangles(0, 1);
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    const float RADIANS_PER_SECOND = 3.14;
    float theta = seconds * RADIANS_PER_SECOND;
    model = M4MakeRotationZ(theta);
    return 1;
}

void dispose()
{
    parg_shader_free(P_SIMPLE);
    parg_buffer_free(tricoords);
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PARG_TOKEN_DEFINE);
    parg_asset_preload(SHADER_SIMPLE);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_ontick(tick);
    parg_window_ondraw(draw);
    parg_window_onexit(dispose);
    return parg_window_exec(185 * 5, 100 * 5, 1);
}
