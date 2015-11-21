#include <par.h>
#include <parwin.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_TEXCOORD, "a_texcoord") \
    F(U_MVP, "u_mvp")
TOKEN_TABLE(PAR_TOKEN_DECLARE);

#define ASSET_TABLE(F)                     \
    F(SHADER_SIMPLE, "hmap.glsl")          \
    F(TEXTURE_COLOR, "msquares_color.png") \
    F(BIN_ISLAND, "msquares_island.1024.bin")
ASSET_TABLE(PAR_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
par_mesh* rectmesh;
par_texture* colortex;
par_texture* graytex;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = {0.5, 0.6, 0.7, 1.0};
    par_state_clearcolor(bgcolor);
    par_state_depthtest(1);
    par_state_cullfaces(0);
    par_shader_load_from_asset(SHADER_SIMPLE);

    colortex = par_texture_from_asset(TEXTURE_COLOR);

    par_buffer* graybuf = par_buffer_from_asset(BIN_ISLAND);
    graytex = par_texture_from_fp32(graybuf, 1024, 1024, 1);
    // do stuff
    par_buffer_free(graybuf);

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
    rectmesh = par_mesh_rectangle(8, 8);
}

void draw()
{
    Matrix4 modelview = M4Mul(view, model);
    Matrix3 invmodelview = M4GetUpper3x3(modelview);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_draw_clear();
    par_shader_bind(P_SIMPLE);
    par_texture_bind(colortex, 0);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_varray_enable(par_mesh_coord(rectmesh), A_POSITION, 2, PAR_FLOAT, 0, 0);
    par_varray_enable(par_mesh_uv(rectmesh), A_TEXCOORD, 2, PAR_FLOAT, 0, 0);
    par_draw_one_quad();
}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_mesh_free(rectmesh);
    par_texture_free(colortex);
    par_texture_free(graytex);
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    ASSET_TABLE(PAR_ASSET_TABLE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
