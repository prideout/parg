#define PAR_BUBBLES_IMPLEMENTATION

#include <parg.h>
#include <parwin.h>
#include <pa.h>
#include <stdio.h>
#include <par/par_bubbles.h>
#include <par/par_shapes.h>

#define TOKEN_TABLE(F)          \
    F(P_SIMPLE, "p_simple")     \
    F(P_POINTS, "p_points")     \
    F(A_POSITION, "a_position") \
    F(U_MVP, "u_mvp")           \
    F(U_COLOR, "u_color")       \
    F(U_ALPHA, "u_alpha")       \
    F(U_POINTSIZE, "u_pointsize")

TOKEN_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_TABLE(F) F(SHADER_SIMPLE, "dragpts.glsl")
ASSET_TABLE(PARG_TOKEN_DECLARE);

const float FOVY = 32 * PARG_TWOPI / 180;
const float WORLDWIDTH = 3;
const double INNERRAD = 0.1;
const double OUTERRAD = 0.15;

struct {
    parg_mesh* disk;
    double* inner_disks;
    double* outer_disks;
    double* enclosing_disk;
    float pointscale;
    int inner_drag;
    int inner_hover;
    int outer_drag;
    int outer_hover;
    float dragx;
    float dragy;
    float dragr;
    int potentially_clicking;
    enum { ENCLOSE_PTS, ENCLOSE_DISKS } mode;
} app = {0};

void update_enclosing_disk();

void init(float winwidth, float winheight, float pixratio)
{
    app.mode = ENCLOSE_PTS;

    parg_state_clearcolor((Vector4){0.5, 0.6, 0.7, 1.0});
    parg_state_depthtest(0);
    parg_state_cullfaces(1);
    parg_state_blending(1);
    parg_shader_load_from_asset(SHADER_SIMPLE);
    parg_zcam_init(WORLDWIDTH, WORLDWIDTH, FOVY);

    // Create template shape.
    float normal[3] = {0, 0, 1};
    float center[3] = {0, 0, 0};
    par_shapes_mesh* disk = par_shapes_create_disk(1.0, 64, center, normal);
    disk->points[2] = 0.5;
    app.disk = parg_mesh_from_shape(disk);
    par_shapes_free_mesh(disk);

    // Initialize the array with two circles.
    app.inner_disks = 0;
    pa_push3(app.inner_disks, 0, 0, INNERRAD);
    pa_push3(app.inner_disks, 1, 0, INNERRAD);
    app.outer_disks = 0;
    pa_push3(app.outer_disks, 0, 0, OUTERRAD);
    pa_push3(app.outer_disks, 1, 0, OUTERRAD);
    app.inner_drag = app.inner_hover = -1;
    app.outer_drag = app.outer_hover = -1;

    // Create space for the enclosing disk.
    pa_push3(app.enclosing_disk, 0, 0, 1);
    pa_remove3(app.enclosing_disk, 0);
    update_enclosing_disk();
}

void draw()
{
    Vector3 yellow = {0.9, 1, 0.1};
    Vector3 red = {1, 0.4, 0.3};
    Vector3 blue = {0.5, 0.6, 1.0};
    Vector3 white = {1, 1, 1};

    Matrix4 view;
    Matrix4 projection;
    parg_zcam_matrices(&projection, &view);
    parg_draw_clear();
    parg_varray_bind(parg_mesh_index(app.disk));
    parg_varray_enable(
        parg_mesh_coord(app.disk), A_POSITION, 3, PARG_FLOAT, 0, 0);

    parg_shader_bind(P_SIMPLE);
    for (double const* xyr = app.outer_disks; xyr != pa_end(app.outer_disks);
        xyr += 3) {
        float x = xyr[0], y = xyr[1], r = xyr[2];
        Matrix4 mvp = M4Mul(
            projection, M4Mul(view, M4Mul(M4MakeTranslation((Vector3){x, y, 0}),
            M4MakeScale((Vector3){r, r, r}))));
        parg_uniform_matrix4f(U_MVP, &mvp);
        int index = (xyr - app.outer_disks) / 3;
        parg_uniform3f(U_COLOR, index == app.outer_hover ? &red : &white);
        parg_uniform1f(U_ALPHA, index == app.outer_hover ? 0.75 : 0.25);
        parg_draw_triangles_u16(0, parg_mesh_ntriangles(app.disk));
    }
    parg_uniform1f(U_ALPHA, 1.0);
    for (double const* xyr = app.inner_disks; xyr != pa_end(app.inner_disks);
        xyr += 3) {
        float x = xyr[0], y = xyr[1], r = xyr[2];
        Matrix4 mvp = M4Mul(
            projection, M4Mul(view, M4Mul(M4MakeTranslation((Vector3){x, y, 0}),
            M4MakeScale((Vector3){r, r, r}))));
        parg_uniform_matrix4f(U_MVP, &mvp);
        int index = (xyr - app.inner_disks) / 3;
        parg_uniform3f(U_COLOR, index == app.inner_hover ? &yellow : &white);
        parg_draw_triangles_u16(0, parg_mesh_ntriangles(app.disk));
    }

    if (pa_count3(app.enclosing_disk)) {
        parg_uniform1f(U_ALPHA, 0.2);
        double const* xyr = app.enclosing_disk;
        float x = xyr[0], y = xyr[1], r = xyr[2];
        Matrix4 mvp = M4Mul(
            projection, M4Mul(view, M4Mul(M4MakeTranslation((Vector3){x, y, 0}),
            M4MakeScale((Vector3){r, r, r}))));
        parg_uniform_matrix4f(U_MVP, &mvp);
        parg_uniform3f(U_COLOR, &blue);
        parg_draw_triangles_u16(0, parg_mesh_ntriangles(app.disk));
    }

    parg_shader_bind(P_POINTS);
    parg_uniform1f(U_POINTSIZE, 5.0f * app.pointscale);
    for (double const* xyr = app.inner_disks; xyr != pa_end(app.inner_disks);
        xyr += 3) {
        float x = xyr[0], y = xyr[1], r = xyr[2];
        Matrix4 mvp = M4Mul(
            projection, M4Mul(view, M4MakeTranslation((Vector3){x, y, 0})));
        parg_uniform_matrix4f(U_MVP, &mvp);
        parg_draw_points(1);
    }
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    app.pointscale = pixratio;
    parg_zcam_tick(winwidth / winheight, seconds);
    return parg_zcam_has_moved();
}

void dispose()
{
    parg_shader_free(P_SIMPLE);
    parg_mesh_free(app.disk);
    pa_free(app.inner_disks);
    pa_free(app.outer_disks);
}

int pick_inner_disk(DPoint3 p)
{
    for (double const* xyr = app.inner_disks; xyr != pa_end(app.inner_disks);
        xyr += 3) {
        float dx = p.x - xyr[0];
        float dy = p.y - xyr[1];
        if (dx * dx + dy * dy < PARG_SQR(xyr[2])) {
            return (xyr - app.inner_disks) / 3;
        }
    }
    return -1;
}

int pick_outer_disk(DPoint3 p)
{
    for (double const* xyr = app.outer_disks; xyr != pa_end(app.outer_disks);
        xyr += 3) {
        float dx = p.x - xyr[0];
        float dy = p.y - xyr[1];
        if (dx * dx + dy * dy < PARG_SQR(xyr[2])) {
            return (xyr - app.outer_disks) / 3;
        }
    }
    return -1;
}

void click(DPoint3 p, int disk)
{
    if (disk == -1) {
        pa_push3(app.inner_disks, p.x, p.y, INNERRAD);
        pa_push3(app.outer_disks, p.x, p.y, OUTERRAD);
        app.inner_hover = pa_count3(app.inner_disks) - 1;
    } else {
        pa_remove3(app.inner_disks, disk);
        pa_remove3(app.outer_disks, disk);
        app.inner_hover = -1;
    }
    parg_zcam_touch();
}

void update_enclosing_disk()
{
    if (pa_count3(app.inner_disks) < 2) {
        if (pa_count3(app.enclosing_disk)) {
            pa_remove3(app.enclosing_disk, 0);
        }
        return;
    }
    if (!pa_count3(app.enclosing_disk)) {
        pa_push3(app.enclosing_disk, 0, 0, 1);
    }
    if (app.mode == ENCLOSE_DISKS) {
        par_bubbles_enclose_disks(
            app.outer_disks, pa_count3(app.outer_disks), app.enclosing_disk);
        return;
    }
    static double* xy = 0;
    while (pa_count2(xy) < pa_count3(app.inner_disks)) {
        pa_push2(xy, 0, 0);
    }
    double* pxy = xy;
    for (double const* xyr = app.inner_disks; xyr != pa_end(app.inner_disks);
        xyr += 3) {
        *pxy++ = xyr[0];
        *pxy++ = xyr[1];
    }
    par_bubbles_enclose_points(
        xy, pa_count3(app.inner_disks), app.enclosing_disk);
}

void input(parg_event evt, float x, float y, float z)
{
    DPoint3 p = parg_zcam_to_world(x, y);
    int inner = pick_inner_disk(p);
    int outer = (inner > -1) ? -1 : pick_outer_disk(p);
    int key = (char) x;
    switch (evt) {
    case PARG_EVENT_KEYPRESS:
        if (key == ' ') {
            app.mode = 1.0 - app.mode;
            update_enclosing_disk();
            parg_zcam_touch();
        }
        break;
    case PARG_EVENT_DOWN:
        app.potentially_clicking = 1;
        if (inner > -1) {
            app.inner_drag = inner;
            app.dragx = app.inner_disks[inner * 3] - p.x;
            app.dragy = app.inner_disks[inner * 3 + 1] - p.y;
        } else if (outer > -1) {
            float dx = p.x - app.outer_disks[outer * 3 + 0];
            float dy = p.y - app.outer_disks[outer * 3 + 1];
            float r = sqrt(dx * dx + dy * dy);
            app.outer_drag = outer;
            app.dragr = app.outer_disks[outer * 3 + 2] - r;
        } else {
            parg_zcam_grab_begin(x, y);
        }
        break;
    case PARG_EVENT_UP:
        app.outer_drag = app.inner_drag = -1;
        parg_zcam_grab_update(x, y, z);
        parg_zcam_grab_end();
        if (app.potentially_clicking) {
            click(p, inner);
        }
        update_enclosing_disk();
        break;
    case PARG_EVENT_MOVE:
        app.potentially_clicking = 0;
        if (app.inner_drag == -1 && app.outer_drag == -1) {
            app.inner_hover = inner;
            app.outer_hover = outer;
        } else if (app.outer_drag > -1) {
            outer = app.outer_drag;
            float dx = p.x - app.outer_disks[outer * 3 + 0];
            float dy = p.y - app.outer_disks[outer * 3 + 1];
            float r = sqrt(dx * dx + dy * dy);
            app.outer_disks[outer * 3 + 2] = r + app.dragr;
            update_enclosing_disk();
        } else {
            inner = app.inner_drag;
            app.outer_disks[inner * 3] = app.inner_disks[inner * 3] =
                    p.x + app.dragx;
            app.outer_disks[inner * 3 + 1] =
                app.inner_disks[inner * 3 + 1] = p.y + app.dragy;
            update_enclosing_disk();
        }
        parg_zcam_grab_update(x, y, z);
        break;
    default:
        break;
    }
}

void message(const char* msg)
{
    if (!strcmp(msg, "disks")) {
        app.mode = ENCLOSE_DISKS;
    } else if (!strcmp(msg, "points")) {
        app.mode = ENCLOSE_PTS;
    }
    update_enclosing_disk();
    parg_zcam_touch();
}

int main(int argc, char* argv[])
{
    puts("Drag the inner part of a disk to move it.");
    puts("Drag the outer part of a disk to scale it.");
    puts("Click in an empty area to create a disk.");
    puts("Click a disk to delete it.");
    puts("Spacebar to toggle between point & disk modes.");
    TOKEN_TABLE(PARG_TOKEN_DEFINE);
    ASSET_TABLE(PARG_ASSET_TABLE);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_ontick(tick);
    parg_window_ondraw(draw);
    parg_window_onexit(dispose);
    parg_window_oninput(input);
    parg_window_onmessage(message);
    return parg_window_exec(400, 400, 1, 1);
}
