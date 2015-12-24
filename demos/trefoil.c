#include <parg.h>
#include <parwin.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_NORMAL, "a_normal")     \
    F(U_MVP, "u_mvp")           \
    F(U_IMV, "u_imv")           \
    F(SHADER_SIMPLE, "trefoil.glsl")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
parg_mesh* knot;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = V4ScalarDiv((Vector4){78, 61, 66, 255}, 255);
    parg_state_clearcolor(bgcolor);
    parg_state_depthtest(1);
    parg_state_cullfaces(1);
    parg_shader_load_from_asset(SHADER_SIMPLE);
    const float fovy = 16 * PARG_TWOPI / 180;
    const float aspect = (float) winwidth / winheight;
    const float znear = 0.1;
    const float zfar = 300;
    projection = M4MakePerspective(fovy, aspect, znear, zfar);
    Point3 eye = {0, 0, 4};
    Point3 target = {0, 0, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    model = M4MakeIdentity();
    knot = parg_mesh_knot(400, 100, 8, 2);
}

void draw()
{
    Matrix4 modelview = M4Mul(view, model);
    Matrix3 invmodelview = M4GetUpper3x3(modelview);
    Matrix4 mvp = M4Mul(projection, modelview);
    parg_draw_clear();
    parg_shader_bind(P_SIMPLE);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_uniform_matrix3f(U_IMV, &invmodelview);
    parg_varray_enable(parg_mesh_coord(knot), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_enable(parg_mesh_norml(knot), A_NORMAL, 3, PARG_FLOAT, 0, 0);
    parg_varray_bind(parg_mesh_index(knot));
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(knot));
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    const float RADIANS_PER_SECOND = 1.57;
    float theta = seconds * RADIANS_PER_SECOND;
    model = M4MakeRotationY(theta);
    return 1;
}

void dispose()
{
    parg_shader_free(P_SIMPLE);
    parg_mesh_free(knot);
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
    return parg_window_exec(185 * 5, 100 * 5, 1, 1);
}
