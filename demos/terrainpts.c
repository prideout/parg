#include <par.h>
#include <parwin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define PAR_BLUENOISE_IMPLEMENTATION
#include <par/par_bluenoise.h>

#define DO_BAKE 0

#define TOKEN_TABLE(F)                    \
    F(P_SIMPLE, "p_simple")               \
    F(P_TEXTURED, "p_textured")           \
    F(A_POSITION, "a_position")           \
    F(A_TEXCOORD, "a_texcoord")           \
    F(U_MVP, "u_mvp")                     \
    F(U_EYEPOS, "u_eyepos")               \
    F(U_MAGNIFICATION, "u_magnification") \
    F(U_DENSITY, "u_density")             \
    F(U_POINTSIZE, "u_pointsize")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

#if DO_BAKE
#define ASSET_TABLE(F)                   \
    F(SHADER_SIMPLE, "terrainpts.glsl")  \
    F(TEXTURE_TERRAIN, "terrainpts.png") \
    F(BUFFER_BLUENOISE, "bluenoise.bin")
#else
#define ASSET_TABLE(F)                   \
    F(SHADER_SIMPLE, "terrainpts.glsl")  \
    F(TEXTURE_TERRAIN, "terrainpts.png") \
    F(BUFFER_TERRAIN, "terrainpts.bin")
#endif

ASSET_TABLE(PAR_TOKEN_DECLARE);

par_buffer* ptsvbo;
par_texture* terraintex;
par_mesh* backquad;
float pointscale = 1;
const float fovy = 16 * PAR_TWOPI / 180;
const float worldwidth = 1;
const int maxpts = 1400000;
const unsigned int ocean_color = 0xFFB2B283;

#define clamp(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define sqr(a) (a * a)

void init(float winwidth, float winheight, float pixratio)
{
    backquad = par_mesh_rectangle(1, 0.5);

#if DO_BAKE

    par_bluenoise_context* ctx;
    par_buffer* buffer;
    void* buffer_data;

    printf("Reading tiles...\n");
    buffer = par_buffer_slurp_asset(BUFFER_BLUENOISE, &buffer_data);
    assert(buffer_data);
    ctx = par_bluenoise_from_buffer(buffer_data, par_buffer_length(buffer), 0);
    par_buffer_free(buffer);

    printf("Pushing density function...\n");
    buffer = par_buffer_slurp_asset(TEXTURE_TERRAIN, &buffer_data);
    assert(buffer_data);
    par_bluenoise_density_from_color(
        ctx, buffer_data + 12, 4096, 2048, 4, ocean_color, 0);

    printf("Generating point sequence...\n");
    float* cpupts = par_bluenoise_generate_exact(ctx, maxpts, 3);
    ptsvbo = par_buffer_alloc(maxpts * 12, PAR_GPU_ARRAY);
    float* gpupts = par_buffer_lock(ptsvbo, PAR_WRITE);
    memcpy(gpupts, cpupts, par_buffer_length(ptsvbo));
    par_buffer_unlock(ptsvbo);

    par_buffer* filevbo = par_buffer_alloc(maxpts * sizeof(float) * 3, PAR_CPU);
    float* filepts = par_buffer_lock(filevbo, PAR_WRITE);
    memcpy(filepts, cpupts, par_buffer_length(filevbo));
    par_buffer_unlock(filevbo);
    par_buffer_to_file(filevbo, "terrainpts.bin");
    par_buffer_free(filevbo);
    par_bluenoise_free(ctx);

#else

    par_buffer* filevbo = par_buffer_from_asset(BUFFER_TERRAIN);
    ptsvbo = par_buffer_dup(filevbo, PAR_GPU_ARRAY);
    par_buffer_free(filevbo);

#endif

    terraintex = par_texture_from_asset(TEXTURE_TERRAIN);
    printf("%d points.\n", maxpts);
    par_state_clearcolor((Vector4){0.51, 0.7, 0.7, 1.0});
    par_state_depthtest(0);
    par_state_cullfaces(0);
    par_state_blending(1);
    par_shader_load_from_asset(SHADER_SIMPLE);
    float worldheight = worldwidth * sqrt(0.75);
    par_zcam_init(worldwidth, worldheight, fovy);
    par_zcam_grab_update(0.5, 0.5, 30.0);
}

void draw()
{
    Matrix4 view;
    Matrix4 projection;
    par_zcam_matrices(&projection, &view);
    Matrix4 model = M4MakeIdentity();

    Point3 eyepos;
    par_zcam_highprec(0, 0, &eyepos);

    Matrix4 modelview = M4Mul(view, model);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_draw_clear();
    par_texture_bind(terraintex, 0);

    par_shader_bind(P_TEXTURED);
    par_varray_enable(par_mesh_coord(backquad), A_POSITION, 2, PAR_FLOAT, 0, 0);
    par_varray_enable(par_mesh_uv(backquad), A_TEXCOORD, 2, PAR_FLOAT, 0, 0);
    par_uniform1f(U_MAGNIFICATION, par_zcam_get_magnification());
    par_uniform_matrix4f(U_MVP, &mvp);
    par_draw_one_quad();

    par_shader_bind(P_SIMPLE);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_uniform_point(U_EYEPOS, &eyepos);
    par_uniform1f(U_MAGNIFICATION, par_zcam_get_magnification());
    par_uniform1f(U_DENSITY, 0.1f);
    par_uniform1f(U_POINTSIZE, 20.0f * pointscale);
    par_varray_enable(ptsvbo, A_POSITION, 3, PAR_FLOAT, 0, 0);
    par_draw_points(maxpts);
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    pointscale = pixratio;
    par_zcam_tick(winwidth / winheight, seconds);
    return par_zcam_has_moved();
}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_shader_free(P_TEXTURED);
    par_buffer_free(ptsvbo);
    par_texture_free(terraintex);
    par_mesh_free(backquad);
}

void input(par_event evt, float x, float y, float z)
{
    switch (evt) {
    case PAR_EVENT_DOWN:
        par_zcam_grab_begin(x, y);
        break;
    case PAR_EVENT_UP:
        par_zcam_grab_update(x, y, z);
        par_zcam_grab_end();
        break;
    case PAR_EVENT_MOVE:
        par_zcam_grab_update(x, y, z);
        break;
    default:
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
    return par_window_exec(700, 350, 1);
}
