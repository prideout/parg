#include <par.h>
#include <parwin.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_NORMAL, "a_normal")     \
    F(U_MVP, "u_mvp")           \
    F(U_IMV, "u_imv")           \
    F(U_CLIPZ, "u_clipz")       \
    F(SHADER_SIMPLE, "clipping.glsl")
TOKEN_TABLE(PAR_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
par_mesh* torus;
float clipz = 0;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = {0.5, 0.6, 0.7, 1.0};
    par_state_clearcolor(bgcolor);
    par_state_depthtest(1);
    par_state_cullfaces(0);
    par_shader_load_from_asset(SHADER_SIMPLE);
    const float h = 5.0f;
    const float w = h * winwidth / winheight;
    const float znear = 65;
    const float zfar = 90;
    projection = M4MakeFrustum(-w, w, -h, h, znear, zfar);
    Point3 eye = {0, -75, 25};
    Point3 target = {0, 0, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    model = M4MakeIdentity();
    torus = par_mesh_torus(400, 100, 8, 2);
}

int draw()
{
    Matrix4 modelview = M4Mul(view, model);
    Matrix3 invmodelview = M4GetUpper3x3(modelview);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_draw_clear();
    par_shader_bind(P_SIMPLE);
    par_uniform1f(U_CLIPZ, clipz);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_uniform_matrix3f(U_IMV, &invmodelview);
    par_varray_enable(par_mesh_coord(torus), A_POSITION, 3, PAR_FLOAT, 0, 0);
    par_varray_enable(par_mesh_norml(torus), A_NORMAL, 3, PAR_FLOAT, 0, 0);
    par_varray_bind(par_mesh_index(torus));
    par_draw_triangles_u16(0, par_mesh_ntriangles(torus));
    return 1;
}

void tick(float winwidth, float winheight, float pixratio, float seconds)
{
    clipz = 0.4 + 0.05 * sin(seconds * 3);
}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_mesh_free(torus);
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    par_asset_preload(SHADER_SIMPLE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
