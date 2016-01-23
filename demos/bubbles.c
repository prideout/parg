#define PAR_BUBBLES_IMPLEMENTATION

#include <parg.h>
#include <parwin.h>
#include <stdio.h>
#include <par/par_bubbles.h>
#include <par/par_shapes.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(A_CENTER, "a_center")     \
    F(U_MVP, "u_mvp")           \
    F(U_SEL, "u_sel")

TOKEN_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_TABLE(F) F(SHADER_SIMPLE, "bubbles.glsl")
ASSET_TABLE(PARG_TOKEN_DECLARE);

const float FOVY = 32 * PARG_TWOPI / 180;
const float WORLDWIDTH = 3;
const double DURATION = 0.5;
const int NNODES = 2e6;

struct {
    parg_mesh* disk;
    par_bubbles_t* bubbles;
    par_bubbles_t* culled;
    parg_buffer* centers;
    int hover;
    int potentially_clicking;
    double current_time;
    parg_zcam_animation camera_animation;
    float bbwidth;
    int* tree;
} app = {0};

void init(float winwidth, float winheight, float pixratio)
{
    parg_state_clearcolor((Vector4){0.5, 0.6, 0.7, 1.0});
    parg_state_depthtest(0);
    parg_state_cullfaces(1);
    parg_state_blending(1);
    parg_shader_load_from_asset(SHADER_SIMPLE);
    parg_zcam_init(WORLDWIDTH, WORLDWIDTH, FOVY);

    // Generate a random tree.  Note that we're squaring the random parent
    // pointers, which makes the graph distribute a bit more interesting, and
    // easies to find deep portions of the tree to dive into.
    puts("Generating tree...");
    app.tree = malloc(sizeof(int) * NNODES);
    srand(1);
    app.tree[0] = 0;
    for (int i = 1; i < NNODES; i++) {
        float a = (float) rand() / RAND_MAX;
        float b = (float) rand() / RAND_MAX;
        app.tree[i] = i * a * b;
    }

    // Perform circle packing.
    puts("Packing circles...");
    app.bubbles = par_bubbles_hpack_circle(app.tree, NNODES, 1.0);
    app.hover = -1;

    // Create template shape.
    float normal[3] = {0, 0, 1};
    float center[3] = {0, 0, 1};
    par_shapes_mesh* template = par_shapes_create_disk(1.0, 64, center, normal);
    template->points[2] = 0;

    // Create the VBO that will vary on a per-instance basis. We re-populate it
    // on every frame, growing it if necessary.  The starting size doesn't
    // matter much.
     app.centers = parg_buffer_alloc(512 * 4 * sizeof(float), PARG_GPU_ARRAY);

    // Create the vertex buffer.
    app.disk = parg_mesh_from_shape(template);
    par_shapes_free_mesh(template);
    puts("Ready to draw.");
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
    parg_uniform1f(U_SEL, app.hover);
    parg_varray_bind(parg_mesh_index(app.disk));
    parg_varray_enable(
        parg_mesh_coord(app.disk), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_instances(A_CENTER, 1);
    parg_varray_enable(app.centers, A_CENTER, 4, PARG_FLOAT, 0, 0);

    double aabb[4];
    parg_zcam_get_viewportd(aabb);
    double minradius = 4.0 * (aabb[2] - aabb[0]) / app.bbwidth;
    app.culled = par_bubbles_cull(app.bubbles, aabb, minradius, app.culled);
    int nbytes = app.culled->count * 4 * sizeof(float);
    float* fdisk = parg_buffer_lock_grow(app.centers, nbytes);
    double const* ddisk = app.culled->xyr;
    for (int i = 0; i < app.culled->count; i++, fdisk += 4, ddisk += 3) {
        fdisk[0] = ddisk[0];
        fdisk[1] = ddisk[1];
        fdisk[2] = ddisk[2];
        fdisk[3] = app.culled->ids[i];
    }
    parg_buffer_unlock(app.centers);
    parg_draw_instanced_triangles_u16(
        0, parg_mesh_ntriangles(app.disk), app.culled->count);
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    app.current_time = seconds;
    app.bbwidth = winwidth * pixratio;
    parg_zcam_animation anim = app.camera_animation;
    if (anim.start_time > 0) {
        double duration = anim.final_time - anim.start_time;
        double t = (app.current_time - anim.start_time) / duration;
        t = PARG_CLAMP(t, 0, 1);
        parg_zcam_blend(anim.start_view, anim.final_view, anim.blend_view, t);
        parg_zcam_frame_position(anim.blend_view);
        if (t == 1.0) {
            app.camera_animation.start_time = 0;
        }
    }
    parg_zcam_tick(winwidth / winheight, seconds);
    return parg_zcam_has_moved();
}

void dispose()
{
    parg_shader_free(P_SIMPLE);
    parg_mesh_free(app.disk);
    par_bubbles_free_result(app.bubbles);
    par_bubbles_free_result(app.culled);
    parg_buffer_free(app.centers);
    free(app.tree);
}

void input(parg_event evt, float x, float y, float z)
{
    DPoint3 p = parg_zcam_to_world(x, y);
    int previous = app.hover;
    switch (evt) {
    case PARG_EVENT_DOWN:
        app.potentially_clicking = 1;
        parg_zcam_grab_begin(x, y);
        break;
    case PARG_EVENT_UP:
        parg_zcam_grab_update(x, y, z);
        parg_zcam_grab_end();
        if (app.potentially_clicking == 1) {
            int i = par_bubbles_pick(app.bubbles, p.x, p.y);
            if (i > -1) {
                parg_aar view = parg_zcam_get_rectangle();
                double const* xyr = app.bubbles->xyr + i * 3;
                app.camera_animation.start_time = app.current_time;
                app.camera_animation.final_time =
                    app.current_time + DURATION;
                app.camera_animation.start_view[0] = parg_aar_centerx(view);
                app.camera_animation.start_view[1] = parg_aar_centery(view);
                app.camera_animation.start_view[2] = parg_aar_width(view);
                app.camera_animation.final_view[0] = xyr[0];
                app.camera_animation.final_view[1] = xyr[1];
                app.camera_animation.final_view[2] = xyr[2] * 3;
            }
        }
        app.potentially_clicking = 0;
        break;
    case PARG_EVENT_MOVE:
        app.potentially_clicking = 0;
        app.hover = par_bubbles_pick(app.bubbles, p.x, p.y);
        parg_zcam_grab_update(x, y, z);
        break;
    default:
        break;
    }
    if (app.hover != previous && app.hover > 0) {
        #ifndef EMSCRIPTEN
        printf("%7d ", app.hover);
        if (app.culled) {
            printf("%4d / %d", app.culled->count, app.bubbles->count);
        }
        puts("");
        #endif
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
    return parg_window_exec(400, 400, 1, 1);
}
