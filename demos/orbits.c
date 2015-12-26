#include <parg.h>
#include <parwin.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static const int WINSIZE = 256;
static const int NASTEROIDS = 3;

#define TOKEN_TABLE(F)              \
    F(P_BACKGROUND, "p_background") \
    F(P_ASTEROIDS, "p_asteroids")   \
    F(P_PARTICLES, "p_particles")   \
    F(P_PHYSICS, "p_physics")       \
    F(A_POSITION, "a_position")     \
    F(A_TEXCOORD, "a_texcoord")     \
    F(A_INDEX, "a_index")           \
    F(U_NPOINTS, "u_npoints")       \
    F(U_POSITIONS, "u_positions")   \
    F(U_PROPERTIES, "u_properties") \
    F(U_DELTASQR, "u_deltasqr")     \
    F(U_BUFSIZE, "u_bufsize")       \
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
    parg_buffer* asteroid_positions;
    parg_buffer* particle_indices;
    parg_framebuffer* particle_positionsa;
    parg_framebuffer* particle_positionsb;
    parg_framebuffer* particle_properties;
    float current_time;
    int request_physics;
    int playing;
    float* asteroids;
    int bufsize;
    int nparticles;
} app;

static void dispose_particles()
{
    parg_buffer_free(app.particle_indices);
    parg_framebuffer_free(app.particle_positionsa);
    parg_framebuffer_free(app.particle_positionsb);
    parg_framebuffer_free(app.particle_properties);
}

static void create_particles()
{
    app.nparticles = app.bufsize * app.bufsize;

    // Initialize a trivial "1 2 3" vertex buffer since OpenGL ES / WebGL
    // do not allow zero-vertex rendering, nor access to gl_Vertex.
    int nbytes = sizeof(float) * app.nparticles;
    float *inds = malloc(nbytes), *pinds = inds;
    for (int i = 0; i < app.nparticles; i++) {
        *pinds++ = i;
    }
    app.particle_indices = parg_buffer_create(inds, nbytes, PARG_GPU_ARRAY);
    free(inds);

    // Initialize particle positions.
    nbytes = sizeof(float) * 4 * app.nparticles;
    float *src = malloc(nbytes), *psrc = src;
    float y = 1;
    float d = 2.0 / (app.nparticles - 1);
    float vx = 0.0025;
    for (int i = 0; i < app.nparticles; i++) {
        float x = -1.0 + i * d;
        *psrc++ = x;
        *psrc++ = y;
        *psrc++ = x + vx;
        *psrc++ = y;
    }
    int res = app.bufsize;
    app.particle_positionsa = parg_framebuffer_create(
        res, res, src, nbytes, PARG_FBO_FLOAT | PARG_FBO_ALPHA);
    app.particle_positionsb = parg_framebuffer_create_empty(
        res, res, PARG_FBO_FLOAT | PARG_FBO_ALPHA);

    // Initialize particle properties.
    psrc = src;
    float birth = 0;
    for (int i = 0; i < app.nparticles; i++) {
        int gravindex = i % NASTEROIDS;
        *psrc++ = app.asteroids[gravindex * 3 + 0];
        *psrc++ = app.asteroids[gravindex * 3 + 1];
        *psrc++ = app.asteroids[gravindex * 3 + 2];
        *psrc++ = birth;
    }
    app.particle_properties = parg_framebuffer_create(
        res, res, src, nbytes, PARG_FBO_FLOAT | PARG_FBO_ALPHA);
    free(src);
}

static void message(const char* msg)
{
    if (!strcmp(msg, "play")) {
        app.playing = 1;
    } else if (!strcmp(msg, "pause")) {
        app.playing = 0;
    } else {
        app.bufsize = atoi(msg);
        printf("Switching to %dx%d (%d)\n", app.bufsize, app.bufsize,
            app.nparticles);
    }
}

static void draw()
{
    const float TIMESTEP = 0.01666;

    if (app.nparticles != app.bufsize * app.bufsize) {
        dispose_particles();
        create_particles();
    }

    parg_buffer* pos = parg_mesh_coord(app.quadmesh);
    parg_buffer* uvs = parg_mesh_uv(app.quadmesh);

    parg_shader_bind(P_BACKGROUND);
    parg_varray_enable(pos, A_POSITION, 2, PARG_FLOAT, 0, 0);
    parg_varray_enable(uvs, A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    parg_texture_bind(app.bkgdtex, 0);
    parg_draw_one_quad();

    parg_shader_bind(P_PHYSICS);
    parg_uniform1i(U_POSITIONS, 0);
    parg_uniform1i(U_PROPERTIES, 1);
    parg_uniform1f(U_DELTASQR, TIMESTEP * TIMESTEP);
    parg_framebuffer_bindtex(app.particle_positionsa, 0);
    parg_framebuffer_bindtex(app.particle_properties, 1);
    parg_framebuffer_pushfbo(app.particle_positionsb, 0);
    parg_draw_one_quad();
    parg_framebuffer_popfbo();
    parg_framebuffer_swap(app.particle_positionsa, app.particle_positionsb);
    parg_varray_disable(A_TEXCOORD);

    parg_shader_bind(P_ASTEROIDS);
    parg_state_blending(1);
    parg_uniform1f(U_TIME, app.current_time);
    parg_uniform1f(U_NPOINTS, NASTEROIDS);
    parg_varray_enable(app.asteroid_positions, A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_texture_bind(app.asteroidtex, 0);
    parg_draw_points(NASTEROIDS);

    parg_shader_bind(P_PARTICLES);
    parg_state_blending(2);
    parg_uniform1f(U_TIME, app.current_time);
    parg_uniform1f(U_NPOINTS, app.nparticles);
    parg_varray_enable(app.particle_indices, A_POSITION, 1, PARG_FLOAT, 0, 0);
    parg_framebuffer_bindtex(app.particle_positionsa, 0);
    parg_uniform1i(U_POSITIONS, 0);
    parg_uniform1f(U_BUFSIZE, app.bufsize);
    parg_draw_points(app.nparticles);
    parg_state_blending(0);
}

static void init(float winwidth, float winheight, float pixratio)
{
    parg_shader_load_from_asset(S_ORBITS);
    app.quadmesh = parg_mesh_rectangle(2, 2);
    app.bkgdtex = parg_texture_from_asset(T_BACKGROUND);
    app.asteroidtex = parg_texture_from_asset(T_ASTEROIDS);

    // Initialize gravitational sources (asteroids).
    int nbytes = sizeof(float) * 3 * NASTEROIDS;
    float *asteroids = app.asteroids = malloc(nbytes), *psrc = asteroids;
    const float dtheta = PARG_TWOPI / NASTEROIDS;
    const float radius = 0.5;
    for (float i = 0, theta = 0; i < NASTEROIDS; i++, theta += dtheta) {
        *psrc++ = radius * cos(theta);
        *psrc++ = radius * sin(theta);
        *psrc++ = i;
    }
    app.asteroid_positions =
        parg_buffer_create(asteroids, nbytes, PARG_GPU_ARRAY);

    create_particles();
    draw();
}

static void dispose()
{
    parg_mesh_free(app.quadmesh);
    parg_texture_free(app.bkgdtex);
    parg_texture_free(app.asteroidtex);
    parg_buffer_free(app.asteroid_positions);
    dispose_particles();
    free(app.asteroids);
}

static void input(parg_event evt, float x, float y, float z)
{
    char key = (char) x;
    switch (evt) {
    case PARG_EVENT_KEYPRESS:
        if (key == ' ') {
            switch (app.bufsize) {
            case 256:
                message("512");
                break;
            case 512:
                message("1024");
                break;
            case 1024:
                message("256");
                break;
            }
        }
        break;
    default:
        break;
    }
}

static int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    app.current_time = seconds;
    return app.playing;
}

int main(int argc, char* argv[])
{
    app.bufsize = 512;
    app.playing = 1;
    TOKEN_TABLE(PARG_TOKEN_DEFINE);
    ASSET_TABLE(PARG_ASSET_TABLE);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_ontick(tick);
    parg_window_ondraw(draw);
    parg_window_onexit(dispose);
    parg_window_oninput(input);
    parg_window_onmessage(message);
    return parg_window_exec(WINSIZE, WINSIZE, 1, 0);
}
