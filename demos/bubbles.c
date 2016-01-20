#define PAR_BUBBLES_IMPLEMENTATION

#include <parg.h>
#include <parwin.h>
#include <stdio.h>
#include <par/par_bubbles.h>
#include <par/par_shapes.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(U_SEL, "u_sel")

TOKEN_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_TABLE(F) F(SHADER_SIMPLE, "bubbles.glsl")
ASSET_TABLE(PARG_TOKEN_DECLARE);

const float FOVY = 32 * PARG_TWOPI / 180;
const float WORLDWIDTH = 3;

#define NNODES 252
static int TREE[NNODES] = {
    0, 0, 1, 2, 2, 2, 2, 1, 7, 7, 7, 7, 7, 1, 13, 0, 15, 15, 15, 18, 18, 18, 18,
    18, 18, 18, 18, 18, 15, 15, 15, 15, 15, 15, 15, 15, 15, 0, 37, 38, 38, 38,
    38, 38, 37, 37, 37, 37, 37, 37, 0, 50, 50, 50, 50, 0, 55, 0, 57, 57, 57, 57,
    57, 57, 57, 57, 0, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
    66, 66, 66, 66, 66, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 66,
    66, 66, 66, 66, 66, 66, 66, 66, 66, 0, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 0, 139, 139, 139, 139, 139, 139, 139, 146, 146, 139, 139,
    139, 139, 152, 152, 152, 139, 139, 139, 158, 158, 158, 158, 139, 139, 139,
    139, 139, 0, 168, 169, 169, 169, 169, 169, 168, 175, 175, 175, 175, 175,
    175, 175, 175, 175, 175, 175, 168, 187, 187, 187, 187, 187, 187, 193, 193,
    193, 193, 187, 187, 187, 168, 201, 201, 201, 201, 168, 206, 206, 206, 168,
    210, 211, 211, 211, 210, 215, 215, 215, 215, 215, 210, 221, 221, 221, 210,
    210, 226, 226, 226, 210, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230,
    230, 230, 230, 230, 230, 210, 210, 210, 210, 210, 168,
};

struct {
    parg_mesh* disks;
} app;

void init(float winwidth, float winheight, float pixratio)
{
    parg_state_clearcolor((Vector4){0.5, 0.6, 0.7, 1.0});
    parg_state_depthtest(0);
    parg_state_cullfaces(1);
    parg_state_blending(1);
    parg_shader_load_from_asset(SHADER_SIMPLE);
    parg_zcam_init(WORLDWIDTH, WORLDWIDTH, FOVY);

    // Perform circle packing.
    par_bubbles_t* bubbles;
    bubbles = par_bubbles_hpack_circle(TREE, NNODES, 1.0);

    // Create template shape.
    float normal[3] = {0, 0, 1};
    float center[3] = {0, 0, 0};
    par_shapes_mesh* template = par_shapes_create_disk(1.0, 64, center, normal);
    template->points[2] = 0.5;

    // Merge each circle into the scene.
    par_shapes_mesh* scene = par_shapes_create_empty();
    double const* xyr = bubbles->xyr;
    for (int i = 0; i < bubbles->count; i++, xyr += 3) {
        par_shapes_mesh* shape = par_shapes_clone(template);
        par_shapes_scale(shape, xyr[2], xyr[2], 1.0);
        par_shapes_translate(shape, xyr[0], xyr[1], i);
        par_shapes_merge_and_free(scene, shape);
    }

    // Free the circle packer.
    par_bubbles_free_result(bubbles);

    // Create the vertex buffer.
    app.disks = parg_mesh_from_shape(scene);
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
    parg_uniform1f(U_SEL, 0);
    parg_varray_bind(parg_mesh_index(app.disks));
    parg_varray_enable(
        parg_mesh_coord(app.disks), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(app.disks));
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    parg_zcam_tick(winwidth / winheight, seconds);
    return parg_zcam_has_moved();
}

void dispose()
{
    parg_shader_free(P_SIMPLE);
    parg_mesh_free(app.disks);
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
    return parg_window_exec(600, 600, 1, 1);
}
