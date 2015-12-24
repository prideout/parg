#include <parg.h>
#include <parwin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAR_BLUENOISE_IMPLEMENTATION
#include <par/par_bluenoise.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(U_DENSITY, "u_density")   \
    F(U_POINTSIZE, "u_pointsize")

TOKEN_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_TABLE(F)                  \
    F(SHADER_SIMPLE, "trillium.glsl")   \
    F(TEXTURE_TRILLIUM, "trillium.png") \
    F(BUFFER_BLUENOISE, "bluenoise.trimmed.bin")
ASSET_TABLE(PARG_TOKEN_DECLARE);

parg_buffer* ptsvbo;
par_bluenoise_context* ctx;
float pointscale = 1;
const float gray = 0.8;
const float fovy = 16 * PARG_TWOPI / 180;
const float worldwidth = 1;
const int maxpts = 100000;

#define clamp(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define sqr(a) (a * a)

void init(float winwidth, float winheight, float pixratio)
{
    parg_buffer* buffer;
    void* buffer_data;

    buffer = parg_buffer_slurp_asset(BUFFER_BLUENOISE, &buffer_data);
    ctx = par_bluenoise_from_buffer(
        buffer_data, parg_buffer_length(buffer), maxpts);
    parg_buffer_free(buffer);

    buffer = parg_buffer_slurp_asset(TEXTURE_TRILLIUM, &buffer_data);
    par_bluenoise_density_from_gray(ctx, buffer_data + 12, 3500, 3500, 4);
    parg_buffer_free(buffer);

    ptsvbo = parg_buffer_alloc(maxpts * sizeof(float) * 3, PARG_GPU_ARRAY);
    parg_state_clearcolor((Vector4){gray, gray, gray, 1});
    parg_state_depthtest(0);
    parg_state_cullfaces(0);
    parg_shader_load_from_asset(SHADER_SIMPLE);
    float worldheight = worldwidth;
    parg_zcam_init(worldwidth, worldheight, fovy);
}

void draw()
{
    float lbrt[4];
    parg_zcam_get_viewport(lbrt);
    float left = lbrt[0];
    float bottom = lbrt[1];
    float right = lbrt[2];
    float top = lbrt[3];
    par_bluenoise_set_viewport(ctx, left, bottom, right, top);

    int npts;
    float* cpupts = par_bluenoise_generate(ctx, 40000, &npts);
    float* gpupts = parg_buffer_lock(ptsvbo, PARG_WRITE);
    memcpy(gpupts, cpupts, npts * 3 * sizeof(float));
    parg_buffer_unlock(ptsvbo);

    Matrix4 view;
    Matrix4 projection;
    parg_zcam_matrices(&projection, &view);
    Matrix4 model = M4MakeIdentity();
    Matrix4 modelview = M4Mul(view, model);
    Matrix4 mvp = M4Mul(projection, modelview);
    parg_draw_clear();
    parg_shader_bind(P_SIMPLE);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_uniform1f(U_POINTSIZE, 2.5f * pointscale);
    parg_varray_enable(ptsvbo, A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_draw_points(npts);
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
    parg_buffer_free(ptsvbo);
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
    return parg_window_exec(700, 350, 1, 1);
}
