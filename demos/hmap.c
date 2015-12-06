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
    F(U_MVP, "u_mvp")
TOKEN_TABLE(PAR_TOKEN_DECLARE);

#define ASSET_TABLE(F)                     \
    F(SHADER_SIMPLE, "hmap.glsl")          \
    F(TEXTURE_COLOR, "msquares_color.png") \
    F(BIN_ISLAND, "msquares_island.1024.bin")
ASSET_TABLE(PAR_TOKEN_DECLARE);

enum {
    STATE_GRAY_SOURCE,
    STATE_GRAY_MESH,
    STATE_COLOR_SOURCE,
    STATE_COLOR_MESH,
    STATE_COUNT
};

#define CELLSIZE 32
#define IMGWIDTH 1024
#define IMGHEIGHT 1024

int state = STATE_COLOR_MESH;
Matrix4 projection;
Matrix4 view;
par_mesh* trimesh = 0;
par_mesh* rectmesh;
par_texture* colortex;
par_texture* graytex;
par_buffer* graybuf;
par_buffer* colorbuf;

par_mesh* create_mesh()
{
    par_msquares_meshlist* mlist;
    if (state == STATE_GRAY_MESH) {
        float const* graydata = par_buffer_lock(graybuf, PAR_READ);
        float threshold = 0;
        int flags = PAR_MSQUARES_HEIGHTS;
        mlist = par_msquares_from_grayscale(
            graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
        par_buffer_unlock(graybuf);
    } else {
        par_byte const* rgbadata = par_buffer_lock(colorbuf, PAR_READ);
        rgbadata += sizeof(int) * 3;
        int flags = PAR_MSQUARES_INVERT | PAR_MSQUARES_HEIGHTS;
        mlist = par_msquares_from_color(
            rgbadata, IMGWIDTH, IMGHEIGHT, CELLSIZE, 0x214562, 4, flags);
        par_buffer_unlock(colorbuf);
    }
    par_msquares_mesh* mesh = par_msquares_get_mesh(mlist, 0);
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

    par_mesh* trimesh = par_mesh_create(
        points, mesh->npoints, mesh->triangles, mesh->ntriangles);

    if (mesh->dim == 2) {
        free(points);
    }

    par_msquares_free(mlist);
    return trimesh;
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
    int mesh = 0;
    switch (state) {
    case STATE_GRAY_SOURCE:
        par_shader_bind(P_GRAY);
        par_texture_bind(graytex, 0);
        break;
    case STATE_GRAY_MESH:
        mesh = 1;
        par_shader_bind(P_GRAYMESH);
        par_texture_bind(colortex, 0);
        break;
    case STATE_COLOR_SOURCE:
        par_shader_bind(P_COLOR);
        break;
    case STATE_COLOR_MESH:
        mesh = 1;
        par_shader_bind(P_COLORMESH);
        par_texture_bind(colortex, 0);
        break;
    default:
        break;
    }

    if (mesh && !trimesh) {
        trimesh = create_mesh();
    } else if (!mesh && trimesh) {
        par_mesh_free(trimesh);
        trimesh = 0;
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
        par_varray_enable(
            par_mesh_coord(trimesh), A_POSITION, 3, PAR_FLOAT, 0, 0);
        par_varray_bind(par_mesh_index(trimesh));
        par_draw_triangles_u16(0, par_mesh_ntriangles(trimesh));
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
    if (trimesh) {
        par_mesh_free(trimesh);
    }
}

void input(par_event evt, float code, float unused0, float unused1)
{
    int key = (char) code;
    if ((evt == PAR_EVENT_KEYPRESS && key == ' ') || evt == PAR_EVENT_UP) {
        state = (state + 1) % STATE_COUNT;
    }
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PAR_TOKEN_DEFINE);
    ASSET_TABLE(PAR_ASSET_TABLE);
    par_window_setargs(argc, argv);
    par_window_oninit(init);
    par_window_oninput(input);
    par_window_ondraw(draw);
    par_window_onexit(dispose);
    return par_window_exec(185 * 5, 100 * 5, 1);
}
