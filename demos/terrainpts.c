#include <parg.h>
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

TOKEN_TABLE(PARG_TOKEN_DECLARE);

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

ASSET_TABLE(PARG_TOKEN_DECLARE);

parg_buffer* ptsvbo;
parg_texture* terraintex;
parg_mesh* backquad;
float pointscale = 1;
const float fovy = 16 * PARG_TWOPI / 180;
const float worldwidth = 1;
const int maxpts = 1400000;
const unsigned int ocean_color = 0xFFB2B283;

#define clamp(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define sqr(a) (a * a)

void init(float winwidth, float winheight, float pixratio)
{
    backquad = parg_mesh_rectangle(1, 0.5);

#if DO_BAKE

    par_bluenoise_context* ctx;
    parg_buffer* buffer;
    void* buffer_data;

    printf("Reading tiles...\n");
    buffer = parg_buffer_slurp_asset(BUFFER_BLUENOISE, &buffer_data);
    assert(buffer_data);
    ctx = par_bluenoise_from_buffer(buffer_data, parg_buffer_length(buffer), 0);
    parg_buffer_free(buffer);

    printf("Pushing density function...\n");
    buffer = parg_buffer_slurp_asset(TEXTURE_TERRAIN, &buffer_data);
    assert(buffer_data);
    par_bluenoise_density_from_color(
        ctx, buffer_data + 12, 4096, 2048, 4, ocean_color, 0);

    printf("Generating point sequence...\n");
    float* cpupts = par_bluenoise_generate_exact(ctx, maxpts, 3);
    ptsvbo = parg_buffer_alloc(maxpts * 12, PARG_GPU_ARRAY);
    float* gpupts = parg_buffer_lock(ptsvbo, PARG_WRITE);
    memcpy(gpupts, cpupts, parg_buffer_length(ptsvbo));
    parg_buffer_unlock(ptsvbo);

    parg_buffer* filevbo =
        parg_buffer_alloc(maxpts * sizeof(float) * 3, PARG_CPU);
    float* filepts = parg_buffer_lock(filevbo, PARG_WRITE);
    memcpy(filepts, cpupts, parg_buffer_length(filevbo));
    parg_buffer_unlock(filevbo);
    parg_buffer_to_file(filevbo, "terrainpts.bin");
    parg_buffer_free(filevbo);
    par_bluenoise_free(ctx);

#else

    parg_buffer* filevbo = parg_buffer_from_asset(BUFFER_TERRAIN);
    ptsvbo = parg_buffer_dup(filevbo, PARG_GPU_ARRAY);
    parg_buffer_free(filevbo);

#endif

    terraintex = parg_texture_from_asset(TEXTURE_TERRAIN);
    printf("%d points.\n", maxpts);
    parg_state_clearcolor((Vector4){0.51, 0.7, 0.7, 1.0});
    parg_state_depthtest(0);
    parg_state_cullfaces(0);
    parg_state_blending(1);
    parg_shader_load_from_asset(SHADER_SIMPLE);
    float worldheight = worldwidth * sqrt(0.75);
    parg_zcam_init(worldwidth, worldheight, fovy);
    parg_zcam_grab_update(0.5, 0.5, 30.0);
}

void draw()
{
    Matrix4 view;
    Matrix4 projection;
    parg_zcam_matrices(&projection, &view);
    Matrix4 model = M4MakeIdentity();

    Point3 eyepos;
    parg_zcam_highprec(0, 0, &eyepos);

    Matrix4 modelview = M4Mul(view, model);
    Matrix4 mvp = M4Mul(projection, modelview);
    parg_draw_clear();
    parg_texture_bind(terraintex, 0);

    parg_shader_bind(P_TEXTURED);
    parg_varray_enable(
        parg_mesh_coord(backquad), A_POSITION, 2, PARG_FLOAT, 0, 0);
    parg_varray_enable(parg_mesh_uv(backquad), A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    parg_uniform1f(U_MAGNIFICATION, parg_zcam_get_magnification());
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_draw_one_quad();

    parg_shader_bind(P_SIMPLE);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_uniform_point(U_EYEPOS, &eyepos);
    parg_uniform1f(U_MAGNIFICATION, parg_zcam_get_magnification());
    parg_uniform1f(U_DENSITY, 0.1f);
    parg_uniform1f(U_POINTSIZE, 20.0f * pointscale);
    parg_varray_enable(ptsvbo, A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_draw_points(maxpts);
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    pointscale = pixratio;
    parg_zcam_tick(winwidth / winheight, seconds);
    return parg_zcam_has_moved();
}

void dispose()
{
    parg_shader_free(P_SIMPLE);
    parg_shader_free(P_TEXTURED);
    parg_buffer_free(ptsvbo);
    parg_texture_free(terraintex);
    parg_mesh_free(backquad);
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
    return parg_window_exec(700, 350, 1);
}
