#include <parg.h>
#include <parwin.h>
#include <assert.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_TEXCOORD, "a_texcoord") \
    F(U_MVP, "u_mvp")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_TABLE(F)                \
    F(SHADER_ZOOMING, "zooming.glsl") \
    F(TEXTURE_PALMS, "arecaceae.png")
ASSET_TABLE(PARG_TOKEN_DECLARE);

parg_mesh* rectmesh;
parg_texture* palmstexture;

const float gray = 0.8;
const float fovy = 16 * PARG_TWOPI / 180;
const float worldwidth = 6000;

void init(float winwidth, float winheight, float pixratio)
{
    parg_state_clearcolor((Vector4){gray, gray, gray, 1});
    parg_state_depthtest(0);
    parg_state_cullfaces(0);
    parg_shader_load_from_asset(SHADER_ZOOMING);
    palmstexture = parg_texture_from_asset(TEXTURE_PALMS);
    int imgwidth, imgheight;
    parg_texture_info(palmstexture, &imgwidth, &imgheight);
    float worldheight = worldwidth * imgheight / imgwidth;
    parg_zcam_init(worldwidth, worldheight, fovy);
    rectmesh = parg_mesh_rectangle(worldwidth, worldheight);
}

void draw()
{
    Matrix4 view;
    Matrix4 projection;
    parg_zcam_matrices(&projection, &view);
    Matrix4 model = M4MakeIdentity();
    Matrix4 modelview = M4Mul(view, model);
    Matrix4 mvp = M4Mul(projection, modelview);
    parg_draw_clear();
    parg_shader_bind(P_SIMPLE);
    parg_texture_bind(palmstexture, 0);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_varray_enable(
        parg_mesh_coord(rectmesh), A_POSITION, 2, PARG_FLOAT, 0, 0);
    parg_varray_enable(parg_mesh_uv(rectmesh), A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    parg_draw_one_quad();
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    parg_zcam_tick(winwidth / winheight, seconds);
    return parg_zcam_has_moved();
}

void dispose()
{
    parg_shader_free(P_SIMPLE);
    parg_mesh_free(rectmesh);
    parg_texture_free(palmstexture);
}

void input(parg_event evt, float x, float y, float z)
{
    switch (evt) {
    case PARG_EVENT_DOWN:
        parg_zcam_grab_begin(x, y);
        break;
    case PARG_EVENT_UP:
        parg_zcam_grab_update(x, y, z);
        parg_zcam_grab_end();
        break;
    case PARG_EVENT_MOVE:
        parg_zcam_grab_update(x, y, z);
        break;
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PARG_TOKEN_DEFINE);
    ASSET_TABLE(PARG_ASSET_TABLE);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_ontick(tick);
    parg_window_ondraw(draw);
    parg_window_onexit(dispose);
    parg_window_oninput(input);
    return parg_window_exec(185 * 5, 100 * 5, 1);
}
