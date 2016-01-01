#include <parg.h>
#include <parwin.h>

#define PAR_SHAPES_IMPLEMENTATION
#include <par/par_shapes.h>

#define TOKEN_TABLE(F)            \
    F(P_TEXTURE, "p_texture")     \
    F(P_CYLINDER, "p_cylinder")   \
    F(P_PODIUM, "p_podium")       \
    F(M_KLEIN, "klein.obj")       \
    F(T_KLEIN, "klein.png")       \
    F(T_ABSTRACT, "Abstract.png") \
    F(T_LOGO, "3Dlabs.png")       \
    F(T_RUST, "Rust.png")         \
    F(A_POSITION, "a_position")   \
    F(A_TEXCOORD, "a_texcoord")   \
    F(U_MVP, "u_mvp")             \
    F(U_RUST, "u_rust")           \
    F(S_SIMPLE, "klein.glsl")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

Matrix4 projection;
Matrix4 model;
Matrix4 view;
parg_mesh* kleingeo;
parg_mesh* cylinder;
parg_mesh* backdrop;
parg_texture* kleintex;
parg_texture* abstract;
parg_texture* logo;
parg_texture* rust;

#define NCHARTS 9
static const int CHARTS[NCHARTS] = {143, 130, 32, 61, 117, 504, 805, 585, 25};
static const int PODIUM_CHART = 2;

void init(float winwidth, float winheight, float pixratio)
{
    const Vector4 bgcolor = {0.5, 0.6, 0.7, 1.0};

    par_shapes_mesh* shape;
    shape = par_shapes_create_parametric("cylinder", 30, 3, 0);
    float axis[3] = {1, 0, 0};
    par_shapes_rotate(shape, M_PI * 0.5, axis);
    cylinder = parg_mesh_from_shape(shape);
    par_shapes_free(shape);

    shape =
        par_shapes_create_parametric("plane", 3, 3, PAR_SHAPES_TEXTURE_COORDS);
    par_shapes_scale(shape, 4, 4, 1);
    par_shapes_translate(shape, -2, -2, -1);
    backdrop = parg_mesh_from_shape(shape);
    par_shapes_free(shape);

    kleingeo = parg_mesh_from_asset(M_KLEIN);
    kleintex = parg_texture_from_asset(T_KLEIN);
    abstract = parg_texture_from_asset(T_ABSTRACT);
    logo = parg_texture_from_asset(T_LOGO);
    rust = parg_texture_from_asset(T_RUST);

    parg_state_clearcolor(bgcolor);
    parg_state_depthtest(1);
    parg_state_cullfaces(1);
    parg_shader_load_from_asset(S_SIMPLE);
    const float h = 1.0f;
    const float w = h * winwidth / winheight;
    const float znear = 4;
    const float zfar = 20;
    projection = M4MakeFrustum(-w, w, -h, h, znear, zfar);
    Point3 eye = {0, 1.8, 5};
    Point3 target = {0, 0.7, 0};
    Vector3 up = {0, 1, 0};
    view = M4MakeLookAt(eye, target, up);
    model = M4MakeIdentity();
}

void draw()
{
    Matrix4 modelview = view;
    Matrix4 mvp = M4Mul(projection, view);

    // Draw the background.
    parg_draw_clear();
    parg_shader_bind(P_TEXTURE);
    parg_texture_bind(abstract, 0);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_varray_enable(
        parg_mesh_coord(backdrop), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_enable(parg_mesh_uv(backdrop), A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    parg_varray_bind(parg_mesh_index(backdrop));
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(backdrop));
    parg_varray_disable(A_TEXCOORD);

    // Prep for the scene.
    modelview = M4Mul(view, model);
    mvp = M4Mul(projection, modelview);
    parg_texture_bind(kleintex, 0);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_varray_enable(
        parg_mesh_coord(kleingeo), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_enable(parg_mesh_uv(kleingeo), A_TEXCOORD, 2, PARG_FLOAT, 0, 0);
    parg_varray_bind(parg_mesh_index(kleingeo));

    // Draw each chart of the Klein bottle, skipping the podium disk.
    int start = 0;
    for (int i = 0; i < NCHARTS; i++) {
        if (i != PODIUM_CHART) {
            parg_draw_triangles_u16(start, CHARTS[i]);
        }
        start += CHARTS[i];
    }

    // Draw the podium.
    parg_shader_bind(P_PODIUM);
    parg_texture_bind(rust, 1);
    parg_uniform1i(U_RUST, 1);
    parg_uniform_matrix4f(U_MVP, &mvp);
    start = 0;
    for (int i = 0; i < NCHARTS; i++) {
        if (i == PODIUM_CHART) {
            parg_draw_triangles_u16(start, CHARTS[i]);
        }
        start += CHARTS[i];
    }
    parg_varray_disable(A_TEXCOORD);

    // Draw the walls of the cylinder.
    #if CYLINDER
    parg_shader_bind(P_CYLINDER);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_varray_enable(
        parg_mesh_coord(cylinder), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_varray_bind(parg_mesh_index(cylinder));
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(cylinder));
    #endif
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    const float RADIANS_PER_SECOND = 3.14;
    float theta = seconds * RADIANS_PER_SECOND;
    model = M4MakeRotationY(theta);
    return 1;
}

void dispose()
{
    parg_shader_free(P_TEXTURE);
    parg_mesh_free(kleingeo);
    parg_mesh_free(cylinder);
    parg_mesh_free(backdrop);
    parg_texture_free(kleintex);
    parg_texture_free(abstract);
    parg_texture_free(logo);
    parg_texture_free(rust);
}

int main(int argc, char* argv[])
{
    TOKEN_TABLE(PARG_TOKEN_DEFINE);
    parg_asset_preload(S_SIMPLE);
    parg_asset_preload(M_KLEIN);
    parg_asset_preload(T_KLEIN);
    parg_asset_preload(T_ABSTRACT);
    parg_asset_preload(T_LOGO);
    parg_asset_preload(T_RUST);
    parg_window_setargs(argc, argv);
    parg_window_oninit(init);
    parg_window_ontick(tick);
    parg_window_ondraw(draw);
    parg_window_onexit(dispose);
    return parg_window_exec(300, 300, 1, 1);
}
