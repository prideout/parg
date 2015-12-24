#include <parg.h>
#include <parwin.h>
#include <assert.h>
#include <sds.h>
#include <stdio.h>

#define PAR_MSQUARES_IMPLEMENTATION
#include <par/par_msquares.h>

#define TOKEN_TABLE(F)                              \
    F(P_LANDMASS, "p_landmass")                     \
    F(P_LANDMASS_FRAGCOORD, "p_landmass_fragcoord") \
    F(P_OCEAN, "p_ocean")                           \
    F(P_SOLID, "p_solid")                           \
    F(A_POSITION, "a_position")                     \
    F(U_MVP, "u_mvp")                               \
    F(U_COLOR, "u_color")                           \
    F(U_SHOWGRID, "u_showgrid")                     \
    F(U_SLIPPYBOX, "u_slippybox")                   \
    F(U_SLIPPYFRACT, "u_slippyfract")
TOKEN_TABLE(PARG_TOKEN_DECLARE);

#define ASSET_TABLE(F)              \
    F(SHADER_DEFAULT, "ztex.glsl")  \
    F(TEXTURE_PAPER, "paper.png")   \
    F(TEXTURE_EUROPE, "europe.png") \
    F(TEXTURE_OCEAN, "water.png")
ASSET_TABLE(PARG_TOKEN_DECLARE);

const Vector3 TARGETPOS = {0.35287, 0.005156, 0.000005};
const float DEMO_DURATION = 6;
const double STARTZ = 1.2;
const float fovy = 16 * PARG_TWOPI / 180;
int mode_highp = 1;
float current_time;
float mode_demo_start = 0;
int mode_demo_direction = 1;
int showgrid = 0;
parg_mesh* landmass_mesh;
parg_mesh* ocean_mesh;
parg_texture* ocean_texture;
parg_texture* paper_texture;
Vector2 fbsize;

void init(float winwidth, float winheight, float pixratio)
{
    printf(
        "Spacebar to toggle texture modes.\n"
        "D to toggle auto-zooming demo mode.\n"
        "G to toggle the slippy map grid.\n");
    parg_state_clearcolor((Vector4){0.43, 0.61, 0.8, 1});
    parg_state_cullfaces(1);
    parg_state_depthtest(0);
    parg_shader_load_from_asset(SHADER_DEFAULT);
    parg_zcam_init(1, 1, fovy);
    parg_zcam_set_position(0, 0, STARTZ);
    ocean_texture = parg_texture_from_asset(TEXTURE_OCEAN);
    paper_texture = parg_texture_from_asset(TEXTURE_PAPER);

    // Decode the europe image.
    int* rawdata;
    parg_buffer* colorbuf =
        parg_buffer_slurp_asset(TEXTURE_EUROPE, (void*) &rawdata);
    int width = *rawdata++;
    int height = *rawdata++;
    int ncomps = *rawdata++;
    parg_texture_fliprows(rawdata, width * ncomps, height);

    // Sample the ocean color and swizzle R with B to appease msquares.
    int ocean_color = rawdata[0];
    ocean_color = ((ocean_color >> 16) & 0xff) |
        ((ocean_color << 16) & 0xff0000) | (ocean_color & 0xff00ff00);

    // Perform marching squares and generate a mesh.
    par_msquares_meshlist* mlist = par_msquares_color((parg_byte*) rawdata,
            width, height, 16, ocean_color, 4,
            PAR_MSQUARES_DUAL | PAR_MSQUARES_HEIGHTS | PAR_MSQUARES_SIMPLIFY);
    par_msquares_mesh const* mesh;
    mesh = par_msquares_get_mesh(mlist, 0);
    landmass_mesh = parg_mesh_create(
        mesh->points, mesh->npoints, mesh->triangles, mesh->ntriangles);
    mesh = par_msquares_get_mesh(mlist, 1);
    ocean_mesh = parg_mesh_create(
        mesh->points, mesh->npoints, mesh->triangles, mesh->ntriangles);
    parg_buffer_unlock(colorbuf);
    par_msquares_free(mlist);
}

void draw()
{
    DMatrix4 view, projection;
    parg_zcam_dmatrices(&projection, &view);
    DMatrix4 model = DM4MakeTranslation((DVector3){-0.5, -0.5, -1});
    Matrix4 mvp = M4MakeFromDM4(DM4Mul(projection, DM4Mul(view, model)));
    const Vector4 BLACK = {0, 0, 0, 1};

    Vector2 mapsize = {1, 1};
    parg_aar rect = parg_zcam_get_rectangle();
    parg_tilerange tiles;
    float slippyfract = parg_aar_to_tilerange(rect, mapsize, &tiles);
    parg_aar slippyaar = parg_aar_from_tilename(tiles.mintile, mapsize);
    Vector4* slippybox = (Vector4*) &slippyaar;
    slippybox->z = 1.0 / (slippybox->z - slippybox->x);
    slippybox->w = 1.0 / (slippybox->w - slippybox->y);

    parg_draw_clear();
    parg_shader_bind(P_OCEAN);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_uniform1i(U_SHOWGRID, showgrid);
    parg_uniform4f(U_SLIPPYBOX, slippybox);
    parg_uniform1f(U_SLIPPYFRACT, slippyfract);
    parg_texture_bind(ocean_texture, 0);
    parg_varray_bind(parg_mesh_index(ocean_mesh));
    parg_varray_enable(
        parg_mesh_coord(ocean_mesh), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(ocean_mesh));
    parg_shader_bind(P_SOLID);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_uniform4f(U_COLOR, &BLACK);
    parg_varray_bind(parg_mesh_index(landmass_mesh));
    parg_varray_enable(
        parg_mesh_coord(landmass_mesh), A_POSITION, 3, PARG_FLOAT, 0, 0);
    parg_draw_wireframe_triangles_u16(0, parg_mesh_ntriangles(landmass_mesh));

    if (mode_highp) {
        float x = parg_aar_width(rect) / fbsize.x;
        float y = parg_aar_height(rect) / fbsize.y;
        slippybox->z *= x;
        slippybox->w *= y;
        slippybox->x = (slippybox->x - rect.left) / x;
        slippybox->y = (slippybox->y - rect.bottom) / y;
    }

    parg_shader_bind(mode_highp ? P_LANDMASS_FRAGCOORD : P_LANDMASS);
    parg_uniform_matrix4f(U_MVP, &mvp);
    parg_uniform1i(U_SHOWGRID, showgrid);
    parg_uniform4f(U_SLIPPYBOX, slippybox);
    parg_uniform1f(U_SLIPPYFRACT, slippyfract);
    parg_texture_bind(paper_texture, 0);
    parg_draw_triangles_u16(0, parg_mesh_ntriangles(landmass_mesh));
}

int tick(float winwidth, float winheight, float pixratio, float seconds)
{
    current_time = seconds;
    fbsize.x = winwidth * pixratio;
    fbsize.y = winheight * pixratio;
    parg_zcam_tick(winwidth / winheight, seconds);
    if (mode_demo_start) {
        double t = (current_time - mode_demo_start) / DEMO_DURATION;
        if (t >= 1) {
            mode_demo_start = 0;
        }
        float scrolldelta = mode_demo_direction ? -3 : 3;
        parg_zcam_grab_update(TARGETPOS.x, TARGETPOS.y, scrolldelta);
    }
    return parg_zcam_has_moved();
}

void dispose()
{
    parg_mesh_free(landmass_mesh);
    parg_mesh_free(ocean_mesh);
    parg_texture_free(ocean_texture);
    parg_texture_free(paper_texture);
}

void input(parg_event evt, float x, float y, float z)
{
    char key = (char) x;
    switch (evt) {
    case PARG_EVENT_KEYPRESS:
        if (key == ' ') {
            mode_highp = !mode_highp;
            printf("Precision %s.\n", mode_highp ? "on" : "off");
            parg_zcam_touch();
        } else if (key == 'G') {
            showgrid = 1 - showgrid;
            parg_zcam_touch();
        } else if (key == 'D') {
            if (!mode_demo_start) {
                mode_demo_start = current_time;
                mode_demo_direction = 1 - mode_demo_direction;
            }
        }
        break;
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

void message(const char* msg)
{
    if (!strcmp(msg, "precision")) {
        mode_highp = 1 - mode_highp;
        parg_zcam_touch();
    } else if (!strcmp(msg, "grid")) {
        showgrid = 1 - showgrid;
        parg_zcam_touch();
    } else if (!strcmp(msg, "demo")) {
        if (!mode_demo_start) {
            mode_demo_start = current_time;
            mode_demo_direction = 1 - mode_demo_direction;
        }
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
    parg_window_onmessage(message);
    return parg_window_exec(400, 300, 1, 1);
}
