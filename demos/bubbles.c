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

struct {
    parg_mesh* disks;
    par_bubbles_t* bubbles;
    par_bubbles_t* culled;
    parg_buffer* centers;
    int hover;
    int potentially_clicking;
    double current_time;
    parg_zcam_animation camera_animation;
} app = {0};

void init(float winwidth, float winheight, float pixratio)
{
    parg_state_clearcolor((Vector4){0.5, 0.6, 0.7, 1.0});
    parg_state_depthtest(0);
    parg_state_cullfaces(1);
    parg_state_blending(1);
    parg_shader_load_from_asset(SHADER_SIMPLE);
    parg_zcam_init(WORLDWIDTH, WORLDWIDTH, FOVY);

    // Perform circle packing.
    int nnodes = 1e4;
    int* tree = malloc(sizeof(int) * nnodes);
    srand(1);
    tree[0] = 0;
    for (int i = 1; i < nnodes; i++) {
        tree[i] = i * (float) rand() / RAND_MAX;
    }
    app.bubbles = par_bubbles_hpack_circle(tree, nnodes, 1.0);
    app.hover = -1;
    printf("%d nodes have been arranged.\n", nnodes);

    // Create template shape.
    float normal[3] = {0, 0, 1};
    float center[3] = {0, 0, 1};
    par_shapes_mesh* template = par_shapes_create_disk(1.0, 64, center, normal);
    template->points[2] = 0;

    // Create the VBO that will vary on a per-instance basis.
    app.centers = parg_buffer_alloc(nnodes * 4 * sizeof(float), PARG_GPU_ARRAY);
    float* fdisk = parg_buffer_lock(app.centers, PARG_WRITE);
    double const* ddisk = app.bubbles->xyr;
    for (int i = 0; i < nnodes; i++, fdisk += 4, ddisk += 3) {
        fdisk[0] = ddisk[0];
        fdisk[1] = ddisk[1];
        fdisk[2] = ddisk[2];
        fdisk[3] = i;
    }
    parg_buffer_unlock(app.centers);

    // Create the vertex buffer.
    app.disks = parg_mesh_from_shape(template);
    par_shapes_free_mesh(template);
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
    parg_varray_bind(parg_mesh_index(app.disks));
    parg_varray_enable(
        parg_mesh_coord(app.disks), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_instances(A_CENTER, 1);
    parg_varray_enable(app.centers, A_CENTER, 4, PARG_FLOAT, 0, 0);
    parg_draw_instanced_triangles_u16(
        0, parg_mesh_ntriangles(app.disks), app.bubbles->count);
    parg_varray_disable(A_CENTER);
    parg_varray_instances(A_CENTER, 0);
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    app.current_time = seconds;
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
    parg_mesh_free(app.disks);
    par_bubbles_free_result(app.bubbles);
    parg_buffer_free(app.centers);
}

void input(parg_event evt, float x, float y, float z)
{
    DPoint3 p = parg_zcam_to_world(x, y);
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
