#include <parg.h>
#include <parwin.h>

static const int WINSIZE = 512;
static const int BUFSIZE = 256;

#define TOKEN_TABLE(F)              \
    F(P_BACKGROUND, "p_background") \
    F(A_POSITION, "a_position")     \
    F(A_TEXCOORD, "a_texcoord")     \
    F(U_TIME, "u_time")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_TABLE(F)                  \
    F(T_BACKGROUND, "blurrycolors.png") \
    F(T_ASTEROIDS, "asteroids.png")     \
    F(S_ORBITS, "orbits.glsl")
ASSET_TABLE(PARG_TOKEN_DECLARE);

struct {
    parg_texture* bkgdtex;
    parg_texture* asteroidtex;
    parg_mesh* quadmesh;
} app;

void init(float winwidth, float winheight, float pixratio)
{
    parg_shader_load_from_asset(S_ORBITS);
    app.quadmesh = parg_mesh_rectangle(2, 2);
    app.bkgdtex = parg_texture_from_asset(T_BACKGROUND);
    app.asteroidtex = parg_texture_from_asset(T_ASTEROIDS);
}

void draw()
{
    parg_shader_bind(P_BACKGROUND);
    parg_buffer* pos = parg_mesh_coord(app.quadmesh);
    parg_buffer* uvs = parg_mesh_uv(app.quadmesh);
    parg_varray_enable(pos, A_POSITION, 2, PARG_FLOAT, 0, 0);
    parg_varray_enable(uvs, A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    parg_texture_bind(app.bkgdtex, 0);
    parg_draw_one_quad();
}

int tick(float seconds, float winwidth, float winheight, float pixratio)
{
    return 1;
}

void dispose()
{
    parg_mesh_free(app.quadmesh);
    parg_texture_free(app.bkgdtex);
    parg_texture_free(app.asteroidtex);
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
    return parg_window_exec(WINSIZE, WINSIZE, 1);
}
