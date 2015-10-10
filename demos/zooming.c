#include <par.h>
#include <parwin.h>
#include <assert.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_TEXCOORD, "a_texcoord") \
    F(U_MVP, "u_mvp")
TOKEN_TABLE(PAR_TOKEN_DECLARE);

#define ASSET_TABLE(F)                \
    F(SHADER_ZOOMING, "zooming.glsl") \
    F(TEXTURE_PALMS, "arecaceae.png")
ASSET_TABLE(PAR_TOKEN_DECLARE);

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
    par_shader_load_from_asset(SHADER_ZOOMING);
    palmstexture = par_texture_from_asset(TEXTURE_PALMS);
    int imgwidth, imgheight;
    par_texture_info(palmstexture, &imgwidth, &imgheight);
    float worldheight = worldwidth * imgheight / imgwidth;
    par_zcam_init(worldwidth, worldheight, fovy);
    rectmesh = par_mesh_create_rectangle(worldwidth, worldheight);
}

int draw()
{
    Matrix4 view;
    Matrix4 projection;
    par_zcam_matrices(&projection, &view);
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

void tick(float winwidth, float winheight, float pixratio, float seconds)
{
    par_zcam_tick(winwidth / winheight, seconds);
}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_mesh_free(rectmesh);
    par_texture_free(palmstexture);
}

void input(par_event evt, float x, float y, float z)
{
    switch (evt) {
        case PAR_EVENT_DOWN:
            par_zcam_grab_begin(x, y);
            break;
        case PAR_EVENT_UP:
            par_zcam_grab_update(x, y, z);
            par_zcam_grab_release();
            break;
        case PAR_EVENT_MOVE:
            par_zcam_grab_update(x, y, z);
            break;
    }
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    ASSET_TABLE(PAR_ASSET_TABLE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    par_window_oninput(input);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
