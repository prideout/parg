#include <par.h>
#include <parwin.h>
#include <stdio.h>

#define TOKEN_TABLE(F)          \
    F(P_COLOR, "p_color")       \
    F(P_GRAY, "p_gray")         \
    F(P_BLACK, "p_black")       \
    F(A_POSITION, "a_position") \
    F(A_TEXCOORD, "a_texcoord") \
    F(U_MVP, "u_mvp")
TOKEN_TABLE(PAR_TOKEN_DECLARE);

#define ASSET_TABLE(F)                     \
    F(SHADER_SIMPLE, "hmap.glsl")          \
    F(TEXTURE_COLOR, "msquares_color.png") \
    F(BIN_ISLAND, "msquares_island.1024.bin")
ASSET_TABLE(PAR_TOKEN_DECLARE);

enum { STATE_GRAY_SOURCE, STATE_COLOR_SOURCE, STATE_GRAY_MESH, STATE_COUNT };

#define CELLSIZE 32
#define IMGWIDTH 1024
#define IMGHEIGHT 1024

int state = STATE_GRAY_MESH;
Matrix4 projection;
Matrix4 view;
par_mesh* trimesh;
par_mesh* rectmesh;
par_texture* colortex;
par_texture* graytex;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = {0.937, 0.937, 0.93, 1.00};
    par_state_clearcolor(bgcolor);
    par_state_cullfaces(1);
    par_state_depthtest(1);
    par_shader_load_from_asset(SHADER_SIMPLE);

    colortex = par_texture_from_asset(TEXTURE_COLOR);

    par_buffer* graybuf = par_buffer_from_asset(BIN_ISLAND);
    graytex = par_texture_from_fp32(graybuf, IMGWIDTH, IMGHEIGHT, 1);
    float const* graydata = par_buffer_lock(graybuf, PAR_READ);
    float threshold = 0;
    int flags = 0;
    par_msquares_meshlist* mlist = par_msquares_from_grayscale(
        graydata, IMGWIDTH, IMGHEIGHT, CELLSIZE, threshold, flags);
    par_buffer_unlock(graybuf);
    par_buffer_free(graybuf);

    par_msquares_mesh* mesh = par_msquares_get_mesh(mlist, 0);
    printf("%d points, %d triangles\n", mesh->npoints, mesh->ntriangles);
    trimesh = par_mesh_create(
        mesh->points, mesh->npoints, mesh->triangles, mesh->ntriangles);
    par_msquares_free(mlist);

    const float h = 7.0f;
    const float w = h * winwidth / winheight;
    const float znear = 50;
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
    par_draw_clear();

    Matrix4 model;
    int mesh = 0;

    if (state == STATE_GRAY_SOURCE) {
        par_shader_bind(P_GRAY);
        par_texture_bind(graytex, 0);
    } else if (state == STATE_GRAY_MESH) {
        mesh = 1;
        par_shader_bind(P_BLACK);
    } else {
        par_shader_bind(P_COLOR);
        par_texture_bind(colortex, 0);
    }

    if (mesh) {
        model = M4MakeScale(V3MakeFromElems(20, 20, 0));
        model = M4Mul(M4MakeTranslation(V3MakeFromElems(-10, -10, 0)), model);
    } else {
        model = M4MakeIdentity();
    }

    Matrix4 modelview = M4Mul(view, model);
    Matrix3 invmodelview = M4GetUpper3x3(modelview);
    Matrix4 mvp = M4Mul(projection, modelview);
    par_uniform_matrix4f(U_MVP, &mvp);

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
    par_mesh_free(rectmesh);
    par_mesh_free(trimesh);
    par_texture_free(colortex);
    par_texture_free(graytex);
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
