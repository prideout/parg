#include <par.h>
#include <parwin.h>
#include <assert.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_TEXCOORD, "a_texcoord") \
    F(U_MVP, "u_mvp")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

Matrix4 projection;
Point3 camerapos;
par_mesh* rectmesh;
par_texture* palmstexture;

const float gray = 0.8;
const float fovy = 16 * PAR_TWOPI / 180;
const float worldwidth = 6000;

void init(float winwidth, float winheight, float pixratio)
{
    par_state_clearcolor((Vector4){gray, gray, gray, 1});
    par_state_depthtest(0);
    par_state_cullfaces(0);
    par_shader_load_from_asset("zooming.glsl");
    palmstexture = par_texture_from_asset("arecaceae.png");
    int imgwidth, imgheight;
    par_texture_info(palmstexture, &imgwidth, &imgheight);
    float worldheight = worldwidth * imgheight / imgwidth;
    rectmesh = par_mesh_create_rectangle(worldwidth, worldheight);
    const float aspect = (float) winwidth / winheight;
    float cameraheight = 0.5 * worldheight / tan(fovy * 0.5);
    camerapos = (Point3){0, 0, cameraheight};
    const float znear = 1.0;
    const float zfar = cameraheight * 1.1;
    projection = M4MakePerspective(fovy, aspect, znear, zfar);
}

int draw()
{
    Point3 target = {camerapos.x, camerapos.y, 0};
    Vector3 up = {0, 1, 0};
    Matrix4 view = M4MakeLookAt(camerapos, target, up);
    Matrix4 model = M4MakeIdentity();
    Matrix4 modelview = M4Mul(view, model);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_draw_clear();
    par_shader_bind(P_SIMPLE);
    par_texture_bind(palmstexture, 0);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_varray_enable(par_mesh_coord(rectmesh), A_POSITION, 2, PAR_FLOAT, 0, 0);
    par_varray_enable(par_mesh_uv(rectmesh), A_TEXCOORD, 2, PAR_FLOAT, 0, 0);
    par_draw_one_quad();
    return 1;
}

void tick(float winwidth, float winheight, float pixratio, float seconds) {}

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
