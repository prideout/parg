#include <parg.h>
#include <parwin.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "lodepng.h"

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
TOKEN_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_TABLE(F)                     \
    F(SHADER_SIMPLE, "hmap.glsl")          \
    F(TEXTURE_COLOR, "msquares_color.png") \
    F(BIN_ISLAND, "msquares_island.1024.bin")
ASSET_TABLE(PARG_TOKEN_DECLARE);

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
    STATE_MULTI_RGBA,
    STATE_MULTI_RGB,
    STATE_MULTI_DIAGRAM,
    STATE_COUNT
};

#define CELLSIZE 32
#define IMGWIDTH 1024
#define IMGHEIGHT 1024

int needs_draw = 1;
int state = STATE_MULTI_RGBA;
Matrix4 projection;
Matrix4 view;
parg_mesh* trimesh[48] = {0};
uint32_t meshcolors[48];
parg_mesh* rectmesh;
parg_texture* colortex;
parg_texture* graytex;
parg_buffer* graybuf;
parg_buffer* colorbuf;
int nmeshes = 0;

static void create_mesh()
{
    par_msquares_meshlist* mlist = 0;
    float threshold = 0;
    int flags = 0;
    if (state == STATE_GRAY_DEFAULT) {
        float const* graydata = parg_buffer_lock(graybuf, PARG_READ);
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        parg_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_SIMPLIFY) {
        float const* graydata = parg_buffer_lock(graybuf, PARG_READ);
        flags = PAR_MSQUARES_SIMPLIFY;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        parg_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_INVERT) {
        float const* graydata = parg_buffer_lock(graybuf, PARG_READ);
        flags = PAR_MSQUARES_INVERT;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        parg_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_DUAL) {
        float const* graydata = parg_buffer_lock(graybuf, PARG_READ);
        flags = PAR_MSQUARES_DUAL;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        parg_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_HEIGHTS) {
        float const* graydata = parg_buffer_lock(graybuf, PARG_READ);
        flags = PAR_MSQUARES_HEIGHTS;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        parg_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_DHS) {
        float const* graydata = parg_buffer_lock(graybuf, PARG_READ);
        flags = PAR_MSQUARES_DUAL | PAR_MSQUARES_HEIGHTS | PAR_MSQUARES_SNAP;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        parg_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_DHSC) {
        float const* graydata = parg_buffer_lock(graybuf, PARG_READ);
        flags = PAR_MSQUARES_DUAL | PAR_MSQUARES_HEIGHTS | PAR_MSQUARES_SNAP |
            PAR_MSQUARES_CONNECT;
        mlist = par_msquares_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        parg_buffer_unlock(graybuf);
    } else if (state == STATE_GRAY_MULTI) {
        float const* graydata = parg_buffer_lock(graybuf, PARG_READ);
        float thresholds[] = {0.0, 0.1};
        flags = PAR_MSQUARES_SIMPLIFY | PAR_MSQUARES_HEIGHTS |
            PAR_MSQUARES_SNAP | PAR_MSQUARES_CONNECT;
        mlist = par_msquares_grayscale_multi(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, thresholds, 2, flags);
        parg_buffer_unlock(graybuf);
    } else if (state == STATE_COLOR_DEFAULT) {
        parg_byte const* rgbadata = parg_buffer_lock(colorbuf, PARG_READ);
        rgbadata += sizeof(int) * 3;
        mlist = par_msquares_color(
            rgbadata, IMGWIDTH, IMGHEIGHT, CELLSIZE, 0x214562, 4, flags);
        parg_buffer_unlock(colorbuf);
    } else if (state == STATE_COLOR_IH) {
        parg_byte const* rgbadata = parg_buffer_lock(colorbuf, PARG_READ);
        rgbadata += sizeof(int) * 3;
        flags = PAR_MSQUARES_INVERT | PAR_MSQUARES_HEIGHTS;
        mlist = par_msquares_color(
            rgbadata, IMGWIDTH, IMGHEIGHT, CELLSIZE, 0x214562, 4, flags);
        parg_buffer_unlock(colorbuf);
    } else if (state == STATE_COLOR_DHSCSI) {
        parg_byte const* rgbadata = parg_buffer_lock(colorbuf, PARG_READ);
        rgbadata += sizeof(int) * 3;
        flags = PAR_MSQUARES_DUAL | PAR_MSQUARES_HEIGHTS | PAR_MSQUARES_SNAP |
            PAR_MSQUARES_CONNECT | PAR_MSQUARES_SIMPLIFY |
            PAR_MSQUARES_INVERT;
        mlist = par_msquares_color(
            rgbadata, IMGWIDTH, IMGHEIGHT, CELLSIZE, 0x214562, 4, flags);
        parg_buffer_unlock(colorbuf);
    } else if (state == STATE_MULTI_RGB) {
        unsigned dims[2] = {0, 0};
        unsigned char* pixels;
        lodepng_decode_file(
            &pixels, &dims[0], &dims[1], "extern/par/test/rgb.png", LCT_RGB, 8);
        mlist = par_msquares_color_multi(
            pixels, dims[0], dims[1], 16, 3, PAR_MSQUARES_SIMPLIFY);
        free(pixels);
    } else if (state == STATE_MULTI_RGBA) {
        unsigned dims[2] = {0, 0};
        unsigned char* pixels;
        lodepng_decode_file(&pixels, &dims[0], &dims[1],
            "extern/par/test/rgba.png", LCT_RGBA, 8);
        mlist = par_msquares_color_multi(pixels, dims[0], dims[1], 16, 4,
                PAR_MSQUARES_HEIGHTS | PAR_MSQUARES_CONNECT |
                PAR_MSQUARES_SIMPLIFY);
        free(pixels);
    } else if (state == STATE_MULTI_DIAGRAM) {
        parg_byte const* rgbadata = parg_buffer_lock(colorbuf, PARG_READ);
        rgbadata += sizeof(int) * 3;
        mlist = par_msquares_color_multi(rgbadata, IMGWIDTH, IMGHEIGHT,
                CELLSIZE, 4, PAR_MSQUARES_SIMPLIFY | PAR_MSQUARES_HEIGHTS);
        parg_buffer_unlock(colorbuf);
    }

    nmeshes = par_msquares_get_count(mlist);
    printf("%d meshes\n", nmeshes);
    for (int imesh = 0; imesh < nmeshes; imesh++) {
        par_msquares_mesh const* mesh = par_msquares_get_mesh(mlist, imesh);

        // mquares_mesh might have dimensionality of 2 or 3, while parg_mesh
        // only
        // supports the latter.  So, we potentially need to expand the data from
        // vec2 to vec3.

        float* points = mesh->points;
        if (mesh->dim == 2) {
            points = malloc(mesh->npoints * sizeof(float) * 3);
            for (int i = 0; i < mesh->npoints; i++) {
                points[i * 3] = mesh->points[i * 2];
                points[i * 3 + 1] = mesh->points[i * 2 + 1];
                points[i * 3 + 2] = 0;
            }
        }
        meshcolors[imesh] = mesh->color;
        trimesh[imesh] = parg_mesh_create(
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
    parg_state_clearcolor(bgcolor);
    parg_state_cullfaces(1);
    parg_state_depthtest(1);
    parg_shader_load_from_asset(SHADER_SIMPLE);

    int* rawdata;
    colorbuf = parg_buffer_slurp_asset(TEXTURE_COLOR, (void*) &rawdata);
    int width = *rawdata++;
    int height = *rawdata++;
    int ncomps = *rawdata++;
    parg_buffer_unlock(colorbuf);

    colortex =
        parg_texture_from_u8(colorbuf, width, height, ncomps, 3 * sizeof(int));
    graybuf = parg_buffer_from_asset(BIN_ISLAND);
    graytex = parg_texture_from_fp32(graybuf, IMGWIDTH, IMGHEIGHT, 1, 0);
    const float h = 1.5f;
    const float w = h * winwidth / winheight;
    const float znear = 10;
    const float zfar = 90;
    projection = M4MakeFrustum(-w, w, -h, h, znear, zfar);
    Point3 eye = {0, -50, 50};
    Point3 target = {0, 0, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    rectmesh = parg_mesh_rectangle(20, 20);
}

void draw()
{
    int mesh = 0, multi = 0, meshcolor = 0;
    switch (state) {
    case STATE_GRAY_SOURCE:
        parg_shader_bind(P_GRAY);
        parg_texture_bind(graytex, 0);
        parg_uniform1f(U_ZSCALE, 1);
        break;
    case STATE_COLOR_IH:
        mesh = 1;
        parg_shader_bind(P_GRAYMESH);
        parg_uniform1f(U_ZSCALE, 0.3);
        break;
    case STATE_COLOR_DHSCSI:
        mesh = multi = 1;
        parg_shader_bind(P_GRAYMESH);
        parg_uniform1f(U_ZSCALE, 0.3);
        break;
    case STATE_MULTI_RGBA:
    case STATE_MULTI_RGB:
    case STATE_MULTI_DIAGRAM:
        meshcolor = mesh = multi = 1;
        parg_shader_bind(P_GRAYMESH);
        parg_uniform1f(U_ZSCALE, 0.25);
        break;
    case STATE_COLOR_DEFAULT:
    case STATE_GRAY_DEFAULT:
    case STATE_GRAY_SIMPLIFY:
    case STATE_GRAY_INVERT:
    case STATE_GRAY_HEIGHTS:
        mesh = 1;
        parg_shader_bind(P_GRAYMESH);
        parg_uniform1f(U_ZSCALE, 1);
        break;
    case STATE_GRAY_MULTI:
        mesh = multi = 1;
        parg_shader_bind(P_GRAYMESH);
        parg_texture_bind(colortex, 0);
        parg_uniform1f(U_ZSCALE, 0.3);
        break;
    case STATE_GRAY_DUAL:
        mesh = multi = 1;
        parg_shader_bind(P_GRAYMESH);
        parg_texture_bind(colortex, 0);
        parg_uniform1f(U_ZSCALE, 1);
        break;
    case STATE_GRAY_DHS:
    case STATE_GRAY_DHSC:
        mesh = multi = 1;
        parg_shader_bind(P_GRAYMESH);
        parg_texture_bind(colortex, 0);
        parg_uniform1f(U_ZSCALE, 0.5);
        break;
    case STATE_COLOR_SOURCE:
        parg_texture_bind(colortex, 0);
        parg_shader_bind(P_COLOR);
        parg_uniform1f(U_ZSCALE, 1);
        break;
    default:
        break;
    }

    if (mesh) {
        for (int i = 0; i < sizeof(trimesh) / sizeof(trimesh[0]); i++) {
            parg_mesh_free(trimesh[i]);
        }
        memset(trimesh, 0, sizeof(trimesh));
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
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_draw_clear();
    if (mesh) {
        Vector4 colors[3];
        colors[0] = (Vector4){0, 0.6, 0.9, 1};
        colors[1] = (Vector4){0, 0.9, 0.6, 1};
        colors[2] = (Vector4){0.9, 0.6, 0, 1};
        Vector4 black = {0, 0, 0, 1.0};

        for (int imesh = 0; imesh < nmeshes; imesh++) {
            parg_varray_enable(parg_mesh_coord(trimesh[imesh]), A_POSITION, 3,
                PARG_FLOAT, 0, 0);
            parg_varray_bind(parg_mesh_index(trimesh[imesh]));
            if (meshcolor) {
                unsigned int b = meshcolors[imesh] & 0xff;
                unsigned int g = (meshcolors[imesh] >> 8) & 0xff;
                unsigned int r = (meshcolors[imesh] >> 16) & 0xff;
                unsigned int a = (meshcolors[imesh] >> 24) & 0xff;
                Vector4 color;
                color.x = r / 255.0f;
                color.y = g / 255.0f;
                color.z = b / 255.0f;
                color.w = a / 255.0f;
                parg_uniform4f(U_COLOR, &color);
            } else {
                parg_uniform4f(U_COLOR, &colors[imesh]);
            }
            parg_draw_triangles_u16(0, parg_mesh_ntriangles(trimesh[imesh]));
            parg_uniform4f(U_COLOR, &black);
            parg_draw_wireframe_triangles_u16(
                0, parg_mesh_ntriangles(trimesh[imesh]));
        }

    } else {
        parg_varray_enable(
            parg_mesh_coord(rectmesh), A_POSITION, 2, PARG_FLOAT, 0, 0);
        parg_varray_enable(
            parg_mesh_uv(rectmesh), A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
        parg_draw_one_quad();
        parg_varray_disable(A_TEXCOORD);
    }
}

void dispose()
{
    parg_shader_free(P_GRAY);
    parg_shader_free(P_COLOR);
    parg_shader_free(P_GRAYMESH);
    parg_shader_free(P_COLORMESH);
    parg_mesh_free(rectmesh);
    parg_texture_free(colortex);
    parg_texture_free(graytex);
    parg_buffer_free(graybuf);
    for (int i = 0; i < sizeof(trimesh) / sizeof(trimesh[0]); i++) {
        parg_mesh_free(trimesh[i]);
    }
}

void input(parg_event evt, float code, float unused0, float unused1)
{
    int key = (char) code;
    if ((evt == PARG_EVENT_KEYPRESS && key == ' ') || evt == PARG_EVENT_UP) {
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
    TOKEN_TABLE(PARG_TOKEN_DEFINE);
    ASSET_TABLE(PARG_ASSET_TABLE);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_oninput(input);
    parg_window_ondraw(draw);
    parg_window_ontick(tick);
    parg_window_onexit(dispose);
    return parg_window_exec(480 * 2, 320 * 2, 1, 1);
}
