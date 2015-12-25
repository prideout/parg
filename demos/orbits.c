#include <parg.h>
#include <parwin.h>

#include <stdlib.h>

static const int WINSIZE = 256;
static const int BUFSIZE = 256;
static const int NASTEROIDS = 3;
static const int NPARTICLES = BUFSIZE * BUFSIZE;

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
    float previous_time;
    int request_physics;
} app;

void init(float winwidth, float winheight, float pixratio)
{
    parg_shader_load_from_asset(S_ORBITS);
    app.quadmesh = parg_mesh_rectangle(2, 2);
    app.bkgdtex = parg_texture_from_asset(T_BACKGROUND);
    app.asteroidtex = parg_texture_from_asset(T_ASTEROIDS);
    app.previous_time = -1;

    // Initialize gravitational sources (asteroids).
    int nbytes = sizeof(float) * 3 * NASTEROIDS;
    float *asteroids = malloc(nbytes), *psrc = asteroids;
    const float dtheta = PARG_TWOPI / NASTEROIDS;
    const float radius = 0.5;
    for (float i = 0, theta = 0; i < NASTEROIDS; i++, theta += dtheta) {
        *psrc++ = radius * cos(theta);
        *psrc++ = radius * sin(theta);
        *psrc++ = i;
    }
    app.asteroid_positions =
        parg_buffer_create(asteroids, nbytes, PARG_GPU_ARRAY);

    // Initialize a trivial "1 2 3" vertex buffer since OpenGL ES / WebGL
    // do not allow zero-vertex rendering, nor access to gl_Vertex.
    nbytes = sizeof(uint16_t) * NPARTICLES;
    uint16_t *inds = malloc(nbytes), *pinds = inds;
    for (int i = 0; i < NPARTICLES; i++) {
        *pinds++ = i;
    }
    app.particle_indices = parg_buffer_create(inds, nbytes, PARG_GPU_ARRAY);
    free(inds);

    // Initialize particle positions.
    nbytes = sizeof(float) * 4 * NPARTICLES;
    float* src = psrc = malloc(nbytes);
    float y = 1;
    for (int i = 0; i < NPARTICLES; i++) {
        float x = -1.0 + 2.0 * i / NPARTICLES;
        *psrc++ = x;
        *psrc++ = y;
        *psrc++ = x + 0.0025;
        *psrc++ = y;
    }
    app.particle_positionsa = parg_framebuffer_create(
        BUFSIZE, BUFSIZE, src, nbytes, PARG_FBO_FLOAT | PARG_FBO_ALPHA);
    app.particle_positionsb = parg_framebuffer_create_empty(
        BUFSIZE, BUFSIZE, PARG_FBO_FLOAT | PARG_FBO_ALPHA);

    // Initialize particle properties.
    psrc = src;
    float birth = 0;
    for (int i = 0; i < NPARTICLES; i++) {
        int gravindex = rand() % NASTEROIDS;
        *psrc++ = asteroids[gravindex * 3 + 0];
        *psrc++ = asteroids[gravindex * 3 + 1];
        *psrc++ = asteroids[gravindex * 3 + 2];
        *psrc++ = birth;
    }
    app.particle_properties = parg_framebuffer_create(
        BUFSIZE, BUFSIZE, src, nbytes, PARG_FBO_FLOAT | PARG_FBO_ALPHA);
    free(src);
    free(asteroids);
}

void draw()
{
    parg_buffer* pos = parg_mesh_coord(app.quadmesh);
    parg_buffer* uvs = parg_mesh_uv(app.quadmesh);

    parg_shader_bind(P_BACKGROUND);
    parg_varray_enable(pos, A_POSITION, 2, PARG_FLOAT, 0, 0);
    parg_varray_enable(uvs, A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    parg_texture_bind(app.bkgdtex, 0);
    parg_draw_one_quad();

    if (app.previous_time != -1) {
        // float delta = app.current_time - app.previous_time;
        float delta = 0.01666;
        parg_shader_bind(P_PHYSICS);
        parg_uniform1i(U_POSITIONS, 0);
        parg_uniform1i(U_PROPERTIES, 1);
        parg_uniform1f(U_DELTASQR, delta * delta);
        parg_framebuffer_bindtex(app.particle_positionsa, 0);
        parg_framebuffer_bindtex(app.particle_properties, 1);
        parg_framebuffer_pushfbo(app.particle_positionsb, 0);
        parg_draw_one_quad();
        parg_framebuffer_popfbo();
        parg_framebuffer_swap(app.particle_positionsa, app.particle_positionsb);
    }
    app.previous_time = app.current_time;
    parg_varray_disable(A_TEXCOORD);

    parg_shader_bind(P_ASTEROIDS);
    parg_state_blending(1);
    parg_uniform1f(U_TIME, app.current_time);
    parg_uniform1f(U_NPOINTS, NASTEROIDS);
    parg_varray_enable(app.asteroid_positions, A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_texture_bind(app.asteroidtex, 0);
    parg_draw_points(NASTEROIDS);

    parg_shader_bind(P_PARTICLES);
    parg_uniform1f(U_TIME, app.current_time);
    parg_uniform1f(U_NPOINTS, NPARTICLES);
    parg_varray_enable(app.particle_indices, A_POSITION, 1, PARG_USHORT, 0, 0);
    parg_framebuffer_bindtex(app.particle_positionsa, 0);
    parg_uniform1i(U_POSITIONS, 0);
    parg_draw_points(NPARTICLES);
    parg_state_blending(0);
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    app.current_time = seconds;
    return 1;
}

void dispose()
{
    parg_mesh_free(app.quadmesh);
    parg_buffer_free(app.asteroid_positions);
    parg_buffer_free(app.particle_indices);
    parg_texture_free(app.bkgdtex);
    parg_texture_free(app.asteroidtex);
    parg_framebuffer_free(app.particle_positionsa);
    parg_framebuffer_free(app.particle_positionsb);
    parg_framebuffer_free(app.particle_properties);
}

void input(parg_event evt, float x, float y, float z)
{
    char key = (char) x;
    switch (evt) {
    case PARG_EVENT_KEYPRESS:
        if (key == ' ') {}
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
    return parg_window_exec(WINSIZE, WINSIZE, 1, 0);
}
