#include <parg.h>
#include <parwin.h>

#define PAR_SHAPES_IMPLEMENTATION
#include <par/par_shapes.h>

#define TOKEN_TABLE(F)          \
    F(P_TEXTURE, "p_texture")   \
    F(M_KLEIN, "klein.obj")     \
    F(T_KLEIN, "klein.png")     \
    F(A_POSITION, "a_position") \
    F(A_TEXCOORD, "a_texcoord") \
    F(U_MVP, "u_mvp")           \
    F(S_SIMPLE, "klein.glsl")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
parg_mesh* kleingeo;
parg_texture* kleintex;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = {0.5, 0.6, 0.7, 1.0};
    kleingeo = parg_mesh_from_asset(M_KLEIN);
    kleintex = parg_texture_from_asset(T_KLEIN);
    parg_state_clearcolor(bgcolor);
    parg_state_depthtest(1);
    parg_state_cullfaces(1);
    parg_shader_load_from_asset(S_SIMPLE);
    const float h = 1.0f;
    const float w = h * winwidth / winheight;
    const float znear = 4;
    const float zfar = 20;
    projection = M4MakeFrustum(-w, w, -h, h, znear, zfar);
    Point3 eye = {0, 1.8, 5};
    Point3 target = {0, 0.7, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    model = M4MakeIdentity();
}

void draw()
{
    Matrix4 modelview = M4Mul(view, model);
    Matrix4 mvp = M4Mul(projection, modelview);
    parg_draw_clear();

    parg_shader_bind(P_TEXTURE);
    parg_texture_bind(kleintex, 0);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_varray_enable(
        parg_mesh_coord(kleingeo), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_enable(parg_mesh_uv(kleingeo), A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    parg_varray_bind(parg_mesh_index(kleingeo));
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(kleingeo));
    parg_varray_disable(A_TEXCOORD);
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    const float RADIANS_PER_SECOND = 3.14;
    float theta = seconds * RADIANS_PER_SECOND;
    model = M4MakeRotationY(theta);
    return 1;
}

void dispose()
{
    parg_shader_free(P_TEXTURE);
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
    return parg_window_exec(300, 300, 1, 1);
}
