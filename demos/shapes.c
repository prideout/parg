#include <parg.h>
#include <parwin.h>

#define PAR_SHAPES_IMPLEMENTATION
#include <par/par_shapes.h>

#define TOKEN_TABLE(F)            \
    F(P_TEXTURE, "p_texture")     \
    F(P_SIMPLE, "p_simple")       \
    F(T_ABSTRACT, "Abstract.png") \
    F(A_POSITION, "a_position")   \
    F(A_TEXCOORD, "a_texcoord")   \
    F(A_NORMAL, "a_normal")       \
    F(U_MVP, "u_mvp")             \
    F(U_IMV, "u_imv")             \
    F(S_SIMPLE, "shapes.glsl")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

const int NSTATES = 5;

Matrix4 projection;
Matrix4 model;
Matrix4 view;
parg_mesh* mesh = 2;
parg_mesh* bckgd;
parg_texture* abstract;
int state = 0;
int dirty = 1;

static void create_mesh()
{
    parg_mesh_free(mesh);
    par_shapes_mesh* shape;
    if (state == 0) {
        shape = par_shapes_create_icosahedron();
        par_shapes_unweld(shape, true);
        par_shapes_compute_facet_normals(shape);
    } else if (state == 1) {
        shape = par_shapes_create_subdivided_sphere(3);
        par_shapes_compute_facet_normals(shape);
    } else if (state == 2) {
        shape = par_shapes_create_parametric_sphere(20, 20);
        par_shapes_unweld(shape, 1);
        par_shapes_compute_facet_normals(shape);
    } else if (state == 3) {
        shape = par_shapes_create_rock(1, 3);
        par_shapes_compute_facet_normals(shape);
    } else if (state == 4) {
        shape = par_shapes_create_rock(2, 3);
        par_shapes_compute_facet_normals(shape);
    }
    mesh = parg_mesh_from_shape(shape);
    par_shapes_free(shape);
}

void init(float winwidth, float winheight, float pixratio)
{
    par_shapes_mesh* shape = par_shapes_create_plane(3, 3);
    par_shapes_scale(shape, 4, 4, 1);
    par_shapes_translate(shape, -2, -2, -1);
    bckgd = parg_mesh_from_shape(shape);
    par_shapes_free(shape);
    abstract = parg_texture_from_asset(T_ABSTRACT);
    parg_state_cullfaces(0);
    parg_state_depthtest(1);
    parg_shader_load_from_asset(S_SIMPLE);
    const float h = 1.0f;
    const float w = h * winwidth / winheight;
    const float znear = 4;
    const float zfar = 20;
    projection = M4MakeFrustum(-w, w, -h, h, znear, zfar);
    Point3 eye = {0, 0, 6};
    Point3 target = {0, 0, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    model = M4MakeIdentity();
}

void draw()
{
    if (dirty) {
        create_mesh();
        dirty = 0;
    }

    Matrix4 vp = M4Mul(projection, view);
    Matrix4 modelview = M4Mul(view, model);
    Matrix3 invmodelview = M4GetUpper3x3(modelview);
    Matrix4 mvp = M4Mul(projection, modelview);
    parg_draw_clear();
    parg_shader_bind(P_TEXTURE);
    parg_texture_bind(abstract, 0);
    parg_uniform_matrix4f(U_MVP, &vp);
    parg_varray_enable(parg_mesh_coord(bckgd), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_enable(parg_mesh_uv(bckgd), A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    parg_varray_bind(parg_mesh_index(bckgd));
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(bckgd));
    parg_varray_disable(A_TEXCOORD);
    parg_shader_bind(P_SIMPLE);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_uniform_matrix3f(U_IMV, &invmodelview);
    parg_varray_enable(parg_mesh_coord(mesh), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_enable(parg_mesh_norml(mesh), A_NORMAL, 3, PARG_FLOAT, 0, 0);
    parg_varray_bind(parg_mesh_index(mesh));
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(mesh));
    parg_varray_disable(A_NORMAL);
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    const float RADIANS_PER_SECOND = 3.14 * 0.5;
    float theta = seconds * RADIANS_PER_SECOND;
    model = M4MakeRotationY(theta);
    return 1;
}

void dispose()
{
    parg_shader_free(P_TEXTURE);
    parg_shader_free(P_SIMPLE);
    parg_mesh_free(mesh);
    parg_mesh_free(bckgd);
    parg_texture_free(abstract);
}

void input(parg_event evt, float code, float unused0, float unused1)
{
    int key = (char) code;
    if ((evt == PARG_EVENT_KEYPRESS && key == ' ') || evt == PARG_EVENT_UP) {
        state = (state + 1) % NSTATES;
        dirty = 1;
    }
}

int main(int argc, char* argv[])
{
    printf("Spacebar to cycle the shape.\n");
    TOKEN_TABLE(PARG_TOKEN_DEFINE);
    parg_asset_preload(S_SIMPLE);
    parg_asset_preload(T_ABSTRACT);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_oninput(input);
    parg_window_ontick(tick);
    parg_window_ondraw(draw);
    parg_window_onexit(dispose);
    return parg_window_exec(250, 250, 1, 1);
}
