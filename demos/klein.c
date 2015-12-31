#include <parg.h>
#include <parwin.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(M_KLEIN, "klein.obj")     \
    F(T_KLEIN, "klein.png")     \
    F(A_POSITION, "a_position") \
    F(A_NORMAL, "a_normal")     \
    F(U_MVP, "u_mvp")           \
    F(U_IMV, "u_imv")           \
    F(U_CLIPZ, "u_clipz")       \
    F(S_SIMPLE, "klein.glsl")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
parg_mesh* torus;
parg_mesh* kleingeo;
parg_texture* kleintex;
float clipz = 0;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = {0.5, 0.6, 0.7, 1.0};
    kleingeo = parg_mesh_from_asset(M_KLEIN);
    kleintex = parg_texture_from_asset(T_KLEIN);
    parg_state_clearcolor(bgcolor);
    parg_state_depthtest(1);
    parg_state_cullfaces(0);
    parg_shader_load_from_asset(S_SIMPLE);
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
    torus = parg_mesh_torus(400, 100, 8, 2);
}

void draw()
{
    Matrix4 modelview = M4Mul(view, model);
    Matrix3 invmodelview = M4GetUpper3x3(modelview);
    Matrix4 mvp = M4Mul(projection, modelview);
    parg_draw_clear();
    parg_shader_bind(P_SIMPLE);
    parg_uniform1f(U_CLIPZ, clipz);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_uniform_matrix3f(U_IMV, &invmodelview);
    parg_varray_enable(parg_mesh_coord(torus), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_enable(parg_mesh_norml(torus), A_NORMAL, 3, PARG_FLOAT, 0, 0);
    parg_varray_bind(parg_mesh_index(torus));
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(torus));
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    clipz = 0.4 + 0.05 * sin(seconds * 3);
    return 1;
}

void dispose()
{
    parg_shader_free(P_SIMPLE);
    parg_mesh_free(torus);
    parg_mesh_free(kleingeo);
    parg_texture_free(kleintex);
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PARG_TOKEN_DEFINE);
    parg_asset_preload(S_SIMPLE);
    parg_asset_preload(M_KLEIN);
    parg_asset_preload(T_KLEIN);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_ontick(tick);
    parg_window_ondraw(draw);
    parg_window_onexit(dispose);
    return parg_window_exec(185 * 5, 100 * 5, 1, 1);
}
