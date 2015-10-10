#include <par.h>
#include <parwin.h>
#include <assert.h>
#include <stdio.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(SHADER_SIMPLE, "sierpinski.glsl")
TOKEN_TABLE(PAR_TOKEN_DECLARE);

par_mesh* trimesh;
const float gray = 0.8;
const float fovy = 16 * PAR_TWOPI / 180;
const float worldwidth = 6000;

void init(float winwidth, float winheight, float pixratio)
{
    par_state_clearcolor((Vector4){gray, gray, gray, 1});
    par_state_depthtest(0);
    par_state_cullfaces(0);
    par_shader_load_from_asset(SHADER_SIMPLE);
    float worldheight = worldwidth * sqrt(0.75);
    par_zcam_init(worldwidth, worldheight, fovy);
    trimesh = par_mesh_create_sierpinski(worldwidth, 10);
    printf("%d triangles\n", par_mesh_ntriangles(trimesh));
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
    par_uniform_matrix4f(U_MVP, &mvp);
    par_varray_enable(par_mesh_coord(trimesh), A_POSITION, 2, PAR_FLOAT, 0, 0);
    par_draw_triangles(0, par_mesh_ntriangles(trimesh));
    return 1;
}

void tick(float winwidth, float winheight, float pixratio, float seconds)
{
    par_zcam_tick(winwidth / winheight, seconds);
}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_mesh_free(trimesh);
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
    par_asset_preload(SHADER_SIMPLE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ontick(tick);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    par_window_oninput(input);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
