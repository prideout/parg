#include <parg.h>
#include <parwin.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define PAR_MSQUARES_IMPLEMENTATION
#include <par/par_msquares.h>

#define TOKEN_TABLE(F)            \
    F(P_COLOR, "p_color")         \
    F(P_GRAY, "p_gray")           \
    F(P_GRAYMESH, "p_graymesh")   \
    F(P_COLORMESH, "p_colormesh") \
    F(A_POSITION, "a_position")   \
    F(A_TEXCOORD, "a_texcoord")   \
    F(U_COLOR, "u_color")         \
    F(U_ZSCALE, "u_zscale")       \
    F(U_MVP, "u_mvp")
TOKEN_TABLE(PAR_TOKEN_DECLARE);

#define ASSET_TABLE(F)                     \
    F(SHADER_SIMPLE, "hmap.glsl")          \
    F(TEXTURE_COLOR, "msquares_color.png") \
    F(BIN_ISLAND, "msquares_island.1024.bin")
ASSET_TABLE(PAR_TOKEN_DECLARE);

enum {
    STATE_GRAY_SOURCE,
    STATE_GRAY_DEFAULT,
    STATE_GRAY_SIMPLIFY,
    STATE_GRAY_INVERT,
    STATE_GRAY_DUAL,
    STATE_GRAY_HEIGHTS,
    STATE_GRAY_DHS,
    STATE_GRAY_DHSC,
    STATE_GRAY_MULTI,
    STATE_COLOR_SOURCE,
    STATE_COLOR_DEFAULT,
    STATE_COLOR_IH,
    STATE_COLOR_DHSCSI,
    STATE_COUNT
};

#define CELLSIZE 32
#define IMGWIDTH 1024
#define IMGHEIGHT 1024

int needs_draw = 1;
int state = STATE_GRAY_MULTI;
Matrix4 projection;
Matrix4 view;
par_mesh* trimesh[3] = {0};
par_mesh* rectmesh;
par_texture* colortex;
par_texture* graytex;
par_buffer* graybuf;
par_buffer* colorbuf;
int nmeshes = 0;

static void create_mesh()
{
    par_msquares_meshlist* mlist;
    float threshold = 0;
    int flags = 0;
    if (state == STATE_GRAY_DEFAULT) {
        float const* graydata = par_buffer_lock(graybuf, PAR_READ);
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        par_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_SIMPLIFY) {
        float const* graydata = par_buffer_lock(graybuf, PAR_READ);
        flags = PAR_MSQUARES_SIMPLIFY;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        par_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_INVERT) {
        float const* graydata = par_buffer_lock(graybuf, PAR_READ);
        flags = PAR_MSQUARES_INVERT;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        par_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_DUAL) {
        float const* graydata = par_buffer_lock(graybuf, PAR_READ);
        flags = PAR_MSQUARES_DUAL;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        par_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_HEIGHTS) {
        float const* graydata = par_buffer_lock(graybuf, PAR_READ);
        flags = PAR_MSQUARES_HEIGHTS;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        par_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_DHS) {
        float const* graydata = par_buffer_lock(graybuf, PAR_READ);
        flags = PAR_MSQUARES_DUAL | PAR_MSQUARES_HEIGHTS | PAR_MSQUARES_SNAP;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        par_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_DHSC) {
        float const* graydata = par_buffer_lock(graybuf, PAR_READ);
        flags = PAR_MSQUARES_DUAL | PAR_MSQUARES_HEIGHTS | PAR_MSQUARES_SNAP |
            PAR_MSQUARES_CONNECT;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        par_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_MULTI) {
        float const* graydata = par_buffer_lock(graybuf, PAR_READ);
        float thresholds[] = {0.0, 0.1};
        flags = PAR_MSQUARES_SIMPLIFY;
        mlist = par_msquares_grayscale_multi(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, thresholds, 2, flags);
        par_buffer_unlock(graybuf);
    } else if (state == STATE_COLOR_DEFAULT) {
        par_byte const* rgbadata = par_buffer_lock(colorbuf, PAR_READ);
        rgbadata += sizeof(int) * 3;
        mlist = par_msquares_color(
            rgbadata, IMGWIDTH, IMGHEIGHT, CELLSIZE, 0x214562, 4, flags);
        par_buffer_unlock(colorbuf);
    } else if (state == STATE_COLOR_IH) {
        par_byte const* rgbadata = par_buffer_lock(colorbuf, PAR_READ);
        rgbadata += sizeof(int) * 3;
        flags = PAR_MSQUARES_INVERT | PAR_MSQUARES_HEIGHTS;
        mlist = par_msquares_color(
            rgbadata, IMGWIDTH, IMGHEIGHT, CELLSIZE, 0x214562, 4, flags);
        par_buffer_unlock(colorbuf);
    } else if (state == STATE_COLOR_DHSCSI) {
        par_byte const* rgbadata = par_buffer_lock(colorbuf, PAR_READ);
        rgbadata += sizeof(int) * 3;
        flags = PAR_MSQUARES_DUAL | PAR_MSQUARES_HEIGHTS | PAR_MSQUARES_SNAP |
            PAR_MSQUARES_CONNECT | PAR_MSQUARES_SIMPLIFY | PAR_MSQUARES_INVERT;
        mlist = par_msquares_color(
            rgbadata, IMGWIDTH, IMGHEIGHT, CELLSIZE, 0x214562, 4, flags);
        par_buffer_unlock(colorbuf);
    }

    nmeshes = par_msquares_get_count(mlist);
    for (int imesh = 0; imesh < nmeshes; imesh++) {
        par_msquares_mesh const* mesh = par_msquares_get_mesh(mlist, imesh);
        printf("%d points, %d triangles\n", mesh->npoints, mesh->ntriangles);

        // mquares_mesh might have dimensionality of 2 or 3, while par_mesh only
        // supports the latter.  So, we potentially need to expand the data from
        // vec2 to vec3.

        float* points = mesh->points;
        if (mesh->dim == 2) {
            printf("Expanding vec2 mesh into a vec3 mesh.\n");
            points = malloc(mesh->npoints * sizeof(float) * 3);
            for (int i = 0; i < mesh->npoints; i++) {
                points[i * 3] = mesh->points[i * 2];
                points[i * 3 + 1] = mesh->points[i * 2 + 1];
                points[i * 3 + 2] = 0;
            }
        }

        trimesh[imesh] = par_mesh_create(
            points, mesh->npoints, mesh->triangles, mesh->ntriangles);

        if (mesh->dim == 2) {
            free(points);
        }
    }

    par_msquares_free(mlist);
}

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = {0.937, 0.937, 0.93, 1.00};
    par_state_clearcolor(bgcolor);
    par_state_cullfaces(1);
    par_state_depthtest(1);
    par_shader_load_from_asset(SHADER_SIMPLE);

    int* rawdata;
    colorbuf = par_buffer_slurp_asset(TEXTURE_COLOR, (void*) &rawdata);
    int width = *rawdata++;
    int height = *rawdata++;
    int ncomps = *rawdata++;
    par_buffer_unlock(colorbuf);

    colortex =
        par_texture_from_u8(colorbuf, width, height, ncomps, 3 * sizeof(int));
    graybuf = par_buffer_from_asset(BIN_ISLAND);
    graytex = par_texture_from_fp32(graybuf, IMGWIDTH, IMGHEIGHT, 1, 0);
    const float h = 1.5f;
    const float w = h * winwidth / winheight;
    const float znear = 10;
    const float zfar = 90;
    projection = M4MakeFrustum(-w, w, -h, h, znear, zfar);
    Point3 eye = {0, -50, 50};
    Point3 target = {0, 0, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    rectmesh = par_mesh_rectangle(20, 20);
}

void draw()
{
    int mesh = 0, multi = 0;
    switch (state) {
    case STATE_GRAY_SOURCE:
        par_shader_bind(P_GRAY);
        par_texture_bind(graytex, 0);
        par_uniform1f(U_ZSCALE, 1);
        break;
    case STATE_COLOR_IH:
        mesh = 1;
        par_shader_bind(P_GRAYMESH);
        par_uniform1f(U_ZSCALE, 0.3);
        break;
    case STATE_COLOR_DHSCSI:
        mesh = multi = 1;
        par_shader_bind(P_GRAYMESH);
        par_uniform1f(U_ZSCALE, 0.3);
        break;
    case STATE_COLOR_DEFAULT:
    case STATE_GRAY_DEFAULT:
    case STATE_GRAY_SIMPLIFY:
    case STATE_GRAY_INVERT:
    case STATE_GRAY_HEIGHTS:
        mesh = 1;
        par_shader_bind(P_GRAYMESH);
        par_uniform1f(U_ZSCALE, 1);
        break;
    case STATE_GRAY_MULTI:
    case STATE_GRAY_DUAL:
        mesh = multi = 1;
        par_shader_bind(P_GRAYMESH);
        par_texture_bind(colortex, 0);
        par_uniform1f(U_ZSCALE, 1);
        break;
    case STATE_GRAY_DHS:
    case STATE_GRAY_DHSC:
        mesh = multi = 1;
        par_shader_bind(P_GRAYMESH);
        par_texture_bind(colortex, 0);
        par_uniform1f(U_ZSCALE, 0.5);
        break;
    case STATE_COLOR_SOURCE:
        par_texture_bind(colortex, 0);
        par_shader_bind(P_COLOR);
        par_uniform1f(U_ZSCALE, 1);
        break;
    default:
        break;
    }

    if (mesh) {
        par_mesh_free(trimesh[0]);
        par_mesh_free(trimesh[1]);
        par_mesh_free(trimesh[2]);
        trimesh[0] = trimesh[1] = trimesh[2] = 0;
        create_mesh();
    }

    Matrix4 model;
    if (mesh) {
        model = M4MakeScale(V3MakeFromElems(20, 20, 10));
        model = M4Mul(M4MakeTranslation(V3MakeFromElems(-10, -10, 0)), model);
    } else {
        model = M4MakeIdentity();
    }

    Matrix4 modelview = M4Mul(view, model);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_uniform_matrix4f(U_MVP, &mvp);
    par_draw_clear();
    if (mesh) {

        Vector4 colors[3];
        colors[0] = (Vector4) {0, 0.6, 0.9, 1};
        colors[1] = (Vector4) {0, 0.9, 0.6, 1};
        colors[2] = (Vector4) {0.9, 0.6, 0, 1};
        Vector4 black = {0, 0, 0, 0.5};

        for (int imesh = 0; imesh < nmeshes; imesh++) {
            par_varray_enable(
                par_mesh_coord(trimesh[imesh]), A_POSITION, 3, PAR_FLOAT, 0, 0);
            par_varray_bind(par_mesh_index(trimesh[imesh]));
            par_uniform4f(U_COLOR, &colors[imesh]);
            par_draw_triangles_u16(0, par_mesh_ntriangles(trimesh[imesh]));
            par_uniform4f(U_COLOR, &black);
            par_state_blending(1);
            par_draw_wireframe_triangles_u16(0, par_mesh_ntriangles(trimesh[imesh]));
            par_state_blending(0);
        }

    } else {
        par_varray_enable(
            par_mesh_coord(rectmesh), A_POSITION, 2, PAR_FLOAT, 0, 0);
        par_varray_enable(
            par_mesh_uv(rectmesh), A_TEXCOORD, 2, PAR_FLOAT, 0, 0);
        par_draw_one_quad();
        par_varray_disable(A_TEXCOORD);
    }
}

void dispose()
{
    par_shader_free(P_GRAY);
    par_shader_free(P_COLOR);
    par_shader_free(P_GRAYMESH);
    par_shader_free(P_COLORMESH);
    par_mesh_free(rectmesh);
    par_texture_free(colortex);
    par_texture_free(graytex);
    par_buffer_free(graybuf);
    par_mesh_free(trimesh[0]);
    par_mesh_free(trimesh[1]);
    par_mesh_free(trimesh[2]);
}

void input(par_event evt, float code, float unused0, float unused1)
{
    int key = (char) code;
    if ((evt == PAR_EVENT_KEYPRESS && key == ' ') || evt == PAR_EVENT_UP) {
        state = (state + 1) % STATE_COUNT;
        needs_draw = 1;
    }
}

int tick(float seconds, float winwidth, float winheight, float pixratio)
{
    int retval = needs_draw;
    needs_draw = 0;
    return retval;
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    ASSET_TABLE(PAR_ASSET_TABLE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_oninput(input);
    par_window_ondraw(draw);
    par_window_ontick(tick);
    par_window_onexit(dispose);
    return par_window_exec(480, 320, 1);
}
