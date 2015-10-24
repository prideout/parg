#include <par.h>
#include <parwin.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lodepng.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(U_EYEPOS, "u_eyepos")     \
    F(U_EYEPOS_LOWPART, "u_eyepos_lowpart")

TOKEN_TABLE(PAR_TOKEN_DECLARE);

#define ASSET_TABLE(F)                  \
    F(SHADER_TRILLIUM, "trillium.glsl") \
    F(TEXTURE_TRILLIUM, "trillium.png") \
    F(BUFFER_BLUENOISE, "bluenoise.bin")
ASSET_TABLE(PAR_TOKEN_DECLARE);

#define clampi(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define sqri(a) (a * a)

static void drawPoints(par_bluenoise_context* ctx, const char* dstfile,
    float vx, float vy, float vz)
{
    int numPoints;
    float* pts = par_bluenoise_generate(ctx, vx, vy, vz, &numPoints);
    const int size = 512;
    unsigned char* buffer = malloc(sqri(size));
    for (int i = 0; i < sqri(size); i++) {
        buffer[i] = 255;
    }
    while (numPoints--) {
        float x = (*pts++ - vx) / vz;
        float y = (*pts++ - vy) / vz;
        int i = clampi(x * size, 0, size - 1);
        int j = clampi(y * size, 0, size - 1);
        assert(i >= 0 && i < size);
        assert(j >= 0 && j < size);
        buffer[i + j * size] = 0;
    }
    lodepng_encode_file(dstfile, buffer, size, size, LCT_GREY, 8);
    free(buffer);
    printf("Write %s\n", dstfile);
}

void init(float winwidth, float winheight, float pixratio)
{
    par_bluenoise_context* ctx;
    int dims[3];
    unsigned char* data;
    par_buffer* buffer;
    void* buffer_data;

    buffer = par_buffer_from_asset(BUFFER_BLUENOISE);
    buffer_data = par_buffer_lock(buffer, PAR_READ);
    ctx = par_bluenoise_create(buffer_data, par_buffer_length(buffer));
    par_buffer_unlock(buffer);
    par_buffer_free(buffer);

	buffer = par_buffer_from_asset(TEXTURE_TRILLIUM);
    buffer_data = par_buffer_lock(buffer, PAR_READ);
	lodepng_decode_memory(&data, (unsigned*) &dims[0], (unsigned*) &dims[1],
		buffer_data, par_buffer_length(buffer), LCT_GREY, 8);
    par_buffer_unlock(buffer);
    par_buffer_free(buffer);

    assert(dims[0] == dims[1]);
    par_bluenoise_set_density(ctx, data, dims[0]);
    free(data);
    drawPoints(ctx, "build/output_01.png", 0, 0, 1);
    drawPoints(ctx, "build/output_02.png", 0.25f, 0.25f, 0.5f);
    drawPoints(ctx, "build/output_03.png", 0.45f, 0.45f, 0.1f);
    par_bluenoise_free(ctx);
}

int draw() { return 1; }

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    ASSET_TABLE(PAR_ASSET_TABLE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_ondraw(draw);
    return par_window_exec(400, 300, 1);
}
