#include <par.h>
#include <parwin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_TABLE(F)                    \
    F(P_SIMPLE, "p_simple")               \
    F(P_HALO, "p_halo")                   \
    F(A_POSITION, "a_position")           \
    F(U_MVP, "u_mvp")                     \
    F(U_EYEPOS, "u_eyepos")               \
    F(U_MAGNIFICATION, "u_magnification") \
    F(U_DENSITY, "u_density")             \
    F(U_POINTSIZE, "u_pointsize")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

#define ASSET_TABLE(F)                   \
    F(SHADER_SIMPLE, "terrainpts.glsl")  \
    F(TEXTURE_TERRAIN, "terrainpts.png") \
    F(BUFFER_BLUENOISE, "bluenoise.bin")
ASSET_TABLE(PAR_TOKEN_DECLARE);

par_buffer* ptsvbo;
par_buffer* vidvbo;
par_texture* terraintex;
const float fovy = 16 * PAR_TWOPI / 180;
const float worldwidth = 1;
const int maxpts = 100 * 1024 * 1024;
const unsigned int ocean_color = 0xFFB2B283;

#define clamp(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define sqr(a) (a * a)

void init(float winwidth, float winheight, float pixratio)
{
    par_bluenoise_context* ctx;
    par_buffer* buffer;
    void* buffer_data;

    printf("Reading tiles...\n");
    buffer = par_buffer_slurp_asset(BUFFER_BLUENOISE, &buffer_data);
    ctx = par_bluenoise_create(buffer_data, par_buffer_length(buffer), maxpts);
    par_buffer_free(buffer);

    printf("Pushing density function...\n");
    buffer = par_buffer_slurp_asset(TEXTURE_TERRAIN, &buffer_data);
    par_bluenoise_density_from_color(
        ctx, buffer_data + 12, 4096, 2048, 4, ocean_color, 0);
    terraintex = par_texture_from_asset(TEXTURE_TERRAIN);

    printf("Generating point sequence...\n");
    int npts;
    float* cpupts =
        par_bluenoise_generate(ctx, 20000000, -.5, -.5, .5, .5, &npts);
    par_bluenoise_sort_by_rank(cpupts, npts);
    ptsvbo = par_buffer_alloc(npts * sizeof(float) * 3, PAR_GPU_ARRAY);
    float* gpupts = par_buffer_lock(ptsvbo, PAR_WRITE);
    memcpy(gpupts, cpupts, par_buffer_length(ptsvbo));
    par_buffer_unlock(ptsvbo);

    printf("Generating vertex ids...\n");
    vidvbo = par_buffer_alloc(npts * sizeof(float), PAR_GPU_ARRAY);
    float* gpuids = par_buffer_lock(vidvbo, PAR_WRITE);
    for (int i = 0; i < npts; i++) {
        gpuids[i] = i;
    }
    par_buffer_unlock(vidvbo);

    printf("%d points.\n", npts);
    par_state_clearcolor((Vector4){0.51, 0.7, 0.7, 1.0});
    par_state_depthtest(0);
    par_state_cullfaces(0);
    par_state_blending(1);
    par_shader_load_from_asset(SHADER_SIMPLE);
    float worldheight = worldwidth * sqrt(0.75);
    par_zcam_init(worldwidth, worldheight, fovy);
    par_bluenoise_free(ctx);
}

int draw()
{
    int npts = par_buffer_length(ptsvbo) / (sizeof(float) * 3);
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

    par_shader_bind(P_HALO);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_uniform_point(U_EYEPOS, &eyepos);
    par_uniform1f(U_MAGNIFICATION, par_zcam_get_magnification());
    par_uniform1f(U_DENSITY, 0.01f);
    par_uniform1f(U_POINTSIZE, 40.0f);
    par_varray_enable(ptsvbo, A_POSITION, 3, PAR_FLOAT, 0, 0);
    par_draw_points(npts);

    par_shader_bind(P_SIMPLE);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_uniform_point(U_EYEPOS, &eyepos);
    par_uniform1f(U_MAGNIFICATION, par_zcam_get_magnification());
    par_uniform1f(U_DENSITY, 0.01f);
    par_uniform1f(U_POINTSIZE, 10.0f);
    par_varray_enable(ptsvbo, A_POSITION, 3, PAR_FLOAT, 0, 0);
    par_draw_points(npts);

    return 1;
}

void tick(float winwidth, float winheight, float pixratio, float seconds)
{
    par_zcam_tick(winwidth / winheight, seconds);
}

void dispose()
{
    par_shader_free(P_SIMPLE);
    par_shader_free(P_HALO);
    par_buffer_free(ptsvbo);
    par_buffer_free(vidvbo);
    par_texture_free(terraintex);
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
