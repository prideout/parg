#include <par.h>
#include <parwin.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_NORMAL, "a_normal")     \
    F(U_MVP, "u_mvp")           \
    F(U_IMV, "u_imv")           \
    F(SHADER_PICKING, "picking.glsl")
TOKEN_TABLE(PAR_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
par_mesh* knot;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = V4ScalarDiv((Vector4){78, 61, 66, 255}, 255);
    par_state_clearcolor(bgcolor);
    par_state_depthtest(1);
    par_state_cullfaces(1);
    par_shader_load_from_asset(SHADER_PICKING);
    const float fovy = 16 * PAR_TWOPI / 180;
    const float aspect = (float) winwidth / winheight;
    const float znear = 0.1;
    const float zfar = 300;
    projection = M4MakePerspective(fovy, aspect, znear, zfar);
    Point3 eye = {0, 0, 4};
    Point3 target = {0, 0, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    model = M4MakeIdentity();
    knot = par_mesh_knot(400, 100, 8, 2);
}

void draw()
{
    Matrix4 modelview = M4Mul(view, model);
    Matrix3 invmodelview = M4GetUpper3x3(modelview);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_draw_clear();
    par_shader_bind(P_SIMPLE);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_uniform_matrix3f(U_IMV, &invmodelview);
    par_varray_enable(par_mesh_coord(knot), A_POSITION, 3, PAR_FLOAT, 0, 0);
    par_varray_enable(par_mesh_norml(knot), A_NORMAL, 3, PAR_FLOAT, 0, 0);
    par_varray_bind(par_mesh_index(knot));
    par_draw_triangles_u16(0, par_mesh_ntriangles(knot));
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
    par_shader_free(P_SIMPLE);
    par_mesh_free(knot);
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    par_asset_preload(SHADER_PICKING);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
