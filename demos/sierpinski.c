#include <parg.h>
#include <parwin.h>
#include <assert.h>
#include <stdio.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(SHADER_SIMPLE, "sierpinski.glsl")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

parg_mesh* trimesh;
const float gray = 0.8;
const float fovy = 16 * PARG_TWOPI / 180;
const float worldwidth = 6000;

void init(float winwidth, float winheight, float pixratio)
{
    parg_state_clearcolor((Vector4){gray, gray, gray, 1});
    parg_state_depthtest(0);
    parg_state_cullfaces(0);
    parg_shader_load_from_asset(SHADER_SIMPLE);
    float worldheight = worldwidth * sqrt(0.75);
    parg_zcam_init(worldwidth, worldheight, fovy);
    trimesh = parg_mesh_sierpinski(worldwidth, 10);
    printf("%d triangles\n", parg_mesh_ntriangles(trimesh));
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
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_varray_enable(
        parg_mesh_coord(trimesh), A_POSITION, 2, PARG_FLOAT, 0, 0);
    parg_draw_triangles(0, parg_mesh_ntriangles(trimesh));
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    parg_zcam_tick(winwidth / winheight, seconds);
    return 1;
}

void dispose()
{
    parg_shader_free(P_SIMPLE);
    parg_mesh_free(trimesh);
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
    parg_asset_preload(SHADER_SIMPLE);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_ontick(tick);
    parg_window_ondraw(draw);
    parg_window_onexit(dispose);
    parg_window_oninput(input);
    return parg_window_exec(185 * 5, 100 * 5, 1);
}
