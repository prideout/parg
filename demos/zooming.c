#include <par.h>
#include <parwin.h>
#include <assert.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_NORMAL, "a_normal")     \
    F(U_MVP, "u_mvp")           \
    F(U_IMV, "u_imv")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
par_mesh* rectmesh;
par_texture* palmstexture;

void init(float winwidth, float winheight, float pixratio)
{
    float gray = 0.8;
    par_state_clearcolor((Vector4){gray, gray, gray, 1});
    par_state_depthtest(0);
    par_state_cullfaces(0);
    par_shader_load_from_asset("zooming.glsl");

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

    palmstexture = par_texture_from_asset("arecaceae.png");
    int w, h;
    par_texture_info(palmstexture, &w, &h);
    rectmesh = par_mesh_create_rectangle(2, 2.0 * h / w);
}

int draw()
{
    Matrix4 modelview = M4Mul(view, model);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_draw_clear();
    par_shader_bind(P_SIMPLE);
    par_texture_bind(palmstexture, 0);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_varray_enable(par_mesh_coord(rectmesh), A_POSITION, 2, PAR_FLOAT, 0, 0);
    par_draw_one_quad();
    return 1;
}

void tick(float winwidth, float winheight, float pixratio, float seconds)
{
    const float RADIANS_PER_SECOND = 1.57;
    float theta = seconds * RADIANS_PER_SECOND;
    model = M4MakeRotationY(theta);
}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_mesh_free(rectmesh);
    par_texture_free(palmstexture);
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
